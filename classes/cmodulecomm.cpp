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
    serialRecvAutoSteerStr = " ** Steer Module Not Connected";
    serialRecvMachineStr = " ** Machine Module Not Connected";

    //WorkSwitch logic
    //isWorkSwitchEnabled = false;

    //does a low, grounded out, mean on
    //isWorkSwitchActiveLow = true;

    isMachineDataSentToAutoSteer = SETTINGS_VEHICLE_ISMACHINECONTROLTOAUTOSTEER;

    //autosteer constant data
    autoSteerData[sdHeaderHi] = 127; // PGN - 32766
    autoSteerData[sdHeaderLo] = 254;
    autoSteerData[sd2] = 0;
    autoSteerData[sdSpeed] = 0;
    autoSteerData[sdDistanceHi] = 125; // PGN - 32020
    autoSteerData[sdDistanceLo] = 20;
    autoSteerData[sdSteerAngleHi] = 125; // PGN - 32020
    autoSteerData[sdSteerAngleLo] = 20;
    autoSteerData[sd8] = 0;
    autoSteerData[sd9] = 0;

    //autosteer steer settings
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

    //arduino basic steer settings
    ardSteerConfig[arHeaderHi] = 127; //PGN - 32763
    ardSteerConfig[arHeaderLo] = 251;
    ardSteerConfig[arSet0] = SETTINGS_ARDSTEER_SETTING0;
    ardSteerConfig[arSet1] = SETTINGS_ARDSTEER_SETTING1;
    ardSteerConfig[arMaxSpd] = SETTINGS_ARDSTEER_MAXSPEED;
    ardSteerConfig[arMinSpd] = SETTINGS_ARDSTEER_MINSPEED;
        uchar inc = (uchar)(SETTINGS_ARDSTEER_INCLINOMETER << 6);
    ardSteerConfig[arIncMaxPulse] = (uchar)(inc + (uchar)SETTINGS_ARDSTEER_MAXPULSECOUNTS);
    ardSteerConfig[arAckermanFix] = SETTINGS_ARDSTEER_ACKERMANFIX;
    ardSteerConfig[ar8] = 0;
    ardSteerConfig[ar9] = 0;

    //machine, sections data array
    machineData[mdHeaderHi] = 127; // PGN - 32758
    machineData[mdHeaderLo] = 246;
    machineData[mdSectionControlByteHi] = 0;
    machineData[mdSectionControlByteLo] = 0;
    machineData[mdSpeedXFour] = 0;
    machineData[mdUTurn] = 0;
    machineData[mdTree] = 0;
    machineData[mdHydLift] = 0;
    machineData[md8] = 0;
    machineData[md9] = 0;

    //arduino machine configuration
    ardMachineConfig[amHeaderHi] = 127; //PGN - 32760
    ardMachineConfig[amHeaderLo] = 248;
    ardMachineConfig[amRaiseTime] = SETTINGS_ARDMAC_HYDRAISETIME;
    ardMachineConfig[amLowerTime] = SETTINGS_ARDMAC_HYDLOWERTIME;
    ardMachineConfig[amEnableHyd] = SETTINGS_ARDMAC_ISHYDENABLED;
    ardMachineConfig[am5] = 0;
    ardMachineConfig[am6] = 0;
    ardMachineConfig[am7] = 0;
    ardMachineConfig[am8] = 0;
    ardMachineConfig[am9] = 0;

     //Section control: switches
    ss[swHeaderHi] = 0;  //PGN - 32609
    ss[swHeaderLo] = 0;  //0xE0
    ss[sw2] = 0;
    ss[sw3] = 0;
    ss[sw4] = 0;
    ss[swONHi] = 0;
    ss[swONLo] = 0;
    ss[swOFFHi] = 0;
    ss[swOFFLo] = 0;
    ss[swMain] = 0;

    ssP[swHeaderHi] = 0;  //PGN - 32609
    ssP[swHeaderLo] = 0;  //0xE0
    ssP[sw2] = 0;
    ssP[sw3] = 0;
    ssP[sw4] = 0;
    ssP[swONHi] = 0;
    ssP[swONLo] = 0;
    ssP[swOFFHi] = 0;
    ssP[swOFFLo] = 0;
    ssP[swMain] = 0;

}

void CModuleComm::resetAllModuleCommValues()
{
    USE_SETTINGS;
    machineData[mdHeaderHi] = 127; // PGN - 32762
    machineData[mdHeaderLo] = 250;
    machineData[mdSectionControlByteHi] = 0;
    machineData[mdSectionControlByteLo] = 0;
    machineData[mdSpeedXFour] = 0;
    machineData[mdUTurn] = 0;
    machineData[mdTree] = 0;
    machineData[mdHydLift] = 0;
    machineData[md8] = 0;
    machineData[md9] = 0;

    emit sendOutUSBMachinePort(machineData, pgnSentenceLength);

    autoSteerData[sdHeaderHi] = 127; // PGN - 32766
    autoSteerData[sdHeaderLo] = 254;
    autoSteerData[sd2] = 0;
    autoSteerData[sdSpeed] = 0;
    autoSteerData[sdDistanceHi] = 125; // PGN - 32020
    autoSteerData[sdDistanceLo] = 20;
    autoSteerData[sdSteerAngleHi] = 125; // PGN - 32020
    autoSteerData[sdSteerAngleLo] = 20;
    autoSteerData[sd8] = 0;
    autoSteerData[sd9] = 0;
    emit sendOutUSBAutoSteerPort(autoSteerData, pgnSentenceLength);

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
    //mf.SendSteerSettingsOutAutoSteerPort();

    ardSteerConfig[arHeaderHi] = 127; //PGN - 32763
    ardSteerConfig[arHeaderLo] = 251;
    ardSteerConfig[arSet0] = SETTINGS_ARDSTEER_SETTING0;
    ardSteerConfig[arSet1] = SETTINGS_ARDSTEER_SETTING1;
    ardSteerConfig[arMaxSpd] = SETTINGS_ARDSTEER_MAXSPEED;
    ardSteerConfig[arMinSpd] = SETTINGS_ARDSTEER_MINSPEED;
        uchar inc = (uchar)(SETTINGS_ARDSTEER_INCLINOMETER << 6);
    ardSteerConfig[arIncMaxPulse] = (uchar)(inc + (uchar)SETTINGS_ARDSTEER_MAXPULSECOUNTS);
    ardSteerConfig[arAckermanFix] = SETTINGS_ARDSTEER_ACKERMANFIX;
    ardSteerConfig[ar8] = 0;
    ardSteerConfig[ar9] = 0;

    //arduino machine configuration
    ardMachineConfig[amHeaderHi] = 127; //PGN - 32760
    ardMachineConfig[amHeaderLo] = 248;
    ardMachineConfig[amRaiseTime] = SETTINGS_ARDMAC_HYDRAISETIME;
    ardMachineConfig[amLowerTime] = SETTINGS_ARDMAC_HYDLOWERTIME;
    ardMachineConfig[amEnableHyd] = SETTINGS_ARDMAC_ISHYDENABLED;
    ardMachineConfig[am5] = 0;
    ardMachineConfig[am6] = 0;
    ardMachineConfig[am7] = 0;
    ardMachineConfig[am8] = 0;
    ardMachineConfig[am9] = 0;

}

void CModuleComm::setHydLift(int value)
{
    //Slot for other modules to signal
    machineData[mdHydLift] = value;
}
