#ifndef CFLAG_H
#define CFLAG_H


#include <QString>

//color of flag - 0 is RED, 1 is green, 2 is PURPLE
enum FLAG_COLOR { FLAG_RED, FLAG_GREEN, FLAG_PURPLE };

class CFlag
{
public:
    //WGS84 Lat Long
    double latitude = 0.0;
    double longitude = 0.0;

    //UTM coordinates
    double easting = 0.0;
    double northing = 0.0;

    double heading = 0.0;

    //color of flag - 0 is RED, 1 is green, 2 is PURPLE
    int color = FLAG_RED;
    int ID = 0;
    QString notes;

    CFlag(double &latitude, double &longitude,
          double &easting, double &northing,
          double &heading,
          int &color, int &ID, QString &notes);
    CFlag();
    CFlag(const CFlag &);
};

#endif // CFLAG_H
