#ifndef CNMEA_H
#define CNMEA_H

#include <sstream>
#include <QString>
#include <QDateTime>
#include <QByteArray>
#include <QBuffer>
#include <QObject>
#include "vec2.h"


struct XY {
    double x;
    double y;
};
class FormGPS;

class CNMEA : public QObject
{
    Q_OBJECT
public:
    CNMEA(FormGPS *mainForm);
    //WGS84 Lat Long
    double latitude = 0, longitude = 0;

    bool updatedGGA=false, updatedVTG=false, updatedRMC=false;
    double centralMeridian, convergenceAngle;
    QString fixForm;

    QByteArray rawBuffer = "";
    QList<QByteArray> words;
    QByteArray nextNMEASentence = "";

    //UTM coordinates
    double northing=0, easting=0;
    double actualEasting=0, actualNorthing=0;
    double zone=0;

    Vec2 *fix;
    //used to offset the antenna position to compensate for drift
    Vec2 *fixOffset;

    //other GIS Info
    double altitude, speed;
    double headingTrue, hdop, ageDiff;

    int fixQuality;
    int satellitesTracked;
    QByteArray status = "q";
    QDateTime utcDateTime;

    char hemisphere = 'N';

    //UTM numbers are huge, these cut them way down.
    int utmNorth = 0, utmEast = 0;
    QByteArray logNMEASentence = "";

    QByteArray currentNMEASentenceGGA = "";
    QByteArray currentNMEASentenceRMC = "";
    QByteArray currentNMEASentenceVTG = "";

    void parseNMEA();
    QByteArray parse();
    void parseGGA();
    void parseRMC();
    void parseVTG();
    bool validateChecksum(QByteArray sentence);
    //TODO move these to utility module
    static double distance(double northing1, double easting1,
                    double northing2, double easting2);
    static double distanceSquared(double northing1, double easting1,
                           double northing2, double easting2);
    void decDeg2UTM();
    double arcLengthOfMeridian(double phi);
    XY mapLatLonToXY(double phi, double lambda, double lambda0);
    void geoUTMConverterXY(double lat, double lon);
    void UpdateNorthingEasting();
private:
    FormGPS *mf;

};

#endif // CNMEA_H
