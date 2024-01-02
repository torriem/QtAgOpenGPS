#include "formgps.h"
#include "classes/csim.h"
#include "qmlutil.h"
#include "aogproperty.h"

/* Callback for Simulator new position */
void FormGPS::simConnectSlots()
{
    connect(&sim,SIGNAL(setActualSteerAngle(double)),this,SLOT(onSimNewSteerAngle(double)));
    connect(&sim,SIGNAL(newPosition(double,double,double,double,double,double,double)),this,SLOT(onSimNewPosition(double,double,double,double,double,double,double)));
    connect(&timerSim,SIGNAL(timeout()),this,SLOT(onSimTimerTimeout()));

    if (property_setMenu_isSimulatorOn) {
        timerSim.start(100); //fire simulator every 100 ms.
        gpsHz = 10;
    }
}

void FormGPS::onSimNewPosition(double vtgSpeed,
                     double headingTrue,
                     double latitude,
                     double longitude, double hdop,
                     double altitude,
                     double satellitesTracked)
{

    pn.vtgSpeed = vtgSpeed;

    vehicle.AverageTheSpeed(vtgSpeed);

    pn.headingTrue = pn.headingTrueDual = headingTrue;
    qDebug() << qSetRealNumberPrecision(9) << headingTrue << vtgSpeed;
    ahrs.imuHeading = pn.headingTrue;
    if (ahrs.imuHeading > 360) ahrs.imuHeading -= 360;

    pn.ConvertWGS84ToLocal(latitude,longitude,pn.fix.northing,pn.fix.easting);
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
    double stepDistance = qmlobject->property("value").toReal() / 10.0 /gpsHz;
    sim.setSimStepDistance(stepDistance);

    qmlobject = qmlItem(qml_root, "simSteer");
    double steerAngle = (qmlobject->property("value").toReal() - 300) * 0.1;
    sim.DoSimTick(steerAngle); //drive straight for now until UI
}
