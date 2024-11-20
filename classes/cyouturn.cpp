#include <limits>
#include <math.h>
#include <QFile>
#include "cyouturn.h"
#include "aogproperty.h"
#include "cabline.h"
#include "cvehicle.h"
#include "cnmea.h"
#include "cabline.h"
#include "cboundary.h"
#include "cabcurve.h"
#include "glm.h"
#include "cdubins.h"
#include "glutils.h"
//#include "common.h"

//constructor
CYouTurn::CYouTurn(QObject *parent) : QObject(parent)
{
    //how far before or after boundary line should turn happen
    loadSettings();
}

void CYouTurn::loadSettings()
{
    uturnDistanceFromBoundary = property_set_youTurnDistanceFromBoundary;

    //how far before or after boundary line should turn happen
    youTurnStartOffset = property_set_youTurnExtensionLength;

    rowSkipsWidth = property_set_youSkipWidth;
    Set_Alternate_skips();

	ytList.capacity(128);

    youTurnRadius = property_set_youTurnRadius;

    uTurnStyle = property_set_uTurnStyle;

    uTurnSmoothing = property_setAS_uTurnSmoothing;

}

//Finds the point where an AB Curve crosses the turn line
bool CYouTurn::BuildCurveDubinsYouTurn(CVehicle &vehicle,
                                       const CBoundary &bnd,
                                       const CABCurve &curve,
                                       bool isTurnRight, Vec3 pivotPos)
{
    double minTurningRadius = property_setVehicle_minTurningRadius;
    double tool_toolWidth = property_setVehicle_toolWidth;
    double tool_toolOverlap = property_setVehicle_toolOverlap;
    double tool_toolOffset = property_setVehicle_toolOffset;

    //TODO: is calculated many taimes after the priveous turn is complete
    //grab the vehicle widths and offsets
    double turnOffset = (tool_toolWidth - tool_toolOverlap) * rowSkipsWidth + (isYouTurnRight ? -tool_toolOffset * 2.0 : tool_toolOffset * 2.0); //AAA change isYouTurnRight to isTurnLeft
    pointSpacing = youTurnRadius * 0.1;

    if (uTurnStyle == 0)
    {
        //Albin turn
        if (turnOffset > (youTurnRadius * 2.0))
        {
            return CreateCurveWideTurn(isTurnLeft, pivotPos);
        }

        //Ohmega turn
        else
        {
            return CreateCurveOmegaTurn(isTurnLeft, pivotPos);
        }
    }
    else if (uTurnStyle == 1)
    {
        return (KStyleTurnCurve(isTurnLeft));
    }

    //prgramming error if you got here
    return false;
}

bool CYouTurn::BuildABLineDubinsYouTurn(bool isTurnLeft,
                                        const CVehicle &vehicle,
                                        const CABLine &ABLine)
{
    double tool_toolWidth = property_setVehicle_toolWidth;
    double tool_toolOverlap = property_setVehicle_toolOverlap;
    double tool_toolOffset = property_setVehicle_toolOffset;

    if (!(bool)isBtnAutoSteerOn) ABLine.isHeadingSameWay
            = M_PI - fabs(fabs(vehicle.fixHeading - ABLine.abHeading) - M_PI) < glm::PIBy2;

    double turnOffset = (tool_toolWidth - tool_toolOverlap) * rowSkipsWidth
                        + (isYouTurnRight ? -tool_toolOffset * 2.0 : tool_toolOffset * 2.0);

    pointSpacing = youTurnRadius * 0.1;

    if (uTurnStyle == 0)
    {

        //Wide turn
        if (turnOffset > (youTurnRadius * 2.0))
        {
            return (CreateABWideTurn(isTurnLeft));
        }
        //Small turn
        else
        {
            return (CreateABOmegaTurn(isTurnLeft));
        }
    }
    else if (uTurnStyle == 1)
    {
        return (KStyleTurnAB(isTurnLeft));
    }

    //prgramming error if you got here
    return false;
}


bool CYouTurn::CreateCurveOmegaTurn(bool isTurnLeft, Vec3 pivotPos,
                                    int makeUTurnCounter,
                                    CVehicle &vehicle,
                                    const CBoundary &bnd,
                                    const CABCurve &curve)
{
    double tool_toolWidth = property_setVehicle_toolWidth;
    double tool_toolOverlap = property_setVehicle_toolOverlap;
    double tool_toolOffset = property_setVehicle_toolOffset;

    //keep from making turns constantly - wait 1.5 seconds
    if (makeUTurnCounter < 4)
    {
        youTurnPhase = 0;
        return true;
    }

    //grab the vehicle widths and offsets
    double turnOffset = (tool_toolWidth - tool_toolOverlap) * rowSkipsWidth + (isYouTurnRight ? -tool_toolOffset * 2.0 : tool_toolOffset * 2.0);

    switch (youTurnPhase)
    {
    case 0: //find the crossing points
        if (!FindCurveTurnPoint(curve))
        {
            FailCreate();
            return false;
        }

        //save a copy
        inClosestTurnPt = CClose(closestTurnPt);

        ytList.clear();

        int count = curve.isHeadingSameWay ? -1 : 1;
        int curveIndex = inClosestTurnPt.curveIndex;

        isOutOfBounds = true;
        int stopIfWayOut = 0;

        double head = 0; //gives error since isnt updated

        while (isOutOfBounds)
        {
            stopIfWayOut++;
            isOutOfBounds = false;

            //creates half a circle starting at the crossing point
            ytList.clear();

            curveIndex += count;

            Vec3 currentPos = curve.curList[curveIndex];

            if (!curve.isHeadingSameWay) currentPos.heading += M_PI;
            if (currentPos.heading >= glm::twoPI) currentPos.heading -= glm::twoPI;
            head = currentPos.heading;

            CDubins dubYouTurnPath;
            CDubins.turningRadius = youTurnRadius;

            //now we go the other way to turn round
            double invertHead = currentPos.heading - M_PI;
            if (invertHead <= -M_PI) invertHead += glm::twoPI;
            if (invertHead >= M_PI) invertHead -= glm::twoPI;

            Vec3 goal;

            //neat trick to not have to add pi/2
            if (isTurnLeft)
            {
                goal.easting = curve.curList[curveIndex - count].easting + (cos(-invertHead) * turnOffset);
                goal.northing = curve.curList[curveIndex - count].northing + (sin(-invertHead) * turnOffset);
            }
            else
            {
                goal.easting = curve.curList[curveIndex - count].easting - (cos(-invertHead) * turnOffset);
                goal.northing = curve.curList[curveIndex - count].northing - (sin(-invertHead) * turnOffset);
            }

            goal.heading = invertHead;

            //generate the turn points
            ytList = dubYouTurnPath.GenerateDubins(currentPos, goal);
            if (ytList.count() == 0)
            {
                FailCreate();
                return false;
            }

            if (stopIfWayOut == 300 || curveIndex < 1 || curveIndex > (curve.curList.count() - 2))
            {
                //for some reason it doesn't go inside boundary
                FailCreate();
                return false;
            }

            for (int i = 0; i < ytList.count(); i++)
            {
                if (bnd.IsPointInsideTurnArea(ytList[i]) != 0)
                {
                    isOutOfBounds = true;
                    break;
                }
            }
        }
        inClosestTurnPt.curveIndex = curveIndex;

        //too many points from Dubins - so cut
        double distance;
        int cnt = ytList.count();
        for (int i = 1; i < cnt - 2; i++)
        {
            distance = glm::DistanceSquared(ytList[i], ytList[i + 1]);
            if (distance < pointSpacing)
            {
                ytList.removeAt(i + 1);
                i--;
                cnt = ytList.count();
            }
        }

        //move the turn to exact at the turnline
        ytList = MoveTurnInsideTurnLine(ytList, head, false, false, vehicle, bnd);
        if (ytList.count() == 0)
        {
            FailCreate();
            return false;
        }

        youTurnPhase = 1;
        break;

    case 1:
        //build the next line to add sequencelines
        double headCurve = curve.curList[curve.currentLocationIndex].heading;
        if (!curve.isHeadingSameWay) headCurve += M_PI;
        if (headCurve > glm::twoPI) headCurve -= glm::twoPI;

        Vec2 tempguidanceLookPos = Vec2(vehicle.guidanceLookPos.easting, vehicle.guidanceLookPos.northing);

        if (!isTurnLeft)
        {
            //creates an imaginary curveline to the right
            vehicle.guidanceLookPos.easting += (cos(headCurve) * turnOffset);
            vehicle.guidanceLookPos.northing -= (sin(headCurve) * turnOffset);
        }
        else
        {
            vehicle.guidanceLookPos.easting -= (cos(headCurve) * turnOffset);
            vehicle.guidanceLookPos.northing += (sin(headCurve) * turnOffset);
        }

        //create the next line with this imaginary point
        nextCurve = CABCurve();
        nextCurve.BuildCurveCurrentList(vehicle.pivotAxlePos,secondsSinceStart,vehicle,trk,bnd,yt);
        vehicle.guidanceLookPos = tempguidanceLookPos;

        //get the index of the last yt point
        double dis = glm::DOUBLE_MAX;
        for (int i = 0; i < nextCurve.curList.count() - 2; i++)
        {
            double newdis = glm::Distance(nextCurve.curList[i], ytList[ytList.count() - 1]);
            if (newdis > dis)
            {
                if (curve.isHeadingSameWay) outClosestTurnPt.curveIndex = i - 2;
                else outClosestTurnPt.curveIndex = i;
                break;
            }
            else dis = newdis;
        }
        outClosestTurnPt.closePt = nextCurve.curList[outClosestTurnPt.curveIndex];
        inClosestTurnPt.closePt = curve.curList[inClosestTurnPt.curveIndex];

        if (!AddCurveSequenceLines()) return false;

        //fill in the gaps
        double distanc;

        int cnt4 = ytList.count();
        for (int i = 1; i < cnt4 - 2; i++)
        {
            int j = i + 1;
            if (j == cnt4 - 1) continue;
            distanc = glm::DistanceSquared(ytList[i], ytList[j]);
            if (distanc > 1)
            {
                Vec3 pointB = Vec3((ytList[i].easting + ytList[j].easting) / 2.0,
                                   (ytList[i].northing + ytList[j].northing) / 2.0, ytList[i].heading);

                ytList.insert(j, pointB);
                cnt4 = ytList.count();
                i--;
            }
        }

        //calculate the new points headings based on fore and aft of point - smoother turns
        QVector<Vec3> arr = ytList;
        cnt4 = ytList.count();
        cnt4 -= 2;
        ytList.clear();

        for (int i = 2; i < cnt4; i++)
        {
            Vec3 pt3 = arr[i];
            pt3.heading = atan2(arr[i + 1].easting - arr[i - 1].easting,
                                arr[i + 1].northing - arr[i - 1].northing);
            if (pt3.heading < 0) pt3.heading += glm::twoPI;
            ytList.append(pt3);
        }

        //check to close
        if (glm::Distance(ytList[0], vehicle.pivotAxlePos) < 3)
        {
            FailCreate();
            return false;
        }

        isOutOfBounds = false;
        youTurnPhase = 10;
        turnTooCloseTrigger = false;
        isTurnCreationTooClose = false;
        return true;
    }
    return true;
}

void CYouTurn::CreateCurveWideTurn(bool isTurnLeft, Vec3 pivotPos,
                                   int makeUTurnCounter,
                                   CVehicle &vehicle,
                                   const CBoundary &bnd,
                                   const CABCurve &curve)
{
    double tool_toolWidth = property_setVehicle_toolWidth;
    double tool_toolOverlap = property_setVehicle_toolOverlap;
    double tool_toolOffset = property_setVehicle_toolOffset;

    //keep from making turns constantly - wait 1.5 seconds
    if (makeUTurnCounter < 4)
    {
        youTurnPhase = 0;
        return true;
    }

    //we are doing a wide turn
    double head = 0;
    int count = curve.isHeadingSameWay ? -1 : 1;
    switch (youTurnPhase)
    {
    case 0:
        //Create first semicircle
        if (!FindCurveTurnPoint(curve))
        {
            //error
            FailCreate();
            return false;
        }
        inClosestTurnPt = CClose(closestTurnPt);
        startOfTurnPt = CClose(inClosestTurnPt);

        int stopIfWayOut = 0;
        isOutOfBounds = true;

        while (isOutOfBounds)
        {
            isOutOfBounds = false;
            stopIfWayOut++;

            Vec3 currentPos = curve.curList[inClosestTurnPt.curveIndex];

            head = currentPos.heading;
            if (!curve.isHeadingSameWay) head += M_PI;
            if (head > glm::twoPI) head -= glm::twoPI;
            currentPos.heading = head;

            // creates half a circle starting at the crossing point
            ytList.clear();
            ytList.append(currentPos);

            //Taken from Dubbins
            while (fabs(head - currentPos.heading) < M_PI) //fabs?? David
            {
                //Update the position of the car
                currentPos.easting += pointSpacing * sin(currentPos.heading);
                currentPos.northing += pointSpacing * cos(currentPos.heading);

                //Which way are we turning?
                double turnParameter = isTurnLeft ? -1.0 : 1.0;

                //Update the heading
                currentPos.heading += (pointSpacing / youTurnRadius) * turnParameter;

                //Add the new coordinate to the path
                ytList.append(currentPos);
            }

            int cnt4 = ytList.count();
            if (cnt4 == 0)
            {
                FailCreate();
                return false;
            }

            //Are we out of bounds?
            for (int j = 0; j < cnt4; j += 2)
            {
                if (bnd.IsPointInsideTurnArea(ytList[j]) != 0)
                {
                    isOutOfBounds = true;
                    break;
                }
            }

            //first check if not out of bounds, add a bit more to clear turn line, set to phase 2
            if (!isOutOfBounds)
            {
                ytList = MoveTurnInsideTurnLine(ytList, head, true, false, vehicle, bnd);
                if (ytList.count() == 0)
                {
                    FailCreate();
                    return false;
                }
                youTurnPhase = 1;
                return true;
            }

            if (stopIfWayOut == 300 || inClosestTurnPt.curveIndex < 1 || inClosestTurnPt.curveIndex > (curve.curList.count() - 2))
            {
                //for some reason it doesn't go inside boundary
                FailCreate();
                return false;
            }

            //keep moving infield till pattern is all inside
            inClosestTurnPt.curveIndex = inClosestTurnPt.curveIndex + count;
            inClosestTurnPt.closePt = curve.curList[inClosestTurnPt.curveIndex];


            //set the flag to Critical stop machine
            if (glm::Distance(ytList[0], vehicle.pivotAxlePos) < 3)
            {
                FailCreate();
                return false;
            }
        }

        return false;

    case 1:
        //Takes the heading of the curve to create an imaginary point on the next line
        double headCurve = curve.curList[curve.currentLocationIndex].heading;
        if (!curve.isHeadingSameWay) headCurve += M_PI;
        if (headCurve > glm::twoPI) headCurve -= glm::twoPI;

        double turnOffset = (tool_toolWidth - tool_toolOverlap) * rowSkipsWidth + (isYouTurnRight ? -tool_toolOffset * 2.0 : tool_toolOffset * 2.0); //change isYouTurnRight?
        Vec2 tempguidanceLookPos = Vec2(vehicle.guidanceLookPos.easting, vehicle.guidanceLookPos.northing);

        if (!isTurnLeft)
        {
            //creates an imaginary curveline to the right
            vehicle.guidanceLookPos.easting += (cos(headCurve) * turnOffset);
            vehicle.guidanceLookPos.northing -= (sin(headCurve) * turnOffset);
        }
        else
        {
            vehicle.guidanceLookPos.easting -= (cos(headCurve) * turnOffset);
            vehicle.guidanceLookPos.northing += (sin(headCurve) * turnOffset);
        }

        //create the next line with this imaginary point
        nextCurve = CABCurve();
        nextCurve.BuildCurveCurrentList(vehicle.pivotAxlePos,secondsSinceStart,vehicle,trk,bnd,yt);
        vehicle.guidanceLookPos = tempguidanceLookPos;


        //going with or against boundary?
        bool isTurnLineSameWay = true;
        double headingDifference = fabs(inClosestTurnPt.turnLineHeading - ytList[ytList.count() - 1].heading);
        if (headingDifference > glm::PIBy2 && headingDifference < 3 * glm::PIBy2) isTurnLineSameWay = false;

        if (!FindCurveOutTurnPoint(curve, nextCurve, startOfTurnPt, isTurnLineSameWay))
        {
            //error
            FailCreate();
            return false;
        }
        outClosestTurnPt = CClose(closestTurnPt);

        //move the turn inside of turnline with help from the crossingCurvePoints
        isOutOfBounds = true;
        while (isOutOfBounds)
        {
            isOutOfBounds = false;
            Vec3 currentPos = nextCurve.curList[outClosestTurnPt.curveIndex];

            head = currentPos.heading;
            if ((!curve.isHeadingSameWay && !isOutSameCurve) || (curve.isHeadingSameWay && isOutSameCurve)) head += M_PI;
            if (head > glm::twoPI) head -= glm::twoPI;
            currentPos.heading = head;

            // creates half a circle starting at the crossing point
            ytList2.clear();
            ytList2.append(currentPos);

            //Taken from Dubbins
            while (fabs(head - currentPos.heading) < M_PI)
            {
                //Update the position of the car
                currentPos.easting += pointSpacing * sin(currentPos.heading);
                currentPos.northing += pointSpacing * cos(currentPos.heading);

                //Which way are we turning?
                double turnParameter = isTurnLeft ? 1.0 : -1.0;

                //Update the heading
                currentPos.heading += (pointSpacing / youTurnRadius) * turnParameter;

                //Add the new coordinate to the path
                ytList2.append(currentPos);
            }

            int cnt3 = ytList2.count();
            if (cnt3 == 0)
            {
                FailCreate();
                return false;
            }

            //Are we out of bounds?
            for (int j = 0; j < cnt3; j += 2)
            {
                if (bnd.IsPointInsideTurnArea(ytList2[j]) != 0)
                {
                    isOutOfBounds = true;
                    break;
                }
            }

            //first check if not out of bounds, add a bit more to clear turn line, set to phase 2
            if (!isOutOfBounds)
            {
                ytList2 = MoveTurnInsideTurnLine(ytList2, head, true, true, vehicle, bnd);
                if (ytList2.count() == 0)
                {
                    FailCreate();
                    return false;
                }
                youTurnPhase = 2;
                return true;
            }

            if (outClosestTurnPt.curveIndex < 1 || outClosestTurnPt.curveIndex > (nextCurve.curList.count() - 2))
            {
                //for some reason it doesn't go inside boundary
                FailCreate();
                return false;
            }

            //keep moving infield till pattern is all inside
            if (!isOutSameCurve) outClosestTurnPt.curveIndex = outClosestTurnPt.curveIndex + count;
            else outClosestTurnPt.curveIndex = outClosestTurnPt.curveIndex - count;
            outClosestTurnPt.closePt = nextCurve.curList[outClosestTurnPt.curveIndex];

        }
        return false;

    case 2:
        //Bind the two turns together
        int cnt1 = ytList.count();
        int cnt2 = ytList2.count();

        //Find if the turn goes same way as turnline heading
        bool isFirstTurnLineSameWay = true;
        double firstHeadingDifference = fabs(inClosestTurnPt.turnLineHeading - ytList[ytList.count() - 1].heading);
        if (firstHeadingDifference > glm::PIBy2 && firstHeadingDifference < 3 * glm::PIBy2) isFirstTurnLineSameWay = false;

        //finds out start and goal point along the tunline
        FindInnerTurnPoints(ytList[cnt1 - 1], ytList[0].heading, inClosestTurnPt, isFirstTurnLineSameWay);
        CClose startClosestTurnPt = CClose(closestTurnPt);

        FindInnerTurnPoints(ytList2[cnt2 - 1], ytList2[0].heading + M_PI, outClosestTurnPt, !isFirstTurnLineSameWay);
        CClose goalClosestTurnPt = CClose(closestTurnPt);

        //we have 2 different turnLine crossings
        if (startClosestTurnPt.turnLineNum != goalClosestTurnPt.turnLineNum)
        {
            FailCreate();
            return false;
        }

        //segment index is the "A" of the segment. segmentIndex+1 would be the "B"
        //is in and out on same segment? so only 1 segment
        if (startClosestTurnPt.turnLineIndex == goalClosestTurnPt.turnLineIndex)
        {
            for (int a = 0; a < cnt2; cnt2--)
            {
                ytList.append(new vec3(ytList2[cnt2 - 1]));
            }
        }
        else
        {
            //mulitple segments
            Vec3 tPoint;
            int turnCount = bnd.bndList[startClosestTurnPt.turnLineNum].turnLine.count();

            //how many points from turnline do we add
            int loops = fabs(startClosestTurnPt.turnLineIndex - goalClosestTurnPt.turnLineIndex);

            //are we crossing a border?
            if (loops > (bnd.bndList[startClosestTurnPt.turnLineNum].turnLine.count() / 2))
            {
                if (startClosestTurnPt.turnLineIndex < goalClosestTurnPt.turnLineIndex)
                {
                    loops = (turnCount - goalClosestTurnPt.turnLineIndex) + startClosestTurnPt.turnLineIndex;
                }
                else
                {
                    loops = (turnCount - startClosestTurnPt.turnLineIndex) + goalClosestTurnPt.turnLineIndex;
                }
            }

            //count up - start with B which is next A
            if (isFirstTurnLineSameWay)
            {
                for (int i = 0; i < loops; i++)
                {
                    if ((startClosestTurnPt.turnLineIndex + 1) >= turnCount) startClosestTurnPt.turnLineIndex = -1;

                    tPoint = bnd.bndList[startClosestTurnPt.turnLineNum].turnLine[startClosestTurnPt.turnLineIndex + 1];
                    startClosestTurnPt.turnLineIndex++;
                    if (startClosestTurnPt.turnLineIndex >= turnCount)
                        startClosestTurnPt.turnLineIndex = 0;
                    ytList.append(tPoint);
                }
            }
            else //count down = start with A
            {
                for (int i = 0; i < loops; i++)
                {
                    tPoint = bnd.bndList[startClosestTurnPt.turnLineNum].turnLine[startClosestTurnPt.turnLineIndex];
                    startClosestTurnPt.turnLineIndex--;
                    if (startClosestTurnPt.turnLineIndex == -1)
                        startClosestTurnPt.turnLineIndex = turnCount - 1;
                    ytList.append(tPoint);
                }
            }

            //add the out from ytList2
            for (int a = 0; a < cnt2; cnt2--)
            {
                ytList.append(ytList2[cnt2 - 1]);
            }
        }

        if (!AddCurveSequenceLines()) return false;

        //fill in the gaps
        double distance;

        int cnt = ytList.count();
        for (int i = 1; i < cnt - 2; i++)
        {
            int j = i + 1;
            if (j == cnt - 1) continue;
            distance = glm::DistanceSquared(ytList[i], ytList[j]);
            if (distance > 1)
            {
                Vec3 pointB = Vec3((ytList[i].easting + ytList[j].easting) / 2.0,
                                   (ytList[i].northing + ytList[j].northing) / 2.0, ytList[i].heading);

                ytList.insert(j, pointB);
                cnt = ytList.count();
                i--;
            }
        }

        //calculate the new points headings based on fore and aft of point - smoother turns
        cnt = ytList.count();
        QVector<Vec3> arr = ytList;
        cnt -= 2;
        ytList.clear();

        for (int i = 2; i < cnt; i++)
        {
            Vec3 pt3 = arr[i];
            pt3.heading = atan2(arr[i + 1].easting - arr[i - 1].easting,
                                arr[i + 1].northing - arr[i - 1].northing);
            if (pt3.heading < 0) pt3.heading += glm::twoPI;
            ytList.append(pt3);
        }

        //check to close
        if (glm::Distance(ytList[0], vehicle.pivotAxlePos) < 3)
        {
            FailCreate();
            return false;
        }

        //are we continuing the same way?
        isGoingStraightThrough = M_PI - fabs(fabs(ytList[ytList.count() - 2].heading - ytList[1].heading) - M_PI) < glm::PIBy2;
        ytList2.clear();
        isOutOfBounds = false;
        youTurnPhase = 10;
        turnTooCloseTrigger = false;
        isTurnCreationTooClose = false;
        return true;
    }

    // just in case
    return false;
}

bool CYouTurn::CreateABOmegaTurn(bool isTurnLeft,
                                 int makeUTurnCounter,
                                 CVehicle &vehicle,
                                 const CBoundary &bnd,
                                 const CABLine &ABLine)
{
    double tool_toolWidth = property_setVehicle_toolWidth;
    double tool_toolOverlap = property_setVehicle_toolOverlap;
    double tool_toolOffset = property_setVehicle_toolOffset;

    //keep from making turns constantly - wait 1.5 seconds
    if (makeUTurnCounter < 4)
    {
        youTurnPhase = 0;
        return true;
    }

    double head = ABLine.abHeading;
    if (!ABLine.isHeadingSameWay) head += M_PI;
    if (head >= glm::twoPI) head -= glm::twoPI;

    //we are doing an omega turn
    switch (youTurnPhase)
    {
    case 0:
        //how far are we from any turn boundary
        FindABTurnPoint(Vec3(ABLine.rEastAB, ABLine.rNorthAB, head));

        //or did we lose the turnLine - we are on the highway cuz we left the outer/inner turn boundary
        if (closestTurnPt.turnLineIndex != -1)
        {
            //calculate the distance to the turnline
            vehicle.distancePivotToTurnLine = glm::Distance(pivotAxlePos, closestTurnPt.closePt);
        }
        else
        {
            //Full emergency stop code goes here, it thinks its auto turn, but its not!
            FailCreate();
            return false;
        }
        inClosestTurnPt = CClose(closestTurnPt);

        CDubins dubYouTurnPath = CDubins();
        CDubins.turningRadius = youTurnRadius;

        //grab the vehicle widths and offsets
        double turnOffset = (tool_toolWidth - tool_toolOverlap) * rowSkipsWidth + (isYouTurnRight ? -tool_toolOffset * 2.0 : tool_toolOffset * 2.0);

        Vec3 start = inClosestTurnPt.closePt;
        start.heading = head;

        Vec3 goal = start;

        //now we go the other way to turn round
        double invertedHead = head - M_PI;
        if (invertedHead < 0) invertedHead += glm::twoPI;
        if (invertedHead > glm::twoPI) invertedHead -= glm::twoPI;

        if (isTurnLeft)
        {
            goal.easting = goal.easting + (cos(-invertedHead) * turnOffset);
            goal.northing = goal.northing + (sin(-invertedHead) * turnOffset);
        }
        else
        {
            goal.easting = goal.easting - (cos(-invertedHead) * turnOffset);
            goal.northing = goal.northing - (sin(-invertedHead) * turnOffset);
        }

        goal.heading = invertedHead;

        //generate the turn points
        ytList = dubYouTurnPath.GenerateDubins(start, goal);
        isOutOfBounds = true;

        if (ytList.count() == 0)
        {
            FailCreate();
            return false;
        }

        youTurnPhase = 1;

        double distance;

        int cnt = ytList.count();
        for (int i = 1; i < cnt - 2; i++)
        {
            distance = glm::DistanceSquared(ytList[i], ytList[i + 1]);
            if (distance < pointSpacing)
            {
                ytList.removeAt(i + 1);
                i--;
                cnt = ytList.count();
            }
        }

        return true;

    case 1:
        //move out
        ytList = MoveTurnInsideTurnLine(ytList, head, false, false, vehicle, bnd);

        if (ytList.count() == 0)
        {
            FailCreate();
            return false;
        }

        isOutOfBounds = false;
        youTurnPhase = 10;
        turnTooCloseTrigger = false;
        isTurnCreationTooClose = false;

        if (!AddABSequenceLines()) return false;

        return true;
    }

    return true;
}

bool CYouTurn::CreateABWideTurn(bool isTurnLeft,
                                int makeUTurnCounter,
                                CVehicle &vehicle,
                                const CBoundary &bnd,
                                const CABLine &ABLine)
{
    //keep from making turns constantly - wait 1.5 seconds
    if (makeUTurnCounter < 4)
    {
        youTurnPhase = 0;
        return true;
    }

    if (!mf.isBtnAutoSteerOn) mf.ABLine.isHeadingSameWay
            = M_PI - fabs(fabs(mf.fixHeading - mf.ABLine.abHeading) - M_PI) < glm::PIBy2;

    double head = mf.ABLine.abHeading;
    if (!mf.ABLine.isHeadingSameWay) head += M_PI;
    if (head >= glm::twoPI) head -= glm::twoPI;


    //step 1 turn in to the turnline
    switch (youTurnPhase)
    {
    case 0:
        //grab the pure pursuit point right on ABLine
        vec3 onPurePoint = new vec3(mf.ABLine.rEastAB, mf.ABLine.rNorthAB, 0);

        //how far are we from any turn boundary
        FindABTurnPoint(onPurePoint);

        //save a copy for first point
        inClosestTurnPt = new CClose(closestTurnPt);
        startOfTurnPt = new CClose(closestTurnPt);

        //already no turnline
        if (inClosestTurnPt.turnLineIndex == -1)
        {
            FailCreate();
            return false;
        }

        //creates half a circle starting at the crossing point
        ytList.clear();
        vec3 currentPos = new vec3(inClosestTurnPt.closePt.easting, inClosestTurnPt.closePt.northing, head);
        ytList.append(currentPos);

        //Taken from Dubbins
        while (fabs(head - currentPos.heading) < M_PI)
        {
            //Update the position of the car
            currentPos.easting += pointSpacing * sin(currentPos.heading);
            currentPos.northing += pointSpacing * cos(currentPos.heading);

            //Which way are we turning?
            double turnParameter = isTurnLeft ? -1.0 : 1.0;

            //Update the heading
            currentPos.heading += (pointSpacing / youTurnRadius) * turnParameter;

            //Add the new coordinate to the path
            ytList.append(currentPos);
        }

        //move the half circle to tangent the turnline
        isOutOfBounds = true;
        ytList = MoveTurnInsideTurnLine(ytList, head, true, false);

        //if it couldn't be done this will trigger
        if (ytList.count() == 0)
        {
            FailCreate();
            return false;
        }

        mf.distancePivotToTurnLine = glm::Distance(ytList[0], mf.pivotAxlePos);

        youTurnPhase = 1;
        return true;

    case 1:
        //we move the turnline crossing point perpenicualar out from the ABline
        double turnOffset = (mf.tool.width - mf.tool.overlap) * rowSkipsWidth + (isYouTurnRight ? -mf.tool.offset * 2.0 : mf.tool.offset * 2.0);
        vec2 tempguidanceLookPos = new vec2(mf.guidanceLookPos.easting, mf.guidanceLookPos.northing);

        if (!isTurnLeft)
        {
            //creates an imaginary curveline
            mf.guidanceLookPos.easting += (cos(head) * turnOffset);
            mf.guidanceLookPos.northing -= (sin(head) * turnOffset);
        }
        else
        {
            mf.guidanceLookPos.easting -= (cos(head) * turnOffset);
            mf.guidanceLookPos.northing += (sin(head) * turnOffset);
        }

        nextLine = new CABLine(mf);
        nextLine.BuildCurrentABLineList(mf.pivotAxlePos);
        mf.guidanceLookPos = tempguidanceLookPos;

        //going with or against boundary?
        bool isTurnLineSameWay = true;
        double headingDifference = fabs(startOfTurnPt.closePt.heading - ytList[ytList.count() - 1].heading);
        if (headingDifference > glm::PIBy2 && headingDifference < 3 * glm::PIBy2) isTurnLineSameWay = false;

        if (!FindABOutTurnPoint(mf.ABLine, ref nextLine, inClosestTurnPt, isTurnLineSameWay))
        {
            //error
            FailCreate();
            return false;
        }
        outClosestTurnPt = new CClose(closestTurnPt);

        vec3 pointPos = new vec3(outClosestTurnPt.closePt.easting, outClosestTurnPt.closePt.northing, 0);
        double headie;
        if (!isOutSameCurve)
        {
            headie = head;
        }
        else
        {
            headie = head + M_PI;
            if (headie >= glm::twoPI) headie -= glm::twoPI;
        }
        pointPos.heading = headie;

        //step 3 create half cirkle in new list
        ytList2.clear();
        ytList2.append(pointPos);

        //Taken from Dubbins
        while (fabs(headie - pointPos.heading) < M_PI)
        {
            //Update the position of the car
            pointPos.easting += pointSpacing * sin(pointPos.heading);
            pointPos.northing += pointSpacing * cos(pointPos.heading);

            //Which way are we turning?
            double turnParameter = isTurnLeft ? 1.0 : -1.0;

            //Update the heading
            pointPos.heading += (pointSpacing / youTurnRadius) * turnParameter;

            //Add the new coordinate to the path
            ytList2.append(pointPos);
        }

        //move the half circle to tangent the turnline
        isOutOfBounds = true;
        ytList2 = MoveTurnInsideTurnLine(ytList2, headie, true, true);

        if (ytList2.count() == 0)
        {
            FailCreate();
            return false;
        }

        youTurnPhase = 2;
        return true;

    case 2:
        int cnt1 = ytList.count();
        int cnt2 = ytList2.count();

        //Find if the turn goes same way as turnline heading
        bool isFirstTurnLineSameWay = true;
        double firstHeadingDifference = fabs(inClosestTurnPt.turnLineHeading - ytList[ytList.count() - 1].heading);

        if (firstHeadingDifference > glm::PIBy2 && firstHeadingDifference < 3 * glm::PIBy2) isFirstTurnLineSameWay = false;

        //finds out start and goal point along the tunline
        FindInnerTurnPoints(ytList[cnt1 - 1], ytList[0].heading, inClosestTurnPt, isFirstTurnLineSameWay);
        CClose startClosestTurnPt = new CClose(closestTurnPt);

        FindInnerTurnPoints(ytList2[cnt2 - 1], ytList2[0].heading + M_PI, outClosestTurnPt, !isFirstTurnLineSameWay);
        CClose goalClosestTurnPt = new CClose(closestTurnPt);

        //we have 2 different turnLine crossings
        if (startClosestTurnPt.turnLineNum != goalClosestTurnPt.turnLineNum)
        {
            FailCreate();
            return false;
        }

        //segment index is the "A" of the segment. segmentIndex+1 would be the "B"
        //is in and out on same segment? so only 1 segment
        if (startClosestTurnPt.turnLineIndex == goalClosestTurnPt.turnLineIndex)
        {
            for (int a = 0; a < cnt2; cnt2--)
            {
                ytList.append(new vec3(ytList2[cnt2 - 1]));
            }

        }
        else
        {
            //multiple segments
            vec3 tPoint = new vec3();
            int turnCount = mf.bnd.bndList[startClosestTurnPt.turnLineNum].turnLine.count();

            //how many points from turnline do we add
            int loops = fabs(startClosestTurnPt.turnLineIndex - goalClosestTurnPt.turnLineIndex);

            //TODO: Generates error if trying to go around more than half of something...... Do we ever?
            //are we crossing a border?
            if (loops > (mf.bnd.bndList[startClosestTurnPt.turnLineNum].turnLine.count() / 2))
            {
                if (startClosestTurnPt.turnLineIndex < goalClosestTurnPt.turnLineIndex)
                {
                    loops = (turnCount - goalClosestTurnPt.turnLineIndex) + startClosestTurnPt.turnLineIndex;
                }
                else
                {
                    loops = (turnCount - startClosestTurnPt.turnLineIndex) + goalClosestTurnPt.turnLineIndex;
                }
            }

            //count up - start with B which is next A
            if (isFirstTurnLineSameWay)
            {
                for (int i = 0; i < loops; i++)
                {
                    if ((startClosestTurnPt.turnLineIndex + 1) >= turnCount) startClosestTurnPt.turnLineIndex = -1;

                    tPoint = mf.bnd.bndList[startClosestTurnPt.turnLineNum].turnLine[startClosestTurnPt.turnLineIndex + 1];
                    startClosestTurnPt.turnLineIndex++;
                    if (startClosestTurnPt.turnLineIndex >= turnCount)
                        startClosestTurnPt.turnLineIndex = 0;
                    ytList.append(tPoint);
                }
            }
            else //count down = start with A
            {
                for (int i = 0; i < loops; i++)
                {
                    tPoint = mf.bnd.bndList[startClosestTurnPt.turnLineNum].turnLine[startClosestTurnPt.turnLineIndex];
                    startClosestTurnPt.turnLineIndex--;
                    if (startClosestTurnPt.turnLineIndex == -1)
                        startClosestTurnPt.turnLineIndex = turnCount - 1;
                    ytList.append(tPoint);
                }
            }

            //add the out from ytList2
            for (int a = 0; a < cnt2; cnt2--)
            {
                ytList.append(new vec3(ytList2[cnt2 - 1]));
            }
        }

        //fill in the gaps
        double distance;

        int cnt = ytList.count();
        for (int i = 1; i < cnt - 2; i++)
        {
            int j = i + 1;
            if (j == cnt - 1) continue;
            distance = glm::DistanceSquared(ytList[i], ytList[j]);
            if (distance > 1)
            {
                vec3 pointB = new vec3((ytList[i].easting + ytList[j].easting) / 2.0,
                                       (ytList[i].northing + ytList[j].northing) / 2.0, ytList[i].heading);

                ytList.insert(j, pointB);
                cnt = ytList.count();
                i--;
            }
        }

        //calculate the new points headings based on fore and aft of point - smoother turns
		QVector<vec3> arr = ytList;
        cnt = ytList.count();
        cnt -= 2;
        ytList.clear();

        for (int i = 2; i < cnt; i++)
        {
            vec3 pt3 = new vec3(arr[i]);
            pt3.heading = atan2(arr[i + 1].easting - arr[i - 1].easting,
                                     arr[i + 1].northing - arr[i - 1].northing);
            if (pt3.heading < 0) pt3.heading += glm::twoPI;
            ytList.append(pt3);
        }

        //AddABSequenceLines
        //check to close
        if (glm::Distance(ytList[0], mf.pivotAxlePos) < 3)
        {
            FailCreate();
            return false;
        }

        //are we continuing the same way?
        isGoingStraightThrough = M_PI - fabs(fabs(ytList[ytList.count() - 2].heading - ytList[1].heading) - M_PI) < glm::PIBy2;

        isOutOfBounds = false;
        youTurnPhase = 10;
        turnTooCloseTrigger = false;
        isTurnCreationTooClose = false;
        ytList2.clear();

        if (!AddABSequenceLines()) return false;

        return true;
    }

    //just in case
    return false;

}

QVector<Vec3> &CYouTurn::MoveTurnInsideTurnLine(QVector<Vec3> uTurnList, double head, bool deleteSecondHalf, bool invertHeading,
                                      CVehicle &vehicle,
                                      const CBoundary &bnd)
{
    //step 1 make array out of the list so that we can modify the position
    double cosHead = cos(head);
    double sinHead = sin(head);

    QVector<Vec3> arr2 = uTurnList;

    uTurnList.clear();

    semiCircleIndex = -1;
    //step 2 move the turn inside with steps of 1 meter
    bool pointOutOfBnd = isOutOfBounds;
    int j = 0;
    int stopIfWayOut = 0;
    while (pointOutOfBnd)
    {
        stopIfWayOut++;
        pointOutOfBnd = false;
        vehicle.distancePivotToTurnLine = glm::Distance(arr2[0], vehicle.pivotAxlePos);

        for (int i = 0; i < cnt; i++)
        {
            arr2[i].easting -= (sinHead);
            arr2[i].northing -= (cosHead);
        }

        for (; j < cnt; j += 1)
        {
            if (bnd.IsPointInsideTurnArea(arr2[j]) != 0)
            {
                pointOutOfBnd = true;
                if (j > 0) j--;
                break;
            }
        }

        if (stopIfWayOut == 1000 || (vehicle.distancePivotToTurnLine < 3))
        {
            //for some reason it doesn't go inside boundary, return empty list
            return uTurnList;
        }
    }

    //step 3, we ar now inside turnline, move the turn forward until it hits the turnfence in steps of 0.1 meters
    while (!pointOutOfBnd)
    {
        for (int i = 0; i < cnt; i++)
        {
            arr2[i].easting += (sinHead * 0.1);
            arr2[i].northing += (cosHead * 0.1);
        }

        for (int a = 0; a < cnt; a++)
        {
            if (bnd.IsPointInsideTurnArea(arr2[a]) != 0)
            {
                semiCircleIndex = a;
                pointOutOfBnd = true;
                break;

            }
        }
    }

    //step 4, Should we delete the points after the one that is outside? and where the points made in the wrong direction?
    for (int i = 0; i < cnt; i++)
    {
        if (i == semiCircleIndex && deleteSecondHalf)
            break;
        if (invertHeading) arr2[i].heading += M_PI;
        if (arr2[i].heading >= glm::twoPI) arr2[i].heading -= glm::twoPI;
        else if (arr2[i].heading < 0) arr2[i].heading += glm::twoPI;
        uTurnList.append(arr2[i]);
    }

    //we have succesfully moved the turn inside
    isOutOfBounds = false;

    //if empty - no creation.
    return uTurnList;
}

/////////////////////////////////
void deleteme(){
    if (youTurnPhase > 0)
    {
        isHeadingSameWay = curve.isHeadingSameWay;

        double head = crossingCurvePoint.heading;
        if (!isHeadingSameWay) head += M_PI;

        //delta between AB heading and boundary closest point heading
        boundaryAngleOffPerpendicular = M_PI - fabs(fabs(crossingheading - head) - M_PI);
        boundaryAngleOffPerpendicular -= glm::PIBy2;
        boundaryAngleOffPerpendicular *= -1;
        if (boundaryAngleOffPerpendicular > 1.25) boundaryAngleOffPerpendicular = 1.25;
        if (boundaryAngleOffPerpendicular < -1.25) boundaryAngleOffPerpendicular = -1.25;

        //for calculating innner circles of turn
        double tangencyAngle = (glm::PIBy2 - fabs(boundaryAngleOffPerpendicular)) * 0.5;

        double distanceTurnBeforeLine;
        //distance from crossPoint to turn line
        if (youTurnRadius * 2 < (tool_toolWidth * rowSkipsWidth))
        {
            if (boundaryAngleOffPerpendicular < 0)
            {
                //which is actually left
                if (isYouTurnRight)
                    distanceTurnBeforeLine = (youTurnRadius * tan(tangencyAngle));//short
                else
                    distanceTurnBeforeLine = (youTurnRadius / tan(tangencyAngle)); //long
            }
            else
            {
                //which is actually left
                if (isYouTurnRight)
                    distanceTurnBeforeLine = (youTurnRadius / tan(tangencyAngle)); //long
                else
                    distanceTurnBeforeLine = (youTurnRadius * tan(tangencyAngle)); //short
            }
        }

        //turn Radius is wider then equipment width so ohmega turn
        else
        {
            distanceTurnBeforeLine = (2 * minTurningRadius);
        }

        CDubins dubYouTurnPath;
        CDubinsTurningRadius = youTurnRadius;

        //grab the vehicle widths and offsets
        double turnOffset = (tool_toolWidth - tool_toolOverlap) * rowSkipsWidth + (isYouTurnRight ? -tool_toolOffset * 2.0 : tool_toolOffset * 2.0);

        //diagonally across
        double turnRadius = turnOffset / cos(boundaryAngleOffPerpendicular);

        //start point of Dubins
        Vec3 start(crossingCurvePoint.easting, crossingCurvePoint.northing, head);
        Vec3 goal;
        //move the cross line calc to not include first turn
        goal.easting = crossingCurvePoint.easting + (sin(head) * distanceTurnBeforeLine);
        goal.northing = crossingCurvePoint.northing + (cos(head) * distanceTurnBeforeLine);

        //headland angle relative to vehicle heading to head along the boundary left or right
        double bndAngle = head - boundaryAngleOffPerpendicular + glm::PIBy2;

        //now we go the other way to turn round
        head -= M_PI;
        if (head < -M_PI) head += glm::twoPI;
        if (head > M_PI) head -= glm::twoPI;

        if ((minTurningRadius * 2.0) < turnOffset)
        {
            //are we right of boundary
            if (boundaryAngleOffPerpendicular > 0)
            {
                if (!isYouTurnRight) //which is actually right now
                {
                    goal.easting += (sin(bndAngle) * turnRadius);
                    goal.northing += (cos(bndAngle) * turnRadius);

                    double dis = (minTurningRadius / tan(tangencyAngle)); //long
                    goal.easting += (sin(head) * dis);
                    goal.northing += (cos(head) * dis);
                }
                else //going left
                {
                    goal.easting -= (sin(bndAngle) * turnRadius);
                    goal.northing -= (cos(bndAngle) * turnRadius);

                    double dis = (minTurningRadius * tan(tangencyAngle)); //short
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

                    double dis = (minTurningRadius * tan(tangencyAngle)); //short
                    goal.easting += (sin(head) * dis);
                    goal.northing += (cos(head) * dis);
                }
                else
                {
                    goal.easting -= (sin(bndAngle) * turnRadius);
                    goal.northing -= (cos(bndAngle) * turnRadius);

                    double dis = (minTurningRadius / tan(tangencyAngle)); //long
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
        AddSequenceLines(head, vehicle.pivotAxlePos);
    }

    int count;
    double tooClose;

    switch (youTurnPhase)
    {
        case 0: //find the crossing points
            if (findCurveTurnPoints(curve,bnd)) youTurnPhase = 1;
            ytList.clear();
            break;

        case 1:
            //now check to make sure we are not in an inner turn boundary - drive thru is ok
            count = ytList.size();
            if (count == 0) return false;

            //Are we out of bounds?
            isOutOfBounds = false;
            for (int j = 0; j < count; j += 2)
            {
                if (bnd.IsPointInsideTurnArea(ytList[j]) != 0)
                {
                    isOutOfBounds = true;
                    break;
                }
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
            if (curve.isHeadingSameWay)
            {
                crossingCurvePoint.index--;
                if (crossingCurvePoint.index < 0) crossingCurvePoint.index = 0;
            }
            else
            {
                crossingCurvePoint.index++;
                if (crossingCurvePoint.index >= curve.curList.count())
                    crossingCurvePoint.index = curve.curList.count() - 1;
            }
            crossingCurvePoint.easting = curve.curList[crossingCurvePoint.index].easting;
            crossingCurvePoint.northing = curve.curList[crossingCurvePoint.index].northing;
            crossingCurvePoint.heading = curve.curList[crossingCurvePoint.index].heading;

            tooClose = glm::Distance(ytList[0], pivotPos);
            isTurnCreationTooClose = tooClose < 3;

            //set the flag to Critical stop machine
            if (isTurnCreationTooClose) emit outOfBounds(); //mf.mc.isOutOfBounds = true;
            break;

        case 2:
            youTurnPhase = 3;
            break;
    }
    return true;
}


//needs CABCurve, CBoundary, called from CYouTurn::buildCurvePatternYouTurn
bool CYouTurn::findCurveTurnPoints(const CABCurve &curve,
                                   const CBoundary &bnd)
{
    crossingCurvePoint.easting = -20000;
    //find closet AB Curve point that will cross and go out of bounds
    int Count = curve.isHeadingSameWay ? 1 : -1;
    int turnNum = 99;
    int j;

    for (j = curve.currentLocationIndex; j > 0 && j < curve.curList.count(); j += Count)
    {
        int turnIndex = bnd.IsPointInsideTurnArea(curve.curList[j]);
        if (turnIndex != 0)
        {
            crossingCurvePoint.easting = curve.curList[j - Count].easting;
            crossingCurvePoint.northing = curve.curList[j - Count].northing;
            crossingCurvePoint.heading = curve.curList[j - Count].heading;
            crossingCurvePoint.index = j - Count;
            turnNum = turnIndex;
            break;
        }
    }

    if (turnNum < 0) //uturn will be on outer boundary turn
        turnNum = 0;
    else if (turnNum == 99)
    {
        //curve does not cross a boundary - oops
        isTurnCreationNotCrossingError = true;
        return false;
    }

    for (int i = 0; i < bnd.bndList[turnNum].turnLine.count() - 2; i++)
    {
        int res = glm::GetLineIntersection(
            bnd.bndList[turnNum].turnLine[i].easting,
            bnd.bndList[turnNum].turnLine[i].northing,
            bnd.bndList[turnNum].turnLine[i + 1].easting,
            bnd.bndList[turnNum].turnLine[i + 1].northing,

            curve.curList[j].easting,
            curve.curList[j].northing,
            curve.curList[j - Count].easting,
            curve.curList[j - Count].northing,

            iE, iN
            );

        if (res == 1)
        {
            crossingCurvePoint.easting = iE;
            crossingCurvePoint.northing = iN;

            double hed = atan2(bnd.bndList[turnNum].turnLine[i + 1].easting - bnd.bndList[turnNum].turnLine[i].easting,
                               bnd.bndList[turnNum].turnLine[i + 1].northing - bnd.bndList[turnNum].turnLine[i].northing);
            if (hed < 0) hed += glm::twoPI;
            crossingheading = hed;
            break;
        }
    }

    return crossingheading != -20000 && crossingCurvePoint.easting != -20000;
}

/*
int CYouTurn::getLineIntersection(double p0x, double p0y, double p1x, double p1y,
                                  double p2x, double p2y, double p3x, double p3y,
                                  double &iEast, double &iNorth)
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
*/

void CYouTurn::AddSequenceLines(double head, Vec3 pivot)
{
    Vec3 pt;
    for (int a = 0; a < youTurnStartOffset*2; a++)
    {
        pt.easting = ytList[0].easting + (sin(head) * 0.5);
        pt.northing = ytList[0].northing + (cos(head) * 0.5);
        pt.heading = ytList[0].heading;
        ytList.insert(0, pt);
    }

    int count = ytList.size();

    for (int i = 1; i <= youTurnStartOffset*2; i++)
    {
        pt.easting = ytList[count - 1].easting + (sin(head) * i * 0.5);
        pt.northing = ytList[count - 1].northing + (cos(head) * i * 0.5);
        pt.heading = head;
        ytList.append(pt);
    }

    double distancePivotToTurnLine;
    count = ytList.size();
    for (int i = 0; i < count; i += 2)
    {
        distancePivotToTurnLine = glm::Distance(ytList[i], pivot);
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

bool CYouTurn::BuildABLineDubinsYouTurn(CVehicle &vehicle,
                                        CBoundary &bnd,
                                        const CABLine &ABLine,
                                        bool isTurnRight)
{
    double minTurningRadius = property_setVehicle_minTurningRadius;
    double tool_toolWidth = property_setVehicle_toolWidth;
    double tool_toolOverlap = property_setVehicle_toolOverlap;
    double tool_toolOffset = property_setVehicle_toolOffset;

    double headAB = ABLine.abHeading;
    if (!ABLine.isHeadingSameWay) headAB += M_PI;

    if (uTurnStyle == 0)
    {
        if (youTurnPhase == 0)
        {
            //grab the pure pursuit point right on ABLine
            Vec3 onPurePoint(ABLine.rEastAB, ABLine.rNorthAB, 0);

            //how far are we from any turn boundary
            bnd.FindClosestTurnPoint(ABLine, onPurePoint);

            //or did we lose the turnLine - we are on the highway cuz we left the outer/inner turn boundary
            if ((int)bnd.closestTurnPt.easting != -20000)
            {
                vehicle.distancePivotToTurnLine = glm::Distance(vehicle.pivotAxlePos, bnd.closestTurnPt);
            }
            else
            {
                //Full emergency stop code goes here, it thinks its auto turn, but its not!
                vehicle.distancePivotToTurnLine = -3333;
            }

            //delta between AB heading and boundary closest point heading
            boundaryAngleOffPerpendicular = M_PI - fabs(fabs(bnd.closestTurnPt.heading - headAB) - M_PI);
            boundaryAngleOffPerpendicular -= glm::PIBy2;
            boundaryAngleOffPerpendicular *= -1;
            if (boundaryAngleOffPerpendicular > 1.25) boundaryAngleOffPerpendicular = 1.25;
            if (boundaryAngleOffPerpendicular < -1.25) boundaryAngleOffPerpendicular = -1.25;

            //for calculating innner circles of turn
            double tangencyAngle = (glm::PIBy2 - fabs(boundaryAngleOffPerpendicular)) * 0.5;

            //baseline away from boundary to start calculations
            double toolTurnWidth = tool_toolWidth * rowSkipsWidth;

            //distance from TurnLine for trigger added in youturn form, include the 3 m bump forward
            double distanceTurnBeforeLine = 0;

            if (youTurnRadius * 2 < toolTurnWidth)
            {
                if (boundaryAngleOffPerpendicular < 0)
                {
                    //which is actually left
                    if (isYouTurnRight)
                        distanceTurnBeforeLine += (minTurningRadius * tan(tangencyAngle));//short
                    else
                        distanceTurnBeforeLine += (minTurningRadius / tan(tangencyAngle)); //long
                }
                else
                {
                    //which is actually left
                    if (isYouTurnRight)
                        distanceTurnBeforeLine += (minTurningRadius / tan(tangencyAngle)); //long
                    else
                        distanceTurnBeforeLine += (minTurningRadius * tan(tangencyAngle)); //short
                }
            }
            else //turn Radius is wider then equipment width so ohmega turn
            {
                distanceTurnBeforeLine += (2 * minTurningRadius);
            }

            //used for distance calc for other part of turn

            CDubins dubYouTurnPath;
            CDubinsTurningRadius = youTurnRadius;

            //point on AB line closest to pivot axle point from ABLine PurePursuit
            rEastYT = ABLine.rEastAB;
            rNorthYT = ABLine.rNorthAB;
            isHeadingSameWay = ABLine.isHeadingSameWay;
            double head = ABLine.abHeading;

            //grab the vehicle widths and offsets
            double turnOffset = (tool_toolWidth - tool_toolOverlap) * rowSkipsWidth + (isYouTurnRight ? -tool_toolOffset * 2.0 : tool_toolOffset * 2.0);
            double turnRadius = turnOffset / cos(boundaryAngleOffPerpendicular);
            if (!isHeadingSameWay) head += M_PI;

            double turnDiagDistance = vehicle.distancePivotToTurnLine;

            //move the start forward 2 meters
            rEastYT += (sin(head) * turnDiagDistance);
            rNorthYT += (cos(head) * turnDiagDistance);

            Vec3 start(rEastYT, rNorthYT, head);
            Vec3 goal;

            //move the cross line calc to not include first turn
            goal.easting = rEastYT + (sin(head) * distanceTurnBeforeLine);
            goal.northing = rNorthYT + (cos(head) * distanceTurnBeforeLine);

            //headland angle relative to vehicle heading to head along the boundary left or right
            double bndAngle = head - boundaryAngleOffPerpendicular + glm::PIBy2;

            //now we go the other way to turn round
            head -= M_PI;
            if (head < -M_PI) head += glm::twoPI;
            if (head > M_PI) head -= glm::twoPI;

            if ((youTurnRadius * 2.0) < turnOffset)
            {
                //are we right of boundary
                if (boundaryAngleOffPerpendicular > 0)
                {
                    if (!isYouTurnRight) //which is actually right now
                    {
                        goal.easting += (sin(bndAngle) * turnRadius);
                        goal.northing += (cos(bndAngle) * turnRadius);

                        double dis = (minTurningRadius / tan(tangencyAngle)); //long
                        goal.easting += (sin(head) * dis);
                        goal.northing += (cos(head) * dis);
                    }
                    else //going left
                    {
                        goal.easting -= (sin(bndAngle) * turnRadius);
                        goal.northing -= (cos(bndAngle) * turnRadius);

                        double dis = (minTurningRadius * tan(tangencyAngle)); //short
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

                        double dis = (minTurningRadius * tan(tangencyAngle)); //short
                        goal.easting += (sin(head) * dis);
                        goal.northing += (cos(head) * dis);
                    }
                    else
                    {
                        goal.easting -= (sin(bndAngle) * turnRadius);
                        goal.northing -= (cos(bndAngle) * turnRadius);

                        double dis = (minTurningRadius / tan(tangencyAngle)); //long
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
                goal.easting += (sin(head) * 1);
                goal.northing += (cos(head) * 1);
                goal.heading = head;
            }

            goal.heading = head;

            //generate the turn points
            ytList = dubYouTurnPath.GenerateDubins(start, goal);
            AddSequenceLines(head,vehicle.pivotAxlePos);
            if (ytList.count() == 0) return false;
            else youTurnPhase = 1;
        }

        if (youTurnPhase == 3) return true;

        // Phase 0 - back up the turn till it is out of bounds.
        // Phase 1 - move it forward till out of bounds.
        // Phase 2 - move forward couple meters away from turn line.
        // Phase 3 - ytList is made, waiting to get close enough to it

        double cosHead;
        double sinHead;
        int cnt;
        QVector<Vec3> arr2;

        isOutOfBounds = false;
        switch (youTurnPhase)
        {
            case 1:
                //the temp array
                vehicle.distancePivotToTurnLine = glm::Distance(ytList[0], vehicle.pivotAxlePos);
                cosHead = cos(headAB);
                sinHead = sin(headAB);

                cnt = ytList.size();
                arr2 = ytList;
                ytList.clear();

                for (int i = 0; i < cnt; i++)
                {
                    arr2[i].easting -= (sinHead);
                    arr2[i].northing -= (cosHead);
                    ytList.append(arr2[i]);
                }

                for (int j = 0; j < cnt; j += 2)
                {
                    if (bnd.IsPointInsideTurnArea(ytList[j]) != 0)
                    {
                        isOutOfBounds = true;
                        break;
                    }
                }

                if (!isOutOfBounds)
                {
                    youTurnPhase = 2;
                }
                else
                {
                    //turn keeps approaching vehicle and running out of space - end of field?
                    if (isOutOfBounds && vehicle.distancePivotToTurnLine > 3)
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
            //move again out of bounds
            case 2:
                //the temp array
                vehicle.distancePivotToTurnLine = glm::Distance(ytList[0], vehicle.pivotAxlePos);
                cosHead = cos(headAB);
                sinHead = sin(headAB);

                cnt = ytList.count();
                QVector<Vec3> arr21 = ytList;
                ytList.clear();

                for (int i = 0; i < cnt; i++)
                {
                    arr21[i].easting += (sinHead * 0.05);
                    arr21[i].northing += (cosHead * 0.05);
                    ytList.append(arr21[i]);
                }

                for (int j = 0; j < cnt; j += 2)
                {
                    if (bnd.IsPointInsideTurnArea(ytList[j]) != 0)
                    {
                        isOutOfBounds = true;
                        break;
                    }
                }

                if (isOutOfBounds)
                {
                    isOutOfBounds = false;
                    youTurnPhase = 3;
                }
                else
                {
                    //turn keeps approaching vehicle and running out of space - end of field?
                    if (!isOutOfBounds && vehicle.distancePivotToTurnLine > 3)
                    {
                        isTurnCreationTooClose = false;
                    }
                    else
                    {
                        isTurnCreationTooClose = true;

                        //set the flag to Critical stop machine
                        if (isTurnCreationTooClose)
                            emit outOfBounds();
                    }
                }
                break;
        }
        return true;
    } else if (uTurnStyle == 1) {
        //grab the pure pursuit point right on ABLine
        Vec3 onPurePoint = Vec3(ABLine.rEastAB, ABLine.rNorthAB, 0);

        //how far are we from any turn boundary
        bnd.FindClosestTurnPoint(ABLine,onPurePoint);

        //or did we lose the turnLine - we are on the highway cuz we left the outer/inner turn boundary
        if ((int)bnd.closestTurnPt.easting != -20000)
        {
                vehicle.distancePivotToTurnLine = glm::Distance(vehicle.pivotAxlePos, bnd.closestTurnPt);
        }
        else
        {
                //Full emergency stop code goes here, it thinksurn its auto turn, but its not!
                vehicle.distancePivotToTurnLine = -3333;
        }

        //delta between AB heading and boundary closest point heading
        boundaryAngleOffPerpendicular = M_PI - fabs(fabs(bnd.closestTurnPt.heading - headAB) - M_PI);
        boundaryAngleOffPerpendicular -= glm::PIBy2;
        boundaryAngleOffPerpendicular *= -1;
        if (boundaryAngleOffPerpendicular > 1.25) boundaryAngleOffPerpendicular = 1.25;
        if (boundaryAngleOffPerpendicular < -1.25) boundaryAngleOffPerpendicular = -1.25;

        //for calculating innner circles of turn
        double tangencyAngle = (glm::PIBy2 - fabs(boundaryAngleOffPerpendicular)) * 0.5;
        double tangencyFactor = tan(tangencyAngle);

        //grab the vehicle widths and offsets
        //double turnOffset = (tool_toolWidth - tool_toolOverlap) * rowSkipsWidth + (isYouTurnRight ? -tool_toolOffset * 2.0 : tool_toolOffset * 2.0);

        //double turnRadius = turnOffset / Math.Cos(boundaryAngleOffPerpendicular);
        double turnRadius = youTurnRadius;

        isHeadingSameWay = ABLine.isHeadingSameWay;
        double head = ABLine.abHeading;
        if (!isHeadingSameWay) head += M_PI;

        double abLineHeading = head;

        //intersection of ABline and boundary
        rEastYT = bnd.closestTurnPt.easting;
        rNorthYT = bnd.closestTurnPt.northing;

        Vec3 oneStart(rEastYT, rNorthYT, 0);

        Vec3 oneEnd(rEastYT, rNorthYT, 0);

        Vec3 twoStart(rEastYT, rNorthYT, 0);

        Vec3 twoEnd(rEastYT, rNorthYT, 0);

        if (boundaryAngleOffPerpendicular < 0)
        {
                //which is actually left
                if (isYouTurnRight)
                {
                    turnRadius = (youTurnRadius * tangencyFactor);//short
                }
                else
                {
                    turnRadius = (youTurnRadius / tangencyFactor); //long
                    tangencyFactor = 1 / tangencyFactor;
                }
        }
        else
        {
                //which is actually left
                if (isYouTurnRight)
                {
                    turnRadius = (youTurnRadius / tangencyFactor); //long
                    tangencyFactor = 1 / tangencyFactor;
                }
                else
                {
                    turnRadius = (youTurnRadius * tangencyFactor); //short
                }
        }

        //move the start back away from turn line youTurnRadius distance based on tangency
        oneStart.easting -= (sin(head) * turnRadius);
        oneStart.northing -= (cos(head) * turnRadius);
        oneStart.heading = head;

        double arcAngle = glm::PIBy2;
        // move the goal left or right at 90 degrees
        if (!isTurnRight) //means going right
        {
                head += glm::PIBy2;
                arcAngle -= boundaryAngleOffPerpendicular;
                //head -= angle;
        }
        else
        {
                head -= glm::PIBy2;
                arcAngle += boundaryAngleOffPerpendicular;
                //head += angle;
        }

        if (head < -M_PI) head += glm::twoPI;
        if (head > M_PI) head -= glm::twoPI;

        //point to set next AB Line via lateral
        pt3TurnNewAB.easting = oneEnd.easting + (sin(head) * tool_toolWidth);
        pt3TurnNewAB.northing = oneEnd.northing + (cos(head) * tool_toolWidth);

        oneEnd.easting = oneStart.easting + sin(head) * youTurnRadius;
        oneEnd.northing = oneStart.northing + cos(head) * youTurnRadius;

        twoEnd.heading = 0; // - angle;
        oneEnd.heading = 0;

        //two.easting = pt3TurnNewAB.easting - (Math.Sin(head) * mf.vehicle.minTurningRadius);
        //two.northing = pt3TurnNewAB.northing - (Math.Cos(head) * mf.vehicle.minTurningRadius);

        double r = youTurnRadius;
        int numSegments = (int)(arcAngle * 16);

        double theta = arcAngle / (double)(numSegments - 1);

        double tanFactor = tan(theta);
        double radialFactor = cos(theta);

        double startAngle = abLineHeading;
        if (!isYouTurnRight)
        {
                startAngle -= 1.57;
        }
        else
        {
                startAngle += 1.57;
        }

        if (startAngle < -M_PI) startAngle += glm::twoPI;
        if (startAngle > M_PI) startAngle -= glm::twoPI;

        double x = r * sin(startAngle);
        double y = r * cos(startAngle);

        Vec3 pt;
        for (int ii = 0; ii < numSegments; ii++)
        {
                //glVertex2f(x + cx, y + cy);
                pt.easting = x + oneEnd.easting;
                pt.northing = y + oneEnd.northing;
                pt.heading = 0;

                ytList.append(pt);

                double tx;
                double ty;
                if (!isYouTurnRight)
                {
                    tx = y;
                    ty = -x;
                }
                else
                {
                    tx = -y;
                    ty = x;
                }

                x += tx * tanFactor;
                y += ty * tanFactor;

                x *= radialFactor;
                y *= radialFactor;
        }

        for (int a = 0; a < 8; a++)
        {
                pt.easting = ytList[0].easting - (sin(abLineHeading) * 0.5);
                pt.northing = ytList[0].northing - (cos(abLineHeading) * 0.5);
                pt.heading = ytList[0].heading;
                ytList.insert(0, pt);
        }

        //we are following the turn line now.
        head -= boundaryAngleOffPerpendicular;

        //from end of turn to over new AB a bit
        //double twoEndExtension = mf.tool.width + mf.vehicle.wheelbase - youTurnRadius;
        //if (mf.tool.width < turnRadius) twoEndExtension = mf.vehicle.wheelbase;
        int twoEndExtension = (int)(tool_toolWidth * 5);

        //add the tail to first turn
        int count = ytList.count();
        for (int i = 1; i <= twoEndExtension; i++)
        {
                pt.easting = ytList[count - 1].easting + (sin(head) * i * 0.5);
                pt.northing = ytList[count - 1].northing + (cos(head) * i * 0.5);
                pt.heading = 0;
                ytList.append(pt);
        }

        //calculate line headings
        QVector<Vec3> arr = ytList;
        ytList.clear();

        //headings of line one
        for (int i = 0; i < arr.count() - 1; i++)
        {
                arr[i].heading = atan2(arr[i + 1].easting - arr[i].easting, arr[i + 1].northing - arr[i].northing);
                if (arr[i].heading < 0) arr[i].heading += glm::twoPI;
                ytList.append(arr[i]);
        }

        //LINE TWO - use end of line one for end of line two, both same direction bit longer
        twoEnd.easting = ytList[ytList.count() - 1].easting;
        twoEnd.northing = ytList[ytList.count() - 1].northing;
        twoEnd.heading = ytList[ytList.count() - 1].heading;

        if (twoEnd.heading < -M_PI) twoEnd.heading += glm::twoPI;
        if (twoEnd.heading > M_PI) twoEnd.heading -= glm::twoPI;

        //straight line
        twoStart = twoEnd;

        ////backing up to this point
        twoStart.easting -= (sin(head) * 40);
        twoStart.northing -= (cos(head) * 40);

        pt3ListSecondLine.clear();
        pt = twoStart;
        pt3ListSecondLine.append(pt);

        //from start to end
        count = pt3ListSecondLine.count();
        for (int i = 1; i <= 80; i++)
        {
                pt.easting = pt3ListSecondLine[count - 1].easting + (sin(head) * i * 0.5);
                pt.northing = pt3ListSecondLine[count - 1].northing + (cos(head) * i * 0.5);
                pt.heading = 0;
                pt3ListSecondLine.append(pt);
        }

        if (pt3ListSecondLine.count() != 0)
        {
                youTurnPhase = 3;
                return true;
        }
        else
        {
                return false;
        }
    }
    else //uturn style == 2
    {
        return false;
    }
}

void CYouTurn::SmoothYouTurn(int smPts)
{
    //count the reference list of original curve
    int cnt = ytList.count();

    //the temp array
    QVector<Vec3> arr;
    arr.fill(Vec3(0,0,0),cnt); //not quite how AOG does it but should work

    //read the points before and after the setpoint
    for (int s = 0; s < smPts / 2; s++)
    {
            arr[s].easting = ytList[s].easting;
            arr[s].northing = ytList[s].northing;
            arr[s].heading = ytList[s].heading;
    }

    for (int s = cnt - (smPts / 2); s < cnt; s++)
    {
            arr[s].easting = ytList[s].easting;
            arr[s].northing = ytList[s].northing;
            arr[s].heading = ytList[s].heading;
    }

    //average them - center weighted average
    for (int i = smPts / 2; i < cnt - (smPts / 2); i++)
    {
            for (int j = -smPts / 2; j < smPts / 2; j++)
            {
                arr[i].easting += ytList[j + i].easting;
                arr[i].northing += ytList[j + i].northing;
            }
            arr[i].easting /= smPts;
            arr[i].northing /= smPts;
            arr[i].heading = ytList[i].heading;
    }

    ytList.clear();

    //calculate new headings on smoothed line
    for (int i = 1; i < cnt - 1; i++)
    {
            arr[i].heading = atan2(arr[i + 1].easting - arr[i].easting,
                                   arr[i + 1].northing - arr[i].northing);
            if (arr[i].heading < 0) arr[i].heading += glm::twoPI;
            ytList.append(arr[i]);
    }
}

//called to initiate turn
void CYouTurn::YouTurnTrigger(CVehicle &vehicle, CABLine &ABLine, CABCurve &curve)
{
    //trigger pulled
    isYouTurnTriggered = true;

    if (alternateSkips && rowSkipsWidth2 > 1)
    {
            if (--turnSkips == 0)
            {
                isYouTurnRight = !isYouTurnRight;
                turnSkips = rowSkipsWidth2 * 2 - 1;
            }
            else if ((previousBigSkip = !previousBigSkip))
                rowSkipsWidth = rowSkipsWidth2 - 1;
            else
                rowSkipsWidth = rowSkipsWidth2;
    }
    else isYouTurnRight = !isYouTurnRight;

    if (uTurnStyle == 0)
    {
            vehicle.guidanceLookPos.easting = ytList[ytList.count() - 1].easting;
            vehicle.guidanceLookPos.northing = ytList[ytList.count() - 1].northing;
    }
    else if (uTurnStyle == 1)
    {
            vehicle.guidanceLookPos.easting = pt3TurnNewAB.easting;
            vehicle.guidanceLookPos.northing = pt3TurnNewAB.northing;

            pt3Phase = 0;
    }

    if (ABLine.isABLineSet)
    {
            ABLine.isLateralTriggered = true;
            ABLine.isABValid = false;
    }
    else
    {
            curve.isLateralTriggered = true;
            curve.isCurveValid = false;
    }
}

//Normal copmpletion of youturn
void CYouTurn::CompleteYouTurn()
{
    isYouTurnTriggered = false;
    ResetCreatedYouTurn();
    //emit resetSequenceEventTriggers();
    //mf.seq.isSequenceTriggered = false;
    //emit setTriggerSequence(false);
    //mf.isBoundAlarming = false;
    emit turnOffBoundAlarm();
}

void CYouTurn::Set_Alternate_skips()
{
    rowSkipsWidth2 = rowSkipsWidth;
    turnSkips = rowSkipsWidth2 * 2 - 1;
    previousBigSkip = false;
}


//something went seriously wrong so reset everything
void CYouTurn::ResetYouTurn()
{
    //fix you turn
    isYouTurnTriggered = false;
    ytList.clear();
    ResetCreatedYouTurn();
    //mf.isBoundAlarming = false;
    emit turnOffBoundAlarm();

    isTurnCreationTooClose = false;
    isTurnCreationNotCrossingError = false;
}

void CYouTurn::ResetCreatedYouTurn()
{
    youTurnPhase = 0;
    ytList.clear();
    pt3ListSecondLine.clear();
    pt3Phase = 0;
}

//build the points and path of youturn to be scaled and transformed
void CYouTurn::BuildManualYouLateral(CVehicle &vehicle, CABLine &ABLine, CABCurve &curve,
                                  bool isTurnRight)
{
    double tool_toolWidth = property_setVehicle_toolWidth;
    double tool_toolOverlap = property_setVehicle_toolOverlap;

    double head;
    //point on AB line closest to pivot axle point from ABLine PurePursuit
    if (ABLine.isABLineSet)
    {
        rEastYT = ABLine.rEastAB;
        rNorthYT = ABLine.rNorthAB;
        isHeadingSameWay = ABLine.isHeadingSameWay;
        head = ABLine.abHeading;
        ABLine.isLateralTriggered = true;
    }
    else if(curve.isCurveSet)
    {
        rEastYT = curve.rEastCu;
        rNorthYT = curve.rNorthCu;
        isHeadingSameWay = curve.isHeadingSameWay;
        head = curve.manualUturnHeading;
        curve.isLateralTriggered = true;
    }
    else return;

    //grab the vehicle widths and offsets
    double turnOffset = (tool_toolWidth - tool_toolOverlap); //remove rowSkips
    //if its straight across it makes 2 loops instead so goal is a little lower then start
    if (!isHeadingSameWay) head += M_PI;

    //move the start forward 2 meters, this point is critical to formation of uturn
    rEastYT += (sin(head) * 2);
    rNorthYT += (cos(head) * 2);

    if (isTurnRight)
    {
        vehicle.guidanceLookPos.easting = rEastYT + (cos(-head) * turnOffset);
        vehicle.guidanceLookPos.northing = rNorthYT + (sin(-head) * turnOffset);
    }
    else
    {
        vehicle.guidanceLookPos.easting = rEastYT - (cos(-head) * turnOffset);
        vehicle.guidanceLookPos.northing = rNorthYT - (sin(-head) * turnOffset);
    }

    ABLine.isABValid = false;
    curve.isCurveValid = false;
}

//build the points and path of youturn to be scaled and transformed
void CYouTurn::BuildManualYouTurn(CVehicle &vehicle, CABLine &ABLine, CABCurve &curve,
                                  bool isTurnRight, bool isTurnButtonTriggered)
{
    double minTurningRadius = property_setVehicle_minTurningRadius;
    double tool_toolWidth = property_setVehicle_toolWidth;
    double tool_toolOverlap = property_setVehicle_toolOverlap;
    double tool_toolOffset = property_setVehicle_toolOffset;

    isYouTurnTriggered = true;

    double head;
    //point on AB line closest to pivot axle point from ABLine PurePursuit
    if (ABLine.isABLineSet)
    {
        rEastYT = ABLine.rEastAB;
        rNorthYT = ABLine.rNorthAB;
        isHeadingSameWay = ABLine.isHeadingSameWay;
        head = ABLine.abHeading;
        ABLine.isLateralTriggered = true;
    }
    else if (curve.isCurveSet)
    {
        rEastYT = curve.rEastCu;
        rNorthYT = curve.rNorthCu;
        isHeadingSameWay = curve.isHeadingSameWay;
        head = curve.manualUturnHeading;
        curve.isLateralTriggered = true;
    }
    else return;

    //grab the vehicle widths and offsets
    double turnOffset = (tool_toolWidth - tool_toolOverlap) * rowSkipsWidth + (isTurnRight ? tool_toolOffset * 2.0 : -tool_toolOffset * 2.0);

    CDubins dubYouTurnPath;
    CDubinsTurningRadius = minTurningRadius;

    //if its straight across it makes 2 loops instead so goal is a little lower then start
    if (!isHeadingSameWay) head += 3.14;
    else head -= 0.01;

    //move the start forward 2 meters, this point is critical to formation of uturn
    rEastYT += (sin(head) * 4);
    rNorthYT += (cos(head) * 4);

    //now we have our start point
    Vec3 start(rEastYT, rNorthYT, head);
    Vec3 goal;

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

    vehicle.guidanceLookPos.easting = ytList[ytList.count() - 1].easting;
    vehicle.guidanceLookPos.northing = ytList[ytList.count() - 1].northing;

    ABLine.isABValid = false;
    curve.isCurveValid = false;
}

//determine distance from youTurn guidance line
bool CYouTurn::DistanceFromYouTurnLine(CVehicle &vehicle, CNMEA &pn)
{
    double maxSteerAngle = property_setVehicle_maxSteerAngle;
    double wheelbase = property_setVehicle_wheelbase;

    //grab a copy from main - the steer position
    double minDistA = 1000000, minDistB = 1000000;
    int ptCount = ytList.size();

    if (ptCount > 0)
    {
        if (property_setVehicle_isStanleyUsed)
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

            if (minDistA > 16)
            {
                CompleteYouTurn();
                return false;
            }

            //just need to make sure the points continue ascending or heading switches all over the place
            if (A > B)
            {
                C = A; A = B; B = C;
            }

            //feed backward to turn slower to keep pivot on
            A -= 7;
            if (A < 0)
            {
                A = 0;
            }
            B = A + 1;

            //return and reset if too far away or end of the line
            if (B >= ptCount - 8)
            {
                CompleteYouTurn();
                return false;
            }

            //get the distance from currently active AB line, precalc the norm of line
            double dx = ytList[B].easting - ytList[A].easting;
            double dz = ytList[B].northing - ytList[A].northing;
            if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dz) < glm::DOUBLE_EPSILON) return false;

            double abHeading = ytList[A].heading;

            //how far from current AB Line is steer point 90 degrees from steer position
            distanceFromCurrentLine = ((dz * pivot.easting) - (dx * pivot.northing) + (ytList[B].easting
                        * ytList[A].northing) - (ytList[B].northing * ytList[A].easting))
                            / sqrt((dz * dz) + (dx * dx));

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

            if(vehicle.isReverse) abFixHeadingDelta *= -1;
            //normally set to 1, less then unity gives less heading error.
            abFixHeadingDelta *= (double)(property_stanleyDistanceErrorGain);
            if (abFixHeadingDelta > 0.74) abFixHeadingDelta = 0.74;
            if (abFixHeadingDelta < -0.74) abFixHeadingDelta = -0.74;

            //the non linear distance error part of stanley
            steerAngleYT = atan((distanceFromCurrentLine * (double)property_stanleyDistanceErrorGain) / ((pn.speed * 0.277777) + 1));

            //clamp it to max 42 degrees
            if (steerAngleYT > 0.74) steerAngleYT = 0.74;
            if (steerAngleYT < -0.74) steerAngleYT = -0.74;

            //add them up and clamp to max in vehicle settings
            steerAngleYT = glm::toDegrees((steerAngleYT + abFixHeadingDelta) * -1.0);
            if (steerAngleYT < -maxSteerAngle) steerAngleYT = -maxSteerAngle;
            if (steerAngleYT > maxSteerAngle) steerAngleYT = maxSteerAngle;
        }
        else
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

            onA = A;
            double distancePiv = glm::Distance(ytList[A], vehicle.pivotAxlePos);

            if (distancePiv > 1 || (B >= ptCount - 1))
            {
                {
                    CompleteYouTurn();
                    return false;
                }
            }

            //get the distance from currently active AB line
            double dx = ytList[B].easting - ytList[A].easting;
            double dz = ytList[B].northing - ytList[A].northing;
            if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dz) < glm::DOUBLE_EPSILON) return false;

            //how far from current AB Line is fix
            distanceFromCurrentLine = ((dz * pivot.easting) - (dx * pivot.northing) + (ytList[B].easting
                        * ytList[A].northing) - (ytList[B].northing * ytList[A].easting))
                            / sqrt((dz * dz) + (dx * dx));

            // ** Pure pursuit ** - calc point on ABLine closest to current position
            double U = (((pivot.easting - ytList[A].easting) * dx)
                        + ((pivot.northing - ytList[A].northing) * dz))
                        / ((dx * dx) + (dz * dz));

            rEastYT = ytList[A].easting + (U * dx);
            rNorthYT = ytList[A].northing + (U * dz);

            //sharp turns on you turn.
            //update base on autosteer settings and distance from line
            double goalPointDistance = 0.8 * vehicle.UpdateGoalPointDistance();

            isHeadingSameWay = true;
            bool reverseHeading = !vehicle.isReverse;

            int count = reverseHeading ? 1 : -1;
            Vec3 start(rEastYT, rNorthYT, 0);
            double distSoFar = 0;

            for (int i = reverseHeading ? B : A; i < ptCount && i >= 0; i += count)
            {
                // used for calculating the length squared of next segment.
                double tempDist = glm::Distance(start, ytList[i]);

                //will we go too far?
                if ((tempDist + distSoFar) > goalPointDistance)
                {
                    double j = (goalPointDistance - distSoFar) / tempDist; // the remainder to yet travel

                    goalPointYT.easting = (((1 - j) * start.easting) + (j * ytList[i].easting));
                    goalPointYT.northing = (((1 - j) * start.northing) + (j * ytList[i].northing));
                    break;
                }
                else distSoFar += tempDist;
                start = ytList[i];
                if (i == ptCount - 1)//goalPointDistance is longer than remaining u-turn
                {
                    CompleteYouTurn();
                    return false;
                }

                if (pt3Phase == 1 && i < 2)
                {
                    CompleteYouTurn();
                    return false;
                }

                if (uTurnStyle == 1 && pt3Phase == 0 && isLastFrameForward && vehicle.isReverse)
                {
                    ytList.clear();
                    pt3Phase++;
                    return true;
                }

                if (uTurnStyle == 1 && pt3Phase == 1 && !isLastFrameForward && !vehicle.isReverse)
                {
                    CompleteYouTurn();
                    return false;
                }

                isLastFrameForward = vehicle.isReverse;
            }

            //calc "D" the distance from pivot axle to lookahead point
            double goalPointDistanceSquared = glm::DistanceSquared(goalPointYT.northing, goalPointYT.easting, pivot.northing, pivot.easting);

            //calculate the the delta x in local coordinates and steering angle degrees based on wheelbase
            double localHeading = glm::twoPI - vehicle.fixHeading;
            ppRadiusYT = goalPointDistanceSquared / (2 * (((goalPointYT.easting - pivot.easting) * cos(localHeading)) + ((goalPointYT.northing - pivot.northing) * sin(localHeading))));

            steerAngleYT = glm::toDegrees(atan(2 * (((goalPointYT.easting - pivot.easting) * cos(localHeading))
                                                        + ((goalPointYT.northing - pivot.northing) * sin(localHeading))) * wheelbase / goalPointDistanceSquared));

            if (steerAngleYT < -maxSteerAngle) steerAngleYT = -maxSteerAngle;
            if (steerAngleYT > maxSteerAngle) steerAngleYT = maxSteerAngle;

            if (ppRadiusYT < -500) ppRadiusYT = -500;
            if (ppRadiusYT > 500) ppRadiusYT = 500;

            radiusPointYT.easting = pivot.easting + (ppRadiusYT * cos(localHeading));
            radiusPointYT.northing = pivot.northing + (ppRadiusYT * sin(localHeading));

            //distance is negative if on left, positive if on right
            if (!isHeadingSameWay)
                distanceFromCurrentLine *= -1.0;
        }

        //used for smooth mode
        vehicle.modeActualXTE = (distanceFromCurrentLine);

        //Convert to centimeters
        vehicle.guidanceLineDistanceOff = (short)glm::roundMidAwayFromZero(distanceFromCurrentLine * 1000.0);
        vehicle.guidanceLineSteerAngle = (short)(steerAngleYT * 100);
        return true;
    }
    else
    {
        CompleteYouTurn();
        return false;
    }
}

void CYouTurn::Check3PtSequence(void)
{
    if (pt3Phase == 0)
    {
        ytList.clear();
        //ytList.AddRange(pt3ListSecondLine);
        pt3Phase++;
        //mf.sim.stepDistance = 0;
        //mf.sim.isAccelBack = true;
    }
    else
    {
        CompleteYouTurn();
        //mf.sim.stepDistance = 0;
        //mf.sim.isAccelForward = true;
    }
}

//Duh.... What does this do....
void CYouTurn::DrawYouTurn(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    GLHelperOneColor gldraw;
    QColor color;

    int ptCount = ytList.size();
    if (ptCount < 3) return;

    if (isYouTurnTriggered)
        color = QColor::fromRgbF(0.95f, 0.5f, 0.95f);
    else if (isOutOfBounds)
        color = QColor::fromRgbF(0.9495f, 0.395f, 0.325f);
    else
        color = QColor::fromRgbF(0.395f, 0.925f, 0.30f);

    for (int i = 0; i < ptCount; i++)
    {
        gldraw.append(QVector3D(ytList[i].easting, ytList[i].northing, 0));
    }

    gldraw.draw(gl, mvp, color,
                GL_POINTS, property_setDisplay_lineWidth);
}
