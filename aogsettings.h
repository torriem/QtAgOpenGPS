#ifndef AOGSETTINGS_H
#define AOGSETTINGS_H

#include <QSettings>
#include <QVariant>
#include <QList>
#include <QVector3D>
#include <QColor>

class AOGSettings : public QSettings
{
public:
    QVariant value(const QString &key, const QVariant &defaultvalue);
};

QColor parseColor(QString setcolor);
QVector3D parseColorVector(QString setcolor);
int colorSettingStringToInt(QString colorSettingString);

//Macros to use settings in a consistant way
#define USE_SETTINGS AOGSettings settings
#define SYNC_SETTINGS settings.sync()

//HEADLAND
#define SETTINGS_HEADLAND_ISSECTIONCONTROLLED          settings.   value("headland/isSectionControlled", false).toBool()
#define SETTINGS_SET_HEADLAND_ISSECTIONCONTROLLED(VAL) settings.setValue("headland/isSectionControlled",VAL)

// TOOL-related settings
#define SETTINGS_TOOL_NAME			settings.   value("tool/name", "unnamed").toQString()
#define SETTINGS_SET_TOOL_NAME(VAL)	settings.setValue("tool/name",VAL)

#define SETTINGS_TOOL_WIDTH 				settings.   value("tool/width", 16).toDouble()
#define SETTINGS_SET_TOOL_WIDTH(VAL)		settings.setValue("tool/width", VAL)

#define SETTINGS_TOOL_OVERLAP 			settings.   value("tool/overlap", 0).toDouble()
#define SETTINGS_SET_TOOL_OVERLAP(VAL)	settings.setValue("tool/overlap", VAL)

#define SETTINGS_TOOL_OFFSET 			settings.   value("tool/offset", 0).toDouble()
#define SETTINGS_SET_TOOL_OFFSET(VAL)	settings.setValue("tool/offset", VAL)

#define SETTINGS_TOOL_TRAILINGHITCHLENGTH			settings.   value("tool/trailingHitchLength", -2).toDouble()
#define SETTINGS_SET_TOOL_TRAILINGHITCHLENGTH(VAL)	settings.setValue("tool/trailingHitchLength", VAL)

#define SETTINGS_TOOL_TANKTRAILINGHITCHLENGTH 			settings.   value("tool/tankTrailingHitchLength", 0).toDouble()
#define SETTINGS_SET_TOOL_TANKTRAILINGHITCHLENGTH(VAL)	settings.setValue("tool/tankTrailingHitchLength", VAL)

#define SETTINGS_TOOL_HITCHLENGTH 				settings.   value("tool/hitchLength", 0).toDouble()
#define SETTINGS_SET_TOOL_HITCHLENGTH(VAL)		settings.setValue("tool/hitchLength", VAL)

#define SETTINGS_TOOL_ISBEHINDPIVOT 				settings.   value("tool/isBehindPivot", true).toBool()
#define SETTINGS_SET_TOOL_ISBEHINDPIVOT(VAL)		settings.setValue("tool/isBehindPivot", VAL)

#define SETTINGS_TOOL_ISTRAILING 				settings.   value("tool/isTrailing", true).toBool()
#define SETTINGS_SET_TOOL_ISTRAILING(VAL)		settings.setValue("tool/isTrailing", VAL)

#define SETTINGS_TOOL_ISTBT 				settings.   value("tool/isTBT", false).toBool()
#define SETTINGS_SET_TOOL_ISTBT(VAL)		settings.setValue("tool/isTBT", VAL)

#define SETTINGS_TOOL_LOOKAHEADON 				settings.   value("tool/lookAheadOn", 1).toDouble()
#define SETTINGS_SET_TOOL_LOOKAHEADON(VAL)		settings.setValue("tool/lookAheadOn", VAL)

#define SETTINGS_TOOL_LOOKAHEADOFF 				settings.   value("tool/lookAheadOff", 0.5).toDouble()
#define SETTINGS_SET_TOOL_LOOKAHEADOFF(VAL)		settings.setValue("tool/lookAheadOff", VAL)

#define SETTINGS_TOOL_OFFDELAY 				settings.   value("tool/offDelay", 0).toDouble()
#define SETTINGS_SET_TOOL_OFFDELAY(VAL)		settings.setValue("tool/offDelay", VAL)

#define SETTINGS_TOOL_NUMSECTIONS 			settings.   value("tool/numSections", 3).toInt()
#define SETTINGS_SET_TOOL_NUMSECTIONS(VAL)		settings.setValue("tool/numSections", VAL)

#define SETTINGS_TOOL_MINAPPLIED				settings.   value("tool/minApplied", 10).toInt()
#define SETTINGS_SET_TOOL_MINAPPLIED(VAL)	settings.setValue("tool/minApplied", VAL)

#define SETTINGS_TOOL_ISWORKSWITCHENABLED 				settings.   value("tool/isWorkSwitchEnabled", false).toBool()
#define SETTINGS_SET_TOOL_ISWORKSWITCHENABLED(VAL)		settings.setValue("tool/isWorkSwitchEnabled", VAL)

#define SETTINGS_TOOL_ISWORKSWITCHACTIVELOW 				settings.   value("tool/isWorkSwitchActiveLow", false).toBool()
#define SETTINGS_SET_TOOL_ISWORKSWITCHACTIVELOW(VAL)		settings.setValue("tool/isWorkSwitchActiveLow", VAL)

#define SETTINGS_TOOL_ISWORKSWITCHMANUAL 				settings.   value("tool/isWorkSwitchManual", false).toBool()
#define SETTINGS_SET_TOOL_ISWORKSWITCHMANUAL(VAL)		settings.setValue("tool/isWorkSwitchManual", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION1 				settings.   value("tool/sectionPosition1", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION1(VAL)		settings.setValue("tool/sectionPosition1", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION2 				settings.   value("tool/sectionPosition2", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION2(VAL)		settings.setValue("tool/sectionPosition2", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION3 				settings.   value("tool/sectionPosition3", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION3(VAL)		settings.setValue("tool/sectionPosition3", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION4 				settings.   value("tool/sectionPosition4", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION4(VAL)		settings.setValue("tool/sectionPosition4", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION5 				settings.   value("tool/sectionPosition5", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION5(VAL)		settings.setValue("tool/sectionPosition5", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION6 				settings.   value("tool/sectionPosition6", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION6(VAL)		settings.setValue("tool/sectionPosition6", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION7 				settings.   value("tool/sectionPosition7", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION7(VAL)		settings.setValue("tool/sectionPosition7", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION8 				settings.   value("tool/sectionPosition8", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION8(VAL)		settings.setValue("tool/sectionPosition8", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION9 				settings.   value("tool/sectionPosition9", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION9(VAL)		settings.setValue("tool/sectionPosition9", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION10 				settings.   value("tool/sectionPosition10", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION10(VAL)		settings.setValue("tool/sectionPosition10", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION11 				settings.   value("tool/sectionPosition11", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION11(VAL)		settings.setValue("tool/sectionPosition11", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION12 				settings.   value("tool/sectionPosition12", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION12(VAL)		settings.setValue("tool/sectionPosition12", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION13 				settings.   value("tool/sectionPosition13", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION13(VAL)		settings.setValue("tool/sectionPosition13", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION14 				settings.   value("tool/sectionPosition14", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION14(VAL)		settings.setValue("tool/sectionPosition14", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION15 				settings.   value("tool/sectionPosition15", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION15(VAL)		settings.setValue("tool/sectionPosition15", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION16 				settings.   value("tool/sectionPosition16", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION16(VAL)		settings.setValue("tool/sectionPosition16", VAL)

#define SETTINGS_TOOL_SECTIONPOSITION17 				settings.   value("tool/sectionPosition17", 0).toDouble()
#define SETTINGS_SET_TOOL_SECTIONPOSITION17(VAL)		settings.setValue("tool/sectionPosition17", VAL)

//CTRAM
#define SETTINGS_TRAM_EQWIDTH 				settings.   value("tram/eqWidth", 24).toDouble()
#define SETTINGS_SET_TRAM_EQWIDTH(VAL)		settings.setValue("tram/eqWidth", VAL)

#define SETTINGS_TRAM_WHEELSPACING 			settings.   value("tram/wheelSpacing",1.7).toDouble()
#define SETTINGS_SET_TRAM_WHEELSPACING(VAL)	settings.setValue("tram/wheelSpacing",VAL)

#define SETTINGS_TRAM_PASSES				settings.   value("tram/passes", 0).toInt()
#define SETTINGS_SET_TRAM_PASSES(VAL)		settings.setValue("tram/passes",VAL)

//Vehicle-related settings
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

#define SETTINGS_VEHICLE_MINTURNINGRADIUS			settings.   value("vehicle/minSteeringRadius", 1).toDouble()
#define SETTINGS_SET_VEHICLE_MINTURNINGRADIUS(VAL)	settings.setValue("vehicle/minSteeringRadius",VAL)

#define SETTINGS_VEHICLE_ISSTEERAXLEAHEAD			settings.   value("vehicle/isSteerAxleAhead", true).toBool()
#define SETTINGS_SET_VEHICLE_ISSTEERAXLEAHEAD(VAL)	settings.setValue("vehicle/isSteerAxleAhead",VAL)

#define SETTINGS_VEHICLE_MINFIXSTEP			settings.   value("vehicle/minFixStep", 1).toDouble()
#define SETTINGS_SET_VEHICLE_MINFIXSTEP(VAL)	settings.setValue("vehicle/minFixStep",VAL)

#define SETTINGS_TOOL_SLOWSPEEDCUTOFF			settings.   value("vehicle/slowSpeedCuttoff", 0).toDouble()
#define SETTINGS_SET_VEHICLE_SLOWSPEEDCUTOFF(VAL)	settings.setValue("vehicle/slowSpeedCuttoff",VAL)

#define SETTINGS_VEHICLE_TYPE			settings.   value("vehicle/vehicleType", 0).toInt()
#define SETTINGS_SET_VEHICLE_TYPE(VAL)	settings.setValue("vehicle/vehicleType",VAL)

#define SETTINGS_VEHICLE_GOALPOINTLOOKAHEAD				settings.   value("vehicle/goalPointLookAhead", 3).toDouble()
#define SETTINGS_SET_VEHICLE_GOALPOINTLOOKAHEAD(VAL)		settings.setValue("vehicle/goalPointLookAhead",VAL)

#define SETTINGS_VEHICLE_LOOKAHEADMINIMUM		settings.   value("vehicle/lookAheadMinimum", 2).toDouble()
#define SETTINGS_SET_VEHICLE_LOOKAHEADMINIMUM(VAL)	settings.setValue("vehicle/lookAheadMinimum",VAL)

#define SETTINGS_VEHICLE_DISTANCEMULTIPLIER			settings.   value("vehicle/lookAheadDistanceFromLine", 0.1).toDouble()
#define SETTINGS_SET_VEHICLE_DISTANCEMULTIPLIER(VAL)	settings.setValue("vehicle/lookAheadDistanceFromLine",VAL)

#define SETTINGS_VEHICLE_LOOKAHEADUTURNMULT			settings.   value("vehicle/lookAheadUturnMult", 0.8).toDouble()
#define SETTINGS_SET_VEHICLE_LOOKAHEADUTURNMULT(VAL)	settings.setValue("vehicle/lookAheadUturnMult",VAL)

#define SETTINGS_VEHICLE_ISSTANLEYUSED			settings.   value("vehicle/isStanleyUsed", true).toBool()
#define SETTINGS_SET_VEHICLE_ISSTANLEYUSED(VAL)	settings.setValue("vehicle/isStanleyUsed",VAL)

#define SETTINGS_VEHICLE_STANLEYGAIN			settings.   value("vehicle/stanleyGain", 1).toDouble()
#define SETTINGS_SET_VEHICLE_STANLEYGAIN(VAL)	settings.setValue("vehicle/stanleyGain",VAL)

#define SETTINGS_VEHICLE_STANLEYHEADINGERRORGAIN			settings.   value("vehicle/stanleyHeadingErrorGain", 1).toDouble()
#define SETTINGS_SET_VEHICLE_STANLEYHEADINGERRORGAIN(VAL)	settings.setValue("vehicle/stanleyHeadingErrorGain",VAL)

#define SETTINGS_VEHICLE_MAXANGULARVELOCITY				settings.   value("vehicle/maxAngularVelocity", 7).toDouble()
#define SETTINGS_SET_VEHICLE_MAXANGULARVELOCITY(VAL)	settings.setValue("vehicle/maxAngularVelocity",VAL)

#define SETTINGS_VEHICLE_MAXSTEERANGLE			settings.   value("vehicle/maxSteerAngle", 40).toDouble()
#define SETTINGS_SET_VEHICLE_MAXSTEERANGLE(VAL)	settings.setValue("vehicle/maxSteerAngle",VAL)

#define SETTINGS_VEHICLE_ISHYDLIFTON			settings.   value("vehicle/isHydLiftOn", false).toBool()
#define SETTINGS_SET_VEHICLE_ISHYDLIFTON(VAL)	settings.setValue("vehicle/isHydLiftOn",VAL)

#define SETTINGS_VEHICLE_HYDLIFTLOOKAHEAD			settings.   value("vehicle/hydraulicLiftLookAhead", 2).toDouble()
#define SETTINGS_SET_VEHICLE_HYDLIFTLOOKAHEAD(VAL)	settings.setValue("vehicle/hydraulicLiftLookAhead",VAL)

#define SETTINGS_VEHICLE_NAME			settings.   value("vehicle/name", "unnamed").toQString()
#define SETTINGS_SET_VEHICLE_NAME(VAL)	settings.setValue("vehicle/name",VAL)

#define SETTINGS_VEHICLE_ISMACHINECONTROLTOAUTOSTEER			settings.   value("vehicle/isMachineControlToAutoSteer", false).toBool()
#define SETTINGS_SET_VEHICLE_ISMACHINECONTROLTOAUTOSTEER(VAL)	settings.setValue("vehicle/isMachineControlToAutoSteer",VAL)

#define SETTINGS_VEHICLE_GEOFENCEDIST			settings.   value("vehicle/geoFenceDistance", 0).toDouble()
#define SETTINGS_SET_VEHICLE_GEOFENCEDIST(VAL)	settings.setValue("vehicle/geoFenceDistance",VAL)

#define SETTINGS_VEHICLE_YOUTURNTRIGGERDISTANCE			settings.   value("vehicle/youTriggerDistance", 8).toDouble()
#define SETTINGS_SET_VEHICLE_YOUTURNTRIGGERDISTANCE(VAL)	settings.setValue("vehicle/youTriggerDistance",VAL)

#define SETTINGS_VEHICLE_YOUTURNDISTANCE			settings.   value("vehicle/youTurnDistance", 5).toInt()
#define SETTINGS_SET_VEHICLE_YOUTURNDISTANCE(VAL)	settings.setValue("vehicle/youTurnDistance",VAL)

#define SETTINGS_VEHICLE_YOUSKIPWIDTH			settings.   value("vehicle/youSkipWidth", 1).toInt()
#define SETTINGS_SET_VEHICLE_YOUSKIPWIDTH(VAL)	settings.setValue("vehicle/youSkipWidth",VAL)

#define SETTINGS_VEHICLE_ISUSINGDUBINSTURN			settings.   value("vehicle/isUsingDubins", true).toBool()
#define SETTINGS_SET_VEHICLE_ISUSINGDUBINSTURN(VAL)	settings.setValue("vehicle/isUsingDubins",VAL)

//CSEQUENCE
#define SETTINGS_VEHICLE_SEQFUNCTIONENTER			settings.   value("vehicle/seqFunctionEnter", "0,0,0,0,0,0,0,0").toString()
#define SETTINGS_SET_VEHICLE_SEQFUNCTIONENTER(VAL)	settings.setValue("vehicle/seqFunctionEnter",VAL)

#define SETTINGS_VEHICLE_SEQFUNCTIONEXIT			settings.   value("vehicle/seqFunctionExit", "0,0,0,0,0,0,0,0").toString()
#define SETTINGS_SET_VEHICLE_SEQFUNCTIONEXIT(VAL)	settings.setValue("vehicle/seqFunctionExit",VAL)

#define SETTINGS_VEHICLE_SEQACTIONENTER			settings.   value("vehicle/seqActionEnter", "0,0,0,0,0,0,0,0").toString()
#define SETTINGS_SET_VEHICLE_SEQACTIONENTER(VAL)	settings.setValue("vehicle/seqActionEnter",VAL)

#define SETTINGS_VEHICLE_SEQACTIONEXIT			settings.   value("vehicle/seqActionExit", "0,0,0,0,0,0,0,0").toString()
#define SETTINGS_SET_VEHICLE_SEQACTIONEXIT(VAL)	settings.setValue("vehicle/seqActionExit",VAL)

#define SETTINGS_VEHICLE_SEQDISTANCEENTER			settings.   value("vehicle/seqDistanceEnter", "0,0,0,0,0,0,0,0").toString()
#define SETTINGS_SET_VEHICLE_SEQDISTANCEENTER(VAL)	settings.setValue("vehicle/seqDistanceEnter",VAL)

#define SETTINGS_VEHICLE_SEQDISTANCEEXIT			settings.   value("vehicle/seqDistanceExit", "0,0,0,0,0,0,0,0").toString()
#define SETTINGS_SET_VEHICLE_SEQDISTANCEEXIT(VAL)	settings.setValue("vehicle/seqDistanceExit",VAL)

#define SETTINGS_VEHICLE_SEQFUNCTIONLIST			settings.   value("vehicle/seqFunctionList", "Relay 1,Relay 2,Relay 3,Relay 4,Relay 5,Relay 6").toString()
#define SETTINGS_SET_VEHICLE_SEQFUNCTIONLIST(VAL)	settings.setValue("vehicle/seqFunctionList",VAL)

#define SETTINGS_VEHICLE_SEQACTIONLIST			settings.   value("vehicle/seqActionList", "Turn Off,Turn On").toString()
#define SETTINGS_SET_VEHICLE_SEQACTIONLIST(VAL)	settings.setValue("vehicle/seqActionList",VAL)


//CMACHINECOMM
#define SETTINGS_ARDSTEER_SETTING1			settings.   value("ardSteer/setting1", 0).toInt()
#define SETTINGS_SET_ARDSTEER_SETTING1(VAL)	settings.setValue("ardSteer/setting1", VAL)

#define SETTINGS_ARDSTEER_MINSPEED			settings.   value("ardSteer/minSpeed", 1).toInt()
#define SETTINGS_SET_ARDSTEER_MINSPEED(VAL)	settings.setValue("ardSteer/minSpeed", VAL)

#define SETTINGS_ARDSTEER_MAXSPEED			settings.   value("ardSteer/maxSpeed", 20).toInt()
#define SETTINGS_SET_ARDSTEER_MAXSPEED(VAL)	settings.setValue("ardSteer/maxSpeed", VAL)

#define SETTINGS_ARDSTEER_SETTING0			settings.   value("ardSteer/setting0", 120).toInt()
#define SETTINGS_SET_ARDSTEER_SETTING0(VAL)	settings.setValue("ardSteer/setting0", VAL)

#define SETTINGS_ARDSTEER_INCLINOMETER			settings.   value("ardSteer/inclinometer", 2).toInt()
#define SETTINGS_SET_ARDSTEER_INCLINOMETER(VAL)	settings.setValue("ardSteer/inclinometer", VAL)

#define SETTINGS_ARDSTEER_MAXPULSECOUNTS			settings.   value("ardSteer/maxPulseCounts", 15).toInt()
#define SETTINGS_SET_ARDSTEER_MAXPULSECOUNTS(VAL)	settings.setValue("ardSteer/maxPulseCounts", VAL)

#define SETTINGS_ARDSTEER_ACKERMANFIX			settings.   value("ardSteer/ackermanFix", 100).toInt()
#define SETTINGS_SET_ARDSTEER_ACKERMANFIX(VAL)	settings.setValue("ardSteer/ackermanFix", VAL)

//CNMEA / GPS / IMU
#define SETTINGS_GPS_HEADINGFROMWHICHSOURCE			settings.   value("gps/headingFromWhichSource", QString("Fix")).toString()
#define SETTINGS_SET_GPS_HEADINGFROMWHICHSOURCE(VAL)	settings.setValue("gps/headingFromWhichSource",VAL)

#define SETTINGS_GPS_FIXFROMWHICH			settings.   value("gps/fixFromWhichSentence", QString("GGA")).toString()
#define SETTINGS_SET_GPS_FIXFROMWHICH(VAL)	settings.setValue("gps/fixFromWhichSentence",VAL)

#define SETTINGS_GPS_ISROLLFROMGPS			settings.   value("gps/isRollFromGPS", false).toBool()
#define SETTINGS_SET_GPS_ISROLLFROMGPS(VAL)	settings.setValue("gps/isRollFromGPS",VAL)

#define SETTINGS_GPS_BRICKUID			settings.   value("gps/brickUID", QString("68wESU")).toString()
#define SETTINGS_SET_GPS_BRICKUID(VAL)	settings.setValue("gps/brickUID",VAL)

#define SETTINGS_GPS_ISROLLFROMAUTOSTEER			settings.   value("gps/isRollFromAutosteer", false).toBool()
#define SETTINGS_SET_GPS_ISROLLFROMAUTOSTEER(VAL)	settings.setValue("gps/isRollFromAutosteer",VAL)

#define SETTINGS_GPS_ISHEADINGCORRECTIONFROMBRICK			settings.   value("gps/isHeadingCorrectionFromBrick", false).toBool()
#define SETTINGS_SET_GPS_ISHEADINGCORRECTIONFROMBRICK(VAL)	settings.setValue("gps/isHeadingCorrectionFromBrick",VAL)

#define SETTINGS_GPS_ISHEADINGCORRECTIONFROMAUTOSTEER			settings.   value("gps/isHeadingCorrectionFromAutoSteer", false).toBool()
#define SETTINGS_SET_GPS_ISHEADINGCORRECTIONFROMAUTOSTEER(VAL)	settings.setValue("gps/isHeadingCorrectionFromAutoSteer",VAL)

#define SETTINGS_GPS_ISROLLFROMBRICK			settings.   value("gps/isRollFromBrick", false).toBool()
#define SETTINGS_SET_GPS_ISROLLFROMBRICK(VAL)	settings.setValue("gps/isRollFromBrick",VAL)

#define SETTINGS_GPS_ISHEADINGCORRECTIONFROMEXTUDP			settings.   value("gps/isHeadingCorrectionFromExtUdp", false).toBool()
#define SETTINGS_SET_GPS_ISHEADINGCORRECTIONFROMEXTUDP(VAL)	settings.setValue("gps/isHeadingCorrectionFromExtUdp",VAL)

#define SETTINGS_GPS_ISROLLFROMOGI			settings.   value("gps/isRollFromOGI", false).toBool()
#define SETTINGS_SET_GPS_ISROLLFROMOGI(VAL)	settings.setValue("gps/isRollFromOGI",VAL)

#define SETTINGS_GPS_ISROLLFROMEXTUDP			settings.   value("gps/isRollFromExtUdp", false).toBool()
#define SETTINGS_SET_GPS_ISROLLFROMEXTUDP(VAL)	settings.setValue("gps/isRollFromExtUdp",VAL)

#define SETTINGS_GPS_IMUPITCHZEROX16			settings.   value("gps/IMUPitchZeroX16", 0).toInt()
#define SETTINGS_SET_GPS_IMUPITCHZEROX16(VAL)	settings.setValue("gps/IMUPitchZeroX16",VAL)

#define SETTINGS_GPS_IMUROLLZEROX16				settings.   value("gps/IMURollZeroX16", 0).toInt()
#define SETTINGS_SET_GPS_IMUROLLZEROX16(VAL)	settings.setValue("gps/IMURollZeroX16",VAL)

#define SETTINGS_GPS_LOGNMEA			settings.   value("gps/logNMEA", false).toBool()
#define SETTINGS_SET_GPS_LOGNMEA(VAL)	settings.setValue("gps/logNMEA",VAL)

#define SETTINGS_GPS_LOGELEVATION			settings.   value("gps/logElevation", false).toBool()
#define SETTINGS_SET_GPS_LOGELEVATION(VAL)	settings.setValue("gps/logElevation",VAL)

#define SETTINGS_GPS_EXPECTRTK			settings.   value("gps/expectRTK", true).toBool()
#define SETTINGS_SET_GPS_EXPECTRTK(VAL)	settings.setValue("gps/expectRTK",VAL)

#define SETTINGS_GPS_NTRIPCASTERIP			settings.   value("gps/NTRIPCasterIP", "").toString()
#define SETTINGS_SET_GPS_NTRIPCASTERIP(VAL)	settings.setValue("gps/NTRIPCasterIP",VAL)

#define SETTINGS_GPS_NTRIPCASTERPORT			settings.   value("gps/NTRIPCasterPort", 2101).toInt()
#define SETTINGS_SET_GPS_NTRIPCASTERPORT(VAL)	settings.setValue("gps/NTRIPCasterPort",VAL)

#define SETTINGS_GPS_NTRIPCASTERURL			settings.   value("gps/NTRIPCasterURL", "").toString()
#define SETTINGS_SET_GPS_NTRIPCASTERURL(VAL)	settings.setValue("gps/NTRIPCasterURL",VAL)

#define SETTINGS_GPS_NTRIPGGAMANUAL			settings.   value("gps/NTRIPGGAManual", false).toBool()
#define SETTINGS_SET_GPS_NTRIPGGAMANUAL(VAL)	settings.setValue("gps/NTRIPGGAManual",VAL)

#define SETTINGS_GPS_NTRIPON			settings.   value("gps/NTRIPon", false).toBool()
#define SETTINGS_SET_GPS_NTRIPON(VAL)	settings.setValue("gps/NTRIPon",VAL)

#define SETTINGS_GPS_NTRIPTCP			settings.   value("gps/NTRIPtcp", true).toBool()
#define SETTINGS_SET_GPS_NTRIPTCP(VAL)	settings.setValue("gps/NTRIPtcp",VAL)

#define SETTINGS_GPS_NTRIPMANUALLAT			settings.   value("gps/NTRIPManualLat", 0).toDouble()
#define SETTINGS_SET_GPS_NTRIPMANUALLAT(VAL)	settings.setValue("gps/NTRIPManualLat",VAL)

#define SETTINGS_GPS_NTRIPMANUALLON			settings.   value("gps/NTRIPManualLon", 0 ).toDouble()
#define SETTINGS_SET_GPS_NTRIPMANUALLON(VAL)	settings.setValue("gps/NTRIPManualLon",VAL)

#define SETTINGS_GPS_NTRIPMOUNT			settings.   value("gps/NTRIPMount", "").toString()
#define SETTINGS_SET_GPS_NTRIPMOUNT(VAL)	settings.setValue("gps/NTRIPMount",VAL)

#define SETTINGS_GPS_NTRIPGGAINTERVAL			settings.   value("gps/NTRIPGGAInterval", 1).toInt()
#define SETTINGS_SET_GPS_NTRIPGGAINTERVAL(VAL)	settings.setValue("gps/NTRIPGGAInterval",VAL)

#define SETTINGS_GPS_NTRIPSENDTOUDPPORT				settings.   value("gps/NTRIPSendToUDPport", 0).toInt()
#define SETTINGS_SET_GPS_NTRIPSENDTOUDPPORT(VAL)	settings.setValue("gps/NTRIPSendToUDPport",VAL)

#define SETTINGS_GPS_NTRIPUSERNAME			settings.   value("gps/NTRIPUsername", "").toString()
#define SETTINGS_SET_GPS_NTRIPUSERNAME(VAL)	settings.setValue("gps/NTRIPUsername",VAL)

#define SETTINGS_GPS_NTRIPPASSWORD			settings.   value("gps/NTRIPPassword", "").toString()
#define SETTINGS_SET_GPS_NTRIPPASSWORD(VAL)	settings.setValue("gps/NTRIPPassword",VAL)

//DISPLAY/GUI/MENU/Environment
#define SETTINGS_DISPLAY_NAME			settings.   value("display/name", "unnamed").toQString()
#define SETTINGS_SET_DISPLAY_NAME(VAL)	settings.setValue("display/name",VAL)

#define SETTINGS_DISPLAY_ISPUREON			settings.   value("display/isPureOn", true).toBool()
#define SETTINGS_SET_DISPLAY_ISPUREON(VAL)	settings.setValue("display/isPureOn",VAL)

#define SETTINGS_DISPLAY_CAMPITCH	        settings.   value("display/camPitch", -32).toDouble()
#define SETTINGS_SET_DISPLAY_CAMPITCH(VAL)	settings.setValue("display/camPitch",VAL)

#define SETTINGS_DISPLAY_TRIANGLERESOLUTION	        	settings.   value("display/triangleResolution", 1).toDouble()
#define SETTINGS_SET_DISPLAY_TRIANGLERESOLUTION(VAL)	settings.setValue("display/triangleResolution",VAL)

#define SETTINGS_DISPLAY_LIGHTBARCMPP	       	settings.   value("display/lightbarCmPerPixel", 4).toInt()
#define SETTINGS_SET_DISPLAY_LIGHTBARCMPP(VAL)	settings.setValue("display/lightbarCmPerPixel",VAL)

#define SETTINGS_DISPLAY_ISBATMANON			settings.   value("display/isBatManOn", false).toBool()
#define SETTINGS_SET_DISPLAY_ISBATMANON(VAL)	settings.setValue("display/isBatManOn",VAL)

#define SETTINGS_DISPLAY_LINEWIDTH			settings.   value("display/lineWidth", 2).toInt()
#define SETTINGS_SET_DISPLAY_LINEWIDTH(VAL)	settings.setValue("display/lineWidth",VAL)

#define SETTINGS_DISPLAY_SIMPANELLOCATION			settings.   value("display/simPanelLocation","97,600").toString()
#define SETTINGS_SET_DISPLAY_SIMPANELLOCATION(VAL)	settings.setValue("display/simPanelLocation",VAL)

#define SETTINGS_DISPLAY_COLORDAY			settings.   value("display/colorDay","210,220,230").toString()
#define SETTINGS_SET_DISPLAY_COLORDAY(VAL)	settings.setValue("display/colorDay",VAL)

#define SETTINGS_DISPLAY_COLORNIGHT				settings.   value("display/colorNight","60,60,60").toString()
#define SETTINGS_SET_DISPLAY_COLORNIGHT(VAL)	settings.setValue("display/colorNight",VAL)

#define SETTINGS_DISPLAY_SECTIONSCOLORDAY			settings.   value("display/colorSectionsDay","27,151,160").toString()
#define SETTINGS_SET_DISPLAY_SECTIONSCOLORDAY(VAL)	settings.setValue("display/colorSectionsDay",VAL)

#define SETTINGS_DISPLAY_SECTIONSCOLORNIGHT				settings.   value("display/colorSectionsNight","27,100,100").toString()
#define SETTINGS_SET_DISPLAY_SECTIONSCOLORNIGHT(VAL)	settings.setValue("display/colorSectionsNight",VAL)

#define SETTINGS_DISPLAY_FIELDCOLORDAY			settings.   value("display/colorFieldDay","90,90,90").toString()
#define SETTINGS_SET_DISPLAY_FIELDCOLORDAY(VAL)	settings.setValue("display/colorFieldDay",VAL)

#define SETTINGS_DISPLAY_FIELDCOLORNIGHT			settings.   value("display/colorFieldNight","60,60,60").toString()
#define SETTINGS_SET_DISPLAY_FIELDCOLORNIGHT(VAL)	settings.setValue("display/colorFieldNight",VAL)

#define SETTINGS_DISPLAY_ISSIMPLE			settings.   value("display/isSimple", true).toBool()
#define SETTINGS_SET_DISPLAY_ISSIMPLE(VAL)	settings.setValue("display/isSimple",VAL)

#define SETTINGS_DISPLAY_ISDAYMODE			settings.   value("display/isDayMode", true).toBool()
#define SETTINGS_SET_DISPLAY_ISDAYMODE(VAL)	settings.setValue("display/isDayMode",VAL)

#define SETTINGS_DISPLAY_ISAUTODAYNIGHT			settings.   value("display/isAutoDayNight", true).toBool()
#define SETTINGS_SET_DISPLAY_ISAUTODAYNIGHT(VAL)	settings.setValue("display/isAutoDayNight",VAL)

#define SETTINGS_DISPLAY_SIDEGUIDELINES			settings.   value("display/showSideGuideLines", false).toBool()
#define SETTINGS_SET_DISPLAY_SIDEGUIDELINES(VAL)	settings.setValue("display/showSideGuideLines",VAL)

#define SETTINGS_DISPLAY_UTURNALWAYSON			settings.   value("display/isUTurnAlwaysOn", false).toBool()
#define SETTINGS_SET_DISPLAY_UTURNALWAYSON(VAL)	settings.setValue("display/isUTurnAlwaysOn",VAL)

#define SETTINGS_DISPLAY_SKYON			settings.   value("display/isSkyOn", true).toBool()
#define SETTINGS_SET_DISPLAY_SKYON(VAL)	settings.setValue("display/isSkyOn",VAL)

#define SETTINGS_DISPLAY_LIGHTBARON				settings.   value("display/isLightBarOn", true).toBool()
#define SETTINGS_SET_DISPLAY_LIGHTBARON(VAL)	settings.setValue("display/isLightBarOn",VAL)

#define SETTINGS_DISPLAY_COMPASS			settings.   value("display/isCompassOn", false).toBool()
#define SETTINGS_SET_DISPLAY_COMPASS(VAL)	settings.setValue("display/isCompassOn",VAL)

#define SETTINGS_DISPLAY_SPEEDO				settings.   value("display/isSpeedoOn", false).toBool()
#define SETTINGS_SET_DISPLAY_SPEEDO(VAL)	settings.setValue("display/isSpeedoOn",VAL)

#define SETTINGS_DISPLAY_DAYMODE				settings.   value("display/isDayMode", true).toBool()
#define SETTINGS_SET_DISPLAY_DAYMODE(VAL)	settings.setValue("display/isDayMode",VAL)

#define SETTINGS_DISPLAY_SHOWGRID				settings.   value("display/showGrid", true).toBool()
#define SETTINGS_SET_DISPLAY_SHOWGRID(VAL)	settings.setValue("display/showGrid",VAL)

#define SETTINGS_DISPLAY_ISMETRIC				settings.   value("display/isMetric", true).toBool()
#define SETTINGS_SET_DISPLAY_ISMETRIC(VAL)	settings.setValue("display/isMetric",VAL)

#define SETTINGS_DISPLAY_ISFONTON			settings.   value("display/isFontOn", true).toBool()
#define SETTINGS_SET_DISPLAY_ISFONTON(VAL)	settings.setValue("display/isFontOn",VAL)

#define SETTINGS_DISPLAY_OGLZOOM			settings.   value("display/oglZoom", false).toBool()
#define SETTINGS_SET_DISPLAY_OGLZOOM(VAL)	settings.setValue("display/oglZoom",VAL)

#define SETTINGS_DISPLAY_FULLSCREEN			settings.   value("display/fullScreen", false).toBool()
#define SETTINGS_SET_DISPLAY_FULLSCREEN(VAL)	settings.setValue("display/fullScreen",VAL)

#define SETTINGS_DISPLAY_SHOWBACK			settings.   value("display/showBack", false).toBool()
#define SETTINGS_SET_DISPLAY_SHOWBACK(VAL)	settings.setValue("display/showBack",VAL)

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

//MACHINE CONTROL
#define SETTINGS_ARDMAC_HYDRAISETIME			settings.   value("ardMac/hydRaiseTime", 6).toInt()
#define SETTINGS_SET_ARDMAC_HYDRAISETIME(VAL)	settings.setValue("ardMac/hydRaiseTime", VAL)

#define SETTINGS_ARDMAC_HYDLOWERTIME			settings.   value("ardMac/hydLowerTime", 7).toInt()
#define SETTINGS_SET_ARDMAC_HYDLOWERTIME(VAL)	settings.setValue("ardMac/hydLowerTime", VAL)

#define SETTINGS_ARDMAC_ISHYDENABLED			settings.   value("ardMac/isHydEnabled", 0).toInt()
#define SETTINGS_SET_ARDMAC_ISHYDENABLED(VAL)	settings.setValue("ardMac/isHydEnabled", VAL)

//CSIM
#define SETTINGS_SIM_LATITUDE			settings.   value("sim/latitude", 49).toDouble()
#define SETTINGS_SET_SIM_LATITUDE(VAL)	settings.setValue("sim/latitude",VAL)

#define SETTINGS_SIM_LONGITUDE			settings.   value("sim/longitude", -111).toDouble()
#define SETTINGS_SET_SIM_LONGITUDE(VAL)	settings.setValue("sim/longitude",VAL)

#define SETTINGS_SIM_ON			settings.   value("sim/on", true).toBool()
#define SETTINGS_SET_SIM_ON(VAL)	settings.setValue("sim/on",VAL)

//POSITION.DESIGNER
#define SETTINGS_COMM_UDPISON			settings.   value("communications/udpIsOn", false).toBool()
#define SETTINGS_SET_COMM_UDPISON(VAL)	settings.setValue("communications/udpIsOn",VAL)

//environment
#define SETTINGS_ENVIRONMENT_NAME			settings.   value("environment/name", "unnamed").toString()
#define SETTINGS_SET_ENVIRONMENT_NAME(VAL)	settings.setValue("environment/name",VAL)

#define SETTINGS_ENVIRONMENT_LASTFIELD			settings.   value("environment/lastField", "unnamed").toString()
#define SETTINGS_SET_ENVIRONMENT_LASTFIELD(VAL)	settings.setValue("environment/lastField",VAL)

#endif // AOGSETTINGS_H

