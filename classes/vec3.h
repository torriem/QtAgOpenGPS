#ifndef VEC3_H
#define VEC3_H


class Vec3
{
public:
    double easting;
    double northing;
    double heading;

    Vec3();
    Vec3(double easting, double northing, double heading);
    Vec3(const Vec3 &v);

    double headingXZ();
    void normalize();
    double getLength();
    double getLengthSquared();

    friend Vec3 operator- (Vec3 lhs, Vec3 rhs);
};

#endif // VEC3_H
