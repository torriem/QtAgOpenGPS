#include "cfielddata.h"
#include "aogproperty.h"
#include "cvehicle.h"

CFieldData::CFieldData() {}

void CFieldData::UpdateFieldBoundaryGUIAreas(QVector<CBoundaryList> &bndList)
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
