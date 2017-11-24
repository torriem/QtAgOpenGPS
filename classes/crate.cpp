#include "crate.h"
#include "cmodulecomm.h"
#include "cvehicle.h"


CRate::CRate()
{

}

void CRate::shutdownRateControl(CModuleComm *mc)
{
    isRateControlOn = false;
    isRate1Selected = true;

    //turn all relays off
    mc->relayRateControl[CModuleComm::rcHeaderHi] = 127; //32762
    mc->relayRateControl[CModuleComm::rcHeaderLo] = 250;
    mc->relayRateControl[CModuleComm::rcSectionControlByte] = 0;
    mc->relayRateControl[CModuleComm::rcRateSetPointHi] = 0;
    mc->relayRateControl[CModuleComm::rcRateSetPointLo] = 0;
    mc->relayRateControl[CModuleComm::rcSpeedXFour] = 0;
    //mf.RateRelayControlOutToPort();
}

void CRate::calculateRateLitersPerMinute(CVehicle *vehicle)
{
    //determine current width based on sections off on
    double currentWidth = 0;

    // is supersection on?
    if (vehicle->section[vehicle->numOfSections].isSectionOn)
    {
        currentWidth = vehicle->toolWidth;
    }

    //individual sections are possibly on
    else
    {
        for (int i = 0; i < vehicle->numOfSections; i++)
        {
            if (vehicle->section[i].isSectionOn) currentWidth += vehicle->section[i].sectionWidth;
        }
    }

    // hectares/min to liters/min based on set rate
    if (isRate1Selected)
        rateSetPoint = (currentWidth * vehicle->speed)/600 * rate1 * 0.1;
    else
        rateSetPoint = (currentWidth * vehicle->speed) / 600 * rate2 * 0.1;
}
