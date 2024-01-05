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
    //Roll and heading from the IMU
    double imuHeading = 99999, prevIMUHeading = 0, imuRoll = 0, imuPitch = 0, imuYawRate = 0;

    int angVel;

    //torriem: these are in properties, not sure why they must be here too
    //actual value in degrees
    double rollZero;
    //property_setIMU_rollZero

    //Roll Filter Value
    double rollFilter;
    //property_setIMU_rollFilter

    //is the auto steer in auto turn on mode or not
    bool isAutoSteerAuto, isRollInvert, isDualAsIMU, isReverseOn;
    //property_setAS_isAutoSteerAutoOn
    //property_setIMU_invertRoll
    //property_setIMU_isDualAsIMU
    //property_setIMU_isReverseOn


    //the factor for fusion of GPS and IMU
    double forwardComp, reverseComp, fusionWeight;
    //property_setGPS_forwardComp
    //property_setGPS_reverseComp
    //property_setIMU_fusionWeight

    //flags for desired sources
    //Roll and heading from the IMU
    int correctionHeadingX16 = 9999, prevCorrectionHeadingX16 = 9999, rollX16 = 9999;

    explicit CAHRS(QObject *parent = 0);
    void loadSettings();

signals:

public slots:
};

#endif // CAHRS_H
