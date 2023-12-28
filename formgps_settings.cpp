#include "formgps.h"
#include "aogproperty.h"

void FormGPS::loadSettings()
{
    guidanceLookAheadTime = property_setAS_guidanceLookAheadTime;
    //hotkeys = Properties.Settings.Default.setKey_hotkeys.ToCharArray();
    //TODO isUTurnOn = property_setFeatures_isUTurnOn;
    //TODO isLateralOn = property_setFeatures.isLateralOn;
    udpWatchLimit = property_SetGPS_udpWatchMsec;
    //check for 255
    frameDayColor = property_setDisplay_colorDayFrame;
    frameNightColor = property_setDisplay_colorNightFrame;
    sectionColorDay = property_setDisplay_colorSectionsDay;
    fieldColorDay = property_setDisplay_colorFieldDay;
    fieldColorNight = property_setDisplay_colorFieldNight;

    //TODO
    //string[] words = Properties.Settings.Default.setDisplay_customColors.Split(',');

    isRTK = property_setGPS_isRTK;
    isRTK_KillAutosteer = property_setGPS_isRTK_KillAutoSteer;
    isConstantContourOn = property_setAS_isConstantContourOn;
    isSteerInReverse = property_setAS_isSteerInReverse;
    guidanceLookAheadTime = property_setAS_guidanceLookAheadTime;
    isStanleyUsed = property_setVehicle_isStanleyUsed;
    isDay = property_setDisplay_isDayMode;

    //defaultSectionWidth = property_setTool_sectionWidthMulti;

    //load various saved settings or properties into the support classes
    ahrs.loadSettings();
    camera.loadSettings();
    pn.loadSettings();
    sim.loadSettings();
    tool.loadSettings();
    vehicle.loadSettings();
    yt.loadSettings();
}
