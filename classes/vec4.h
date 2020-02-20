#ifndef VEC4_H
#define VEC4_H


class Vec4
{
public:
    double easting;
    double heading;
    double northing;
    int index;

    Vec4();
    Vec4(double _easting, double _northing, double _heading, int _index);
    Vec4(const Vec4& src);
};

#endif // VEC4_H
