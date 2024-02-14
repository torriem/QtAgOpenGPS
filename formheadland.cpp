#include "formheadland.h"
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

//here for now.  Put in another module for use in other places.

void CalculateHeadings(QVector<Vec3> &xList)
{
    //to calc heading based on next and previous points to give an average heading.
    int cnt = xList.count();
    if (cnt > 3)
    {
        QVector<Vec3> arr = xList; //copy list
        cnt--;
        xList.clear();

        Vec3 pt3 = arr[0];
        pt3.heading = atan2(arr[1].easting - arr[0].easting, arr[1].northing - arr[0].northing);
        if (pt3.heading < 0) pt3.heading += glm::twoPI;
        xList.append(pt3);

        //middle points
        for (int i = 1; i < cnt; i++)
        {
            pt3 = arr[i];
            pt3.heading = atan2(arr[i + 1].easting - arr[i - 1].easting, arr[i + 1].northing - arr[i - 1].northing);
            if (pt3.heading < 0) pt3.heading += glm::twoPI;
            xList.append(pt3);
        }

        pt3 = arr[arr.count() - 1];
        pt3.heading = atan2(arr[arr.count() - 1].easting - arr[arr.count() - 2].easting,
                                 arr[arr.count() - 1].northing - arr[arr.count() - 2].northing);
        if (pt3.heading < 0) pt3.heading += glm::twoPI;
        xList.append(pt3);
    }
}

void MakePointMinimumSpacing(QVector<Vec3> &xList, double minDistance)
{
    int cnt = xList.count();
    if (cnt > 3)
    {
        //make sure point distance isn't too big
        for (int i = 0; i < cnt - 1; i++)
        {
            int j = i + 1;
            //if (j == cnt) j = 0;
            double distance = glm::Distance(xList[i], xList[j]);
            if (distance > minDistance)
            {
                Vec3 pointB((xList[i].easting + xList[j].easting) / 2.0,
                                       (xList[i].northing + xList[j].northing) / 2.0,
                                       xList[i].heading);

                xList.insert(j, pointB);
                cnt = xList.count();
                i = -1;
            }
        }
    }
}

int GetLineIntersection(double p0x, double p0y, double p1x, double p1y,
                        double p2x, double p2y, double p3x, double p3y,
                        double &iEast,
                        double &iNorth)
{
    double s1x, s1y, s2x, s2y;
    s1x = p1x - p0x;
    s1y = p1y - p0y;

    s2x = p3x - p2x;
    s2y = p3y - p2y;

    double s, t;
    s = (-s1y * (p0x - p2x) + s1x * (p0y - p2y)) / (-s2x * s1y + s1x * s2y);

    if (s >= 0 && s <= 1)
    {
        //check oher side
        t = (s2x * (p0y - p2y) - s2y * (p0x - p2x)) / (-s2x * s1y + s1x * s2y);
        if (t >= 0 && t <= 1)
        {
            // Collision detected
            iEast = p0x + (t * s1x);
            iNorth = p0y + (t * s1y);
            return 1;
        }
    }

    return 0; // No collision
}

FormHeadland::FormHeadland(QObject *parent)
    : QObject{parent}
{}

void FormHeadland::connect_ui(QObject *headland_designer_instance) {
    this->headland_designer_instance = headland_designer_instance;

    InterfaceProperty<HeadlandDesigner,int>::set_qml_root(headland_designer_instance);
    InterfaceProperty<HeadlandDesigner,bool>::set_qml_root(headland_designer_instance);
    InterfaceProperty<HeadlandDesigner,double>::set_qml_root(headland_designer_instance);

    //set up OpenGL renderer
    QObject *renderer = qmlItem(headland_designer_instance,"headlandRenderer");
    renderer->setProperty("callbackObject", QVariant::fromValue((void *) this));
    renderer->setProperty("initCallback",QVariant::fromValue<std::function<void (void)>>(std::bind(&FormHeadland::ogl_initialize, this)));
    renderer->setProperty("paintCallback",QVariant::fromValue<std::function<void (void)>>(std::bind(&FormHeadland::ogl_paint, this)));
    renderer->setProperty("samples",settings->value("display/antiAliasSamples", 0));

    //connect UI signals
    connect(headland_designer_instance,SIGNAL(load()),this,SLOT(load_headline()));
    connect(headland_designer_instance,SIGNAL(mouseClicked(int, int)),this,SLOT(clicked(int,int)));
    connect(headland_designer_instance,SIGNAL(slice()),this,SLOT(btnSlice_Click()));
    connect(headland_designer_instance,SIGNAL(deletePoints()),this,SLOT(btnDeletePoints_Click()));
    connect(headland_designer_instance,SIGNAL(undo()),this,SLOT(btnUndo_Click()));
    connect(headland_designer_instance,SIGNAL(ashrink()),this,SLOT(btnAShrink_Click()));
    connect(headland_designer_instance,SIGNAL(bshrink()),this,SLOT(btnBShrink_Click()));
    connect(headland_designer_instance,SIGNAL(alength()),this,SLOT(btnALength_Click()));
    connect(headland_designer_instance,SIGNAL(blength()),this,SLOT(btnBLength_Click()));
    connect(headland_designer_instance,SIGNAL(headlandOff()),this,SLOT(btnHeadlandOff_Click()));
    connect(headland_designer_instance,SIGNAL(isSectionControlled(bool)),this,SLOT(isSectionControlled(bool)));
}

void FormHeadland::setFieldInfo(double maxFieldDistance, double fieldCenterX, double fieldCenterY) {
    this->maxFieldDistance = maxFieldDistance;
    this->fieldCenterX = fieldCenterX;
    this->fieldCenterY = fieldCenterY;
}

void FormHeadland::SetLineDistance() {
    hdl->desList.clear();

    if (sliceArr.count() < 1) return;

    double distAway = (double)lineDistance;

    double distSqAway = (distAway * distAway) - 0.01;
    Vec3 point;

    int refCount = sliceArr.count();
    for (int i = 0; i < refCount; i++)
    {
        Vec3 point(
            sliceArr[i].easting - (sin(glm::PIBy2 + sliceArr[i].heading) * distAway),
            sliceArr[i].northing - (cos(glm::PIBy2 + sliceArr[i].heading) * distAway),
            sliceArr[i].heading);
        bool Add = true;

        for (int t = 0; t < refCount; t++)
        {
            double dist = ((point.easting - sliceArr[t].easting) * (point.easting - sliceArr[t].easting))
                          + ((point.northing - sliceArr[t].northing) * (point.northing - sliceArr[t].northing));
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

    sliceArr.clear();

    for (int i = 0; i < hdl->desList.count(); i++)
    {
        sliceArr.append(Vec3(hdl->desList[i]));
    }

    hdl->desList.clear();
    sliceCount = sliceArr.count();
}

QVector3D FormHeadland::mouseClickToField(int mouseX, int mouseY) {
    /* returns the field easting and northing position of a
     * mouse click
     */

    QMatrix4x4 modelview;
    QMatrix4x4 projection;

    int width = qmlItem(headland_designer_instance, "headlandRenderer")->property("width").toReal();
    int height = qmlItem(headland_designer_instance, "headlandRenderer")->property("height").toReal();

    projection.setToIdentity();

    //to shift, translate projection here. -1,0,0 is far left, 1,0,0 is far right.
    //58 degrees view
    projection.viewport(0,0,width,height);
    projection.perspective(1.01f, 1.0f, 1.0f, 20000);

    modelview.setToIdentity();
    //back the camera up
    modelview.translate(0, 0, -maxFieldDistance * (double)zoom);

    //translate to that spot in the world
    modelview.translate(-fieldCenterX + (double)sX * maxFieldDistance, -fieldCenterY + (double)sY * maxFieldDistance, 0);

    float x,y;
    x = mouseX;
    y = height - mouseY;

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

void FormHeadland::load_headline() {
    hdl->idx = -1;

    start = 99999; end = 99999;
    isA = true;
    hdl->desList.clear();

    sliceArr.clear();
    backupList.clear();
    sliceCount = 0;
    backupCount = 0;

    if(bnd->bndList[0].hdLine.count() == 0)
    {
        bnd->bndList[0].hdLine.clear();

        if (bnd->bndList[0].fenceLine.count() > 0)
        {
            for (int i = 0; i < bnd->bndList[0].fenceLine.count(); i++)
            {
                bnd->bndList[0].hdLine.append(Vec3(bnd->bndList[0].fenceLine[i]));
            }
        }
    }
    else
    {
        //make sure point distance isn't too big
        MakePointMinimumSpacing(bnd->bndList[0].hdLine, 1.2);
        CalculateHeadings(bnd->bndList[0].hdLine);
    }
    sliceCount = sliceArr.count();
    backupCount = backupList.count();
}

void FormHeadland::clicked(int mouseX, int mouseY) {
    sliceArr.clear();

    QVector3D fieldCoords = mouseClickToField(mouseX, mouseY);

    pint.easting = fieldCoords.x();
    pint.northing = fieldCoords.y();

    zoomToggle = false;
    zoom = 1;
    sX = 0;
    sY = 0;

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

        //build the lines
        if ((bool)curveLine)
        {
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

            sliceArr.clear();
            Vec3 pt3;

            if (start < end)
            {
                for (int i = start; i <= end; i++)
                {
                    //calculate the point inside the boundary
                    pt3 = bnd->bndList[bndSelect].fenceLine[i];
                    sliceArr.append(pt3);

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
                    sliceArr.append(pt3);

                    if (isLoop && i == 0)
                    {
                        i = bnd->bndList[bndSelect].fenceLine.count() - 1;
                        isLoop = false;
                        end = limit;
                    }
                }
            }

            int ptCnt = sliceArr.count() - 1;

            if (ptCnt > 0)
            {
                //who knows which way it actually goes
                CalculateHeadings(sliceArr);

                for (int i = 1; i < 30; i++)
                {
                    Vec3 pt(sliceArr[ptCnt]);
                    pt.easting += (sin(pt.heading) * i);
                    pt.northing += (cos(pt.heading) * i);
                    sliceArr.append(pt);
                }

                Vec3 stat(sliceArr[0]);

                for (int i = 1; i < 30; i++)
                {
                    Vec3 pt(stat);
                    pt.easting -= (sin(pt.heading) * i);
                    pt.northing -= (cos(pt.heading) * i);
                    sliceArr.insert(0, pt);
                }

                mode = (int)TrackMode::Curve;
            }
            else
            {
                start = 99999; end = 99999;
                return;
            }

            //update the arrays
            start = 99999; end = 99999;
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

            sliceArr.clear();

            ptA.heading = abHead;
            ptB.heading = abHead;

            for (int i = 0; i <= (int)(glm::Distance(ptA, ptB)); i++)
            {
                Vec3 ptC(
                        (sin(abHead) * i) + ptA.easting,
                        (cos(abHead) * i) + ptA.northing,
                        abHead
                    );
                sliceArr.append(ptC);
            }

            int ptCnt = sliceArr.count() - 1;

            for (int i = 1; i < 30; i++)
            {
                Vec3 pt(sliceArr[ptCnt]);
                pt.easting += (sin(pt.heading) * i);
                pt.northing += (cos(pt.heading) * i);
                sliceArr.append(pt);
            }

            Vec3 stat(sliceArr[0]);

            for (int i = 1; i < 30; i++)
            {
                Vec3 pt(stat);
                pt.easting -= (sin(pt.heading) * i);
                pt.northing -= (cos(pt.heading) * i);
                sliceArr.insert(0, pt);
            }

            mode = (int)TrackMode::AB;

            start = 99999; end = 99999;
        }

        //Move the line
        if ((double)lineDistance != 0)
            SetLineDistance();

        //QML: btnSlice.Enabled = true;
    }

    sliceCount = sliceArr.count();
    backupCount = sliceArr.count();
}

void FormHeadland::ogl_initialize() {

}

void FormHeadland::ogl_paint() {
    QOpenGLContext *glContext = QOpenGLContext::currentContext();
    QOpenGLFunctions *gl = glContext->functions();

    QMatrix4x4 modelview;
    QMatrix4x4 projection;

    int width = qmlItem(headland_designer_instance, "headlandRenderer")->property("width").toReal();
    int height = qmlItem(headland_designer_instance, "headlandRenderer")->property("height").toReal();

    projection.setToIdentity();

    //to shift, translate projection here. -1,0,0 is far left, 1,0,0 is far right.
    //58 degrees view
    projection.viewport(0,0,width,height);
    projection.perspective(1.01f, 1.0f, 1.0f, 20000);

    modelview.setToIdentity();
    //back the camera up
    modelview.translate(0, 0, -maxFieldDistance * (double)zoom);

    //translate to that spot in the world
    modelview.translate(-fieldCenterX + (double)sX * maxFieldDistance, -fieldCenterY + (double)sY * maxFieldDistance, 0);

    gl->glViewport(0,0,width, height); //is this necessary?
    gl->glCullFace(GL_BACK);
    gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gl->glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    //draw all the boundaries
    double lineWidth = 4;

    GLHelperOneColor gldraw;
    QColor color;

    for (int j = 0; j < bnd->bndList.count(); j++)
    {
        if (j == bndSelect)
            color = QColor::fromRgbF(0.75f, 0.75f, 0.750f);
        else
            color = QColor::fromRgbF(0.0f, 0.25f, 0.10f);

        gldraw.clear();
        for (int i = 0; i < bnd->bndList[j].fenceLine.count(); i++)
        {
            gldraw.append( QVector3D (bnd->bndList[j].fenceLine[i].easting,
                           bnd->bndList[j].fenceLine[i].northing, 0) );
        }

        gldraw.draw(gl,projection * modelview, color, GL_LINE_STRIP,lineWidth);
    }

    //the vehicle
    gldraw.clear();
    gldraw.append( QVector3D ( vehicle->pivotAxlePos.easting, vehicle->pivotAxlePos.northing, 0.0 ) );
    color = QColor::fromRgbF(0.95f, 0.190f, 0.20f);
    gldraw.draw(gl, projection * modelview, color, GL_POINTS, 16.0f);

    if (start != 99999 || end != 99999) DrawABTouchLine(gl, projection * modelview);

    //draw the actual built lines
    DrawBuiltLines(gl, projection * modelview);
}

void FormHeadland::ogl_resize() {

}

void FormHeadland::DrawBuiltLines(QOpenGLFunctions *gl, QMatrix4x4 mvp) {
    //GL.LineWidth(8);
    //GL.Color3(0.03f, 0.0f, 0.150f);
    //GL.Begin(PrimitiveType.LineLoop);

    //for (int i = 0; i < mf.bnd.bndList[0].hdLine.Count; i++)
    //{
    //    GL.Vertex3(mf.bnd.bndList[0].hdLine[i].easting, mf.bnd.bndList[0].hdLine[i].northing, 0);
    //}
    //GL.End();

    GLHelperOneColor gldraw;
    QColor color;

    color = QColor::fromRgbF(0.943f, 0.9083f, 0.09150f);

    for (int i = 0; i < bnd->bndList[0].hdLine.count(); i++)
    {
        gldraw.append( QVector3D (bnd->bndList[0].hdLine[i].easting, bnd->bndList[0].hdLine[i].northing, 0 ));
    }

    gldraw.draw(gl, mvp, color, GL_LINE_LOOP, 8.0f);

    if (sliceArr.count() > 0)
    {
        //GL.Enable(EnableCap.LineStipple);
        //GL.LineStipple(1, 0x7070);

        if (mode == (int)TrackMode::AB)
        {
            color = QColor::fromRgbF(0.95f, 0.09f, 0.0f);
        }
        else
        {
            color = QColor::fromRgbF(0.13f, 0.95f, 0.020f);
        }

        gldraw.clear();
        for (Vec3 item: sliceArr)
        {
            gldraw.append( QVector3D (item.easting, item.northing, 0) );
        }
        gldraw.draw(gl,mvp, color, GL_LINE_STRIP, 8.0f);

        int cnt = sliceArr.count() - 1;
        gldraw.clear();
        color = QColor::fromRgbF(1.0f, 0.6f, 0.3f);
        gldraw.append( QVector3D( sliceArr[0].easting, sliceArr[0].northing, 0) );
        gldraw.draw(gl, mvp, color, GL_POINT, 24.0f);

        gldraw.clear();
        color = QColor::fromRgbF(0.5f, 0.73f, 0.99f);
        gldraw.append ( QVector3D( sliceArr[cnt].easting, sliceArr[cnt].northing, 0) );
        gldraw.draw(gl, mvp, color, GL_POINT, 24.0f);
    }
}

void FormHeadland::DrawABTouchLine(QOpenGLFunctions *gl, QMatrix4x4 mvp) {
    GLHelperOneColor gldraw;
    QColor color;

    color = QColor::fromRgbF(0, 0, 0);
    if (start != 99999) gldraw.append( QVector3D (bnd->bndList[bndSelect].fenceLine[start].easting, bnd->bndList[bndSelect].fenceLine[start].northing, 0) );
    if (end != 99999) gldraw.append( QVector3D (bnd->bndList[bndSelect].fenceLine[end].easting, bnd->bndList[bndSelect].fenceLine[end].northing, 0) );
    gldraw.draw(gl, mvp, color, GL_POINTS, 24.0f);

    gldraw.clear();
    color = QColor::fromRgbF(1.0f, 0.75f, 0.350f);
    if (start != 99999) gldraw.append( QVector3D ( bnd->bndList[bndSelect].fenceLine[start].easting, bnd->bndList[bndSelect].fenceLine[start].northing, 0) );
    gldraw.draw(gl,mvp, color, GL_POINT, 18.0f);

    gldraw.clear();
    color = QColor::fromRgbF(0.5f, 0.75f, 1.0f);
    if (end != 99999) gldraw.append( QVector3D ( bnd->bndList[bndSelect].fenceLine[end].easting, bnd->bndList[bndSelect].fenceLine[end].northing, 0) );
    gldraw.draw(gl, mvp, color, GL_POINT, 18.0f);
}

void FormHeadland::btn_Exit_Click() {
    QVector<Vec3> hdArr;

    if (bnd->bndList[0].hdLine.count() > 0)
    {
        hdArr = bnd->bndList[0].hdLine;
        bnd->bndList[0].hdLine.clear();

        //does headland control sections
        //bnd->isSectionControlledByHeadland = cboxIsSectionControlled.Checked;
        //Properties.Settings.Default.setHeadland_isSectionControlled = cboxIsSectionControlled.Checked;
        //Properties.Settings.Default.Save();

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
        Vec3 ptEnd(hdArr[hdArr.count() - 1].easting, hdArr[hdArr.count() - 1].northing, hdArr[hdArr.count() - 1].heading);

        bnd->bndList[0].hdLine.append(ptEnd);
    }

    emit saveHeadland();
}

void FormHeadland::isSectionControlled(bool wellIsIt) {
    bnd->isSectionControlledByHeadland = wellIsIt;
    property_setHeadland_isSectionControlled = wellIsIt;
}

void FormHeadland::btnBndLoop_Click() {
    int ptCount = bnd->bndList[0].fenceLine.count();

    if ((double)lineDistance == 0)
    {
        hdl->desList.clear();

        bnd->bndList[0].hdLine.clear();

        for (int i = 0; i < ptCount; i++)
        {
            bnd->bndList[0].hdLine.append(Vec3(bnd->bndList[0].fenceLine[i]));
        }
    }
    else
    {
        hdl->desList.clear();

        //outside point
        Vec3 pt3;

        double moveDist = (double)lineDistance;
        double distSq = (moveDist) * (moveDist) * 0.999;

        //make the boundary tram outer array
        for (int i = 0; i < ptCount; i++)
        {
            //calculate the point inside the boundary
            pt3.easting = bnd->bndList[0].fenceLine[i].easting -
                          (sin(glm::PIBy2 + bnd->bndList[0].fenceLine[i].heading) * (moveDist));

            pt3.northing = bnd->bndList[0].fenceLine[i].northing -
                           (cos(glm::PIBy2 + bnd->bndList[0].fenceLine[i].heading) * (moveDist));

            pt3.heading = bnd->bndList[0].fenceLine[i].heading;

            bool Add = true;

            for (int j = 0; j < ptCount; j++)
            {
                double check = glm::DistanceSquared(pt3.northing, pt3.easting,
                                                    bnd->bndList[0].fenceLine[j].northing, bnd->bndList[0].fenceLine[j].easting);
                if (check < distSq)
                {
                    Add = false;
                    break;
                }
            }

            if (Add)
            {
                if (hdl->desList.count() > 0)
                {
                    double dist = ((pt3.easting - hdl->desList[hdl->desList.count() - 1].easting) * (pt3.easting - hdl->desList[hdl->desList.count() - 1].easting))
                                  + ((pt3.northing - hdl->desList[hdl->desList.count() - 1].northing) * (pt3.northing - hdl->desList[hdl->desList.count() - 1].northing));
                    if (dist > 1)
                        hdl->desList.append(pt3);
                }
                else hdl->desList.append(pt3);
            }
        }

        if (hdl->desList.count() == 0)
        {
            return;
        }

        pt3 = hdl->desList[0];
        hdl->desList.append(pt3);

        int cnt = hdl->desList.count();
        if (cnt > 3)
        {
            pt3 = hdl->desList[0];
            hdl->desList.append(pt3);

            //make sure point distance isn't too big
            MakePointMinimumSpacing(hdl->desList, 1.2);
            CalculateHeadings(hdl->desList);

            bnd->bndList[0].hdLine.clear();

            //write out the Points
            for (Vec3 item: hdl->desList)
            {
                bnd->bndList[0].hdLine.append(item);
            }
        }
    }

    emit saveHeadland();
}

void FormHeadland::btnSlice_Click() {
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
    sliceArr.clear();
    sliceCount = sliceArr.count();
    backupCount = backupList.count();
}

void FormHeadland::btnDeletePoints_Click() {
    start = 99999; end = 99999;
    isA = true;
    hdl->desList.clear();
    sliceArr.clear();
    backupList.clear();
    bnd->bndList[0].hdLine.clear();

    int ptCount = bnd->bndList[0].fenceLine.count();

    for (int i = 0; i < ptCount; i++)
    {
        bnd->bndList[0].hdLine.append(bnd->bndList[0].fenceLine[i]);
    }
    sliceCount = sliceArr.count();
    backupCount = backupList.count();
}

void FormHeadland::btnUndo_Click() {
    bnd->bndList[0].hdLine.clear();
    for (auto item: backupList)
    {
        bnd->bndList[0].hdLine.append(item);
    }
    backupList.clear();
    backupCount = backupList.count();
}

void FormHeadland::btnALength_Click() {
    if (sliceArr.count() > 0)
    {
        //and the beginning
        Vec3 start = sliceArr[0];

        for (int i = 1; i < 10; i++)
        {
            Vec3 pt = start;
            pt.easting -= (sin(pt.heading) * i);
            pt.northing -= (cos(pt.heading) * i);
            sliceArr.insert(0, pt);
        }
    }
    sliceCount = sliceArr.count();
}

void FormHeadland::btnBLength_Click()
{
    if (sliceArr.count() > 0)
    {
        int ptCnt = sliceArr.count() - 1;

        for (int i = 1; i < 10; i++)
        {
            Vec3 pt = sliceArr[ptCnt];
            pt.easting += (sin(pt.heading) * i);
            pt.northing += (cos(pt.heading) * i);
            sliceArr.append(pt);
        }
    }
    sliceCount = sliceArr.count();
}

void FormHeadland::btnBShrink_Click()
{
    if (sliceArr.count() > 8)
        sliceArr.remove(sliceArr.count() - 5, 5);
    sliceCount = sliceArr.count();
}

void FormHeadland::btnAShrink_Click()
{
    if (sliceArr.count() > 8)
        sliceArr.remove(0, 5);
    sliceCount = sliceArr.count();
}

void FormHeadland::btnHeadlandOff_Click()
{
    bnd->bndList[0].hdLine.clear();
    emit saveHeadland();
    bnd->isHeadlandOn = false;
    vehicle->isHydLiftOn = false;
}

