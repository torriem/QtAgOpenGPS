#ifndef CFLAG_H
#define CFLAG_H


//color of flag - 0 is RED, 1 is green, 2 is PURPLE
enum FLAG_COLOR { FLAG_RED, FLAG_GREEN, FLAG_PURPLE };

class CFlag
{
public:
    //WGS84 Lat Long
    double latitude = 0.0;
    double longitude = 0.0;

    //UTM coordinates
    double northing = 0.0;
    double easting = 0.0;

    //color of flag - 0 is RED, 1 is green, 2 is PURPLE
    int color = FLAG_RED;
    int ID = 0;

    CFlag(double latitude, double longitude,
          double easting, double northing,
          int color, int ID);
};

#endif // CFLAG_H
