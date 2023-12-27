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
    bool isBtnCurveOn, isOkToAddDesPoints, isCurveSet;

    double distanceFromCurrentLinePivot;
    double distanceFromRefLine;

    bool isHeadingSameWay = true;

    double howManyPathsAway;

    Vec2 refPoint1 = Vec2(1, 1), refPoint2 = Vec2(2, 2);

    double refHeading, moveDistance;

    Vec2 boxC = Vec2(1, 1), boxD = Vec2(2, 3);
    int currentLocationIndex;

    double aveLineHeading;

    //pure pursuit values
    Vec2 goalPointCu = Vec2(0, 0);

    Vec2 radiusPointCu = Vec2(0, 0);
    double steerAngleCu, rEastCu, rNorthCu, ppRadiusCu, manualUturnHeading;

    //the list of points of the ref line.
    QVector<Vec3> refList;
    //the list of points of curve to drive on
    QVector<Vec3> curList;

    bool isSmoothWindowOpen;
    QVector<Vec3> smooList;

    QVector<CCurveLines> curveArr;
    int numCurveLines, numCurveLineSelected;

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
                               const CBoundary &bnd,
                               const CYouTurn &yt);
    void GetCurrentCurveLine(Vec3 pivot,
                             Vec3 steer,
                             double secondsSinceStart,
                             bool isAutoSteerBtnOn,
                             bool steerSwitchHigh,
                             CVehicle &vehicle,
                             const CBoundary &bnd,
                             CYouTurn &yt,
                             const CAHRS &ahrs,
                             CGuidance &gyd,
                             CNMEA &pn);


    void DrawCurve(QOpenGLFunctions *gl, const QMatrix4x4 &mvp,
                   bool isFontOn,
                   const CVehicle &vehicle,
                   CYouTurn &yt, const CCamera &camera
                   );

    //void drawTram(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);

    void BuildTram(CBoundary &bnd, CTram &tram);
    void SmoothAB(int smPts);
    void CalculateTurnHeadings();
    void SaveSmoothAsRefList();
    void MoveABCurve(double dist);
    bool PointOnLine(Vec3 pt1, Vec3 pt2, Vec3 pt);
    void AddFirstLastPoints(QVector<Vec3> &xList, const CBoundary &bnd);
    void ResetCurveLine();


signals:
    void TimedMessage(int timeout, QString title, QString message);
    void stopAutoSteer();

public slots:
};

#endif // CABCURVE_H
