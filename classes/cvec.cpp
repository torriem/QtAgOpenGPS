#include "cvec.h"

CVec::CVec()
    :x(0), z(0), h(0),strip(0),pt(0)
{

}
CVec::CVec(double x, double z, double h, int s, int p)
    :x(x), z(z), h(h), strip(s), pt(p)
{
}

CVec::CVec(const Vec3 &v) {
    x = v.easting;
    z = v.northing;
    h = v.heading;
    strip = 99999;
    pt = 99999;
}

