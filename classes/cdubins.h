#ifndef CDUBINS_H
#define CDUBINS_H

#include <QVector>
#include <QtMath>
#include <QList>

#include "vec2.h"
#include "vec3.h"

class CBoundary;

using namespace std;

//The radius the car can turn 360 degrees with
extern double CDubinsTurningRadius;

enum PathType { RSR, LSL, RSL, LSR, RLR, LRL };

class OneDubinsPath
{
public:
    //Tthe total length of this path
    double totalLength;

    //Need the individual path lengths for debugging and to find the final path
    double length1, length2, length3;

    //The 2 tangent points we need to connect the lines and curves
    Vec2 tangent1, tangent2;

    //The type, such as RSL
    PathType pathType;

    //The coordinates of the final path
    QVector<Vec2> pathCoordinates;

    //Are we turning or driving straight in segment 2?
    bool segment2Turning;

    //Are we turning right in the particular segment?
    bool segment1TurningRight, segment2TurningRight, segment3TurningRight;

    OneDubinsPath();

    OneDubinsPath(double length1, double length2, double length3,
                  const Vec2 &tangent1, const Vec2 &tangent2, PathType pathType);

    //inline bool operator< (OneDubinsPath &rhs) {
    //    return (totalLength < rhs.totalLength);
    //}
    inline friend bool operator< (const OneDubinsPath lhs, const OneDubinsPath rhs) {
        return (lhs.totalLength < rhs.totalLength);
    }

    void SetIfTurningRight(bool segment1TurningRight,
                           bool segment2TurningRight,
                           bool segment3TurningRight);

};


class CDubins
{
public:
    //How far we are driving each update, the accuracy will improve if we lower the driveDistance
    //static float turningRadius // in cpp file
    //double driveDistance = 0.05; //defined in cpp file

    //The radius the car can turn 360 degrees with
    //double turningRadius = Properties.Settings.Default.setVehicle_minTurningRadius;

    void loadSettings();

    CDubins();
    //To keep track of the different paths when debugging

    //How far we are driving each update, the accuracy will improve if we lower the driveDistance
    //static const double driveDistance;

    //takes 2 points and headings to create a path - returns list of vec3 points and headings
    QVector<Vec3> GenerateDubins(Vec3 _start, Vec3 _goal);


private:
    //Position, Heading is in radians
    Vec2 startPos, goalPos;

    double startHeading, goalHeading;

    QVector<OneDubinsPath> pathDataList;

    //QVector<Vec3> dubinsShortestPathList;

    Vec2 startLeftCircle, startRightCircle, goalLeftCircle, goalRightCircle;

    QVector<OneDubinsPath> GetAllDubinsPaths();
    void PositionLeftRightCircles();
    void CalculateDubinsPathsLengths();
    void Get_RSR_Length();
    void Get_LSL_Length();
    void Get_RSL_Length();
    void Get_LSR_Length();
    void Get_RLR_Length();
    void Get_LRL_Length();
    void GeneratePathCoordinates();
    void GetTotalPath(OneDubinsPath &pathData);

};



#endif // CDUBINS_H
