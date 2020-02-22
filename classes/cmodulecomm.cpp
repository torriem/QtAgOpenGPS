#include "cmodulecomm.h"
#include "aogsettings.h"

//TODO move to single file for defaults
#define DEFAULT_SETAS_KP 5
#define DEFAULT_SETAS_KI 0
#define DEFAULT_SETAS_KD 5
#define DEFAULT_SETAS_KO 5
#define DEFAULT_SETAS_STEERINGANGLEOFFSET 70
#define DEFAULT_SETAS_MINSTEERPWM 10
#define DEFAULT_SETAS_MAXINTEGRAL 20
#define DEFAULT_SETAS_COUNTSPERDEGREE 40

CModuleComm::CModuleComm(QObject *parent) : QObject(parent)
{
    USE_SETTINGS;
    serialRecvAutoSteerStr = "Oops NC";
    serialRecvRelayStr = "Oops NC";

    pgn.clear();

    //WorkSwitch logic
    isWorkSwitchEnabled = false;

    //does a low, grounded out, mean on
    isWorkSwitchActiveLow = true;

    autoSteerData[sdHeaderHi] = 127; // PGN - 32766
    autoSteerData[sdHeaderLo] = 254;
    autoSteerData[sdRelayLo] = 0;
    autoSteerData[sdSpeed] = 0;
    autoSteerData[sdDistanceHi] = 125; // PGN - 32020
    autoSteerData[sdDistanceLo] = 20;
    autoSteerData[sdSteerAngleHi] = 125; // PGN - 32020
    autoSteerData[sdSteerAngleLo] = 20;
    autoSteerData[sdYouTurnByte] = 0;
    pgn.append((const char *)autoSteerData, pgnSentenceLength);

    autoSteerSettings[ssHeaderHi] = 127;// PGN - 32764 as header
    autoSteerSettings[ssHeaderLo] = 252;
    autoSteerSettings[ssKp] = SETTINGS_AUTOSTEER_KP;
    autoSteerSettings[ssKi] = SETTINGS_AUTOSTEER_KI;
    autoSteerSettings[ssKd] = SETTINGS_AUTOSTEER_KD;
    autoSteerSettings[ssKo] = SETTINGS_AUTOSTEER_KO;
    autoSteerSettings[ssSteerOffset] = SETTINGS_AUTOSTEER_STEERINGANGLEOFFSET;
    autoSteerSettings[ssMinPWM] = SETTINGS_AUTOSTEER_MINSTEERPWM;
    autoSteerSettings[ssMaxIntegral] = SETTINGS_AUTOSTEER_MAXINTEGRAL;
    autoSteerSettings[ssCountsPerDegree] = SETTINGS_AUTOSTEER_COUNTSPERDEGREE;
    pgn.append((const char*)autoSteerSettings, pgnSentenceLength);

    relayData[rdHeaderHi] = 127; // PGN - 32762
    relayData[rdHeaderLo] = 250;
    relayData[rdSectionControlByteHi] = 0;
    relayData[rdSectionControlByteLo] = 0;
    relayData[rdSpeedXFour] = 0;
    relayData[rdTramLine] = 0;
    relayData[rdTree] = 0;
    relayData[rdUTurn] = 0;
    relayData[rdHydLift] = 0;
    relayData[rd9] = 0;
    pgn.append((const char*)relayData, pgnSentenceLength);

    machineControlData[cnHeaderHi] = 127; // PGN - 32758
    machineControlData[cnHeaderLo] = 246;
    machineControlData[cnPedalControl] = 0;
    machineControlData[cnSpeed] = 0;
    machineControlData[cnRelayLo] = 0;
    machineControlData[cnYouTurn] = 0;
    machineControlData[6] = 0;
    machineControlData[7] = 0;
    machineControlData[8] = 0;
    machineControlData[9] = 0;
    pgn.append((const char*)machineControlData,pgnSentenceLength);


}

void CModuleComm::resetAllModuleCommValues()
{
    USE_SETTINGS;

    relayData[rdHeaderHi] = 127; // PGN - 32762
    relayData[rdHeaderLo] = 250;
    relayData[rdSectionControlByteHi] = 0;
    relayData[rdSectionControlByteLo] = 0;
    relayData[rdSpeedXFour] = 0;
    relayData[rdTramLine] = 0;
    relayData[rdTree] = 0;
    relayData[rdUTurn] = 0;
    relayData[rdHydLift] = 0;
    relayData[rd9] = 0;

    emit sendRelayOutToPort(relayData, pgnSentenceLength);

    //prefill the autosteer data
    autoSteerData[sdHeaderHi] = 127; //PGN - 32766
    autoSteerData[sdHeaderLo] = 254;
    autoSteerData[sdRelayLo] = 0;
    autoSteerData[sdSpeed] = 0;
    autoSteerData[sdDistanceHi] = 25; //PGN - 32020
    autoSteerData[sdDistanceLo] = 20;
    autoSteerData[sdSteerAngleHi] = 125; //PGN - 32020
    autoSteerData[sdSteerAngleLo] = 20;
    autoSteerData[sdYouTurnByte] = 0;

    emit sendAutoSteerDataOutToPort();

    //prefill the autosteer settings from settings xml
    autoSteerSettings[ssHeaderHi] = 127;
    autoSteerSettings[ssHeaderLo] = 252; //PGN - 32764 as header
    autoSteerSettings[ssKp] = SETTINGS_AUTOSTEER_KP;
    autoSteerSettings[ssKi] = SETTINGS_AUTOSTEER_KI;
    autoSteerSettings[ssKd] = SETTINGS_AUTOSTEER_KD;
    autoSteerSettings[ssKo] = SETTINGS_AUTOSTEER_KO;
    autoSteerSettings[ssSteerOffset] = SETTINGS_AUTOSTEER_STEERINGANGLEOFFSET;
    autoSteerSettings[ssMinPWM] = SETTINGS_AUTOSTEER_MINSTEERPWM;
    autoSteerSettings[ssMaxIntegral] = SETTINGS_AUTOSTEER_MAXINTEGRAL;
    autoSteerSettings[ssCountsPerDegree] = SETTINGS_AUTOSTEER_COUNTSPERDEGREE;

    emit sendAutoSteerSettingsOutToPort();

    machineControlData[cnHeaderHi] = 127; // PGN - 32758
    machineControlData[cnHeaderLo] = 246;
    machineControlData[cnPedalControl] = 0;
    machineControlData[cnSpeed] = 0;
    machineControlData[cnRelayLo] = 0;
    machineControlData[cnYouTurn] = 0;
    machineControlData[6] = 0;
    machineControlData[7] = 0;
    machineControlData[8] = 0;
    machineControlData[9] = 0;

}
