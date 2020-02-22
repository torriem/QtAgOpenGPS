#ifndef CAHRS_H
#define CAHRS_H

#include <QObject>

class CAHRS : public QObject
{
    Q_OBJECT
public:
    //flags for desired sources
    bool isHeadingFromAutoSteer, isHeadingFromBrick, isHeadingFromPAOGI, isHeadingFromExtUDP;
    bool isRollFromAutoSteer, isRollFromBrick, isRollFromGPS, isRollFromExtUDP;
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
    void setRollZeroX16(int);
    void setIsHeadingFromAutoSteer(bool);
    void setIsHeadingFromBrick(bool);
    void setIsHeadingFromPAOGI(bool);
    void setIsHeadingFromExtUDP(bool);
    void setIsRollFromAutoSteer(bool);
    void setIsRollFromBrick(bool);
    void setIsRollFromGPS(bool);
    void setIsRollFromExtUDP(bool);
    void setIsAutoSteerOn(bool);
};

#endif // CAHRS_H
