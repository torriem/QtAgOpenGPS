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

    youTurnRadius = property_set_youTurnRadius;

    uTurnStyle = property_set_uTurnStyle;

    uTurnSmoothing = property_setAS_uTurnSmoothing;

}

//Finds the point where an AB Curve crosses the turn line
bool CYouTurn::BuildCurveDubinsYouTurn(bool isTurnLeft, Vec3 pivotPos,
                                       CVehicle &vehicle,
                                       const CBoundary &bnd,
                                       CABCurve &curve,
                                       const CTrack &trk,
                                       int &makeUTurnCounter,
                                       int secondsSinceStart
                                       )
{
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
            return CreateCurveWideTurn(isTurnLeft, pivotPos, makeUTurnCounter, vehicle, bnd, curve, trk, secondsSinceStart);
        }

        //Ohmega turn
        else
        {
            return CreateCurveOmegaTurn(isTurnLeft, pivotPos, makeUTurnCounter, vehicle, bnd, curve, trk, secondsSinceStart);
        }
    }
    else if (uTurnStyle == 1)
    {
        return (KStyleTurnCurve(isTurnLeft, makeUTurnCounter,vehicle,curve,bnd));
    }

    //prgramming error if you got here
    return false;
}

bool CYouTurn::BuildABLineDubinsYouTurn(bool isTurnLeft,
                                        CVehicle &vehicle,
                                        const CBoundary &bnd,
                                        CABLine &ABLine,
                                        CTrack &trk,
                                        int &makeUTurnCounter,
                                        int secondsSinceStart)
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
            return (CreateABWideTurn(isTurnLeft, makeUTurnCounter, vehicle, bnd, ABLine, trk, secondsSinceStart));
        }
        //Small turn
        else
        {
            return (CreateABOmegaTurn(isTurnLeft, makeUTurnCounter, vehicle, bnd, ABLine));
        }
    }
    else if (uTurnStyle == 1)
    {
        return (KStyleTurnAB(isTurnLeft, makeUTurnCounter,vehicle,ABLine,bnd));
    }

    //prgramming error if you got here
    return false;
}

bool CYouTurn::CreateCurveOmegaTurn(bool isTurnLeft, Vec3 pivotPos,
                                    int makeUTurnCounter,
                                    CVehicle &vehicle,
                                    const CBoundary &bnd,
                                    const CABCurve &curve,
                                    const CTrack &trk,
                                    int secondsSinceStart
                                    )
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
    {
        if (!FindCurveTurnPoint(curve,bnd))
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
            CDubinsTurningRadius = youTurnRadius;

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
    }
        break;

    case 1:
    {
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
        //nextCurve = QSharedPointer<ABCurve>(new CABCurve());
        CABCurve nextCurve; // = QSharedPointer<CABCurve>(new CABCurve());
        nextCurve.BuildCurveCurrentList(vehicle.pivotAxlePos,secondsSinceStart,vehicle,trk,bnd,*this);
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

        if (!AddCurveSequenceLines(curve, nextCurve)) return false;

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
    }
    return true;
}

bool CYouTurn::CreateCurveWideTurn(bool isTurnLeft, Vec3 pivotPos,
                                   int makeUTurnCounter,
                                   CVehicle &vehicle,
                                   const CBoundary &bnd,
                                   CABCurve &curve,
                                   const CTrack &trk,
                                   int secondsSinceStart
                                   )
{
    double tool_toolWidth = property_setVehicle_toolWidth;
    double tool_toolOverlap = property_setVehicle_toolOverlap;
    double tool_toolOffset = property_setVehicle_toolOffset;

    CABCurve nextCurve;

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
    {
        //Create first semicircle
        if (!FindCurveTurnPoint(curve,bnd))
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
    }
    case 1:
    {
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
        //nextCurve = QSharedPointer<ABCurve>(new CABCurve());
        nextCurve.BuildCurveCurrentList(vehicle.pivotAxlePos,secondsSinceStart,vehicle,trk,bnd,*this);
        vehicle.guidanceLookPos = tempguidanceLookPos;


        //going with or against boundary?
        bool isTurnLineSameWay = true;
        double headingDifference = fabs(inClosestTurnPt.turnLineHeading - ytList[ytList.count() - 1].heading);
        if (headingDifference > glm::PIBy2 && headingDifference < 3 * glm::PIBy2) isTurnLineSameWay = false;

        if (!FindCurveOutTurnPoint(curve, nextCurve, startOfTurnPt, isTurnLineSameWay,bnd))
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
    }

    case 2:
    {
        //Bind the two turns together
        int cnt1 = ytList.count();
        int cnt2 = ytList2.count();

        //Find if the turn goes same way as turnline heading
        bool isFirstTurnLineSameWay = true;
        double firstHeadingDifference = fabs(inClosestTurnPt.turnLineHeading - ytList[ytList.count() - 1].heading);
        if (firstHeadingDifference > glm::PIBy2 && firstHeadingDifference < 3 * glm::PIBy2) isFirstTurnLineSameWay = false;

        //finds out start and goal point along the tunline
        FindInnerTurnPoints(ytList[cnt1 - 1], ytList[0].heading, inClosestTurnPt, isFirstTurnLineSameWay, bnd);
        CClose startClosestTurnPt = CClose(closestTurnPt);

        FindInnerTurnPoints(ytList2[cnt2 - 1], ytList2[0].heading + M_PI, outClosestTurnPt, !isFirstTurnLineSameWay, bnd);
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
                ytList.append(Vec3(ytList2[cnt2 - 1]));
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

        if (!AddCurveSequenceLines(curve, nextCurve)) return false;

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
    {
        //how far are we from any turn boundary
        FindABTurnPoint(Vec3(ABLine.rEastAB, ABLine.rNorthAB, head), ABLine, bnd);

        //or did we lose the turnLine - we are on the highway cuz we left the outer/inner turn boundary
        if (closestTurnPt.turnLineIndex != -1)
        {
            //calculate the distance to the turnline
            vehicle.distancePivotToTurnLine = glm::Distance(vehicle.pivotAxlePos, closestTurnPt.closePt);
        }
        else
        {
            //Full emergency stop code goes here, it thinks its auto turn, but its not!
            FailCreate();
            return false;
        }
        inClosestTurnPt = CClose(closestTurnPt);

        CDubins dubYouTurnPath = CDubins();
        CDubinsTurningRadius = youTurnRadius;

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
        break;
    }
    case 1:
    {
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

        if (!AddABSequenceLines(ABLine, vehicle)) return false;

        return true;
    }
    }

    return true;
}

bool CYouTurn::CreateABWideTurn(bool isTurnLeft,
                                int makeUTurnCounter,
                                CVehicle &vehicle,
                                const CBoundary &bnd,
                                CABLine &ABLine,
                                CTrack &trk,
                                int secondsSinceStart)
{
    double tool_width = property_setVehicle_toolWidth;
    double tool_overlap = property_setVehicle_toolOverlap;
    double tool_offset = property_setVehicle_toolOffset;

    //keep from making turns constantly - wait 1.5 seconds
    if (makeUTurnCounter < 4)
    {
        youTurnPhase = 0;
        return true;
    }

    if (!isBtnAutoSteerOn) ABLine.isHeadingSameWay
            = M_PI - fabs(fabs(vehicle.fixHeading - ABLine.abHeading) - M_PI) < glm::PIBy2;

    double head = ABLine.abHeading;
    if (!ABLine.isHeadingSameWay) head += M_PI;
    if (head >= glm::twoPI) head -= glm::twoPI;


    //step 1 turn in to the turnline
    switch (youTurnPhase)
    {
    case 0:
    {
        //grab the pure pursuit point right on ABLine
        Vec3 onPurePoint = Vec3(ABLine.rEastAB, ABLine.rNorthAB, 0);

        //how far are we from any turn boundary
        FindABTurnPoint(onPurePoint, ABLine, bnd);

        //save a copy for first point
        inClosestTurnPt = CClose(closestTurnPt);
        startOfTurnPt = CClose(closestTurnPt);

        //already no turnline
        if (inClosestTurnPt.turnLineIndex == -1)
        {
            FailCreate();
            return false;
        }

        //creates half a circle starting at the crossing point
        ytList.clear();
        Vec3 currentPos = Vec3(inClosestTurnPt.closePt.easting, inClosestTurnPt.closePt.northing, head);
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
        ytList = MoveTurnInsideTurnLine(ytList, head, true, false,vehicle, bnd);

        //if it couldn't be done this will trigger
        if (ytList.count() == 0)
        {
            FailCreate();
            return false;
        }

        vehicle.distancePivotToTurnLine = glm::Distance(ytList[0], vehicle.pivotAxlePos);

        youTurnPhase = 1;
        return true;
    }

    case 1:
    {
        //we move the turnline crossing point perpenicualar out from the ABline
        double turnOffset = (tool_width - tool_overlap) * rowSkipsWidth + (isYouTurnRight ? -tool_offset * 2.0 : tool_offset * 2.0);
        Vec2 tempguidanceLookPos = Vec2(vehicle.guidanceLookPos.easting, vehicle.guidanceLookPos.northing);

        if (!isTurnLeft)
        {
            //creates an imaginary curveline
            vehicle.guidanceLookPos.easting += (cos(head) * turnOffset);
            vehicle.guidanceLookPos.northing -= (sin(head) * turnOffset);
        }
        else
        {
            vehicle.guidanceLookPos.easting -= (cos(head) * turnOffset);
            vehicle.guidanceLookPos.northing += (sin(head) * turnOffset);
        }

        CABLine nextLine;
        nextLine.BuildCurrentABLineList(vehicle.pivotAxlePos,secondsSinceStart,trk,*this,vehicle);
        vehicle.guidanceLookPos = tempguidanceLookPos;

        //going with or against boundary?
        bool isTurnLineSameWay = true;
        double headingDifference = fabs(startOfTurnPt.closePt.heading - ytList[ytList.count() - 1].heading);
        if (headingDifference > glm::PIBy2 && headingDifference < 3 * glm::PIBy2) isTurnLineSameWay = false;

        if (!FindABOutTurnPoint(ABLine, nextLine, inClosestTurnPt, isTurnLineSameWay, ABLine, bnd))
        {
            //error
            FailCreate();
            return false;
        }
        outClosestTurnPt = closestTurnPt;

        Vec3 pointPos = Vec3(outClosestTurnPt.closePt.easting, outClosestTurnPt.closePt.northing, 0);
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
        ytList2 = MoveTurnInsideTurnLine(ytList2, headie, true, true, vehicle, bnd);

        if (ytList2.count() == 0)
        {
            FailCreate();
            return false;
        }

        youTurnPhase = 2;
        return true;
    }

    case 2:
    {
        int cnt1 = ytList.count();
        int cnt2 = ytList2.count();

        //Find if the turn goes same way as turnline heading
        bool isFirstTurnLineSameWay = true;
        double firstHeadingDifference = fabs(inClosestTurnPt.turnLineHeading - ytList[ytList.count() - 1].heading);

        if (firstHeadingDifference > glm::PIBy2 && firstHeadingDifference < 3 * glm::PIBy2) isFirstTurnLineSameWay = false;

        //finds out start and goal point along the tunline
        FindInnerTurnPoints(ytList[cnt1 - 1], ytList[0].heading, inClosestTurnPt, isFirstTurnLineSameWay, bnd);
        CClose startClosestTurnPt = CClose(closestTurnPt);

        FindInnerTurnPoints(ytList2[cnt2 - 1], ytList2[0].heading + M_PI, outClosestTurnPt, !isFirstTurnLineSameWay, bnd);
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
                ytList.append(Vec3(ytList2[cnt2 - 1]));
            }

        }
        else
        {
            //multiple segments
            Vec3 tPoint;
            int turnCount = bnd.bndList[startClosestTurnPt.turnLineNum].turnLine.count();

            //how many points from turnline do we add
            int loops = fabs(startClosestTurnPt.turnLineIndex - goalClosestTurnPt.turnLineIndex);

            //TODO: Generates error if trying to go around more than half of something...... Do we ever?
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
                ytList.append(Vec3(ytList2[cnt2 - 1]));
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
                Vec3 pointB = Vec3((ytList[i].easting + ytList[j].easting) / 2.0,
                                   (ytList[i].northing + ytList[j].northing) / 2.0, ytList[i].heading);

                ytList.insert(j, pointB);
                cnt = ytList.count();
                i--;
            }
        }

        //calculate the new points headings based on fore and aft of point - smoother turns
        QVector<Vec3> arr = ytList;
        cnt = ytList.count();
        cnt -= 2;
        ytList.clear();

        for (int i = 2; i < cnt; i++)
        {
            Vec3 pt3(arr[i]);
            pt3.heading = atan2(arr[i + 1].easting - arr[i - 1].easting,
                                     arr[i + 1].northing - arr[i - 1].northing);
            if (pt3.heading < 0) pt3.heading += glm::twoPI;
            ytList.append(pt3);
        }

        //AddABSequenceLines
        //check to close
        if (glm::Distance(ytList[0], vehicle.pivotAxlePos) < 3)
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

        if (!AddABSequenceLines(ABLine, vehicle)) return false;

        return true;
    }
    }

    //just in case
    return false;
}

bool CYouTurn::KStyleTurnCurve(bool isTurnLeft,
                               int &makeUTurnCounter,
                               CVehicle &vehicle,
                               const CABCurve &curve,
                               const CBoundary &bnd)
{
    double tool_width = property_setVehicle_toolWidth;
    double tool_overlap = property_setVehicle_toolOverlap;
    double tool_offset = property_setVehicle_toolOffset;

    //grab the vehicle widths and offsets
    double turnOffset = (tool_width - tool_overlap) * rowSkipsWidth + (isYouTurnRight ? -tool_offset * 2.0 : tool_offset * 2.0);
    double pointSpacing = youTurnRadius * 0.1;

    isHeadingSameWay = curve.isHeadingSameWay;

    int turnIndex = bnd.IsPointInsideTurnArea(vehicle.pivotAxlePos);
    if (makeUTurnCounter < 4 || turnIndex != 0)
    {
        youTurnPhase = 0;
        return true;
    }

    makeUTurnCounter = 0;

    if (!FindCurveTurnPoints(curve.curList,curve,bnd))
    {
        FailCreate();
        return false;
    }

    //save a copy
    inClosestTurnPt = CClose(closestTurnPt);

    ytList.clear();

    int count = isHeadingSameWay ? -1 : 1;
    int curveIndex = inClosestTurnPt.curveIndex + count;

    bool pointOutOfBnd = true;
    int stopIfWayOut = 0;

    double head = 0;

    while (pointOutOfBnd)
    {
        stopIfWayOut++;
        pointOutOfBnd = false;

        //creates half a circle starting at the crossing point
        ytList.clear();
        if (curveIndex >= curve.curList.count() || curveIndex < 0)
        {
            FailCreate();
            return false;
        }
        Vec3 currentPos(curve.curList[curveIndex]);

        curveIndex += count;

        if (!isHeadingSameWay) currentPos.heading += M_PI;
        if (currentPos.heading >= glm::twoPI) currentPos.heading -= glm::twoPI;

        ytList.append(currentPos);

        while (fabs(ytList[0].heading - currentPos.heading) < 2.2)
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

        for (int i = 0; i < ytList.count(); i++)
        {
            if (bnd.IsPointInsideTurnArea(ytList[i]) != 0)
            {
                pointOutOfBnd = true;
                break;
            }
        }
    }

    //move out
    head = ytList[0].heading;
    double cosHead = cos(head) * 0.1;
    double sinHead = sin(head) * 0.1;
    QVector<Vec3> arr2(ytList);
    ytList.clear();

    //step 2 move the turn inside with steps of 0.1 meter
    int j = 0;
    pointOutOfBnd = false;

    while (!pointOutOfBnd)
    {
        stopIfWayOut++;
        pointOutOfBnd = false;
        vehicle.distancePivotToTurnLine = glm::DistanceSquared(arr2[0], vehicle.pivotAxlePos);

        for (int i = 0; i < arr2.count(); i++)
        {
            arr2[i].easting += (sinHead);
            arr2[i].northing += (cosHead);
        }

        for (j = 0; j < arr2.count(); j++)
        {
            int bob = bnd.IsPointInsideTurnArea(arr2[j]);
            if (bob != 0)
            {
                pointOutOfBnd = true;
                break;
            }
        }

        if (stopIfWayOut == 300 || (vehicle.distancePivotToTurnLine < 6))
        {
            //for some reason it doesn't go inside boundary, return empty list
            return false;
        }
    }

    ytList.append(arr2);

    //add start extension from curve points
    curveIndex -= count;

    //point used to set next guidance line
    kStyleNewLookPos = Vec3(ytList[0]);

    //now we go the other way to turn round
    head = ytList[0].heading;
    head -= M_PI;
    if (head < -M_PI) head += glm::twoPI;
    if (head > M_PI) head -= glm::twoPI;

    if (isTurnLeft)
    {
        kStyleNewLookPos.easting += (cos(-head) * turnOffset);
        kStyleNewLookPos.northing += (sin(-head) * turnOffset);
    }
    else
    {
        kStyleNewLookPos.easting -= (cos(-head) * turnOffset);
        kStyleNewLookPos.northing -= (sin(-head) * turnOffset);
    }

    if (head >= glm::twoPI) head -= glm::twoPI;
    else if (head < 0) head += glm::twoPI;

    kStyleNewLookPos.heading = head;

    //add the tail to first turn
    head = ytList[ytList.count() - 1].heading;

    Vec3 pt;
    for (int i = 1; i <= (int)(3 * turnOffset); i++)
    {
        pt.easting = ytList[ytList.count() - 1].easting + (sin(head) * 0.5);
        pt.northing = ytList[ytList.count() - 1].northing + (cos(head) * 0.5);
        pt.heading = 0;
        ytList.append(pt);
    }

    //leading in line of turn
    for (int i = 0; i < 4; i++)
    {
        ytList.insert(0, Vec3(curve.curList[curveIndex + i * count]));
    }

    //fill in the gaps
    double distance;

    int cnt = ytList.count();
    for (int i = 1; i < cnt - 2; i++)
    {
        j = i + 1;
        if (j == cnt - 1) continue;
        distance = glm::DistanceSquared(ytList[i], ytList[j]);
        if (distance > 1)
        {
            Vec3 pointB((ytList[i].easting + ytList[j].easting) / 2.0,
                (ytList[i].northing + ytList[j].northing) / 2.0, ytList[i].heading);

            ytList.insert(j, pointB);
            cnt = ytList.count();
            i--;
        }
    }

    //calculate line headings
    QVector<Vec3> arr(ytList);
    ytList.clear();

    for (int i = 0; i < arr.count() - 1; i++)
    {
        arr[i].heading = atan2(arr[i + 1].easting - arr[i].easting, arr[i + 1].northing - arr[i].northing);
        if (arr[i].heading < 0) arr[i].heading += glm::twoPI;
        ytList.append(arr[i]);
    }

    vehicle.distancePivotToTurnLine = glm::Distance(ytList[0], vehicle.pivotAxlePos);

    isOutOfBounds = false;
    youTurnPhase = 10;
    turnTooCloseTrigger = false;
    isTurnCreationTooClose = false;

    return true;
}

bool CYouTurn::KStyleTurnAB(bool isTurnLeft,
                            int &makeUTurnCounter,
                            CVehicle &vehicle,
                            const CABLine &ABLine,
                            const CBoundary &bnd)
{
    double tool_width = property_setVehicle_toolWidth;
    double tool_overlap = property_setVehicle_toolOverlap;
    double tool_offset = property_setVehicle_toolOffset;

    double pointSpacing = youTurnRadius * 0.1;

    int turnIndex = bnd.IsPointInsideTurnArea(vehicle.pivotAxlePos);
    if (makeUTurnCounter < 4 || turnIndex != 0)
    {
        youTurnPhase = 0;
        return true;
    }

    makeUTurnCounter = 0;

    //step 1 turn in to the turnline
    if (youTurnPhase == 0)
    {
        isOutOfBounds = true;


        //timer.Start();
        //grab the pure pursuit point right on ABLine
        Vec3 onPurePoint(ABLine.rEastAB, ABLine.rNorthAB, 0);

        //how far are we from any turn boundary
        FindClosestTurnPoint(onPurePoint,ABLine,bnd);

        //save a copy for first point
        inClosestTurnPt = closestTurnPt;

        //already no turnline
        if (inClosestTurnPt.turnLineIndex == -1) return false;

        //calculate the distance to the turnline
        vehicle.distancePivotToTurnLine = glm::Distance(vehicle.pivotAxlePos, closestTurnPt.closePt);

        //point on AB line closest to pivot axle point from ABLine PurePursuit aka where we are
        rEastYT = ABLine.rEastAB;
        rNorthYT = ABLine.rNorthAB;
        isHeadingSameWay = ABLine.isHeadingSameWay;
        double head = ABLine.abHeading;

        if (!isHeadingSameWay) head += M_PI;
        if (head >= glm::twoPI) head -= glm::twoPI;

        //thistance to turnline from where we are
        double turnDiagDistance = vehicle.distancePivotToTurnLine;

        //moves the point to the crossing with the turnline
        rEastYT += (sin(head) * turnDiagDistance);
        rNorthYT += (cos(head) * turnDiagDistance);

        //creates half a circle starting at the crossing point
        ytList.clear();
        Vec3 currentPos(rEastYT, rNorthYT, head);
        ytList.append(currentPos);

        //make semi circle - not quite
        while (fabs(head - currentPos.heading) < 2.2)
        {
            //Update the position of the car
            currentPos.easting += pointSpacing * sin(currentPos.heading);
            currentPos.northing += pointSpacing * cos(currentPos.heading);

            //Which way are we turning?
            double turnParameter = 1.0;

            if (isTurnLeft) turnParameter = -1.0;

            //Update the heading
            currentPos.heading += (pointSpacing / youTurnRadius) * turnParameter;

            //Add the new coordinate to the path
            ytList.append(currentPos);
        }

        //move the half circle to tangent the turnline
        ytList = MoveABTurnInsideTurnLine(ytList, head, vehicle, bnd);

        //if it couldn't be done this will trigger
        if (ytList.count() < 5 || semiCircleIndex == -1)
        {
            FailCreate();
            return false;
        }

        //point used to set next guidance line
        kStyleNewLookPos = ytList[0];


        //grab the vehicle widths and offsets
        double turnOffset = (tool_width - tool_overlap) * rowSkipsWidth + (isYouTurnRight ? -tool_offset : tool_offset);

        //now we go the other way to turn round
        head = ytList[0].heading;
        head -= M_PI;
        if (head < -M_PI) head += glm::twoPI;
        if (head > M_PI) head -= glm::twoPI;

        if (isTurnLeft)
        {
            kStyleNewLookPos.easting += (cos(-head) * turnOffset);
            kStyleNewLookPos.northing += (sin(-head) * turnOffset);
        }
        else
        {
            kStyleNewLookPos.easting -= (cos(-head) * turnOffset);
            kStyleNewLookPos.northing -= (sin(-head) * turnOffset);
        }

        if (head >= glm::twoPI) head -= glm::twoPI;
        else if (head < 0) head += glm::twoPI;

        kStyleNewLookPos.heading = head;

        //add the tail to first turn
        int count = ytList.count();
        head = ytList[count - 1].heading;

        Vec3 pt;
        for (int i = 1; i <= (int)(3 * turnOffset); i++)
        {
            pt.easting = ytList[count - 1].easting + (sin(head) * i * 0.5);
            pt.northing = ytList[count - 1].northing + (cos(head) * i * 0.5);
            pt.heading = 0;
            ytList.append(pt);
        }

        //leading in line of turn
        head = ABLine.abHeading;
        if (isHeadingSameWay) head += M_PI;
        if (head >= glm::twoPI) head -= glm::twoPI;

        for (int a = 0; a < 8; a++)
        {
            pt.easting = ytList[0].easting + (sin(head) * 0.511);
            pt.northing = ytList[0].northing + (cos(head) * 0.511);
            pt.heading = ytList[0].heading;
            ytList.insert(0, pt);
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

        vehicle.distancePivotToTurnLine = glm::Distance(ytList[0], vehicle.pivotAxlePos);

        isOutOfBounds = false;
        youTurnPhase = 10;
        turnTooCloseTrigger = false;
        isTurnCreationTooClose = false;
        return true;
    }

    return true;
}

QVector<Vec3> &CYouTurn::MoveABTurnInsideTurnLine(QVector<Vec3> &uTurnList,
                                                  double head,
                                                  CVehicle &vehicle,
                                                  const CBoundary &bnd)
{
    //step 1 make array out of the list so that we can modify the position
    double cosHead = cos(head);
    double sinHead = sin(head);

    int cnt = uTurnList.count();
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

    //step 5, we ar now inside turnfence by 0-0.1 meters, move the turn forward until it hits the turnfence in steps of 0.05 meters
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
                uTurnList.append(arr2);
                return uTurnList;
            }
        }
    }

    //if empty - no creation.
    return uTurnList;
}

void CYouTurn::FindClosestTurnPoint(Vec3 fromPt,
                                    const CABLine &ABLine,
                                    const CBoundary &bnd)
{
    double eP = fromPt.easting;
    double nP = fromPt.northing;
    double eAB, nAB;
    turnClosestList.clear();

    CClose cClose;

    if (ABLine.isHeadingSameWay)
    {
        eAB = ABLine.currentLinePtB.easting;
        nAB = ABLine.currentLinePtB.northing;
    }
    else
    {
        eAB = ABLine.currentLinePtA.easting;
        nAB = ABLine.currentLinePtA.northing;
    }

    turnClosestList.clear();

    for (int j = 0; j < bnd.bndList.count(); j++)
    {
        for (int i = 0; i < bnd.bndList[j].turnLine.count() - 1; i++)
        {
            int res = GetLineIntersection(
                bnd.bndList[j].turnLine[i].easting,
                bnd.bndList[j].turnLine[i].northing,
                bnd.bndList[j].turnLine[i + 1].easting,
                bnd.bndList[j].turnLine[i + 1].northing,
                eP, nP, eAB, nAB, iE, iN
            );

            if (res == 1)
            {
                cClose = CClose();
                cClose.closePt.easting = iE;
                cClose.closePt.northing = iN;

                double hed = atan2(bnd.bndList[j].turnLine[i + 1].easting - bnd.bndList[j].turnLine[i].easting,
                    bnd.bndList[j].turnLine[i + 1].northing - bnd.bndList[j].turnLine[i].northing);
                if (hed < 0) hed += glm::twoPI;
                cClose.closePt.heading = hed;
                cClose.turnLineNum = j;
                cClose.turnLineIndex = i;

                turnClosestList.append(cClose);
            }
        }
    }

    //determine closest point
    double minDistance = glm::DOUBLE_MAX;

    if (turnClosestList.count() > 0)
    {
        for (int i = 0; i < turnClosestList.count(); i++)
        {
            double dist = (((fromPt.easting - turnClosestList[i].closePt.easting) * (fromPt.easting - turnClosestList[i].closePt.easting))
                            + ((fromPt.northing - turnClosestList[i].closePt.northing) * (fromPt.northing - turnClosestList[i].closePt.northing)));

            if (minDistance >= dist)
            {

                minDistance = dist;
                closestTurnPt = turnClosestList[i];
            }
        }
    }
}

//needs CABCurve, CBoundary, called from CYouTurn::buildCurvePatternYouTurn
bool CYouTurn::FindCurveTurnPoints(const QVector<Vec3> &xList,
                                   const CABCurve &curve,
                                   const CBoundary &bnd
                                   )
{
    //find closet AB Curve point that will cross and go out of bounds
    int Count = curve.isHeadingSameWay ? 1 : -1;
    int turnNum = 99;
    int j;

    closestTurnPt = CClose();

    for (j = curve.currentLocationIndex; j > 0 && j < xList.count(); j += Count)
    {
        int turnIndex = bnd.IsPointInsideTurnArea(xList[j]);
        if (turnIndex != 0)
        {
            closestTurnPt.curveIndex = j - Count;
            closestTurnPt.turnLineNum = turnIndex;
            turnNum = turnIndex;
            break;
        }
    }

    if (turnNum < 0) //uturn will be on outer boundary turn
    {
        closestTurnPt.turnLineNum = 0;
        turnNum = 0;
    }
    else if (turnNum == 99)
    {
        //curve does not cross a boundary - oops
        isTurnCreationNotCrossingError = true;
        return false;
    }

    if (closestTurnPt.curveIndex == -1)
    {
        isTurnCreationNotCrossingError = true;
        return false;
    }


    for (int i = 0; i < bnd.bndList[turnNum].turnLine.count() - 1; i++)
    {
        int res = GetLineIntersection(
                bnd.bndList[turnNum].turnLine[i].easting,
                bnd.bndList[turnNum].turnLine[i].northing,
                bnd.bndList[turnNum].turnLine[i + 1].easting,
                bnd.bndList[turnNum].turnLine[i + 1].northing,

                xList[closestTurnPt.curveIndex].easting,
                xList[closestTurnPt.curveIndex].northing,
                xList[closestTurnPt.curveIndex + Count].easting,
                xList[closestTurnPt.curveIndex + Count].northing,

                iE, iN);

        if (res == 1)
        {
            closestTurnPt.closePt.easting = iE;
            closestTurnPt.closePt.northing = iN;

            double hed = atan2(bnd.bndList[turnNum].turnLine[i + 1].easting - bnd.bndList[turnNum].turnLine[i].easting,
                bnd.bndList[turnNum].turnLine[i + 1].northing - bnd.bndList[turnNum].turnLine[i].northing);
            if (hed < 0) hed += glm::twoPI;
            crossingheading = hed;
            closestTurnPt.closePt.heading = hed;
            closestTurnPt.turnLineIndex = i;

            break;
        }
    }

    return closestTurnPt.turnLineIndex != -1 && closestTurnPt.curveIndex != -1;
    //return true;


}

bool CYouTurn::FindCurveOutTurnPoint(CABCurve &thisCurve,
                                     CABCurve &nextCurve,
                                     CClose inPt,
                                     bool isTurnLineSameWay,
                                     const CBoundary &bnd
                                     )
{
    int a = isTurnLineSameWay ? 1 : -1;

    int turnLineIndex = inPt.turnLineIndex;
    int turnLineNum = inPt.turnLineNum;
    int stopTurnLineIndex = inPt.turnLineIndex - a;
    if (stopTurnLineIndex < 0) stopTurnLineIndex = bnd.bndList[turnLineNum].turnLine.count() - 3;
    if (stopTurnLineIndex > bnd.bndList[turnLineNum].turnLine.count() - 1) turnLineIndex = 3;


    for (; turnLineIndex != stopTurnLineIndex; turnLineIndex += a)
    {
        if (turnLineIndex < 0) turnLineIndex = bnd.bndList[turnLineNum].turnLine.count() - 2; //AAA could be less than 0???
        if (turnLineIndex > bnd.bndList[turnLineNum].turnLine.count() - 2) turnLineIndex = 0;

        for (int i = 0; i < nextCurve.curList.count() - 2; i++)
        {
            int res = GetLineIntersection(
                            bnd.bndList[turnLineNum].turnLine[turnLineIndex].easting,
                            bnd.bndList[turnLineNum].turnLine[turnLineIndex].northing,
                            bnd.bndList[turnLineNum].turnLine[turnLineIndex + 1].easting,
                            bnd.bndList[turnLineNum].turnLine[turnLineIndex + 1].northing,

                            nextCurve.curList[i].easting,
                            nextCurve.curList[i].northing,
                            nextCurve.curList[i + 1].easting,
                            nextCurve.curList[i + 1].northing,
                            iE, iN);
            if (res == 1)
            {
                closestTurnPt = CClose();
                closestTurnPt.closePt.easting = iE;
                closestTurnPt.closePt.northing = iN;
                closestTurnPt.closePt.heading = nextCurve.curList[i].heading;
                closestTurnPt.turnLineIndex = turnLineIndex;
                closestTurnPt.curveIndex = i;
                closestTurnPt.turnLineHeading = bnd.bndList[turnLineNum].turnLine[turnLineIndex].heading;
                closestTurnPt.turnLineNum = turnLineNum;
                return true;
            }
        }

        for (int i = 0; i < thisCurve.curList.count() - 2; i++)
        {
            int res = GetLineIntersection(
                            bnd.bndList[turnLineNum].turnLine[turnLineIndex].easting,
                            bnd.bndList[turnLineNum].turnLine[turnLineIndex].northing,
                            bnd.bndList[turnLineNum].turnLine[turnLineIndex + 1].easting,
                            bnd.bndList[turnLineNum].turnLine[turnLineIndex + 1].northing,

                            thisCurve.curList[i].easting,
                            thisCurve.curList[i].northing,
                            thisCurve.curList[i + 1].easting,
                            thisCurve.curList[i + 1].northing,

                            iE, iN);
            if (res == 1)
            {
                if ((i < inPt.curveIndex && thisCurve.isHeadingSameWay) || (i > inPt.curveIndex && !thisCurve.isHeadingSameWay))
                {
                    return false; //hitting the curve behind us
                }
                else if (i == inPt.curveIndex)
                {
                    //do nothing hitting the curve at the same place as in
                }
                else
                {
                    closestTurnPt = CClose();
                    closestTurnPt.closePt.easting = iE;
                    closestTurnPt.closePt.northing = iN;
                    closestTurnPt.closePt.heading = thisCurve.curList[i].heading; //ändrad nyss till this curve
                    closestTurnPt.turnLineIndex = turnLineIndex;
                    closestTurnPt.curveIndex = i;
                    closestTurnPt.turnLineHeading = bnd.bndList[turnLineNum].turnLine[turnLineIndex].heading;
                    closestTurnPt.turnLineNum = turnLineNum;
                    isOutSameCurve = true;
                    nextCurve = thisCurve;
                    return true;
                }
            }
        }
    }
    return false;
}

bool CYouTurn::FindABOutTurnPoint(CABLine &thisCurve,
                                  CABLine &nextCurve,
                                  CClose inPt, bool isTurnLineSameWay,
                                  const CABLine &ABLine,
                                  const CBoundary &bnd
                                  )
{
    int a = isTurnLineSameWay ? 1 : -1;

    int turnLineIndex = inPt.turnLineIndex;
    int turnLineNum = inPt.turnLineNum;
    int stopTurnLineIndex = inPt.turnLineIndex - a;
    if (stopTurnLineIndex < 0) stopTurnLineIndex = bnd.bndList[turnLineNum].turnLine.count() - 3;
    if (stopTurnLineIndex > bnd.bndList[turnLineNum].turnLine.count() - 1) turnLineIndex = 3;


    for (; turnLineIndex != stopTurnLineIndex; turnLineIndex += a)
    {
        if (turnLineIndex < 0) turnLineIndex = bnd.bndList[turnLineNum].turnLine.count() - 2;
        if (turnLineIndex > bnd.bndList[turnLineNum].turnLine.count() - 2) turnLineIndex = 0;


        int res = GetLineIntersection(
                        bnd.bndList[turnLineNum].turnLine[turnLineIndex].easting,
                        bnd.bndList[turnLineNum].turnLine[turnLineIndex].northing,
                        bnd.bndList[turnLineNum].turnLine[turnLineIndex + 1].easting,
                        bnd.bndList[turnLineNum].turnLine[turnLineIndex + 1].northing,

                        nextCurve.currentLinePtA.easting,
                        nextCurve.currentLinePtA.northing,
                        nextCurve.currentLinePtB.easting,
                        nextCurve.currentLinePtB.northing,
                        iE, iN);
        if (res == 1)
        {
            closestTurnPt = CClose();
            closestTurnPt.closePt.easting = iE;
            closestTurnPt.closePt.northing = iN;
            closestTurnPt.closePt.heading = nextCurve.abHeading;
            closestTurnPt.turnLineIndex = turnLineIndex;
            closestTurnPt.curveIndex = -1;
            closestTurnPt.turnLineHeading = bnd.bndList[turnLineNum].turnLine[turnLineIndex].heading;
            closestTurnPt.turnLineNum = turnLineNum;
            return true;
        }



        int res2 = GetLineIntersection(
                        bnd.bndList[turnLineNum].turnLine[turnLineIndex].easting,
                        bnd.bndList[turnLineNum].turnLine[turnLineIndex].northing,
                        bnd.bndList[turnLineNum].turnLine[turnLineIndex + 1].easting,
                        bnd.bndList[turnLineNum].turnLine[turnLineIndex + 1].northing,

                        thisCurve.currentLinePtA.easting,
                        thisCurve.currentLinePtA.northing,
                        thisCurve.currentLinePtB.easting,
                        thisCurve.currentLinePtB.northing,

                        iE, iN);
        if (res2 == 1)
        {
            double hed;
            if (ABLine.isHeadingSameWay) hed = atan2(ABLine.rEastAB - iE, ABLine.rNorthAB - iN);
            else hed = atan2(iE - ABLine.rEastAB, iN - ABLine.rNorthAB);
            if (hed < 0) hed += glm::twoPI;
            //hed = Math.Round(hed, 3);
            //double hedAB = Math.Round(thisCurve.abHeading, 3);

            if (fabs(hed - thisCurve.abHeading) < 0.0001)
            {
                return false; //hitting the curve behind us
            }
            else if (turnLineIndex == inPt.turnLineIndex)
            {
                //do nothing hitting the curve at the same place as in
            }
            else
            {
                closestTurnPt = CClose();
                closestTurnPt.closePt.easting = iE;
                closestTurnPt.closePt.northing = iN;
                closestTurnPt.closePt.heading = thisCurve.abHeading;
                closestTurnPt.turnLineIndex = turnLineIndex;
                closestTurnPt.curveIndex = -1;
                closestTurnPt.turnLineHeading = bnd.bndList[turnLineNum].turnLine[turnLineIndex].heading;
                closestTurnPt.turnLineNum = turnLineNum;
                isOutSameCurve = true;
                nextCurve = thisCurve;
                return true;
            }
        }
    }
    return false;

}

bool CYouTurn::FindInnerTurnPoints(Vec3 fromPt,
                                   double inDirection,
                                   CClose refClosePt,
                                   bool isTurnLineSameWay,
                                   const CBoundary &bnd
                                   )
{
    double eP, nP;

    eP = fromPt.easting + sin(inDirection);
    nP = fromPt.northing + cos(inDirection);

    int a = isTurnLineSameWay ? 1 : -1;

    int turnLineIndex = refClosePt.turnLineIndex;
    int turnLineNum = refClosePt.turnLineNum;
    int stopTurnLineIndex = refClosePt.turnLineIndex - a;
    if (stopTurnLineIndex < 0) stopTurnLineIndex = bnd.bndList[turnLineNum].turnLine.count() - 3;
    if (stopTurnLineIndex > bnd.bndList[turnLineNum].turnLine.count() - 1) turnLineIndex = 3;


    for (; turnLineIndex != stopTurnLineIndex; turnLineIndex += a)
    {
        if (turnLineIndex < 0) turnLineIndex = bnd.bndList[turnLineNum].turnLine.count() - 2;
        if (turnLineIndex > bnd.bndList[turnLineNum].turnLine.count() - 2) turnLineIndex = 0;


        int res = GetLineIntersection(
            bnd.bndList[turnLineNum].turnLine[turnLineIndex].easting,
            bnd.bndList[turnLineNum].turnLine[turnLineIndex].northing,
            bnd.bndList[turnLineNum].turnLine[turnLineIndex + 1].easting,
            bnd.bndList[turnLineNum].turnLine[turnLineIndex + 1].northing,

            fromPt.easting,
            fromPt.northing, eP, nP, iE, iN);
        if (res == 1)
        {
            closestTurnPt = CClose();
            closestTurnPt.closePt.easting = iE;
            closestTurnPt.closePt.northing = iN;
            closestTurnPt.closePt.heading = -1; //isnt needed but could be calculated
            closestTurnPt.turnLineIndex = turnLineIndex;
            closestTurnPt.curveIndex = -1;
            closestTurnPt.turnLineHeading = bnd.bndList[turnLineNum].turnLine[turnLineIndex].heading;
            closestTurnPt.turnLineNum = turnLineNum;
            return true;
        }

    }
    return false;
}

bool CYouTurn::FindCurveTurnPoint(const CABCurve &thisCurve,
                                  const CBoundary &bnd
                                  )
{
    //AAA Is updated but not tested....
    //find closet AB Curve point that will cross and go out of bounds
    //NOTE: as of v6.3.3 thisCurve is also the same as mf.curve so
    //I use it in the line below instead of mf.curve
    int Count = thisCurve.isHeadingSameWay ? 1 : -1;
    int turnNum = 99;
    int j;

    closestTurnPt = CClose();

    for (j = thisCurve.currentLocationIndex; j > 0 && j < thisCurve.curList.count(); j += Count)
    {
        int turnIndex = bnd.IsPointInsideTurnArea(thisCurve.curList[j]);
        if (turnIndex != 0)
        {
            closestTurnPt.curveIndex = j - Count;
            closestTurnPt.turnLineNum = turnIndex;
            turnNum = turnIndex;
            break;
        }
    }

    if (turnNum < 0) //uturn will be on outer boundary turn
    {
        closestTurnPt.turnLineNum = 0;
        turnNum = 0;
    }
    else if (turnNum == 99)
    {
        //curve does not cross a boundary - oops
        isTurnCreationNotCrossingError = true;
        return false;
    }

    if (closestTurnPt.curveIndex == -1)
    {
        isTurnCreationNotCrossingError = true;
        return false;
    }


    for (int i = 0; i < bnd.bndList[turnNum].turnLine.count() - 1; i++)
    {
        int res = GetLineIntersection(
                bnd.bndList[turnNum].turnLine[i].easting,
                bnd.bndList[turnNum].turnLine[i].northing,
                bnd.bndList[turnNum].turnLine[i + 1].easting,
                bnd.bndList[turnNum].turnLine[i + 1].northing,

                thisCurve.curList[closestTurnPt.curveIndex].easting,
                thisCurve.curList[closestTurnPt.curveIndex].northing,
                thisCurve.curList[closestTurnPt.curveIndex + Count].easting,
                thisCurve.curList[closestTurnPt.curveIndex + Count].northing,

                iE, iN);

        if (res == 1)
        {
            closestTurnPt.closePt.easting = iE;
            closestTurnPt.closePt.northing = iN;

            /*double hed = Math.Atan2(mf.bnd.bndList[turnNum].turnLine[i + 1].easting - mf.bnd.bndList[turnNum].turnLine[i].easting,
                mf.bnd.bndList[turnNum].turnLine[i + 1].northing - mf.bnd.bndList[turnNum].turnLine[i].northing);
            if (hed < 0) hed += glm.twoPI;
            crossingheading = hed;*/
            closestTurnPt.closePt.heading = thisCurve.curList[closestTurnPt.curveIndex].heading;
            closestTurnPt.turnLineIndex = i;
            closestTurnPt.turnLineNum = turnNum;
            closestTurnPt.turnLineHeading = bnd.bndList[turnNum].turnLine[i].heading;
            if (!thisCurve.isHeadingSameWay && closestTurnPt.curveIndex > 0) closestTurnPt.curveIndex--;

            break;
        }
    }

    return closestTurnPt.turnLineIndex != -1 && closestTurnPt.curveIndex != -1;
}

void CYouTurn::FindABTurnPoint(Vec3 fromPt,
                               const CABLine &ABLine,
                               const CBoundary &bnd
                               )
{
    double eP = fromPt.easting;
    double nP = fromPt.northing;
    double eAB, nAB;
    turnClosestList.clear();

    CClose cClose;

    if (ABLine.isHeadingSameWay)
    {
        eAB = ABLine.currentLinePtB.easting;
        nAB = ABLine.currentLinePtB.northing;
    }
    else
    {
        eAB = ABLine.currentLinePtA.easting;
        nAB = ABLine.currentLinePtA.northing;
    }

    turnClosestList.clear();

    for (int j = 0; j < bnd.bndList.count(); j++)
    {
        for (int i = 0; i < bnd.bndList[j].turnLine.count() - 1; i++)
        {
            int res = GetLineIntersection(
                bnd.bndList[j].turnLine[i].easting,
                bnd.bndList[j].turnLine[i].northing,
                bnd.bndList[j].turnLine[i + 1].easting,
                bnd.bndList[j].turnLine[i + 1].northing,
                eP, nP, eAB, nAB, iE, iN
            );

            if (res == 1)
            {
                cClose = CClose();
                cClose.closePt.easting = iE;
                cClose.closePt.northing = iN;

                double hed = atan2(bnd.bndList[j].turnLine[i + 1].easting - bnd.bndList[j].turnLine[i].easting,
                    bnd.bndList[j].turnLine[i + 1].northing - bnd.bndList[j].turnLine[i].northing);
                if (hed < 0) hed += glm::twoPI;
                cClose.closePt.heading = hed;
                cClose.turnLineHeading = bnd.bndList[j].turnLine[i].heading;
                cClose.turnLineNum = j;
                cClose.turnLineIndex = i;

                turnClosestList.append(cClose);
            }
        }
    }

    //determine closest point
    double minDistance = glm::DOUBLE_MAX;

    if (turnClosestList.count() > 0)
    {
        for (int i = 0; i < turnClosestList.count(); i++)
        {
            double dist = (((fromPt.easting - turnClosestList[i].closePt.easting) * (fromPt.easting - turnClosestList[i].closePt.easting))
                            + ((fromPt.northing - turnClosestList[i].closePt.northing) * (fromPt.northing - turnClosestList[i].closePt.northing)));

            if (minDistance >= dist)
            {

                minDistance = dist;
                closestTurnPt = turnClosestList[i];
            }
        }
    }
}

bool CYouTurn::AddABSequenceLines(const CABLine &ABLine,
                                  const CVehicle &vehicle
    )
{
    double inhead = ABLine.abHeading;
    if (!ABLine.isHeadingSameWay) inhead += M_PI;
    if (inhead > glm::twoPI) inhead -= glm::twoPI;

    double outhead = inhead;
    if (isOutSameCurve) outhead += M_PI;
    if (outhead > glm::twoPI) outhead -= glm::twoPI;

    //how many points straight out
    double lenny = 15;

    Vec3 pt;
    for (int a = 0; a < lenny; a++)
    {
        pt.easting = ytList[0].easting - (sin(inhead) * pointSpacing);
        pt.northing = ytList[0].northing - (cos(inhead) * pointSpacing);
        pt.heading = inhead;
        ytList.insert(0, pt);
    }

    int count = ytList.count();

    for (int i = 0; i <= lenny; i++)
    {
        pt.easting = ytList[count - 1].easting - (sin(outhead) * i * pointSpacing);
        pt.northing = ytList[count - 1].northing - (cos(outhead) * i * pointSpacing);
        pt.heading = outhead;
        ytList.append(pt);
    }

    double distancePivotToTurnLine;
    count = ytList.count();
    for (int i = 0; i < count; i += 2)
    {
        distancePivotToTurnLine = glm::DistanceSquared(ytList[i], vehicle.pivotAxlePos);
        if (distancePivotToTurnLine > 3)
        {
            isTurnCreationTooClose = false;
        }
        else
        {
            //set the flag to Critical stop machine
            FailCreate();
            return false;
        }
    }

    return true;

}

bool CYouTurn::AddCurveSequenceLines(const CABCurve &curve,
                                     const CABCurve &nextCurve
                                     )
{
    //how many points striaght out
    double lenny = 5;
    bool sameWay = curve.isHeadingSameWay;
    int a = sameWay ? -1 : 1;

    for (int i = 0; i < lenny && i > -lenny; i += a)
    {
        ytList.insert(0, Vec3(curve.curList[inClosestTurnPt.curveIndex]));
        inClosestTurnPt.curveIndex += a;
        if (inClosestTurnPt.curveIndex < 2 || inClosestTurnPt.curveIndex > curve.curList.count() - 3)
        {
            FailCreate();
            return false;
        }
    }
    if (isOutSameCurve) sameWay = !sameWay;
    a = sameWay ? -1 : 1;

    for (int i = 0; i < lenny && i > -lenny; i += a)
    {
        ytList.append(Vec3(nextCurve.curList[outClosestTurnPt.curveIndex]));
        outClosestTurnPt.curveIndex += a;
        if (outClosestTurnPt.curveIndex < 2 || outClosestTurnPt.curveIndex > curve.curList.count() - 3)
        {
            FailCreate();
            return false;
        }
    }

    return true;
}

int CYouTurn::GetLineIntersection(double p0x, double p0y, double p1x, double p1y, double p2x, double p2y, double p3x, double p3y, double &iEast, double &iNorth)
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

QVector<Vec3> CYouTurn::MoveTurnInsideTurnLine(QVector<Vec3> uTurnList,
                                               double head,
                                               bool deleteSecondHalf,
                                               bool invertHeading,
                                               CVehicle &vehicle,
                                               const CBoundary &bnd)
{
    //step 1 make array out of the list so that we can modify the position
    double cosHead = cos(head);
    double sinHead = sin(head);
    int cnt = uTurnList.count();
    QVector<Vec3> arr2(uTurnList);
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
void CYouTurn::YouTurnTrigger(const CTrack &trk, CVehicle &vehicle, CABLine &ABLine, CABCurve &curve)
{
    //trigger pulled
    isYouTurnTriggered = true;

    if (isGoingStraightThrough)
        isYouTurnRight = !isYouTurnRight;

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
            vehicle.guidanceLookPos.easting = kStyleNewLookPos.easting;
            vehicle.guidanceLookPos.northing = kStyleNewLookPos.northing;

            pt3Phase = 0;
    }

    if (trk.idx > -1 && trk.gArr.count() > 0)
    {
        if (trk.gArr[trk.idx].mode == (int)TrackMode::AB)
        {
            if (!isGoingStraightThrough)
                ABLine.isLateralTriggered = true;
            ABLine.isABValid = false;
        }
        else
        {
            if (!isGoingStraightThrough)
                curve.isLateralTriggered = true;
            curve.isCurveValid = false;
            curve.lastHowManyPathsAway = 98888;
        }
    }
}

//Normal copmpletion of youturn
void CYouTurn::CompleteYouTurn(int &makeUTurnCounter)
{
    isYouTurnTriggered = false;
    ResetCreatedYouTurn(makeUTurnCounter);
    //mf.sounds.isBoundAlarming = false;
    emit turnOffBoundAlarm();
}

void CYouTurn::Set_Alternate_skips()
{
    rowSkipsWidth2 = rowSkipsWidth;
    turnSkips = rowSkipsWidth2 * 2 - 1;
    previousBigSkip = false;
}


//something went seriously wrong so reset everything
void CYouTurn::ResetYouTurn(int &makeUTurnCounter)
{
    //fix you turn
    isYouTurnTriggered = false;
    makeUTurnCounter = 0;
    ytList.clear();
    ResetCreatedYouTurn(makeUTurnCounter);
    //mf.isBoundAlarming = false;
    emit turnOffBoundAlarm();
    isTurnCreationTooClose = false;
    isTurnCreationNotCrossingError = false;
    emit uTurnReset(); //receiver will cancel pgn 239.uturn byte
}

void CYouTurn::ResetCreatedYouTurn(int &makeUturnCounter)
{
    youTurnPhase = 0;
    ytList.clear();
    pt3Phase = 0;
    makeUturnCounter = 0;
    emit uTurnReset(); //ask receiver to cancel pgn 239 uturn byte
    isOutSameCurve = false;
    isGoingStraightThrough = false;
}

void CYouTurn::FailCreate()
{
    isOutOfBounds = true;
    isTurnCreationTooClose = true;
    emit outOfBounds();
    youTurnPhase = 11;
}

//build the points and path of youturn to be scaled and transformed
void CYouTurn::BuildManualYouLateral(bool isTurnRight,
                                     CVehicle &vehicle,
                                     const CTrack &trk,
                                     CABLine &ABLine,
                                     CABCurve &curve
                                     )
{
    double tool_toolWidth = property_setVehicle_toolWidth;
    double tool_toolOverlap = property_setVehicle_toolOverlap;

    double head;
    //point on AB line closest to pivot axle point from ABLine PurePursuit
    if (trk.idx > -1 && trk.gArr.count() >0)
    {
        if (trk.gArr[trk.idx].mode == (int)TrackMode::AB)
        {
            rEastYT = ABLine.rEastAB;
            rNorthYT = ABLine.rNorthAB;
            isHeadingSameWay = ABLine.isHeadingSameWay;
            head = ABLine.abHeading;
            ABLine.isLateralTriggered = true;
        }
        else
        {
            rEastYT = curve.rEastCu;
            rNorthYT = curve.rNorthCu;
            isHeadingSameWay = curve.isHeadingSameWay;
            head = curve.manualUturnHeading;
            curve.isLateralTriggered = true;
        }
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

void CYouTurn::BuildManualYouTurn(bool isTurnRight,
                                  bool isTurnButtonTriggered,
                                  CVehicle &vehicle,
                                  const CTrack &trk,
                                  CABLine &ABLine,
                                  CABCurve &curve)
{
    double minTurningRadius = property_setVehicle_minTurningRadius;
    double tool_toolWidth = property_setVehicle_toolWidth;
    double tool_toolOverlap = property_setVehicle_toolOverlap;
    double tool_toolOffset = property_setVehicle_toolOffset;

    isYouTurnTriggered = true;

    double head;
    //point on AB line closest to pivot axle point from ABLine PurePursuit
    if (trk.idx > -1 && trk.gArr.count() > 0)
    {
        if (trk.gArr[trk.idx].mode == (int)TrackMode::AB)
        {
            rEastYT = ABLine.rEastAB;
            rNorthYT = ABLine.rNorthAB;
            isHeadingSameWay = ABLine.isHeadingSameWay;
            head = ABLine.abHeading;
            ABLine.isLateralTriggered = true;
        }
        else
        {
            rEastYT = curve.rEastCu;
            rNorthYT = curve.rNorthCu;
            isHeadingSameWay = curve.isHeadingSameWay;
            head = curve.manualUturnHeading;
            curve.isLateralTriggered = true;
        }
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
    curve.lastHowManyPathsAway = 98888;
}

//determine distance from youTurn guidance line
bool CYouTurn::DistanceFromYouTurnLine(CVehicle &vehicle,
                                       CNMEA &pn,
                                       int &makeUTurnCounter)
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
                CompleteYouTurn(makeUTurnCounter);
                return false;
            }

            //just need to make sure the points continue ascending or heading switches all over the place
            if (A > B)
            {
                int C;
                C = A; A = B; B = C;
            }

            //feed backward to turn slower to keep pivot on
            if (A < 0)
            {
                A = 0;
            }
            B = A + 1;

            //return and reset if too far away or end of the line
            if (B >= ptCount - 8)
            {
                CompleteYouTurn(makeUTurnCounter);
                return false;
            }

            if (uTurnStyle == 1 && pt3Phase == 0 && vehicle.isReverse)
            {
                CompleteYouTurn(makeUTurnCounter);
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
            if (A > B)
            {
                int C;
                C = A; A = B; B = C;
            }

            onA = A;
            double distancePiv = glm::Distance(ytList[A], pivot);

            if (distancePiv > 1 || (B >= ptCount - 1))
            {
                {
                    CompleteYouTurn(makeUTurnCounter);
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
                    CompleteYouTurn(makeUTurnCounter);
                    return false;
                }

                if (uTurnStyle == 1 && pt3Phase == 0 && vehicle.isReverse)
                {
                    CompleteYouTurn(makeUTurnCounter);
                    return false;
                }
            }

            //calc "D" the distance from pivot axle to lookahead point
            double goalPointDistanceSquared = glm::DistanceSquared(goalPointYT.northing, goalPointYT.easting, pivot.northing, pivot.easting);

            //calculate the the delta x in local coordinates and steering angle degrees based on wheelbase
            double localHeading = glm::twoPI - vehicle.fixHeading;
            ppRadiusYT = goalPointDistanceSquared / (2 * (((goalPointYT.easting - pivot.easting) * cos(localHeading)) + ((goalPointYT.northing - pivot.northing) * sin(localHeading))));

            steerAngleYT = glm::toDegrees(atan(2 * (((goalPointYT.easting - pivot.easting) * cos(localHeading))
                                                        + ((goalPointYT.northing - pivot.northing) * sin(localHeading))) * wheelbase / goalPointDistanceSquared));

            steerAngleYT *= vehicle.uturnCompensation;

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
        CompleteYouTurn(makeUTurnCounter);
        return false;
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
