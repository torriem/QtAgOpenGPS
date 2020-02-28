#include <QtMath>
#include "cpointdata.h"

CPointData::CPointData(double _easting, double _northing, double _lati, double _longi)
{
    //let the display do the rounding.  Rounding floating point numbers
    //doesn't make much sense, depending on the number.
    latitude = _lati;
    longitude = _longi;
    easting = _easting;
    northing = _northing;
}
