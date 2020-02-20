#ifndef CVEHICLE_H
#define CVEHICLE_H

#include "csection.h"
#include "vec2.h"
#include "vec3.h"
#include "common.h"
#include <QMatrix4x4>

#include <QOpenGLBuffer>

class QOpenGLFunctions;
class CCamera;
class CYouTurn;
class CTool;
class CBoundary;


class CVehicle
{
private:
    QOpenGLBuffer trailingTankHitchBuffer;
    QOpenGLBuffer tankDotBuffer;
    QOpenGLBuffer hitchBuffer;
    QOpenGLBuffer toolHitchBuffer;
    QOpenGLBuffer superSectionBuffer;
    QOpenGLBuffer sectionBuffer[MAXSECTIONS+1];
    QOpenGLBuffer sectionDotsBuffer;
    QOpenGLBuffer vehicleBodyBuffer;
    QOpenGLBuffer pinsAndMarkersBuffer;
    QOpenGLBuffer pointerBuffer;
    QOpenGLBuffer rigidHitchBuffer;

    bool buffersCurrent = false;

    void makeBuffers();


public:
    bool isSteerAxleAhead;
    bool isPivotBehindAntenna;
    double antennaHeight;
    double antennaPivot;
    double wheelbase;
    double minTurningRadius;
    double antennaOffset;
    double slowSpeedCutoff = 0;
    int vehicleType;

    //autosteer values
    double  goalPointLookAheadSeconds, goalPointLookAheadMinimumDistance, goalPointDistanceMultiplier, goalPointLookAheadUturnMult;

    bool isStanleyUsed;
    double stanleyGain, stanleyHeadingErrorGain;
    double minLookAheadDistance = 2.0;
    double maxSteerAngle;
    double maxAngularVelocity;
    double treeSpacing;
    double hydLiftLookAhead;
    bool isHydLiftOn;

    //autosteer values
    double goalPointLookAhead;
    int youTurnDistance;
    int youTurnToolWidths;

    bool youTurnUseDubins;

    double youTurnTriggerDistance;

    //how many individual sections
    int numOfSections;
    int numSuperSection;

    //used for super section off on
    bool isSuperSectionAllowedOn;
    bool areAllSectionBtnsOn = true;

    //read pixel values
    int rpXPosition;
    int rpWidth;


    double avgSpeed[10];//for average speed
    int ringCounter = 0;

    //headings
    double fixHeading = 0.0;

    //storage for the cos and sin of heading
    double cosSectionHeading = 1.0, sinSectionHeading = 0.0;
    double fixHeadingSection = 0.0, fixHeadingTank = 0.0;
    Vec3 pivotAxlePos;
    Vec3 toolPos;
    Vec3 tankPos;
    Vec3 hitchPos;

    //Current fix positions
    double fixEasting = 0.0;
    double fixNorthing = 3.0;
    double fixZ = 0.0;
    double speed = 0.0;
    double altitude = 0.0;

    // autosteer variables for sending serial
    short int guidanceLineDistanceOff, guidanceLineSteerAngle;

    //how far travelled since last section was added, section points
    double sectionTriggerDistance = 0, sectionTriggerStepDistance = 0;
    Vec2 prevSectionPos;


    //tally counters for display
    double totalSquareMeters = 0, totalUserSquareMeters = 0, userSquareMetersAlarm = 0;

    //list of the list of patch data individual triangles for field sections
    QVector<QSharedPointer<QVector<QVector3D>>> patchSaveList;

    CVehicle();
    double updateGoalPointDistance(double distanceFromCurrentLine);
    void drawVehicle(QOpenGLFunctions *gl, QMatrix4x4 &mvp, CCamera &camera, CTool &tool, CBoundary &bnd);


public slots:
    //void settingsChanged(); //notify us that settings changed so buffers need to be redone.
};

#endif // CVEHICLE_H
