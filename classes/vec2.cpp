#include "vec2.h"
#include <math.h>
#include <assert.h>

#ifdef DEBUG_VEC
#include <iostream>
#endif

Vec2::Vec2()
    : easting(0),northing(0)
{
#ifdef DEBUG_VEC
    std::cout << "vec2 empty construction" << std::endl;
#endif
}

Vec2::Vec2(double easting, double northing)
    : easting(easting),northing(northing)
{
#ifdef DEBUG_VEC
    std::cout << "vec2 init construction" << std::endl;
#endif
}

Vec2::Vec2(const Vec2 &src) {
    easting = src.easting;
    northing = src.northing;

#ifdef DEBUG_VEC
    std::cout << "vec2 copy construction" << std::endl;
#endif

}
#ifdef DEBUG_VEC
Vec2::~Vec2()
{
    std::cout << "vec2 destroyed" << std::endl;
}
#endif

Vec2 operator+ (Vec2 lhs, Vec2 rhs) {
    return Vec2(lhs.easting+rhs.easting,
                lhs.northing+rhs.northing);
}

Vec2 operator- (Vec2 lhs, Vec2 rhs) {
    return Vec2(lhs.easting-rhs.easting,
                lhs.northing-rhs.northing);
}

double Vec2::headingXZ() {
    return atan2(easting,northing);
}

void Vec2::normalize() {
    double length = getLength();

    //if (fabs(length) == 0) {
    //	throw new DivideByZeroException("Trying to normalize a vector with length of zero.");
    //}
    assert(fabs(length) > 0);

    easting /= length;
    northing /= length;
}

double Vec2::getLength() {
    return sqrt(easting * easting + northing * northing);
}

double Vec2::getLengthSquared() {
    return easting * easting + northing * northing;
}





