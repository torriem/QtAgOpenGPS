#include "crecordedpath.h"
#include "glutils.h"
#include "cvehicle.h"
#include "cgeofence.h"
#include "cdubins.h"
#include "cmazegrid.h"
#include "cnmea.h"
#include "aogsettings.h"
#include "btnenum.h"
#include "glm.h"

CRecordedPath::CRecordedPath(QObject *parent) : QObject(parent)
{

}

bool CRecordedPath::StartDrivingRecordedPath(const CVehicle &vehicle,
                                             const CBoundary &bnd,
                                             CGeoFence &gf,
                                             CMazeGrid &mazeGrid,
                                             double minFieldX, double minFieldY)
{
    //create the dubins path based on start and goal to start of recorded path
    A = B = C = 0;
    recListCount = recList.size();
    if (recListCount < 5) return false;

    //the goal is the first point of path, the start is the current position
    Vec3 goal(recList[0].easting, recList[0].northing, recList[0].heading);

    //save a copy of where we started.
    homePos = vehicle.pivotAxlePos;

    //get the dubins for approach to recorded path
    GetDubinsPath(vehicle, bnd, gf, mazeGrid, minFieldX, minFieldY, goal);
    shuttleListCount = shuttleDubinsList.size();

    //has a valid dubins path been created?
    if (shuttleListCount == 0) return false;

    //technically all good if we get here so set all the flags
    isFollowingDubinsHome = false;
    isFollowingRecPath = false;
    isFollowingDubinsToPath = true;
    isEndOfTheRecLine = false;
    currentPositonIndex = 0;
    isDrivingRecordedPath = true;

    isPausedDrivingRecordedPath = false;
    return true;
}

void CRecordedPath::UpdatePosition(CVehicle &vehicle,
                                   const CBoundary &bnd,
                                   const CNMEA &pn,
                                   CGeoFence &gf,
                                   CMazeGrid &mazeGrid,
                                   double minFieldX,
                                   double minFieldY)
{
    if (isFollowingDubinsToPath)
    {
        //set a speed of 10 kmh
        emit setSimStepDistance(shuttleDubinsList[C].speed / 17.86);
        //mf.sim.stepDistance = shuttleDubinsList[C].speed / 17.86;

        steerAxlePosRP = vehicle.steerAxlePos;

        StanleyDubinsPath(vehicle, pn, shuttleListCount);

        //check if close to recorded path
        int cnt = shuttleDubinsList.size();
        pathCount = cnt - B;
        if (pathCount < 8)
        {
            double distSqr = glm::distanceSquared(steerAxlePosRP.northing, steerAxlePosRP.easting, recList[0].northing, recList[0].easting);
            if (distSqr < 2)
            {
                isFollowingRecPath = true;
                isFollowingDubinsToPath = false;
                shuttleDubinsList.clear();
                shortestDubinsList.clear();
            }
        }
    }

    if (isFollowingRecPath)
    {
        steerAxlePosRP = vehicle.steerAxlePos;

        StanleyRecPath(vehicle, pn, recListCount);

        //if end of the line then stop
        if (!isEndOfTheRecLine)
        {
            emit setSimStepDistance(recList[C].speed / 17.86);
            //mf.sim.stepDistance = recList[C].speed / 17.86;
            north = recList[C].northing;

            pathCount = recList.size() - C;

            //section control - only if different click the button
            emit btnSectionSet(btnStates::Auto);
            //bool autoBtn = (mf.autoBtnState == btnStates::Auto);
            //trig = autoBtn;
            //if (autoBtn != recList[C].autoBtnState) emit btnSectionOffAutoOnClicked();
        }
        else
        {
            //create the dubins path based on start and goal to start trip home
            GetDubinsPath(vehicle,bnd, gf,mazeGrid, minFieldX, minFieldY, homePos);
            shuttleListCount = shuttleDubinsList.size();

            //its too small
            if (shuttleListCount < 3)
            {
                StopDrivingRecordedPath();
                return;
            }

            //set all the flags
            isFollowingDubinsHome = true;
            A = B = C = 0;
            isFollowingRecPath = false;
            isFollowingDubinsToPath = false;
            isEndOfTheRecLine = false;
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

        emit setSimStepDistance(shuttleDubinsList[C].speed / 17.86);
        //mf.sim.stepDistance = shuttleDubinsList[C].speed / 17.86;
        steerAxlePosRP = vehicle.steerAxlePos;

        StanleyDubinsPath(vehicle, pn, shuttleListCount);
    }

    //if paused, set the sim to 0
    if (isPausedDrivingRecordedPath)
        emit setSimStepDistance(0);
        //mf.sim.stepDistance = 0 / 17.86;

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

    emit stoppedDriving();
    //mf.goPathMenu.Image = Properties.Resources.AutoGo;
    isPausedDrivingRecordedPath = false;
}

void CRecordedPath::GetDubinsPath(const CVehicle &vehicle, const CBoundary &bnd, CGeoFence &gf, CMazeGrid &mazeGrid, double minFieldX, double minFieldY, Vec3 goal)
{
    CDubinsTurningRadius = vehicle.minTurningRadius * 1.0;
    CDubins dubPath;

    // current psition
    steerAxlePosRP = vehicle.pivotAxlePos;

    //bump it forward
    Vec3 pt2(steerAxlePosRP.easting + (sin(steerAxlePosRP.heading) * 3),
             steerAxlePosRP.northing + (cos(steerAxlePosRP.heading) * 3),
             steerAxlePosRP.heading);

    //get the dubins path vec3 point coordinates of turn
    shortestDubinsList.clear();
    shuttleDubinsList.clear();

    shortestDubinsList = dubPath.GenerateDubins(pt2, goal, bnd, gf);

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

    //find a path from start to goal - diagnostic, but also used later
    mazeList = mazeGrid.searchForPath(minFieldX, minFieldY, pt2, goal);

    //you can't get anywhere!
    if (mazeList.size() == 0) return;

    //start is navigateable - maybe
    int cnt = mazeList.size();

    if (cnt > 0)
    {
        {
            int turnRadius = (int)(3 * vehicle.minTurningRadius);
            if (cnt > 2 * turnRadius)
            {
                mazeList.remove(0, turnRadius);
                cnt = mazeList.size();
                mazeList.remove(cnt - turnRadius, turnRadius);
            }
        }

        shortestDubinsList = dubPath.GenerateDubins(pt2, mazeList[0], bnd, gf);
        if (shortestDubinsList.size() > 0)
        {
            for (int i = 0; i < shortestDubinsList.size(); i++)
            {
                CRecPathPt pt(shortestDubinsList[i].easting, shortestDubinsList[i].northing, shortestDubinsList[i].heading, 10.0, false);
                shuttleDubinsList.append(pt);
            }
        }
        else
        {
            return; //unable to generate a dubins to the start
        }

        for (int i = 0; i < mazeList.size(); i++)
        {
            CRecPathPt pt(mazeList[i].easting, mazeList[i].northing, mazeList[i].heading, 15.0, false);
            shuttleDubinsList.append(pt);
        }

        shortestDubinsList = dubPath.GenerateDubins(mazeList[mazeList.size() - 1], goal, bnd, gf);

        for (int i = 0; i < shortestDubinsList.size(); i++)
        {
            CRecPathPt pt(shortestDubinsList[i].easting, shortestDubinsList[i].northing, shortestDubinsList[i].heading, 11.0, false);
            shuttleDubinsList.append(pt);
        }
        return;
    }
}

void CRecordedPath::StanleyRecPath(CVehicle &vehicle, const CNMEA &pn, int ptCount)
{
    //find the closest 2 points to current fix
    double minDistA = 9999999999;

    //set the search range close to current position
    int top = currentPositonIndex + 5;
    if (top > ptCount) top = ptCount;

    double dist;
    for (int t = currentPositonIndex; t < top; t++)
    {
        dist = ((steerAxlePosRP.easting - recList[t].easting) * (steerAxlePosRP.easting - recList[t].easting))
                        + ((steerAxlePosRP.northing - recList[t].northing) * (steerAxlePosRP.northing - recList[t].northing));
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
    double dx = recList[B].easting - recList[A].easting;
    double dz = recList[B].northing - recList[A].northing;

    if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dz) < glm::DOUBLE_EPSILON) return;

    abHeading = atan2(dx, dz);
    //abHeading = recList[A].heading;

    //how far from current AB Line is fix
    distanceFromCurrentLine =
        ((dz * steerAxlePosRP.easting) - (dx * steerAxlePosRP.northing) + (recList[B].easting
                * recList[A].northing) - (recList[B].northing * recList[A].easting))
                    / sqrt((dz * dz) + (dx * dx));

    //are we on the right side or not
    isOnRightSideCurrentLine = distanceFromCurrentLine > 0;

    // calc point on ABLine closest to current position
    double U = (((steerAxlePosRP.easting - recList[A].easting) * dx)
                + ((steerAxlePosRP.northing - recList[A].northing) * dz))
                / ((dx * dx) + (dz * dz));

    rEastRP = recList[A].easting + (U * dx);
    rNorthRP = recList[A].northing + (U * dz);

    //the first part of stanley is to extract heading error
    double abFixHeadingDelta = (steerAxlePosRP.heading - abHeading);

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
    steerAngleRP = atan((distanceFromCurrentLine * vehicle.stanleyGain) / ((pn.speed * 0.277777) + 1));

    //clamp it to max 42 degrees
    if (steerAngleRP > 0.74) steerAngleRP = 0.74;
    if (steerAngleRP < -0.74) steerAngleRP = -0.74;

    //add them up and clamp to max in vehicle settings
    steerAngleRP = glm::toDegrees((steerAngleRP + abFixHeadingDelta) * -1.0);
    if (steerAngleRP < -vehicle.maxSteerAngle) steerAngleRP = -vehicle.maxSteerAngle;
    if (steerAngleRP > vehicle.maxSteerAngle) steerAngleRP = vehicle.maxSteerAngle;

    //Convert to millimeters and round properly to above/below .5
    distanceFromCurrentLine = glm::roundAwayFromZero(distanceFromCurrentLine * 1000.0);

    //every guidance method dumps into these that are used and sent everywhere, last one wins
    vehicle.guidanceLineDistanceOff = vehicle.distanceDisplay = (int)distanceFromCurrentLine;
    vehicle.guidanceLineSteerAngle = (int)(steerAngleRP * 100);
}

void CRecordedPath::StanleyDubinsPath(CVehicle &vehicle, const CNMEA &pn, int ptCount)
{
    //distanceFromCurrentLine = 9999;
    //find the closest 2 points to current fix
    double minDistA = 9999999999;
    for (int t = 0; t < ptCount; t++)
    {
        double dist = ((steerAxlePosRP.easting - shuttleDubinsList[t].easting) * (steerAxlePosRP.easting - shuttleDubinsList[t].easting))
                        + ((steerAxlePosRP.northing - shuttleDubinsList[t].northing) * (steerAxlePosRP.northing - shuttleDubinsList[t].northing));
        if (dist < minDistA)
        {
            minDistA = dist;
            A = t;
        }
    }

    //save the closest point
    C = A;
    //next point is the next in list
    B = A + 1;
    if (B == ptCount) { A--; B--; }                //don't go past the end of the list - "end of the line" trigger

    //get the distance from currently active AB line
    //x2-x1
    double dx = shuttleDubinsList[B].easting - shuttleDubinsList[A].easting;
    //z2-z1
    double dz = shuttleDubinsList[B].northing - shuttleDubinsList[A].northing;

    if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dz) < glm::DOUBLE_EPSILON) return;

    //abHeading = atan2(dz, dx);
    abHeading = shuttleDubinsList[A].heading;

    //how far from current AB Line is fix
    distanceFromCurrentLine = ((dz * steerAxlePosRP.easting) - (dx * steerAxlePosRP
        .northing) + (shuttleDubinsList[B].easting
                * shuttleDubinsList[A].northing) - (shuttleDubinsList[B].northing * shuttleDubinsList[A].easting))
                    / sqrt((dz * dz) + (dx * dx));

    //are we on the right side or not
    isOnRightSideCurrentLine = distanceFromCurrentLine > 0;

    // calc point on ABLine closest to current position
    double U = (((steerAxlePosRP.easting - shuttleDubinsList[A].easting) * dx)
                + ((steerAxlePosRP.northing - shuttleDubinsList[A].northing) * dz))
                / ((dx * dx) + (dz * dz));

    rEastRP = shuttleDubinsList[A].easting + (U * dx);
    rNorthRP = shuttleDubinsList[A].northing + (U * dz);

    //the first part of stanley is to extract heading error
    double abFixHeadingDelta = (steerAxlePosRP.heading - abHeading);

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
    steerAngleRP = atan((distanceFromCurrentLine * vehicle.stanleyGain) / ((pn.speed * 0.277777) + 1));

    //clamp it to max 42 degrees
    if (steerAngleRP > 0.74) steerAngleRP = 0.74;
    if (steerAngleRP < -0.74) steerAngleRP = -0.74;

    //add them up and clamp to max in vehicle settings
    steerAngleRP = glm::toDegrees((steerAngleRP + abFixHeadingDelta) * -1.0);
    if (steerAngleRP < -vehicle.maxSteerAngle) steerAngleRP = -vehicle.maxSteerAngle;
    if (steerAngleRP > vehicle.maxSteerAngle) steerAngleRP = vehicle.maxSteerAngle;

    //Convert to millimeters and round properly to above/below .5
    distanceFromCurrentLine = glm::roundAwayFromZero(distanceFromCurrentLine * 1000.0);

    //every guidance method dumps into these that are used and sent everywhere, last one wins
    vehicle.guidanceLineDistanceOff = vehicle.distanceDisplay = (int)distanceFromCurrentLine;
    vehicle.guidanceLineSteerAngle = (int)(steerAngleRP * 100);
}

void CRecordedPath::drawRecordedLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    USE_SETTINGS;

    int ptCount = recList.size();
    if (ptCount < 1) return;

    GLHelperColors gldraw;
    ColorVertex cv;

    gl->glLineWidth(1);

    cv.color = QVector4D(0.98f, 0.92f, 0.460f, 1.0f);
    for (int h = 0; h < ptCount; h++) {
        cv.vertex = QVector3D(recList[h].easting, recList[h].northing, 0);
        gldraw.append(cv);
    }
    gldraw.draw(gl, mvp, GL_LINE_STRIP, 1.0f);

    if (SETTINGS_DISPLAY_ISPUREON)
    {
        gldraw.clear();
        //Draw lookahead Point
        //GL.Color(1.0f, 1.0f, 0.25f);
        //GL.Vertex(rEast, rNorth, 0.0);
        cv.color = QVector4D(1.0f, 0.5f, 0.95f, 1.0f);
        cv.vertex = QVector3D(rEastRP, rNorthRP, 0.0);
        gldraw.append(cv);

        gldraw.draw(gl, mvp, GL_POINTS, 8.0f);
    }
}

void CRecordedPath::drawDubins(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
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
