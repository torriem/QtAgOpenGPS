#ifndef CVEC_H
#define CVEC_H

#include <vec3.h>

class CVec
{
public:
    double x;
    double z;
    double h;
    int strip;
    int pt;

    CVec();
    CVec(double x, double z, double h, int s, int p);
    CVec(const Vec3 &v);
};

#endif // CVEC_H
