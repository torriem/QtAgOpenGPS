#ifndef CABCURVE_H
#define CABCURVE_H

#include <QObject>
#include <QVector>
#include "vec2.h"
#include "vec3.h"

class QOpenGLFunctions;
class QMatrix4x4;
class CVehicle;
class CYouTurn;
class CTram;
class CCamera;
class CBoundary;
class CNMEA;
class CAHRS;
class CGuidance;
class CTrack;


class CCurveLines
{
public:
    QVector<Vec3> curvePts;
    double aveHeading = 3;
    QString Name = "aa";
    bool isVisible = true;
};



class CABCurve : public QObject
{
    Q_OBJECT
private:
    int A, B, C;
    int rA, rB;

    int counter2;

public:
    //flag for starting stop adding points
    bool isBtnTrackOn, isMakingCurve;

    double distanceFromCurrentLinePivot;
    double distanceFromRefLine;

    bool isHeadingSameWay = true;

    double howManyPathsAway;

    Vec2 refPoint1 = Vec2(1, 1), refPoint2 = Vec2(2, 2);

    Vec2 boxC = Vec2(1, 1), boxD = Vec2(2, 3);
    int currentLocationIndex;

    //pure pursuit values
    Vec2 goalPointCu = Vec2(0, 0);

    Vec2 radiusPointCu = Vec2(0, 0);
    double steerAngleCu, rEastCu, rNorthCu, ppRadiusCu, manualUturnHeading;

    bool isSmoothWindowOpen, isLooping;
    QVector<Vec3> smooList;

    //the list of points of curve to drive on
    QVector<Vec3> curList;

    //the current curve reference line.
    //CTrk refCurve;

    bool isCurveValid, isLateralTriggered;

    double lastSecond = 0;

    QVector<Vec3> desList;
    QString desName = "**";

    double pivotDistanceError, pivotDistanceErrorLast, pivotDerivative, pivotDerivativeSmoothed, lastCurveDistance = 10000;

    double inty;

    explicit CABCurve(QObject *parent = 0);

    void BuildCurveCurrentList(Vec3 pivot,
                               double secondsSinceStart,
                               const CVehicle &vehicle,
                               CTrack &trk,
                               const CBoundary &bnd,
                               const CYouTurn &yt);
    void GetCurrentCurveLine(Vec3 pivot,
                             Vec3 steer,
                             bool isAutoSteerBtnOn,
                             CVehicle &vehicle,
                             CTrack &trk,
                             CYouTurn &yt,
                             const CAHRS &ahrs,
                             CGuidance &gyd,
                             CNMEA &pn);


    void DrawCurveNew(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);

    void DrawCurve(QOpenGLFunctions *gl, const QMatrix4x4 &mvp,
                   bool isFontOn,
                   const CTrack trk,
                   CYouTurn &yt, const CCamera &camera
                   );

    //void drawTram(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);

    void BuildTram(CBoundary &bnd, CTram &tram, const CTrack &trk);
    void SmoothAB(int smPts, const CTrack &trk);
    void CalculateHeadings(QVector<Vec3> &xList);
    void MakePointMinimumSpacing(QVector<Vec3> &xList, double minDistance);
    void SaveSmoothList(CTrack &trk);
    void MoveABCurve(double dist);
    bool PointOnLine(Vec3 pt1, Vec3 pt2, Vec3 pt);
    void AddFirstLastPoints(QVector<Vec3> &xList, const CBoundary &bnd);
    void ResetCurveLine(CTrack &trk);


signals:
    void TimedMessage(int timeout, QString title, QString message);
    void stopAutoSteer();

public slots:
};

#endif // CABCURVE_H
