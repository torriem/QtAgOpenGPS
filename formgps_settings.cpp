#include "formgps.h"
#include "aogproperty.h"

void FormGPS::loadSettings()
{
    isMetric = property_setMenu_isMetric;

    //These properties will be accessed directly by QML
    //tramLinesMenuField.Visible = setFeatures.isTramOn;
    //headlandToolStripMenuItem.Visible = property_setFeature_isHeadlandOn;

    //boundariesToolStripMenuItem.Visible = property_setFeature_isBoundaryOn;
    //recordedPathStripMenu.Visible = property_setFeature_isRecPathOn;
    //SmoothABtoolStripMenu.Visible = property_setFeature_isABSmoothOn;
    //deleteContourPathsToolStripMenuItem.Visible = property_setFeature_isHideContourOn;
    //webcamToolStrip.Visible = property_setFeature_isWebCamOn;
    //offsetFixToolStrip.Visible = property_setFeature_isOffsetFixOn;
    //btnContour.Visible = property_setFeature_isContourOn;
    //btnAutoYouTurn.Visible = property_setFeature_isYouTurnOn;
    //btnStartAgIO.Visible = property_setFeature_isAgIOOn;

    //btnAutoSteer.Visible = property_setFeature_isAutoSteerOn;
    //btnCycleLines.Visible = property_setFeature_isCycleLinesOn;
    //btnCycleLinesBk.Visible = property_setFeature_isCycleLinesOn;
    //btnSectionMasterManual.Visible = property_setFeature_isManualSectionOn;
    //btnSectionMasterAuto.Visible = property_setFeature_isAutoSectionOn;
    //btnABLine.Visible = property_setFeature_isABLineOn;
    //btnCurve.Visible = property_setFeature_isCurveOn;
    //btnStanleyPure.Visible = property_setFeature_isSteerModeOn;

    isUTurnOn = property_setFeature_isUTurnOn;
    isLateralOn = property_setFeature_isLateralOn;

    if (isMetric)
    {
        inchOrCm2m = 0.01;
        m2InchOrCm = 100.0;

        m2FtOrM = 1.0;
        ftOrMtoM = 1.0;

        inOrCm2Cm = 1.0;
        cm2CmOrIn = 1.0;

        unitsFtM = " m";
        unitsInCm = " cm";
    }
    else
    {
        inchOrCm2m = glm::in2m;
        m2InchOrCm = glm::m2in;

        m2FtOrM = glm::m2ft;
        ftOrMtoM = glm::ft2m;

        inOrCm2Cm = 2.54;
        cm2CmOrIn = 0.394;

        unitsInCm = " in";
        unitsFtM = " ft";
    }

    pn.headingTrueDualOffset = property_setGPS_dualHeadingOffset;

    frameDayColor = property_setDisplay_colorDayFrame;
    frameNightColor = property_setDisplay_colorNightFrame;
    sectionColorDay = property_setDisplay_colorSectionsDay;
    fieldColorDay = property_setDisplay_colorFieldDay;
    fieldColorNight = property_setDisplay_colorFieldNight;

    //check color for 255, reset it to properties
    //Properties.Settings.Default.setDisplay_colorDayFrame = frameDayColor;
    //Properties.Settings.Default.setDisplay_colorNightFrame = frameNightColor;
    //Properties.Settings.Default.setDisplay_colorSectionsDay = sectionColorDay;
    //Properties.Settings.Default.setDisplay_colorFieldDay = fieldColorDay;
    //Properties.Settings.Default.setDisplay_colorFieldNight = fieldColorNight;

    isSkyOn = property_setMenu_isSkyOn;
    isTextureOn = property_setDisplay_isTextureOn;

    isGridOn = property_setMenu_isGridOn;
    isBrightnessOn = property_setDisplay_isBrightnessOn;

    isCompassOn = property_setMenu_isCompassOn;
    isSpeedoOn = property_setMenu_isSpeedoOn;
    isSideGuideLines = property_setMenu_isSideGuideLines;
    isSvennArrowOn = property_setDisplay_isSvennArrowOn;

    lightbarCmPerPixel = property_setDisplay_lightbarCmPerPixel;

    //isLogNMEA = property_setMenu_isLogNMEA;
    isPureDisplayOn = property_setMenu_isPureOn;

    isAutoStartAgIO = property_setDisplay_isAutoStartAgIO;

    vehicleOpacity = ((double)(property_setDisplay_vehicleOpacity) * 0.01);
    vehicleOpacityByte = (char)(255 * ((double)(property_setDisplay_vehicleOpacity) * 0.01));
    isVehicleImage = property_setDisplay_isVehicleImage;

    //TODO: custom colors for display

    //TODO: check for 255
    textColorDay = property_setDisplay_colorTextDay;
    textColorNight = property_setDisplay_colorTextNight;

    vehicleColor = property_setDisplay_colorVehicle;

    isLightbarOn = property_setMenu_isLightbarOn;


    //hotkeys = Properties.Settings.Default.setKey_hotkeys.ToCharArray();
    udpWatchLimit = property_setGPS_udpWatchMsec;
    //check for 255
    //TODO
    //string[] words = Properties.Settings.Default.setDisplay_customColors.Split(',');

    isRTK = property_setGPS_isRTK;
    isRTK_KillAutosteer = property_setGPS_isRTK_KillAutoSteer;

    pn.ageAlarm = property_setGPS_ageAlarm;

    isConstantContourOn = property_setAS_isConstantContourOn;
    isSteerInReverse = property_setAS_isSteerInReverse;

    guidanceLookAheadTime = property_setAS_guidanceLookAheadTime;

    //gyd pulls directly from settings
    //gyd.sideHillCompFactor = property_setAS_sideHillComp;

    fd.UpdateFieldBoundaryGUIAreas(bnd.bndList);

    isStanleyUsed = property_setVehicle_isStanleyUsed;
    isDay = property_setDisplay_isDayMode;

    if (tool.isSectionsNotZones){
        tool.sectionSetPositions();
        tool.sectionCalcWidths();
        LineUpIndividualSectionBtns();
    } else {
        tool.sectionCalcMulti();
        LineUpAllZoneButtons();
    }

    //disable youturn buttons
    headingFromSource = (QString)property_setGPS_headingFromWhichSource;


    //load various saved settings or properties into the support classes
    ahrs.loadSettings();
    camera.loadSettings();
    pn.loadSettings();
    sim.loadSettings();
    tool.loadSettings();
    vehicle.loadSettings();
    yt.loadSettings();
}
