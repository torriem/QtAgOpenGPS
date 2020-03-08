#ifndef CSECTION_H
#define CSECTION_H

#include <QObject>
#include <QVector>
#include <QSharedPointer>
#include "vec2.h"
#include "vec3.h"
#include "btnenum.h"

typedef QVector<QVector3D> TriangleList;

class CVehicle;
class CTool;

class CSection : public QObject
{
    Q_OBJECT
private:

public:
    //list of patch data individual triangles
    QSharedPointer<TriangleList> triangleList;

    //list of the list of patch data individual triangles for that entire section activity
    QVector<QSharedPointer<TriangleList>> patchList;

    //is this section on or off
    bool isSectionOn = false;
    bool isAllowedOn = false;
    bool isSectionRequiredOn = false;

    bool sectionOnRequest = false;
    bool sectionOffRequest = false;
    bool sectionOnOffCycle = false;

    int sectionOnTimer = 0;
    int sectionOffTimer = 0;

    //mapping
    bool isMappingOn = false;

    bool isMappingAllowedOn = false;
    bool isMappingRequiredOn = false;

    bool mappingOnRequest = false;
    bool mappingOffRequest = false;
    bool mappingOnOffCycle = false;
    int  mappingOnTimer = 0;
    int  mappingOffTimer = 0;

    double speedPixels = 0;


    //the left side is always negative, right side is positive
    //so a section on the left side only would be -8, -4
    //in the center -4,4  on the right side only 4,8
    //reads from left to right
    //   ------========---------||----------========---------
    //        -8      -4      -1  1         4      8
    // in (meters)

    double positionLeft = -4;
    double positionRight = 4;
    double sectionWidth = 0;

    double foreDistance = 0;

    //used by readpixel to determine color in pixel array
    int rpSectionWidth = 0;
    int rpSectionPosition = 0;

    //points in world space that start and end of section are in
    Vec3 leftPoint;
    Vec3 rightPoint;

    //used to determine left and right speed of section
    Vec3 lastLeftPoint;
    Vec3 lastRightPoint;

    //whether or not this section is in boundary, headland
    bool isInBoundary = true, isHydLiftInWorkArea = true;
    bool isInHeadlandArea = true;
    bool isLookOnInHeadland = true;

    int numTriangles = 0;

    //used to determine state of Manual section button - Off Auto On
    btnStates manBtnState=btnStates::Off; //default to Off (see enum in formgps.h)

    explicit CSection(QObject *parent = NULL);

    void turnMappingOn();
    void turnMappingOff(CTool &tool);
    void addMappingPoint(CTool &tool);
signals:
    void addToTotalArea(double);
    void addToUserArea(double);
};

#endif // CSECTION_H
