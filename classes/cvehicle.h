#ifndef CVEHICLE_H
#define CVEHICLE_H

#include "csection.h"
#include "vec2.h"
#include "vec3.h"
#include "common.h"
#include <QObject>
#include <QMatrix4x4>

#include <QOpenGLBuffer>

class QOpenGLFunctions;
class CCamera;
class CYouTurn;
class CTool;
class CBoundary;
class CNMEA;
class CCamera;
class CABCurve;
class CABLine;
class CContour;

class CVehicle: public QObject
{
    Q_OBJECT
private:

public:
    bool isSteerAxleAhead;
    bool isPivotBehindAntenna;

    double antennaHeight;
    double antennaPivot;
    double wheelbase;
    double minTurningRadius;
    double antennaOffset, panicStopSpeed;
    int vehicleType;

    //min vehicle speed allowed before turning shit off
    double slowSpeedCutoff = 0;

    //autosteer values
    double goalPointLookAhead, goalPointLookAheadHold, goalPointLookAheadMult;

    double stanleyDistanceErrorGain, stanleyHeadingErrorGain;
    double minLookAheadDistance = 2.0;
    double maxSteerAngle, maxSteerSpeed, minSteerSpeed;
    double maxAngularVelocity;
    double hydLiftLookAheadTime, trackWidth;

    double hydLiftLookAheadDistanceLeft, hydLiftLookAheadDistanceRight;

    bool isHydLiftOn;
    double stanleyIntegralDistanceAwayTriggerAB, stanleyIntegralGainAB, purePursuitIntegralGain;

    //flag for free drive window to control autosteer
    bool isInFreeDriveMode;

    //the trackbar angle for free drive
    double driveFreeSteerAngle = 0;

    double modeXTE, modeActualXTE = 0, modeActualHeadingError = 0;
    int modeTime = 0;

    double functionSpeedLimit;


    //from pn or main form:
    double avgSpeed;//for average speed
    int ringCounter = 0;

    //headings
    double fixHeading = 0.0;

    //storage for the cos and sin of heading
    double cosSectionHeading = 1.0, sinSectionHeading = 0.0;
    double fixHeadingSection = 0.0, fixHeadingTank = 0.0;
    Vec3 pivotAxlePos;
    Vec3 steerAxlePos;
    Vec3 toolPos;
    Vec3 tankPos;
    Vec3 hitchPos;
    Vec2 guidanceLookPos;

    //Current fix positions
    double fixEasting = 0.0;
    double fixNorthing = 3.0;

    // autosteer variables for sending serial
    short int guidanceLineDistanceOff, guidanceLineSteerAngle;
    short int distanceDisplay;

    //how far travelled since last section was added, section points
    double sectionTriggerDistance = 0, sectionTriggerStepDistance = 0;
    Vec2 prevSectionPos;

    CSection sections[MAXSECTIONS];

    //tally counters for display
    double totalSquareMeters = 0, totalUserSquareMeters = 0, userSquareMetersAlarm = 0;

    double distancePivotToTurnLine;

    int modeTimeCounter = 0;
    double goalDistance = 0;

    //from Position.Designer.cs
    bool isReverse;



    void loadSettings();

    explicit CVehicle(QObject *parent = 0);
    double UpdateGoalPointDistance();
    void DrawVehicle(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection,
                     double steerAngle,
                     bool isFirstHeadingSet,
                     const CCamera &camera,
                     const CTool &tool,
                     CBoundary &bnd,
                     const CContour &ct,
                     const CABCurve &curve,
                     const CABLine &ABLine);




signals:
    //void setLookAheadGoal(double);

public slots:
    void AverageTheSpeed(double newSpeed);
    //void settingsChanged(); //notify us that settings changed so buffers need to be redone.
};

#endif // CVEHICLE_H
