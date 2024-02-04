#ifndef CFIELDDATA_H
#define CFIELDDATA_H

#include <QVector>
#include "cboundarylist.h"
#include "glm.h"
#include <QLocale>
#include "interfaceproperty.h"

class CVehicle;

class CFieldData
{

public:
    //all the section area added up;
    InterfaceProperty<AOGInterface, double> workedAreaTotal = InterfaceProperty<AOGInterface, double>("workedAreaTotal");

    //just a cumulative tally based on distance and eq width.
    InterfaceProperty<AOGInterface, double> workedAreaTotalUser = InterfaceProperty<AOGInterface, double>("workedAreaTotalUser");

    //accumulated user distance
    InterfaceProperty<AOGInterface, double> distanceUser = InterfaceProperty<AOGInterface, double>("distanceUser");

    double barPercent = 0;

    double overlapPercent = 0;

    //Outside area minus inner boundaries areas (m)
    InterfaceProperty<AOGInterface, double> areaBoundaryOuterLessInner = InterfaceProperty<AOGInterface, double>("areaBoundaryOuterLessInner");

    //used for overlap calcs - total done minus overlap
    InterfaceProperty<AOGInterface, double> actualAreaCovered = InterfaceProperty<AOGInterface, double>("actualAreaCovered");

    //Inner area of outer boundary(m)
    InterfaceProperty<AOGInterface, double> areaOuterBoundary = InterfaceProperty<AOGInterface, double>("areaOuterBoundary");

    //not really used - but if needed
    InterfaceProperty<AOGInterface, double> userSquareMetersAlarm = InterfaceProperty<AOGInterface, double>("userSquareMetersAlarm");

    CFieldData();

    void UpdateFieldBoundaryGUIAreas(QVector<CBoundaryList> &bndList);
};

#endif // CFIELDDATA_H
