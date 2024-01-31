#ifndef CFIELDDATA_H
#define CFIELDDATA_H

#include <QVector>
#include "cboundarylist.h"
#include "glm.h"
#include <QLocale>
#include "aogiface_property.h"

class CVehicle;

class CFieldData
{

public:
    //all the section area added up;
    AOGIFace_Property<double> workedAreaTotal = AOGIFace_Property<double>("workedAreaTotal");

    //just a cumulative tally based on distance and eq width.
    AOGIFace_Property<double> workedAreaTotalUser = AOGIFace_Property<double>("workedAreaTotalUser");

    //accumulated user distance
    AOGIFace_Property<double> distanceUser = AOGIFace_Property<double>("distanceUser");

    double barPercent = 0;

    double overlapPercent = 0;

    //Outside area minus inner boundaries areas (m)
    AOGIFace_Property<double> areaBoundaryOuterLessInner = AOGIFace_Property<double>("areaBoundaryOuterLessInner");

    //used for overlap calcs - total done minus overlap
    AOGIFace_Property<double> actualAreaCovered = AOGIFace_Property<double>("actualAreaCovered");

    //Inner area of outer boundary(m)
    AOGIFace_Property<double> areaOuterBoundary = AOGIFace_Property<double>("areaOuterBoundary");

    //not really used - but if needed
    AOGIFace_Property<double> userSquareMetersAlarm = AOGIFace_Property<double>("userSquareMetersAlarm");

    CFieldData();

    void UpdateFieldBoundaryGUIAreas(QVector<CBoundaryList> &bndList);
};

#endif // CFIELDDATA_H
