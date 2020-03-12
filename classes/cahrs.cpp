#include "cahrs.h"
#include "aogsettings.h"

CAHRS::CAHRS(QObject *parent) : QObject(parent)
{
    USE_SETTINGS;

    isAutoSteerAuto = SETTINGS_AUTOSTEER_AUTOON;

    if (SETTINGS_GPS_ISHEADINGCORRECTIONFROMBRICK )
    {
        //TODO: implement brick connection

    }
}

//SLOTS
void CAHRS::setRollX16(int chx16) {
    rollX16 = chx16;
}

void CAHRS::setRollZeroX16(int nRoll) {
    USE_SETTINGS;

    rollK = nRoll; //input to the kalman filter
    //kalman filter
    Pc = P + varProcess;//0.001
    G = Pc / (Pc + varRoll);
    P = (1 - G) * Pc;
    Xp = XeRoll;
    Zp = Xp;
    XeRoll = (G * (rollK - Zp)) + Xp;//result

    SETTINGS_SET_GPS_IMUROLLZEROX16(XeRoll);
}

void CAHRS::setCorrectionHeadingX16(int chx16) { correctionHeadingX16 = chx16;}

void CAHRS::setIsAutoSteerOn(bool state) { isAutoSteerAuto = state; }

void CAHRS::setIsHeadingCorrectionFromAutoSteer(bool state) { USE_SETTINGS; SETTINGS_SET_GPS_ISHEADINGCORRECTIONFROMAUTOSTEER(state); }

void CAHRS::setIsHeadingCorrectionFromBrick(bool state) { USE_SETTINGS; SETTINGS_SET_GPS_ISHEADINGCORRECTIONFROMBRICK(state); }

void CAHRS::setIsHeadingCorrectionFromExtUDP(bool state) { USE_SETTINGS; SETTINGS_SET_GPS_ISHEADINGCORRECTIONFROMEXTUDP(state); }

void CAHRS::setIsRollFromAutoSteer(bool state) { USE_SETTINGS; SETTINGS_SET_GPS_ISROLLFROMAUTOSTEER(state); }

void CAHRS::setIsRollFromGPS(bool state) { USE_SETTINGS; SETTINGS_SET_GPS_ISROLLFROMGPS(state); }

void CAHRS::setIsRollFromExtUDP(bool state) { USE_SETTINGS; SETTINGS_SET_GPS_ISROLLFROMEXTUDP(state); }
