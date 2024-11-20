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

class CABLine;
class CBoundary;
class CABCurve;
class CVehicle;
class QOpenGLFunctions;
class CVehicle;
class CNMEA;

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
    CABCurve nextCurve;
    CABLine nextLine;
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

    //constructor
    explicit CYouTurn(QObject *parent = 0);

    void loadSettings();

    //Finds the point where an AB Curve crosses the turn line
    bool BuildCurveDubinsYouTurn(CVehicle &vehicle,
                                  const CBoundary &bnd,
                                  const CABCurve &curve,
                                  bool isTurnRight,
                                  Vec3 pivotPos);

    bool BuildABLineDubinsYouTurn(bool isTurnLeft,
                                  const CBoundary &bnd,
                                  const CABLine &ABLine,
                                  );


private:
    bool CreateCurveOmegaTurn(bool isTurnLeft, Vec3 pivotPos,
                              int makeUTurnCounter,
                              CVehicle &vehicle,
                              const CBoundary &bnd,
                              const CABCurve &curve);
    bool CreateCurveWideTurn(bool isTurnLeft, Vec3 pivotPos,
                              int makeUTurnCounter,
                              CVehicle &vehicle,
                              const CBoundary &bnd,
                              const CABCurve &curve);

    bool CreateABOmegaTurn(bool isTurnLeft,                              int makeUTurnCounter,
                           CVehicle &vehicle,
                           const CBoundary &bnd,
                           const CABLine &ABLine);
    bool CreateABWideTurn(bool isTurnLeft,
                          int makeUTurnCounter,
                          CVehicle &vehicle,
                          const CBoundary &bnd,
                          const CABLine &ABLine);

    bool KStyleTurnCurve(bool isTurnLeft);
    bool KStyleTurnAB(bool isTurnLeft);

    QVector<Vec3> &MoveABTurnInsideTurnLine(QVector<Vec3> &uTurList, double head);

public:
    void FindClosestTurnPoint(Vec3 fromPt);
    bool FindCurveTurnPoints(const QVector<Vec3> &xList);
    bool FindCurveOutTurnPoint(CABCurve thisCurve, CABCurve &nextCurve, CClose inPt, bool isTurnLineSameWay);
    bool FindABOutTurnPoint(CABLine thisCurve, CABLine &nextCurve, CClose inPt, bool isTurnLineSameWay);

private:
    bool FindInnerTurnPoints(Vec3 fromPt, double inDirection, CClose refClosePt, bool isTurnLineSameWay);
    bool FindCurveTurnPoint(CABCurve thisCurve);
    void FindABTurnPoint(Vec3 fromPt);

    bool AddABSequenceLines();
    bool AddCurveSequenceLines();

public:
    int GetLineIntersection(double p0x, double p0y, double p1x, double p1y,
                            double p2x, double p2y, double p3x, double p3y, double &iEast, double &iNorth);

private:
    QVector<Vec3> MoveTurnInsideTurnLine(QVector<Vec3> uTurnList, double head, bool deleteSecondHalf, bool invertHeading,
                                         CVehicle &vehicle,
                                         const CBoundary &bnd);

public:
    void SmoothYouTurn(int smPts);

    //called to initiate turn
    void YouTurnTrigger(CVehicle &vehicle, CABLine &ABLine, CABCurve &curve);

    //Normal copmpletion of youturn
    void CompleteYouTurn();

    void Set_Alternate_skips();

    //something went seriously wrong so reset everything
    void ResetYouTurn();

    void ResetCreatedYouTurn();

    void FailCreate();

    //build the points and path of youturn to be scaled and transformed
    void BuildManualYouLateral(CVehicle &vehicle,
                               CABLine &ABLine, CABCurve &curve,
                            bool isTurnLeft);
    void BuildManualYouTurn(CVehicle &vehicle,
                            CABLine &ABLine, CABCurve &curve,
                            bool isTurnLeft, bool isTurnButtonTriggered);

    //determine distance from youTurn guidance line
    bool DistanceFromYouTurnLine(CVehicle &v, CNMEA &pn);

    //Duh.... What does this do....
    void DrawYouTurn(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
signals:
    void TimedMessage(int,QString,QString);
    void outOfBounds();
    void swapDirection();
    void setTriggerSequence(bool);
    void resetSequenceEventTriggers();
    void turnOffBoundAlarm();
    //void guidanceLineDistanceOff(int);
    //void guidanceLineSteerAngle(int);
    //void setLookaheadGoal(double);
};

#endif // YOUTURN_H
