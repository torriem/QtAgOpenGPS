 #include "cahrs.h"
#include "aogsettings.h"

CAHRS::CAHRS(QObject *parent) : QObject(parent)
{
    USE_SETTINGS;
    isHeadingCorrectionFromAutoSteer = SETTINGS_GPS_ISHEADINGCORRECTIONFROMAUTOSTEER;
    isHeadingCorrectionFromBrick = SETTINGS_GPS_ISHEADINGCORRECTIONFROMBRICK;
    isHeadingCorrectionFromExtUDP = SETTINGS_GPS_ISHEADINGCORRECTIONFROMEXTUDP;

    isRollFromAutoSteer = SETTINGS_GPS_ISROLLFROMAUTOSTEER;
    isRollFromGPS = SETTINGS_GPS_ISROLLFROMGPS;
    isRollFromExtUDP = SETTINGS_GPS_ISROLLFROMEXTUDP;

    rollZeroX16 = SETTINGS_GPS_IMUROLLZEROX16;
    pitchZeroX16 = SETTINGS_GPS_IMUPITCHZEROX16;

    isAutoSteerAuto = SETTINGS_AUTOSTEER_AUTOON;

    if (isHeadingCorrectionFromBrick )
    {
        //TODO: implement brick connection

    }
}

//SLOTS
void CAHRS::setRollX16(int chx16) {
    rollX16 = chx16;
}

void CAHRS::setRollZeroX16(int nRoll) {
    rollK = nRoll; //input to the kalman filter
    //kalman filter
    Pc = P + varProcess;//0.001
    G = Pc / (Pc + varRoll);
    P = (1 - G) * Pc;
    Xp = XeRoll;
    Zp = Xp;
    XeRoll = (G * (rollK - Zp)) + Xp;//result

    rollZeroX16 = XeRoll;
}

void CAHRS::setCorrectionHeadingX16(int chx16) { correctionHeadingX16 = chx16;}

void CAHRS::setIsAutoSteerOn(bool state) { isAutoSteerAuto = state; }

void CAHRS::setIsHeadingCorrectionFromAutoSteer(bool state) { isHeadingCorrectionFromAutoSteer = state; }

void CAHRS::setIsHeadingCorrectionFromBrick(bool state) { isHeadingCorrectionFromBrick = state; }

void CAHRS::setIsHeadingCorrectionFromExtUDP(bool state) { isHeadingCorrectionFromExtUDP = state; }

void CAHRS::setIsRollFromAutoSteer(bool state) { isRollFromAutoSteer = state; }

void CAHRS::setIsRollFromGPS(bool state) { isRollFromGPS = state; }

void CAHRS::setIsRollFromExtUDP(bool state) { isRollFromExtUDP = state; }
