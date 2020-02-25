#ifndef CPOINTDATA_H
#define CPOINTDATA_H


class CPointData
{
public:
    //WGS84 Lat Long
    double latitude = 0;

    double longitude = 0;

    //UTM coordinates
    double northing = 0;

    double easting = 0;

    //other GIS Info
    //public double altitude = 0;
    //public double speed = 0;
    //public double headingTrue = 0;
    //public DateTime utcDateTime;

    //public CPointData(double latitude, double longitude, double northing, double easting, double altitude, double speed, double headingTrue, DateTime utcDateTime)
    CPointData(double _easting, double _northing, double _lati, double _longi);

};

#endif // CPOINTDATA_H
