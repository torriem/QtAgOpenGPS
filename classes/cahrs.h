#ifndef CAHRS_H
#define CAHRS_H

#include <QObject>

const double varRoll = 0.1; // variance, smaller, more faster filtering
const double varProcess = 0.0003;

class CAHRS : public QObject
{
    Q_OBJECT
private:
    //kalman filtering variables for Roll
    double rollK, Pc, G, Xp, Zp, XeRoll;
    double P = 1.0;

public:
    //flags for desired sources
    bool isHeadingCorrectionFromAutoSteer, isHeadingCorrectionFromBrick, isHeadingCorrectionFromExtUDP;
    bool isRollFromAutoSteer, isRollFromGPS, isRollFromExtUDP;
    //Roll and heading from the IMU
    int correctionHeadingX16 = 9999, prevCorrectionHeadingX16 = 9999, rollX16 = 9999;

    //actual value in degrees* 16 to modify the imu*16 values
    int rollZeroX16, pitchZeroX16;

    //is the auto steer in auto turn on mode or not
    bool isAutoSteerAuto;

    explicit CAHRS(QObject *parent = 0);

signals:

public slots:
    void setRollX16(int);
    void setCorrectionHeadingX16(int);
    void setRollZeroX16(int nRoll);
    void setIsHeadingCorrectionFromAutoSteer(bool);
    void setIsHeadingCorrectionFromBrick(bool);
    void setIsHeadingCorrectionFromExtUDP(bool);
    void setIsRollFromAutoSteer(bool);
    void setIsRollFromGPS(bool);
    void setIsRollFromExtUDP(bool);
    void setIsAutoSteerOn(bool);
};

#endif // CAHRS_H
