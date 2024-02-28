#include "formheadache.h"
#include "cheadline.h"
#include "cboundary.h"
#include "cabcurve.h"
#include "cvehicle.h"
#include "glm.h"
#include "qmlutil.h"
#include "ctrack.h"
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include "glutils.h"
#include "aogproperty.h"
#include "aogrenderer.h"
#include <QTime>

//here for now.  Put in another module for use in other places.

void CalculateHeadings(QVector<Vec3> &xList);
void MakePointMinimumSpacing(QVector<Vec3> &xList, double minDistance);
int GetLineIntersection(double p0x, double p0y, double p1x, double p1y,
                        double p2x, double p2y, double p3x, double p3y,
                        double &iEast,
                        double &iNorth);

FormHeadache::FormHeadache(QObject *parent)
    : QObject{parent}
{}

void FormHeadache::connect_ui(QObject *headache_designer_instance) {
    this->headache_designer_instance = headache_designer_instance;

    InterfaceProperty<HeadacheDesigner,int>::set_qml_root(headache_designer_instance);
    InterfaceProperty<HeadacheDesigner,bool>::set_qml_root(headache_designer_instance);
    InterfaceProperty<HeadacheDesigner,double>::set_qml_root(headache_designer_instance);
    InterfaceProperty<HeadacheDesigner,QColor>::set_qml_root(headache_designer_instance);
    InterfaceProperty<HeadacheDesigner,QPoint>::set_qml_root(headache_designer_instance);

    //connect UI signals
    connect(headache_designer_instance,SIGNAL(load()),this,SLOT(load_headline()));
    connect(headache_designer_instance,SIGNAL(update_lines()),this,SLOT(update_lines()));
    connect(headache_designer_instance,SIGNAL(mouseClicked(int, int)),this,SLOT(clicked(int,int)));
    connect(headache_designer_instance,SIGNAL(cycleForward()),this,SLOT(btnCycleForward_Click()));
    connect(headache_designer_instance,SIGNAL(cycleBackward()),this,SLOT(btnCycleBackward_Click()));
    connect(headache_designer_instance,SIGNAL(deleteCurve()),this,SLOT(btnDeleteCurve_Click()));

    connect(headache_designer_instance,SIGNAL(deleteHeadland()), this, SLOT(btnDeleteHeadland_Click()));
    connect(headache_designer_instance,SIGNAL(createHeadland()),this,SLOT(btnBndLoop_Click()));
    //connect(headache_designer_instance,SIGNAL(deletePoints()),this,SLOT(btnDeletePoints_Click()));
    connect(headache_designer_instance,SIGNAL(ashrink()),this,SLOT(btnAShrink_Click()));
    connect(headache_designer_instance,SIGNAL(bshrink()),this,SLOT(btnBShrink_Click()));
    connect(headache_designer_instance,SIGNAL(alength()),this,SLOT(btnALength_Click()));
    connect(headache_designer_instance,SIGNAL(blength()),this,SLOT(btnBLength_Click()));
    connect(headache_designer_instance,SIGNAL(headlandOff()),this,SLOT(btnHeadlandOff_Click()));
    connect(headache_designer_instance,SIGNAL(cancelTouch()),this,SLOT(btnCancelTouch_Click()));


    connect(headache_designer_instance,SIGNAL(isSectionControlled(bool)),this,SLOT(isSectionControlled(bool)));
    connect(headache_designer_instance,SIGNAL(save_exit()),this,SLOT(btnExit_Click()));
}

void FormHeadache::setFieldInfo(double maxFieldDistance, double fieldCenterX, double fieldCenterY) {
    this->maxFieldDistance = maxFieldDistance;
    this->fieldCenterX = fieldCenterX;
    this->fieldCenterY = fieldCenterY;
}

QVector3D FormHeadache::mouseClickToField(int mouseX, int mouseY) {
    /* returns the field easting and northing position of a
     * mouse click
     */

    QMatrix4x4 modelview;
    QMatrix4x4 projection;

    int width = qmlItem(headache_designer_instance, "headacheRenderer")->property("width").toReal();
    int height = qmlItem(headache_designer_instance, "headacheRenderer")->property("height").toReal();

    projection.setToIdentity();

    //to shift, translate projection here. -1,0,0 is far left, 1,0,0 is far right.
    //58 degrees view
    //projection.viewport(0,0,width,height);
    projection.perspective(58, 1.0f, 1.0f, 20000);

    modelview.setToIdentity();
    //back the camera up
    modelview.translate(0, 0, -(double)maxFieldDistance * (double)zoom);

    //translate to that spot in the world
    modelview.translate(-(double)fieldCenterX + (double)sX * (double)maxFieldDistance,
                        -(double)fieldCenterY + (double)sY * (double)maxFieldDistance,
                        0);

    float x,y;
    x = mouseX;
    y = height - mouseY;
    //y = mouseY;

    //get point on the near plane
    QVector3D worldpoint_near = QVector3D( { x, y, 0} ).unproject(modelview,projection,QRect(0,0,width, height));
    //get point on the far plane
    QVector3D worldpoint_far = QVector3D( { x, y, 1} ).unproject(modelview, projection,QRect(0,0,width, height));
    //get direction vector from near to far
    QVector3D direction = worldpoint_far - worldpoint_near;
    //determine intercept with z=0 plane, and calculate easting and northing
    double lambda = -(worldpoint_near.z()) / direction.z();

    float mouseEasting = worldpoint_near.x() + lambda * direction.x();
    float mouseNorthing = worldpoint_near.y() + lambda * direction.y();

    QVector3D fieldCoord = QVector3D( { (float)mouseEasting, (float)mouseNorthing, 0 } );
    return fieldCoord;
}

void FormHeadache::load_headline() {
    hdl->idx = -1;
    headache_designer_instance->setProperty("showa", false);
    headache_designer_instance->setProperty("showb", false);

    emit saveHeadlines();
    update_lines();
    update_headland();
}

void FormHeadache::setup_matrices(QMatrix4x4 &modelview, QMatrix4x4 &projection) {
    projection.setToIdentity();

    //to shift, translate projection here. -1,0,0 is far left, 1,0,0 is far right.
    //58 degrees view
    projection.perspective(58, 1.0f, 1.0f, 20000);

    modelview.setToIdentity();
    //back the camera up
    modelview.translate(0, 0, -(double)maxFieldDistance * (double)zoom);

    //translate to that spot in the world
    modelview.translate(-(double)fieldCenterX + (double)sX * (double)maxFieldDistance,
                        -(double)fieldCenterY + (double)sY * (double)maxFieldDistance,
                        0);
}


void FormHeadache::update_lines() {
    QVariantList lines;
    QVariantMap linemap;
    QVariantList linepoints;
    QPoint linepoint;
    QColor color;
    QMatrix4x4 modelview;
    QMatrix4x4 projection;

    QVector3D s;
    QVector3D p;

    setup_matrices(modelview, projection);

    int width = qmlItem(headache_designer_instance, "headacheRenderer")->property("width").toReal();
    int height = qmlItem(headache_designer_instance, "headacheRenderer")->property("height").toReal();

    for (int j = 0; j < bnd->bndList.count(); j++)
    {
        if (j == bndSelect)
            color = QColor::fromRgbF(0.75f, 0.75f, 0.750f);
        else
            color = QColor::fromRgbF(0.0f, 0.25f, 0.10f);

        linemap.clear();
        linemap["index"] = j;
        linemap["color"] = color;
        linemap["width"] = 4;

        linepoints.clear();
        for (int i = 0; i < bnd->bndList[j].fenceLine.count(); i++)
        {
            p = QVector3D (bnd->bndList[j].fenceLine[i].easting,
                           bnd->bndList[j].fenceLine[i].northing,
                           0);
            s = p.project(modelview, projection, QRect(0,0,width,height));
            linepoint = QPoint(s.x(),height - s.y());
            linepoints.append(linepoint);
        }
        linemap["points"] = linepoints;

        lines.append(linemap);
    }
    headache_designer_instance->setProperty("boundaryLines", lines);

    if (isLinesVisible && hdl->tracksArr.count() > 0)
    {
        //GL.Enable(EnableCap.LineStipple);
        //GL.LineStipple(1, 0x7070);
        //GL.PointSize(3);

        lines.clear();
        linemap.clear();

        for (int i = 0; i < hdl->tracksArr.count(); i++)
        {
            if (hdl->tracksArr[i].mode == (int)TrackMode::AB)
            {
                color = QColor::fromRgbF(0.973f, 0.9f, 0.10f);
            }
            else
            {
                color = QColor::fromRgbF(0.3f, 0.99f, 0.20f);
            }
            linemap["color"] = color;

            linepoints.clear();
            for (Vec3 item: hdl->tracksArr[i].trackPts)
            {
                p = QVector3D(item.easting, item.northing, 0);
                s = p.project(modelview, projection, QRect(0,0,width,height));
                linepoint = QPoint(s.x(), height - s.y());
                linepoints.append(linepoint);
            }
            linemap["points"] = linepoints;
            linemap["dashed"] = true;
            linemap["width"] = 3;
            linemap["index"] = 0; //unused
            lines.append(linemap);
        }


        //GL.Disable(EnableCap.LineStipple);

        if (hdl->idx > -1)
        {
            linemap.clear();
            linemap["width"] = 6;
            //GL.LineWidth(6);
            linemap["color"] = QColor::fromRgbF(1.0f, 0.0f, 1.0f);
            linemap["dashed"] = false;

            //GL.Begin(PrimitiveType.LineStrip);
            for (Vec3 item: hdl->tracksArr[hdl->idx].trackPts)
            {
                p = QVector3D(item.easting, item.northing, 0);
                s = p.project(modelview, projection, QRect(0,0,width,height));
                linepoint = QPoint(s.x(), height - s.y());
                linepoints.append(linepoint);
            }
            linemap["points"] = linepoints;
            lines.append(linemap);

            int cnt = hdl->tracksArr[hdl->idx].trackPts.count() - 1;

            apoint = QPoint(hdl->tracksArr[hdl->idx].trackPts[0].easting, hdl->tracksArr[hdl->idx].trackPts[0].northing);
            showa = true;
            bpoint = QPoint(hdl->tracksArr[hdl->idx].trackPts[cnt].easting, hdl->tracksArr[hdl->idx].trackPts[cnt].northing);
            showb = true;
        }

        headache_designer_instance->setProperty("headacheLines", lines);
    }

    update_ab();

    update_headland();
}

void FormHeadache::update_ab() {
    QMatrix4x4 modelview, projection;
    QVector3D p, s;

    setup_matrices(modelview, projection);

    int width = qmlItem(headache_designer_instance, "headacheRenderer")->property("width").toReal();
    int height = qmlItem(headache_designer_instance, "headacheRenderer")->property("height").toReal();

    //draw A and B points
    if (start != 99999) {
        p = QVector3D(bnd->bndList[bndSelect].fenceLine[start].easting, bnd->bndList[bndSelect].fenceLine[start].northing, 0);
        s = p.project(modelview, projection, QRect(0,0,width,height));
        showa = true;
        apoint = QPoint(s.x(), height - s.y());
    } //else {
        //showa = false;
    //}
    if (end != 99999) {
        p = QVector3D(bnd->bndList[bndSelect].fenceLine[end].easting, bnd->bndList[bndSelect].fenceLine[end].northing, 0);
        s = p.project(modelview, projection, QRect(0,0,width,height));
        bpoint = QPoint(s.x(), height - s.y());
        showb = true;
    }

}

void FormHeadache::update_headland() {
    QMatrix4x4 modelview, projection;
    QVector3D p, s;

    QVariantList line;
    QPoint linepoint;

    setup_matrices(modelview, projection);

    int width = qmlItem(headache_designer_instance, "headacheRenderer")->property("width").toReal();
    int height = qmlItem(headache_designer_instance, "headacheRenderer")->property("height").toReal();

     //draw headland line if exists
    if (bnd->bndList[0].hdLine.count()) {
        //color is set in QML

        for (int i = 0; i < bnd->bndList[0].hdLine.count(); i++)
        {
            p = QVector3D (bnd->bndList[0].hdLine[i].easting,
                           bnd->bndList[0].hdLine[i].northing,
                           0);
            s = p.project(modelview, projection, QRect(0,0,width,height));

            linepoint = QPoint(s.x(),height - s.y());
            line.append(linepoint);
        }
    }
    headache_designer_instance->setProperty("headlandLine", line);
}

void FormHeadache::FormHeadLine_FormClosing()
{
    //hdl
    if (hdl->idx == -1)
    {
        isBtnAutoSteerOn = false;
        isYouTurnBtnOn = false;
    }

    emit saveHeadlines();

    if (hdl->tracksArr.count() > 0)
    {
        hdl->idx = 0;
    }
    else hdl->idx = -1;
}

void FormHeadache::btnCycleBackward_Click()
{
    bnd->bndList[0].hdLine.clear();

    if (hdl->tracksArr.count() > 0)
    {
        hdl->idx++;
        if (hdl->idx > (hdl->tracksArr.count() - 1)) hdl->idx = 0;
    }
    else
    {
        hdl->idx = -1;
    }
}

void FormHeadache::btnCycleForward_Click()
{
    bnd->bndList[0].hdLine.clear();

    if (hdl->tracksArr.count() > 0)
    {
        hdl->idx--;
        if (hdl->idx < 0) hdl->idx = (hdl->tracksArr.count() - 1);
    }
    else
    {
        hdl->idx = -1;
    }
}

void FormHeadache::clicked(int mouseX, int mouseY) {
    if ((double)lineDistance == 0 && (bool)curveLine) {
        timedMessageBox(3000, tr("Distance Error"), tr("Distance Set to 0, Nothing to Move"));
        return;
    }

    QVector3D fieldCoords = mouseClickToField(mouseX, mouseY);

    pint.easting = fieldCoords.x();
    pint.northing = fieldCoords.y();

    bnd->bndList[0].hdLine.clear();
    hdl->idx = -1;
    headache_designer_instance->setProperty("showa", false);
    headache_designer_instance->setProperty("showb", false);

    if (isA)
    {
        double minDistA = glm::DOUBLE_MAX;
        start = 99999; end = 99999;

        for (int j = 0; j < bnd->bndList.count(); j++)
        {
            for (int i = 0; i < bnd->bndList[j].fenceLine.count(); i++)
            {
                double dist = ((pint.easting - bnd->bndList[j].fenceLine[i].easting) * (pint.easting - bnd->bndList[j].fenceLine[i].easting))
                              + ((pint.northing - bnd->bndList[j].fenceLine[i].northing) * (pint.northing - bnd->bndList[j].fenceLine[i].northing));
                if (dist < minDistA)
                {
                    minDistA = dist;
                    bndSelect = j;
                    start = i;
                }
            }
        }

        isA = false;
    }
    else
    {
        double minDistA = glm::DOUBLE_MAX;
        int j = bndSelect;

        for (int i = 0; i < bnd->bndList[j].fenceLine.count(); i++)
        {
            double dist = ((pint.easting - bnd->bndList[j].fenceLine[i].easting) * (pint.easting - bnd->bndList[j].fenceLine[i].easting))
                          + ((pint.northing - bnd->bndList[j].fenceLine[i].northing) * (pint.northing - bnd->bndList[j].fenceLine[i].northing));
            if (dist < minDistA)
            {
                minDistA = dist;
                end = i;
            }
        }

        isA = true;

        if (start == end)
        {
            start = 99999; end = 99999;
            emit timedMessageBox(3000, tr("Line Error"), tr("Start Point = End Point"));
            return;
        }

        //build the lines
        if ((bool)curveLine)
        {
            hdl->tracksArr.append(CHeadPath());
            hdl->idx = hdl->tracksArr.count() - 1;

            bool isLoop = false;
            int limit = end;

            if ((abs(start - end)) > (bnd->bndList[bndSelect].fenceLine.count() * 0.5))
            {
                if (start < end)
                {
                    int tmp = start;
                    start = end;
                    end = tmp;
                }

                isLoop = true;
                if (start < end)
                {
                    limit = end;
                    end = 0;
                }
                else
                {
                    limit = end;
                    end = bnd->bndList[bndSelect].fenceLine.count();
                }
            }
            else
            {
                if (start > end)
                {
                    int tmp = start;
                    start = end;
                    end = tmp;
                }
            }

            hdl->tracksArr[hdl->idx].a_point = start;
            hdl->tracksArr[hdl->idx].trackPts.clear();

            Vec3 pt3;

            if (start < end)
            {
                for (int i = start; i <= end; i++)
                {
                    //calculate the point inside the boundary
                    pt3 = bnd->bndList[bndSelect].fenceLine[i];
                    hdl->tracksArr[hdl->idx].trackPts.append(pt3);

                    if (isLoop && i == bnd->bndList[bndSelect].fenceLine.count() - 1)
                    {
                        i = -1;
                        isLoop = false;
                        end = limit;
                    }
                }
            }
            else
            {
                for (int i = start; i >= end; i--)
                {
                    //calculate the point inside the boundary
                    pt3 = bnd->bndList[bndSelect].fenceLine[i];
                    hdl->tracksArr[hdl->idx].trackPts.append(pt3);

                    if (isLoop && i == 0)
                    {
                        i = bnd->bndList[bndSelect].fenceLine.count() - 1;
                        isLoop = false;
                        end = limit;
                    }
                }
            }

            //who knows which way it actually goes
            CalculateHeadings(hdl->tracksArr[hdl->idx].trackPts);

            int ptCnt = hdl->tracksArr[hdl->idx].trackPts.count() - 1;

            for (int i = 1; i < 30; i++)
            {
                Vec3 pt(hdl->tracksArr[hdl->idx].trackPts[ptCnt]);
                pt.easting += (sin(pt.heading) * i);
                pt.northing += (cos(pt.heading) * i);
                hdl->tracksArr[hdl->idx].trackPts.append(pt);
            }

            Vec3 stat(hdl->tracksArr[hdl->idx].trackPts[0]);

            for (int i = 1; i < 30; i++)
            {
                Vec3 pt(stat);
                pt.easting -= (sin(pt.heading) * i);
                pt.northing -= (cos(pt.heading) * i);
                hdl->tracksArr[hdl->idx].trackPts.insert(0, pt);
            }

            //create a name
            hdl->tracksArr[hdl->idx].name = QString("%1 Cu %2").arg(hdl->idx).arg(QTime::currentTime().toString("mm:ss"));

            hdl->tracksArr[hdl->idx].moveDistance = 0;

            hdl->tracksArr[hdl->idx].mode = (int)TrackMode::Curve;

            emit saveHeadlines();

            //update arrays
            start = 99999; end = 99999;

            //FixLabelsCurve();
        }
        else //straight line
        {
            if ((abs(start - end)) > (bnd->bndList[bndSelect].fenceLine.count() * 0.5))
            {
                if (start < end)
                {
                    int tmp = start;
                    start = end;
                    end = tmp;
                }
            }
            else
            {
                if (start > end)
                {
                    int tmp = start;
                    start = end;
                    end = tmp;
                }
            }

            Vec3 ptA(bnd->bndList[bndSelect].fenceLine[start]);
            Vec3 ptB(bnd->bndList[bndSelect].fenceLine[end]);

            //calculate the AB Heading
            double abHead = atan2(
                bnd->bndList[bndSelect].fenceLine[end].easting - bnd->bndList[bndSelect].fenceLine[start].easting,
                bnd->bndList[bndSelect].fenceLine[end].northing - bnd->bndList[bndSelect].fenceLine[start].northing);
            if (abHead < 0) abHead += glm::twoPI;

            if (hdl->idx < hdl->tracksArr.count() - 1)
            {
                hdl->idx++;
                hdl->tracksArr.insert(hdl->idx, CHeadPath());
            }
            else
            {
                hdl->tracksArr.append(CHeadPath());
                hdl->idx = hdl->tracksArr.count() - 1;
            }

            hdl->tracksArr[hdl->idx].a_point = start;
            hdl->tracksArr[hdl->idx].trackPts.clear();

            ptA.heading = abHead;
            ptB.heading = abHead;

            for (int i = 0; i <= (int)(glm::Distance(ptA, ptB)); i++)
            {
                Vec3 ptC(
                        (sin(abHead) * i) + ptA.easting,
                        (cos(abHead) * i) + ptA.northing,
                        abHead
                    );
                hdl->tracksArr[hdl->idx].trackPts.append(ptC);
            }

            int ptCnt = hdl->tracksArr[hdl->idx].trackPts.count() - 1;

            for (int i = 1; i < 30; i++)
            {
                Vec3 pt(hdl->tracksArr[hdl->idx].trackPts[ptCnt]);
                pt.easting += (sin(pt.heading) * i);
                pt.northing += (cos(pt.heading) * i);
                hdl->tracksArr[hdl->idx].trackPts.append(pt);
            }

            Vec3 stat(hdl->tracksArr[hdl->idx].trackPts[0]);

            for (int i = 1; i < 30; i++)
            {
                Vec3 pt(stat);
                pt.easting -= (sin(pt.heading) * i);
                pt.northing -= (cos(pt.heading) * i);
                hdl->tracksArr[hdl->idx].trackPts.insert(0, pt);
            }

            //create a name
            hdl->tracksArr[hdl->idx].name = QString("%1 AB %2").arg(hdl->idx).arg(QTime::currentTime().toString("mm:ss"));

            hdl->tracksArr[hdl->idx].moveDistance = 0;

            hdl->tracksArr[hdl->idx].mode = (int)TrackMode::AB;

            emit saveHeadlines();

            start = 99999; end = 99999;
        }

        //bnd->bndList[0].hdLine.clear();
        hdl->desList.clear();

        if (hdl->tracksArr.count() < 1 || hdl->idx == -1) return;

        double distAway = (double)lineDistance;
        hdl->tracksArr[hdl->idx].moveDistance += distAway;

        double distSqAway = (distAway * distAway) - 0.01;
        Vec3 point;

        int refCount = hdl->tracksArr[hdl->idx].trackPts.count();
        for (int i = 0; i < refCount; i++)
        {
            point = Vec3(
                hdl->tracksArr[hdl->idx].trackPts[i].easting - (sin(glm::PIBy2 + hdl->tracksArr[hdl->idx].trackPts[i].heading) * distAway),
                hdl->tracksArr[hdl->idx].trackPts[i].northing - (cos(glm::PIBy2 + hdl->tracksArr[hdl->idx].trackPts[i].heading) * distAway),
                hdl->tracksArr[hdl->idx].trackPts[i].heading);
            bool Add = true;

            for (int t = 0; t < refCount; t++)
            {
                double dist = ((point.easting - hdl->tracksArr[hdl->idx].trackPts[t].easting) * (point.easting - hdl->tracksArr[hdl->idx].trackPts[t].easting))
                              + ((point.northing - hdl->tracksArr[hdl->idx].trackPts[t].northing) * (point.northing - hdl->tracksArr[hdl->idx].trackPts[t].northing));
                if (dist < distSqAway)
                {
                    Add = false;
                    break;
                }
            }

            if (Add)
            {
                if (hdl->desList.count() > 0)
                {
                    double dist = ((point.easting - hdl->desList[hdl->desList.count() - 1].easting) * (point.easting - hdl->desList[hdl->desList.count() - 1].easting))
                                  + ((point.northing - hdl->desList[hdl->desList.count() - 1].northing) * (point.northing - hdl->desList[hdl->desList.count() - 1].northing));
                    if (dist > 1)
                        hdl->desList.append(point);
                }
                else hdl->desList.append(point);
            }
        }

        hdl->tracksArr[hdl->idx].trackPts.clear();

        for (int i = 0; i < hdl->desList.count(); i++)
        {
            hdl->tracksArr[hdl->idx].trackPts.append(Vec3(hdl->desList[i]));
        }

        hdl->desList.clear();
    }
}

void FormHeadache::btnDeleteCurve_Click()
{
    //bnd->bndList[0].hdLine.clear();

    if (hdl->tracksArr.count() > 0 && hdl->idx > -1)
    {
        hdl->tracksArr.remove(hdl->idx);
        hdl->idx--;
    }

    if (hdl->tracksArr.count() > 0)
    {
        if (hdl->idx == -1)
        {
            hdl->idx++;
        }
    }
    else hdl->idx = -1;

}

void FormHeadache::btnExit_Click() {
    emit saveHeadlines();
}

void FormHeadache::isSectionControlled(bool wellIsIt) {
    bnd->isSectionControlledByHeadland = wellIsIt;
    property_setHeadland_isSectionControlled = wellIsIt;
}

void FormHeadache::btnBndLoop_Click() {
    //sort the lines using CHeadPath::operator<
    std::sort(hdl->tracksArr.begin(), hdl->tracksArr.end());
    emit saveHeadlines();

    hdl->idx = -1;

    //build the headland
    bnd->bndList[0].hdLine.clear();

    //int numOfLines = hdl->tracksArr.count();
    int nextLine = 0;
    crossings.clear();

    int isStart = 0;

    for (int lineNum = 0; lineNum < hdl->tracksArr.count(); lineNum++)
    {
        nextLine = lineNum - 1;
        if (nextLine < 0) nextLine = hdl->tracksArr.count() - 1;

        if (nextLine == lineNum)
        {
            emit timedMessageBox(2000, tr("Create Error"), tr("Is there maybe only one line?"));
            return;
        }

        for (int i = 0; i < hdl->tracksArr[lineNum].trackPts.count() - 2; i++)
        {
            for (int k = 0; k < hdl->tracksArr[nextLine].trackPts.count() - 2; k++)
            {
                int res = GetLineIntersection(
                    hdl->tracksArr[lineNum].trackPts[i].easting,
                    hdl->tracksArr[lineNum].trackPts[i].northing,
                    hdl->tracksArr[lineNum].trackPts[i + 1].easting,
                    hdl->tracksArr[lineNum].trackPts[i + 1].northing,

                    hdl->tracksArr[nextLine].trackPts[k].easting,
                    hdl->tracksArr[nextLine].trackPts[k].northing,
                    hdl->tracksArr[nextLine].trackPts[k + 1].easting,
                    hdl->tracksArr[nextLine].trackPts[k + 1].northing,
                    iE, iN);
                if (res == 1)
                {
                    if (isStart == 0) i++;
                    crossings.append(i);
                    isStart++;
                    if (isStart == 2) goto again;
                    nextLine = lineNum + 1;

                    if (nextLine > hdl->tracksArr.count() - 1) nextLine = 0;
                }
            }
        }

    again:
        isStart = 0;
    }

    if (crossings.count() != hdl->tracksArr.count() * 2)
    {
        emit timedMessageBox(2000, tr("Crosings Error"), tr("Make sure all ends cross only once"));
        bnd->bndList[0].hdLine.clear();
        return;
    }

    for (int i = 0; i < hdl->tracksArr.count(); i++)
    {
        int low = crossings[i * 2];
        int high = crossings[i * 2 + 1];
        for (int k = low; k < high; k++)
        {
            bnd->bndList[0].hdLine.append(hdl->tracksArr[i].trackPts[k]);
        }
    }

    //for (int i = 0; i < hdl->tracksArr.count(); i++)
    //{
    //    hdl->desList.clear();

    //    int low = crossings[i * 2];
    //    int high = crossings[i * 2 + 1];
    //    for (int k = low; k < high; k++)
    //    {
    //        hdl->desList.append(hdl->tracksArr[i].trackPts[k]);
    //    }

    //    hdl->tracksArr[i].trackPts.clear();

    //    foreach (var item in hdl->desList)
    //    {
    //        hdl->tracksArr[i].trackPts.append(Vec3(item));
    //    }
    //}

    QVector<Vec3> hdArr;

    if (bnd->bndList[0].hdLine.count() > 0)
    {
        hdArr = bnd->bndList[0].hdLine;
    }
    else
    {
        bnd->bndList[0].hdLine.clear();
        return;
    }

    //middle points
    for (int i = 1; i < hdArr.count(); i++)
    {
        hdArr[i - 1].heading = atan2(hdArr[i - 1].easting - hdArr[i].easting, hdArr[i - 1].northing - hdArr[i].northing);
        if (hdArr[i].heading < 0) hdArr[i].heading += glm::twoPI;
    }

    double delta = 0;
    for (int i = 0; i < hdArr.count(); i++)
    {
        if (i == 0)
        {
            bnd->bndList[0].hdLine.append(Vec3(hdArr[i].easting, hdArr[i].northing, hdArr[i].heading));
            continue;
        }
        delta += (hdArr[i - 1].heading - hdArr[i].heading);

        if (fabs(delta) > 0.005)
        {
            Vec3 pt(hdArr[i].easting, hdArr[i].northing, hdArr[i].heading);

            bnd->bndList[0].hdLine.append(pt);
            delta = 0;
        }
    }
    emit saveHeadland();
}

void FormHeadache::btnSlice_Click() {
    int startBnd = 0, endBnd = 0, startLine = 0, endLine = 0;
    int isStart = 0;

    if (sliceArr.count() == 0) return;

    //save a backup
    backupList.clear();
    for (auto item: bnd->bndList[0].hdLine)
    {
        backupList.append(item);
    }

    for (int i = 0; i < sliceArr.count() - 2; i++)
    {
        for (int k = 0; k < bnd->bndList[0].hdLine.count() - 2; k++)
        {
            int res = GetLineIntersection(
                sliceArr[i].easting,
                sliceArr[i].northing,
                sliceArr[i + 1].easting,
                sliceArr[i + 1].northing,

                bnd->bndList[0].hdLine[k].easting,
                bnd->bndList[0].hdLine[k].northing,
                bnd->bndList[0].hdLine[k + 1].easting,
                bnd->bndList[0].hdLine[k + 1].northing,
                iE, iN);
            if (res == 1)
            {
                if (isStart == 0)
                {
                    startBnd = k + 1;
                    startLine = i + 1;
                }
                else
                {
                    endBnd = k + 1;
                    endLine = i;
                }
                isStart++;
            }
        }
    }

    if (isStart < 2)
    {
        emit timedMessageBox(2000, tr("Error"), tr("Crossings not Found"));
        return;
    }

    //overlaps start finish
    if ((fabs(startBnd - endBnd)) > (bnd->bndList[bndSelect].fenceLine.count() * 0.5))
    {
        if (startBnd < endBnd)
        {
            int tmp = startBnd;
            startBnd = endBnd;
            endBnd = tmp;
        }

        hdl->desList.clear();

        //first bnd segment
        for (int i = endBnd; i < startBnd; i++)
        {
            hdl->desList.append(bnd->bndList[0].hdLine[i]);
        }

        for (int i = startLine; i < endLine; i++)
        {
            hdl->desList.append(sliceArr[i]);
        }

        //build headline from desList
        bnd->bndList[0].hdLine.clear();

        for (auto item: hdl->desList)
        {
            bnd->bndList[0].hdLine.append(item);
        }
    }
    // completely in between start finish
    else
    {
        if (startBnd > endBnd)
        {
            int tmp = startBnd;
            startBnd = endBnd;
            endBnd = tmp;
        }

        hdl->desList.clear();

        //first bnd segment
        for (int i = 0; i < startBnd; i++)
        {
            hdl->desList.append(bnd->bndList[0].hdLine[i]);
        }

        //line segment
        for (int i = startLine; i < endLine; i++)
        {
            hdl->desList.append(sliceArr[i]);
        }

        //final bnd segment
        for (int i = endBnd; i < bnd->bndList[0].hdLine.count(); i++)
        {
            hdl->desList.append(bnd->bndList[0].hdLine[i]);
        }

        //build headline from desList
        bnd->bndList[0].hdLine.clear();

        for (auto item: hdl->desList)
        {
            bnd->bndList[0].hdLine.append(item);
        }
    }

    hdl->desList.clear();
    update_headland();
}

void FormHeadache::btnDeleteHeadland_Click()
{
    start = 99999; end = 99999;
    isA = true;
    hdl->desList.clear();
    bnd->bndList[0].hdLine.clear();
    update_ab();
    update_headland();
}

/*
void FormHeadache::btnDeletePoints_Click() {
    start = 99999; end = 99999;
    isA = true;
    update_ab();
}
*/

void FormHeadache::btnALength_Click() {
    if (hdl->idx > -1)
    {
        //and the beginning
        Vec3 start = hdl->tracksArr[hdl->idx].trackPts[0];
        for (int i = 1; i < 10; i++)
        {
            Vec3 pt = start;
            pt.easting -= (sin(pt.heading) * i);
            pt.northing -= (cos(pt.heading) * i);
            hdl->tracksArr[hdl->idx].trackPts.insert(0, pt);
        }
    }
}

void FormHeadache::btnBLength_Click()
{
    if (hdl->idx > -1)
    {
        int ptCnt = hdl->tracksArr[hdl->idx].trackPts.count() - 1;

        for (int i = 1; i < 10; i++)
        {
            Vec3 pt(hdl->tracksArr[hdl->idx].trackPts[ptCnt]);
            pt.easting += (sin(pt.heading) * i);
            pt.northing += (cos(pt.heading) * i);
            hdl->tracksArr[hdl->idx].trackPts.append(pt);
        }
    }
}

void FormHeadache::btnBShrink_Click()
{
    if (hdl->idx > -1)
    {
        if (hdl->tracksArr[hdl->idx].trackPts.count() > 8)
            hdl->tracksArr[hdl->idx].trackPts.remove(hdl->tracksArr[hdl->idx].trackPts.count()-5, 5);
    }
}

void FormHeadache::btnAShrink_Click()
{
    if (hdl->idx > -1)
    {
        if (hdl->tracksArr[hdl->idx].trackPts.count() > 8)
            hdl->tracksArr[hdl->idx].trackPts.remove(0, 5);
    }
}

void FormHeadache::btnHeadlandOff_Click()
{
    bnd->bndList[0].hdLine.clear();
    update_headland();
    emit saveHeadlines();
    bnd->isHeadlandOn = false;
    vehicle->isHydLiftOn = false;
    update_ab();
    update_headland();
}

void FormHeadache::btnCancelTouch_Click()
{
    //update the arrays
    start = 99999; end = 99999;
    isA = true;
    //FixLabelsCurve();
    //TODO: why is this here?
    //curve->desList.clear();
}
