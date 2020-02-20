#ifndef CTOOL_H
#define CTOOL_H

#include <QString>

class QOpenGLFunctions;
class QMatrix4x4;
class CVehicle;
class CCamera;

class CTool
{
public:
    double toolWidth;
    double toolFarLeftPosition = 0;
    double toolFarLeftSpeed = 0;
    double toolFarRightPosition = 0;
    double toolFarRightSpeed = 0;
    //double toolFarLeftContourSpeed = 0, toolFarRightContourSpeed = 0;

    double toolOverlap;
    double toolTrailingHitchLength, toolTankTrailingHitchLength;
    double toolOffset;

    double toolTurnOffDelay;
    double toolLookAhead;

    bool isToolTrailing, isToolTBT;
    bool isToolBehindPivot;
    QString toolAttachType;

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

    CTool();
    void drawTool(CVehicle &v, CCamera &camera, QOpenGLFunctions *gl, QMatrix4x4 &mvp);
};

#endif // CTOOL_H
