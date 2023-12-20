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
    //TODO: these settings
    setAlternateSkips();


    //youTurnRadius = Properties.Settings.Default.set_youTurnRadius;
    //uTurnStyle = Properties.Settings.Default.set_uTurnStyle;
    //uTurnSmoothing = Properties.Settings.Default.setAS_uTurnSmoothing;
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
        int turnIndex = bnd.isPointInsideTurnArea(curve.curList[j]);
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
        int res = getLineIntersection(
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

void CYouTurn::addSequenceLines(double head, Vec3 pivot)
{
    int youTurnStartOffset = property_set_youTurnExtensionLength;

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

bool CYouTurn::buildABLineDubinsYouTurn(CVehicle &vehicle,
                                        const CBoundary &bnd,
                                        const CABLine &ABLine,
                                        bool isTurnRight)
{
    //double uturnDistanceFromBoundary = property_set_youTurnDistanceFromBoundary;
    double youTurnRadius = property_set_youTurnRadius;
    int uTurnStyle = property_set_uTurnStyle;
    double minTurningRadius = property_setVehicle_minTurningRadius;
    int rowSkipsWidth = property_set_youSkipWidth;
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
            bnd.findClosestTurnPoint(onPurePoint);

            //or did we lose the turnLine - we are on the highway cuz we left the outer/inner turn boundary
            if ((int)bnd.closestTurnPt.easting != -20000)
            {
                vehicle.distancePivotToTurnLine = glm::distance(vehicle.pivotAxlePos, bnd.closestTurnPt);
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
            addSequenceLines(head,vehicle.pivotAxlePos);
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
                vehicle.distancePivotToTurnLine = glm::distance(ytList[0], vehicle.pivotAxlePos);
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
                    if (bnd.isPointInsideTurnArea(ytList[j]) != 0)
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
                vehicle.distancePivotToTurnLine = glm::distance(ytList[0], vehicle.pivotAxlePos);
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
                    if (bnd.isPointInsideTurnArea(ytList[j]) != 0)
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
        bnd.findClosestTurnPoint(onPurePoint);

        //or did we lose the turnLine - we are on the highway cuz we left the outer/inner turn boundary
        if ((int)bnd.closestTurnPt.easting != -20000)
        {
                vehicle.distancePivotToTurnLine = glm::distance(vehicle.pivotAxlePos, bnd.closestTurnPt);
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

bool CYouTurn::buildCurveDubinsYouTurn(CVehicle &vehicle,
                                       const CBoundary &bnd,
                                       const CABCurve &curve,
                                       bool isTurnRight, Vec3 pivotPos)
{
    double youTurnRadius = property_set_youTurnRadius;
    double minTurningRadius = property_setVehicle_minTurningRadius;
    int rowSkipsWidth = property_set_youSkipWidth;
    double tool_toolWidth = property_setVehicle_toolWidth;
    double tool_toolOverlap = property_setVehicle_toolOverlap;
    double tool_toolOffset = property_setVehicle_toolOffset;

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
        addSequenceLines(head, vehicle.pivotAxlePos);
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
                if (bnd.isPointInsideTurnArea(ytList[j]) != 0)
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

            tooClose = glm::distance(ytList[0], pivotPos);
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

void CYouTurn::smoothYouTurn(int smPts)
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
void CYouTurn::youTurnTrigger(CVehicle &vehicle, CABLine &ABLine, CABCurve &curve)
{
    int uTurnStyle = property_set_uTurnStyle;
    int rowSkipsWidth = property_set_youSkipWidth;

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
void CYouTurn::completeYouTurn()
{
    isYouTurnTriggered = false;
    resetCreatedYouTurn();
    //emit resetSequenceEventTriggers();
    //mf.seq.isSequenceTriggered = false;
    //emit setTriggerSequence(false);
    //mf.isBoundAlarming = false;
    emit turnOffBoundAlarm();
}

void CYouTurn::setAlternateSkips()
{
    int rowSkipsWidth = property_set_youSkipWidth;

    rowSkipsWidth2 = rowSkipsWidth;
    turnSkips = rowSkipsWidth2 * 2 - 1;
    previousBigSkip = false;
}


//something went seriously wrong so reset everything
void CYouTurn::resetYouTurn()
{
    //fix you turn
    isYouTurnTriggered = false;
    ytList.clear();
    resetCreatedYouTurn();
    //mf.isBoundAlarming = false;
    emit turnOffBoundAlarm();

    isTurnCreationTooClose = false;
    isTurnCreationNotCrossingError = false;
}

void CYouTurn::resetCreatedYouTurn()
{
    youTurnPhase = 0;
    ytList.clear();
    pt3ListSecondLine.clear();
    pt3Phase = 0;
}

//build the points and path of youturn to be scaled and transformed
void CYouTurn::buildManualYouLateral(CVehicle &vehicle, CABLine &ABLine, CABCurve &curve,
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
void CYouTurn::buildManualYouTurn(CVehicle &vehicle, CABLine &ABLine, CABCurve &curve,
                                  bool isTurnRight, bool isTurnButtonTriggered)
{
    double minTurningRadius = property_setVehicle_minTurningRadius;
    int rowSkipsWidth = property_set_youSkipWidth;
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
bool CYouTurn::distanceFromYouTurnLine(CVehicle &vehicle, CNMEA &pn)
{
    int uTurnStyle = property_set_uTurnStyle;
    double maxSteerAngle = property_setVehicle_maxSteerAngle;
    double wheelbase = property_setVehicle_wheelbase;
    double maxAngularVelocity = property_setVehicle_maxAngularVelocity;

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
                completeYouTurn();
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
                completeYouTurn();
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
            double distancePiv = glm::distance(ytList[A], vehicle.pivotAxlePos);

            if (distancePiv > 1 || (B >= ptCount - 1))
            {
                {
                    completeYouTurn();
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
            double goalPointDistance = 0.8 * vehicle.updateGoalPointDistance(pn);

            isHeadingSameWay = true;
            bool reverseHeading = !vehicle.isReverse;

            int count = reverseHeading ? 1 : -1;
            Vec3 start(rEastYT, rNorthYT, 0);
            double distSoFar = 0;

            for (int i = reverseHeading ? B : A; i < ptCount && i >= 0; i += count)
            {
                // used for calculating the length squared of next segment.
                double tempDist = glm::distance(start, ytList[i]);

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
                    completeYouTurn();
                    return false;
                }

                if (pt3Phase == 1 && i < 2)
                {
                    completeYouTurn();
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
                    completeYouTurn();
                    return false;
                }

                isLastFrameForward = vehicle.isReverse;
            }

            //calc "D" the distance from pivot axle to lookahead point
            double goalPointDistanceSquared = glm::distanceSquared(goalPointYT.northing, goalPointYT.easting, pivot.northing, pivot.easting);

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
        completeYouTurn();
        return false;
    }
}

void CYouTurn::check3PtSequence(void)
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
        completeYouTurn();
        //mf.sim.stepDistance = 0;
        //mf.sim.isAccelForward = true;
    }
}

//Duh.... What does this do....
void CYouTurn::drawYouTurn(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
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
