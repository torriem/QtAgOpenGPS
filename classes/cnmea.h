#ifndef CNMEA_H
#define CNMEA_H

#include <sstream>
#include <QString>
#include <QDateTime>
#include <QByteArray>
#include <QBuffer>

class FormGPS;

struct XY {
    double x;
    double y;
};


class CNMEA
{
public:
    //WGS84 Lat Long
    double latitude, longitude;

    bool updatedGGA=false, updatedVTG=false, updatedRMC=false;

    QByteArray rawBuffer = "";
    QByteArray theSent = "";
    QList<QByteArray> words;
    QByteArray nextNMEASentence = "";

    //UTM coordinates
    double northing=0, easting=0;
    double actualEasting, actualNorthing;
    double zone=0;

    //other GIS Info
    double altitude, speed;
    double headingTrue, hdop, ageDiff;

    int fixQuality;
    int satellitesTracked;
    QByteArray status = "q";
    QDateTime utcDateTime;

    char hemisphere = 'N';

    //UTM numbers are huge, these cut them way down.
    int utmNorth, utmEast;
    QByteArray logNMEASentence;

    QByteArray currentNMEASentenceGGA = "";
    QByteArray currentNMEASentenceRMC = "";
    QByteArray currentNMEASentenceVTG = "";

    FormGPS *mf;

    CNMEA(FormGPS *mf);
    void parseNMEA();
    QByteArray parse();
    void parseGGA();
    void parseRMC();
    void parseVTG();
    bool validateChecksum(QByteArray sentence);
    double distance(double northing1, double easting1,
                    double northing2, double easting2);
    double distanceSquared(double northing1, double easting1,
                           double northing2, double easting2);
    void decDeg2UTM();
    double arcLengthOfMeridian(double phi);
    XY mapLatLonToXY(double phi, double lambda, double lambda0);
    void geoUTMConverterXY(double lat, double lon);

};

#endif // CNMEA_H
