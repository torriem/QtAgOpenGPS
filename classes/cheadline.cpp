#include "cheadline.h"
#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include "cboundary.h"
#include "glutils.h"
#include "vec3.h"
#include "vec2.h"
#include "glm.h"

CHeadLine::CHeadLine()
{

}

void CHeadLine::CalculateHeadings(QVector<Vec3> &xList)
{
    //to calc heading based on next and previous points to give an average heading.
    int cnt = xList.count();
    if (cnt > 3)
    {
        QVector<Vec3> arr(xList);
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

//add extensons
void CHeadLine::AddFirstLastPoints(QVector<Vec3> &xList, const CBoundary &bnd)
 {
    int ptCnt = xList.count() - 1;
    Vec3 start(xList[0]);

    if (bnd.bndList.count() > 0)
    {
        //end
        while (glm::IsPointInPolygon(bnd.bndList[0].fenceLineEar, xList[xList.count() - 1]))
        {
            for (int i = 1; i < 10; i++)
            {
                Vec3 pt(xList[ptCnt]);
                pt.easting += (sin(pt.heading) * i);
                pt.northing += (cos(pt.heading) * i);
                xList.append(pt);
            }
            ptCnt = xList.count() - 1;
        }

        //and the beginning
        start = Vec3(xList[0]);

        while (glm::IsPointInPolygon(bnd.bndList[0].fenceLineEar,xList[0]))
        {
            for (int i = 1; i < 10; i++)
            {
                Vec3 pt(start);
                pt.easting -= (sin(pt.heading) * i);
                pt.northing -= (cos(pt.heading) * i);
                xList.insert(0, pt);
            }
            start = Vec3(xList[0]);
        }
    }
    else
    {
        for (int i = 1; i < 1000; i += 5)
        {
            Vec3 pt(xList[ptCnt]);
            pt.easting += (sin(pt.heading) * i);
            pt.northing += (cos(pt.heading) * i);
            xList.append(pt);
        }

        //and the beginning
        start=Vec3(xList[0]);

        for (int i = 1; i < 1000; i += 5)
        {
            Vec3 pt(start);
            pt.easting -= (sin(pt.heading) * i);
            pt.northing -= (cos(pt.heading) * i);
            xList.insert(0, pt);
        }
    }
}
