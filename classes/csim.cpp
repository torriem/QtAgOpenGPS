#include "csim.h"
#include <QDateTime>
#include <iostream>
#include <cmath>
#include "glm.h"
#include "aogsettings.h"


CSim::CSim()
{
    USE_SETTINGS;

    latitude = SETTINGS_SIM_LATITUDE;
    longitude = SETTINGS_SIM_LONGITUDE;
}
void CSim::DoSimTick(double _st)
{
    QByteArray sbSendText;

    steerAngle = _st;
    double temp = (stepDistance *qTan(steerAngle * 0.0165329252) / 3.3);
    headingTrue += temp;
    if (headingTrue > (2.0 * M_PI)) headingTrue -= glm::twoPI;
    if (headingTrue < 0) headingTrue += glm::twoPI;

    degrees = glm::toDegrees(headingTrue);
    //Calculate the next Latitude and Longitude based on heading and distance
    CalculateNewPositionFromBearingDistance(latitude, longitude, degrees, stepDistance / 1000.0);
    //qDebug() << fixed << qSetRealNumberPrecision(7) << latitude << ", " << fixed << qSetRealNumberPrecision(7) << longitude;
    // calc the speed
    speed = 1.944 * stepDistance * 10; //in knots
    //qDebug() << stepDistance << ", " << speed;
    sbSendText.append(BuildGGA().toLatin1());
    sbSendText.append(BuildVTG().toLatin1());

    // if you want to see the generated data, uncomment it
    //qDebug() << sbSendText.constData();

    emit new_position(sbSendText);

    // should assign those sbSendText to the FormGPS's recvSentenceSettings

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

    //qDebug() << bearing <<" " << distance;

    // convert to DMS from Degrees

    latMinu = latitude;
    longMinu = longitude;

    latDeg = (int) latitude;
    longDeg = (int) longitude;

    latMinu -= latDeg;
    longMinu -= longDeg;

    latMinu = (latMinu * 60.0);
    longMinu = (longMinu * 60.0);

    latDeg *= 100.0;
    longDeg *= 100.0;

    latNMEA = latMinu + latDeg;
    longNMEA = longMinu + longDeg;

    if (latitude >= 0) NS = 'N';
    else NS = 'S';
    if (longitude >= 0) EW = 'E';
    else EW = 'W';
}

void CSim::CalculateCheckSum(QString Sentence)
{
    int sum = 0, inx;
    QByteArray sentence_chars = Sentence.toLatin1();
    char tmp;
    for (inx = 1; inx < Sentence.length(); inx++)
    {
        tmp = sentence_chars.at(inx);
        if (tmp == '*') break;
        sum ^= tmp;
    }
    sumStr = QString("%1").arg(sum, 2, 16);

}

QString CSim::BuildGGA()
{
    QString sbGGA;
    QString hms = QDateTime::currentDateTime().toString("HHmmss.00,");
    sbGGA.clear();
    sbGGA.append("$GPGGA,");
    sbGGA.append(hms);
    sbGGA.append(QString::number(fabs(latNMEA),'f',7)).append(',').append(NS).append(',');
    sbGGA.append(QString::number(fabs(longNMEA),'f',7)).append(',').append(EW).append(',');
    sbGGA.append(QString::number(fixQuality)).
            append(',').
            append(QString::number(sats)).
            append(',').
            append(QString::number(HDOP)).
            append(',').
            append(QString::number(altitude));
    sbGGA.append(",M,46.9,M,,,*");
    CalculateCheckSum(sbGGA);
    sbGGA.append(sumStr);
    sbGGA.append("\r\n");

    return sbGGA;

}

QString CSim::BuildVTG()
{
    QString sbVTG;
    sbVTG.clear();
    sbVTG.append("$GPVTG,");
    sbVTG.append(QString::number(degrees));
    sbVTG.append(",T,034.4,M,");
    sbVTG.append(QString::number(speed));
    sbVTG.append(",N,");
    sbVTG.append(QString::number(speed * 1.852));
    sbVTG.append(",K*");
    CalculateCheckSum(sbVTG);
    sbVTG.append(sumStr);
    sbVTG.append("\r\n");

    return sbVTG;
}

void CSim::setSimStepDistance(double _stepDistance)
{
    stepDistance = _stepDistance;
}
