#ifndef VEC2FIX2FIX_H
#define VEC2FIX2FIX_H

class Vec2Fix2Fix
{
public:
    double easting; //easting
    double distance; //distance since last point
    double northing; //norting
    int isSet;    //altitude

    Vec2Fix2Fix(double _easting, double _northing, double _distance, int _isSet) :
        easting(_easting), distance(_distance), northing(_northing), isSet(_isSet)
    { }

};

#endif // VEC2FIX2FIX_H
