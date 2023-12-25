#include <QCoreApplication>
#include <math.h>
#include "cnmea.h"
#include "vec2.h"
#include "glm.h"
#include "aogproperty.h"


CNMEA::CNMEA(QObject *parent) : QObject(parent)
{
    latStart = 0;
    lonStart = 0;
    loadSettings();
}

void CNMEA::loadSettings(void)
{
    ageAlarm  = property_setGPS_ageAlarm;
}

//moved to CVehicle
/*
void CNMEA::AverageTheSpeed()
{
    mf.avgSpeed = (mf.avgSpeed * 0.75) + (speed * 0.25);
}
*/

void CNMEA::SetLocalMetersPerDegree()
{
    mPerDegreeLat = 111132.92 - 559.82 * cos(2.0 * latStart * 0.01745329251994329576923690766743) + 1.175
                          * cos(4.0 * latStart * 0.01745329251994329576923690766743) - 0.0023
                          * cos(6.0 * latStart * 0.01745329251994329576923690766743);

    mPerDegreeLon = 111412.84 * cos(latStart * 0.01745329251994329576923690766743) - 93.5
                          * cos(3.0 * latStart * 0.01745329251994329576923690766743) + 0.118
                          * cos(5.0 * latStart * 0.01745329251994329576923690766743);

    double northing, easting;

    ConvertWGS84ToLocal(latitude, longitude, northing, easting);
    emit checkZoomWorldGrid(northing, easting); //ask main form to call checkZoomWorldGrid

}

void CNMEA::ConvertWGS84ToLocal(double Lat, double Lon, double &outNorthing, double &outEasting)
{
    mPerDegreeLon = 111412.84 * cos(Lat * 0.01745329251994329576923690766743) - 93.5 * cos(3.0 * Lat * 0.01745329251994329576923690766743) + 0.118 * cos(5.0 * Lat * 0.01745329251994329576923690766743);

    outNorthing = (Lat - latStart) * mPerDegreeLat;
    outEasting = (Lon - lonStart) * mPerDegreeLon;

    //Northing += mf.RandomNumber(-0.02, 0.02);
    //Easting += mf.RandomNumber(-0.02, 0.02);
}

void CNMEA::ConvertLocalToWGS84(double Northing, double Easting, double &outLat, double &outLon)
{
    outLat = ((Northing + fixOffset.northing) / mPerDegreeLat) + latStart;
    mPerDegreeLon = 111412.84 * cos(outLat * 0.01745329251994329576923690766743) - 93.5 * cos(3.0 * outLat * 0.01745329251994329576923690766743) + 0.118 * cos(5.0 * outLat * 0.01745329251994329576923690766743);
    outLon = ((Easting + fixOffset.easting) / mPerDegreeLon) + lonStart;
}

QString CNMEA::GetLocalToWSG84_KML(double Easting, double Northing)
{
    double Lat = (Northing / mPerDegreeLat) + latStart;
    mPerDegreeLon = 111412.84 * cos(Lat * 0.01745329251994329576923690766743) - 93.5 * cos(3.0 * Lat * 0.01745329251994329576923690766743) + 0.118 * cos(5.0 * Lat * 0.01745329251994329576923690766743);
    double Lon = (Easting / mPerDegreeLon) + lonStart;

    return QString("%1, %2, 0").arg(Lon,0,'g',7).arg(Lat,0,'g',7); //shouldn't use locale
}



