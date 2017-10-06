#include "cvec.h"

CVec::CVec(double x, double z, double h, int s, int p) :
    x(x), z(z), h(h), strip(s), pt(p)
{
}

CVec::CVec(const Vec3 &v) {
    x = v.easting;
    z = v.northing;
    h = v.heading;
    strip = 99999;
    pt = 99999;
}

