#ifndef YOUTURN_H
#define YOUTURN_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector4D>
#include "vec4.h"
#include "vec3.h"
#include "vec2.h"
#include "interfaceproperty.h"
#include "cabline.h"

class CBoundary;
class CABCurve;
class CABLine;
class CVehicle;
class QOpenGLFunctions;
class CVehicle;
class CNMEA;
class CTrack;

//class QMatrix4x4;

class CClose {
public:
    Vec3 closePt;
    int turnLineNum;
    int turnLineIndex;
    double turnLineHeading;
    int curveIndex;

    CClose()
    {
        closePt = Vec3();
        turnLineNum = -1;
        turnLineIndex = -1;
        turnLineHeading = -1;
        curveIndex = -1;
    }

    CClose(const CClose &_clo)
    {
        closePt = _clo.closePt;
        turnLineNum = _clo.turnLineNum;
        turnLineIndex = _clo.turnLineIndex;
        turnLineHeading = _clo.turnLineHeading;
        curveIndex = _clo.curveIndex;
    }
    CClose &operator=(const CClose &other) = default;
};

class CYouTurn: public QObject
{
    Q_OBJECT
private:
    int A, B;
    bool isHeadingSameWay = true;
    int semiCircleIndex = -1;

    //how far should the distance between points on the uTurn be
    double pointSpacing;


public:
    //triggered right after youTurnTriggerPoint is set
    bool isYouTurnTriggered, isGoingStraightThrough = false;

    //turning right or left?
    bool isYouTurnRight;

    // Is the youturn button enabled?
    InterfaceProperty<AOGInterface,bool> isYouTurnBtnOn = InterfaceProperty<AOGInterface,bool>("isYouTurnBtnOn");
    InterfaceProperty<AOGInterface,bool> isBtnAutoSteerOn = InterfaceProperty<AOGInterface,bool>("isBtnAutoSteerOn");

    double boundaryAngleOffPerpendicular, youTurnRadius;

    int rowSkipsWidth = 1, uTurnSmoothing;

    bool alternateSkips = false, previousBigSkip = true;
    int rowSkipsWidth2 = 3, turnSkips = 2;

    /// <summary>  /// distance from headland as offset where to start turn shape /// </summary>
    int youTurnStartOffset;

    //guidance values
    double distanceFromCurrentLine, uturnDistanceFromBoundary, dxAB, dyAB;

    double distanceFromCurrentLineSteer, distanceFromCurrentLinePivot;
    double steerAngleGu, rEastSteer, rNorthSteer, rEastPivot, rNorthPivot;
    double pivotCurvatureOffset, lastCurveDistance = 10000;

    bool isTurnCreationTooClose = false, isTurnCreationNotCrossingError = false, turnTooCloseTrigger = false;

    //pure pursuit values
    Vec3 pivot = Vec3(0,0,0);

    Vec2 goalPointYT = Vec2(0,0);
    Vec2 radiusPointYT = Vec2(0,0);
    double steerAngleYT;
    double rEastYT, rNorthYT;
    double ppRadiusYT;

    //list of points for scaled and rotated YouTurn line
    QVector<Vec3> ytList;
    QVector<Vec3> ytList2;

    //next curve or line to build out turn and point over
    //QSharedPointer<CABCurve> nextCurve;
    //QSharedPointer<CABLine> nextLine;
    Vec3 nextLookPos;

    //if we continue on the same line or change to the next one after the uTurn
    bool isOutSameCurve;

    //for 3Pt turns - second turn
    QVector<Vec3> pt3ListSecondLine;

    int uTurnStyle = 0;

    int pt3Phase = 0;
    Vec3 kStyleNewLookPos = Vec3(0, 0, 0);
    bool isLastFrameForward = true;

    //is UTurn pattern in or out of bounds
    bool isOutOfBounds = false;

    //sequence of operations of finding the next turn 0 to 3
    int youTurnPhase;

    double crossingheading = 0;

    // Returns 1 if the lines intersect, otherwis
    double iE = 0, iN = 0;

    // the list of possible bounds points
    QVector<CClose> turnClosestList;

    //point at the farthest turn segment from pivotAxle
    CClose closestTurnPt;

    //where the in and out tangents cross for Albin curve
    CClose inClosestTurnPt;
    CClose outClosestTurnPt;
    CClose startOfTurnPt;

    int onA;

    //constructor
    explicit CYouTurn(QObject *parent = 0);

    void loadSettings();

    //Finds the point where an AB Curve crosses the turn line
    bool BuildCurveDubinsYouTurn(bool isTurnLeft,
                                 Vec3 pivotPos,
                                 CVehicle &vehicle,
                                 const CBoundary &bnd,
                                 CABCurve &curve,
                                 const CTrack &trk,
                                 int &makeUTurnCounter,
                                 int secondsSinceStart
                                 );

    bool BuildABLineDubinsYouTurn(bool isTurnLeft,
                                  CVehicle &vehicle,
                                  const CBoundary &bnd,
                                  CTrack &trk,
                                  int &makeUTurnCounter,
                                  int secondsSinceStart
                                  );


private:
    bool CreateCurveOmegaTurn(bool isTurnLeft, Vec3 pivotPos,
                              int makeUTurnCounter,
                              CVehicle &vehicle,
                              const CBoundary &bnd,
                              const CABCurve &curve,
                              const CTrack &trk,
                              int secondsSinceStart);

    bool CreateCurveWideTurn(bool isTurnLeft, Vec3 pivotPos,
                             int makeUTurnCounter,
                             CVehicle &vehicle,
                             const CBoundary &bnd,
                             CABCurve &curve,
                             const CTrack &trk,
                             int secondsSinceStart
                             );

    bool CreateABOmegaTurn(bool isTurnLeft,                              int makeUTurnCounter,
                           CVehicle &vehicle,
                           const CBoundary &bnd,
                           const CABLine &ABLine);
    bool CreateABWideTurn(bool isTurnLeft,
                          int makeUTurnCounter,
                          CVehicle &vehicle,
                          const CBoundary &bnd,
                          CABLine &ABLine,
                          CTrack &trk,
                          int secondsSinceStart);

    bool KStyleTurnCurve(bool isTurnLeft, int &makeUTurnCounter,
                         CVehicle &vehicle,
                         const CABCurve &curve,
                         const CBoundary &bnd);

    bool KStyleTurnAB(bool isTurnLeft, int &makeUTurnCounter,
                         CVehicle &vehicle,
                         const CABLine &ABLine,
                         const CBoundary &bnd);

    QVector<Vec3> &MoveABTurnInsideTurnLine(QVector<Vec3> &uTurList,
                                            double head,
                                            CVehicle &vehicle,
                                            const CBoundary &bnd)
;

public:
    void FindClosestTurnPoint(Vec3 fromPt,
                         const CABLine &ABLine,
                         const CBoundary &bnd
                              );

    bool FindCurveTurnPoints(const QVector<Vec3> &xList,
                             const CABCurve &curve,
                             const CBoundary &bnd
                             );

    bool FindCurveOutTurnPoint(CABCurve &thisCurve,
                               CABCurve &nextCurve,
                               CClose inPt,
                               bool isTurnLineSameWay,
                               const CBoundary &bnd);

    bool FindABOutTurnPoint(CABLine &thisCurve,
                            CABLine &nextCurve,
                            CClose inPt,
                            bool isTurnLineSameWay,
                            const CABLine &ABLine,
                            const CBoundary &bnd);

private:
    bool FindInnerTurnPoints(Vec3 fromPt, double inDirection, CClose refClosePt, bool isTurnLineSameWay, const CBoundary &bnd);
    bool FindCurveTurnPoint(const CABCurve &thisCurve, const CBoundary &bnd);
    void FindABTurnPoint(Vec3 fromPt, const CABLine &ABLine, const CBoundary &bnd);

    bool AddABSequenceLines(const CABLine &ABLine, const CVehicle &vehicle);
    bool AddCurveSequenceLines(const CABCurve &curve, const CABCurve &nextCurve);

public:
    int GetLineIntersection(double p0x, double p0y, double p1x, double p1y,
                            double p2x, double p2y, double p3x, double p3y, double &iEast, double &iNorth);

private:
    QVector<Vec3> MoveTurnInsideTurnLine(QVector<Vec3> uTurnList,
                                         double head,
                                         bool deleteSecondHalf,
                                         bool invertHeading,
                                         CVehicle &vehicle,
                                         const CBoundary &bnd);

public:
    void SmoothYouTurn(int smPts);

    //called to initiate turn
    void YouTurnTrigger(const CTrack &trk, CVehicle &vehicle, CABLine &ABLine, CABCurve &curve);

    //Normal copmpletion of youturn
    void CompleteYouTurn(int &makeUTurnCounter);

    void Set_Alternate_skips();

    //something went seriously wrong so reset everything
    void ResetYouTurn(int &makeUTurnCounter);

    void ResetCreatedYouTurn(int &makeUturnCounter);

    void FailCreate();

    //build the points and path of youturn to be scaled and transformed
    void BuildManualYouLateral(bool isTurnLeft,
                               CVehicle &vehicle,
                               const CTrack &trk,
                               CABLine &ABLine,
                               CABCurve &curve);

    //build the points and path of youturn to be scaled and transformed
    void BuildManualYouTurn(bool isTurnLeft, bool isTurnButtonTriggered,
                            CVehicle &vehicle,
                            const CTrack &trk,
                            CABLine &ABLine,
                            CABCurve &curve
                            );

    //determine distance from youTurn guidance line
    bool DistanceFromYouTurnLine(CVehicle &v, CNMEA &pn, int &makeUTurnCounter);

    //Duh.... What does this do....
    void DrawYouTurn(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
signals:
    void TimedMessage(int,QString,QString);
    void outOfBounds();
    //void setTriggerSequence(bool);
    //void resetSequenceEventTriggers();
    void turnOffBoundAlarm();
    void uTurnReset();
    //void guidanceLineDistanceOff(int);
    //void guidanceLineSteerAngle(int);
    //void setLookaheadGoal(double);
};

#endif // YOUTURN_H
