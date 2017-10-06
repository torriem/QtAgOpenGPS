#include "cmodulecomm.h"

//TODO move to single file for defaults
#define DEFAULT_SETAS_KP 5
#define DEFAULT_SETAS_KI 0
#define DEFAULT_SETAS_KD 5
#define DEFAULT_SETAS_KO 5
#define DEFAULT_SETAS_STEERINGANGLEOFFSET 70
#define DEFAULT_SETAS_MINSTEERPWM 10
#define DEFAULT_SETAS_MAXINTEGRAL 20
#define DEFAULT_SETAS_COUNTSPERDEGREE 40

CModuleComm::CModuleComm()
{
    serialRecvAutoSteerStr = ",";
    serialRecvRelayStr = ",";

    //control all relays based on byte value, 1 means on, 0 means off
    relaySectionControl[0] = 0;

    //prefill the autosteer data
    autoSteerData[sdHeaderHi] = 127; //32766
    autoSteerData[sdHeaderLo] = (254);
    autoSteerData[sdRelay] = 0;
    autoSteerData[sdSpeed] = (0);
    autoSteerData[sdDistanceHi] = (125); //32020
    autoSteerData[sdDistanceLo] = 20;
    autoSteerData[sdSteerAngleHi] = (125); //32020
    autoSteerData[sdSteerAngleLo] = 20;

    //WorkSwitch logic
    isWorkSwitchEnabled = false;
    isWorkSwitchOn = false;

    //does a low, grounded out, mean on
    isWorkSwitchActiveLow = true;

    //prefill the autosteer settings from settings xml
    autoSteerSettings[ssHeaderHi] = 127;
    autoSteerSettings[ssHeaderLo] = 252; //32764 as header

    QSettings s;
    autoSteerSettings[ssKp] = s.value("setAS_Kp", DEFAULT_SETAS_KP).toInt();
    autoSteerSettings[ssKi] = s.value("setAS_Ki", DEFAULT_SETAS_KI).toInt();
    autoSteerSettings[ssKd] = s.value("setAS_Kd", DEFAULT_SETAS_KD).toInt();
    autoSteerSettings[ssKo] = s.value("setAS_Ko", DEFAULT_SETAS_KO).toInt();
    autoSteerSettings[ssSteerOffset] = s.value("setAS_steerAngleOffset", DEFAULT_SETAS_STEERINGANGLEOFFSET).toInt();
    autoSteerSettings[ssMinPWM] = s.value("setAS_minSteerPWM", DEFAULT_SETAS_MINSTEERPWM).toInt();
    autoSteerSettings[ssMaxIntegral] = s.value("setAS_maxIntegral", DEFAULT_SETAS_MAXINTEGRAL).toInt();
    autoSteerSettings[ssCountsPerDegree] = s.value("setAS_countsPerDegree", DEFAULT_SETAS_COUNTSPERDEGREE).toInt();
}
