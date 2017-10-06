#include "vec4.h"

Vec4::Vec4()
{
}

Vec4::Vec4(double x, double y, double z, double k) :
    x(x), y(y), z(z), k(k)
{
}

Vec4::Vec4(const Vec4 &src) {
    x=src.x;
    y=src.y;
    z=src.z;
    k=src.k;
}
