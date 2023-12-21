#include "cboundary.h"
#include "cnmea.h"
#include "glm.h"
#include "cvehicle.h"
#include <math.h>
#include <QLocale>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <limits.h>
#include "glutils.h"
#include "aogproperty.h"
#include "cabline.h"
#include "cfielddata.h"
#include "cmodulecomm.h"

CBoundary::CBoundary()
{
    turnSelected = 0;
}

bool CBoundary::isPointInsideFenceArea(Vec3 testPoint) const
{
    //first where are we, must be inside outer and outside of inner geofence non drive thru turn borders
    if (glm::isPointInPolygon(bndList[0].fenceLineEar,testPoint))
    {
        for (int i = 1; i < bndList.count(); i++)
        {
            //make sure not inside a non drivethru boundary
            if (bndList[i].isDriveThru) continue;
            if (glm::isPointInPolygon(bndList[i].fenceLineEar,testPoint))
            {
                return false;
            }
        }
        //we are safely inside outer, outside inner boundaries
        return true;
    }
    return false;
}


bool CBoundary::isPointInsideFenceArea(Vec2 testPoint) const
{
    //first where are we, must be inside outer and outside of inner geofence non drive thru turn borders
    if (glm::isPointInPolygon(bndList[0].fenceLineEar,testPoint))
    {
        for (int i = 1; i < bndList.count(); i++)
        {
            //make sure not inside a non drivethru boundary
            if (bndList[i].isDriveThru) continue;
            if (glm::isPointInPolygon(bndList[i].fenceLineEar,testPoint))
            {
                return false;
            }
        }
        //we are safely inside outer, outside inner boundaries
        return true;
    }
    return false;

}

void CBoundary::drawFenceLines(const CVehicle &v, const CModuleComm &mc,
                               QOpenGLFunctions *gl,
                                 const QMatrix4x4 &mvp)
{
    QColor color;
    float line_width;
    Vec3 pivot = v.pivotAxlePos;

    GLHelperOneColor gldraw;

    if (!mc.isOutOfBounds)
    {
        color.setRgbF(0.95f, 0.75f, 0.50f);
        line_width = property_setDisplay_lineWidth;
    }
    else
    {
        line_width = (float) property_setDisplay_lineWidth * 3;
        color.setRgbF(0.95f, 0.25f, 0.25f);
    }

    for (int i = 0; i < bndList.count(); i++)
    {
        gldraw.clear();
        for (int j = 0; j < bndList[i].fenceLineEar.count(); j++) {
            gldraw.append(QVector3D(bndList[i].fenceLineEar[j].easting,
                                    bndList[i].fenceLineEar[j].northing, 0));
        }
        gldraw.draw(gl, mvp, color, GL_TRIANGLE_STRIP, line_width);
    }


    //closest points  TooDoo
    gldraw.append(QVector3D(closestTurnPt.easting, closestTurnPt.northing, 0));
    gldraw.draw(gl,mvp,QColor::fromRgbF(0.70f, 0.95f, 0.95f),
                GL_POINTS,6.0f);

    if (bndBeingMadePts.count() > 0)
    {
        GLHelperColors gldraw1;
        ColorVertex cv;


        //the boundary so far
        //vec3 pivot = mf.pivotAxlePos;
        cv.color = QVector4D(0.825f, 0.22f, 0.90f, 1.0f);

        for (int h = 0; h < bndBeingMadePts.count(); h++) {
            cv.vertex = QVector3D(bndBeingMadePts[h].easting, bndBeingMadePts[h].northing, 0);
            gldraw1.append(cv);
        }

        cv.color = QVector4D(0.295f, 0.972f, 0.290f, 1.0f);
        cv.vertex = QVector3D(bndBeingMadePts[0].easting, bndBeingMadePts[0].northing, 0);
        gldraw1.append(cv);

        gldraw1.draw(gl, mvp, GL_LINE_STRIP, property_setDisplay_lineWidth);

        //line from last point to pivot marker
        gldraw.clear();
        if (isDrawRightSide)
        {
            gldraw.append(QVector3D(bndBeingMadePts[0].easting, bndBeingMadePts[0].northing, 0));
            gldraw.append(QVector3D(pivot.easting + (sin(pivot.heading - glm::PIBy2) * -createBndOffset),
                                     pivot.northing + (cos(pivot.heading - glm::PIBy2) * -createBndOffset), 0));

            gldraw.append(QVector3D(bndBeingMadePts[bndBeingMadePts.size() - 1].easting,
                                     bndBeingMadePts[bndBeingMadePts.size() - 1].northing, 0));
        }
        else
        {
            gldraw.append(QVector3D(bndBeingMadePts[0].easting, bndBeingMadePts[0].northing, 0));
            gldraw.append(QVector3D(pivot.easting + (sin(pivot.heading - glm::PIBy2) * createBndOffset),
                                     pivot.northing + (cos(pivot.heading - glm::PIBy2) * createBndOffset), 0));
            gldraw.append(QVector3D(bndBeingMadePts[bndBeingMadePts.size() - 1].easting, bndBeingMadePts[bndBeingMadePts.size() - 1].northing, 0));
        }
        gldraw.draw(gl,mvp,QColor::fromRgbF(0.825f, 0.842f, 0.0f),
                     GL_LINE_STIPPLE,1.0f);

        //boundary points
        gldraw.clear();
        for (int h = 0; h < bndBeingMadePts.size(); h++)
            gldraw.append(QVector3D(bndBeingMadePts[h].easting, bndBeingMadePts[h].northing, 0));

        gldraw.draw(gl,mvp,QColor::fromRgbF(0.0f, 0.95f, 0.95f),
                    GL_POINTS, 6.0f);
    }
}

void CBoundary::findClosestTurnPoint(const CABLine &ABLine, Vec3 fromPt)
{
    double eP = fromPt.easting;
    double nP = fromPt.northing;
    double eAB, nAB;

    if (ABLine.isHeadingSameWay)
    {
        eAB = ABLine.currentABLineP2.easting;
        nAB = ABLine.currentABLineP2.northing;
    }
    else
    {
        eAB = ABLine.currentABLineP1.easting;
        nAB = ABLine.currentABLineP1.northing;
    }

    turnClosestList.clear();

    for (int j = 0; j < bndList.count(); j++)
    {
        for (int i = 0; i < bndList[j].turnLine.count() - 1; i++)
        {
            int res = glm::getLineIntersection(
                bndList[j].turnLine[i].easting,
                bndList[j].turnLine[i].northing,
                bndList[j].turnLine[i + 1].easting,
                bndList[j].turnLine[i + 1].northing,
                eP, nP, eAB, nAB, iE, iN );

            if (res == 1)
            {
                closePt.easting = iE;
                closePt.northing = iN;

                double hed = atan2(bndList[j].turnLine[i + 1].easting - bndList[j].turnLine[i].easting,
                                   bndList[j].turnLine[i + 1].northing - bndList[j].turnLine[i].northing);
                if (hed < 0) hed += glm::twoPI;
                closePt.heading = hed;
                turnClosestList.append(closePt);
            }
        }
    }

    //determine closest point
    double minDistance = glm::DOUBLE_MAX;

    if (turnClosestList.count() == 1)
    {
        closestTurnPt.easting = turnClosestList[0].easting;
        closestTurnPt.northing = turnClosestList[0].northing;
        closestTurnPt.heading = turnClosestList[0].heading;

        if (closestTurnPt.heading < 0) closestTurnPt.heading += glm::twoPI;
        return;
    }

    if (turnClosestList.count() > 0 || turnClosestList.count() != 0)
    {
        for (int i = 0; i < turnClosestList.count(); i++)
        {
            double dist = (((fromPt.easting - turnClosestList[i].easting) * (fromPt.easting - turnClosestList[i].easting))
                           + ((fromPt.northing - turnClosestList[i].northing) * (fromPt.northing - turnClosestList[i].northing)));

            if (minDistance >= dist)
            {
                minDistance = dist;
                closestTurnPt.easting = turnClosestList[i].easting;
                closestTurnPt.northing = turnClosestList[i].northing;
                closestTurnPt.heading = turnClosestList[i].heading;
            }
        }
        if (closestTurnPt.heading < 0) closestTurnPt.heading += glm::twoPI;
    }
}

void CBoundary::buildTurnLines(CFieldData &fd)
{
    if (bndList.count() == 0)
    {
        //mf.TimedMessageBox(1500, " No Boundaries", "No Turn Lines Made");
        return;
    }

    //update the GUI values for boundaries
    fd.updateFieldBoundaryGUIAreas(bndList);

    //to fill the list of line points
    Vec3 point;

    //determine how wide a headland space
    double totalHeadWidth = property_set_youTurnDistanceFromBoundary;

    //inside boundaries
    for (int j = 0; j < bndList.count(); j++)
    {
        bndList[j].turnLine.clear();
        if (bndList[j].isDriveThru) continue;

        int ptCount = bndList[j].fenceLine.count();

        for (int i = ptCount - 1; i >= 0; i--)
        {
            //calculate the point outside the boundary
            point.easting = bndList[j].fenceLine[i].easting + (-sin(glm::PIBy2 + bndList[j].fenceLine[i].heading) * totalHeadWidth);
            point.northing = bndList[j].fenceLine[i].northing + (-cos(glm::PIBy2 + bndList[j].fenceLine[i].heading) * totalHeadWidth);
            point.heading = bndList[j].fenceLine[i].heading;
            if (point.heading < -glm::twoPI) point.heading += glm::twoPI;

            //only add if outside actual field boundary
            if ((j == 0) && (0 == glm::isPointInPolygon(bndList[j].fenceLineEar,point)))
            {
                Vec3 tPnt(point.easting, point.northing, point.heading);
                bndList[j].turnLine.append(tPnt);
            }
        }
        bndList[j].fixTurnLine(totalHeadWidth, 2);

        //count the reference list of original curve
        //int cnt = bndList[j].turnLine.count();

        //the temp array
        QVector<Vec3> arr = bndList[j].turnLine;

        //for (int s = 0; s < cnt; s++)
        //{
        //    arr[s] = bndList[j].turnLine[s];
        //}

        double delta = 0;
        bndList[j].turnLine.clear();

        for (int i = 0; i < arr.count(); i++)
        {
            if (i == 0)
            {
                bndList[j].turnLine.append(arr[i]);
                continue;
            }
            delta += (arr[i - 1].heading - arr[i].heading);
            if (fabs(delta) > 0.005)
            {
                bndList[j].turnLine.append(arr[i]);
                delta = 0;
            }
        }

        Vec3 end(bndList[j].turnLine[0].easting,
                 bndList[j].turnLine[0].northing, bndList[j].turnLine[0].heading);
        bndList[j].turnLine.append(end);
    }
}



