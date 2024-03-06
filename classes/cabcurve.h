#ifndef CABCURVE_H
#define CABCURVE_H

#include <QObject>
#include <QVector>
#include "vec2.h"
#include "vec3.h"
#include "ctrk.h"

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


//TODO: move these to their own module
//currently defined in formheadland.cpp
void CalculateHeadings(QVector<Vec3> &xList);
void MakePointMinimumSpacing(QVector<Vec3> &xList, double minDistance);
int GetLineIntersection(double p0x, double p0y, double p1x, double p1y,
                        double p2x, double p2y, double p3x, double p3y,
                        double &iEast,
                        double &iNorth);

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
    int currentLocationIndex;

    //pure pursuit values
    Vec2 goalPointCu = Vec2(0, 0);

    Vec2 radiusPointCu = Vec2(0, 0);
    double steerAngleCu, rEastCu, rNorthCu, ppRadiusCu, manualUturnHeading;

    bool isSmoothWindowOpen, isLooping;
    QVector<Vec3> smooList;

    //the list of points of curve to drive on
    QVector<Vec3> curList;

    bool isCurveValid, isLateralTriggered;

    double lastSecond = 0;

    QVector<Vec3> desList;
    QString desName = "**";

    double pivotDistanceError, pivotDistanceErrorLast, pivotDerivative, pivotDerivativeSmoothed, lastCurveDistance = 10000;

    double inty;

    explicit CABCurve(QObject *parent = 0);

    void BuildCurveCurrentList(Vec3 pivot,
                               double secondsSinceStart,
                               CTrack &trk,
                               const CVehicle &vehicle,
                               const CBoundary &bnd,
                               const CYouTurn &yt);
    void GetCurrentCurveLine(Vec3 pivot,
                             Vec3 steer,
                             double secondsSinceStart,
                             bool isAutoSteerBtnOn,
                             bool steerSwitchHigh,
                             CTrack &trk,
                             CVehicle &vehicle,
                             CYouTurn &yt,
                             const CAHRS &ahrs,
                             CGuidance &gyd,
                             CNMEA &pn);

    void DrawCurveNow(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);

    void DrawCurve(QOpenGLFunctions *gl, const QMatrix4x4 &mvp,
                   bool isFontOn,
                   const CTrack &trk,
                   CYouTurn &yt, const CCamera &camera
                   );

    //void drawTram(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);

    void BuildTram(const CTrack &trk, const CBoundary &bnd, CTram &tram);
    void SmoothAB(const CTrack &trk, int smPts);
    void CalculateHeadings(QVector<Vec3> &xList);
    void MakePointMinimumSpacing(QVector<Vec3> &xList, double minDistance);
    void SaveSmoothList(CTrack &trk);
    bool PointOnLine(Vec3 pt1, Vec3 pt2, Vec3 pt);
    void AddFirstLastPoints(QVector<Vec3> &xList, const CBoundary &bnd);
    void ResetCurveLine(CTrack &trk);
    void BuildOutGuidanceList(Vec3 pivot, CTrack &trk, CYouTurn &yt,
                              const CBoundary &bnd
                              );


signals:
    void TimedMessage(int timeout, QString title, QString message);
    void stopAutoSteer();

public slots:
};

#endif // CABCURVE_H
