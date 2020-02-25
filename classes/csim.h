#ifndef CSIM_H
#define CSIM_H

#include <QString>
#include <QStringList>
#include <QtMath>
#include <QDateTime>
#include <QDebug>


#define ANG_TO_RAD 57.2957795

using namespace std;

class CSim : public QObject
{
    Q_OBJECT

public:
    CSim();

    void CalculateNewPositionFromBearingDistance(double lat, double lng, double bearing, double distance);
    void CalculateCheckSum(QString Sentence);
    void BuildGGA();
    void BuildVTG();
    void DoSimTick(double _st);

private:
    QString sbHDT;
    QString sbOGI;
    QString sbGGA;
    QString sbVTG;

    // The entire string to send out

    QByteArray sbSendText;

    // GPS related properties
    const double simLat = 35.990525;
    const double simLon = 127.0197155;
    QChar EW = 'W';
    QChar NS = 'N';
    double latitude, longitude;
    double latDeg, latMinu, longDeg, longMinu, latNMEA, longNMEA;
    double speed = 0.6, headingTrue, stepDistance = 0.2, steerAngle;
    double steerAngleScrollBar = 0;
    double degrees;
    const int fixQuality = 5, sats = 12;
    const double HDOP = 0.9;
    double altitude = 300;

    // The checksum of an NMEA line
    QString sumStr = "";
    int counterForStar = 0;

    QDateTime time;
signals:
    void new_position(QByteArray nmea_data);

};

#endif // CSIM_H
