#include "cfielddata.h"
#include "ctool.h"
#include "cboundary.h"
#include "cnmea.h"
#include <QString>

CFieldData::CFieldData(QObject *parent) : QObject(parent)
{
    workedAreaTotal = 0;
    workedAreaTotalUser = 0;
    userSquareMetersAlarm = 0;
}

void CFieldData::updateFieldBoundaryGUIAreas(const CBoundary &bnd)
{
    if (bnd.bndArr.size() > 0)
    {
        areaOuterBoundary = bnd.bndArr[0].area;
        areaBoundaryOuterLessInner = areaOuterBoundary;

        for (int i = 1; i < bnd.bndArr.size(); i++)
        {
            if (bnd.bndArr[i].isSet) areaBoundaryOuterLessInner -= bnd.bndArr[i].area;
        }
    }
}
