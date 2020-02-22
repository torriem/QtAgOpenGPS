#ifndef CFIELDDATA_H
#define CFIELDDATA_H

#include <QObject>
#include <QString>
#include <QLocale>
#include "glm.h"

class CTool;
class CBoundary;
class CNMEA;

QLocale locale;

class CFieldData : public QObject
{
private:
    //all the section area added up;
    double workedAreaTotal;

    //just a cumulative tally based on distance and eq width.
    double workedAreaTotalUser;

public:
    //accumulated user distance
    double distanceUser;
    double barPercent = 0;
    double overlapPercent = 0;

    //Outside area minus inner boundaries areas (m)
    double areaBoundaryOuterLessInner;

    //used for overlap calcs - total done minus overlap
    double actualAreaCovered;

    //Inner area of outer boundary(m)
    double areaOuterBoundary;

    //not really used - but if needed
    double userSquareMetersAlarm;



    explicit CFieldData(QObject *parent = 0);

    void updateFieldBoundaryGUIAreas(const CBoundary &bnd);

    //Area inside Boundary less inside boundary areas
    inline QString getAreaBoundaryLessInnersHectares() {
        return locale.toString(areaBoundaryOuterLessInner * glm::m2ha, 'f', 2);
    }

    inline QString getAreaBoundaryLessInnersAcres() {
        return locale.toString(areaBoundaryOuterLessInner * glm::m2ac, 'f', 2);
    }

    //USer tally string
    inline QString getWorkedUserHectares() {
        return locale.toString(workedAreaTotalUser * glm::m2ha, 'f', 2);
    }
    inline QString getWorkedUserHectares2() {
        return locale.toString(workedAreaTotalUser * glm::m2ha, 'f', 2);
    }

    //user tally string
    inline QString getWorkedUserAcres() {
        return locale.toString(workedAreaTotalUser * glm::m2ac, 'f', 2);
    }
    inline QString getWorkedUserAcres2(){
        return locale.toString(workedAreaTotalUser * glm::m2ac, 'f', 2);
    }

    //string of area worked
    inline QString getWorkedAcres() {
        if (workedAreaTotal < 404048)
            return locale.toString(workedAreaTotal * 0.000247105, 'f', 2);
        else
            return locale.toString(workedAreaTotal * 0.000247105, 'f', 2);
    }

    inline QString getWorkedHectares() {
        if (workedAreaTotal < 99000)
            return locale.toString(workedAreaTotal * 0.0001, 'f', 2);
        else
            return locale.toString(workedAreaTotal * 0.0001, 'f', 2);
    }

    //user distance strings
    inline QString getDistanceUserMeters() {
        return locale.toString((int)distanceUser);
    }
    inline QString getDistanceUserFeet(){
        return locale.toString((int)(distanceUser * glm::m2ft));
    }

    //remaining area to be worked
    inline QString getWorkedAreaRemainHectares(){
        return locale.toString((areaBoundaryOuterLessInner - workedAreaTotal) * glm::m2ha, 'f', 2);
    }
    inline QString getWorkedAreaRemainAcres(){
        return locale.toString((areaBoundaryOuterLessInner - workedAreaTotal) * glm::m2ac, 'f', 2);
    }

    //overlap strings
    inline QString getWorkedAreaRemainPercentage(){
        if (areaBoundaryOuterLessInner > 10) {
            barPercent = ((areaBoundaryOuterLessInner - workedAreaTotal) * 100 / areaBoundaryOuterLessInner);
            return locale.toString( barPercent, 'f', 2) + "%";
        } else {
            barPercent = 0;
            return locale.toString( (float)0.0, 'f', 2) + "%";
        }
    }

    inline QString getTimeTillFinished(double toolWidth, double speed) {
        if (speed > 0.1)
            return locale.toString(((areaBoundaryOuterLessInner - workedAreaTotal) * glm::m2ha)
                / (toolWidth * speed * 0.1), 'f', 1) + " " + tr("Hrs");
        else
            return QString("\u221E ") + tr("Hrs");
    }

    inline QString getWorkRateHectares(double toolWidth, double speed) {
        return locale.toString(speed * toolWidth * 0.1, 'f', 1);
    }

    inline QString getWorkRateAcres(double toolWidth, double speed)	{
        return locale.toString(speed * toolWidth * 0.2471, 'f', 1);
    }

};

#endif // CFIELDDATA_H
