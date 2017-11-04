#ifndef CVEHICLE_H
#define CVEHICLE_H

#include "csection.h"
#include "vec2.h"
#include "common.h"
#include <QMatrix4x4>

#include <QOpenGLBuffer>

class QOpenGLContext;


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
    double toolWidth;
    double toolFarLeftPosition = 0;
    double toolFarLeftSpeed = 0;
    double toolFarRightPosition = 0;
    double toolFarRightSpeed = 0;

    double toolOverlap;
    double toolTrailingHitchLength, tankTrailingHitchLength;
    double toolOffset;

    double toolTurnOffDelay;
    double toolLookAhead;

    bool isToolTrailing;
    bool isToolBehindPivot;
    bool isSteerAxleAhead;

    //vehicle specific
    bool isPivotBehindAntenna;

    double antennaHeight;
    double antennaPivot;
    double wheelbase;
    double hitchLength;

    //how many individual sections
    int numOfSections;
    int numSuperSection;

    //used for super section off on
    int toolMinUnappliedPixels;
    bool isSuperSectionAllowedOn;
    bool areAllSectionBtnsOn = true;

    //read pixel values
    int rpXPosition;
    int rpWidth;

    //min vehicle speed allowed before turning shit off
    double slowSpeedCutoff = 0;

    //autosteer values
    double goalPointLookAhead;
    double maxSteerAngle;
    double maxAngularVelocity;

    double avgSpeed[10];//for average speed
    int ringCounter = 0;

    //headings
    double fixHeading = 0.0;

    //storage for the cos and sin of heading
    double cosSectionHeading = 1.0, sinSectionHeading = 0.0;
    double fixHeadingSection = 0.0, fixHeadingTank = 0.0;
    Vec2 pivotAxlePos;
    Vec2 toolPos;
    Vec2 tankPos;
    Vec2 hitchPos;

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

    CSection section[MAXSECTIONS+1];

    //tally counters for display
    double totalSquareMeters = 0, totalUserSquareMeters = 0, userSquareMetersAlarm = 0;

    //list of the list of patch data individual triangles for field sections
    QVector<QSharedPointer<QVector<QVector3D>>> patchSaveList;

    CVehicle();
    void drawVehicle(QOpenGLContext *c, QMatrix4x4 &modelview, const QMatrix4x4 &projection, bool drawSectionMarkers = true);
    void destroyGLBuffers();

public slots:
    void settingsChanged(); //notify us that settings changed so buffers need to be redone.
};

#endif // CVEHICLE_H
