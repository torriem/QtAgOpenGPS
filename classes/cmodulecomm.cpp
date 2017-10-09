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

    AOGSettings s;
    autoSteerSettings[ssKp] = s.value("autosteer/Kp", DEFAULT_SETAS_KP).toInt();
    autoSteerSettings[ssKi] = s.value("autosteer/Ki", DEFAULT_SETAS_KI).toInt();
    autoSteerSettings[ssKd] = s.value("autosteer/Kd", DEFAULT_SETAS_KD).toInt();
    autoSteerSettings[ssKo] = s.value("autosteer/Ko", DEFAULT_SETAS_KO).toInt();
    autoSteerSettings[ssSteerOffset] = s.value("autosteer/steerAngleOffset", DEFAULT_SETAS_STEERINGANGLEOFFSET).toInt();
    autoSteerSettings[ssMinPWM] = s.value("autosteer/minSteerPWM", DEFAULT_SETAS_MINSTEERPWM).toInt();
    autoSteerSettings[ssMaxIntegral] = s.value("autosteer/maxIntegral", DEFAULT_SETAS_MAXINTEGRAL).toInt();
    autoSteerSettings[ssCountsPerDegree] = s.value("autosteer/countsPerDegree", DEFAULT_SETAS_COUNTSPERDEGREE).toInt();
}
