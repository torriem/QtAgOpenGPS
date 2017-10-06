#include "vec3.h"
#include <math.h>
#include <assert.h>

Vec3::Vec3()
    :easting(0),northing(0),heading(0)
{
}
Vec3::Vec3(double easting, double northing, double heading):
    easting(easting),northing(northing),heading(heading)
{
}

Vec3::Vec3(const Vec3 &v) {
    easting=v.easting;
    northing=v.northing;
    heading=v.heading;
}

double Vec3::headingXZ() {
    return atan2(easting, northing);
}

void Vec3::normalize() {
    double length = getLength();

    //TODO: throw exception if length is 0
    assert(fabs(length)>0);

    easting /= length;
    northing /= length;
    heading /= length;
}

double Vec3::getLength() {
    return sqrt(easting * easting +
                heading * heading +
                northing *northing);
}

double Vec3::getLengthSquared() {
    return easting * easting +
           heading * heading +
           northing * northing;
}

Vec3 operator- (Vec3 lhs, Vec3 rhs) {
    return Vec3(lhs.easting - rhs.easting,
                lhs.northing - rhs.northing,
                lhs.heading - rhs.heading);
}


