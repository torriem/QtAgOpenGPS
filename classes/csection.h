#ifndef CSECTION_H
#define CSECTION_H

#include <QVector>
#include <QSharedPointer>
#include "vec2.h"
#include "btnenum.h"

typedef QVector<QVector3D> TriangleList;

class FormGPS;
class CVehicle;

class CSection
{
private:
    FormGPS *mf;
    CVehicle *vehicle;

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

    //used by readpixel to determine color in pixel array
    int rpSectionWidth = 0;
    int rpSectionPosition = 0;

    //points in world space that start and end of section are in
    Vec2 leftPoint;
    Vec2 rightPoint;

    //used to determine left and right point
    Vec2 lastLeftPoint;
    Vec2 lastRightPoint;

    double sectionLookAhead = 6;

    //whether or not this section is in boundary
    bool isInsideBoundary = true;

    int numTriangles = 0;

    //used to determine state of Manual section button - Off Auto On
    btnStates manBtnState=btnStates::Off; //default to Off (see enum in formgps.h)

    CSection();

    void set_vehicle(CVehicle *v);

    void turnSectionOn();
    void turnSectionOff();
    void addPathPoint(double northing, double easting,
                      double cosHeading, double sinHeading);

};

#endif // CSECTION_H
