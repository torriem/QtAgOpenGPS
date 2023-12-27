#include "ccontour.h"
#include "cvehicle.h"
#include <math.h>
#include <limits>
#include <QOpenGLFunctions>
#include "glm.h"
#include "cvehicle.h"
#include <QSettings>
#include "glutils.h"
#include "cboundary.h"
#include "cyouturn.h"
#include "cahrs.h"
#include "aogproperty.h"
#include "cnmea.h"
#include "common.h"

CContour::CContour(QObject *parent)
    : QObject(parent)
{

}

void CContour::SetLockToLine()
{
    if (ctList.count() > 5) isLocked = !isLocked;
}

void CContour::BuildContourGuidanceLine(double secondsSinceStart, CVehicle &vehicle, Vec3 pivot)
{
    double tool_toolWidth = property_setVehicle_toolWidth;
    double tool_toolOverlap = property_setVehicle_toolOverlap;
    double tool_toolOffset = property_setVehicle_toolOffset;
    double tool_toolHalfWidth = (tool_toolWidth - tool_toolOverlap) / 2;

    if (ctList.count() == 0)
    {
        if ((secondsSinceStart - lastSecond) < 0.3) return;
    }
    else
    {
        if ((secondsSinceStart - lastSecond) < 2) return;
    }

    lastSecond = secondsSinceStart;
    int ptCount;
    minDistance = glm::DOUBLE_MAX;
    int start, stop;

    double toolContourDistance = (tool_toolWidth * 3 + fabs(tool_toolOffset));

    //check if no strips yet, return
    int stripCount = stripList.count();

    //if making a new strip ignore it or it will win always
    //stripCount--;
    if (stripCount < 1) return;

    double sinH = sin(pivot.heading) * 0.2;
    double cosH = cos(pivot.heading) * 0.2;

    double sin2HL = sin(pivot.heading + glm::PIBy2);
    double cos2HL = cos(pivot.heading + glm::PIBy2);

    boxA.easting = pivot.easting - sin2HL + sinH;
    boxA.northing = pivot.northing - cos2HL + cosH;

    boxB.easting = pivot.easting + sin2HL + sinH;
    boxB.northing = pivot.northing + cos2HL + cosH;

    if (!isLocked)
    {
        stripNum = -1;
        for (int s = 0; s < stripCount; s++)
        {
            int p;
            ptCount = stripList[s].count();
            if (ptCount == 0) continue;
            double dist;
            for (p = 0; p < ptCount; p += 3)
            {
                //if (s == stripCount - 1)
                {
                    if ((((boxA.easting - boxB.easting) * (stripList[s][p].northing - boxB.northing))
                         - ((boxA.northing - boxB.northing) * (stripList[s][p].easting - boxB.easting))) > 0)
                    {
                        continue;
                    }
                }

                dist = ((pivot.easting - stripList[s][p].easting) * (pivot.easting - stripList[s][p].easting))
                       + ((pivot.northing - stripList[s][p].northing) * (pivot.northing - stripList[s][p].northing));
                if (dist < minDistance)
                {
                    minDistance = dist;
                    stripNum = s;
                    pt = lastLockPt = p;
                    //B = p;
                }
            }
        }
        minDistance = sqrt(minDistance);

        if (stripNum < 0 || minDistance > toolContourDistance || stripList[stripNum].count() < 4)
        {
            //no points in the box, exit
            ctList.clear();
            isLocked = false;
            return;
        }
    }

    //locked to this stripNum so find closest within a range
    else
    {
        //no points in the box, exit
        ptCount = stripList[stripNum].count();

        if (ptCount < 2)
        {
            ctList.clear();
            isLocked = false;
            return;
        }

        start = lastLockPt - 20; if (start < 0) start = 0;
        stop = lastLockPt + 20; if (stop > ptCount) stop = ptCount;

        //determine closest point
        minDistance = glm::DOUBLE_MAX;

        //if being built, start high, keep from guiding latest points made
        //int currentStripBox = 0;
        //if (stripNum == stripCount) currentStripBox = 10;
        for (int i = start; i < stop; i += 3)
        {
            double dist = ((pivot.easting - stripList[stripNum][i].easting) * (pivot.easting - stripList[stripNum][i].easting))
                          + ((pivot.northing - stripList[stripNum][i].northing) * (pivot.northing - stripList[stripNum][i].northing));

            if (minDistance >= dist)
            {
                minDistance = dist;
                pt = lastLockPt = i;
            }
        }

        minDistance = sqrt(minDistance);

        if (minDistance > toolContourDistance)
        {
            ctList.clear();
            isLocked = false;
            return;
        }
    }

    //now we have closest point, the distance squared from it, and which patch and point its from
    refX = stripList[stripNum][pt].easting;
    refZ = stripList[stripNum][pt].northing;

    double dx, dz, distanceFromRefLine;

    if (pt < stripList[stripNum].count() - 1)
    {
        dx = stripList[stripNum][pt + 1].easting - refX;
        dz = stripList[stripNum][pt + 1].northing - refZ;

        //how far are we away from the reference line at 90 degrees - 2D cross product and distance
        distanceFromRefLine = ((dz * pivot.easting) - (dx * pivot.northing) + (stripList[stripNum][pt + 1].easting
                                                                               * refZ) - (stripList[stripNum][pt + 1].northing * refX))
                              / sqrt((dz * dz) + (dx * dx));
    }
    else if (pt > 0)
    {
        dx = refX - stripList[stripNum][pt - 1].easting;
        dz = refZ - stripList[stripNum][pt - 1].northing;

        //how far are we away from the reference line at 90 degrees - 2D cross product and distance
        distanceFromRefLine = ((dz * pivot.easting) - (dx * pivot.northing) + (refX
                                                                               * stripList[stripNum][pt - 1].northing) - (refZ * stripList[stripNum][pt - 1].easting))
                              / sqrt((dz * dz) + (dx * dx));
    }
    else return;

    //are we going same direction as stripList was created?
    bool isSameWay = M_PI - fabs(fabs(vehicle.fixHeading - stripList[stripNum][pt].heading) - M_PI) < 1.57;

    double RefDist = (distanceFromRefLine + (isSameWay ? tool_toolOffset : -tool_toolOffset))
                     / (tool_toolWidth - tool_toolOverlap);

    double howManyPathsAway;

    if (fabs(distanceFromRefLine) > tool_toolHalfWidth
        || fabs(tool_toolOffset) > tool_toolHalfWidth)
    {
        //beside what is done
        if (RefDist < 0) howManyPathsAway = -1;
        else howManyPathsAway = 1;
    }
    else
    {
        //driving on what is done
        howManyPathsAway = 0;
    }

    if (howManyPathsAway >= -1 && howManyPathsAway <= 1)
    {
        ctList.clear();

        //don't guide behind yourself
        //if (stripNum == stripList.count()-1 && howManyPathsAway == 0)
        //return;

        //make the new guidance line list called guideList
        ptCount = stripList[stripNum].count();

        //shorter behind you
        if (isSameWay)
        {
            start = pt - 20; if (start < 0) start = 0;
            stop = pt + 70; if (stop > ptCount) stop = ptCount;
        }
        else
        {
            start = pt - 70; if (start < 0) start = 0;
            stop = pt + 20; if (stop > ptCount) stop = ptCount;
        }

        //if (howManyPathsAway != 0 && (mf.tool.halfToolWidth < (0.5*mf.tool.toolOffset)))
        {
            double distAway = (tool_toolWidth - tool_toolOverlap) * howManyPathsAway
                              + (isSameWay ? -tool_toolOffset : tool_toolOffset);
            double distSqAway = (distAway * distAway) * 0.97;

            for (int i = start; i < stop; i++)
            {
                Vec3 point(
                    stripList[stripNum][i].easting + (cos(stripList[stripNum][i].heading) * distAway),
                    stripList[stripNum][i].northing - (sin(stripList[stripNum][i].heading) * distAway),
                    stripList[stripNum][i].heading);

                bool isOkToAdd = true;
                //make sure its not closer then 1 eq width
                for (int j = start; j < stop; j++)
                {
                    double check = glm::distanceSquared(point.northing, point.easting,
                                                       stripList[stripNum][j].northing, stripList[stripNum][j].easting);
                    if (check < distSqAway)
                    {
                        isOkToAdd = false;
                        break;
                    }
                }

                if (isOkToAdd)
                {
                    if (ctList.count() > 0)
                    {
                        double dist =
                            ((point.easting - ctList[ctList.count() - 1].easting) * (point.easting - ctList[ctList.count() - 1].easting))
                            + ((point.northing - ctList[ctList.count() - 1].northing) * (point.northing - ctList[ctList.count() - 1].northing));
                        if (dist > 0.2)
                            ctList.append(point);
                    }
                    else ctList.append(point);
                }
            }
        }

        int ptc = ctList.count();
        if (ptc < 5)
        {
            ctList.clear();
            isLocked = false;
            return;
        }
    }
    else
    {
        ctList.clear();
        isLocked = false;
        return;
    }


}

void CContour::DistanceFromContourLine(bool isAutoSteerBtnOn,
                                       CVehicle &vehicle,
                                       CYouTurn &yt,
                                       CAHRS &ahrs,
                                       CNMEA &pn,
                                       Vec3 pivot, Vec3 steer)
{
    double wheelbase = property_setVehicle_wheelbase;
    double maxSteerAngle = property_setVehicle_maxSteerAngle;
    //double maxAngularVelocity = property_setVehicle_maxAngularVelocity;
    double stanleyHeadingErrorGain = property_stanleyHeadingErrorGain;
    double stanleyDistanceErrorGain = property_stanleyDistanceErrorGain;
    double purePursuitIntegralGain = property_purePursuitIntegralGainAB;

    bool isStanleyUsed = property_setVehicle_isStanleyUsed;

    double minDistA = 1000000, minDistB = 1000000;
    int ptCount = ctList.count();
    //distanceFromCurrentLine = 9999;
    if (ptCount > 8)
    {
        if (isStanleyUsed)
        {
            //find the closest 2 points to current fix
            for (int t = 0; t < ptCount; t++)
            {
                double dist = ((steer.easting - ctList[t].easting) * (steer.easting - ctList[t].easting))
                              + ((steer.northing - ctList[t].northing) * (steer.northing - ctList[t].northing));
                if (dist < minDistA)
                {
                    minDistB = minDistA;
                    B = A;
                    minDistA = dist;
                    A = t;
                }
                else if (dist < minDistB)
                {
                    minDistB = dist;
                    B = t;
                }
            }

            //just need to make sure the points continue ascending in list order or heading switches all over the place
            if (A > B) { C = A; A = B; B = C; }

            //get the distance from currently active AB line
            //x2-x1
            double dx = ctList[B].easting - ctList[A].easting;
            //z2-z1
            double dy = ctList[B].northing - ctList[A].northing;

            if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dy) < glm::DOUBLE_EPSILON) return;

            //how far from current AB Line is fix
            distanceFromCurrentLinePivot = ((dy * steer.easting) - (dx * steer.northing) +
                                            (ctList[B].easting * ctList[A].northing) -
                                            (ctList[B].northing * ctList[A].easting)) /
                                           sqrt((dy * dy) + (dx * dx));

            abHeading = atan2(dx, dy);
            if (abHeading < 0) abHeading += glm::twoPI;

            isHeadingSameWay = M_PI - fabs(fabs(pivot.heading - abHeading) - M_PI) < glm::PIBy2;

            // calc point on ABLine closest to current position
            double U = (((steer.easting - ctList[A].easting) * dx) + ((steer.northing - ctList[A].northing) * dy))
                       / ((dx * dx) + (dy * dy));

            rEastCT = ctList[A].easting + (U * dx);
            rNorthCT = ctList[A].northing + (U * dy);

            //distance is negative if on left, positive if on right
            if (isHeadingSameWay)
            {
                abFixHeadingDelta = (steer.heading - abHeading);
            }
            else
            {
                distanceFromCurrentLinePivot *= -1.0;
                abFixHeadingDelta = (steer.heading - abHeading + M_PI);
            }

            //Fix the circular error
            if (abFixHeadingDelta > M_PI) abFixHeadingDelta -= M_PI;
            else if (abFixHeadingDelta < M_PI) abFixHeadingDelta += M_PI;

            if (abFixHeadingDelta > glm::PIBy2) abFixHeadingDelta -= M_PI;
            else if (abFixHeadingDelta < -glm::PIBy2) abFixHeadingDelta += M_PI;

            if (vehicle.isReverse) abFixHeadingDelta *= -1;

            abFixHeadingDelta *= stanleyHeadingErrorGain;
            if (abFixHeadingDelta > 0.74) abFixHeadingDelta = 0.74;
            if (abFixHeadingDelta < -0.74) abFixHeadingDelta = -0.74;

            steerAngleCT = atan((distanceFromCurrentLinePivot * stanleyDistanceErrorGain)
                                     / ((fabs(vehicle.avgSpeed) * 0.277777) + 1));

            if (steerAngleCT > 0.74) steerAngleCT = 0.74;
            if (steerAngleCT < -0.74) steerAngleCT = -0.74;

            steerAngleCT = glm::toDegrees((steerAngleCT + abFixHeadingDelta) * -1.0);

            if (steerAngleCT < -maxSteerAngle) steerAngleCT = maxSteerAngle;
            if (steerAngleCT > maxSteerAngle) steerAngleCT = maxSteerAngle;
        }
        else
        {
            //find the closest 2 points to current fix
            for (int t = 0; t < ptCount; t++)
            {
                double dist = ((pivot.easting - ctList[t].easting) * (pivot.easting - ctList[t].easting))
                              + ((pivot.northing - ctList[t].northing) * (pivot.northing - ctList[t].northing));
                if (dist < minDistA)
                {
                    minDistB = minDistA;
                    B = A;
                    minDistA = dist;
                    A = t;
                }
                else if (dist < minDistB)
                {
                    minDistB = dist;
                    B = t;
                }
            }

            //just need to make sure the points continue ascending in list order or heading switches all over the place
            if (A > B) { C = A; A = B; B = C; }

            if (isLocked && (A < 2 || B > ptCount - 3))
            {
                //ctList.clear();
                isLocked = false;
                lastLockPt = INT_MAX;
                return;
            }

            //get the distance from currently active AB line
            //x2-x1
            double dx = ctList[B].easting - ctList[A].easting;
            //z2-z1
            double dy = ctList[B].northing - ctList[A].northing;

            if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dy) < glm::DOUBLE_EPSILON) return;

            //how far from current AB Line is fix
            distanceFromCurrentLinePivot = ((dy * pn.fix.easting) - (dx * pn.fix.northing) +
                                            (ctList[B].easting * ctList[A].northing) -
                                            (ctList[B].northing * ctList[A].easting)) /
                                           sqrt((dy * dy) + (dx * dx));

            //integral slider is set to 0
            if (purePursuitIntegralGain != 0)
            {
                pivotDistanceError = distanceFromCurrentLinePivot * 0.2 + pivotDistanceError * 0.8;

                if (counter2++ > 4)
                {
                    pivotDerivative = pivotDistanceError - pivotDistanceErrorLast;
                    pivotDistanceErrorLast = pivotDistanceError;
                    counter2 = 0;
                    pivotDerivative *= 2;

                    //limit the derivative
                    //if (pivotDerivative > 0.03) pivotDerivative = 0.03;
                    //if (pivotDerivative < -0.03) pivotDerivative = -0.03;
                    //if (fabs(pivotDerivative) < 0.01) pivotDerivative = 0;
                }

                //pivotErrorTotal = pivotDistanceError + pivotDerivative;

                if (isAutoSteerBtnOn
                    && fabs(pivotDerivative) < (0.1)
                    && vehicle.avgSpeed > 2.5
                    && !yt.isYouTurnTriggered)
                {
                    //if over the line heading wrong way, rapidly decrease integral
                    if ((inty < 0 && distanceFromCurrentLinePivot < 0) || (inty > 0 && distanceFromCurrentLinePivot > 0))
                    {
                        inty += pivotDistanceError * purePursuitIntegralGain * -0.06;
                    }
                    else
                    {
                        if (fabs(distanceFromCurrentLinePivot) > 0.02)
                        {
                            inty += pivotDistanceError * purePursuitIntegralGain * -0.02;
                            if (inty > 0.2) inty = 0.2;
                            else if (inty < -0.2) inty = -0.2;
                        }
                    }
                }
                else inty *= 0.95;
            }
            else inty = 0;

            if (vehicle.isReverse) inty = 0;

            isHeadingSameWay = M_PI - fabs(fabs(pivot.heading - ctList[A].heading) - M_PI) < glm::PIBy2;

            if (!isHeadingSameWay)
                distanceFromCurrentLinePivot *= -1.0;

            // ** Pure pursuit ** - calc point on ABLine closest to current position
            double U = (((pivot.easting - ctList[A].easting) * dx) + ((pivot.northing - ctList[A].northing) * dy))
                       / ((dx * dx) + (dy * dy));

            rEastCT = ctList[A].easting + (U * dx);
            rNorthCT = ctList[A].northing + (U * dy);

            //update base on autosteer settings and distance from line
            double goalPointDistance = vehicle.UpdateGoalPointDistance();

            bool ReverseHeading = vehicle.isReverse ? !isHeadingSameWay : isHeadingSameWay;

            int count = ReverseHeading ? 1 : -1;
            Vec3 start(rEastCT, rNorthCT, 0);
            double distSoFar = 0;

            for (int i = ReverseHeading ? B : A; i < ptCount && i >= 0; i += count)
            {
                // used for calculating the length squared of next segment.
                double tempDist = glm::distance(start, ctList[i]);

                //will we go too far?
                if ((tempDist + distSoFar) > goalPointDistance)
                {
                    double j = (goalPointDistance - distSoFar) / tempDist; // the remainder to yet travel

                    goalPointCT.easting = (((1 - j) * start.easting) + (j * ctList[i].easting));
                    goalPointCT.northing = (((1 - j) * start.northing) + (j * ctList[i].northing));
                    break;
                }
                else distSoFar += tempDist;
                start = ctList[i];
            }

            //calc "D" the distance from pivot axle to lookahead point
            double goalPointDistanceSquared = glm::distanceSquared(goalPointCT.northing, goalPointCT.easting, pivot.northing, pivot.easting);

            //calculate the the delta x in local coordinates and steering angle degrees based on wheelbase
            double localHeading;// = glm::twoPI - mf.fixHeading;

            if (isHeadingSameWay) localHeading = glm::twoPI - vehicle.fixHeading + inty;
            else localHeading = glm::twoPI - vehicle.fixHeading - inty;

            steerAngleCT = glm::toDegrees(atan(2 * (((goalPointCT.easting - pivot.easting) * cos(localHeading))
                                                        + ((goalPointCT.northing - pivot.northing) * sin(localHeading))) * wheelbase / goalPointDistanceSquared));

            if (ahrs.imuRoll != 88888)
                steerAngleCT += ahrs.imuRoll * -(double)property_setAS_sideHillComp;

            if (steerAngleCT < -maxSteerAngle) steerAngleCT = -maxSteerAngle;
            if (steerAngleCT > maxSteerAngle) steerAngleCT = maxSteerAngle;

        }

        //used for smooth mode
        vehicle.modeActualXTE = (distanceFromCurrentLinePivot);

        //fill in the autosteer variables
        vehicle.guidanceLineDistanceOff = (short)glm::roundMidAwayFromZero(distanceFromCurrentLinePivot * 1000.0);
        vehicle.guidanceLineSteerAngle = (short)(steerAngleCT * 100);
    }
    else
    {
        //invalid distance so tell AS module
        distanceFromCurrentLinePivot = 32000; //???
        vehicle.guidanceLineDistanceOff = 32000;
    }

}

//start stop and add points to list
void CContour::StartContourLine() {
    qDebug() << "starting contour line.";
    //if (stripList.count() == 0)
    //{
    //make new ptList
    ptList.clear();
    //ptList.append(new vec3(pivot.easting + cos(pivot.heading)
    //    * mf.tool.toolOffset, pivot.northing - sin(pivot.heading) * mf.tool.toolOffset, pivot.heading));
    stripList.append(ptList); //append empty list
    isContourOn = true;
    return;
    //}
    //else
    //{
    //    //reuse ptList
    //    ptList?.clear();
    //    //ptList.append(new vec3(pivot.easting + cos(pivot.heading)
    //    //    * mf.tool.toolOffset, pivot.northing - sin(pivot.heading) * mf.tool.toolOffset, pivot.heading));
    //    isContourOn = true;
    //}
}

//Add current position to stripList
void CContour::AddPoint(Vec3 pivot) {
    double tool_toolOffset = property_setVehicle_toolOffset;

    ptList.append(Vec3(pivot.easting + cos(pivot.heading) * tool_toolOffset,
                        pivot.northing - sin(pivot.heading) * tool_toolOffset,
                        pivot.heading));
}

//End the strip
void CContour::StopContourLine(QVector<QVector<Vec3>> &contourSaveList)
{
    qDebug() << ptList.count() << "Stopping contour line.";

    //make sure its long enough to bother
    if (ptList.count() > 5)
    {
        //add the point list to the save list for appending to contour file
        contourSaveList.append(ptList);
    }

    //delete ptList
    else
    {
        ptList.clear();
    }

    //turn it off
    isContourOn = false;
}

//build contours for boundaries
void CContour::BuildFenceContours(CBoundary &bnd, double spacingInt, int patchCounter)
{
    double tool_toolWidth = property_setVehicle_toolWidth;
    double tool_toolOverlap = property_setVehicle_toolOverlap;

    spacingInt *= 0.01;
    if (bnd.bndList.count() == 0)
    {
        emit TimedMessage(1500, tr("Boundary Contour Error"), tr("No Boundaries Made"));
        return;
    }

    if (patchCounter != 0)
    {
        emit TimedMessage(1500, tr("Section Control On"), tr("Turn Off Section Control"));
        return;
    }

    Vec3 point;
    double totalHeadWidth;
    int signPass;

    signPass = -1;
    //determine how wide a headland space
    totalHeadWidth = ((tool_toolWidth - tool_toolOverlap) * 0.5) - spacingInt;

    //totalHeadWidth = (mf.tool.toolWidth - mf.tool.toolOverlap) * 0.5 + 0.2 + (mf.tool.toolWidth - mf.tool.toolOverlap);

    for (int j = 0; j < bnd.bndList.count(); j++)
    {
        //count the points from the boundary
        int ptCount = bnd.bndList[j].fenceLine.count();

        ptList.clear();
        stripList.append(ptList);

        for (int i = ptCount - 1; i >= 0; i--)
        {
            //calculate the point inside the boundary
            point.easting = bnd.bndList[j].fenceLine[i].easting - (signPass * sin(glm::PIBy2 + bnd.bndList[j].fenceLine[i].heading) * totalHeadWidth);
            point.northing =bnd.bndList[j].fenceLine[i].northing - (signPass * cos(glm::PIBy2 + bnd.bndList[j].fenceLine[i].heading) * totalHeadWidth);
            point.heading = bnd.bndList[j].fenceLine[i].heading - M_PI;
            if (point.heading < -glm::twoPI) point.heading += glm::twoPI;

            ptList.append(point);
        }
    }

    emit TimedMessage(1500, tr("Boundary Contour"), tr("Contour Path Created"));
}

//draw the red follow me line
void CContour::DrawContourLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    double lineWidth = property_setDisplay_lineWidth;
    bool isStanleyUsed = property_setVehicle_isStanleyUsed;
    bool isPureDisplayOn = property_setMenu_isPureOn;

    GLHelperOneColor gldraw;
    QColor color;

    ////draw the guidance line
    int ptCount = ctList.count();
    if (ptCount < 2) return;


    color.setRgbF(0.98f, 0.2f, 0.980f);
    for (int h = 0; h < ptCount; h++)
        gldraw.append(QVector3D(ctList[h].easting, ctList[h].northing, 0));

    gldraw.draw(gl,mvp,color,GL_LINE_STRIP, lineWidth);


    gldraw.clear();
    color.setRgbF(0.87f, 08.7f, 0.25f);
    for (int h = 0; h < ptCount; h++)
        gldraw.append(QVector3D(ctList[h].easting, ctList[h].northing, 0));

    gldraw.draw(gl,mvp,color,GL_POINTS,lineWidth);

    //Draw the captured ref strip, red if locked
    double useWidth;

    if (isLocked)
    {
        color.setRgbF(0.983f, 0.92f, 0.420f);
        useWidth = 4;
    }
    else
    {
        color.setRgbF(0.3f, 0.982f, 0.0f);
        useWidth = lineWidth;
    }

    if (stripNum > -1)
    {
        gldraw.clear();

        for (int h = 0; h < stripList[stripNum].count(); h++)
            gldraw.append(QVector3D(stripList[stripNum][h].easting, stripList[stripNum][h].northing, 0));

        gldraw.draw(gl,mvp,color,GL_POINTS,useWidth);
    }

    color.setRgbF(0.35f, 0.30f, 0.90f);
    gldraw.clear();
    gldraw.append(QVector3D(stripList[stripNum][pt].easting, stripList[stripNum][pt].northing, 0));
    gldraw.draw(gl,mvp,color,GL_POINTS,6.0f);

    if (isPureDisplayOn && distanceFromCurrentLinePivot != 32000 && !isStanleyUsed)
    {
        gldraw.clear();
        color.setRgbF(1.0f, 0.95f, 0.095f);
        gldraw.append(QVector3D(goalPointCT.easting, goalPointCT.northing, 0.0));
        gldraw.draw(gl,mvp,color,GL_POINTS,6.0f);
    }
}

//Reset the contour to zip
void CContour::ResetContour()
{
    stripList.clear();
    ptList.clear();
    ctList.clear();
}
