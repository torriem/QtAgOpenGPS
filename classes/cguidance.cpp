#include "cguidance.h"
#include "cvehicle.h"
#include "cahrs.h"
#include "cyouturn.h"
#include "cabline.h"
#include "cabcurve.h"
#include "glm.h"
#include "aogproperty.h"

CGuidance::CGuidance() {}

void CGuidance::doSteerAngleCalc(bool isAutoSteerBtnOn,
                                 CVehicle &vehicle,
                                 const CAHRS &ahrs
                                 )
{
    double stanleyHeadingErrorGain = property_stanleyHeadingErrorGain;
    double stanleyDistanceErrorGain = property_stanleyDistanceErrorGain;
    double stanleyIntegralGainAB = property_stanleyIntegralGainAB;
    double sideHillCompFactor = property_setAS_sideHillComp;
    double maxSteerAngle = property_setVehicle_maxSteerAngle;

    if (vehicle.isReverse) steerHeadingError *= -1;
    //Overshoot setting on Stanley tab
    steerHeadingError *= stanleyHeadingErrorGain;

    double sped = fabs(vehicle.avgSpeed);
    if (sped > 1) sped = 1 + 0.277 * (sped - 1);
    else sped = 1;
    double XTEc = atan((distanceFromCurrentLineSteer * stanleyDistanceErrorGain)
                            / (sped));

    xTrackSteerCorrection = (xTrackSteerCorrection * 0.5) + XTEc * (0.5);

    //derivative of steer distance error
    distSteerError = (distSteerError * 0.95) + ((xTrackSteerCorrection * 60) * 0.05);
    if (counter++ > 5)
    {
        derivativeDistError = distSteerError - lastDistSteerError;
        lastDistSteerError = distSteerError;
        counter = 0;
    }

    steerAngleGu = glm::toDegrees((xTrackSteerCorrection + steerHeadingError) * -1.0);

    if (fabs(distanceFromCurrentLineSteer) > 0.5) steerAngleGu *= 0.5;
    else steerAngleGu *= (1 - fabs(distanceFromCurrentLineSteer));

    //pivot PID
    pivotDistanceError = (pivotDistanceError * 0.6) + (distanceFromCurrentLinePivot * 0.4);
    //pivotDistanceError = atan((distanceFromCurrentLinePivot) / (sped)) * 0.2;
    //pivotErrorTotal = pivotDistanceError + pivotDerivative;

    if (vehicle.avgSpeed > 1
        && isAutoSteerBtnOn
        && fabs(derivativeDistError) < 1
        && fabs(pivotDistanceError) < 0.25)
    {
        //if over the line heading wrong way, rapidly decrease integral
        if ((inty < 0 && distanceFromCurrentLinePivot < 0) || (inty > 0 && distanceFromCurrentLinePivot > 0))
        {
            inty += pivotDistanceError * stanleyIntegralGainAB * -0.03;
        }
        else
        {
            inty += pivotDistanceError * stanleyIntegralGainAB * -0.01;
        }

        //integral slider is set to 0
        if (stanleyIntegralGainAB == 0) inty = 0;
    }
    else inty *= 0.7;

    if (vehicle.isReverse) inty = 0;

    if (ahrs.imuRoll != 88888)
        steerAngleGu += ahrs.imuRoll * -sideHillCompFactor;

    if (steerAngleGu < -maxSteerAngle) steerAngleGu = -maxSteerAngle;
    else if (steerAngleGu > maxSteerAngle) steerAngleGu = maxSteerAngle;

    //used for smooth mode
    vehicle.modeActualXTE = (distanceFromCurrentLinePivot);

    //Convert to millimeters from meters
    vehicle.guidanceLineDistanceOff = (short)glm::roundMidAwayFromZero(distanceFromCurrentLinePivot * 1000.0);
    vehicle.guidanceLineSteerAngle = (short)(steerAngleGu * 100);
}

/// <summary>
/// Function to calculate steer angle for AB Line Segment only
/// No curvature calc on straight line
/// </summary>
/// <param name="curPtA"></param>
/// <param name="curPtB"></param>
/// <param name="pivot"></param>
/// <param name="steer"></param>
/// <param name="isValid"></param>
void CGuidance::StanleyGuidanceABLine(Vec3 curPtA, Vec3 curPtB,
                                      Vec3 pivot, Vec3 steer,
                                      bool isAutoSteerBtnOn,
                                      CVehicle &vehicle,
                                      CABLine &ABLine,
                                      const CAHRS &ahrs,
                                      CYouTurn &yt)
{
    /*
    double stanleyHeadingErrorGain = property_stanleyHeadingErrorGain;
    double stanleyDistanceErrorGain = property_stanleyDistanceErrorGain;
    double stanleyIntegralGainAB = property_stanleyIntegralGainAB;
    double sideHillCompFactor = property_setAS_sideHillComp;
    double maxSteerAngle = property_setVehicle_maxSteerAngle;
    */

    //get the pivot distance from currently active AB segment   ///////////  Pivot  ////////////
    double dx = curPtB.easting - curPtA.easting;
    double dy = curPtB.northing - curPtA.northing;
    if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dy) < glm::DOUBLE_EPSILON) return;

    //save a copy of dx,dy in youTurn
    yt.dxAB = dx; yt.dyAB = dy;

    //how far from current AB Line is fix
    distanceFromCurrentLinePivot = ((dy * pivot.easting) - (dx * pivot.northing) + (curPtB.easting
                                                                                    * curPtA.northing) - (curPtB.northing * curPtA.easting))
                                   / sqrt((dy * dy) + (dx * dx));

    if (!ABLine.isHeadingSameWay)
        distanceFromCurrentLinePivot *= -1.0;

    ABLine.distanceFromCurrentLinePivot = distanceFromCurrentLinePivot;
    double U = (((pivot.easting - curPtA.easting) * dx)
                + ((pivot.northing - curPtA.northing) * dy))
               / ((dx * dx) + (dy * dy));

    rEastPivot = curPtA.easting + (U * dx);
    rNorthPivot = curPtA.northing + (U * dy);

    ABLine.rEastAB = rEastPivot;
    ABLine.rNorthAB = rNorthPivot;

    //get the distance from currently active AB segment of steer axle //////// steer /////////////
    Vec3 steerA = Vec3(curPtA);
    Vec3 steerB = Vec3(curPtB);

    //create the AB segment to offset
    steerA.easting += (sin(steerA.heading + glm::PIBy2) * (inty));
    steerA.northing += (cos(steerA.heading + glm::PIBy2) * (inty));

    steerB.easting += (sin(steerB.heading + glm::PIBy2) * (inty));
    steerB.northing += (cos(steerB.heading + glm::PIBy2) * (inty));

    dx = steerB.easting - steerA.easting;
    dy = steerB.northing - steerA.northing;

    if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dy) < glm::DOUBLE_EPSILON) return;

    //how far from current AB Line is fix
    distanceFromCurrentLineSteer = ((dy * steer.easting) - (dx * steer.northing) + (steerB.easting
                                                                                    * steerA.northing) - (steerB.northing * steerA.easting))
                                   / sqrt((dy * dy) + (dx * dx));

    if (!ABLine.isHeadingSameWay)
        distanceFromCurrentLineSteer *= -1.0;

    // calc point on ABLine closest to current position - for display only
    U = (((steer.easting - steerA.easting) * dx)
         + ((steer.northing - steerA.northing) * dy))
        / ((dx * dx) + (dy * dy));

    rEastSteer = steerA.easting + (U * dx);
    rNorthSteer = steerA.northing + (U * dy);

    double steerErr = atan2(rEastSteer - rEastPivot, rNorthSteer - rNorthPivot);
    steerHeadingError = (steer.heading - steerErr);
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

    doSteerAngleCalc(isAutoSteerBtnOn, vehicle,ahrs);
}

/// <summary>
/// Find the steer angle for a curve list, curvature and integral
/// </summary>
/// <param name="pivot">Pivot position vector</param>
/// <param name="steer">Steer position vector</param>
/// <param name="curList">the current list of guidance points</param>
void CGuidance::StanleyGuidanceCurve(Vec3 pivot, Vec3 steer,
                                     QVector<Vec3> &curList,
                                     bool isAutoSteerBtnOn,
                                     CVehicle &vehicle,
                                     CABCurve &curve,
                                     const CAHRS &ahrs)
{            //calculate required steer angle
    /*
    double stanleyHeadingErrorGain = property_stanleyHeadingErrorGain;
    double standleyDistanceErrorGain = property_stanleyDistanceErrorGain;
    double stanleyIntegralGainAB = property_stanleyIntegralGainAB;
    double sideHillCompFactor = property_setAS_sideHillComp;
    double maxSteerAngle = property_setVehicle_maxSteerAngle;
    */

    //find the closest point roughly
    int cc = 0, dd;
    int ptCount = curList.count();
    if (ptCount > 5)
    {
        double minDistA = 1000000, minDistB;

        for (int j = 0; j < ptCount; j += 10)
        {
            double dist = ((steer.easting - curList[j].easting) * (steer.easting - curList[j].easting))
                          + ((steer.northing - curList[j].northing) * (steer.northing - curList[j].northing));
            if (dist < minDistA)
            {
                minDistA = dist;
                cc = j;
            }
        }

        minDistA = minDistB = 1000000;
        dd = cc + 7; if (dd > ptCount - 1) dd = ptCount;
        cc -= 7; if (cc < 0) cc = 0;

        //find the closest 2 points to current close call
        for (int j = cc; j < dd; j++)
        {
            double dist = ((steer.easting - curList[j].easting) * (steer.easting - curList[j].easting))
                          + ((steer.northing - curList[j].northing) * (steer.northing - curList[j].northing));
            if (dist < minDistA)
            {
                minDistB = minDistA;
                sB = sA;
                minDistA = dist;
                sA = j;
            }
            else if (dist < minDistB)
            {
                minDistB = dist;
                sB = j;
            }
        }

        ////too far from guidance line? Lost? Fresh delete of ref?
        //if (minDistA < (1.5 * (mf.tool.toolWidth * mf.tool.toolWidth)))
        //{
        //    if (minDistA == 100000000)
        //        return;
        //}
        //else
        //{
        //    curList.Clear();
        //    return;
        //}

        //just need to make sure the points continue ascending or heading switches all over the place
        if (sA > sB) { C = sA; sA = sB; sB = C; }

        //currentLocationIndex = sA;
        if (sA > ptCount - 1 || sB > ptCount - 1) return;

        minDistA = minDistB = 1000000;

        if (curve.isHeadingSameWay)
        {
            dd = sB; cc = dd - 12; if (cc < 0) cc = 0;
        }
        else
        {
            cc = sA; dd = sA + 12; if (dd >= ptCount) dd = ptCount - 1;
        }

        //find the closest 2 points of pivot back from steer
        for (int j = cc; j < dd; j++)
        {
            double dist = ((pivot.easting - curList[j].easting) * (pivot.easting - curList[j].easting))
                          + ((pivot.northing - curList[j].northing) * (pivot.northing - curList[j].northing));
            if (dist < minDistA)
            {
                minDistB = minDistA;
                pB = pA;
                minDistA = dist;
                pA = j;
            }
            else if (dist < minDistB)
            {
                minDistB = dist;
                pB = j;
            }
        }

        //just need to make sure the points continue ascending or heading switches all over the place
        if (pA > pB) { C = pA; pA = pB; pB = C; }

        if (pA > ptCount - 1 || pB > ptCount - 1)
        {
            pA = ptCount - 2;
            pB = ptCount - 1;
        }

        Vec3 pivA = Vec3(curList[pA]);
        Vec3 pivB = Vec3(curList[pB]);

        if (!curve.isHeadingSameWay)
        {
            pivA = curList[pB];
            pivB = curList[pA];

            pivA.heading += M_PI;
            if (pivA.heading > glm::twoPI) pivA.heading -= glm::twoPI;
        }

        curve.manualUturnHeading = pivA.heading;

        //get the pivot distance from currently active AB segment   ///////////  Pivot  ////////////
        double dx = pivB.easting - pivA.easting;
        double dz = pivB.northing - pivA.northing;

        if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dz) < glm::DOUBLE_EPSILON) return;

        //how far from current AB Line is fix
        distanceFromCurrentLinePivot = ((dz * pivot.easting) - (dx * pivot.northing) + (pivB.easting
                                                                                        * pivA.northing) - (pivB.northing * pivA.easting))
                                       / sqrt((dz * dz) + (dx * dx));

        curve.distanceFromCurrentLinePivot = distanceFromCurrentLinePivot;
        double U = (((pivot.easting - pivA.easting) * dx)
                    + ((pivot.northing - pivA.northing) * dz))
                   / ((dx * dx) + (dz * dz));

        rEastPivot = pivA.easting + (U * dx);
        rNorthPivot = pivA.northing + (U * dz);

        curve.rEastCu = rEastPivot;
        curve.rNorthCu = rNorthPivot;

        curve.currentLocationIndex = pA;

        //get the distance from currently active AB segment of steer axle //////// steer /////////////
        Vec3 steerA = Vec3(curList[sA]);
        Vec3 steerB = Vec3(curList[sB]);

        if (!curve.isHeadingSameWay)
        {
            steerA = curList[sB];
            steerA.heading += M_PI;
            if (steerA.heading > glm::twoPI) steerA.heading -= glm::twoPI;

            steerB = curList[sA];
            steerB.heading += M_PI;
            if (steerB.heading > glm::twoPI) steerB.heading -= glm::twoPI;
        }

        //double curvature = pivA.heading - steerA.heading;
        //if (curvature > M_PI) curvature -= M_PI; else if (curvature < M_PI) curvature += M_PI;
        //if (curvature > glm::PIBy2) curvature -= M_PI; else if (curvature < -glm::PIBy2) curvature += M_PI;

        ////because of draft
        //curvature = sin(curvature) * mf.vehicle.wheelbase * 0.8;
        //pivotCurvatureOffset = (pivotCurvatureOffset * 0.7) + (curvature * 0.3);
        //pivotCurvatureOffset = 0;

        //create the AB segment to offset
        steerA.easting += (sin(steerA.heading + glm::PIBy2) * (inty));
        steerA.northing += (cos(steerA.heading + glm::PIBy2) * (inty));

        steerB.easting += (sin(steerB.heading + glm::PIBy2) * (inty));
        steerB.northing += (cos(steerB.heading + glm::PIBy2) * (inty));

        dx = steerB.easting - steerA.easting;
        dz = steerB.northing - steerA.northing;

        if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dz) < glm::DOUBLE_EPSILON) return;

        //how far from current AB Line is fix
        distanceFromCurrentLineSteer = ((dz * steer.easting) - (dx * steer.northing) + (steerB.easting
                                                                                        * steerA.northing) - (steerB.northing * steerA.easting))
                                       / sqrt((dz * dz) + (dx * dx));

        // calc point on ABLine closest to current position - for display only
        U = (((steer.easting - steerA.easting) * dx)
             + ((steer.northing - steerA.northing) * dz))
            / ((dx * dx) + (dz * dz));

        rEastSteer = steerA.easting + (U * dx);
        rNorthSteer = steerA.northing + (U * dz);

        //double segHeading = atan2(rEastSteer - rEastPivot, rNorthSteer - rNorthPivot);

        //steerHeadingError = M_PI - fabs(fabs(pivot.heading - segHeading) - M_PI);
        steerHeadingError = steer.heading - steerB.heading;

        //Fix the circular error
        if (steerHeadingError > M_PI) steerHeadingError -= M_PI;
        else if (steerHeadingError < M_PI) steerHeadingError += M_PI;

        if (steerHeadingError > glm::PIBy2) steerHeadingError -= M_PI;
        else if (steerHeadingError < -glm::PIBy2) steerHeadingError += M_PI;

        doSteerAngleCalc(isAutoSteerBtnOn, vehicle,ahrs);
    }
    else
    {
        //invalid distance so tell AS module
        distanceFromCurrentLineSteer = 32000;
        vehicle.guidanceLineDistanceOff = 32000;
    }
}

