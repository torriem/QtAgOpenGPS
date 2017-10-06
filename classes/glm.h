#ifndef GLM_H
#define GLM_H

#include <math.h>

//inches to meters
static const double in2m = 0.0254;

//meters to inches
static const double m2in = 39.3701;

//meters to feet
static const double m2ft = 3.28084;

//the pi's
static const double twoPI = 6.28318530717958647692;

static const double PIBy2 = 1.57079632679489661923;

static inline float acos(float x)
{
    return acosf(x);
}

static inline float acosh(float x)
{
    return acoshf(x);
}

static inline float asin(float x)
{
    return asinf(x);
}

static inline float asinh(float x)
{
    return asinhf(x);
}

static inline float atan(float y, float x)
{
    return atan2f(y,x);
}

static inline float atan(float y_over_x)
{
    return atanf(y_over_x);
}

static inline float atanh(float x)
{
    return atanhf(x);
}

static inline float cos(float angle)
{
    return cosf(angle);
}

static inline float cosh(float angle)
{
    return coshf(angle);
}

static inline float toDegrees(float radians)
{
    return radians * 180.0 / M_PI;
}

static inline double toDegrees(double radians)
{
    return radians * 180.0 / M_PI;
}

static inline float toRadians(float degrees)
{
    return degrees * M_PI / 180.0;
}

static inline double toRadians(double degrees)
{
    return degrees * M_PI / 180.0;
}

static inline float sin(float angle)
{
    return sinf(angle);
}

static inline float sinh(float angle)
{
    return sinhf(angle);
}

static inline float tan(float angle)
{
    return tanf(angle);
}

static inline float tanh(float angle)
{
     return tanhf(angle);
}

#endif // GLM_H
