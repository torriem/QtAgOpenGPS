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
class CTurn;
class CTool;
class CGeoFence;
class CMazeGrid;
class CNMEA;

//class QMatrix4x4;

class CYouTurn: public QObject
{
    Q_OBJECT
private:
    int A, B, C;
    bool isABSameAsFixHeading = true, isOnRightSideCurrentLine = true;
    int numShapePoints;

public:
    //triggered right after youTurnTriggerPoint is set
    bool isYouTurnTriggered;

    //turning right or left?
    bool isYouTurnRight, isLastToggle;

    //What was the last successful you turn direction?
    bool isLastYouTurnRight;

    //if not in workArea but in bounds, then we are on headland
    bool isInWorkArea, isInBoundz;

    //controlled by user in GUI to en/dis able
    bool isRecordingCustomYouTurn;

    // Is the youturn button enabled?
    bool isYouTurnBtnOn;

    //Patterns or Dubins
    bool isUsingDubinsTurn;

    double boundaryAngleOffPerpendicular;
    double distanceTurnBeforeLine = 0, tangencyAngle;

    int rowSkipsWidth = 1;

    // distance from headland as offset where to start turn shape
    int youTurnStartOffset;

    //guidance values
    double distanceFromCurrentLine, triggerDistanceOffset, geoFenceDistance, dxAB, dyAB;
    bool isTurnCreationTooClose = false, isTurnCreationNotCrossingError = false;

    //pure pursuit values
    Vec3 pivot;
    Vec2 goalPointYT;
    Vec2 radiusPointYT;
    double steerAngleYT;
    double rEastYT, rNorthYT;
    double ppRadiusYT;
    double minLookAheadDistance;

    //list of points for scaled and rotated YouTurn line
    QVector<Vec3> ytList;

    //list of points read from file, this is the actual pattern from a bunch of sources possible
    QVector<Vec2> youFileList;

    //to try and pull a UTurn back in bounds
    double turnDistanceAdjuster;

    //is UTurn pattern in or out of bounds
    bool isOutOfBounds = false;

    //sequence of operations of finding the next turn 0 to 3
    int youTurnPhase, curListCount;

    Vec4 crossingCurvePoint;
    Vec4 crossingTurnLinePoint;

    //list of points of collision path avoidance
    QVector<Vec3> mazeList;

    int onA;

    //moved from FormGPS to here since we are the only ones who use it,
    //other than the display
    double distancePivotToTurnLine;

    //constructor
    explicit CYouTurn(QObject *parent = 0);

    //Finds the point where an AB Curve crosses the turn line
    bool findCurveTurnPoints(const CABCurve &curve, const CBoundary &bnd, CTurn &turn);

    void addSequenceLines(double head, Vec3 pivot);

    bool buildDriveAround(const CVehicle &vehicle, const CABLine &ABLine,
                          CGeoFence &gf, CTurn &turn,
                          const CBoundary &bnd, CMazeGrid &mazeGrid,
                          double minFieldX, double minFieldY);

    bool buildABLineDubinsYouTurn(const CVehicle &vehicle,
                                  const CTool &tool,
                                  const CBoundary &bnd,
                                  CGeoFence &gf,
                                  const CABLine &ABLine, CTurn &turn,
                                  CMazeGrid &mazeGrid,
                                  double minFieldX, double minFieldY,
                                  bool isTurnRight);

    bool buildABLinePatternYouTurn(const CVehicle &vehicle,
                                   const CTool &tool,
                                   const CBoundary &bnd,
                                   const CABLine &ABLine, CTurn &turn,
                                   bool isTurnRight);

    bool buildCurvePatternYouTurn(const CVehicle &vehicle,
                                  const CTool &tool,
                                  const CBoundary &bnd,
                                  const CABCurve &curve, CTurn &turn,
                                  bool isTurnRight, Vec3 pivotPos);

    bool buildCurveDubinsYouTurn(const CVehicle &vehicle,
                                 const CTool &tool,
                                 const CBoundary &bnd,
                                 const CABCurve &curve, CTurn &turn,
                                 bool isTurnRight, Vec3 pivotPos);

    //called to initiate turn
    void youTurnTrigger();

    //Normal copmpletion of youturn
    void completeYouTurn();

    //something went seriously wrong so reset everything
    void resetYouTurn();

    void resetCreatedYouTurn();

    //get list of points from txt shape file
    void loadYouTurnShapeFromFile(QString filename);

    //build the points and path of youturn to be scaled and transformed
    void buildManualYouTurn(const CVehicle &vehicle, CTool &tool,
                            const CABLine &ABLine, CABCurve &curve,
                            bool isTurnRight, bool isTurnButtonTriggered);

    //determine distance from youTurn guidance line
    void distanceFromYouTurnLine(CVehicle &v, CNMEA &pn);

    //Duh.... What does this do....
    void drawYouTurn(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
signals:
    void showMessage(int,QString,QString);
    void outOfBounds();
    void swapDirection();
    void triggerSequence();
    void resetSequence();
    void turnOffBoundingAlarm();
    void guidanceLineDistanceOff(int);
    void guidanceLineSteerAngle(int);
    void setLookaheadGoal(double);
};

#endif // YOUTURN_H
