#include "cfielddata.h"
#include "aogproperty.h"
#include "cvehicle.h"

CFieldData::CFieldData() {}

QString CFieldData::timeTillFinished(CVehicle &vehicle)
{
    if (vehicle.avgSpeed > 2)
    {
        double total_time = ((areaBoundaryOuterLessInner - workedAreaTotal) * glm::m2ha /
                             ((double)property_setVehicle_toolWidth * vehicle.avgSpeed * 0.1));
        int hours = (int) total_time;
        int minutes = (total_time - hours) * 60;

        return QString("%1:%2").arg(hours,2).arg(minutes,2);
    }
    else return QString("\u221E Hrs");
 }

QString CFieldData::workRateHectares(CVehicle &vehicle) {
    return QString("%1 ha/hr").arg((double)property_setVehicle_toolWidth * vehicle.avgSpeed * 0.1,0,'g',1);
}

QString CFieldData::workRateAcres(CVehicle &vehicle) {
    return QString("%1 ac/hr").arg((double)property_setVehicle_toolWidth * vehicle.avgSpeed * 0.2471,0,'g',1);
}

void CFieldData::updateFieldBoundaryGUIAreas(QVector<CBoundaryLines> &bndList)
{
    if (bndList.count() > 0)
    {
        areaOuterBoundary = bndList[0].area;
        areaBoundaryOuterLessInner = areaOuterBoundary;

        for (int i = 1; i < bndList.count(); i++)
        {
            areaBoundaryOuterLessInner -= bndList[i].area;
        }
    }
    else
    {
        areaOuterBoundary = 0;
        areaBoundaryOuterLessInner = 0;
    }
    //if (mf.isMetric) mf.btnManualOffOn.Text = AreaBoundaryLessInnersHectares;
    //else mf.btnManualOffOn.Text = AreaBoundaryLessInnersAcres;

}
