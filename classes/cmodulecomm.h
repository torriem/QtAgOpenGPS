#ifndef CMODULECOMM_H
#define CMODULECOMM_H

#include <QObject>
#include <QtCore>
#include <QString>

const int pgnSentenceLength = 10;

class CModuleComm: public QObject
{
    Q_OBJECT

public:
    bool isOutOfBounds = true;
    //receive string for RateRelay
    QString serialRecvAutoSteerStr;
    QString serialRecvMachineStr;

    bool isMachineDataSentToAutoSteer;

    //AutoSteer

    // PGN - 32766 - 127.254 0x7FFD
    uchar autoSteerData[pgnSentenceLength];
    enum steerDataItemsIndex {
        sdHeaderHi = 0, sdHeaderLo=1, sd2=2, sdSpeed=3, sdDistanceHi=4,
        sdDistanceLo=5, sdSteerAngleHi=6, sdSteerAngleLo=7, sd8 = 8, sd9 = 9
    };

    //AutoSteer Basic Setting
    // PGN - 32764 - 127.252 0x7FFC
    uchar autoSteerSettings[pgnSentenceLength];
    enum steerSettingItemsIndex { ssHeaderHi = 0, ssHeaderLo = 1,
                                  ssKp = 2, ssKi = 3, ssKd = 4,
                                  ssKo = 5, ssSteerOffset = 6, ssMinPWM = 7,
                                  ssMaxIntegral = 8, ssCountsPerDegree = 9 };

    //Arduino Steer Config
    //PGN - 32763 - 127.251 0x7FFB
    uchar ardSteerConfig[pgnSentenceLength];
    enum arduinoSteerItems { arHeaderHi = 0, arHeaderLo = 1, arSet0 = 2, arSet1 = 3, arMaxSpd = 4, arMinSpd = 5, arIncMaxPulse = 6,
        arAckermanFix = 7, ar8 = 8, ar9 = 9  };

     //Machine Module Data ------------------------------------------------------------------------------------
    // PGN - 32762 - 127.250 0x7FFA
    uchar machineData[pgnSentenceLength];
    enum machineDataItems { mdHeaderHi=0, mdHeaderLo = 1, mdSectionControlByteHi = 2, mdSectionControlByteLo = 3,
        mdSpeedXFour = 4, mdUTurn = 5, mdTree = 6, mdHydLift = 7, md8 = 8, md9 = 9 };

    // ---- Arduino configuration on machine module  ---------------------------------------------------------
    //PGN - 32760 - 127.248 0x7FF9
    uchar ardMachineConfig[pgnSentenceLength];
    enum ardConfigItems { amHeaderHi=0 , amHeaderLo = 1, amRaiseTime = 2, amLowerTime = 3, amEnableHyd = 4,
         am5 = 5, am6 = 6, am7 = 7, am8 = 8, am9 = 9 };

    // ---- Section control switches to AOG  ---------------------------------------------------------
    //PGN - 32761 - 127.224 0x7FE0
    uchar ss[pgnSentenceLength];
    uchar ssP[pgnSentenceLength];
    enum sectionContolItems {swHeaderHi=0, swHeaderLo = 1, sw2 = 2, sw3 = 3, sw4 = 4,
         swONHi = 5, swONLo = 6, swOFFHi = 7, swOFFLo = 8, swMain = 9 };



    //LIDAR
    //UDP sentence just rec'd
    QString recvUDPSentence = "Initial UDP";

    int lidarDistance;

    //for the workswitch
    //bool isWorkSwitchActiveLow=false, isWorkSwitchEnabled=false, isWorkSwitchManual = false; //in settings
    int workSwitchValue = 0, steerSwitchValue = 1 ;

    explicit CModuleComm(QObject *parent = 0);
    void resetAllModuleCommValues();
signals:
    void sendOutUSBMachinePort(uchar *, int);
    void sendOutUSBAutoSteerPort(uchar *, int);
public slots:
    void setOutOfBounds() {
        isOutOfBounds = true;
    }

    void setHydLift(int);
};

#endif // CMODULECOMM_H
