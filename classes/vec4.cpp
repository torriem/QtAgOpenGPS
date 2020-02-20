#include "vec4.h"

Vec4::Vec4()
{
}

Vec4::Vec4(double _easting, double _northing, double _heading, int _index) :
    easting(_easting), northing(_northing), heading(_heading), index(_index)
{
}

Vec4::Vec4(const Vec4 &src) {
    easting=src.easting;
    northing=src.northing;
    heading=src.heading;
    index=src.index;
}
