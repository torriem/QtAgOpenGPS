#include "cmodulecomm.h"
#include "cahrs.h"

CModuleComm::CModuleComm(QObject *parent) : QObject(parent)
{
    //WorkSwitch logic
    isRemoteWorkSystemOn = false;

    //does a low, grounded out, mean on
    isWorkSwitchActiveLow = true;
}

void CModuleComm::CheckWorkAndSteerSwitch(CAHRS &ahrs, bool isAutoSteerBtnOn)
{
    //AutoSteerAuto button enable - Ray Bear inspired code - Thx Ray!
    if (ahrs.isAutoSteerAuto && steerSwitchHigh != oldSteerSwitchRemote)
    {
        oldSteerSwitchRemote = steerSwitchHigh;
        //steerSwith is active low
        if (steerSwitchHigh == isAutoSteerBtnOn)
            emit stopAutoSteer();
            //mf.btnAutoSteer.PerformClick();
    }

    if (isRemoteWorkSystemOn)
    {
        if (isWorkSwitchEnabled && (oldWorkSwitchHigh != workSwitchHigh))
        {
            oldWorkSwitchHigh = workSwitchHigh;

            if (workSwitchHigh != isWorkSwitchActiveLow)
            {
                if (isWorkSwitchManualSections)
                {
                    emit turnOffManulSections();
                    //mf.btnSectionMasterManual.PerformClick();
                }
                else
                {
                    emit turnOffAutoSections();
                    //mf.btnSectionMasterAuto.PerformClick();
                }
            }

            else//Checks both on-screen buttons, performs click if button is not off
            {
                emit turnOffAutoSections();
                //mf.btnSectionMasterAuto.PerformClick();
                emit turnOffManulSections();
                //mf.btnSectionMasterManual.PerformClick();
            }
        }

        if (isSteerWorkSwitchEnabled && (oldSteerSwitchHigh != steerSwitchHigh))
        {
            oldSteerSwitchHigh = steerSwitchHigh;

            if ((isAutoSteerBtnOn && ahrs.isAutoSteerAuto)
                || (!ahrs.isAutoSteerAuto && !steerSwitchHigh))
            {
                if (isSteerWorkSwitchManualSections)
                {
                    emit turnOffManulSections();
                    //mf.btnSectionMasterManual.PerformClick();
                }
                else
                {
                    emit turnOffAutoSections();
                    //mf.btnSectionMasterAuto.PerformClick();
                }
            }

            else//Checks both on-screen buttons, performs click if button is not off
            {
                emit turnOffAutoSections();
                //mf.btnSectionMasterAuto.PerformClick();
                emit turnOffManulSections();
                //mf.btnSectionMasterManual.PerformClick();
            }
        }
    }
}
