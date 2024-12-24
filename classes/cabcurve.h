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
class CTrk;

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

    double howManyPathsAway, lastHowManyPathsAway;

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

    bool isReady = false, isBusyWorking = false;

    //the list of points of curve new list from async
    QVector<Vec3> newCurList;

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
                               const CTrk &track,
                               const CBoundary &bnd,
                               const CYouTurn &yt);

    void BuildNewCurveAsync(double distAway,
                            int refCount,
                            const CTrk &track,
                            const CBoundary &bnd);

    void GetCurrentCurveLine(Vec3 pivot,
                             Vec3 steer,
                             bool isAutoSteerBtnOn,
                             CVehicle &vehicle,
                             CTrk &track,
                             CYouTurn &yt,
                             const CAHRS &ahrs,
                             CGuidance &gyd,
                             CNMEA &pn, int &makeUTurnCounter);


    void DrawCurveNew(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);

    void DrawCurve(QOpenGLFunctions *gl, const QMatrix4x4 &mvp,
                   bool isFontOn,
                   const CTrk &track,
                   CYouTurn &yt, const CCamera &camera
                   );

    //void drawTram(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);

    void BuildTram(CBoundary &bnd, CTram &tram, const CTrk &track);
    void SmoothAB(int smPts, const CTrk &track);
    void SmoothABDesList(int smPts);
    void CalculateHeadings(QVector<Vec3> &xList);
    void MakePointMinimumSpacing(QVector<Vec3> &xList, double minDistance);
    void SaveSmoothList(CTrk &track);
    void MoveABCurve(double dist);
    bool PointOnLine(Vec3 pt1, Vec3 pt2, Vec3 pt);
    void AddFirstLastPoints(QVector<Vec3> &xList, const CBoundary &bnd);

    CABCurve &operator=(CABCurve &src)
    {
        A=src.A;
        B=src.B;
        C=src.C;
        rA=src.rA;
        rB=src.rB;
        counter2 = src.counter2;
        isBtnTrackOn = src.isBtnTrackOn;
        isMakingCurve = src.isMakingCurve;

        distanceFromCurrentLinePivot = src.distanceFromCurrentLinePivot;
        distanceFromRefLine = src.distanceFromRefLine;

        isHeadingSameWay = true;

        howManyPathsAway = src.howManyPathsAway;
        lastHowManyPathsAway = src.lastHowManyPathsAway;

        refPoint1 = src.refPoint1;
        refPoint2 = src.refPoint2;

        boxC = src.boxC;
        boxD = src.boxD;
        currentLocationIndex = src.currentLocationIndex;

        goalPointCu = src.goalPointCu;

        radiusPointCu = src.radiusPointCu;
        steerAngleCu = src.steerAngleCu;
        rEastCu = src.rEastCu;
        rNorthCu = src.rNorthCu;
        ppRadiusCu = src.ppRadiusCu;
        manualUturnHeading = src.manualUturnHeading;

        isSmoothWindowOpen = src.isSmoothWindowOpen;
        isLooping = src.isLooping;

        smooList = src.smooList;
        curList = src.curList;

        isReady = src.isReady;
        isBusyWorking = src.isBusyWorking;

        newCurList = src.newCurList;

        isCurveValid = src.isCurveValid;
        isLateralTriggered = src.isLateralTriggered;

        lastSecond = src.lastSecond;

        desList = src.desList;
        desName = src.desName;

        pivotDistanceError = src.pivotDistanceError;
        pivotDistanceErrorLast = src.pivotDistanceErrorLast;
        pivotDerivative = src.pivotDerivative;
        pivotDerivativeSmoothed = src.pivotDerivativeSmoothed;
        lastCurveDistance = src.lastCurveDistance;

        inty = src.inty;

        return *this;
    }

signals:
    void TimedMessage(int timeout, QString title, QString message);
    void stopAutoSteer();

public slots:
};

#endif // CABCURVE_H
