#ifndef VEC4_H
#define VEC4_H


class Vec4
{
public:
    double x;
    double y;
    double z;
    double k;

    Vec4();
    Vec4(double x, double y, double z, double k);
    Vec4(const Vec4& src);
};

#endif // VEC4_H
