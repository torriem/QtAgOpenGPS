#include "cahrs.h"
#include "aogsettings.h"

CAHRS::CAHRS(QObject *parent) : QObject(parent)
{
    USE_SETTINGS;
    isHeadingFromAutoSteer = SETTINGS_GPS_ISHEADINGFROMAUTOSTEER;
    isHeadingFromBrick = SETTINGS_GPS_ISHEADINGFROMBRICK;
    isHeadingFromPAOGI = SETTINGS_GPS_ISHEADINGFROMPAOGI;
    isHeadingFromExtUDP = SETTINGS_GPS_ISHEADINGFROMEXTUDP;

    isRollFromAutoSteer = SETTINGS_GPS_ISROLLFROMAUTOSTEER;
    isRollFromBrick = SETTINGS_GPS_ISROLLFROMBRICK;
    isRollFromGPS = SETTINGS_GPS_ISROLLFROMGPS;
    isRollFromExtUDP = SETTINGS_GPS_ISROLLFROMEXTUDP;

    rollZeroX16 = SETTINGS_GPS_IMUROLLZEROX16;
    pitchZeroX16 = SETTINGS_GPS_IMUPITCHZEROX16;

    isAutoSteerAuto = SETTINGS_AUTOSTEER_AUTOON;

    if (isHeadingFromBrick || isRollFromBrick )
    {
        //TODO: implement brick connection

    }
}

//SLOTS
void CAHRS::setRollX16(int chx16) { rollX16 = chx16;}

void CAHRS::setRollZeroX16(int chx16) { rollZeroX16 = chx16;}

void CAHRS::setCorrectionHeadingX16(int chx16) { correctionHeadingX16 = chx16;}

void CAHRS::setIsAutoSteerOn(bool state) { isAutoSteerAuto = state; }

void CAHRS::setIsHeadingFromAutoSteer(bool state) { isHeadingFromAutoSteer = state; }

void CAHRS::setIsHeadingFromBrick(bool state) { isHeadingFromBrick = state; }

void CAHRS::setIsHeadingFromPAOGI(bool state) { isHeadingFromPAOGI = state; }

void CAHRS::setIsHeadingFromExtUDP(bool state) { isHeadingFromExtUDP = state; }

void CAHRS::setIsRollFromAutoSteer(bool state) { isRollFromAutoSteer = state; }

void CAHRS::setIsRollFromBrick(bool state) { isRollFromBrick = state; }

void CAHRS::setIsRollFromGPS(bool state) { isRollFromGPS = state; }

void CAHRS::setIsRollFromExtUDP(bool state) { isRollFromExtUDP = state; }
