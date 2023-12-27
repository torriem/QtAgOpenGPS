#ifndef CFIELDDATA_H
#define CFIELDDATA_H

#include <QVector>
#include "cboundarylines.h"
#include "glm.h"
#include <QLocale>

class CVehicle;

class CFieldData
{

public:
    //all the section area added up;
    double workedAreaTotal;

    //just a cumulative tally based on distance and eq width.
    double workedAreaTotalUser;

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

    CFieldData();

    void UpdateFieldBoundaryGUIAreas(QVector<CBoundaryLines> &bndList);

    //Area inside Boundary less inside boundary areas
    inline QString AreaBoundaryLessInnerHectares() {
        if (areaBoundaryOuterLessInner < 404048) {
            return QString("%1").arg(areaBoundaryOuterLessInner * glm::m2ha, 0, 'g', 2);
        } else {
            return QString("%1").arg(areaBoundaryOuterLessInner * glm::m2ha, 0, 'g', 1);
        }
    }

     inline QString AreaBoundaryLessInnerAcres() {
        if (areaBoundaryOuterLessInner < 404048) {
            return QString("%1").arg(areaBoundaryOuterLessInner * glm::m2ac, 0, 'g', 2);
        } else {
            return QString("%1").arg(areaBoundaryOuterLessInner * glm::m2ac, 0, 'g', 1);
        }
    }

    //USer tally string
     inline QString WorkedUserAcres() {
        return QString("%1").arg(workedAreaTotalUser * glm::m2ac,0,'g',2);
     }

     inline QString WorkedUserHectares() {
        return QString("%1").arg(workedAreaTotalUser * glm::m2ha,0,'g',2);
     }


    //String of Area worked
    inline QString WorkedAcres() {
        if (workedAreaTotal < 404048) {
            return QString("%1").arg(workedAreaTotal * glm::m2ac, 0,'g', 2);
        } else {
            return QString("%1").arg(workedAreaTotal * glm::m2ac, 0,'g', 1);
        }
    }

     inline QString WorkedHectares() {
        if (workedAreaTotal < 404048) {
            return QString("%1").arg(workedAreaTotal * glm::m2ha, 0,'g', 2);
        } else {
            return QString("%1").arg(workedAreaTotal * glm::m2ha, 0,'g', 1);
        }
    }

     //User Distance strings
    inline QString DistanceUserMeters() {
        return QString("%1").arg(distanceUser, 0,'g', 3);
    }

    inline QString DistanceUserFeet() {
        return QString("%1").arg(distanceUser * glm::m2ft,0,'g', 1);
    }

    //remaining area to be worked
    inline QString WorkedAreaRemainHectares() {
        return QString("%1").arg((areaBoundaryOuterLessInner - workedAreaTotal) * glm::m2ha, 0,'g', 2);
    }

    inline QString WorkedAreaRemainAcres() {
        return QString("%1").arg((areaBoundaryOuterLessInner - workedAreaTotal) * glm::m2ac, 0,'g', 2);
    }

    inline QString WorkedAreaRemainPercentage() {
        if (areaBoundaryOuterLessInner > 10)
        {
            barPercent = ((areaBoundaryOuterLessInner - workedAreaTotal) * 100 / areaBoundaryOuterLessInner);
            return QString("%1").arg(barPercent, 0,'g',0) + "%";
        }
        else
        {
            barPercent = 0;
            return QString ("0%");
        }
    }

    //overlap strings
    inline QString ActualAreaWorkedHectares() {
        return QString("%1").arg(actualAreaCovered * glm::m2ha,0,'g',2);
    }

    inline QString ActualAreaWorkedAcres() {
        return QString("%1").arg(actualAreaCovered * glm::m2ac,0,'g',2);
    }

    inline QString ActualRemainHectares() {
        return QString("%1").arg((areaBoundaryOuterLessInner - actualAreaCovered) * glm::m2ha,0,'g',2);
    }

    inline QString ActualRemainAcres() {
        return QString("%1").arg((areaBoundaryOuterLessInner - actualAreaCovered) * glm::m2ac,0,'g',2);
    }

    inline QString ActualOverlapPercent() {
        return QString("%1").arg(overlapPercent,0,'g',1) + "%  ";
    }

    QString TimeTillFinished(CVehicle &vehicle);

    QString WorkRateHectares(CVehicle &vehicle);
    QString WorkRateAcres(CVehicle &vehicle);

};

#endif // CFIELDDATA_H
