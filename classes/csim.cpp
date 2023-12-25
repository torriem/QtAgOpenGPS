#include "csim.h"
#include <QDateTime>
#include <iostream>
#include <cmath>
#include "glm.h"
#include "aogproperty.h"


CSim::CSim(QObject *parent) : QObject(parent)
{
    loadSettings();
}

void CSim::loadSettings()
{
    latitude = property_setGPS_SimLatitude;
    longitude = property_setGPS_SimLongitude;

}
void CSim::DoSimTick(double _st)
{
    QByteArray sbSendText;

    steerAngle = _st;

    double diff = fabs(steerAngle - steerangleAve);

    if (diff > 11)
    {
        if (steerangleAve >= steerAngle)
        {
            steerangleAve -= 6;
        }
        else steerangleAve += 6;
    }
    else if (diff > 5)
    {
        if (steerangleAve >= steerAngle)
        {
            steerangleAve -= 2;
        }
        else steerangleAve += 2;
    }
    else if (diff > 1)
    {
        if (steerangleAve >= steerAngle)
        {
            steerangleAve -= 0.5;
        }
        else steerangleAve += 0.5;
    }
    else
    {
        steerangleAve = steerAngle;
    }

    emit setActualSteerAngle(steerangleAve);

    double temp = stepDistance * tan(steerangleAve * 0.0165329252) / 2;
    headingTrue += temp;
    if (headingTrue > glm::twoPI) headingTrue -= glm::twoPI;
    if (headingTrue < 0) headingTrue += glm::twoPI;

    double vtgSpeed = fabs(4 * stepDistance * 10);
    //mf.pn.AverageTheSpeed(); TODO: done in signal handler

    //Calculate the next Lat Long based on heading and distance
    CalculateNewPositionFromBearingDistance(glm::toRadians(latitude), glm::toRadians(longitude), headingTrue, stepDistance / 1000.0);

    //This is done by main form slot
    //mf.pn.ConvertWGS84ToLocal(latitude, longitude, out mf.pn.fix.northing, out mf.pn.fix.easting);

    headingTrue = glm::toDegrees(headingTrue);

    //TODO this must be in the main form slot
    //mf.ahrs.imuHeading = mf.pn.headingTrue;
    //if (headingTrue > 360) headingTrue -= 360;

    //mf.pn.latitude = latitude;
    //mf.pn.longitude = longitude;

    //mf.pn.hdop = 0.7;
    //mf.pn.altitude = 732;
    //mf.pn.satellitesTracked = 12;

    emit newPosition(vtgSpeed, headingTrue, latitude, longitude, 0.7f, 732, 12);

    //done in main form slot:
    //mf.sentenceCounter = 0;
    //mf.UpdateFixPosition();

    if (isAccelForward)
    {
        isAccelBack = false;
        stepDistance += 0.02;
        if (stepDistance > 0.12) isAccelForward = false;
    }

    if (isAccelBack)
    {
        isAccelForward = false;
        stepDistance -= 0.01;
        if (stepDistance < -0.06) isAccelBack = false;
    }
}

void CSim::CalculateNewPositionFromBearingDistance(double lat, double lng, double bearing, double distance)
{
    // 1 degree = 0.0175 radian = M_PI / 180

    const double R = 6371; // Earth Radius in Km

    double lat2 = asin((sin(M_PI / 180 * lat) * cos(distance / R))
        + (cos(M_PI / 180 * lat) * sin(distance / R) * cos(M_PI / 180 * bearing)));

    double lon2 = (M_PI / 180 * lng) + atan2(sin(M_PI / 180 * bearing) * sin(distance / R)
        * cos(M_PI / 180 * lat), cos(distance / R) - (sin(M_PI / 180 * lat) * sin(lat2)));


    //double lat2 = qAsin((qSin(M_PI / 180 * lat) * qCos(distance / R)) + (qCos(M_PI / 180 * lat) * qSin(distance / R)
    //                                                                     *qCos(M_PI / 180 * bearing)));
    //double lon2 = (M_PI / 180 * lng) + qAtan2(qSin(M_PI / 180 * bearing) * qSin(distance / R) *
    //                                          qCos(M_PI / 180 * lat), qCos(distance / R) - (qSin(M_PI / 180 * lat) * qSin(lat2)));
    latitude = 180 / M_PI * lat2;
    longitude = 180 / M_PI * lon2;
}

void CSim::setSimStepDistance(double _stepDistance)
{
    stepDistance = _stepDistance;
}
