#include "cflag.h"

CFlag::CFlag(double latitude, double longitude,
             double easting, double northing,
             int color, int ID)
    :latitude(latitude), longitude(longitude),
    easting(easting), northing(northing),
    color(color),ID(ID)
{
}

CFlag::CFlag()
{
}

CFlag::CFlag(const CFlag& src) {
    latitude = src.latitude;
    longitude = src.longitude;
    easting = src.easting;
    northing = src.northing;
}
