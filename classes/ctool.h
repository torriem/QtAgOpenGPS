#ifndef CTOOL_H
#define CTOOL_H

#include <QString>
#include "csection.h"
#include "cpatches.h"
#include "common.h"
#include <QColor>
#include "qmlsectionbuttons.h"

class QOpenGLFunctions;
class QMatrix4x4;
class CVehicle;
class CCamera;
class CTram;

class CTool
{
public:
    ///---- in settings
    double width;
    ///----
    double halfWidth, contourWidth;
    double farLeftPosition = 0;
    double farLeftSpeed = 0;
    double farRightPosition = 0;
    double farRightSpeed = 0;

    ///---- in settings
    double overlap;
    double trailingHitchLength, tankTrailingHitchLength;
    double trailingToolToPivotLength;
    double offset;

    double lookAheadOffSetting, lookAheadOnSetting;
    double turnOffDelay;
    ///----

    double lookAheadDistanceOnPixelsLeft, lookAheadDistanceOnPixelsRight;
    double lookAheadDistanceOffPixelsLeft, lookAheadDistanceOffPixelsRight;

    ///---- in settings
    bool isToolTrailing, isToolTBT;
    bool isToolRearFixed, isToolFrontFixed;

    bool isMultiColoredSections, isSectionOffWhenOut;
    ///----

    QString toolAttachType;

    ///---- in settings
    double hitchLength;

    //how many individual sections
    int numOfSections;

    //used for super section off on
    int minCoverage;
    ///----

    bool areAllSectionBtnsOn = true;

    bool isLeftSideInHeadland = true, isRightSideInHeadland = true, isSectionsNotZones;

    //read pixel values
    int rpXPosition;

    int rpWidth;

    ///---- in settings
    QColor secColors[16];

    int zones;
    QVector<int> zoneRanges;
    ///----

    bool isDisplayTramControl;

    //moved the following from the main form to here
    CSection section[MAXSECTIONS+1];
    QMLSectionButtons sectionButtonState;

    //list of the list of patch data individual triangles for field sections
    QVector<QSharedPointer<PatchTriangleList>> patchSaveList;

    void sectionCalcWidths();
    void sectionCalcMulti();
    void sectionSetPositions();
    void loadSettings();

    CTool();
    //this class needs modelview and projection as separate matrices because some
    //additiona transformations need to be done.
    void DrawTool(QOpenGLFunctions *gl, QMatrix4x4 &modelview, QMatrix4x4 projection,
                  bool isJobStarted,
                  CVehicle &v, CCamera &camera, CTram &tram);
};

#endif // CTOOL_H
