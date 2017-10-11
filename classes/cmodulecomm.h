#ifndef CMODULECOMM_H
#define CMODULECOMM_H

#include <QString>
#include <QtCore>


class CModuleComm
{
public:
    //properties for relay control of sections and input lines, 8 bit bytes
    static const int numRelayControls = 1;
    uchar relaySectionControl[numRelayControls];

    QString serialRecvRelayStr = "";

    //public double angularVelocity = 0;
    //public double imuHeading = 0;

    static const int numSteerDataItems = 8;
    enum steerDataItemsIndex { sdHeaderHi = 0, sdHeaderLo=1,
                               sdRelay=2, sdSpeed=3, sdDistanceHi=4,
                               sdDistanceLo=5, sdSteerAngleHi=6,
                               sdSteerAngleLo=7 };

    static const int numSteerSettingItems = 10;
    enum steerSettingItemsIndex { ssHeaderHi = 0, ssHeaderLo = 1,
                                  ssKp = 2, ssKi = 3, ssKd = 4,
                                  ssKo = 5, ssSteerOffset = 6, ssMinPWM = 7,
                                  ssMaxIntegral = 8, ssCountsPerDegree = 9 };


    //control for the auto steer module

    uchar autoSteerData[numSteerDataItems];
    uchar autoSteerSettings[numSteerSettingItems];
    QString serialRecvAutoSteerStr = "";

    //from autosteer module the a/d conversion of inclinometer
    int rollRaw = 0;

    //for the workswitch
    bool isWorkSwitchOn=false, isWorkSwitchActiveLow=false, isWorkSwitchEnabled=false;
    int workSwitchValue = 0;

    CModuleComm();
};

#endif // CMODULECOMM_H
