#ifndef AOGSETTINGS_H
#define AOGSETTINGS_H

#include <QSettings>
#include <QVariant>

class AOGSettings : public QSettings
{
public:
    QVariant value(const QString &key, const QVariant &defaultvalue);
};

//Macros to use settings in a consistant way
#define USE_SETTINGS AOGSettings settings

//CTOOL
#define SETTINGS_TOOLWIDTH 				settings.   value("vehicle/toolWidth", 16).toDouble()
#define SETTINGS_SET_TOOLWIDTH(VAL)		settings.setValue("vehicle/toolWidth", VAL)

#define SETTINGS_TOOLOVERLAP 			settings.   value("vehicle/toolOverlap", 0).toDouble()
#define SETTINGS_SET_TOOLOVERLAP(VAL)	settings.setValue("vehicle/toolOverlap", VAL)

#define SETTINGS_TOOLOFFSET 			settings.   value("vehicle/toolOffset", 0).toDouble()
#define SETTINGS_SET_TOOLOFFSET(VAL)	settings.setValue("vehicle/toolOffset", VAL)

#define SETTINGS_TOOLTRAILINGHITCHLENGTH			settings.   value("vehicle/toolTrailingHitchLength", -2).toDouble()
#define SETTINGS_SET_TOOLTRAILINGHITCHLENGTH(VAL)	settings.setValue("vehicle/toolTrailingHitchLength", VAL)

#define SETTINGS_TOOLTANKTRAILINGHITCHLENGTH 			settings.   value("vehicle/toolTrailingHitchLength", 0).toDouble()
#define SETTINGS_SET_TOOLTANKTRAILINGHITCHLENGTH(VAL)	settings.setValue("vehicle/toolTrailingHitchLength", VAL)

#define SETTINGS_HITCHLENGTH 				settings.   value("vehicle/hitchLength", 0).toDouble()
#define SETTINGS_SET_HITCHLENGTH(VAL)		settings.setValue("vehicle/hitchLength", VAL)

#define SETTINGS_TOOLISBEHINDPIVOT 				settings.   value("vehicle/toolIsBehindPivot", true).toBool()
#define SETTINGS_SET_TOOLISBEHIND(PIVOTVAL)		settings.setValue("vehicle/toolIsBehindPivot", VAL)

#define SETTINGS_TOOLISTRAILING 				settings.   value("vehicle/toolIsTrailing", true).toBool()
#define SETTINGS_SET_TOOLISTRAILING(VAL)		settings.setValue("vehicle/toolIsTrailing", VAL)

#define SETTINGS_TOOLISTBT 				settings.   value("vehicle/toolIsTBT", false).toBool()
#define SETTINGS_SET_TOOLISTBT(VAL)		settings.setValue("vehicle/toolIsTBT", VAL)

#define SETTINGS_TOOLLOOKAHEAD 				settings.   value("vehicle/toolLookAhead", 0.1).toDouble()
#define SETTINGS_SET_TOOLLOOKAHEAD(VAL)		settings.setValue("vehicle/toolLookAhead", VAL)

#define SETTINGS_TOOLTURNOFFDELAY 				settings.   value("vehicle/toolTurnOffDelay", 1).toDouble()
#define SETTINGS_SET_TOOLTURNOFFDELAY(VAL)		settings.setValue("vehicle/toolTurnOffDelay", VAL)

#define SETTINGS_NUMSECTIONS 			settings.   value("vehicle/numSections", 3).toInt()
#define SETTINGS_SET_NUMSECTIONS(VAL)		settings.setValue("vehicle/numSections", VAL)

#define SETTINGS_TOOLMINAPPLIED				settings.   value("vehicle/minApplied", 10).toInt()
#define SETTINGS_SET_TOOLMINAPPLIED(VAL)	settings.setValue("vehicle/minApplied", VAL)

//CTRAM
#define SETTINGS_TRAM_EQWIDTH 				settings.   value("tram/eqWidth", 24).toDouble()
#define SETTINGS_SET_TRAM_EQWIDTH(VAL)		settings.setValue("tram/eqWidth", VAL)

#define SETTINGS_TRAM_WHEELSPACING 			settings.   value("tram/wheelSpacing",1.7).toDouble()
#define SETTINGS_SET_TRAM_WHEELSPACING(VAL)	settings.setValue("tram/wheelSpacing",VAL)

#define SETTINGS_TRAM_PASSES				settings.   value("tram/passes", 0).toInt()
#define SETTINGS_SET_TRAM_PASSES(VAL)		settings.setValue("tram/passes",VAL)

//CVEHICLE
#define SETTINGS_VEHICLE_ISPIVOTBEHINDANTENNA			settings.   value("vehicle/isPivotBehindAntenna", true).toBool()
#define SETTINGS_SET_VEHICLE_ISPIVOTBEHINDANTENNA(VAL)	settings.setValue("vehicle/isPivotBehindAntenna",VAL)

#define SETTINGS_VEHICLE_ANTENNAHEIGHT			settings.   value("vehicle/antennaHeight", 3).toDouble()
#define SETTINGS_SET_VEHICLE_ANTENNAHEIGHT(VAL)	settings.setValue("vehicle/antennaHeight",VAL)

#define SETTINGS_VEHICLE_ANTENNAPIVOT			settings.   value("vehicle/antennaPivot", 0.1).toDouble()
#define SETTINGS_SET_VEHICLE_ANTENNAPIVOT(VAL)	settings.setValue("vehicle/antennaPivot",VAL)

#define SETTINGS_VEHICLE_ANTENNAOFFSET			settings.   value("vehicle/antennaOffset", 0).toDouble()
#define SETTINGS_SET_VEHICLE_ANTENNAOFFSET(VAL)	settings.setValue("vehicle/antennaOffset",VAL)

#define SETTINGS_VEHICLE_WHEELBASE			settings.   value("vehicle/wheelbase", 2.5).toDouble()
#define SETTINGS_SET_VEHICLE_WHEELBASE(VAL)	settings.setValue("vehicle/wheelbase",VAL)

#define SETTINGS_VEHICLE_MINTURNINGRADIUS			settings.   value("vehicle/isPivotBehindAntenna", true).toBool()
#define SETTINGS_SET_VEHICLE_MINTURNINGRADIUS(VAL)	settings.setValue("vehicle/isPivotBehindAntenna",VAL)

#define SETTINGS_VEHICLE_ISSTEERAXLEAHEAD			settings.   value("vehicle/isSteerAxleAhead", true).toBool()
#define SETTINGS_SET_VEHICLE_ISSTEERAXLEAHEAD(VAL)	settings.setValue("vehicle/isSteerAxleAhead",VAL)

#define SETTINGS_VEHICLE_SLOWSPEEDCUTOFF			settings.   value("vehicle/slowSpeedCuttoff", 0).toDouble()
#define SETTINGS_SET_VEHICLE_SLOWSPEEDCUTOFF(VAL)	settings.setValue("vehicle/slowSpeedCuttoff",VAL)

#define SETTINGS_VEHICLE_LOOKAHEAD				settings.   value("vehicle/goalPointLookAhead", 3).toDouble()
#define SETTINGS_SET_VEHICLE_LOOKAHEAD(VAL)		settings.setValue("vehicle/goalPointLookAhead",VAL)

#define SETTINGS_VEHICLE_LOOKAHEADMINIMUM		settings.   value("vehicle/lookAheadMinimum", 2).toDouble()
#define SETTINGS_SET_VEHICLE_LOOKAHEADMINIMUM(VAL)	settings.setValue("vehicle/lookAheadMinimum",VAL)

#define SETTINGS_VEHICLE_DISTANCEMULTIPLIER			settings.   value("vehicle/lookAheadDistanceFromLine", 0.1).toDouble()
#define SETTINGS_SET_VEHICLE_DISTANCEMULTIPLIER(VAL)	settings.setValue("vehicle/lookAheadDistanceFromLine",VAL)

#define SETTINGS_VEHICLE_LOOKAHEADUTURNMULT			settings.   value("vehicle/lookAheadUturnMult", 0.8).toDouble()
#define SETTINGS_SET_VEHICLE_LOOKAHEADUTURNMULT(VAL)	settings.setValue("vehicle/lookAheadUturnMult",VAL)

#define SETTINGS_VEHICLE_ISSTANLEYUSED			settings.   value("vehicle/isStanleyUsed", true).toBool()
#define SETTINGS_SET_VEHICLE_ISSTANLEYUSED(VAL)	settings.setValue("vehicle/isStanleyUsed",VAL)

#define SETTINGS_VEHICLE_STANLEY_GAIN			settings.   value("vehicle/stanleyGain", 1).toDouble()
#define SETTINGS_SET_VEHICLE_STANLEY_GAIN(VAL)	settings.setValue("vehicle/stanleyGain",VAL)

#define SETTINGS_VEHICLE_STANLEYHEADINGERRORGAIN			settings.   value("vehicle/stanleyHeadingErrorGain", 1).toDouble()
#define SETTINGS_SET_VEHICLE_STANLEYHEADINGERRORGAIN(VAL)	settings.setValue("vehicle/stanleyHeadingErrorGain",VAL)

#define SETTINGS_VEHICLE_MAXANGULARVELOCITY				settings.   value("vehicle/maxAngularVelocity", 7).toDouble()
#define SETTINGS_SET_VEHICLE_MAXANGULARVELOCITY(VAL)	settings.setValue("vehicle/maxAngularVelocity",VAL)

#define SETTINGS_VEHICLE_MAXSTEERANGLE			settings.   value("vehicle/maxSteerAngle", 40).toDouble();
#define SETTINGS_SET_VEHICLE_MAXSTEERANGLE(VAL)	settings.setValue("vehicle/maxSteerAngle",VAL)

#define SETTINGS_VEHICLE_ISHYDLIFTON			settings.   value("vehicle/isHydLiftOn", false).toBool()
#define SETTINGS_SET_VEHICLE_ISHYDLIFTON(VAL)	settings.setValue("vehicle/stanleyHeadingErrorGain",VAL)

#define SETTINGS_VEHICLE_VEHICLETYPE			settings.   value("vehicle/vehicleType", 0).toInt()
#define SETTINGS_SET_VEHICLE_VEHICLETYPE(VAL)	settings.setValue("vehicle/vehicleType",VAL)

#define SETTINGS_VEHICLE_HYDLIFTLOOKAHEAD			settings.   value("vehicle/hydraulicLiftLookAhead", 2).toDouble()
#define SETTINGS_SET_VEHICLE_HYDLIFTLOOKAHEAD(VAL)	settings.setValue("vehicle/hydraulicLiftLookAhead",VAL)

#define SETTINGS_VEHICLE_NAME			settings.   value("vehicle/name", "unnamed").toQString()
#define SETTINGS_SET_VEHICLE_NAME(VAL)	settings.setValue("vehicle/name",VAL)

//CABLINE
#define SETTINGS_DISPLAY_LINEWIDTH			settings.   value("display/lineWidth", 2).toInt()
#define SETTINGS_SET_DISPLAY_LINEWIDTH(VAL)	settings.setValue("display/lineWidth",VAL)

//CFONT
#define SETTINGS_DISPLAY_ISFONTON			settings.   value("display/isFontOn", true).toBool()
#define SETTINGS_SET_DISPLAY_ISFONTON(VAL)	settings.setValue("display/isFontOn",VAL)

//CYOUTURN
#define SETTINGS_VEHICLE_YOUTURNTRIGGERDISTANCE			settings.   value("vehicle/youTriggerDistance", 8).toDouble()
#define SETTINGS_SET_VEHICLE_YOUTURNTRIGGERDISTANCE(VAL)	settings.setValue("vehicle/youTriggerDistance",VAL)

#define SETTINGS_VEHICLE_GEOFENCEDIST			settings.   value("vehicle/geoFenceDistance", 0).toDouble()
#define SETTINGS_SET_VEHICLE_GEOFENCEDIST(VAL)	settings.setValue("vehicle/geoFenceDistance",VAL)

#define SETTINGS_VEHICLE_YOUTURNDISTANCE			settings.   value("vehicle/youTurnDistance", 5).toDouble()
#define SETTINGS_SET_VEHICLE_YOUTURNDISTANCE(VAL)	settings.setValue("vehicle/youTurnDistance",VAL)

#define SETTINGS_VEHICLE_YOUSKIPWIDTH			settings.   value("vehicle/youSkipWidth", 1).toInt()
#define SETTINGS_SET_VEHICLE_YOUSKIPWIDTH(VAL)	settings.setValue("vehicle/youSkipWidth",VAL)

#define SETTINGS_VEHICLE_ISUSINGDUBINSTURN			settings.   value("vehicle/isUsingDubins", true).toBool()
#define SETTINGS_SET_VEHICLE_ISUSINGDUBINSTURN(VAL)	settings.setValue("vehicle/isUsingDubins",VAL)

//CNMEA / GPS / IMU
#define SETTINGS_GPS_FIXFROMWHICH			settings.   value("gps/fixFromWhichSentence", QString("GGA")).toString()
#define SETTINGS_SET_GPS_FIXFROMWHICH(VAL)	settings.setValue("gps/fixFromWhichSentence",VAL)

#define SETTINGS_GPS_ISROLLFROMGPS			settings.   value("gps/isRollFromGPS", false).toBool()
#define SETTINGS_SET_GPS_ISROLLFROMGPS(VAL)	settings.setValue("gps/isRollFromGPS",VAL)

#define SETTINGS_GPS_ISHEADINGFROMPAOGI			settings.   value("gps/isHeadingFromPAOGI", false).toBool()
#define SETTINGS_SET_GPS_HEADINGFROMPAOGI(VAL)	settings.setValue("gps/isHeadingFromPAOGI",VAL)

#define SETTINGS_GPS_BRICKUID			settings.   value("gps/brickUID", QString("68wESU")).toString()
#define SETTINGS_SET_GPS_BRICKUID(VAL)	settings.setValue("gps/brickUID",VAL)

#define SETTINGS_GPS_ISROLLFROMAUTOSTEER			settings.   value("gps/isRollFromAutosteer", false).toBool()
#define SETTINGS_SET_GPS_ISROLLFROMAUTOSTEER(VAL)	settings.setValue("gps/isRollFromAutosteer",VAL)

#define SETTINGS_GPS_ISHEADINGFROMBRICK			settings.   value("gps/isHeadingFromBrick", false).toBool();
#define SETTINGS_SET_GPS_ISHEADINGFROMBRICK(VAL)	settings.setValue("gps/isHeadingFromBrick",VAL)

#define SETTINGS_GPS_ISHEADINGFROMAUTOSTEER			settings.   value("gps/isHeadingFromAutoSteer", false).toBool();
#define SETTINGS_SET_GPS_ISHEADINGFROMAUTOSTEER(VAL)	settings.setValue("gps/isHeadingFromAutoSteer",VAL)

#define SETTINGS_GPS_ISROLLFROMBRICK			settings.   value("gps/isRollFromBrick", false).toBool();
#define SETTINGS_SET_GPS_ISROLLFROMBRICK(VAL)	settings.setValue("gps/isRollFromBrick",VAL)

#define SETTINGS_GPS_ISHEADINGFROMEXTUDP			settings.   value("gps/isHeadingFromExtUdp", false).toBool();
#define SETTINGS_SET_GPS_ISHEADINGFROMEXTUDP(VAL)	settings.setValue("gps/isHeadingFromExtUdp",VAL)

#define SETTINGS_GPS_ISROLLFROMEXTUDP			settings.   value("gps/isRollFromExtUdp", false).toBool();
#define SETTINGS_SET_GPS_ISROLLFROMEXTUDP(VAL)	settings.setValue("gps/isRollFromExtUdp",VAL)

#define SETTINGS_GPS_IMUPITCHZEROX16			settings.   value("gps/IMUPitchZeroX16", 0).toInt();
#define SETTINGS_SET_GPS_IMUPITCHZEROX16(VAL)	settings.setValue("gps/IMUPitchZeroX16",VAL)

#define SETTINGS_GPS_IMUROLLZEROX16				settings.   value("gps/IMURollZeroX16", 0).toInt();
#define SETTINGS_SET_GPS_IMUROLLZEROX16(VAL)	settings.setValue("gps/IMURollZeroX16",VAL)

//DISPLAY/GUI/MENU
#define SETTINGS_DISPLAY_ISPUREON			settings.   value("display/isPureOn", true).toBool()
#define SETTINGS_SET_DISPLAY_ISPUREON(VAL)	settings.setValue("display/isPureOn",VAL)

#define SETTINGS_DISPLAY_CAMPITCH	settings.   value("display/camPitch", -32).toDouble()
#define SETTINGS_SET_CAMPITCH(VAL)	settings.setValue("display/camPitch",VAL)


//AUTOSTEER
#define SETTINGS_AUTOSTEER_KO			settings.   value("autosteer/Ko", 5).toInt()
#define SETTINGS_SET_AUTOSTEER_KO(VAL)	settings.setValue("autosteer/Ko",VAL)

#define SETTINGS_AUTOSTEER_KP			settings.   value("autosteer/Kp", 5).toInt()
#define SETTINGS_SET_AUTOSTEER_KP(VAL)	settings.setValue("autosteer/Kp",VAL)

#define SETTINGS_AUTOSTEER_KI			settings.   value("autosteer/Ki", 0).toInt()
#define SETTINGS_SET_AUTOSTEER_KI(VAL)	settings.setValue("autosteer/Ki",VAL)

#define SETTINGS_AUTOSTEER_KD			settings.   value("autosteer/Kd", 0).toInt()
#define SETTINGS_SET_AUTOSTEER_KD(VAL)	settings.setValue("autosteer/Kd",VAL)

#define SETTINGS_AUTOSTEER_STEERINGANGLEOFFSET			settings.   value("autosteer/steeringAngleOffset", 127).toInt()
#define SETTINGS_SET_AUTOSTEER_STEERINGANGLEOFFSET(VAL)	settings.setValue("autosteer/steeringAngleOffset",VAL)

#define SETTINGS_AUTOSTEER_MINSTEERPWM			settings.   value("autosteer/minSteerPWM", 10).toInt()
#define SETTINGS_SET_AUTOSTEER_MINSTEERPWM(VAL)	settings.setValue("autosteer/minSteerPWM",VAL)

#define SETTINGS_AUTOSTEER_MAXINTEGRAL			settings.   value("autosteer/maxIntegral", 20).toInt()
#define SETTINGS_SET_AUTOSTEER_MAXINTEGRAL(VAL)	settings.setValue("autosteer/maxIntegral",VAL)

#define SETTINGS_AUTOSTEER_COUNTSPERDEGREE			settings.   value("autosteer/countsPerDegree", 10).toInt()
#define SETTINGS_SET_AUTOSTEER_COUNTSPERDEGREE(VAL)	settings.setValue("autosteer/countsPerDegree",VAL)

#define SETTINGS_AUTOSTEER_YOUTURNSHAPEFILE			settings.   value("autosteer/youTurnShape", QString("keyhold.txt")).toString()
#define SETTINGS_SET_AUTOSTEER_YOUTURNSHAPEFILE(VAL)	settings.setValue("autosteer/youTurnShape",VAL)

#define SETTINGS_AUTOSTEER_SNAPDISTANCE				settings.   value("autosteer/snapDistance", 40).toInt()
#define SETTINGS_SET_AUTOSTEER_SNAPDISTANCE(VAL)	settings.setValue("autosteer/snapDistance",VAL)

#define SETTINGS_AUTOSTEER_AUTOON			settings.   value("autosteer/autoOn", false).toBool()
#define SETTINGS_SET_AUTOSTEER_AUTOON(VAL)	settings.setValue("autosteer/autoOn",VAL)

#define SETTINGS_AUTOSTEER_ISJRK			settings.   value("autosteer/isJRK", false).toBool()
#define SETTINGS_SET_AUTOSTEER_ISJRK(VAL)	settings.setValue("autosteer/isJRK",VAL)
#endif // AOGSETTINGS_H

