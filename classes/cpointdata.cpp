#include <QtMath>
#include "cpointdata.h"

CPointData::CPointData(double _easting, double _northing, double _lati, double _longi)
{
    //let the display do the rounding.  Rounding floating point numbers
    //doesn't make much sense, depending on the number.
    latitude = _lati, 7;
    longitude = _longi, 7;
    easting = _easting, 7;
    northing = _northing, 7;
}
