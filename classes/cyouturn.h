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

class CABLine;
class CBoundary;
class CABCurve;
class CVehicle;
class QOpenGLFunctions;
class CVehicle;
class CNMEA;

//class QMatrix4x4;

class CYouTurn: public QObject
{
    Q_OBJECT
private:
    int A, B, C;
    bool isHeadingSameWay = true;

public:
    //triggered right after youTurnTriggerPoint is set
    bool isYouTurnTriggered;

    //turning right or left?
    bool isYouTurnRight;

    // Is the youturn button enabled?
    bool isYouTurnBtnOn;

    ///The following application settings:
    //bool isUsingDubinsTurn;
    //int rowSkipsWidth = 1;
    // distance from headland as offset where to start turn shape
    //int youTurnStartOffset;
    //guidance values
    //double distanceFromCurrentLine, triggerDistanceOffset, geoFenceDistance, dxAB, dyAB;

    double boundaryAngleOffPerpendicular;
    //int uTurnSmoothing;
    bool alternateSkips = false, previousBigSkip = true;
    int rowSkipsWidth2 = 3, turnSkips = 2;

    //guidance values
    double distanceFromCurrentLine, uturnDistanceFromBoundary, dxAB, dyAB;
    double distanceFromCurrentLineSteer, distanceFromCurrentLinePivot;
    double steerAngleGu, rEastSteer, rNorthSteer, rEastPivot, rNorthPivot;
    double pivotCurvatureOffset, lastCurveDistance = 10000;

    bool isTurnCreationTooClose = false, isTurnCreationNotCrossingError = false;

    //pure pursuit values
    Vec3 pivot = Vec3(0,0,0);
    Vec2 goalPointYT = Vec2(0,0);
    Vec2 radiusPointYT = Vec2(0,0);
    double steerAngleYT;
    double rEastYT, rNorthYT;
    double ppRadiusYT;

    //list of points for scaled and rotated YouTurn line
    QVector<Vec3> ytList;

    //for 3Pt turns - second turn
    QVector<Vec3> pt3ListSecondLine;
    //int uTurnStyle = 0;

    int pt3Phase = 0;
    Vec3 pt3TurnNewAB = Vec3(0, 0, 0);
    bool isLastFrameForward = true;

    //is UTurn pattern in or out of bounds
    bool isOutOfBounds = false;

    //sequence of operations of finding the next turn 0 to 3
    int youTurnPhase;

    Vec4 crossingCurvePoint;
    double crossingheading = 0;

    double iE = 0, iN = 0;
    int onA;

    //constructor
    explicit CYouTurn(QObject *parent = 0);

    //Finds the point where an AB Curve crosses the turn line
    bool findCurveTurnPoints(const CABCurve &curve, const CBoundary &bnd);

    /* moved to glm
    int getLineIntersection(double p0x, double p0y, double p1x, double p1y,
                            double p2x, double p2y, double p3x, double p3y,
                            double &iEast, double &iNorth);
    */

    void addSequenceLines(double head, Vec3 pivot);

    bool buildABLineDubinsYouTurn(CVehicle &vehicle,
                                  CBoundary &bnd,
                                  const CABLine &ABLine,
                                  bool isTurnRight);

    bool buildCurveDubinsYouTurn(CVehicle &vehicle,
                                  const CBoundary &bnd,
                                  const CABCurve &curve,
                                  bool isTurnRight,
                                  Vec3 pivotPos);

    void smoothYouTurn(int smPts);

    //called to initiate turn
    void youTurnTrigger(CVehicle &vehicle, CABLine &ABLine, CABCurve &curve);

    //Normal copmpletion of youturn
    void completeYouTurn();

    void setAlternateSkips();

    //something went seriously wrong so reset everything
    void resetYouTurn();

    void resetCreatedYouTurn();

    //build the points and path of youturn to be scaled and transformed
    void buildManualYouLateral(CVehicle &vehicle,
                               CABLine &ABLine, CABCurve &curve,
                            bool isTurnRight);
    void buildManualYouTurn(CVehicle &vehicle,
                            CABLine &ABLine, CABCurve &curve,
                            bool isTurnRight, bool isTurnButtonTriggered);

    //get list of points from txt shape file
    //void loadYouTurnShapeFromFile(QString filename);


    //determine distance from youTurn guidance line
    bool distanceFromYouTurnLine(CVehicle &v, CNMEA &pn);

    void check3PtSequence(void);

    //Duh.... What does this do....
    void drawYouTurn(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
signals:
    void showMessage(int,QString,QString);
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
