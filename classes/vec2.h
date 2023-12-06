#ifndef VEC2_H
#define VEC2_H

#include <QVector3D>

class Vec2
{
public:
    double easting;
    double northing;

    Vec2();
    Vec2(double easting, double northing);
    Vec2(const Vec2 &src);

#ifdef DEBUG_VEC
    ~Vec2();
#else
    ~Vec2() = default;
#endif

    double headingXZ();
    void normalize();
    double getLength();
    double getLengthSquared();

    friend Vec2 operator+ (Vec2 lhs, Vec2 rhs);
    friend Vec2 operator- (Vec2 lhs, Vec2 rhs);
    Vec2 operator*= (double rhs);
    Vec2 &operator=(const Vec2 &other) = default;

    operator QVector3D() { return QVector3D(easting,northing,0); }
};


#endif // VEC2_H
