#ifndef CTOOL_H
#define CTOOL_H

#include <QString>
#include "csection.h"
#include "common.h"

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

    double slowSpeedCutoff = 0;

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

    CSection section[MAXSECTIONS+1];

    //list of the list of patch data individual triangles for field sections
    QVector<QSharedPointer<QVector<QVector3D>>> patchSaveList;

    void sectionCalcWidths();
    CTool();
    void drawTool(CVehicle &v, CCamera &camera, QOpenGLFunctions *gl, QMatrix4x4 &modelview, QMatrix4x4 projection);
};

#endif // CTOOL_H
