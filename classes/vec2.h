#ifndef VEC2_H
#define VEC2_H


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
#endif

    double headingXZ();
    void normalize();
    double getLength();
    double getLengthSquared();

    friend Vec2 operator+ (Vec2 lhs, Vec2 rhs);
};


#endif // VEC2_H
