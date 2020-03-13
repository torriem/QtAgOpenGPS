#ifndef GLM_H
#define GLM_H

#include <math.h>
#include <limits>
#include "vec2.h"
#include "vec3.h"


#ifndef M_PI
#  ifndef PI
#     define M_PI 3.14159265358979323846
#  else
#     define M_PI PI
#  endif
#endif

namespace glm {
    static const double DOUBLE_EPSILON=std::numeric_limits<double>::epsilon();
    //inches to meters
    static const double in2m = 0.0254;

    //meters to inches
    static const double m2in = 39.3701;

    //meters to feet
    static const double m2ft = 3.28084;

    //the pi's
    static const double twoPI = 6.28318530717958647692;

    static const double PIBy2 = 1.57079632679489661923;

    //Hectare to Acres
    static const double ha2ac = 2.47105;

    //Acres to Hectare
    static const double ac2ha = 0.404686;

    //Meters to Acres
    static const double m2ac = 0.000247105;

    //Meters to Hectare
    static const double m2ha = 0.0001;

    // liters per hectare to us gal per acre
    static const double galAc2Lha = 9.35396;

    //us gal per acre to liters per hectare
    static const double LHa2galAc = 0.106907;

    //Liters to Gallons
    static const double L2Gal = 0.264172;

    //Gallons to Liters
    static const double Gal2L = 3.785412534258;

    //Distance calcs of all kinds
    static inline double distance(double east1, double north1, double east2, double north2)
    {
        return sqrt( (east1 - east2) * (east1 - east2) +
                     (north1 - north2) * (north1 - north2) );
    }

    static inline double distance(Vec2 first, Vec2 second)
    {
        return sqrt(pow(first.easting - second.easting, 2) +
                    pow(first.northing - second.northing, 2));
    }

    static inline double distance(Vec2 first, Vec3 second)
    {
        return sqrt(pow(first.easting - second.easting, 2) +
                    pow(first.northing - second.northing, 2));
    }

    static inline double distance(Vec3 first, Vec2 second)
    {
        return sqrt(pow(first.easting - second.easting, 2) +
                    pow(first.northing - second.northing, 2));
    }

    static inline double distance(Vec3 first, Vec3 second)
    {
        return sqrt(pow(first.easting - second.easting, 2) +
                    pow(first.northing - second.northing, 2));
    }

    static inline double distance(Vec2 first, double east, double north)
    {
        return sqrt(pow(first.easting - east, 2)+
                    pow(first.northing - north, 2));
    }

    static inline double distance(Vec3 first, double east, double north)
    {
        return sqrt(pow(first.easting - east, 2) +
                    pow(first.northing - north, 2));
    }

    //not normalized distance, no square root
    static inline double distanceSquared(double northing1, double easting1, double northing2, double easting2)
    {
        return pow(easting1 - easting2, 2) + pow(northing1 - northing2, 2);
    }

    static inline double distanceSquared(Vec3 first, Vec2 second)
    {
        return (pow(first.easting - second.easting, 2) +
                pow(first.northing - second.northing, 2));
    }

    static inline double distanceSquared(Vec2 first, Vec3 second)
    {
        return (pow(first.easting - second.easting, 2) +
                pow(first.northing - second.northing, 2));
    }

    static inline double distanceSquared(Vec3 first, Vec3 second)
    {
        return (pow(first.easting - second.easting, 2) +
                pow(first.northing - second.northing, 2));
    }
    static inline double distanceSquared(Vec2 first, Vec2 second)
    {
        return (pow(first.easting - second.easting, 2) +
                pow(first.northing - second.northing, 2));
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

    static inline double roundMidAwayFromZero(double number) {
        return lround(number);
    }


}
#endif // GLM_H
