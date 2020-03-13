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
class CHead;
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

    double treeSpacing;

    double hydLiftLookAheadDistanceLeft, hydLiftLookAheadDistanceRight;

    //used for super section off on
    bool isSuperSectionAllowedOn;
    bool areAllSectionBtnsOn = true;

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

    //Current fix positions
    double fixEasting = 0.0;
    double fixNorthing = 3.0;
    double fixZ = 0.0;
    double speed = 0.0;
    double altitude = 0.0;

    // autosteer variables for sending serial
    short int guidanceLineDistanceOff, guidanceLineSteerAngle;
    short int distanceDisplay;

    //how far travelled since last section was added, section points
    double sectionTriggerDistance = 0, sectionTriggerStepDistance = 0;
    Vec2 prevSectionPos;

    CSection sections[MAXSECTIONS];


    //tally counters for display
    double totalSquareMeters = 0, totalUserSquareMeters = 0, userSquareMetersAlarm = 0;

    explicit CVehicle(QObject *parent = 0);
    double updateGoalPointDistance(CNMEA &pn, double distanceFromCurrentLine);
    void drawVehicle(QOpenGLFunctions *gl, QMatrix4x4 modelview, QMatrix4x4 projection, const CCamera &camera, CBoundary &bnd, CHead &hd, const CContour &ct, const CABCurve &curve, const CABLine &ABLine);


signals:
    //void setLookAheadGoal(double);

public slots:
    void onNewSpeed(double);
    //void settingsChanged(); //notify us that settings changed so buffers need to be redone.
};

#endif // CVEHICLE_H
