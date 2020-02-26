#include "csim.h"
#include <QDateTime>
#include <iostream>
#include <cmath>


CSim::CSim()
{
    latitude = simLat;
    longitude = simLon;
}
void CSim::DoSimTick(double _st)
{
    steerAngle = _st;
    double temp = (stepDistance *qTan(steerAngle * 0.0165329252) / 3.3);
    headingTrue += temp;
    if (headingTrue > (2.0 * M_PI)) headingTrue -=(2.0 * M_PI);
    if (headingTrue < 0) headingTrue += (2.0 * M_PI);

    degrees = headingTrue * ANG_TO_RAD;
    degrees = qRound(degrees);
    //Calculate the next Latitude and Longitude based on heading and distance
    CalculateNewPositionFromBearingDistance(latitude, longitude, degrees, stepDistance / 1000.0);
    // calc the speed
    speed = qRound(1.944 * stepDistance * 5);
    BuildGGA();
    BuildVTG();

    sbSendText.append(sbGGA);

    sbSendText.append(sbVTG);

    // if you want to see the generated data, uncomment it

//    cout << sbSendText.data();

    emit new_position(sbSendText);

    // should assign those sbSendText to the FormGPS's recvSentenceSettings

}

void CSim::CalculateNewPositionFromBearingDistance(double lat, double lng, double bearing, double distance)
{
    // 1 degree = 0.0175 radian = M_PI / 180

    const double R = 6371; // Earth Radius in Km

    double lat2 = qAsin((qSin(M_PI / 180 * lat) * qCos(distance / R)) + (qCos(M_PI / 180 * lat) * qSin(distance / R)
                                                                         *qCos(M_PI / 180 * bearing)));
    double lon2 = (M_PI / 180 * lng) + qAtan2(qSin(M_PI / 180 * bearing) * qSin(distance / R) *
                                              qCos(M_PI / 180 * lat), qCos(distance / R) - (qSin(M_PI / 180 * lat) * qSin(lat2)));
    latitude = 180 / M_PI * lat2;
    longitude = 180 / M_PI * lon2;

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
    const char * sentence_chars = Sentence.toLatin1().data();
    char tmp;
    for (inx = 1; ; inx++)
    {
        tmp = sentence_chars[inx];
        if (tmp == '*') break;
        sum ^= tmp;
    }
    sumStr = QString("%1").arg(sum, 2, 16);

}

void CSim::BuildGGA()
{
    QString hms = QDateTime::currentDateTime().toString("HHmmss.00,");
    sbGGA.clear();
    sbGGA.append("$GPGGA,");
    sbGGA.append(hms);
    sbGGA.append(QString::number(latNMEA,'g',7)).append(',').append(NS).append(',');
    sbGGA.append(QString::number(longNMEA,'g',7)).append(',').append(EW).append(',');
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

}

void CSim::BuildVTG()
{
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
}

void CSim::setSimStepDistance(double _stepDistance)
{
    stepDistance = _stepDistance;
}
