#ifndef CNMEA_H
#define CNMEA_H

#include <QObject>
#include <sstream>
#include <QString>
#include <QDateTime>
#include <QByteArray>
#include <QBuffer>
#include "vec2.h"

class Vec2;

/*struct XY {
    double easting;
    double northing;
};
*/

class CVehicle;


class CNMEA : public QObject
{
    Q_OBJECT
private:
    QList<QByteArray> words;
    QByteArray nextNMEASentence = "";
    int nmeaCntr = 0;

    double rollK, Pc, G, Xp, Zp, XeRoll;
    double P = 1.0;
    const double varRoll = 0.1; // variance, smaller, more faster filtering
    const double varProcess = 0.0003;

    //Some methods require these items to be passed in from outside
    double lastHeading;
    double roll;

public:
    //WGS84 Lat Long
    double latitude = 0, longitude = 0;

    double latStart, lonStart;
    double actualEasting=0, actualNorthing=0;
    double zone=0;
    double centralMeridian, convergenceAngle = 0;

    bool updatedGGA=false, updatedOGI=false, updatedRMC=false;

    bool isFirstFixPositionSet = false;

    QByteArray rawBuffer = "";
    QString fixFrom;

    //UTM coordinates
    Vec2 fix = Vec2(0,0);

    //used to offset the antenna position to compensate for drift
    Vec2 fixOffset = Vec2(0,0);

    //other GIS Info
    double altitude, speed;
    double headingTrue, headingHDT, hdop, ageDiff;

    //imu
    double nRoll, nPitch, nYaw, nAngularVelocity;

    bool isValidIMU;
    int fixQuality;
    int satellitesTracked;
    QByteArray status = "q";
    QDateTime utcDateTime;
    char hemisphere = 'N';

    //UTM numbers are huge, these cut them way down.
    int utmNorth = 0, utmEast = 0;
    QByteArray logNMEASentence = "";

    explicit CNMEA(QObject *parent = 0);
    void updateNorthingEasting();
    void parseNMEA(double lastHeading, double roll);
    void parseAVR();
    QByteArray parse();

    void parseGGA();
    void parseOGI();
    void parseVTG();
    void parseHDT();
    void parseTRA();
    void parseRMC();
    bool validateChecksum(QByteArray sentence);

    Vec2 decDeg2UTM(double latitude, double longitude);
    static double arcLengthOfMeridian(double phi);
    static Vec2 mapLatLonToXY(double phi, double lambda, double lambda0);
signals:
    void setRollX16(int); //set mf.ahrs.rollX16
    void setCorrectionHeadingX16(int); //set mf.ahrs.correctionHeadingX16
    void setRollUsed(double);
    void setRollCorrectionDistance(double);
    void headingSource(int);
    void clearRecvCounter(); //tell watchdog to reset it's counter
    void newSpeed(double); //so display can average this into it's speed

};

#endif // CNMEA_H
