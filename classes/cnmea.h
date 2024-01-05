#ifndef CNMEA_H
#define CNMEA_H

#include <QObject>
#include <sstream>
#include <QString>
#include <QDateTime>
#include <QByteArray>
#include <QBuffer>
#include "vec2.h"
#include "glm.h"
#include <QTextStream>

class CNMEA : public QObject
{
    Q_OBJECT
private:
public:
    //WGS84 Lat Long
    double latitude, longitude;

    double prevLatitude, prevLongitude;

    //local plane geometry
    double latStart, lonStart;

    double mPerDegreeLat, mPerDegreeLon;

    //our current fix
    //moved to CVehicle
    Vec2 fix = Vec2(0, 0);

    Vec2 prefSpeedFix= Vec2(0, 0);

    //used to offset the antenna position to compensate for drift
    Vec2 fixOffset = Vec2(0, 0);

    //other GIS Info
    double altitude, speed, newSpeed, vtgSpeed = glm::DOUBLE_MAX;

    double headingTrueDual, headingTrue, hdop, age, headingTrueDualOffset;

    int fixQuality, ageAlarm;
    int satellitesTracked;

    QString logNMEASentence;

    //StringBuilder logNMEASentence = new StringBuilder();

    explicit CNMEA(QObject *parent = 0);

    //
    //void AverageTheSpeed(); moved to CVehicle
    void SetLocalMetersPerDegree();
    void ConvertWGS84ToLocal(double Lat, double Lon, double &outNorthing, double &outEasting);
    void ConvertLocalToWGS84(double Northing, double Easting, double &outLat, double &outLon);
    QString GetLocalToWSG84_KML(double Easting, double Northing);

    void loadSettings(void);

signals:
    //void setAveSpeed(double);
    void checkZoomWorldGrid(double, double);
};

#endif // CNMEA_H
