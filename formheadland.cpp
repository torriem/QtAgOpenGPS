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



FormHeadland::FormHeadland(QObject *parent)
    : QObject{parent}
{}

void FormHeadland::connect_ui(QObject *headland_designer_instance) {
    this->headland_designer_instance = headland_designer_instance;

    InterfaceProperty<HeadlandDesigner,int>::set_qml_root(headland_designer_instance);
    InterfaceProperty<HeadlandDesigner,bool>::set_qml_root(headland_designer_instance);
    InterfaceProperty<HeadlandDesigner,double>::set_qml_root(headland_designer_instance);

    //example
    connect(qmlItem(headland_designer_instance,"somebtn"),SIGNAL(blah()),this,SLOT(load_headline()));
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
            gldraw.append( { bnd->bndList[j].fenceLine[i].easting,
                           bnd->bndList[j].fenceLine[i].northing, 0} );
        }

        gldraw.draw(gl,modelview * projection, color, GL_LINE_STRIP,lineWidth);
    }

    //the vehicle
    gldraw.clear();
    gldraw.append(  { vehicle->pivotAxlePos.easting, vehicle->pivotAxlePos.northing, 0.0 } );
    color = QColor::fromRgbF(0.95f, 0.190f, 0.20f);
    gldraw.draw(gl, modelview * projection, color, GL_POINTS, 16.0f);

    //draw the actual built lines
    DrawBuiltLines(gl, modelview * projection);
}

void FormHeadland::ogl_resize() {

}

void FormHeadland::DrawBuiltLines(QOpenGLFunctions *gl, QMatrix4x4 mvp) {

}
