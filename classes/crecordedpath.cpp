#include "crecordedpath.h"
#include "glutils.h"
#include "cvehicle.h"
#include "cdubins.h"
#include "cyouturn.h"
#include "aogproperty.h"
#include "btnenum.h"
#include "glm.h"

CRecordedPath::CRecordedPath(QObject *parent) : QObject(parent)
{

}

bool CRecordedPath::StartDrivingRecordedPath(CVehicle &vehicle)
{
    //create the dubins path based on start and goal to start of recorded path
    A = B = C = 0;

    if (recList.count() < 5) return false;

    //save a copy of where we started.
    homePos = vehicle.pivotAxlePos;

    // Try to find the nearest point of the recordet path in relation to the current position:
    double distance = glm::DOUBLE_MAX;
    int idx = 0;
    int i = 0;

    if (resumeState == 0) //start at the start
    {
        currentPositonIndex = 0;
        idx = 0;
        starPathIndx = 0;
    }
    else if (resumeState == 1) //resume from where stopped mid path
    {
        if (currentPositonIndex + 5 > recList.count())
        {
            currentPositonIndex = 0;
            idx = 0;
            starPathIndx = 0;
        }
        else
        {
            idx = starPathIndx = currentPositonIndex;
        }
    }
    else //find closest point
    {
        foreach  (CRecPathPt pt, recList)
        {
            double temp = ((pt.easting - homePos.easting) * (pt.easting - homePos.easting))
                          + ((pt.northing - homePos.northing) * (pt.northing - homePos.northing));

            if (temp < distance)
            {
                distance = temp;
                idx = i;
            }
            i++;
        }

        //scootch down the line a bit
        if (idx + 5 < recList.count()) idx += 5;
        else idx = recList.count() - 1;

        starPathIndx = currentPositonIndex = idx;
    }

    //the goal is the first point of path, the start is the current position
    Vec3 goal = Vec3(recList[idx].easting, recList[idx].northing, recList[idx].heading);

    //get the dubins for approach to recorded path
    GetDubinsPath(vehicle, goal);
    shuttleListCount = shuttleDubinsList.count();

    //has a valid dubins path been created?
    if (shuttleListCount == 0) return false;

    //starPathIndx = idxFieldSelected;

    //technically all good if we get here so set all the flags
    isFollowingDubinsHome = false;
    isFollowingRecPath = false;
    isFollowingDubinsToPath = true;
    isEndOfTheRecLine = false;
    //currentPositonIndex = 0;
    isDrivingRecordedPath = true;
    return true;
}

void CRecordedPath::UpdatePosition(CVehicle &vehicle, const CYouTurn &yt, bool isAutoSteerBtnOn)
{
    if (isFollowingDubinsToPath)
    {
        //set a speed of 10 kmh
        emit setSimStepDistance(shuttleDubinsList[C].speed / 50);
        //mf.sim.stepDistance = shuttleDubinsList[C].speed / 50;

        pivotAxlePosRP = vehicle.steerAxlePos;

        //StanleyDubinsPath(vehicle, pn, shuttleListCount);
        PurePursuitDubins(vehicle, yt, isAutoSteerBtnOn, shuttleListCount);

        //check if close to recorded path
        int cnt = shuttleDubinsList.size();
        pathCount = cnt - B;
        if (pathCount < 8)
        {
            double distSqr = glm::DistanceSquared(pivotAxlePosRP.northing, pivotAxlePosRP.easting, recList[0].northing, recList[0].easting);
            if (distSqr < 2)
            {
                isFollowingRecPath = true;
                isFollowingDubinsToPath = false;
                shuttleDubinsList.clear();
                shortestDubinsList.clear();
                C = starPathIndx;
                A = C + 3;
                B = A + 1;
            }
        }
    }

    if (isFollowingRecPath)
    {
        pivotAxlePosRP = vehicle.steerAxlePos;

        //StanleyRecPath(vehicle, pn, recListCount);
        PurePursuitRecPath(vehicle, recListCount);

        //if end of the line then stop
        if (!isEndOfTheRecLine)
        {
            emit setSimStepDistance(recList[C].speed / 34.86);
            //mf.sim.stepDistance = recList[C].speed / 17.86;
            north = recList[C].northing;

            pathCount = recList.size() - C;

            //section control - only if different click the button
            //torriem: turn off sectionMasterAuto if on
            emit turnOffSectionMasterAuto();
            //bool autoBtn = (mf.autoBtnState == btnStates::Auto);
            //trig = autoBtn;
            //if (autoBtn != recList[C].autoBtnState) emit btnSectionOffAutoOnClicked();
        }
        else
        {
            StopDrivingRecordedPath();
            return;
        }
    }

    if (isFollowingDubinsHome)
    {
        int cnt = shuttleDubinsList.size();
        pathCount = cnt - B;
        if (pathCount < 3)
        {
            StopDrivingRecordedPath();
            return;
        }

        emit setSimStepDistance(shuttleDubinsList[C].speed / 35);
        //mf.sim.stepDistance = shuttleDubinsList[C].speed / 35;
        pivotAxlePosRP = vehicle.steerAxlePos;

        PurePursuitDubins(vehicle, yt, isAutoSteerBtnOn, shuttleListCount);
    }
}

void CRecordedPath::StopDrivingRecordedPath()
{
    isFollowingDubinsHome = false;
    isFollowingRecPath = false;
    isFollowingDubinsToPath = false;
    shuttleDubinsList.clear();
    shortestDubinsList.clear();
    emit setSimStepDistance(0);
    //mf.sim.stepDistance = 0;
    isDrivingRecordedPath = false;

    /* slot in main form can make sure gui is right*/
    //mf.btnPathGoStop.Image = Properties.Resources.boundaryPlay;
    //mf.btnPathRecordStop.Enabled = true;
    //mf.btnPickPath.Enabled = true;
    //mf.btnResumePath.Enabled = true;
    emit stoppedDriving();
}

void CRecordedPath::GetDubinsPath(CVehicle &vehicle, Vec3 goal)
{
    double minTurningRadius = vehicle.minTurningRadius;

    CDubinsTurningRadius = minTurningRadius * 1.2;
    CDubins dubPath;

    // current psition
    pivotAxlePosRP = vehicle.pivotAxlePos;

    //bump it forward
    Vec3 pt2(pivotAxlePosRP.easting + (sin(pivotAxlePosRP.heading) * 3),
             pivotAxlePosRP.northing + (cos(pivotAxlePosRP.heading) * 3),
             pivotAxlePosRP.heading);

    //get the dubins path vec3 point coordinates of turn
    shortestDubinsList.clear();
    shuttleDubinsList.clear();

    shortestDubinsList = dubPath.GenerateDubins(pt2, goal);

    //if Dubins returns 0 elements, there is an unavoidable blockage in the way.
    if (shortestDubinsList.size() > 0)
    {
        shortestDubinsList.insert(0, vehicle.pivotAxlePos);

        //transfer point list to recPath class point style
        for (int i = 0; i < shortestDubinsList.size(); i++)
        {
            CRecPathPt pt(shortestDubinsList[i].easting, shortestDubinsList[i].northing, shortestDubinsList[i].heading, 9.0, false);
            shuttleDubinsList.append(pt);
        }
        return;
    }
}

void CRecordedPath::PurePursuitRecPath(CVehicle &vehicle, int ptCount)
{
    //find the closest 2 points to current fix
    double minDistA = 9999999999;
    double dist, dx, dz;

    //set the search range close to current position
    int top = currentPositonIndex + 5;
    if (top > ptCount) top = ptCount;

    for (int t = currentPositonIndex; t < top; t++)
    {
        dist = ((pivotAxlePosRP.easting - recList[t].easting) * (pivotAxlePosRP.easting - recList[t].easting))
                        + ((pivotAxlePosRP.northing - recList[t].northing) * (pivotAxlePosRP.northing - recList[t].northing));
        if (dist < minDistA)
        {
            minDistA = dist;
            A = t;
        }
    }

    //Save the closest point
    C = A;

    //next point is the next in list
    B = A + 1;
    if (B == ptCount)
    {
        //don't go past the end of the list - "end of the line" trigger
        A--;
        B--;
        isEndOfTheRecLine = true;
    }

    //save current position
    currentPositonIndex = A;

    //get the distance from currently active AB line
    dx = recList[B].easting - recList[A].easting;
    dz = recList[B].northing - recList[A].northing;

    if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dz) < glm::DOUBLE_EPSILON) return;

    //how far from current AB Line is fix
    distanceFromCurrentLinePivot =
        ((dz * pivotAxlePosRP.easting) - (dx * pivotAxlePosRP.northing) + (recList[B].easting
                * recList[A].northing) - (recList[B].northing * recList[A].easting))
                    / sqrt((dz * dz) + (dx * dx));

    //integral slider is set to 0
    if (vehicle.purePursuitIntegralGain != 0)
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

        if (isFollowingRecPath
            && fabs(pivotDerivative) < (0.1)
            && vehicle.avgSpeed > 2.5)
        //&& fabs(pivotDistanceError) < 0.2)

        {
            //if over the line heading wrong way, rapidly decrease integral
            if ((inty < 0 && distanceFromCurrentLinePivot < 0) || (inty > 0 && distanceFromCurrentLinePivot > 0))
            {
                inty += pivotDistanceError * vehicle.purePursuitIntegralGain * -0.04;
            }
            else
            {
                if (fabs(distanceFromCurrentLinePivot) > 0.02)
                {
                    inty += pivotDistanceError * vehicle.purePursuitIntegralGain * -0.02;
                    if (inty > 0.2) inty = 0.2;
                    else if (inty < -0.2) inty = -0.2;
                }
            }
        }
        else inty *= 0.95;
    }
    else inty = 0;

    if (vehicle.isReverse) inty = 0;

    // ** Pure pursuit ** - calc point on ABLine closest to current position
    double U = (((pivotAxlePosRP.easting - recList[A].easting) * dx)
                + ((pivotAxlePosRP.northing - recList[A].northing) * dz))
               / ((dx * dx) + (dz * dz));

    rEastRP = recList[A].easting + (U * dx);
    rNorthRP = recList[A].northing + (U * dz);

    //update base on autosteer settings and distance from line
    double goalPointDistance = vehicle.UpdateGoalPointDistance();

    bool ReverseHeading = !vehicle.isReverse;

    int count = ReverseHeading ? 1 : -1;
    CRecPathPt start(rEastRP, rNorthRP, 0, 0, false);
    double distSoFar = 0;

    for (int i = ReverseHeading ? B : A; i < ptCount && i >= 0; i += count)
    {
        // used for calculating the length squared of next segment.
        double tempDist = sqrt((start.easting - recList[i].easting) * (start.easting - recList[i].easting)
                                    + (start.northing - recList[i].northing) * (start.northing - recList[i].northing));

        //will we go too far?
        if ((tempDist + distSoFar) > goalPointDistance)
        {
            double j = (goalPointDistance - distSoFar) / tempDist; // the remainder to yet travel

            goalPointRP.easting = (((1 - j) * start.easting) + (j * recList[i].easting));
            goalPointRP.northing = (((1 - j) * start.northing) + (j * recList[i].northing));
            break;
        }
        else distSoFar += tempDist;
        start = recList[i];
    }

    //calc "D" the distance from pivotAxlePosRP axle to lookahead point
    double goalPointDistanceSquared = glm::DistanceSquared(goalPointRP.northing, goalPointRP.easting, pivotAxlePosRP.northing, pivotAxlePosRP.easting);

    //calculate the the delta x in local coordinates and steering angle degrees based on wheelbase
    double localHeading = glm::twoPI - vehicle.fixHeading + inty;

    ppRadiusRP = goalPointDistanceSquared / (2 * (((goalPointRP.easting - pivotAxlePosRP.easting) * cos(localHeading)) + ((goalPointRP.northing - pivotAxlePosRP.northing) * sin(localHeading))));

    steerAngleRP = glm::toDegrees(atan(2 * (((goalPointRP.easting - pivotAxlePosRP.easting) * cos(localHeading))
                                                + ((goalPointRP.northing - pivotAxlePosRP.northing) * sin(localHeading))) * vehicle.wheelbase / goalPointDistanceSquared));

    if (steerAngleRP < -vehicle.maxSteerAngle) steerAngleRP = -vehicle.maxSteerAngle;
    if (steerAngleRP > vehicle.maxSteerAngle) steerAngleRP = vehicle.maxSteerAngle;

    //used for smooth mode
    vehicle.modeActualXTE = (distanceFromCurrentLinePivot);

    //Convert to centimeters
    vehicle.guidanceLineDistanceOff = (short)glm::roundMidAwayFromZero(distanceFromCurrentLinePivot * 1000.0);
    vehicle.guidanceLineSteerAngle = (short)(steerAngleRP * 100);
}

void CRecordedPath::PurePursuitDubins(CVehicle &vehicle, const CYouTurn &yt, bool isAutoSteerBtnOn, int ptCount)
{
    double dist, dx, dz;
    double minDistA = 1000000, minDistB = 1000000;

    //find the closest 2 points to current fix
    for (int t = 0; t < ptCount; t++)
    {
        dist = ((pivotAxlePosRP.easting - shuttleDubinsList[t].easting) * (pivotAxlePosRP.easting - shuttleDubinsList[t].easting))
               + ((pivotAxlePosRP.northing - shuttleDubinsList[t].northing) * (pivotAxlePosRP.northing - shuttleDubinsList[t].northing));
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

    //currentLocationIndex = A;

    //get the distance from currently active AB line
    dx = shuttleDubinsList[B].easting - shuttleDubinsList[A].easting;
    dz = shuttleDubinsList[B].northing - shuttleDubinsList[A].northing;

    if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dz) < glm::DOUBLE_EPSILON) return;

    //abHeading = atan2(dz, dx);

    //how far from current AB Line is fix
    distanceFromCurrentLinePivot = ((dz * pivotAxlePosRP.easting) - (dx * pivotAxlePosRP.northing) + (shuttleDubinsList[B].easting
                                                                                                      * shuttleDubinsList[A].northing) - (shuttleDubinsList[B].northing * shuttleDubinsList[A].easting))
                                   / sqrt((dz * dz) + (dx * dx));

    //integral slider is set to 0
    if (vehicle.purePursuitIntegralGain != 0)
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
                inty += pivotDistanceError * vehicle.purePursuitIntegralGain * -0.04;
            }
            else
            {
                if (fabs(distanceFromCurrentLinePivot) > 0.02)
                {
                    inty += pivotDistanceError * vehicle.purePursuitIntegralGain * -0.02;
                    if (inty > 0.2) inty = 0.2;
                    else if (inty < -0.2) inty = -0.2;
                }
            }
        }
        else inty *= 0.95;
    }
    else inty = 0;

    if (vehicle.isReverse) inty = 0;

    // ** Pure pursuit ** - calc point on ABLine closest to current position
    double U = (((pivotAxlePosRP.easting - shuttleDubinsList[A].easting) * dx)
                + ((pivotAxlePosRP.northing - shuttleDubinsList[A].northing) * dz))
               / ((dx * dx) + (dz * dz));

    rEastRP = shuttleDubinsList[A].easting + (U * dx);
    rNorthRP = shuttleDubinsList[A].northing + (U * dz);

    //update base on autosteer settings and distance from line
    double goalPointDistance = vehicle.UpdateGoalPointDistance();

    bool ReverseHeading = !vehicle.isReverse;

    int count = ReverseHeading ? 1 : -1;
    CRecPathPt start(rEastRP, rNorthRP, 0, 0, false);
    double distSoFar = 0;

    for (int i = ReverseHeading ? B : A; i < ptCount && i >= 0; i += count)
    {
        // used for calculating the length squared of next segment.
        double tempDist = sqrt((start.easting - shuttleDubinsList[i].easting) * (start.easting - shuttleDubinsList[i].easting)
                                    + (start.northing - shuttleDubinsList[i].northing) * (start.northing - shuttleDubinsList[i].northing));

        //will we go too far?
        if ((tempDist + distSoFar) > goalPointDistance)
        {
            double j = (goalPointDistance - distSoFar) / tempDist; // the remainder to yet travel

            goalPointRP.easting = (((1 - j) * start.easting) + (j * shuttleDubinsList[i].easting));
            goalPointRP.northing = (((1 - j) * start.northing) + (j * shuttleDubinsList[i].northing));
            break;
        }
        else distSoFar += tempDist;
        start = shuttleDubinsList[i];
    }

    //calc "D" the distance from pivotAxlePosRP axle to lookahead point
    double goalPointDistanceSquared = glm::DistanceSquared(goalPointRP.northing, goalPointRP.easting, pivotAxlePosRP.northing, pivotAxlePosRP.easting);

    //calculate the the delta x in local coordinates and steering angle degrees based on wheelbase
    //double localHeading = glm::twoPI - vehicle.fixHeading;

    double localHeading = glm::twoPI - vehicle.fixHeading + inty;

    ppRadiusRP = goalPointDistanceSquared / (2 * (((goalPointRP.easting - pivotAxlePosRP.easting) * cos(localHeading)) + ((goalPointRP.northing - pivotAxlePosRP.northing) * sin(localHeading))));

    steerAngleRP = glm::toDegrees(atan(2 * (((goalPointRP.easting - pivotAxlePosRP.easting) * cos(localHeading))
                                   + ((goalPointRP.northing - pivotAxlePosRP.northing) * sin(localHeading))) * vehicle.wheelbase / goalPointDistanceSquared));

    if (steerAngleRP < -vehicle.maxSteerAngle) steerAngleRP = -vehicle.maxSteerAngle;
    if (steerAngleRP > vehicle.maxSteerAngle) steerAngleRP = vehicle.maxSteerAngle;

    if (ppRadiusRP < -500) ppRadiusRP = -500;
    if (ppRadiusRP > 500) ppRadiusRP = 500;

    radiusPointRP.easting = pivotAxlePosRP.easting + (ppRadiusRP * cos(localHeading));
    radiusPointRP.northing = pivotAxlePosRP.northing + (ppRadiusRP * sin(localHeading));

    //angular velocity in rads/sec  = 2PI * m/sec * radians/meters
    // double angVel = glm::twoPI * 0.277777 * vehicle.pn.speed * (tan(glm::toRadians(steerAngleRP))) / vehicle.vehicle.wheelbase;

    //clamp the steering angle to not exceed safe angular velocity
    //if (fabs(angVel) > mf.vehicle.maxAngularVelocity)
    //{
    //    steerAngleRP = glm::toDegrees(steerAngleRP > 0 ?
    //            (atan((mf.vehicle.wheelbase * mf.vehicle.maxAngularVelocity) / (glm::twoPI * mf.avgSpeed * 0.277777)))
    //        : (atan((mf.vehicle.wheelbase * -mf.vehicle.maxAngularVelocity) / (glm::twoPI * mf.avgSpeed * 0.277777))));
    //}

    //Convert to centimeters
    vehicle.guidanceLineDistanceOff = (short)glm::roundMidAwayFromZero(distanceFromCurrentLinePivot * 1000.0);
    vehicle.guidanceLineSteerAngle = (short)(steerAngleRP * 100);

}

void CRecordedPath::DrawRecordedLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    int ptCount = recList.size();
    if (ptCount < 1) return;

    GLHelperOneColor gldraw;
    QColor color;
    GLHelperColors gldraw1;

    color.fromRgbF(0.98f, 0.92f, 0.460f);
    for (int h = 0; h < ptCount; h++)
        gldraw.append(QVector3D(recList[h].easting, recList[h].northing, 0));
    gldraw.draw(gl,mvp,color,GL_LINE_STRIP,1.0f);

    if (!isRecordOn)
    {
        //Draw lookahead Point
        gldraw1.append( { QVector3D(recList[currentPositonIndex].easting, recList[currentPositonIndex].northing, 0),
                        QVector4D(1.0f, 0.5f, 0.95f, 1.0f) } );
        gldraw1.draw(gl,mvp,GL_POINTS,16.0f);
    }
   }

void CRecordedPath::DrawDubins(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    if (shuttleDubinsList.size() > 1)
    {
        GLHelperOneColor gldraw;
        for (int h = 0; h < shuttleDubinsList.size(); h++)
            gldraw.append(QVector3D(shuttleDubinsList[h].easting, shuttleDubinsList[h].northing, 0));

        gldraw.draw(gl, mvp, QColor::fromRgbF(0.298f, 0.96f, 0.2960f),
                    GL_POINTS, 2.0f);
    }
}
