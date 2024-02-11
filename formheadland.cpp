#include "formheadland.h"
#include "cheadline.h"
#include "cboundary.h"
#include "cabcurve.h"
#include "glm.h"

//here for now.  Put in another module for use in other places.

void CalculateHeadings(QVector<Vec3> &xList)
{
    //to calc heading based on next and previous points to give an average heading.
    int cnt = xList.count();
    if (cnt > 3)
    {
        QVector<Vec3> arr = xList; //copy list
        cnt--;
        xList.clear();

        Vec3 pt3 = arr[0];
        pt3.heading = atan2(arr[1].easting - arr[0].easting, arr[1].northing - arr[0].northing);
        if (pt3.heading < 0) pt3.heading += glm::twoPI;
        xList.append(pt3);

        //middle points
        for (int i = 1; i < cnt; i++)
        {
            pt3 = arr[i];
            pt3.heading = atan2(arr[i + 1].easting - arr[i - 1].easting, arr[i + 1].northing - arr[i - 1].northing);
            if (pt3.heading < 0) pt3.heading += glm::twoPI;
            xList.append(pt3);
        }

        pt3 = arr[arr.count() - 1];
        pt3.heading = atan2(arr[arr.count() - 1].easting - arr[arr.count() - 2].easting,
                                 arr[arr.count() - 1].northing - arr[arr.count() - 2].northing);
        if (pt3.heading < 0) pt3.heading += glm::twoPI;
        xList.append(pt3);
    }
}

void MakePointMinimumSpacing(QVector<Vec3> &xList, double minDistance)
{
    int cnt = xList.count();
    if (cnt > 3)
    {
        //make sure point distance isn't too big
        for (int i = 0; i < cnt - 1; i++)
        {
            int j = i + 1;
            //if (j == cnt) j = 0;
            double distance = glm::Distance(xList[i], xList[j]);
            if (distance > minDistance)
            {
                Vec3 pointB((xList[i].easting + xList[j].easting) / 2.0,
                                       (xList[i].northing + xList[j].northing) / 2.0,
                                       xList[i].heading);

                xList.insert(j, pointB);
                cnt = xList.count();
                i = -1;
            }
        }
    }
}



FormHeadland::FormHeadland(QObject *parent)
    : QObject{parent}
{}




void FormHeadland::load_headline() {
    hdl->idx = -1;

    start = 99999; end = 99999;
    isA = true;
    hdl->desList.clear();

    sliceArr.clear();
    backupList.clear();

    if(bnd->bndList[0].hdLine.count() == 0)
    {
        bnd->bndList[0].hdLine.clear();

        if (bnd->bndList[0].fenceLine.count() > 0)
        {
            for (int i = 0; i < bnd->bndList[0].fenceLine.count(); i++)
            {
                bnd->bndList[0].hdLine.append(Vec3(bnd->bndList[0].fenceLine[i]));
            }
        }
    }
    else
    {
        //make sure point distance isn't too big
        MakePointMinimumSpacing(bnd->bndList[0].hdLine, 1.2);
        CalculateHeadings(bnd->bndList[0].hdLine);
    }
}


