#ifndef CCONTOUR_H
#define CCONTOUR_H
#include <QObject>
#include <QVector>
#include <QSharedPointer>
#include "vec2.h"
#include "vec3.h"

class QOpenGLFunctions;
class QMatrix4x4;
class CVehicle;
class CBoundary;
class CYouTurn;
class CTool;
class CAHRS;
class CNMEA;

class CContour: public QObject
{
    Q_OBJECT
private:
    int A = 0, B = 0, C = 0;
    int stripNum, lastLockPt = INT_MAX;
    int counter2;
    double lastSecond;
    int pt = 0;

public:
    bool isContourOn=false, isContourBtnOn=false, isRightPriority = true;
    // for closest line point to current fix
    double minDistance = 99999.0, refX, refZ;

    double distanceFromCurrentLinePivot;

    double abFixHeadingDelta, abHeading;

    Vec2 boxA = Vec2(0, 0);
    Vec2 boxB = Vec2(0, 2);

    bool isHeadingSameWay = true;

    Vec2 goalPointCT = Vec2(0, 0);
    double steerAngleCT;
    double rEastCT, rNorthCT;
    double ppRadiusCT;

    double pivotDistanceError, pivotDistanceErrorLast, pivotDerivative, pivotDerivativeSmoothed;

    double inty;
    double steerAngleSmoothed, pivotErrorTotal;
    double distSteerError, lastDistSteerError, derivativeDistError;

    //list of strip data individual points
    QSharedPointer<QVector<Vec3>> ptList;

    //list of the list of individual Lines for entire field
    QVector<QSharedPointer<QVector<Vec3>>> stripList;

    //list of points for the new contour line
    QVector<Vec3> ctList;

    bool isLocked = false;

    explicit CContour(QObject *parent = 0);
    void SetLockToLine();
    void BuildContourGuidanceLine(double secondsSinceStart, CVehicle &vehicle, Vec3 pivot);
    void DistanceFromContourLine(bool isAutoSteerBtnOn, CVehicle &vehicle, CYouTurn &yt, CAHRS &ahrs,  CNMEA &pn, Vec3 pivot, Vec3 steer);
    void StartContourLine();
    void AddPoint(Vec3 pivot);
    void StopContourLine(QVector<QSharedPointer<QVector<Vec3>>> &contourSaveList);
    void BuildFenceContours(CBoundary &bnd, double spacingInt, int patchCounter);
    void DrawContourLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
    void ResetContour();
signals:
    //void guidanceLineDistanceOff(int);
    //void distanceDisplay(int);
    //void guidanceLineSteerAngle(int);
    void TimedMessage(int,QString,QString);
};

#endif // CCONTOUR_H
