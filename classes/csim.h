#ifndef CSIM_H
#define CSIM_H

#include <QString>
#include <QStringList>
#include <QtMath>
#include <QDateTime>
#include <QDebug>


#define ANG_TO_RAD 57.2957795

using namespace std;

class CSim : public QObject
{
    Q_OBJECT

public:
    double altitude = 300;

    double latitude, longitude;
    double headingTrue, stepDistance = 0.0, steerAngle, steerangleAve = 0.0;
    double steerAngleScrollBar = 0;

    bool isAccelForward, isAccelBack;

    explicit CSim(QObject *parent = 0);

    void loadSettings();

    void CalculateNewPositionFromBearingDistance(double lat, double lng, double bearing, double distance);
    void DoSimTick(double _st);

private:

signals:
    void setActualSteerAngle(double);
    void newPosition(double vtgSpeed,
                     double headingTrue,
                     double latitude,
                     double longitude, double hdop,
                     double altitude,
                     double satellitesTracked);

public slots:
    void setSimStepDistance(double);

};

#endif // CSIM_H
