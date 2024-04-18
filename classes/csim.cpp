#include "csim.h"
#include <QDateTime>
#include <iostream>
#include <cmath>
#include "glm.h"
#include "aogproperty.h"


CSim::CSim(QObject *parent) : QObject(parent)
{
    loadSettings();
    headingTrue = 0;
    stepDistance = 0;
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

    temp = fabs(latitude * 100);
    temp -= ((int)(temp));
    temp *= 00;
    double altitude = temp + 200;

    temp = fabs(longitude * 100);
    temp -= ((int)(temp));
    temp *= 100;
    altitude += temp;

    //Calculate the next Lat Long based on heading and distance
    CalculateNewPositionFromBearingDistance(glm::toRadians(latitude), glm::toRadians(longitude), headingTrue, stepDistance / 1000.0);
    //qDebug() << qSetRealNumberPrecision(9) << latitude << ", " << longitude << ", " << headingTrue << ", " << stepDistance;

    //This is all done by main form slot onSimNewPositon()
    //mf.pn.ConvertWGS84ToLocal(latitude, longitude, out mf.pn.fix.northing, out mf.pn.fix.easting);

    //headingTrue = glm::toDegrees(headingTrue);

    //TODO this must be in the main form slot
    //mf.ahrs.imuHeading = mf.pn.headingTrue;
    //if (headingTrue > 360) headingTrue -= 360;

    //mf.pn.latitude = latitude;
    //mf.pn.longitude = longitude;

    //mf.pn.hdop = 0.7;
    //see altitude calculations above
    //mf.pn.satellitesTracked = 12;

    emit newPosition(vtgSpeed, glm::toDegrees(headingTrue), latitude, longitude, 0.7f, altitude, 12);

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

    const double R = distance / 6371; // Earth Radius in Km

    double lat2 = asin((sin(lat) * cos(R)) + (cos(lat) * sin(R) * cos(bearing)));

    double lon2 = lng + atan2(sin(bearing) * sin(R) * cos(lat), cos(R) - (sin(lat) * sin(lat2)));


    //double lat2 = qAsin((qSin(M_PI / 180 * lat) * qCos(distance / R)) + (qCos(M_PI / 180 * lat) * qSin(distance / R)
    //                                                                     *qCos(M_PI / 180 * bearing)));
    //double lon2 = (M_PI / 180 * lng) + qAtan2(qSin(M_PI / 180 * bearing) * qSin(distance / R) *
    //                                          qCos(M_PI / 180 * lat), qCos(distance / R) - (qSin(M_PI / 180 * lat) * qSin(lat2)));
    latitude = glm::toDegrees(lat2);
    longitude = glm::toDegrees(lon2);
}

void CSim::setSimStepDistance(double _stepDistance)
{
    stepDistance = _stepDistance;
}

void CSim::speed_bump(bool up) {
    if (up) {
        if (stepDistance < 0)
        {
            stepDistance = 0;
            return;
        }
        if (stepDistance < 0.2 ) stepDistance += 0.02;
        else
            stepDistance *= 1.15;

        if (stepDistance > 7.5) stepDistance = 7.5;
    } else {
        if (stepDistance < 0.2 && stepDistance > -0.51) stepDistance -= 0.02;
        else stepDistance *= 0.8;
        if (stepDistance < -0.5) stepDistance = -0.5;
    }
}

void CSim::speed_zero() {
    stepDistance = 0;
}

void CSim::reverse() {
    stepDistance = 0;
    isAccelBack = true;
}

void CSim::forward() {
    stepDistance = 0;
    isAccelForward = true;
}

void CSim::reset() {
    latitude = property_setGPS_SimLatitude;
    longitude = property_setGPS_SimLongitude;
}
