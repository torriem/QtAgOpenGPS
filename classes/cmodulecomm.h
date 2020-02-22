#ifndef CMODULECOMM_H
#define CMODULECOMM_H

#include <QObject>
#include <QtCore>
#include <QString>

const int pgnSentenceLength = 10;

class CModuleComm
{
    Q_OBJECT
private:
    bool isOutOfBounds = true;

public:
    //receive string for RateRelay
    QString serialRecvAutoSteerStr;
    QString serialRecvRelayStr;

    QByteArray pgn;

    //RateRelay
    enum relayRateItemsIndex {
        azSteerData = 0,
        azSteerSettings = 1,
        azRelayData = 2,
        azRelaySettings = 3,
        azMachineControl = 4
    };

    //AutoSteer

    // PGN - 32766 - 127.254 0x7FFD
    uchar autoSteerData[pgnSentenceLength];
    enum steerDataItemsIndex {
        sdHeaderHi = 0, sdHeaderLo=1, sdRelayLo=2, sdSpeed=3, sdDistanceHi=4,
        sdDistanceLo=5, sdSteerAngleHi=6, sdSteerAngleLo=7, sdYouTurnByte = 8, sd8 = 9
    };

    // PGN - 32764 - 127.252 0x7FFC
    uchar autoSteerSettings[pgnSentenceLength];
    enum steerSettingItemsIndex { ssHeaderHi = 0, ssHeaderLo = 1,
                                  ssKp = 2, ssKi = 3, ssKd = 4,
                                  ssKo = 5, ssSteerOffset = 6, ssMinPWM = 7,
                                  ssMaxIntegral = 8, ssCountsPerDegree = 9 };

    // PGN - 32762 - 127.250 0x7FFA
    uchar relayData[pgnSentenceLength];
    enum relayItemsIndex {
        rdHeaderHi=0, rdHeaderLo = 1, rdSectionControlByteHi = 2, rdSectionControlByteLo = 3,
        rdSpeedXFour = 4, rdTramLine = 5, rdTree = 6, rdUTurn = 7, rdHydLift = 8, rd9 = 9
    };

    //Machine control
    //PGN 32758 - 127.246 0x7FF6
    uchar machineControlData[pgnSentenceLength];
    enum machnineControlDataItems {
        cnHeaderHi, cnHeaderLo = 1, cnPedalControl = 2, cnSpeed = 3, cnRelayLo = 4, cnYouTurn = 5
    };

    //LIDAR
    //UDP sentence just rec'd
    QString recvUDPSentence = "Initial UDP";

    int lidarDistance;

    //for the workswitch
    bool isWorkSwitchActiveLow=false, isWorkSwitchEnabled=false, isWorkSwitchManual = false;
    int workSwitchValue = 1, steerSwitchValue ;

    CModuleComm();
    void resetAllModuleCommValues();
signals:
    void sendRelayOutToPort(uchar *, int);
    void sendAutoSteerDataOutToPort();
    void sendAutoSteerSettingsOutToPort();
};

#endif // CMODULECOMM_H
