#include "cahrs.h"
#include "aogproperty.h"

CAHRS::CAHRS(QObject *parent) : QObject(parent)
{
    loadSettings();
}

void CAHRS::loadSettings()
{
    rollZero = property_setIMU_rollZero;
    rollFilter = property_setIMU_rollFilter;

    //is the auto steer in auto turn on mode or not
    isAutoSteerAuto = property_setAS_isAutoSteerAutoOn;
    isRollInvert = property_setIMU_invertRoll;
    isDualAsIMU = property_setIMU_isDualAsIMU;
    isReverseOn = property_setIMU_isReverseOn;

    //the factor for fusion of GPS and IMU
    forwardComp = property_setGPS_forwardComp;
    reverseComp = property_setGPS_reverseComp;
    fusionWeight = property_setIMU_fusionWeight;

}
