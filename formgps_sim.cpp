#include "formgps.h"
#include "classes/csim.h"
#include "qmlutil.h"

/* Callback for Simulator new position */
void FormGPS::onSimNewPosition(double vtgSpeed,
                     double headingTrue,
                     double latitude,
                     double longitude, double hdop,
                     double altitude,
                     double satellitesTracked)
{

    pn.vtgSpeed = vtgSpeed;

    vehicle.AverageTheSpeed(vtgSpeed);

    pn.headingTrue = pn.headingTrueDual = glm::toDegrees(headingTrue);
    ahrs.imuHeading = pn.headingTrue;
    if (ahrs.imuHeading > 360) ahrs.imuHeading -= 360;

    pn.latitude = latitude;
    pn.longitude = longitude;

    pn.hdop = hdop;
    pn.altitude = altitude;
    pn.satellitesTracked = satellitesTracked;
    sentenceCounter = 0;
    UpdateFixPosition();
}

void FormGPS::onSimNewSteerAngle(double steerAngleAve)
{
    mc.actualSteerAngleDegrees = steerAngleAve;
}

/* iterate the simulator on a timer */
void FormGPS::onSimTimerTimeout()
{
    QObject *qmlobject = qmlItem(qml_root,"simSpeed");
    double stepDistance = qmlobject->property("value").toReal() / 10.0 /fixUpdateHz;
    sim.setSimStepDistance(stepDistance);

    qmlobject = qmlItem(qml_root, "simSteer");
    double steerAngle = (qmlobject->property("value").toReal() - 300) * 0.1;
    sim.DoSimTick(steerAngle); //drive straight for now until UI
}
