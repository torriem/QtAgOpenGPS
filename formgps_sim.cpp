#include "formgps.h"
#include "classes/csim.h"

void FormGPS::onSimNewPosition(QByteArray nmea_data) {
    pn.rawBuffer.append(nmea_data);
}

void FormGPS::onSimTimerTimeout()
{
    //if not serial
    if (isAutoSteerBtnOn && (vehicle.guidanceLineDistanceOff != 32000))
        sim.DoSimTick(vehicle.guidanceLineSteerAngle * 0.01);
    else if (recPath.isDrivingRecordedPath)
        sim.DoSimTick(vehicle.guidanceLineSteerAngle * 0.01);
    //else if (self.isSelfDriving) sim.DoSimTick(guidanceLineSteerAngle * 0.01);
    else
        //sim.DoSimTick(sim.steerAngleScrollBar);
        sim.DoSimTick(0); //drive straight for now until UI


}
