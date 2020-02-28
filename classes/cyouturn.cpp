#include <limits>
#include <math.h>
#include <QFile>
#include "cyouturn.h"
#include "aogsettings.h"
#include "cabline.h"
#include "cvehicle.h"
#include "cnmea.h"
#include "cabline.h"
#include "cboundary.h"
#include "cabcurve.h"
#include "cturn.h"
#include "cmazegrid.h"
#include "glm.h"
#include "cdubins.h"
#include "glutils.h"
#include "ctool.h"
#include "common.h"
#include "cgeofence.h"


//constructor
CYouTurn::CYouTurn(QObject *parent) : QObject(parent)
{
    USE_SETTINGS;

    triggerDistanceOffset = SETTINGS_VEHICLE_YOUTURNTRIGGERDISTANCE;
    geoFenceDistance = SETTINGS_VEHICLE_GEOFENCEDIST;

    //how far before or after boundary line should turn happen
    youTurnStartOffset = SETTINGS_VEHICLE_YOUTURNDISTANCE;

    //the youturn shape scaling.
    rowSkipsWidth = SETTINGS_VEHICLE_YOUSKIPWIDTH;

    isUsingDubinsTurn = SETTINGS_VEHICLE_ISUSINGDUBINSTURN;

}

//needs CABCurve, CBoundary, called from CYouTurn::buildCurvePatternYouTurn
bool CYouTurn::findCurveTurnPoints(const CABCurve &curve, const CBoundary &bnd, CTurn &turn)
{
    crossingCurvePoint.easting = -20000;
    crossingTurnLinePoint.easting = -20000;

    //find closet AB Curve point that will cross and go out of bounds
    curListCount = curve.curList.size();

    //otherwise we count down
    bool isCountingUp = curve.isABSameAsVehicleHeading;

    //check if outside a border
    if (isCountingUp)
    {
        crossingTurnLinePoint.index = 99;

        //for each point in succession keep going till a turnLine is found.
        for (int j = curve.currentLocationIndex; j < curListCount; j++)
        {
            if (!turn.turnArr[0].isPointInTurnWorkArea(curve.curList[j]))
            {                                        //it passed outer turnLine
                crossingCurvePoint.easting = curve.curList[j - 1].easting;
                crossingCurvePoint.northing = curve.curList[j - 1].northing;
                crossingCurvePoint.heading = curve.curList[j - 1].heading;
                crossingCurvePoint.index = j - 1;
                crossingTurnLinePoint.index = 0;
                goto CrossingFound;
            }

            for (int i = 1; i < bnd.bndArr.size(); i++)
            {
                //make sure not inside a non drivethru boundary
                if (!bnd.bndArr[i].isSet) continue;
                if (bnd.bndArr[i].isDriveThru) continue;
                if (bnd.bndArr[i].isDriveAround) continue;
                if (turn.turnArr[i].isPointInTurnWorkArea(curve.curList[j]))
                {
                    crossingCurvePoint.easting = curve.curList[j - 1].easting;
                    crossingCurvePoint.northing = curve.curList[j - 1].northing;
                    crossingCurvePoint.heading = curve.curList[j - 1].heading;
                    crossingCurvePoint.index = j - 1;
                    crossingTurnLinePoint.index = i;
                    goto CrossingFound;
                }
            }
        }

    //escape for multiple for's
    CrossingFound:;

    }
    else //counting down, going opposite way mf.curve was created.
    {
        crossingTurnLinePoint.index = 99;

        for (int j = curve.currentLocationIndex; j > 0; j--)
        {
            if (!turn.turnArr[0].isPointInTurnWorkArea(curve.curList[j]))
            {                                        //it passed outer turnLine
                crossingCurvePoint.easting = curve.curList[j + 1].easting;
                crossingCurvePoint.northing = curve.curList[j + 1].northing;
                crossingCurvePoint.heading = curve.curList[j + 1].heading;
                crossingCurvePoint.index = j + 1;
                crossingTurnLinePoint.index = 0;
                goto CrossingFound1;
            }

            for (int i = 1; i < bnd.bndArr.size(); i++)
            {
                //make sure not inside a non drivethru boundary
                if (!bnd.bndArr[i].isSet) continue;
                if (bnd.bndArr[i].isDriveThru) continue;
                if (bnd.bndArr[i].isDriveAround) continue;
                if (turn.turnArr[i].isPointInTurnWorkArea(curve.curList[j]))
                {
                    crossingCurvePoint.easting = curve.curList[j].easting;
                    crossingCurvePoint.northing = curve.curList[j].northing;
                    crossingCurvePoint.heading = curve.curList[j].heading;
                    crossingCurvePoint.index = j;
                    crossingTurnLinePoint.index = i;
                    goto CrossingFound1;
                }
            }
        }

    //escape for multiple for's, point and turnLine index are found
    CrossingFound1:;
    }

    int turnNum = crossingTurnLinePoint.index;

    if (turnNum == 99)
    {
        isTurnCreationNotCrossingError = true;
        return false;
    }

    int curTurnLineCount = turn.turnArr[turnNum].turnLine.size();

    //possible points close to AB Curve point
    QVector<int> turnLineCloseList;

    for (int j = 0; j < curTurnLineCount; j++)
    {
        if ((turn.turnArr[turnNum].turnLine[j].easting - crossingCurvePoint.easting) < 2
            && (turn.turnArr[turnNum].turnLine[j].easting - crossingCurvePoint.easting) > -2
            && (turn.turnArr[turnNum].turnLine[j].northing - crossingCurvePoint.northing) < 2
            && (turn.turnArr[turnNum].turnLine[j].northing - crossingCurvePoint.northing) > -2)
        {
            turnLineCloseList.append(j);
        }
    }

    double dist1, dist2 = 99;
    curTurnLineCount = turnLineCloseList.size();
    for (int i = 0; i < curTurnLineCount; i++)
    {
        dist1 = glm::distance(turn.turnArr[turnNum].turnLine[turnLineCloseList[i]].easting,
                                turn.turnArr[turnNum].turnLine[turnLineCloseList[i]].northing,
                                    crossingCurvePoint.easting, crossingCurvePoint.northing);
        if (dist1 < dist2)
        {
            crossingTurnLinePoint.index = turnLineCloseList[i];
            dist2 = dist1;
        }
    }

    //fill up the coords
    crossingTurnLinePoint.easting = turn.turnArr[turnNum].turnLine[crossingTurnLinePoint.index].easting;
    crossingTurnLinePoint.northing = turn.turnArr[turnNum].turnLine[crossingTurnLinePoint.index].northing;
    crossingTurnLinePoint.heading = turn.turnArr[turnNum].turnLine[crossingTurnLinePoint.index].heading;

    return crossingCurvePoint.easting != -20000 && crossingCurvePoint.easting != -20000;
}

void CYouTurn::addSequenceLines(double head, Vec3 pivot)
{
    Vec3 pt;
    for (int a = 0; a < youTurnStartOffset; a++)
    {
        pt.easting = ytList[0].easting + (sin(head));
        pt.northing = ytList[0].northing + (cos(head));
        pt.heading = ytList[0].heading;
        ytList.insert(0, pt);
    }

    int count = ytList.size();

    for (int i = 1; i <= youTurnStartOffset; i++)
    {
        pt.easting = ytList[count - 1].easting + (sin(head) * i);
        pt.northing = ytList[count - 1].northing + (cos(head) * i);
        pt.heading = head;
        ytList.append(pt);
    }

    double distancePivotToTurnLine;
    count = ytList.size();
    for (int i = 0; i < count; i += 2)
    {
        distancePivotToTurnLine = glm::distance(ytList[i], pivot);
        if (distancePivotToTurnLine > 3)
        {
            isTurnCreationTooClose = false;
        }
        else
        {
            isTurnCreationTooClose = true;
            //set the flag to Critical stop machine
            //if (isTurnCreationTooClose) mf.mc.isOutOfBounds = true;
            if (isTurnCreationTooClose) emit outOfBounds();
            break;
        }
    }
}

bool CYouTurn::buildDriveAround(const CVehicle &vehicle,
                                const CABLine &ABLine,
                                CGeoFence &gf, CTurn &turn,
                                const CBoundary &bnd, CMazeGrid &mazeGrid,
                                double minFieldX, double minFieldY)
{
    double headAB = ABLine.abHeading;
    if (!ABLine.isABSameAsVehicleHeading) headAB += M_PI;

    double cosHead = cos(headAB);
    double sinHead = sin(headAB);

    Vec3 start;
    Vec3 stop;
    Vec3 pt2;

    //grab the pure pursuit point right on ABLine
    Vec3 onPurePoint(ABLine.rEastAB, ABLine.rNorthAB, 0);

    //how far are we from any geoFence
    gf.findPointsDriveAround(turn, bnd, mazeGrid, minFieldX, minFieldY, onPurePoint, headAB, start, stop);

    //not an inside border
    if (start.easting == 88888) return false;

    //get the dubins path Vec3 point coordinates of path
    ytList.clear();

    //find a path from start to goal - diagnostic, but also used later
    mazeList = mazeGrid.searchForPath(minFieldX, minFieldY, start, stop);

    //you can't get anywhere!
    if (mazeList.size() == 0) return false;

    //not really changing direction so need to fake a turn twice.
    //emit?
    //mf.SwapDirection();
    emit swapDirection();

    //list of Vec3 points of Dubins shortest path between 2 points - To be converted to RecPt
    QVector<Vec3> shortestDubinsList;

    //Dubins at the start and stop of mazePath
    CDubinsTurningRadius = vehicle.minTurningRadius * 1.0;
    CDubins dubPath;

    //start is navigateable - maybe
    int cnt = mazeList.size();
    int cut = 8;
    if (cnt < 18) cut = 3;

    if (cnt > 0)
    {
        pt2.easting = start.easting - (sinHead * vehicle.minTurningRadius * 1.5);
        pt2.northing = start.northing - (cosHead * vehicle.minTurningRadius * 1.5);
        pt2.heading = headAB;

        shortestDubinsList = dubPath.GenerateDubins(pt2, mazeList[cut - 1], bnd, gf);
        for (int i = 1; i < shortestDubinsList.size(); i++)
        {
            Vec3 pt(shortestDubinsList[i].easting, shortestDubinsList[i].northing, shortestDubinsList[i].heading);
            ytList.append(pt);
        }

        for (int i = cut; i < mazeList.size() - cut; i++)
        {
            Vec3 pt(mazeList[i].easting, mazeList[i].northing, mazeList[i].heading);
            ytList.append(pt);
        }

        pt2.easting = stop.easting + (sinHead * vehicle.minTurningRadius * 1.5);
        pt2.northing = stop.northing + (cosHead * vehicle.minTurningRadius * 1.5);
        pt2.heading = headAB;

        shortestDubinsList = dubPath.GenerateDubins(mazeList[cnt - cut], pt2, bnd, gf);

        for (int i = 1; i < shortestDubinsList.size(); i++)
        {
            Vec3 pt(shortestDubinsList[i].easting, shortestDubinsList[i].northing, shortestDubinsList[i].heading);
            ytList.append(pt);
        }
    }

    if (ytList.size() > 10) youTurnPhase = 3;

    Vec3 pt3;

    for (int a = 0; a < youTurnStartOffset; a++)
    {
        pt3.easting = ytList[0].easting - sinHead;
        pt3.northing = ytList[0].northing - cosHead;
        pt3.heading = headAB;
        ytList.insert(0, pt3);
    }

    int count = ytList.size();

    for (int i = 1; i <= youTurnStartOffset; i++)
    {
        pt3.easting = ytList[count - 1].easting + (sinHead * i);
        pt3.northing = ytList[count - 1].northing + (cosHead * i);
        pt3.heading = headAB;
        ytList.append(pt3);
    }

    return true;

}

bool CYouTurn::buildABLineDubinsYouTurn(const CVehicle &vehicle,
                                        const CTool &tool,
                                        const CBoundary &bnd,
                                        CGeoFence &gf,
                                        const CABLine &ABLine, CTurn &turn,
                                        CMazeGrid &mazeGrid,
                                        double minFieldX, double minFieldY,
                                        bool isTurnRight)
{
    double headAB = ABLine.abHeading;
    if (!ABLine.isABSameAsVehicleHeading) headAB += M_PI;

    if (youTurnPhase == 0)
    {
        if (buildDriveAround(vehicle, ABLine, gf, turn, bnd, mazeGrid, minFieldX, minFieldY )) return true;

        //grab the pure pursuit point right on ABLine
        Vec3 onPurePoint(ABLine.rEastAB, ABLine.rNorthAB, 0);

        //how far are we from any turn boundary
        turn.findClosestTurnPoint(bnd, tool, isYouTurnRight, onPurePoint, headAB);

        //or did we lose the turnLine - we are on the highway cuz we left the outer/inner turn boundary
        if ((int)turn.closestTurnPt.easting != -20000)
        {
            distancePivotToTurnLine = glm::distance(vehicle.pivotAxlePos, turn.closestTurnPt);
        }
        else
        {
            //Full emergency stop code goes here, it thinks its auto turn, but its not!
            distancePivotToTurnLine = -3333;
        }

        //delta between AB heading and boundary closest point heading
        boundaryAngleOffPerpendicular = M_PI - fabs(fabs(turn.closestTurnPt.heading - headAB) - M_PI);
        boundaryAngleOffPerpendicular -= glm::PIBy2;
        boundaryAngleOffPerpendicular *= -1;
        if (boundaryAngleOffPerpendicular > 1.25) boundaryAngleOffPerpendicular = 1.25;
        if (boundaryAngleOffPerpendicular < -1.25) boundaryAngleOffPerpendicular = -1.25;

        //for calculating innner circles of turn
        tangencyAngle = (glm::PIBy2 - fabs(boundaryAngleOffPerpendicular)) * 0.5;

        //baseline away from boundary to start calculations
        double toolTurnWidth = tool.toolWidth * rowSkipsWidth;

        //distance from TurnLine for trigger added in youturn form, include the 3 m bump forward
        distanceTurnBeforeLine = 0;

        if (vehicle.minTurningRadius * 2 < toolTurnWidth)
        {
            if (boundaryAngleOffPerpendicular < 0)
            {
                //which is actually left
                if (isYouTurnRight)
                    distanceTurnBeforeLine += (vehicle.minTurningRadius * tan(tangencyAngle));//short
                else
                    distanceTurnBeforeLine += (vehicle.minTurningRadius / tan(tangencyAngle)); //long
            }
            else
            {
                //which is actually left
                if (isYouTurnRight)
                    distanceTurnBeforeLine += (vehicle.minTurningRadius / tan(tangencyAngle)); //long
                else
                    distanceTurnBeforeLine += (vehicle.minTurningRadius * tan(tangencyAngle)); //short
            }
        }
        else //turn Radius is wider then equipment width so ohmega turn
        {
            distanceTurnBeforeLine += (2 * vehicle.minTurningRadius);
        }

        //used for distance calc for other part of turn

        CDubins dubYouTurnPath;
        CDubinsTurningRadius = vehicle.minTurningRadius;

        //point on AB line closest to pivot axle point from ABLine PurePursuit
        rEastYT = ABLine.rEastAB;
        rNorthYT = ABLine.rNorthAB;
        isABSameAsFixHeading = ABLine.isABSameAsVehicleHeading;
        double head = ABLine.abHeading;

        //grab the vehicle widths and offsets
        double widthMinusOverlap = tool.toolWidth - tool.toolOverlap;
        double toolOffset = tool.toolOffset * 2.0;
        double turnOffset;

        //turning right
        if (isTurnRight) turnOffset = (widthMinusOverlap + toolOffset);
        else turnOffset = (widthMinusOverlap - toolOffset);

        double turnRadius = turnOffset / cos(boundaryAngleOffPerpendicular);
        if (!isABSameAsFixHeading) head += M_PI;

        double turnDiagDistance = distancePivotToTurnLine;

        //move the start forward 2 meters
        rEastYT += (sin(head) * turnDiagDistance);
        rNorthYT += (cos(head) * turnDiagDistance);

        Vec3 start(rEastYT, rNorthYT, head);
        Vec3 goal;

        turnRadius *= rowSkipsWidth;
        turnOffset *= rowSkipsWidth;

        //move the cross line calc to not include first turn
        goal.easting = rEastYT + (sin(head) * distanceTurnBeforeLine);
        goal.northing = rNorthYT + (cos(head) * distanceTurnBeforeLine);

        //headland angle relative to vehicle heading to head along the boundary left or right
        double bndAngle = head - boundaryAngleOffPerpendicular + glm::PIBy2;

        //now we go the other way to turn round
        head -= M_PI;
        if (head < 0) head += glm::twoPI;

        if ((vehicle.minTurningRadius * 2.0) < turnOffset)
        {
            //are we right of boundary
            if (boundaryAngleOffPerpendicular > 0)
            {
                if (!isYouTurnRight) //which is actually right now
                {
                    goal.easting += (sin(bndAngle) * turnRadius);
                    goal.northing += (cos(bndAngle) * turnRadius);

                    double dis = (vehicle.minTurningRadius / tan(tangencyAngle)); //long
                    goal.easting += (sin(head) * dis);
                    goal.northing += (cos(head) * dis);
                }
                else //going left
                {
                    goal.easting -= (sin(bndAngle) * turnRadius);
                    goal.northing -= (cos(bndAngle) * turnRadius);

                    double dis = (vehicle.minTurningRadius * tan(tangencyAngle)); //short
                    goal.easting += (sin(head) * dis);
                    goal.northing += (cos(head) * dis);
                }
            }
            else // going left of boundary
            {
                if (!isYouTurnRight) //pointing to right
                {
                    goal.easting += (sin(bndAngle) * turnRadius);
                    goal.northing += (cos(bndAngle) * turnRadius);

                    double dis = (vehicle.minTurningRadius * tan(tangencyAngle)); //short
                    goal.easting += (sin(head) * dis);
                    goal.northing += (cos(head) * dis);
                }
                else
                {
                    goal.easting -= (sin(bndAngle) * turnRadius);
                    goal.northing -= (cos(bndAngle) * turnRadius);

                    double dis = (vehicle.minTurningRadius / tan(tangencyAngle)); //long
                    goal.easting += (sin(head) * dis);
                    goal.northing += (cos(head) * dis);
                }
            }
        }
        else
        {
            if (!isTurnRight)
            {
                goal.easting = rEastYT - (cos(-head) * turnOffset);
                goal.northing = rNorthYT - (sin(-head) * turnOffset);
            }
            else
            {
                goal.easting = rEastYT + (cos(-head) * turnOffset);
                goal.northing = rNorthYT + (sin(-head) * turnOffset);
            }
        }

        goal.heading = head;

        //generate the turn points
        ytList = dubYouTurnPath.GenerateDubins(start, goal);
        addSequenceLines(head,vehicle.pivotAxlePos);
        int count = ytList.size();
        if (count == 0) return false;
        else youTurnPhase = 1;
    }

    if (youTurnPhase == 3) return true;

    // Phase 0 - back up the turn till it is out of bounds.
    // Phase 1 - move it forward till out of bounds.
    // Phase 2 - move forward couple meters away from turn line.
    // Phase 3 - ytList is made, waiting to get close enough to it

    isOutOfBounds = false;
    switch (youTurnPhase)
    {
        case 1:
            //the temp array
            distancePivotToTurnLine = glm::distance(ytList[0], vehicle.pivotAxlePos);
            double cosHead = cos(headAB);
            double sinHead = sin(headAB);

            int cnt = ytList.size();
            QVector<Vec3> arr2 = ytList;
            ytList.clear();

            for (int i = 0; i < cnt; i++)
            {
                arr2[i].easting -= (sinHead);
                arr2[i].northing -= (cosHead);
                ytList.append(arr2[i]);
            }

            for (int j = 0; j < cnt; j += 2)
            {
                if (!turn.turnArr[0].isPointInTurnWorkArea(ytList[j])) isOutOfBounds = true;
                if (isOutOfBounds) break;

                for (int i = 1; i < bnd.bndArr.size(); i++)
                {
                    //make sure not inside a non drivethru boundary
                    if (!bnd.bndArr[i].isSet) continue;
                    if (bnd.bndArr[i].isDriveThru) continue;
                    if (bnd.bndArr[i].isDriveAround) continue;
                    if (turn.turnArr[i].isPointInTurnWorkArea(ytList[j]))
                    {
                        isOutOfBounds = true;
                        break;
                    }
                }
                if (isOutOfBounds) break;
            }

            if (!isOutOfBounds)
            {
                youTurnPhase = 3;
            }
            else
            {
                //turn keeps approaching vehicle and running out of space - end of field?
                if (isOutOfBounds && distancePivotToTurnLine > 3)
                {
                    isTurnCreationTooClose = false;
                }
                else
                {
                    isTurnCreationTooClose = true;

                    //set the flag to Critical stop machine
                    emit outOfBounds();
                    //if (isTurnCreationTooClose) mf.mc.isOutOfBounds = true;
                }
            }
            break;
    }
    return true;
}

bool CYouTurn::buildABLinePatternYouTurn(const CVehicle &vehicle,
                                         const CTool &tool,
                                         const CBoundary &bnd,
                                         const CABLine &ABLine, CTurn &turn,
                                         bool isTurnRight)
{
    double headAB = ABLine.abHeading;
    if (!ABLine.isABSameAsVehicleHeading) headAB += M_PI;

    //grab the pure pursuit point right on ABLine
    Vec3 onPurePoint(ABLine.rEastAB, ABLine.rNorthAB, 0);

    //how far are we from any turn boundary
    turn.findClosestTurnPoint(bnd, tool, isYouTurnRight, onPurePoint, headAB);

    //or did we lose the turnLine - we are on the highway cuz we left the outer/inner turn boundary
    if ((int)turn.closestTurnPt.easting != -20000)
    {
        distancePivotToTurnLine = glm::distance(vehicle.pivotAxlePos, turn.closestTurnPt);
    }
    else
    {
        //Full emergency stop code goes here, it thinks its auto turn, but its not!
        distancePivotToTurnLine = -3333;
    }

    distanceTurnBeforeLine = turnDistanceAdjuster;

    ytList.clear();

    //point on AB line closest to pivot axle point from ABLine PurePursuit
    rEastYT = ABLine.rEastAB;
    rNorthYT = ABLine.rNorthAB;
    isABSameAsFixHeading = ABLine.isABSameAsVehicleHeading;
    double head = ABLine.abHeading;

    //grab the vehicle widths and offsets
    double widthMinusOverlap = tool.toolWidth - tool.toolOverlap;
    double toolOffset = tool.toolOffset * 2.0;
    double turnOffset;

    //turning right
    if (isTurnRight) turnOffset = (widthMinusOverlap + toolOffset);
    else turnOffset = (widthMinusOverlap - toolOffset);

    //Pattern Turn
    numShapePoints = youFileList.size();
    Vec3 pt[numShapePoints];

    //Now put the shape into an array since lists are immutable
    for (int i = 0; i < numShapePoints; i++)
    {
        pt[i].easting = youFileList[i].easting;
        pt[i].northing = youFileList[i].northing;
    }

    //start of path on the origin. Mirror the shape if left turn
    if (isTurnRight)
    {
        for (int i = 0; i < numShapePoints; i++) pt[i].easting *= -1;
    }

    //scaling - Drawing is 10m wide so find ratio of tool width
    double scale = turnOffset * 0.1;
    for (int i = 0; i < numShapePoints; i++)
    {
        pt[i].easting *= scale * rowSkipsWidth;
        pt[i].northing *= scale * rowSkipsWidth;
    }

    if (!isABSameAsFixHeading) head += M_PI;

    double _turnDiagDistance = distancePivotToTurnLine - distanceTurnBeforeLine;

    //move the start forward
    if (youTurnPhase < 2)
    {
        rEastYT += (sin(head) * (_turnDiagDistance - turnOffset));
        rNorthYT += (cos(head) * (_turnDiagDistance - turnOffset));
    }
    else
    {
        _turnDiagDistance -= 2;
        turnDistanceAdjuster += 5;
        rEastYT += (sin(head) * (_turnDiagDistance - turnOffset));
        rNorthYT += (cos(head) * (_turnDiagDistance - turnOffset));
        youTurnPhase = 3;
    }

    //rotate pattern to match AB Line heading
    double xr, yr, xr2, yr2;
    for (int i = 0; i < numShapePoints - 1; i++)
    {
        xr = (cos(-head) * pt[i].easting) - (sin(-head) * pt[i].northing) + rEastYT;
        yr = (sin(-head) * pt[i].easting) + (cos(-head) * pt[i].northing) + rNorthYT;

        xr2 = (cos(-head) * pt[i + 1].easting) - (sin(-head) * pt[i + 1].northing) + rEastYT;
        yr2 = (sin(-head) * pt[i + 1].easting) + (cos(-head) * pt[i + 1].northing) + rNorthYT;

        pt[i].easting = xr;
        pt[i].northing = yr;
        pt[i].heading = atan2(xr2 - xr, yr2 - yr);
        if (pt[i].heading < 0) pt[i].heading += glm::twoPI;
        ytList.append(pt[i]);
    }
    xr = (cos(-head) * pt[numShapePoints - 1].easting) - (sin(-head) * pt[numShapePoints - 1].northing) + rEastYT;
    yr = (sin(-head) * pt[numShapePoints - 1].easting) + (cos(-head) * pt[numShapePoints - 1].northing) + rNorthYT;

    pt[numShapePoints - 1].easting = xr;
    pt[numShapePoints - 1].northing = yr;
    pt[numShapePoints - 1].heading = pt[numShapePoints - 2].heading;
    ytList.append(pt[numShapePoints - 1]);

    //pattern all made now is it outside a boundary
    //now check to make sure we are not in an inner turn boundary - drive thru is ok
    int count = ytList.size();
    if (count == 0) return false;
    isOutOfBounds = false;

    head += M_PI;

    Vec3 ptt;
    for (int a = 0; a < youTurnStartOffset; a++)
    {
        ptt.easting = ytList[0].easting + (sin(head));
        ptt.northing = ytList[0].northing + (cos(head));
        ptt.heading = ytList[0].heading;
        ytList.insert(0, ptt);
    }

    count = ytList.size();

    for (int i = 1; i <= youTurnStartOffset; i++)
    {
        ptt.easting = ytList[count - 1].easting + (sin(head) * i);
        ptt.northing = ytList[count - 1].northing + (cos(head) * i);
        ptt.heading = ytList[count - 1].heading;
        ytList.append(ptt);
    }

    double distancePivotToTurnLine;
    count = ytList.size();
    for (int i = 0; i < count; i += 2)
    {
        distancePivotToTurnLine = glm::distance(ytList[i], vehicle.pivotAxlePos);
        if (distancePivotToTurnLine > 3)
        {
            isTurnCreationTooClose = false;
        }
        else
        {
            isTurnCreationTooClose = true;
            //set the flag to Critical stop machine
            if (isTurnCreationTooClose) emit outOfBounds(); //mf.mc.isOutOfBounds = true;
            break;
        }
    }

    // Phase 0 - back up the turn till it is out of bounds.
    // Phase 1 - move it forward till out of bounds.
    // Phase 2 - move forward couple meters away from turn line.

    switch (youTurnPhase)
    {
        case 0:
            //if (turnDiagnosticAdjuster == 0) turnDiagnosticAdjuster = turnRadius;
            turnDistanceAdjuster -= 2;
            for (int j = 0; j < count; j += 2)
            {
                if (!turn.turnArr[0].isPointInTurnWorkArea(ytList[j])) isOutOfBounds = true;
                if (isOutOfBounds) break;

                for (int i = 1; i < bnd.bndArr.size(); i++)
                {
                    //make sure not inside a non drivethru boundary
                    if (!bnd.bndArr[i].isSet) continue;
                    if (bnd.bndArr[i].isDriveThru) continue;
                    if (bnd.bndArr[i].isDriveAround) continue;
                    if (turn.turnArr[i].isPointInTurnWorkArea(ytList[j]))
                    {
                        isOutOfBounds = true;
                        break;
                    }
                }
                if (isOutOfBounds) break;
            }

            if (isOutOfBounds) youTurnPhase = 1;
            break;

        case 1:
            for (int j = 0; j < count; j += 2)
            {
                if (!turn.turnArr[0].isPointInTurnWorkArea(ytList[j])) isOutOfBounds = true;
                if (isOutOfBounds) break;

                for (int i = 1; i < bnd.bndArr.size(); i++)
                {
                    //make sure not inside a non drivethru boundary
                    if (!bnd.bndArr[i].isSet) continue;
                    if (bnd.bndArr[i].isDriveThru) continue;
                    if (bnd.bndArr[i].isDriveAround) continue;
                    if (turn.turnArr[i].isPointInTurnWorkArea(ytList[j]))
                    {
                        isOutOfBounds = true;
                        break;
                    }
                }
                if (isOutOfBounds) break;
            }

            if (!isOutOfBounds)
            {
                youTurnPhase = 2;
            }
            else
            {
                //turn keeps approaching vehicle and running out of space - end of field?
                if (isOutOfBounds && _turnDiagDistance > 3)
                {
                    turnDistanceAdjuster += 2;
                    isTurnCreationTooClose = false;
                }
                else
                {
                    isTurnCreationTooClose = true;

                    //set the flag to Critical stop machine
                    //if (isTurnCreationTooClose) mf.mc.isOutOfBounds = true;
                    emit outOfBounds();
                    break;
                }
            }
            break;
    }

    return isOutOfBounds;
}

bool CYouTurn::buildCurvePatternYouTurn(const CVehicle &vehicle,
                                        const CTool &tool,
                                        const CBoundary &bnd,
                                        const CABCurve &curve,
                                        CTurn &turn,
                                        bool isTurnRight, Vec3 pivotPos)
{
    if (youTurnPhase > 0)
    {
        ytList.clear();
        double delta = curve.deltaOfRefAndAveHeadings;

        double head = crossingCurvePoint.heading;
        if (!curve.isABSameAsVehicleHeading) head += M_PI;

        //are we going same way as creation of curve
        //bool isCountingUp = mf.curve.isABSameAsVehicleHeading;

        //grab the vehicle widths and offsets
        double widthMinusOverlap = tool.toolWidth - tool.toolOverlap;
        double toolOffset = tool.toolOffset * 2.0;
        double turnOffset;

        //turning right
        if (isTurnRight) turnOffset = (widthMinusOverlap + toolOffset);
        else turnOffset = (widthMinusOverlap - toolOffset);

        //to compensate for AB Curve overlap
        turnOffset *= delta;

        //Pattern Turn
        numShapePoints = youFileList.size();
        Vec3 pt[numShapePoints];

        //Now put the shape into an array since lists are immutable
        for (int i = 0; i < numShapePoints; i++)
        {
            pt[i].easting = youFileList[i].easting;
            pt[i].northing = youFileList[i].northing;
        }

        //start of path on the origin. Mirror the shape if left turn
        if (isTurnRight)
        {
            for (int i = 0; i < numShapePoints; i++) pt[i].easting *= -1;
        }

        //scaling - Drawing is 10m wide so find ratio of tool width
        double scale = turnOffset * 0.1;
        for (int i = 0; i < numShapePoints; i++)
        {
            pt[i].easting *= scale * rowSkipsWidth;
            pt[i].northing *= scale * rowSkipsWidth;
        }

        //rotate pattern to match AB Line heading
        double xr, yr, xr2, yr2;
        for (int i = 0; i < numShapePoints - 1; i++)
        {
            xr = (cos(-head) * pt[i].easting) - (sin(-head) * pt[i].northing) + crossingCurvePoint.easting;
            yr = (sin(-head) * pt[i].easting) + (cos(-head) * pt[i].northing) + crossingCurvePoint.northing;

            xr2 = (cos(-head) * pt[i + 1].easting) - (sin(-head) * pt[i + 1].northing) + crossingCurvePoint.easting;
            yr2 = (sin(-head) * pt[i + 1].easting) + (cos(-head) * pt[i + 1].northing) + crossingCurvePoint.northing;

            pt[i].easting = xr;
            pt[i].northing = yr;

            pt[i].heading = atan2(xr2 - xr, yr2 - yr);
            if (pt[i].heading < 0) pt[i].heading += glm::twoPI;
            ytList.append(pt[i]);
        }
        xr = (cos(-head) * pt[numShapePoints - 1].easting) - (sin(-head) * pt[numShapePoints - 1].northing) + crossingCurvePoint.easting;
        yr = (sin(-head) * pt[numShapePoints - 1].easting) + (cos(-head) * pt[numShapePoints - 1].northing) + crossingCurvePoint.northing;

        pt[numShapePoints - 1].easting = xr;
        pt[numShapePoints - 1].northing = yr;
        pt[numShapePoints - 1].heading = pt[numShapePoints - 2].heading;
        ytList.append(pt[numShapePoints - 1]);

        //pattern all made now is it outside a boundary
        head -= M_PI;

        Vec3 ptt;
        for (int a = 0; a < youTurnStartOffset; a++)
        {
            ptt.easting = ytList[0].easting + (sin(head));
            ptt.northing = ytList[0].northing + (cos(head));
            ptt.heading = ytList[0].heading;
            ytList.insert(0, ptt);
        }

        int count = ytList.size();

        for (int i = 1; i <= youTurnStartOffset; i++)
        {
            ptt.easting = ytList[count - 1].easting + (sin(head) * i);
            ptt.northing = ytList[count - 1].northing + (cos(head) * i);
            ptt.heading = ytList[count - 1].heading;
            ytList.append(ptt);
        }

        double distancePivotToTurnLine;
        count = ytList.size();
        for (int i = 0; i < count; i += 2)
        {
            distancePivotToTurnLine = glm::distance(ytList[i], vehicle.pivotAxlePos);
            if (distancePivotToTurnLine > 3)
            {
                isTurnCreationTooClose = false;
            }
            else
            {
                isTurnCreationTooClose = true;
                //set the flag to Critical stop machine
                if (isTurnCreationTooClose) emit outOfBounds(); //mf.mc.isOutOfBounds = true;
                break;
            }
        }
    }

    switch (youTurnPhase)
    {
        case 0: //find the crossing points
            if (findCurveTurnPoints(curve, bnd, turn)) youTurnPhase = 1;
            else emit outOfBounds();//mf.mc.isOutOfBounds = true;
            ytList.clear();
            break;

        case 1:
            //now check to make sure turn is not in an inner turn boundary - drive thru is ok
            {
            int count = ytList.size();
            if (count == 0) return false;
            isOutOfBounds = false;

            //Out of bounds?
            for (int j = 0; j < count; j += 2)
            {
                if (!turn.turnArr[0].isPointInTurnWorkArea(ytList[j])) isOutOfBounds = true;
                if (isOutOfBounds) break;

                for (int i = 1; i < bnd.bndArr.size(); i++)
                {
                    //make sure not inside a non drivethru boundary
                    if (!bnd.bndArr[i].isSet) continue;
                    if (bnd.bndArr[i].isDriveThru) continue;
                    if (bnd.bndArr[i].isDriveAround) continue;
                    if (turn.turnArr[i].isPointInTurnWorkArea(ytList[j]))
                    {
                        isOutOfBounds = true;
                        break;
                    }
                }
                if (isOutOfBounds) break;
            }

            //first check if not out of bounds, add a bit more to clear turn line, set to phase 2
            if (!isOutOfBounds)
            {
                youTurnPhase = 2;
                //if (mf.curve.isABSameAsVehicleHeading)
                //{
                //    crossingCurvePoint.index -= 2;
                //    if (crossingCurvePoint.index < 0) crossingCurvePoint.index = 0;
                //}
                //else
                //{
                //    crossingCurvePoint.index += 2;
                //    if (crossingCurvePoint.index >= curListCount)
                //        crossingCurvePoint.index = curListCount - 1;
                //}

                //crossingCurvePoint.easting = mf.curve.curList[crossingCurvePoint.index].easting;
                //crossingCurvePoint.northing = mf.curve.curList[crossingCurvePoint.index].northing;
                //crossingCurvePoint.heading = mf.curve.curList[crossingCurvePoint.index].heading;
                return true;
            }

            //keep moving infield till pattern is all inside
            if (curve.isABSameAsVehicleHeading)
            {
                crossingCurvePoint.index--;
                if (crossingCurvePoint.index < 0) crossingCurvePoint.index = 0;
            }
            else
            {
                crossingCurvePoint.index++;
                if (crossingCurvePoint.index >= curListCount)
                    crossingCurvePoint.index = curListCount - 1;
            }

            crossingCurvePoint.easting = curve.curList[crossingCurvePoint.index].easting;
            crossingCurvePoint.northing = curve.curList[crossingCurvePoint.index].northing;
            crossingCurvePoint.heading = curve.curList[crossingCurvePoint.index].heading;

            double tooClose = glm::distance(ytList[0], pivotPos);
            isTurnCreationTooClose = tooClose < 3;

            //set the flag to Critical stop machine
            if (isTurnCreationTooClose) emit outOfBounds(); //mf.mc.isOutOfBounds = true;
            break;
            }
        case 2:
            youTurnPhase = 3;
            break;
    }
    return true;
}

bool CYouTurn::buildCurveDubinsYouTurn(const CVehicle &vehicle,
                                       const CTool &tool,
                                       const CBoundary &bnd,
                                       const CABCurve &curve,
                                       CTurn &turn,
                                       bool isTurnRight, Vec3 pivotPos)
{
    if (youTurnPhase > 0)
    {
        isABSameAsFixHeading = curve.isSameWay;

        double head = crossingCurvePoint.heading;
        if (!isABSameAsFixHeading) head += M_PI;
        double delta = curve.deltaOfRefAndAveHeadings;

        //delta between AB heading and boundary closest point heading
        boundaryAngleOffPerpendicular = M_PI - fabs(fabs(crossingTurnLinePoint.heading - head) - M_PI);
        boundaryAngleOffPerpendicular -= glm::PIBy2;
        boundaryAngleOffPerpendicular *= -1;
        if (boundaryAngleOffPerpendicular > 1.25) boundaryAngleOffPerpendicular = 1.25;
        if (boundaryAngleOffPerpendicular < -1.25) boundaryAngleOffPerpendicular = -1.25;

        //for calculating innner circles of turn
        tangencyAngle = (glm::PIBy2 - fabs(boundaryAngleOffPerpendicular)) * 0.5;

        //distance from crossPoint to turn line
        if (vehicle.minTurningRadius * 2 < (tool.toolWidth * rowSkipsWidth))
        {
            if (boundaryAngleOffPerpendicular < 0)
            {
                //which is actually left
                if (isYouTurnRight)
                    distanceTurnBeforeLine = (vehicle.minTurningRadius * tan(tangencyAngle));//short
                else
                    distanceTurnBeforeLine = (vehicle.minTurningRadius / tan(tangencyAngle)); //long
            }
            else
            {
                //which is actually left
                if (isYouTurnRight)
                    distanceTurnBeforeLine = (vehicle.minTurningRadius / tan(tangencyAngle)); //long
                else
                    distanceTurnBeforeLine = (vehicle.minTurningRadius * tan(tangencyAngle)); //short
            }
        }

        //turn Radius is wider then equipment width so ohmega turn
        else
        {
            distanceTurnBeforeLine = (2 * vehicle.minTurningRadius);
        }

        CDubins dubYouTurnPath;
        CDubinsTurningRadius = vehicle.minTurningRadius;

        //grab the vehicle widths and offsets
        double widthMinusOverlap = tool.toolWidth - tool.toolOverlap;
        double toolOffset = tool.toolOffset * 2.0;
        double turnOffset;

        //calculate the true width
        if (isTurnRight) turnOffset = (widthMinusOverlap + toolOffset);
        else turnOffset = (widthMinusOverlap - toolOffset);

        //to compensate for AB Curve overlap
        turnOffset *= delta;

        //diagonally across
        double turnRadius = turnOffset / cos(boundaryAngleOffPerpendicular);

        //start point of Dubins
        Vec3 start(crossingCurvePoint.easting, crossingCurvePoint.northing, head);

        Vec3 goal;
        turnRadius *= rowSkipsWidth;
        turnOffset *= rowSkipsWidth;

        //move the cross line calc to not include first turn
        goal.easting = crossingCurvePoint.easting + (sin(head) * distanceTurnBeforeLine);
        goal.northing = crossingCurvePoint.northing + (cos(head) * distanceTurnBeforeLine);

        //headland angle relative to vehicle heading to head along the boundary left or right
        double bndAngle = head - boundaryAngleOffPerpendicular + glm::PIBy2;

        //now we go the other way to turn round
        head -= M_PI;
        if (head < 0) head += glm::twoPI;

        if ((vehicle.minTurningRadius * 2.0) < turnOffset)
        {
            //are we right of boundary
            if (boundaryAngleOffPerpendicular > 0)
            {
                if (!isYouTurnRight) //which is actually right now
                {
                    goal.easting += (sin(bndAngle) * turnRadius);
                    goal.northing += (cos(bndAngle) * turnRadius);

                    double dis = (vehicle.minTurningRadius / tan(tangencyAngle)); //long
                    goal.easting += (sin(head) * dis);
                    goal.northing += (cos(head) * dis);
                }
                else //going left
                {
                    goal.easting -= (sin(bndAngle) * turnRadius);
                    goal.northing -= (cos(bndAngle) * turnRadius);

                    double dis = (vehicle.minTurningRadius * tan(tangencyAngle)); //short
                    goal.easting += (sin(head) * dis);
                    goal.northing += (cos(head) * dis);
                }
            }
            else // going left of boundary
            {
                if (!isYouTurnRight) //pointing to right
                {
                    goal.easting += (sin(bndAngle) * turnRadius);
                    goal.northing += (cos(bndAngle) * turnRadius);

                    double dis = (vehicle.minTurningRadius * tan(tangencyAngle)); //short
                    goal.easting += (sin(head) * dis);
                    goal.northing += (cos(head) * dis);
                }
                else
                {
                    goal.easting -= (sin(bndAngle) * turnRadius);
                    goal.northing -= (cos(bndAngle) * turnRadius);

                    double dis = (vehicle.minTurningRadius / tan(tangencyAngle)); //long
                    goal.easting += (sin(head) * dis);
                    goal.northing += (cos(head) * dis);
                }
            }
        }
        else
        {
            if (!isTurnRight)
            {
                goal.easting = crossingCurvePoint.easting - (cos(-head) * turnOffset);
                goal.northing = crossingCurvePoint.northing - (sin(-head) * turnOffset);
            }
            else
            {
                goal.easting = crossingCurvePoint.easting + (cos(-head) * turnOffset);
                goal.northing = crossingCurvePoint.northing + (sin(-head) * turnOffset);
            }
        }

        goal.heading = head;

        //generate the turn points
        ytList = dubYouTurnPath.GenerateDubins(start, goal);
        int count = ytList.size();
        if (count == 0) return false;

        //these are the lead in lead out lines that add to the turn
        addSequenceLines(head, vehicle.pivotAxlePos);
    }

    switch (youTurnPhase)
    {
        case 0: //find the crossing points
            if (findCurveTurnPoints(curve,bnd,turn)) youTurnPhase = 1;
            ytList.clear();
            break;

        case 1:
            //now check to make sure we are not in an inner turn boundary - drive thru is ok
            {
            int count = ytList.size();
            if (count == 0) return false;

            //Are we out of bounds?
            isOutOfBounds = false;
            for (int j = 0; j < count; j += 2)
            {
                if (!turn.turnArr[0].isPointInTurnWorkArea(ytList[j]))
                {
                    isOutOfBounds = true;
                    break;
                }

                for (int i = 1; i < bnd.bndArr.size(); i++)
                {
                    //make sure not inside a non drivethru boundary
                    if (!bnd.bndArr[i].isSet) continue;
                    if (bnd.bndArr[i].isDriveThru) continue;
                    if (bnd.bndArr[i].isDriveAround) continue;
                    if (turn.turnArr[i].isPointInTurnWorkArea(ytList[j]))
                    {
                        isOutOfBounds = true;
                        break;
                    }
                }
                if (isOutOfBounds) break;
            }

            //first check if not out of bounds, add a bit more to clear turn line, set to phase 2
            if (!isOutOfBounds)
            {
                youTurnPhase = 2;
                //if (curve.isABSameAsVehicleHeading)
                //{
                //    crossingCurvePoint.index -= 2;
                //    if (crossingCurvePoint.index < 0) crossingCurvePoint.index = 0;
                //}
                //else
                //{
                //    crossingCurvePoint.index += 2;
                //    if (crossingCurvePoint.index >= curListCount)
                //        crossingCurvePoint.index = curListCount - 1;
                //}
                //crossingCurvePoint.easting = curve.curList[crossingCurvePoint.index].easting;
                //crossingCurvePoint.northing = curve.curList[crossingCurvePoint.index].northing;
                //crossingCurvePoint.heading = curve.curList[crossingCurvePoint.index].heading;
                return true;
            }

            //keep moving infield till pattern is all inside
            if (curve.isABSameAsVehicleHeading)
            {
                crossingCurvePoint.index--;
                if (crossingCurvePoint.index < 0) crossingCurvePoint.index = 0;
            }
            else
            {
                crossingCurvePoint.index++;
                if (crossingCurvePoint.index >= curListCount)
                    crossingCurvePoint.index = curListCount - 1;
            }
            crossingCurvePoint.easting = curve.curList[crossingCurvePoint.index].easting;
            crossingCurvePoint.northing = curve.curList[crossingCurvePoint.index].northing;
            crossingCurvePoint.heading = curve.curList[crossingCurvePoint.index].heading;

            double tooClose = glm::distance(ytList[0], pivotPos);
            isTurnCreationTooClose = tooClose < 3;

            //set the flag to Critical stop machine
            if (isTurnCreationTooClose) emit outOfBounds(); //mf.mc.isOutOfBounds = true;
            break;
            }

        case 2:
            youTurnPhase = 3;
            break;
    }
    return true;

}

//called to initiate turn
void CYouTurn::youTurnTrigger()
{
    //trigger pulled
    isYouTurnTriggered = true;

    emit setTriggerSequence(true);
    //mf.seq.isSequenceTriggered = true;

    //just do the opposite of last turn
    isYouTurnRight = !isLastYouTurnRight;
    isLastYouTurnRight = !isLastYouTurnRight;
}

//Normal copmpletion of youturn
void CYouTurn::completeYouTurn()
{
    isYouTurnTriggered = false;
    resetCreatedYouTurn();
    //mf.seq.ResetSequenceEventTriggers();
    emit resetSequenceEventTriggers();
    //mf.seq.isSequenceTriggered = false;
    emit setTriggerSequence(false);
    //mf.isBoundAlarming = false;
    emit turnOffBoundAlarm();
}

//something went seriously wrong so reset everything
void CYouTurn::resetYouTurn()
{
    //fix you turn
    isYouTurnTriggered = false;
    ytList.clear();
    resetCreatedYouTurn();
    turnDistanceAdjuster = 0;
    //mf.isBoundAlarming = false;
    emit turnOffBoundAlarm();

    isTurnCreationTooClose = false;
    isTurnCreationNotCrossingError = false;

    //reset sequence
    emit resetSequenceEventTriggers();
    //mf.seq.ResetSequenceEventTriggers();
    emit setTriggerSequence(false);
    //mf.seq.isSequenceTriggered = false;
}

void CYouTurn::resetCreatedYouTurn()
{
    turnDistanceAdjuster = 0;
    youTurnPhase = 0;
    ytList.clear();
}


void CYouTurn::loadYouTurnShapeFromFile(QString filename)
{
    //if there is existing shape, delete it
    if (youFileList.size() > 0) youFileList.clear();

    QFile File(filename);

    if (!File.exists())
    {
        emit showMessage(messageBox::warning, tr("Missing Youturn file!"), tr("Restore") + " " +filename);
    } else {
        if (!File.open(QIODevice::ReadOnly | QIODevice::Text)) {
            //popup error message about not being able to read file
            emit showMessage(messageBox::warning, tr("Cannot open Youturn file!"), filename + " " + tr("is unreadable or corrupt."));
            return;
        }

        QTextStream reader(&File);

        QString line = reader.readLine();
        if(line.isNull()) {
            emit showMessage(messageBox::warning, tr("Cannot open Youturn file!"), filename + " " + tr("is unreadable or corrupt."));
            return;
        }

        int points = line.toInt();

        if (points) {
            Vec2 coords;

            for (int v=0; v < points; v++) {
                line = reader.readLine();

                if(line.isNull()) {
                    emit showMessage(messageBox::warning, tr("Cannot open Youturn file!"), filename + " " + tr("is unreadable or corrupt."));
                    return;
                }

                QStringList words = line.split(',');

                coords.easting = words[0].toDouble();
                coords.northing = words[1].toDouble();
                youFileList.append(coords);
            }
        }
    }
}

//build the points and path of youturn to be scaled and transformed
void CYouTurn::buildManualYouTurn(const CVehicle &vehicle, CTool &tool,
                                  const CABLine &ABLine, CABCurve &curve,
                                  bool isTurnRight, bool isTurnButtonTriggered)
{
    isYouTurnTriggered = true;

    double delta, head;
    //point on AB line closest to pivot axle point from ABLine PurePursuit
    if (ABLine.isABLineSet)
    {
        rEastYT = ABLine.rEastAB;
        rNorthYT = ABLine.rNorthAB;
        isABSameAsFixHeading = ABLine.isABSameAsVehicleHeading;
        head = ABLine.abHeading;
        delta = 1;
    }
    else
    {
        rEastYT = curve.rEastCu;
        rNorthYT = curve.rNorthCu;
        isABSameAsFixHeading = curve.isSameWay;
        head = curve.refHeading;
        delta = curve.deltaOfRefAndAveHeadings;
    }

    //grab the vehicle widths and offsets
    double widthMinusOverlap = tool.toolWidth - tool.toolOverlap;
    double toolOffset = tool.toolOffset * 2.0;
    double turnOffset;

    //turning right
    if (isTurnRight) turnOffset = (widthMinusOverlap + toolOffset);
    else turnOffset = (widthMinusOverlap - toolOffset);

    //to compensate for AB Curve overlap
    turnOffset *= delta;

    //if using dubins to calculate youturn
    //if (isUsingDubinsTurn)
    {
        CDubins dubYouTurnPath;
        CDubinsTurningRadius = vehicle.minTurningRadius;

        //if its straight across it makes 2 loops instead so goal is a little lower then start
        if (!isABSameAsFixHeading) head += 3.14;
        else head -= 0.01;

        //move the start forward 2 meters, this point is critical to formation of uturn
        rEastYT += (sin(head) * 2);
        rNorthYT += (cos(head) * 2);

        //now we have our start point
        Vec3 start(rEastYT, rNorthYT, head);
        Vec3 goal;

        turnOffset *= rowSkipsWidth;

        //now we go the other way to turn round
        head -= M_PI;
        if (head < 0) head += glm::twoPI;

        //set up the goal point for Dubins
        goal.heading = head;
        if (isTurnButtonTriggered)
        {
            if (isTurnRight)
            {
                goal.easting = rEastYT - (cos(-head) * turnOffset);
                goal.northing = rNorthYT - (sin(-head) * turnOffset);
            }
            else
            {
                goal.easting = rEastYT + (cos(-head) * turnOffset);
                goal.northing = rNorthYT + (sin(-head) * turnOffset);
            }
        }

        //generate the turn points
        ytList = dubYouTurnPath.GenerateDubins(start, goal);

        Vec3 pt;
        for (int a = 0; a < 3; a++)
        {
            pt.easting = ytList[0].easting + (sin(head));
            pt.northing = ytList[0].northing + (cos(head));
            pt.heading = ytList[0].heading;
            ytList.insert(0, pt);
        }

        int count = ytList.size();

        for (int i = 1; i <= 7; i++)
        {
            pt.easting = ytList[count - 1].easting + (sin(head) * i);
            pt.northing = ytList[count - 1].northing + (cos(head) * i);
            pt.heading = head;
            ytList.append(pt);
        }
    }
}

//determine distance from youTurn guidance line
void CYouTurn::distanceFromYouTurnLine(CVehicle &vehicle, CNMEA &pn)
{
   //grab a copy from main - the steer position
    double minDistA = 1000000, minDistB = 1000000;
    int ptCount = ytList.size();

    if (ptCount > 0)
    {
        if (vehicle.isStanleyUsed)
        {
            pivot = vehicle.steerAxlePos;

            //find the closest 2 points to current fix
            for (int t = 0; t < ptCount; t++)
            {
                double dist = ((pivot.easting - ytList[t].easting) * (pivot.easting - ytList[t].easting))
                                + ((pivot.northing - ytList[t].northing) * (pivot.northing - ytList[t].northing));
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

            //just need to make sure the points continue ascending or heading switches all over the place
            if (A > B) { C = A; A = B; B = C; }

            minDistA = 100;
            int closestPt = 0;
            for (int i = 0; i < ptCount; i++)
            {
                double distancePiv = glm::distance(ytList[i], pivot);
                if (distancePiv < minDistA)
                {
                    minDistA = distancePiv;
                    closestPt = i;
                }
            }

            //used for sequencing to find entry, exit positioning
            onA = ptCount / 2;
            if (closestPt < onA) onA = -closestPt;
            else onA = ptCount - closestPt;

            //return and reset if too far away or end of the line
            if (B >= ptCount - 1)
            {
                completeYouTurn();
                return;
            }

            //feed forward to turn faster
            A++;
            B++;

            //get the distance from currently active AB line, precalc the norm of line
            double dx = ytList[B].easting - ytList[A].easting;
            double dz = ytList[B].northing - ytList[A].northing;
            if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dz) < glm::DOUBLE_EPSILON) return;

            double abHeading = ytList[A].heading;

            //how far from current AB Line is steer point 90 degrees from steer position
            distanceFromCurrentLine = ((dz * pivot.easting) - (dx * pivot.northing) + (ytList[B].easting
                        * ytList[A].northing) - (ytList[B].northing * ytList[A].easting))
                            / sqrt((dz * dz) + (dx * dx));

            //are we on the right side or not, the sign from above determines that
            isOnRightSideCurrentLine = distanceFromCurrentLine > 0;

            //Calc point on ABLine closest to current position and 90 degrees to segment heading
            double U = (((pivot.easting - ytList[A].easting) * dx)
                        + ((pivot.northing - ytList[A].northing) * dz))
                        / ((dx * dx) + (dz * dz));

            //critical point used as start for the uturn path - critical
            rEastYT = ytList[A].easting + (U * dx);
            rNorthYT = ytList[A].northing + (U * dz);

            //the first part of stanley is to extract heading error
            double abFixHeadingDelta = (pivot.heading - abHeading);

            //Fix the circular error - get it from -Pi/2 to Pi/2
            if (abFixHeadingDelta > M_PI) abFixHeadingDelta -= M_PI;
            else if (abFixHeadingDelta < M_PI) abFixHeadingDelta += M_PI;
            if (abFixHeadingDelta > glm::PIBy2) abFixHeadingDelta -= M_PI;
            else if (abFixHeadingDelta < -glm::PIBy2) abFixHeadingDelta += M_PI;

            //normally set to 1, less then unity gives less heading error.
            abFixHeadingDelta *= vehicle.stanleyHeadingErrorGain;
            if (abFixHeadingDelta > 0.74) abFixHeadingDelta = 0.74;
            if (abFixHeadingDelta < -0.74) abFixHeadingDelta = -0.74;

            //the non linear distance error part of stanley
            steerAngleYT = atan((distanceFromCurrentLine * vehicle.stanleyGain) / ((pn.speed * 0.277777) + 1));

            //clamp it to max 42 degrees
            if (steerAngleYT > 0.74) steerAngleYT = 0.74;
            if (steerAngleYT < -0.74) steerAngleYT = -0.74;

            //add them up and clamp to max in vehicle settings
            steerAngleYT = glm::toDegrees((steerAngleYT + abFixHeadingDelta) * -1.0);
            if (steerAngleYT < -vehicle.maxSteerAngle) steerAngleYT = -vehicle.maxSteerAngle;
            if (steerAngleYT > vehicle.maxSteerAngle) steerAngleYT = vehicle.maxSteerAngle;

            //Convert to millimeters and round properly to above/below .5
            distanceFromCurrentLine = glm::roundAwayFromZero(distanceFromCurrentLine * 1000.0);

            //every guidance method dumps into these that are used and sent everywhere, last one wins
            vehicle.guidanceLineDistanceOff = vehicle.distanceDisplay = (int)distanceFromCurrentLine;
            vehicle.guidanceLineSteerAngle = (int)(steerAngleYT * 100);
        }
        else
        {
            pivot = vehicle.pivotAxlePos;

            //find the closest 2 points to current fix
            for (int t = 0; t < ptCount; t++)
            {
                double dist = ((pivot.easting - ytList[t].easting) * (pivot.easting - ytList[t].easting))
                                + ((pivot.northing - ytList[t].northing) * (pivot.northing - ytList[t].northing));
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

            //just need to make sure the points continue ascending or heading switches all over the place
            if (A > B) { C = A; A = B; B = C; }

            minDistA = 100;
            int closestPt = 0;
            for (int i = 0; i < ptCount; i++)
            {
                double distancePiv = glm::distance(ytList[i], vehicle.pivotAxlePos);
                if (distancePiv < minDistA)
                {
                    minDistA = distancePiv;
                    closestPt = i;
                }
            }

            onA = ptCount / 2;
            if (closestPt < onA)
            {
                onA = -closestPt;
            }
            else
            {
                onA = ptCount - closestPt;
            }

            //return and reset if too far away or end of the line
            if (B >= ptCount - 1)
            {
                completeYouTurn();
                return;
            }

            //get the distance from currently active AB line
            double dx = ytList[B].easting - ytList[A].easting;
            double dz = ytList[B].northing - ytList[A].northing;
            if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dz) < glm::DOUBLE_EPSILON) return;

            //abHeading = atan2(dz, dx);
            //double abHeading = ytList[A].heading;

            //how far from current AB Line is fix
            distanceFromCurrentLine = ((dz * pivot.easting) - (dx * pivot.northing) + (ytList[B].easting
                        * ytList[A].northing) - (ytList[B].northing * ytList[A].easting))
                            / sqrt((dz * dz) + (dx * dx));

            //are we on the right side or not
            isOnRightSideCurrentLine = distanceFromCurrentLine > 0;

            //absolute the distance
            distanceFromCurrentLine = fabs(distanceFromCurrentLine);

            // ** Pure pursuit ** - calc point on ABLine closest to current position
            double U = (((pivot.easting - ytList[A].easting) * dx)
                        + ((pivot.northing - ytList[A].northing) * dz))
                        / ((dx * dx) + (dz * dz));

            rEastYT = ytList[A].easting + (U * dx);
            rNorthYT = ytList[A].northing + (U * dz);

            //update base on autosteer settings and distance from line
            double goalPointDistance = vehicle.updateGoalPointDistance(pn,distanceFromCurrentLine);

            //sharp turns on you turn.
            goalPointDistance = vehicle.goalPointLookAheadUturnMult * goalPointDistance;
            //emit setLookaheadGoal(goalPointDistance);
            //mf.lookaheadActual = goalPointDistance; //unused

            //used for accumulating distance to find goal point
            double distSoFar;

            isABSameAsFixHeading = true;
            distSoFar = glm::distance(ytList[B], rEastYT, rNorthYT);

            // used for calculating the length squared of next segment.
            double tempDist = 0.0;

            //Is this segment long enough to contain the full lookahead distance?
            if (distSoFar > goalPointDistance)
            {
                //treat current segment like an AB Line
                goalPointYT.easting = rEastYT + (sin(ytList[A].heading) * goalPointDistance);
                goalPointYT.northing = rNorthYT + (cos(ytList[A].heading) * goalPointDistance);
            }

            //multiple segments required
            else
            {
                //cycle thru segments and keep adding lengths. check if end and break if so.
                while (B < ptCount - 1)
                {
                    B++; A++;
                    tempDist = glm::distance(ytList[B], ytList[A]);
                    if ((tempDist + distSoFar) > goalPointDistance) break; //will we go too far?
                    distSoFar += tempDist;
                }

                double t = (goalPointDistance - distSoFar); // the remainder to yet travel
                t /= tempDist;
                goalPointYT.easting = (((1 - t) * ytList[A].easting) + (t * ytList[B].easting));
                goalPointYT.northing = (((1 - t) * ytList[A].northing) + (t * ytList[B].northing));
            }

            //calc "D" the distance from pivot axle to lookahead point
            double goalPointDistanceSquared = glm::distanceSquared(goalPointYT.northing, goalPointYT.easting, pivot.northing, pivot.easting);

            //calculate the the delta x in local coordinates and steering angle degrees based on wheelbase
            double localHeading = glm::twoPI - vehicle.fixHeading;
            ppRadiusYT = goalPointDistanceSquared / (2 * (((goalPointYT.easting - pivot.easting) * cos(localHeading)) + ((goalPointYT.northing - pivot.northing) * sin(localHeading))));

            steerAngleYT = glm::toDegrees(atan(2 * (((goalPointYT.easting - pivot.easting) * cos(localHeading))
                + ((goalPointYT.northing - pivot.northing) * sin(localHeading))) * vehicle.wheelbase / goalPointDistanceSquared));

            if (steerAngleYT < -vehicle.maxSteerAngle) steerAngleYT = -vehicle.maxSteerAngle;
            if (steerAngleYT > vehicle.maxSteerAngle) steerAngleYT = vehicle.maxSteerAngle;

            if (ppRadiusYT < -500) ppRadiusYT = -500;
            if (ppRadiusYT > 500) ppRadiusYT = 500;

            radiusPointYT.easting = pivot.easting + (ppRadiusYT * cos(localHeading));
            radiusPointYT.northing = pivot.northing + (ppRadiusYT * sin(localHeading));

            //angular velocity in rads/sec  = 2PI * m/sec * radians/meters
            double angVel = glm::twoPI * 0.277777 * pn.speed * (tan(glm::toRadians(steerAngleYT))) / vehicle.wheelbase;

            //clamp the steering angle to not exceed safe angular velocity
            if (fabs(angVel) > vehicle.maxAngularVelocity)
            {
                steerAngleYT = glm::toDegrees(steerAngleYT > 0 ?
                        (atan((vehicle.wheelbase * vehicle.maxAngularVelocity) / (glm::twoPI * pn.speed * 0.277777)))
                    : (atan((vehicle.wheelbase * -vehicle.maxAngularVelocity) / (glm::twoPI * pn.speed * 0.277777))));
            }
            //Convert to centimeters
            distanceFromCurrentLine = glm::roundAwayFromZero(distanceFromCurrentLine * 1000.0);

            //distance is negative if on left, positive if on right
            //if you're going the opposite direction left is right and right is left
            if (isABSameAsFixHeading)
            {
                if (!isOnRightSideCurrentLine) distanceFromCurrentLine *= -1.0;
            }

            //opposite way so right is left
            else
            {
                if (isOnRightSideCurrentLine) distanceFromCurrentLine *= -1.0;
            }

            vehicle.guidanceLineDistanceOff = vehicle.distanceDisplay = (int)distanceFromCurrentLine;
            vehicle.guidanceLineSteerAngle = (int)(steerAngleYT * 100);
        }
    }
    else
    {
        completeYouTurn();
    }
}

//Duh.... What does this do....
void CYouTurn::drawYouTurn(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    USE_SETTINGS;

    GLHelperOneColor gldraw;

    int ptCount = ytList.size();
    if (ptCount < 3) return;

    if (isYouTurnTriggered)
    {
        for (int i = 0; i < ptCount; i++)
        {
            gldraw.append(QVector3D(ytList[i].easting, ytList[i].northing, 0));
        }
        gldraw.draw(gl, mvp, QColor::fromRgbF(0.95f, 0.95f, 0.25f),
                    GL_POINTS, SETTINGS_DISPLAY_LINEWIDTH);
    }
    else
    {
        QColor color;
        if (!isOutOfBounds)
            color = QColor::fromRgbF(0.395f, 0.925f, 0.30f);
        else
            color = QColor::fromRgbF(0.9495f, 0.395f, 0.325f);

        for (int i = 0; i < ptCount; i++)
        {
            gldraw.append(QVector3D(ytList[i].easting, ytList[i].northing, 0));
        }

        gldraw.draw(gl, mvp, color, GL_POINTS, SETTINGS_DISPLAY_LINEWIDTH);
    }
}
