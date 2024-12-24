#include "cabcurve.h"
#include "glutils.h"
#include "glm.h"
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include "vec3.h"
#include "vec2.h"
#include "cvehicle.h"
#include "cyouturn.h"
#include "cboundary.h"
#include "cyouturn.h"
#include "ctram.h"
#include "ccamera.h"
#include "cnmea.h"
#include "cahrs.h"
#include "cguidance.h"
#include "aogproperty.h"
#include "ctrack.h"

CABCurve::CABCurve(QObject *parent) : QObject(parent)
{

}

void CABCurve::BuildCurveCurrentList(Vec3 pivot,
                                     double secondsSinceStart,
                                     const CVehicle &vehicle,
                                     const CTrk &track,
                                     const CBoundary &bnd,
                                     const CYouTurn &yt)
{
    double minDistA = 1000000, minDistB;

    double tool_width = property_setVehicle_toolWidth;
    double tool_overlap = property_setVehicle_toolOverlap;
    double tool_offset = property_setVehicle_toolOffset;

    //move the ABLine over based on the overlap amount set in vehicle
    double widthMinusOverlap = tool_width - tool_overlap;

    if (track.mode != (int)TrackMode::waterPivot)
    {
        int refCount = track.curvePts.count();
        if (refCount < 5)
        {
            curList.clear();
            return;
        }

        //close call hit
        int cc = 0, dd;

        for (int j = 0; j < refCount; j += 10)
        {
            double dist = ((vehicle.guidanceLookPos.easting - track.curvePts[j].easting)
                           * (vehicle.guidanceLookPos.easting - track.curvePts[j].easting))
                          + ((vehicle.guidanceLookPos.northing - track.curvePts[j].northing)
                             * (vehicle.guidanceLookPos.northing - track.curvePts[j].northing));
            if (dist < minDistA)
            {
                minDistA = dist;
                cc = j;
            }
        }

        minDistA = minDistB = 1000000;

        dd = cc + 7;
        if (dd > refCount - 1) dd = refCount;
        cc -= 7;
        if (cc < 0) cc = 0;

        //find the closest 2 points to current close call
        for (int j = cc; j < dd; j++)
        {
            double dist = ((vehicle.guidanceLookPos.easting - track.curvePts[j].easting)
                           * (vehicle.guidanceLookPos.easting - track.curvePts[j].easting))
                          + ((vehicle.guidanceLookPos.northing - track.curvePts[j].northing)
                             * (vehicle.guidanceLookPos.northing - track.curvePts[j].northing));
            if (dist < minDistA)
            {
                minDistB = minDistA;
                rB = rA;
                minDistA = dist;
                rA = j;
            }
            else if (dist < minDistB)
            {
                minDistB = dist;
                rB = j;
            }
        }

        //reset the line over jump
        isLateralTriggered = false;

        if (rA > rB) {
            C = rA;
            rA = rB;
            rB = C;
        }

        //same way as line creation or not
        isHeadingSameWay = M_PI - fabs(fabs(pivot.heading - track.curvePts[rA].heading) - M_PI) < glm::PIBy2;

        if (yt.isYouTurnTriggered && yt.isGoingStraightThrough) isHeadingSameWay = !isHeadingSameWay;

        //which side of the closest point are we on is next
        //calculate endpoints of reference line based on closest point
        refPoint1.easting = track.curvePts[rA].easting - (sin(track.curvePts[rA].heading) * 300.0);
        refPoint1.northing = track.curvePts[rA].northing - (cos(track.curvePts[rA].heading) * 300.0);

        refPoint2.easting = track.curvePts[rA].easting + (sin(track.curvePts[rA].heading) * 300.0);
        refPoint2.northing = track.curvePts[rA].northing + (cos(track.curvePts[rA].heading) * 300.0);

        if (track.nudgeDistance != 0)
        {
            refPoint1.easting += (sin(track.curvePts[rA].heading + glm::PIBy2) * track.nudgeDistance);
            refPoint1.northing += (cos(track.curvePts[rA].heading + glm::PIBy2) * track.nudgeDistance);

            refPoint2.easting += (sin(track.curvePts[rA].heading + glm::PIBy2) * track.nudgeDistance);
            refPoint2.northing += (cos(track.curvePts[rA].heading + glm::PIBy2) * track.nudgeDistance);
        }

        //x2-x1
        double dx = refPoint2.easting - refPoint1.easting;
        //z2-z1
        double dz = refPoint2.northing - refPoint1.northing;

        //how far are we away from the reference line at 90 degrees - 2D cross product and distance
        distanceFromRefLine = ((dz * vehicle.guidanceLookPos.easting) -
                               (dx * vehicle.guidanceLookPos.northing) +
                               (refPoint2.easting * refPoint1.northing) -
                               (refPoint2.northing * refPoint1.easting))
                              / sqrt((dz * dz) + (dx * dx));

        distanceFromRefLine -= (0.5 * widthMinusOverlap);

        double RefDist = (distanceFromRefLine +
                          (isHeadingSameWay ? tool_offset : -tool_offset)) / widthMinusOverlap;

        if (RefDist < 0) howManyPathsAway = (int)(RefDist - 0.5);
        else howManyPathsAway = (int)(RefDist + 0.5);

        if (track.mode != (int)TrackMode::bndCurve)
        {

            //build current list
            isCurveValid = true;

            if (howManyPathsAway == lastHowManyPathsAway)
            {
                return;
            }

            lastHowManyPathsAway = howManyPathsAway;

            //build the current line
            curList.clear();

            double distAway = widthMinusOverlap * howManyPathsAway +
                              (isHeadingSameWay ? -tool_offset : tool_offset);

            distAway += (0.5 * widthMinusOverlap);

            if (howManyPathsAway > -1) howManyPathsAway += 1;

            double step = widthMinusOverlap * 0.48;
            if (step > 4) step = 4;
            if (step < 1) step = 1;

            double distSqAway = (distAway * distAway) - 0.01;

            Vec3 point;
            for (int i = 0; i < refCount; i++)
            {
                point = Vec3(
                    track.curvePts[i].easting + (sin(glm::PIBy2 + track.curvePts[i].heading) * distAway),
                    track.curvePts[i].northing + (cos(glm::PIBy2 + track.curvePts[i].heading) * distAway),
                    track.curvePts[i].heading);
                bool Add = true;

                for (int t = 0; t < refCount; t++)
                {
                    double dist = ((point.easting - track.curvePts[t].easting) *
                                   (point.easting - track.curvePts[t].easting))
                                  + ((point.northing - track.curvePts[t].northing) *
                                     (point.northing - track.curvePts[t].northing));
                    if (dist < distSqAway)
                    {
                        Add = false;
                        break;
                    }
                }

                if (Add)
                {
                    if (curList.count() > 0)
                    {
                        double dist = ((point.easting - curList[curList.count() - 1].easting) *
                                       (point.easting - curList[curList.count() - 1].easting))
                                      + ((point.northing - curList[curList.count() - 1].northing) *
                                         (point.northing - curList[curList.count() - 1].northing));
                        if (dist > 2)
                            curList.append(point);
                    }
                    else curList.append(point);
                }
            }

            int cnt = curList.count();
            if (cnt > 6)
            {
                QVector<Vec3> arr(curList);

                curList.clear();

                for (int i = 0; i < (arr.count() - 1); i++)
                {
                    arr[i].heading = atan2(arr[i + 1].easting - arr[i].easting,
                                           arr[i + 1].northing - arr[i].northing);
                    if (arr[i].heading < 0) arr[i].heading += glm::twoPI;
                    if (arr[i].heading >= glm::twoPI) arr[i].heading -= glm::twoPI;
                }

                arr[arr.length() - 1].heading = arr[arr.length() - 2].heading;

                cnt = arr.length();
                double distance;

                //add the first point of loop - it will be p1
                curList.append(arr[0]);
                //curList.append(arr[1]);

                for (int i = 0; i < cnt - 3; i++)
                {
                    // add p1
                    curList.append(arr[i + 1]);

                    distance = glm::Distance(arr[i + 1], arr[i + 2]);

                    if (distance > step)
                    {
                        int loopTimes = (int)(distance / step + 1);
                        for (int j = 1; j < loopTimes; j++)
                        {
                            Vec3 pos(glm::Catmull(j / (double)(loopTimes), arr[i], arr[i + 1], arr[i + 2], arr[i + 3]));
                            curList.append(pos);
                        }
                    }
                }

                curList.append(arr[cnt - 2]);
                curList.append(arr[cnt - 1]);

                //to calc heading based on next and previous points to give an average heading.
                cnt = curList.count();
                arr = QVector<Vec3>(curList);
                cnt--;
                curList.clear();

                curList.append(Vec3(arr[0]));

                //middle points
                for (int i = 1; i < cnt; i++)
                {
                    Vec3 pt3(arr[i]);
                    pt3.heading = atan2(arr[i + 1].easting - arr[i - 1].easting,
                                        arr[i + 1].northing - arr[i - 1].northing);
                    if (pt3.heading < 0) pt3.heading += glm::twoPI;
                    curList.append(pt3);
                }

                int k = arr.length() - 1;
                Vec3 pt33(arr[k]);
                pt33.heading = atan2(arr[k].easting - arr[k - 1].easting,
                                     arr[k].northing - arr[k - 1].northing);
                if (pt33.heading < 0) pt33.heading += glm::twoPI;
                curList.append(pt33);

                if (bnd.bndList.count() > 0 && !(track.mode == (int)TrackMode::bndCurve))
                {
                    int ptCnt = curList.count() - 1;

                    bool isAdding = false;
                    //end
                    while (glm::IsPointInPolygon(bnd.bndList[0].fenceLineEar,curList[curList.count() - 1]))
                    {
                        isAdding = true;
                        for (int i = 1; i < 10; i++)
                        {
                            Vec3 pt(curList[ptCnt]);
                            pt.easting += (sin(pt.heading) * i);
                            pt.northing += (cos(pt.heading) * i);
                            curList.append(pt);
                        }
                        ptCnt = curList.count() - 1;
                    }

                    if (isAdding)
                    {
                        Vec3 pt(curList[curList.count() - 1]);
                        for (int i = 1; i < 5; i++)
                        {
                            pt.easting += (sin(pt.heading) * 2);
                            pt.northing += (cos(pt.heading) * 2);
                            curList.append(pt);
                        }
                    }

                    isAdding = false;

                    //and the beginning
                    pt33 = Vec3(curList[0]);

                    while (glm::IsPointInPolygon(bnd.bndList[0].fenceLineEar,curList[0]))
                    {
                        isAdding = true;
                        pt33 = Vec3(curList[0]);

                        for (int i = 1; i < 10; i++)
                        {
                            Vec3 pt(pt33);
                            pt.easting -= (sin(pt.heading) * i);
                            pt.northing -= (cos(pt.heading) * i);
                            curList.insert(0, pt);
                        }
                    }

                    if (isAdding)
                    {
                        Vec3 pt(curList[0]);
                        for (int i = 1; i < 5; i++)
                        {
                            pt.easting -= (sin(pt.heading) * 2);
                            pt.northing -= (cos(pt.heading) * 2);
                            curList.insert(0, pt);
                        }
                    }

                }
            }
        }
        else
        {
            //is boundary curve - use task
            if (isReady)
            {
                curList.clear();
                isReady = false;
            }

            //build current list
            isCurveValid = true;

            if (howManyPathsAway == lastHowManyPathsAway)
            {
                return;
            }

            lastHowManyPathsAway = howManyPathsAway;

            //build the current line
            //curList?.Clear();

            double distAway = (tool_width - tool_overlap) * howManyPathsAway + (isHeadingSameWay ? -tool_offset : tool_offset) + track.nudgeDistance;

            if (howManyPathsAway > -1) howManyPathsAway += 1;

            distAway += (0.5 * (tool_width - tool_overlap));

            if (!isBusyWorking) auto result = QtConcurrent::run([this, distAway, refCount, &track, &bnd] () { BuildNewCurveAsync( distAway, refCount, track, bnd); }  ) ;
        }
    }
    else //pivot guide list
    {
        //pivot circle center
        refPoint1 = track.ptA;

        //cross product
        isHeadingSameWay = ((vehicle.pivotAxlePos.easting - refPoint1.easting) * (vehicle.steerAxlePos.northing - refPoint1.northing)
                            - (vehicle.pivotAxlePos.northing - refPoint1.northing) * (vehicle.steerAxlePos.easting - refPoint1.easting)) < 0;

        //how far are we away from the reference line at 90 degrees - 2D cross product and distance
        distanceFromRefLine = glm::Distance(vehicle.guidanceLookPos, refPoint1);

        distanceFromRefLine -= (0.5 * widthMinusOverlap);

        double RefDist = (distanceFromRefLine
                          + (isHeadingSameWay ? tool_offset : -tool_offset)
                          + track.nudgeDistance) / widthMinusOverlap;

        if (RefDist < 0) howManyPathsAway = (int)(RefDist - 0.5);
        else howManyPathsAway = (int)(RefDist + 0.5);

        //build current list
        isCurveValid = true;

        //build the current line
        curList.clear();

        double distAway = widthMinusOverlap * howManyPathsAway
                          + (isHeadingSameWay ? -tool_offset : tool_offset) - track.nudgeDistance;

        distAway += (0.5 * widthMinusOverlap);

        if (howManyPathsAway > -1) howManyPathsAway += 1;

        double pointSpacing = distAway * 0.05;

        //distAway += mf.trk.gArr[trk.idx].nudgeDistance;

        Vec3 currentPos(refPoint1.easting-distAway, refPoint1.northing, 0);

        while (currentPos.heading < glm::twoPI)
        {
            //Update the position of the car
            currentPos.easting += pointSpacing * sin(currentPos.heading);
            currentPos.northing += pointSpacing * cos(currentPos.heading);

            //Update the heading
            currentPos.heading += (pointSpacing / distAway);

            //Add the new coordinate to the path
            curList.append(currentPos);
        }

        QVector<Vec3> arr(curList);
        curList.clear();

        for (int i = 0; i < (arr.count() - 1); i++)
        {
            arr[i].heading = atan2(arr[i + 1].easting - arr[i].easting, arr[i + 1].northing - arr[i].northing);
            if (arr[i].heading < 0) arr[i].heading += glm::twoPI;
            if (arr[i].heading >= glm::twoPI) arr[i].heading -= glm::twoPI;
        }

        arr[arr.count() - 1].heading = arr[arr.count() - 2].heading;

        curList.append(arr);
    }

    lastSecond = secondsSinceStart;

}

void CABCurve::BuildNewCurveAsync(double distAway,
                                  int refCount,
                                  const CTrk &track,
                                  const CBoundary &bnd)
{
    //was an async task in C#, using QtConcurrent::run here
    isBusyWorking = true;
    isReady = false;

    newCurList.clear();

    double tool_width = property_setVehicle_toolWidth;
    double tool_overlap = property_setVehicle_toolOverlap;
    double step = (tool_width - tool_overlap) * 0.48;
    if (step > 4) step = 4;
    if (step < 1) step = 1;

    double distSqAway = (distAway * distAway) - 0.01;

    Vec3 point;

    //try
    for (int i = 0; i < refCount; i++)
    {
        point = Vec3(
            track.curvePts[i].easting + (sin(glm::PIBy2 + track.curvePts[i].heading) * distAway),
            track.curvePts[i].northing + (cos(glm::PIBy2 + track.curvePts[i].heading) * distAway),
            track.curvePts[i].heading);
        bool Add = true;

        for (int t = 0; t < refCount; t++)
        {
            double dist = ((point.easting - track.curvePts[t].easting) * (point.easting - track.curvePts[t].easting))
                          + ((point.northing - track.curvePts[t].northing) * (point.northing - track.curvePts[t].northing));
            if (dist < distSqAway)
            {
                Add = false;
                break;
            }
        }

        if (Add)
        {
            if (newCurList.count() > 0)
            {
                double dist = ((point.easting - newCurList[newCurList.count() - 1].easting) * (point.easting - newCurList[newCurList.count() - 1].easting))
                              + ((point.northing - newCurList[newCurList.count() - 1].northing) * (point.northing - newCurList[newCurList.count() - 1].northing));
                if (dist > step)
                    newCurList.append(point);
            }
            else newCurList.append(point);
        }
    }

    int cnt = newCurList.count();
    if (cnt > 6)
    {
        QVector<Vec3> arr(newCurList);
        newCurList.clear();

        for (int i = 0; i < (arr.count() - 1); i++)
        {
            arr[i].heading = atan2(arr[i + 1].easting - arr[i].easting, arr[i + 1].northing - arr[i].northing);
            if (arr[i].heading < 0) arr[i].heading += glm::twoPI;
            if (arr[i].heading >= glm::twoPI) arr[i].heading -= glm::twoPI;
        }

        arr[arr.count() - 1].heading = arr[arr.count() - 2].heading;

        cnt = arr.count();
        double distance;

        //add the first point of loop - it will be p1
        newCurList.append(arr[0]);
        //newCurList.append(arr[1]);

        for (int i = 0; i < cnt - 3; i++)
        {
            // add p1
            newCurList.append(arr[i + 1]);

            distance = glm::Distance(arr[i + 1], arr[i + 2]);

            if (distance > step)
            {
                int loopTimes = (int)(distance / step + 1);
                for (int j = 1; j < loopTimes; j++)
                {
                    Vec3 pos = Vec3(glm::Catmull(j / (double)(loopTimes), arr[i], arr[i + 1], arr[i + 2], arr[i + 3]));
                    newCurList.append(pos);
                }
            }
        }

        newCurList.append(arr[cnt - 2]);
        newCurList.append(arr[cnt - 1]);

        //to calc heading based on next and previous points to give an average heading.
        cnt = newCurList.count();
        arr = newCurList;
        cnt--;
        newCurList.clear();

        newCurList.append(Vec3(arr[0]));

        //middle points
        for (int i = 1; i < cnt; i++)
        {
            Vec3 pt3 = Vec3(arr[i]);
            pt3.heading = atan2(arr[i + 1].easting - arr[i - 1].easting, arr[i + 1].northing - arr[i - 1].northing);
            if (pt3.heading < 0) pt3.heading += glm::twoPI;
            newCurList.append(pt3);
        }

        int k = arr.count() - 1;
        Vec3 pt33 = Vec3(arr[k]);
        pt33.heading = atan2(arr[k].easting - arr[k - 1].easting, arr[k].northing - arr[k - 1].northing);
        if (pt33.heading < 0) pt33.heading += glm::twoPI;
        newCurList.append(pt33);

        if (track.curvePts.count() == 0)
        {
            isReady = false;
            isBusyWorking = false;
            return;
        }

        if (bnd.bndList.count() > 0 && !(track.mode == (int)TrackMode::bndCurve))
        {
            int ptCnt = newCurList.count() - 1;

            bool isAdding = false;
            //end
            while (glm::IsPointInPolygon(bnd.bndList[0].fenceLineEar,newCurList[newCurList.count() - 1]))
            {
                isAdding = true;
                for (int i = 1; i < 10; i++)
                {
                    Vec3 pt = Vec3(newCurList[ptCnt]);
                    pt.easting += (sin(pt.heading) * i * 2);
                    pt.northing += (cos(pt.heading) * i * 2);
                    newCurList.append(pt);
                }
                ptCnt = newCurList.count() - 1;
            }

            if (isAdding)
            {
                Vec3 pt = Vec3(newCurList[newCurList.count() - 1]);
                for (int i = 1; i < 5; i++)
                {
                    pt.easting += (sin(pt.heading) * 2);
                    pt.northing += (cos(pt.heading) * 2);
                    newCurList.append(pt);
                }
            }

            isAdding = false;

            //and the beginning
            pt33 = Vec3(newCurList[0]);

            while (glm::IsPointInPolygon(bnd.bndList[0].fenceLineEar,newCurList[0]))
            {
                isAdding = true;
                pt33 = Vec3(newCurList[0]);

                for (int i = 1; i < 10; i++)
                {
                    Vec3 pt = Vec3(pt33);
                    pt.easting -= (sin(pt.heading) * i * 2);
                    pt.northing -= (cos(pt.heading) * i * 2);
                    newCurList.insert(0, pt);
                }
            }

            if (isAdding)
            {
                Vec3 pt = Vec3(newCurList[0]);
                for (int i = 1; i < 5; i++)
                {
                    pt.easting -= (sin(pt.heading) * 2);
                    pt.northing -= (cos(pt.heading) * 2);
                    newCurList.insert(0, pt);
                }
            }
        }

        isReady = true;
    }
    //catch (Exception)

    isBusyWorking = false;

}

void CABCurve::GetCurrentCurveLine(Vec3 pivot,
                                   Vec3 steer,
                                   bool isAutoSteerBtnOn,
                                   CVehicle &vehicle,
                                   CTrk &track,
                                   CYouTurn &yt,
                                   const CAHRS &ahrs,
                                   CGuidance &gyd,
                                   CNMEA &pn,
                                   int &makeUTurnCounter)
{
    double purePursuitGain = property_purePursuitIntegralGainAB;
    double wheelBase = property_setVehicle_wheelbase;
    double maxSteerAngle = property_setVehicle_maxSteerAngle;

    if (track.curvePts.count() == 0 || track.curvePts.count() < 5)
    {
        if (track.mode != (int)TrackMode::waterPivot)
        {
            return;
        }
    }

    double dist, dx, dz;
    double minDistA = 1000000, minDistB = 1000000;
    //int ptCount = curList.count();

    if (curList.count() > 0)
    {
        if (yt.isYouTurnTriggered && yt.DistanceFromYouTurnLine(vehicle,pn,makeUTurnCounter))//do the pure pursuit from youTurn
        {
            //now substitute what it thinks are AB line values with auto turn values
            steerAngleCu = yt.steerAngleYT;
            distanceFromCurrentLinePivot = yt.distanceFromCurrentLine;

            goalPointCu = yt.goalPointYT;
            radiusPointCu.easting = yt.radiusPointYT.easting;
            radiusPointCu.northing = yt.radiusPointYT.northing;
            ppRadiusCu = yt.ppRadiusYT;
            vehicle.modeActualXTE = (distanceFromCurrentLinePivot);
        }
        else if (property_setVehicle_isStanleyUsed)//Stanley
        {
            gyd.StanleyGuidanceCurve(pivot, steer, curList, isAutoSteerBtnOn, vehicle, *this, ahrs);
        }
        else// Pure Pursuit ------------------------------------------
        {
            minDistA = glm::DOUBLE_MAX;
            //close call hit

            //If is a curve
            if (track.mode <= (int)TrackMode::Curve)
            {
                minDistA = minDistB = glm::DOUBLE_MAX;
                //close call hit
                int cc = 0, dd;

                for (int j = 0; j < curList.count(); j += 10)
                {
                    dist = glm::DistanceSquared(pivot, curList[j]);
                    if (dist < minDistA)
                    {
                        minDistA = dist;
                        cc = j;
                    }
                }

                minDistA = glm::DOUBLE_MAX;

                dd = cc + 8;
                if (dd > curList.count() - 1) dd = curList.count();
                cc -= 8;
                if (cc < 0) cc = 0;

                //find the closest 2 points to current close call
                for (int j = cc; j < dd; j++)
                {
                    dist = glm::DistanceSquared(pivot, curList[j]);
                    if (dist < minDistA)
                    {
                        minDistB = minDistA;
                        B = A;
                        minDistA = dist;
                        A = j;
                    }
                    else if (dist < minDistB)
                    {
                        minDistB = dist;
                        B = j;
                    }
                }

                //just need to make sure the points continue ascending or heading switches all over the place
                if (A > B) {
                    C = A;
                    A = B;
                    B = C;
                }

                currentLocationIndex = A;

                //get the distance from currently active AB line

                if (A > curList.count() - 1 || B > curList.count() - 1)
                    return;
            }
            else
            {
                for (int j = 0; j < curList.count(); j++)
                {
                    dist = glm::DistanceSquared(pivot, curList[j]);
                    if (dist < minDistA)
                    {
                        minDistA = dist;
                        A = j;
                    }
                }

                currentLocationIndex = A;

                if (A > curList.count() - 1)
                    return;

                //initial forward Test if pivot InRange AB
                if (A == curList.count() - 1) B = 0;
                else B = A + 1;

                if (glm::InRangeBetweenAB(curList[A].easting, curList[A].northing,
                                         curList[B].easting, curList[B].northing, pivot.easting, pivot.northing))
                    goto SegmentFound;

                A = currentLocationIndex;
                //step back one
                if (A == 0)
                {
                    A = curList.count() - 1;
                    B = 0;
                }
                else
                {
                    A = A - 1;
                    B = A + 1;
                }

                if (glm::InRangeBetweenAB(curList[A].easting, curList[A].northing,
                                         curList[B].easting, curList[B].northing, pivot.easting, pivot.northing))
                    goto SegmentFound;

                //realy really lost
                return;

            }

            SegmentFound:

            //get the distance from currently active AB line

            dx = curList[B].easting - curList[A].easting;
            dz = curList[B].northing - curList[A].northing;

            if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dz) < glm::DOUBLE_EPSILON) return;

            //abHeading = atan2(dz, dx);

            //how far from current AB Line is fix
            distanceFromCurrentLinePivot = ((dz * pivot.easting) - (dx * pivot.northing) + (curList[B].easting
                                                                                            * curList[A].northing) - (curList[B].northing * curList[A].easting))
                                           / sqrt((dz * dz) + (dx * dx));

            //integral slider is set to 0
            if (purePursuitGain != 0 && !vehicle.isReverse)
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

                if (isAutoSteerBtnOn && vehicle.avgSpeed > 2.5 && fabs(pivotDerivative) < 0.1)
                {
                    //if over the line heading wrong way, rapidly decrease integral
                    if ((inty < 0 && distanceFromCurrentLinePivot < 0) || (inty > 0 && distanceFromCurrentLinePivot > 0))
                    {
                        inty += pivotDistanceError * purePursuitGain * -0.04;
                    }
                    else
                    {
                        if (fabs(distanceFromCurrentLinePivot) > 0.02)
                        {
                            inty += pivotDistanceError * purePursuitGain * -0.02;
                            if (inty > 0.2) inty = 0.2;
                            else if (inty < -0.2) inty = -0.2;
                        }
                    }
                }
                else inty *= 0.95;
            }
            else inty = 0;

            // ** Pure pursuit ** - calc point on ABLine closest to current position
            double U = (((pivot.easting - curList[A].easting) * dx)
                        + ((pivot.northing - curList[A].northing) * dz))
                       / ((dx * dx) + (dz * dz));

            rEastCu = curList[A].easting + (U * dx);
            rNorthCu = curList[A].northing + (U * dz);
            manualUturnHeading = curList[A].heading;

            //update base on autosteer settings and distance from line
            double goalPointDistance = vehicle.UpdateGoalPointDistance();

            bool ReverseHeading = vehicle.isReverse ? !isHeadingSameWay : isHeadingSameWay;

            int count = ReverseHeading ? 1 : -1;
            Vec3 start(rEastCu, rNorthCu, 0);
            double distSoFar = 0;

            for (int i = ReverseHeading ? B : A; i < curList.count() && i >= 0; i += count)
            {
                // used for calculating the length squared of next segment.
                double tempDist = glm::Distance(start, curList[i]);

                //will we go too far?
                if ((tempDist + distSoFar) > goalPointDistance)
                {
                    double j = (goalPointDistance - distSoFar) / tempDist; // the remainder to yet travel

                    goalPointCu.easting = (((1 - j) * start.easting) + (j * curList[i].easting));
                    goalPointCu.northing = (((1 - j) * start.northing) + (j * curList[i].northing));
                    break;
                }
                else distSoFar += tempDist;
                start = curList[i];
                i += count;
                if (i < 0) i = curList.count() - 1;
                if (i > curList.count() - 1) i = 0;
            }

            if (track.mode <= (int)TrackMode::Curve)
            {
                if (isAutoSteerBtnOn && !vehicle.isReverse)
                {
                    if (isHeadingSameWay)
                    {
                        if (glm::Distance(goalPointCu, curList[(curList.count() - 1)]) < 0.5)
                        {
                            emit TimedMessage(2000,tr("Guidance Stopped"), tr("Past end of curve"));
                            emit stopAutoSteer();
                        }
                    }
                    else
                    {
                        if (glm::Distance(goalPointCu, curList[0]) < 0.5)
                        {
                            emit TimedMessage(2000,tr("Guidance Stopped"), tr("Past end of curve"));
                            emit stopAutoSteer();
                        }
                    }
                }
            }

            //calc "D" the distance from pivot axle to lookahead point
            double goalPointDistanceSquared = glm::DistanceSquared(goalPointCu.northing, goalPointCu.easting, pivot.northing, pivot.easting);

            //calculate the the delta x in local coordinates and steering angle degrees based on wheelbase
            //double localHeading = glm::twoPI - mf.fixHeading;

            double localHeading;
            if (ReverseHeading) localHeading = glm::twoPI - vehicle.fixHeading + inty;
            else localHeading = glm::twoPI - vehicle.fixHeading - inty;

            ppRadiusCu = goalPointDistanceSquared / (2 * (((goalPointCu.easting - pivot.easting) * cos(localHeading)) + ((goalPointCu.northing - pivot.northing) * sin(localHeading))));

            steerAngleCu = glm::toDegrees(atan(2 * (((goalPointCu.easting - pivot.easting) * cos(localHeading))
                                                        + ((goalPointCu.northing - pivot.northing) * sin(localHeading))) * wheelBase / goalPointDistanceSquared));

            if (ahrs.imuRoll != 88888)
                steerAngleCu += ahrs.imuRoll * -(double)property_setAS_sideHillComp; /* gyd.sideHillCompFactor*/

            if (steerAngleCu < -maxSteerAngle) steerAngleCu = -maxSteerAngle;
            if (steerAngleCu > maxSteerAngle) steerAngleCu = maxSteerAngle;

            if (!isHeadingSameWay)
                distanceFromCurrentLinePivot *= -1.0;

            //used for acquire/hold mode
            vehicle.modeActualXTE = (distanceFromCurrentLinePivot);

            double steerHeadingError = (pivot.heading - curList[A].heading);
            //Fix the circular error
            if (steerHeadingError > M_PI)
                steerHeadingError -= M_PI;
            else if (steerHeadingError < -M_PI)
                steerHeadingError += M_PI;

            if (steerHeadingError > glm::PIBy2)
                steerHeadingError -= M_PI;
            else if (steerHeadingError < -glm::PIBy2)
                steerHeadingError += M_PI;

            vehicle.modeActualHeadingError = glm::toDegrees(steerHeadingError);

            //Convert to centimeters
            vehicle.guidanceLineDistanceOff = (short)glm::roundMidAwayFromZero(distanceFromCurrentLinePivot * 1000.0);
            vehicle.guidanceLineSteerAngle = (short)(steerAngleCu * 100);
        }
    }
    else
    {
        //invalid distance so tell AS module
        distanceFromCurrentLinePivot = 32000;
        vehicle.guidanceLineDistanceOff = 32000;
    }
}

void CABCurve::DrawCurveNew(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    GLHelperOneColor gldraw;

    for (int h = 0; h < desList.count(); h++) gldraw.append(QVector3D(desList[h].easting, desList[h].northing, 0));

    gldraw.draw(gl, mvp, QColor::fromRgbF(0.95f, 0.42f, 0.750f), GL_LINE_STRIP, 4.0f); //TODO is 4 pixels right?  check main form call
}

void CABCurve::DrawCurve(QOpenGLFunctions *gl, const QMatrix4x4 &mvp,
                         bool isFontOn,
                         const CTrk &track,
                         CYouTurn &yt, const CCamera &camera)
{
    //double tool_toolWidth = property_setVehicle_toolWidth;
    //double tool_toolOverlap = property_setVehicle_toolOverlap;


    GLHelperColors gldraw_colors;
    GLHelperOneColor gldraw;
    ColorVertex cv;
    QColor color;

    double lineWidth = property_setDisplay_lineWidth;

    if (desList.count() > 0)
    {
        for (int h = 0; h < desList.count(); h++)
            gldraw.append(QVector3D(desList[h].easting, desList[h].northing, 0));

        gldraw.draw(gl,mvp,QColor::fromRgbF(0.95f, 0.42f, 0.750f),GL_LINE_STRIP,lineWidth);
    }

    int ptCount = track.curvePts.count();
    if (track.mode != TrackMode::waterPivot)
    {
        if (track.curvePts.count() == 0) return;

        gldraw.clear();
        //cv.color = QVector4D(0.96, 0.2f, 0.2f, 1.0f);
        for (int h = 0; h < ptCount; h++) {
            gldraw.append(QVector3D(track.curvePts[h].easting, track.curvePts[h].northing, 0));
        }

        gldraw.draw(gl,mvp,QColor::fromRgbF(0.96, 0.2f, 0.2f), GL_LINES, 4.0);

        if (isFontOn)
        {
            color.setRgbF(0.40f, 0.90f, 0.95f);
            drawText3D(camera, gl, mvp, track.curvePts[0].easting, track.curvePts[0].northing, "&A", 1.0, true, color);
            drawText3D(camera, gl, mvp, track.curvePts[track.curvePts.count() - 1].easting, track.curvePts[track.curvePts.count() - 1].northing, "&B", 1.0, true, color);
        }

        //just draw ref and smoothed line if smoothing window is open
        if (isSmoothWindowOpen)
        {
            if (smooList.count() == 0) return;

            gldraw.clear();

            for (int h = 0; h < smooList.count(); h++)
                gldraw.append(QVector3D(smooList[h].easting, smooList[h].northing, 0));

            gldraw.draw(gl,mvp,QColor::fromRgbF(0.930f, 0.92f, 0.260f),GL_LINES,lineWidth);
        }
        else //normal. Smoothing window is not open.
        {
            if (curList.count() > 0)
            {
                color.setRgbF(0.95f, 0.2f, 0.95f);
                gldraw.clear();

                for (int h = 0; h < curList.count(); h++)
                    gldraw.append(QVector3D(curList[h].easting, curList[h].northing, 0));

                //ablines and curves are a line - the rest a loop

                if(track.mode <= (int)TrackMode::Curve)
                    gldraw.draw(gl,mvp,color,GL_LINE_STRIP,lineWidth);
                else
                    gldraw.draw(gl,mvp,color,GL_LINE_LOOP,lineWidth);

                if (!(bool)property_setVehicle_isStanleyUsed && camera.camSetDistance > -200)
                {
                    gldraw.clear();
                    //Draw lookahead Point
                    color.setRgbF(1.0f, 0.95f, 0.195f);

                    gldraw.append(QVector3D(goalPointCu.easting, goalPointCu.northing, 0.0));
                    gldraw.draw(gl,mvp,color,GL_POINTS,8.0f);
                }

                yt.DrawYouTurn(gl,mvp);

                gldraw.clear();
                for (int h = 0; h < curList.count(); h++)
                    gldraw.append(QVector3D(curList[h].easting, curList[h].northing, 0));

                gldraw.draw(gl, mvp, QColor::fromRgbF(0.920f, 0.6f, 0.950f), GL_POINTS, 3.0f);
            }
        }
    }

    else
    {
        if (curList.count() > 0)
        {
            gldraw.clear();
            gldraw.append(QVector3D(track.ptA.easting, track.ptA.northing, 0));
            for (int h = 0; h < curList.count(); h++)
                gldraw.append(QVector3D(curList[h].easting, curList[h].northing, 0));

            gldraw.draw(gl, mvp, QColor::fromRgbF(0.95f, 0.2f, 0.95f),GL_LINE_STRIP, lineWidth);

            if (!(bool)property_setVehicle_isStanleyUsed && camera.camSetDistance > -200)
            {
                //Draw lookahead Point
                gldraw.clear();
                gldraw.append(QVector3D(goalPointCu.easting, goalPointCu.northing, 0.0));
                gldraw.draw(gl, mvp, QColor::fromRgbF(1.0f, 0.95f, 0.195f), GL_POINTS, 8.0f);
            }
        }
    }
}

void CABCurve::BuildTram(CBoundary &bnd, CTram &tram, const CTrk &track)
{
    double halfWheelTrack = (double)property_setVehicle_trackWidth * 0.5;

    //if all or bnd only then make outer loop pass
    if (tram.generateMode != 1)
    {
        tram.BuildTramBnd(bnd);
    }
    else
    {
        tram.tramBndOuterArr.clear();
        tram.tramBndInnerArr.clear();
    }

    tram.tramList.clear();
    tram.tramArr->clear();

    if (tram.generateMode == 2) return;

    bool isBndExist = bnd.bndList.count() != 0;

    int refCount = track.curvePts.count();

    int cntr = 0;
    if (isBndExist)
    {
        if (tram.generateMode == 1)
            cntr = 0;
        else
            cntr = 1;
    }
    double widd = 0;

    for (int i = cntr; i <= (int)property_setTram_passes; i++)
    {
        tram.tramArr = QSharedPointer<QVector<Vec2>>(new QVector<Vec2>());
        tram.tramList.append(tram.tramArr);

        widd = (double)property_setTram_tramWidth * 0.5 - halfWheelTrack;
        widd += ((double)property_setTram_tramWidth * i);

        double distSqAway = widd * widd * 0.999999;

        for (int j = 0; j < refCount; j += 1)
        {
            Vec2 point(
                (sin(glm::PIBy2 + track.curvePts[j].heading) *
                 widd) + track.curvePts[j].easting,
                (cos(glm::PIBy2 + track.curvePts[j].heading) *
                 widd) + track.curvePts[j].northing
                );

            bool Add = true;
            for (int t = 0; t < refCount; t++)
            {
                //distance check to be not too close to ref line
                double dist = ((point.easting - track.curvePts[t].easting) * (point.easting - track.curvePts[t].easting))
                              + ((point.northing - track.curvePts[t].northing) * (point.northing - track.curvePts[t].northing));
                if (dist < distSqAway)
                {
                    Add = false;
                    break;
                }
            }
            if (Add)
            {
                //a new point only every 2 meters
                double dist = (tram.tramArr->count() > 0 ? ((point.easting - (*tram.tramArr)[tram.tramArr->count() - 1].easting) * (point.easting - (*tram.tramArr)[tram.tramArr->count() - 1].easting))
                                                               + ((point.northing - (*tram.tramArr)[tram.tramArr->count() - 1].northing) * (point.northing - (*tram.tramArr)[tram.tramArr->count() - 1].northing)) : 3.0);
                if (dist > 2)
                {
                    //if inside the boundary, add
                    if (!isBndExist || glm::IsPointInPolygon(bnd.bndList[0].fenceLineEar,point))
                    {
                        tram.tramArr->append(point);
                    }
                }
            }
        }
    }

    for (int i = cntr; i <= (int)property_setTram_passes; i++)
    {
        tram.tramArr = QSharedPointer<QVector<Vec2>>(new QVector<Vec2>());
        tram.tramList.append(tram.tramArr);

        widd = (double)property_setTram_tramWidth * 0.5 + halfWheelTrack;
        widd += ((double)property_setTram_tramWidth * i);

        double distSqAway = widd * widd * 0.999999;

        for (int j = 0; j < refCount; j += 1)
        {
            Vec2 point(
                sin(glm::PIBy2 + track.curvePts[j].heading) *
                        widd + track.curvePts[j].easting,
                cos(glm::PIBy2 + track.curvePts[j].heading) *
                        widd + track.curvePts[j].northing
                );

            bool Add = true;
            for (int t = 0; t < refCount; t++)
            {
                //distance check to be not too close to ref line
                double dist = ((point.easting - track.curvePts[t].easting) * (point.easting - track.curvePts[t].easting))
                              + ((point.northing - track.curvePts[t].northing) * (point.northing - track.curvePts[t].northing));
                if (dist < distSqAway)
                {
                    Add = false;
                    break;
                }
            }
            if (Add)
            {
                //a new point only every 2 meters
                double dist = tram.tramArr->count() > 0 ? ((point.easting - (*tram.tramArr)[tram.tramArr->count() - 1].easting) * (point.easting - (*tram.tramArr)[tram.tramArr->count() - 1].easting))
                                                              + ((point.northing - (*tram.tramArr)[tram.tramArr->count() - 1].northing) * (point.northing - (*tram.tramArr)[tram.tramArr->count() - 1].northing)) : 3.0;
                if (dist > 2)
                {
                    //if inside the boundary, add
                    if (!isBndExist || glm::IsPointInPolygon(bnd.bndList[0].fenceLineEar,point))
                    {
                        tram.tramArr->append(point);
                    }
                }
            }
        }
    }
}

void CABCurve::SmoothAB(int smPts, const CTrk &track)
{
    //count the reference list of original curve
    int cnt = track.curvePts.count();

    //just go back if not very long
    if (cnt < 200) return;

    //the temp array
    Vec3 *arr = new Vec3[cnt];

    //read the points before and after the setpoint
    for (int s = 0; s < smPts / 2; s++)
    {
        arr[s].easting = track.curvePts[s].easting;
        arr[s].northing = track.curvePts[s].northing;
        arr[s].heading = track.curvePts[s].heading;
    }

    for (int s = cnt - (smPts / 2); s < cnt; s++)
    {
        arr[s].easting = track.curvePts[s].easting;
        arr[s].northing = track.curvePts[s].northing;
        arr[s].heading = track.curvePts[s].heading;
    }

    //average them - center weighted average
    for (int i = smPts / 2; i < cnt - (smPts / 2); i++)
    {
        for (int j = -smPts / 2; j < smPts / 2; j++)
        {
            arr[i].easting += track.curvePts[j + i].easting;
            arr[i].northing += track.curvePts[j + i].northing;
        }
        arr[i].easting /= smPts;
        arr[i].northing /= smPts;
        arr[i].heading = track.curvePts[i].heading;
    }

    //make a list to draw
    smooList.clear();
    for (int i = 0; i < cnt; i++)
    {
        smooList.append(arr[i]);
    }

    delete[] arr;
}

void CABCurve::SmoothABDesList(int smPts)
{
    //count the reference list of original curve
    int cnt = desList.count();

    //the temp array
    //the temp array
    Vec3 *arr = new Vec3[cnt];

    //read the points before and after the setpoint
    for (int s = 0; s < smPts / 2; s++)
    {
        arr[s].easting = desList[s].easting;
        arr[s].northing = desList[s].northing;
        arr[s].heading = desList[s].heading;
    }

    for (int s = cnt - (smPts / 2); s < cnt; s++)
    {
        arr[s].easting = desList[s].easting;
        arr[s].northing = desList[s].northing;
        arr[s].heading = desList[s].heading;
    }

    //average them - center weighted average
    for (int i = smPts / 2; i < cnt - (smPts / 2); i++)
    {
        for (int j = -smPts / 2; j < smPts / 2; j++)
        {
            arr[i].easting += desList[j + i].easting;
            arr[i].northing += desList[j + i].northing;
        }
        arr[i].easting /= smPts;
        arr[i].northing /= smPts;
        arr[i].heading = desList[i].heading;
    }

    //make a list to draw
    desList.clear();
    for (int i = 0; i < cnt; i++)
    {
        desList.append(arr[i]);
    }

    delete[] arr;
}

void CABCurve::CalculateHeadings(QVector<Vec3> &xList)
{
    //to calc heading based on next and previous points to give an average heading.
    int cnt = xList.count();
    if (cnt > 3)
    {
        QVector<Vec3> arr = xList;
        cnt--;
        xList.clear();

        Vec3 pt3(arr[0]);
        pt3.heading = atan2(arr[1].easting - arr[0].easting, arr[1].northing - arr[0].northing);
        if (pt3.heading < 0) pt3.heading += glm::twoPI;
        xList.append(pt3);

        //middle points
        for (int i = 1; i < cnt; i++)
        {
            Vec3 pt3 = arr[i];
            pt3.heading = atan2(arr[i + 1].easting - arr[i - 1].easting, arr[i + 1].northing - arr[i - 1].northing);
            if (pt3.heading < 0) pt3.heading += glm::twoPI;
            xList.append(pt3);
        }
    }
}

void CABCurve::MakePointMinimumSpacing(QVector<Vec3> &xList, double minDistance)
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

void CABCurve::SaveSmoothList(CTrk &track)
{
    //oops no smooth list generated
    int cnt = smooList.size();
    if (cnt == 0) return;

    //eek
    track.curvePts.clear();

    //copy to an array to calculate all the new headings
    QVector<Vec3> arr = smooList;

    //calculate new headings on smoothed line
    for (int i = 1; i < cnt - 1; i++)
    {
        arr[i].heading = atan2(arr[i + 1].easting - arr[i].easting, arr[i + 1].northing - arr[i].northing);
        if (arr[i].heading < 0) arr[i].heading += glm::twoPI;
        track.curvePts.append(arr[i]);
    }
}

bool CABCurve::PointOnLine(Vec3 pt1, Vec3 pt2, Vec3 pt)
{
    Vec2 r(0, 0);
    if (pt1.northing == pt2.northing && pt1.easting == pt2.easting) { pt1.northing -= 0.00001; }

    double U = ((pt.northing - pt1.northing) * (pt2.northing - pt1.northing)) + ((pt.easting - pt1.easting) * (pt2.easting - pt1.easting));

    double Udenom = pow(pt2.northing - pt1.northing, 2) + pow(pt2.easting - pt1.easting, 2);

    U /= Udenom;

    r.northing = pt1.northing + (U * (pt2.northing - pt1.northing));
    r.easting = pt1.easting + (U * (pt2.easting - pt1.easting));

    double minx, maxx, miny, maxy;

    minx = fmin(pt1.northing, pt2.northing);
    maxx = fmax(pt1.northing, pt2.northing);

    miny = fmin(pt1.easting, pt2.easting);
    maxy = fmax(pt1.easting, pt2.easting);
    return (r.northing >= minx && r.northing <= maxx && (r.easting >= miny && r.easting <= maxy));

}

void CABCurve::AddFirstLastPoints(QVector<Vec3> &xList,
                                  const CBoundary &bnd)
{
    int ptCnt = xList.count() - 1;
    Vec3 start(xList[0]);

    if (bnd.bndList.count() > 0)
    {
        //end
        while (glm::IsPointInPolygon(bnd.bndList[0].fenceLineEar, xList[xList.count() - 1]))
        {
            for (int i = 1; i < 10; i++)
            {
                Vec3 pt(xList[ptCnt]);
                pt.easting += (sin(pt.heading) * i);
                pt.northing += (cos(pt.heading) * i);
                xList.append(pt);
            }
            ptCnt = xList.count() - 1;
        }

        //and the beginning
        start = Vec3(xList[0]);

        while (glm::IsPointInPolygon(bnd.bndList[0].fenceLineEar,xList[0]))
        {
            for (int i = 1; i < 10; i++)
            {
                Vec3 pt(start);
                pt.easting -= (sin(pt.heading) * i);
                pt.northing -= (cos(pt.heading) * i);
                xList.insert(0, pt);
            }
            start = Vec3(xList[0]);
        }
    }
    else
    {
        for (int i = 1; i < 300; i++)
        {
            Vec3 pt(xList[ptCnt]);
            pt.easting += (sin(pt.heading) * i);
            pt.northing += (cos(pt.heading) * i);
            xList.append(pt);
        }

        //and the beginning
        start = Vec3(xList[0]);

        for (int i = 1; i < 300; i++)
        {
            Vec3 pt(start);
            pt.easting -= (sin(pt.heading) * i);
            pt.northing -= (cos(pt.heading) * i);
            xList.insert(0, pt);
        }
    }
}
