#include "cdubins.h"
#include "cgeofence.h"
#include "glm.h"

static const double driveDistance = 0.1;
double CDubinsTurningRadius = 1;

//Takes care of all standardized methods related the generating of Dubins paths
//Calculate center positions of the Right circle
Vec2 GetRightCircleCenterPos(Vec2 circlePos, double heading)
{
   Vec2 rightCirclePos = Vec2(0, 0);

   //The circle is 90 degrees (pi/2 radians) to the right of the car's heading
   rightCirclePos.easting = circlePos.easting + (CDubinsTurningRadius * qSin(heading + glm::PIBy2));
   rightCirclePos.northing = circlePos.northing + (CDubinsTurningRadius * qCos(heading + glm::PIBy2));
   return rightCirclePos;
}

//Calculate center positions of the Left circle
Vec2 GetLeftCircleCenterPos(Vec2 circlePos, double heading)
{
    Vec2 rightCirclePos = Vec2(0, 0);

    //The circle is 90 degrees (pi/2 radians) to the left of the car's heading
    rightCirclePos.easting = circlePos.easting + (CDubinsTurningRadius * qSin(heading - glm::PIBy2));
    rightCirclePos.northing = circlePos.northing + (CDubinsTurningRadius * qCos(heading - glm::PIBy2));
    return rightCirclePos;
}


//Outer tangent (LSL and RSR)
void LSLorRSR(Vec2 startCircle, Vec2 goalCircle, bool isBottom, Vec2 &startTangent, Vec2 &goalTangent)
{
    //The angle to the first tangent coordinate is always 90 degrees if the both circles have the same radius
    double theta = glm::PIBy2;

    //Need to modify theta if the circles are not on the same height (z)
    theta += qAtan2(goalCircle.northing - startCircle.northing, goalCircle.easting - startCircle.easting);

    //Add pi to get the "bottom" coordinate which is on the opposite side (180 degrees = pi)
    if (isBottom) theta += M_PI;

    //The coordinates of the first tangent points
    double xT1 = startCircle.easting + (CDubinsTurningRadius * qCos(theta));
    double zT1 = startCircle.northing + (CDubinsTurningRadius * qSin(theta));

    //To get the second coordinate we need a direction
    //This direction is the same as the direction between the center pos of the circles
    Vec2 dirVec = goalCircle - startCircle;

    double xT2 = xT1 + dirVec.easting;
    double zT2 = zT1 + dirVec.northing;

    //The final coordinates of the tangent lines
     //return ?
    startTangent = Vec2(xT1, zT1);
    goalTangent = Vec2(xT2, zT2);
}

//Inner tangent (RSL and LSR)
void RSLorLSR(Vec2 startCircle, Vec2 goalCircle, bool isBottom, Vec2 &startTangent, Vec2 &goalTangent)
{
    //Find the distance between the circles
    double D = (startCircle - goalCircle).getLength();

    //If the circles have the same radius we can use cosine and not the law of cosines
    //to calculate the angle to the first tangent coordinate
    double theta = qAcos((2 * CDubinsTurningRadius) / D);

    //If the circles is LSR, then the first tangent pos is on the other side of the center line
    if (isBottom) theta *= -1.0;

    //Need to modify theta if the circles are not on the same height
    theta += qAtan2(goalCircle.northing - startCircle.northing, goalCircle.easting - startCircle.easting);

    //The coordinates of the first tangent point
    double xT1 = startCircle.easting + (CDubinsTurningRadius * qCos(theta));
    double zT1 = startCircle.northing + (CDubinsTurningRadius * qSin(theta));

    //To get the second tangent coordinate we need the direction of the tangent
    //To get the direction we move up 2 circle radius and end up at this coordinate
    double xT1_tmp = startCircle.easting + (2.0 * CDubinsTurningRadius * qCos(theta));
    double zT1_tmp = startCircle.northing + (2.0 * CDubinsTurningRadius * qSin(theta));

    //The direction is between the new coordinate and the center of the target circle
    Vec2 dirVec = goalCircle - Vec2(xT1_tmp, zT1_tmp);

    //The coordinates of the second tangent point is the
    double xT2 = xT1 + dirVec.easting;
    double zT2 = zT1 + dirVec.northing;

    //The final coordinates of the tangent lines
    // ? return

    startTangent = Vec2(xT1, zT1);
    goalTangent = Vec2(xT2, zT2);
}

//Get the RLR or LRL tangent points
void GetRLRorLRLTangents(Vec2 startCircle, Vec2 goalCircle,
                         bool isLRL, Vec2 &startTangent,
                         Vec2 &goalTangent, Vec2 &middleCircle)
{
    //The distance between the circles
    double D = (startCircle - goalCircle).getLength();

    //The angle between the goal and the new 3rd circle we create with the law of cosines
    //4.0f percision ?
    double theta = qAcos(D / (4.0f * CDubinsTurningRadius));

    //But we need to modify the angle theta if the circles are not on the same line
    Vec2 V1 = goalCircle - startCircle;

    //Different depending on if we calculate LRL or RLR
    if (isLRL)
        theta = qAtan2(V1.northing, V1.easting) + theta;
    else
        theta = qAtan2(V1.northing, V1.easting) - theta;

    //Calculate the position of the third circle
    double x = startCircle.easting + (2 * CDubinsTurningRadius * qCos(theta));
    double z = startCircle.northing + (2 * CDubinsTurningRadius * qSin(theta));

// return ?
    middleCircle = Vec2(x, z);

    //Calculate the tangent points
    Vec2 V2 = (startCircle - middleCircle);
    V2.normalize();
    Vec2 V3 = (goalCircle - middleCircle);
    V2.normalize();
    V2 *= CDubinsTurningRadius;
    V3 *= CDubinsTurningRadius;
// return ?
    startTangent = middleCircle + V2;
    goalTangent = middleCircle + V3;
}


//Calculate the length of an circle arc depending on which direction we are driving
double GetArcLength(Vec2 circleCenterPos, Vec2 startPos, Vec2 goalPos, bool isLeftCircle)
{
    Vec2 V1 = startPos - circleCenterPos;
    Vec2 V2 = goalPos - circleCenterPos;

    double theta = qAtan2(V2.northing, V2.easting) - qAtan2(V1.northing, V1.easting);
    if (theta < 0.0f && isLeftCircle) theta += 2.0 * M_PI;
    else if (theta > 0 && !isLeftCircle) theta -= 2.0 * M_PI;
    return qAbs(theta * CDubinsTurningRadius);
}


//Loops through segments of a path and add new coordinates to the final path
void AddCoordinatesToPath(Vec2 &currentPos, double &theta, QVector<Vec2> &finalPath, int segments, bool isTurning, bool isTurningRight)
{
    for (int i = 0; i <= segments; i++)
    {
        //Update the position of the car
        currentPos.easting += driveDistance * qSin(theta);
        currentPos.northing += driveDistance * qCos(theta);

        //Don't update the heading if we are driving straight
        if (isTurning)
        {
            //Which way are we turning?
            double turnParameter = 1.0;

            if (!isTurningRight) turnParameter = -1.0;

            //Update the heading
            theta += (driveDistance / CDubinsTurningRadius) * turnParameter;
        }

        //Add the new coordinate to the path
        finalPath.append(currentPos);
    }
}

CDubins::CDubins()
{

}

QVector<Vec3> CDubins::GenerateDubins(Vec3 _start, Vec3 _goal)
{
    QVector<Vec3> dubinsShortestPathList;
    startPos.easting = _start.easting;
    startPos.northing = _start.northing;
    startHeading = _start.heading;

    goalPos.easting = _goal.easting;
    goalPos.northing = _goal.northing;
    goalHeading = _goal.heading;

    //Get all valid Dubins paths
    pathDataList = GetAllDubinsPaths();

    //clear out existing path of vec3 points
    //dubinsShortestPathList.clear();

    if (pathDataList.count() > 0)
    {
        int cnt = pathDataList[0].pathCoordinates.count();
        if (cnt > 1)
        {
            //calculate the heading for each point
            for (int i = 0; i < cnt - 1; i += 5)
            {
                Vec3 pt(pathDataList[0].pathCoordinates[i].easting, pathDataList[0].pathCoordinates[i].northing, 0);
                pt.heading = qAtan2(pathDataList[0].pathCoordinates[i + 1].easting - pathDataList[0].pathCoordinates[i].easting,
                                    pathDataList[0].pathCoordinates[i + 1].northing - pathDataList[0].pathCoordinates[i].northing);
                dubinsShortestPathList.append(pt);
            }
        }
    }
    return dubinsShortestPathList;
}

QVector<Vec3> CDubins::GenerateDubins(Vec3 _start, Vec3 _goal, const CBoundary &bnd, CGeoFence &fence)
{
    QVector<Vec3> dubinsShortestPathList;
    //positions and heading
    startPos.easting = _start.easting;
    startPos.northing = _start.northing;
    startHeading = _start.heading;

    goalPos.easting = _goal.easting;
    goalPos.northing = _goal.northing;
    goalHeading = _goal.heading;

    //Get all valid Dubins paths
    pathDataList = GetAllDubinsPaths();

    //clear out existing path of vec3 points
    dubinsShortestPathList.clear();

    int pathsCnt = pathDataList.count();
    if (pathsCnt > 0)
    {
        for (int i = 0; i < pathsCnt; i++)
        {
            int cnt = pathDataList[i].pathCoordinates.count();
            if (cnt > 1)
            {
                for (int j = 0; j < cnt; j++)
                {
                    if (!fence.isPointInsideGeoFences(bnd,pathDataList[i].pathCoordinates[j]))
                    {
                        pathDataList.removeAt(i);
                        pathsCnt = pathDataList.count();
                        i = -1;
                        break;
                    }
                }
            }
        }
    }

    if (pathDataList.count()> 0)
    {
        int cnt = pathDataList[0].pathCoordinates.count();
        if (cnt > 1)
        {
            //calculate the heading for each point
            for (int i = 0; i < cnt - 1; i += 5)
            {
                Vec3 pt(pathDataList[0].pathCoordinates[i].easting, pathDataList[0].pathCoordinates[i].northing, 0);
                pt.heading = qAtan2(pathDataList[0].pathCoordinates[i + 1].easting - pathDataList[0].pathCoordinates[i].easting,
                pathDataList[0].pathCoordinates[i + 1].northing - pathDataList[0].pathCoordinates[i].northing);
                dubinsShortestPathList.append(pt);
            }
        }
    }
    return dubinsShortestPathList;
}

QVector<OneDubinsPath> CDubins::GetAllDubinsPaths()
{
    //Reset the list with all Dubins paths
    pathDataList.clear();

    //Position the circles that are to the left/right of the cars
    PositionLeftRightCircles();

    //Find the length of each path with tangent coordinates
    CalculateDubinsPathsLengths();

    //If we have paths
    if (pathDataList.count() > 0)
    {
        //Sort the list with paths so the shortest path is first
//        pathDataList.Sort((x, y) => x.totalLength.CompareTo(y.totalLength));
        std::sort(pathDataList.begin(), pathDataList.end());
        //Generate the final coordinates of the path from tangent points and segment lengths
        GeneratePathCoordinates();
    }

    //No paths could be found
    else
    {
        pathDataList.clear();
    }

    //return either empty or the actual list.
    return pathDataList;
}

void CDubins::PositionLeftRightCircles()
{
    //Goal pos
   goalRightCircle = GetRightCircleCenterPos(goalPos, goalHeading);

   goalLeftCircle = GetLeftCircleCenterPos(goalPos, goalHeading);

   //Start pos
   startRightCircle = GetRightCircleCenterPos(startPos, startHeading);

   startLeftCircle = GetLeftCircleCenterPos(startPos, startHeading);
}

void CDubins::CalculateDubinsPathsLengths()
{
    //

    //RSR ****RSR and LSL is only working if the circles don't have the same position
    if (startRightCircle.easting != goalRightCircle.easting && startRightCircle.northing != goalRightCircle.northing)
    {
        Get_RSR_Length();
    }

    //LSL
    if (startLeftCircle.easting != goalLeftCircle.easting && startLeftCircle.northing != goalLeftCircle.northing)
    {
        Get_LSL_Length();
    }

    //RSL and LSR is only working of the circles don't intersect
    double comparisonSqr = CDubinsTurningRadius * 2.0f * CDubinsTurningRadius * 2.0f;

    //RSL
    if ((startRightCircle - goalLeftCircle).getLengthSquared() > comparisonSqr)
    {
        Get_RSL_Length();
    }

    //LSR
    if ((startLeftCircle - goalRightCircle).getLengthSquared() > comparisonSqr)
    {
        Get_LSR_Length();
    }

    //With the LRL and RLR paths, the distance between the circles have to be less than 4 * r
    comparisonSqr = 4.0f * CDubinsTurningRadius * 4.0f * CDubinsTurningRadius;

    //RLR
    if ((startRightCircle - goalRightCircle).getLengthSquared() < comparisonSqr)
    {
        Get_RLR_Length();
    }

    //LRL
    if ((startLeftCircle - goalLeftCircle).getLengthSquared() < comparisonSqr)
    {
        Get_LRL_Length();
    }
}

void CDubins::Get_RSR_Length()
{
    //Find both tangent positons
   Vec2 startTangent(0, 0);
   Vec2 goalTangent(0, 0);

   LSLorRSR(startRightCircle, goalRightCircle, false, startTangent, goalTangent);


   //Calculate lengths
   double length1 = GetArcLength(startRightCircle, startPos, startTangent, false);
   double length2 = (startTangent - goalTangent).getLength();
   double length3 = GetArcLength(goalRightCircle, goalTangent, goalPos, false);

   //Save the data
   OneDubinsPath pathData(length1, length2, length3, startTangent, goalTangent, PathType::RSR);

   //We also need this data to simplify when generating the final path
   pathData.segment2Turning = false;

   //RSR
   pathData.SetIfTurningRight(true, false, true);

   //Add the path to the collection of all paths
   pathDataList.append(pathData);
}

void CDubins::Get_LSL_Length()
{
    //Find both tangent positions
      Vec2 startTangent = Vec2(0, 0);
      Vec2 goalTangent = Vec2(0, 0);

      LSLorRSR(startLeftCircle, goalLeftCircle, true, startTangent, goalTangent);

      //Calculate lengths
      double length1 = GetArcLength(startLeftCircle, startPos, startTangent, true);
      double length2 = (startTangent - goalTangent).getLength();
      double length3 = GetArcLength(goalLeftCircle, goalTangent, goalPos, true);

      //Save the data
      OneDubinsPath pathData(length1, length2, length3, startTangent, goalTangent, PathType::LSL);

      //We also need this data to simplify when generating the final path
      pathData.segment2Turning = false;

      //LSL
      pathData.SetIfTurningRight(false, false, false);

      //Add the path to the collection of all paths
      pathDataList.append(pathData);
}

void CDubins::Get_RSL_Length()
{
    //Find both tangent positions
    Vec2 startTangent = Vec2(0, 0);
    Vec2 goalTangent = Vec2(0, 0);

    RSLorLSR(startRightCircle, goalLeftCircle, false, startTangent, goalTangent);

    //Calculate lengths
    double length1 = GetArcLength(startRightCircle, startPos, startTangent, false);
    double length2 = (startTangent - goalTangent).getLength();
    double length3 = GetArcLength(goalLeftCircle, goalTangent, goalPos, true);

    //Save the data
    OneDubinsPath pathData(length1, length2, length3, startTangent, goalTangent, PathType::RSL);

    //We also need this data to simplify when generating the final path
    pathData.segment2Turning = false;

    //RSL
    pathData.SetIfTurningRight(true, false, false);

    //Add the path to the collection of all paths
    pathDataList.append(pathData);
}

void CDubins::Get_LSR_Length()
{
    //Find both tangent positions
    Vec2 startTangent = Vec2(0, 0);
    Vec2 goalTangent = Vec2(0, 0);

    RSLorLSR(startLeftCircle, goalRightCircle, true, startTangent, goalTangent);

    //Calculate lengths
    double length1 = GetArcLength(startLeftCircle, startPos, startTangent, true);
    double length2 = (startTangent - goalTangent).getLength();
    double length3 = GetArcLength(goalRightCircle, goalTangent, goalPos, false);

    //Save the data
    OneDubinsPath pathData(length1, length2, length3, startTangent, goalTangent, PathType::LSR);

    //We also need this data to simplify when generating the final path
    pathData.segment2Turning = false;

    //LSR
    pathData.SetIfTurningRight(false, false, true);

    //Add the path to the collection of all paths
    pathDataList.append(pathData);
}

void CDubins::Get_RLR_Length()
{
    Vec2 startTangent = Vec2(0, 0);
    Vec2 goalTangent = Vec2(0, 0);
    Vec2 middleCircle = Vec2(0, 0);

    GetRLRorLRLTangents(
        startRightCircle,
        goalRightCircle,
        false,
        startTangent,
        goalTangent,
        middleCircle);

    //Calculate lengths
    double length1 = GetArcLength(startRightCircle, startPos, startTangent, false);
    double length2 = GetArcLength(middleCircle, startTangent, goalTangent, true);
    double length3 = GetArcLength(goalRightCircle, goalTangent, goalPos, false);

    //Save the data
    OneDubinsPath pathData(length1, length2, length3, startTangent, goalTangent, PathType::RLR);

    //We also need this data to simplify when generating the final path
    pathData.segment2Turning = true;

    //RLR
    pathData.SetIfTurningRight(true, false, true);

    //Add the path to the collection of all paths
    pathDataList.append(pathData);
}

void CDubins::Get_LRL_Length()
{
    Vec2 startTangent = Vec2(0, 0);
    Vec2 goalTangent = Vec2(0, 0);
    Vec2 middleCircle = Vec2(0, 0);

    GetRLRorLRLTangents(
        startLeftCircle,
        goalLeftCircle,
        true,
        startTangent,
        goalTangent,
        middleCircle);

    //Calculate the total length of this path
    double length1 = GetArcLength(startLeftCircle, startPos, startTangent, true);
    double length2 = GetArcLength(middleCircle, startTangent, goalTangent, false);
    double length3 = GetArcLength(goalLeftCircle, goalTangent, goalPos, true);

    //Save the data
    OneDubinsPath pathData(length1, length2, length3, startTangent, goalTangent, PathType::LRL);

    //We also need this data to simplify when generating the final path
    pathData.segment2Turning = true;

    //LRL
    pathData.SetIfTurningRight(false, true, false);

    //Add the path to the collection of all paths
    pathDataList.append(pathData);
}
// Generate the final path from the tangent points
//When we have found the tangent points and lengths of each path we need to get the individual coordinates
//of the entire path so we can travel along the path
void CDubins::GeneratePathCoordinates()
{
    for (int i = 0; i < pathDataList.count(); i++)
    {
        GetTotalPath(pathDataList[i]);
    }
}

void CDubins::GetTotalPath(OneDubinsPath &pathData)
{
//Store the waypoints of the final path here
    QVector<Vec2> finalPath = QVector<Vec2>();

    //Start position of the car
    Vec2 currentPos = startPos;
    //Start heading of the car
    double theta = startHeading;

    //We always have to add the first position manually
    finalPath.append(currentPos);

    //How many line segments can we fit into this part of the path

    //First
    int segments = (int)qFloor(pathData.length1 / driveDistance);

    AddCoordinatesToPath(
        currentPos,
        theta,
        finalPath,
        segments,
        true,
        pathData.segment1TurningRight);

//    //Second
    segments = (int)qFloor(pathData.length2 / driveDistance);

    AddCoordinatesToPath(
        currentPos,
        theta,
        finalPath,
        segments,
        pathData.segment2Turning,
        pathData.segment2TurningRight);

//    //Third
    segments = (int)qFloor(pathData.length3 / driveDistance);

    AddCoordinatesToPath(
        currentPos,
        theta,
        finalPath,
        segments,
        true,
        pathData.segment3TurningRight);

    //Add the final goal coordinate
    finalPath.append(Vec2(goalPos.easting, goalPos.northing));

    //Save the final path in the path data
    pathData.pathCoordinates = finalPath;
}

OneDubinsPath::OneDubinsPath() :
    totalLength(0), length1(0), length2(0), length3(0)
{

}

//OneDubinsPath(const OneDubinsPath &src) :
//    totalLength(src.totalLength), length1(src.length1), length2(src.length2),
//    length3(src.length3), tangent1(src.tangent1), tangent2(src.tangent2),
//    pathType(src.pathType)
//{ }

OneDubinsPath::OneDubinsPath(double length1, double length2, double length3, const Vec2 &tangent1, const Vec2 &tangent2, PathType pathType)
{
    this->totalLength = length1 + length2 + length3;

    this->length1 = length1;
    this->length2 = length2;
    this->length3 = length3;

    this->tangent1 = tangent1;
    this->tangent2 = tangent2;

    this->pathType = pathType;
}

void OneDubinsPath::SetIfTurningRight(bool segment1TurningRight, bool segment2TurningRight, bool segment3TurningRight)
{
    this->segment1TurningRight = segment1TurningRight;
    this->segment2TurningRight = segment2TurningRight;
    this->segment3TurningRight = segment3TurningRight;
}
