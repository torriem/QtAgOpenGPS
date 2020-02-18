#include "formgps.h"
#include "glm.h"

QString FormGPS::Zone () const
{
    return QString::number(pn->zone);
}
QString FormGPS::FixNorthing () const
{
    //New update of AoG has just only one vec3 struct. So should we fix QtAoG same with AoG ?
}
QString FormGPS::FixEasting () const
{
    //New update of AoG has just only one vec3 struct. So should we fix QtAoG same with AoG ?
}
QString FormGPS::Latitude () const
{
    return QString::number(pn->latitude);
}
QString FormGPS::Longitude () const
{
    return QString::number(pn->longitude);
}
QString FormGPS::SatsTracked () const
{
    return QString::number(pn->satellitesTracked);
}
QString FormGPS::HDOP () const
{
    return QString::number(pn->hdop);
}
QString FormGPS::NMEAHz () const
{
    return QString::number(fixUpdateHz);
}
QString FormGPS::PassNumber () const
{
    return QString::number(ABLine->passNumber);
}
QString FormGPS::CurveNumber () const
{
    return "0";
}
QString FormGPS::Heading () const
{
    return "0";
}
QString FormGPS::GPSHeading () const
{
    return "0";
}
QString FormGPS::Status () const
{
    if (pn->status == "A") return "Active";
    else
        return "Void";

}
QString FormGPS::FixQuality () const
{
    // TODO: if timerSim enabled, this function must be return "Sim"
    // I think, we have to seperate timer for simulation and GPS data receiver function.
    // GPS data function might be slot and then it should controlled by signal which is activated when data is coming.

    if (tmrWatchdog->isActive()) return "Sim";
    else if (pn->fixQuality == 0) return "Invalid: ";
    else if (pn->fixQuality == 1) return "GPS single: ";
    else if (pn->fixQuality == 2) return "DGPS: ";
    else if (pn->fixQuality == 3) return "PPS: ";
    else if (pn->fixQuality == 4) return "RTK fix: ";
    else if (pn->fixQuality == 5) return "Float: ";
    else if (pn->fixQuality == 6) return "Estimate: ";
    else if (pn->fixQuality == 7) return "Man IP: ";
    else if (pn->fixQuality == 8) return "Sim: ";
    else return "Unknown ";

}
QString FormGPS::GyroInDegrees () const
{
    return "0";
}
QString FormGPS::RollInDegrees () const
{
    return "0";
}
QString FormGPS::SetSteerAngle() const
{
    return "0";
}
QString FormGPS::ActualSteerAngle() const
{
    return "0";
}
QString FormGPS::FixHeading () const
{
    return "0";
}
QString FormGPS::LookAhead () const
{
    return "0";
}
QString FormGPS::StepFixNum () const
{
    return "0";
}
QString FormGPS::CurrentStepDistance () const
{
    return "0";
}
QString FormGPS::TotalStepDistance () const
{
    return "0";
}
QString FormGPS::WorkSwitchValue () const
{
    return "0";
}
QString FormGPS::AgeDiff () const
{
    return "0";
}
QString FormGPS::SpeedMPH () const
{
    double spd = 0;
    for (int c =0; c < 10; c++) spd += avgSpeed[c];
    spd *= 0.0621371;
    return QString::number(qRound(spd));
}
QString FormGPS::SpeedKPH () const
{
    double spd = 0;
    for (int c =0; c < 10; c++) spd += avgSpeed[c];
    spd *= 0.1;
    return QString::number(qRound(spd));
}
QString FormGPS::XTE () const
{
    return QString::number(crossTrackError/10) + "cm"; // Localization might be need later
}
QString FormGPS::inchXTE () const
{
    return QString::number(crossTrackError/25.54) + "cm";
}
QString FormGPS::FixOffset () const
{
    return "0";
}
QString FormGPS::FixOffSetInch() const
{
    return "0";
}
QString FormGPS::Altitude () const
{
    return QString::number(qRound(pn->altitude));
}
QString FormGPS::AltitudeFeet () const
{
    return QString::number(qRound(pn->altitude * 3.28084));
}
QString FormGPS::DistPivotM() const
{
    if (distancePivotToTurnLine > 0)
        return QString::number((int)distancePivotToTurnLine) + "m";
    else
        return "--";
}
QString FormGPS::DistPivotFt() const
{
    if (distancePivotToTurnLine > 0) return QString::number((int)(m2ft * (distancePivotToTurnLine))) + "ft";
    else return "--";
}
