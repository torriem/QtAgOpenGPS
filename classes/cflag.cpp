#include "cflag.h"

CFlag::CFlag(double &latitude, double &longitude,
             double &easting, double &northing, double &heading,
             int &color, int &ID, QString &notes)
    :latitude(latitude), longitude(longitude),
    easting(easting), northing(northing),
    heading(heading),
    color(color),ID(ID), notes(notes)
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
    heading = src.heading;
    ID = src.ID;
    color = src.color;
    notes = src.notes;
}
