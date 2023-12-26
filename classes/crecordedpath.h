#ifndef CRECORDEDPATH_H
#define CRECORDEDPATH_H

#include <QObject>
#include <QVector>
#include "vec3.h"
#include "vec2.h"
#include "btnenum.h"

class QOpenGLFunctions;
class QMatrix4x4;
class CVehicle;
class CYouTurn;

class CRecPathPt
{
public:
    double easting;
    double northing;
    double heading;
    double speed;
    bool autoBtnState;

    //constructor
    CRecPathPt(): easting(0), northing(0), heading(0), speed(0), autoBtnState(btnStates::Off)
    {
    }

    CRecPathPt(double _easting, double _northing, double _heading, double _speed,
                        bool _autoBtnState)
    {
        easting = _easting;
        northing = _northing;
        heading = _heading;
        speed = _speed;
        autoBtnState = _autoBtnState;
    }
};


class CRecordedPath : public QObject
{
    Q_OBJECT
private:
    int A, B, C;
    int counter2;

public:
    //the recorded path from driving around
    QVector<CRecPathPt> recList;

    int recListCount;

    //the dubins path to get there
    QVector<CRecPathPt> shuttleDubinsList;

    int shuttleListCount;

    //list of vec3 points of Dubins shortest path between 2 points - To be converted to RecPt
    QVector<Vec3> shortestDubinsList;

    //generated reference line
    Vec2 refPoint1 = Vec2(1, 1), refPoint2 = Vec2(2, 2);

    double distanceFromRefLine, distanceFromCurrentLinePivot;

    int currentPositonIndex;

    //pure pursuit values
    Vec3 pivotAxlePosRP = Vec3(0, 0, 0);

    Vec3 homePos;
    Vec2 goalPointRP = Vec2(0, 0);
    double steerAngleRP, rEastRP, rNorthRP, ppRadiusRP;
    Vec2 radiusPointRP = Vec2(0,0);

    bool isBtnFollowOn, isEndOfTheRecLine, isRecordOn;
    bool isDrivingRecordedPath, isFollowingDubinsToPath, isFollowingRecPath, isFollowingDubinsHome;

    double pivotDistanceError, pivotDistanceErrorLast, pivotDerivative, pivotDerivativeSmoothed;

    //derivative counters

    double inty;
    double steerAngleSmoothed, pivotErrorTotal;
    double distSteerError, lastDistSteerError, derivativeDistError;

    int resumeState;

    int starPathIndx = 0;

    bool trig;
    double north;
    int pathCount = 0;


    explicit CRecordedPath(QObject *parent = 0);
    bool StartDrivingRecordedPath(CVehicle &vehicle);
    void UpdatePosition(CVehicle &vehicle, const CYouTurn &yt, bool isAutoSteerBtnOn);
    void StopDrivingRecordedPath();
    void GetDubinsPath(CVehicle &vehicle, Vec3 goal);
    void PurePursuitRecPath(CVehicle &vehicle, int ptCount);
    void PurePursuitDubins(CVehicle &vehicle, const CYouTurn &yt, bool isAutoSteerButtonOn, int ptCount);

    void drawRecordedLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
    void drawDubins(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);

signals:
    void setSimStepDistance(double speed);
    void stoppedDriving();
    //void guidanceLineDistanceOff(int);
    //void guidanceLineSteerAngle(int);
    //void btnSectionSet(btnStates);
    void turnOffSectionMasterAuto();

public slots:
};

#endif // CRECORDEDPATH_H
