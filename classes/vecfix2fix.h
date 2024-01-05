#ifndef VECFIX2FIX_H
#define VECFIX2FIX_H

class VecFix2Fix
{
public:
    double easting; //easting
    double distance; //distance since last point
    double northing; //norting
    int isSet;    //altitude


    VecFix2Fix() : easting(0), distance(0), northing(0), isSet(0) {}
    VecFix2Fix(double _easting, double _northing, double _distance, int _isSet):
        easting(_easting), distance(_distance), northing(_northing), isSet(_isSet)
    { }

};

#endif // VECFIX2FIX_H
