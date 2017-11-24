#ifndef CMODULECOMM_H
#define CMODULECOMM_H

#include <QString>
#include <QtCore>


class CModuleComm
{
public:
    //receive string for RateRelay
    QString serialRecvRelayRateStr;

    //properties for rate control of sections and input lines, 8 bit bytes
    static const int numRelayRateControls = 6;
    uchar relayRateControl[numRelayRateControls];
    enum relayRatecontrolItemsIndex {
        rcHeaderHi = 0, rcHeaderLo = 1, rcSectionControlByte = 2, rcSpeedXFour = 3,
        rcRateSetPointHi = 4, rcRateSetPointLo = 5
    };


    //recv for the AutoSteer module
    QString serialRecvAutoSteerStr;
    static const int numSteerDataItems = 8;
    uchar autoSteerData[numSteerDataItems];
    enum steerDataItemsIndex {
        sdHeaderHi = 0, sdHeaderLo=1, sdRelay=2, sdSpeed=3, sdDistanceHi=4,
        sdDistanceLo=5, sdSteerAngleHi=6, sdSteerAngleLo=7
    };

    static const int numSteerSettingItems = 10;
    uchar autoSteerSettings[numSteerSettingItems];
    enum steerSettingItemsIndex { ssHeaderHi = 0, ssHeaderLo = 1,
                                  ssKp = 2, ssKi = 3, ssKd = 4,
                                  ssKo = 5, ssSteerOffset = 6, ssMinPWM = 7,
                                  ssMaxIntegral = 8, ssCountsPerDegree = 9 };

    //for the workswitch
    bool isWorkSwitchOn=false, isWorkSwitchActiveLow=false, isWorkSwitchEnabled=false;
    int workSwitchValue = 0;

    //from autosteer module the a/d conversion of inclinometer
    //imu and roll inclinometer
    int steerSwitchValue, gyroHeading=9999, prevGyroHeading = 9999;
    int rollRaw = 9999; //inclinometer ?


    CModuleComm();
    void resetAllModuleCommValues();
};

#endif // CMODULECOMM_H
