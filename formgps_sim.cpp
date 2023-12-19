#include "formgps.h"
#include "classes/csim.h"
#include "qmlutil.h"

void FormGPS::onSimNewPosition(QByteArray nmea_data) {
    pn.rawBuffer.append(nmea_data);
}

void FormGPS::onSimTimerTimeout()
{
    QObject *qmlobject = qmlItem(qml_root,"simSpeed");
    double stepDistance = qmlobject->property("value").toReal() / 10.0 /fixUpdateHz;
    sim.setSimStepDistance(stepDistance);

    qmlobject = qmlItem(qml_root, "simSteer");
    double steerAngle = (qmlobject->property("value").toReal() - 300) * 0.1;

    //if a GPS is connected disable sim
    if (!sp.isOpen())
    {
        if (isAutoSteerBtnOn && (vehicle.guidanceLineDistanceOff != 32000))
            sim.DoSimTick(vehicle.guidanceLineSteerAngle * 0.01);
        else if (recPath.isDrivingRecordedPath)
            sim.DoSimTick(vehicle.guidanceLineSteerAngle * 0.01);
        //else if (self.isSelfDriving) sim.DoSimTick(guidanceLineSteerAngle * 0.01);
        else
            //TODO: sim.DoSimTick(sim.steerAngleScrollBar);
            sim.DoSimTick(steerAngle); //drive straight for now until UI
    }

}
