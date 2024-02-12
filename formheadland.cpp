#include "formheadland.h"
#include "cheadline.h"
#include "cboundary.h"
#include "cabcurve.h"
#include "glm.h"
#include "qmlutil.h"

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

void FormHeadland::connect_ui(QObject *headland_designer_instance) {
    qml_object = headland_designer_instance;

    InterfaceProperty<HeadlandDesigner,int>::set_qml_root(qml_object);
    InterfaceProperty<HeadlandDesigner,bool>::set_qml_root(qml_object);
    InterfaceProperty<HeadlandDesigner,double>::set_qml_root(qml_object);

    //example
    connect(qmlItem(qml_object,"somebtn"),SIGNAL(blah()),this,SLOT(load_headline()));
}

void FormHeadland::setFieldInfo(double maxFieldDistance, double fieldCenterX, double fieldCenterY) {
    this->maxFieldDistance = maxFieldDistance;
    this->fieldCenterX = fieldCenterX;
    this->fieldCenterY = fieldCenterY;
}

void FormHeadland::load_headline() {
    hdl->idx = -1;

    start = 99999; end = 99999;
    isA = true;
    hdl->desList.clear();

    sliceArr.clear();
    backupList.clear();
    sliceCount = 0;
    backupCount = 0;

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

void FormHeadland::clicked(int mouseX, int mouseY) {
    /*
    sliceArr.clear();

    Point ptt = oglSelf.PointToClient(Cursor.Position);

    int wid = oglSelf.Width;
    int halfWid = oglSelf.Width / 2;
    double scale = (double)wid * 0.903;

    if (cboxIsZoom.Checked && !zoomToggle)
    {
        sX = ((halfWid - (double)ptt.X) / wid) * 1.1;
        sY = ((halfWid - (double)ptt.Y) / -wid) * 1.1;
        zoom = 0.1;
        zoomToggle = true;
        return;
    }

    //Convert to Origin in the center of window, 800 pixels
    fixPt.X = ptt.X - halfWid;
    fixPt.Y = (wid - ptt.Y - halfWid);
    Vec3 plotPt(
            //convert screen coordinates to field coordinates
            easting = fixPt.X * mf.maxFieldDistance / scale * zoom,
            northing = fixPt.Y * mf.maxFieldDistance / scale * zoom,
            heading = 0
        );

    plotPt.easting += mf.fieldCenterX + mf.maxFieldDistance * -sX;
    plotPt.northing += mf.fieldCenterY + mf.maxFieldDistance * -sY;

    pint.easting = plotPt.easting;
    pint.northing = plotPt.northing;

    zoomToggle = false;
    zoom = 1;
    sX = 0;
    sY = 0;

    if (isA)
    {
        double minDistA = glm::DOUBLE_MAX;
        start = 99999; end = 99999;

        for (int j = 0; j < bnd->bndList.count(); j++)
        {
            for (int i = 0; i < bnd->bndList[j].fenceLine.count(); i++)
            {
                double dist = ((pint.easting - bnd->bndList[j].fenceLine[i].easting) * (pint.easting - bnd->bndList[j].fenceLine[i].easting))
                              + ((pint.northing - bnd->bndList[j].fenceLine[i].northing) * (pint.northing - bnd->bndList[j].fenceLine[i].northing));
                if (dist < minDistA)
                {
                    minDistA = dist;
                    bndSelect = j;
                    start = i;
                }
            }
        }

        isA = false;
    }
    else
    {
        double minDistA = glm::DOUBLE_MAX;
        int j = bndSelect;

        for (int i = 0; i < bnd->bndList[j].fenceLine.count(); i++)
        {
            double dist = ((pint.easting - bnd->bndList[j].fenceLine[i].easting) * (pint.easting - bnd->bndList[j].fenceLine[i].easting))
                          + ((pint.northing - bnd->bndList[j].fenceLine[i].northing) * (pint.northing - bnd->bndList[j].fenceLine[i].northing));
            if (dist < minDistA)
            {
                minDistA = dist;
                end = i;
            }
        }

        isA = true;

        //build the lines
        if ((bool)curveLine)
        {
            bool isLoop = false;
            int limit = end;

            if ((Math.Abs(start - end)) > (bnd->bndList[bndSelect].fenceLine.count() * 0.5))
            {
                if (start < end)
                {
                    (start, end) = (end, start);
                }

                isLoop = true;
                if (start < end)
                {
                    limit = end;
                    end = 0;
                }
                else
                {
                    limit = end;
                    end = bnd->bndList[bndSelect].fenceLine.count();
                }
            }
            else
            {
                if (start > end)
                {
                    (start, end) = (end, start);
                }
            }

            sliceArr.clear();
            vec3 pt3;

            if (start < end)
            {
                for (int i = start; i <= end; i++)
                {
                    //calculate the point inside the boundary
                    pt3 = bnd->bndList[bndSelect].fenceLine[i];
                    sliceArr.Add(pt3);

                    if (isLoop && i == bnd->bndList[bndSelect].fenceLine.count() - 1)
                    {
                        i = -1;
                        isLoop = false;
                        end = limit;
                    }
                }
            }
            else
            {
                for (int i = start; i >= end; i--)
                {
                    //calculate the point inside the boundary
                    pt3 = bnd->bndList[bndSelect].fenceLine[i];
                    sliceArr.Add(pt3);

                    if (isLoop && i == 0)
                    {
                        i = bnd->bndList[bndSelect].fenceLine.count() - 1;
                        isLoop = false;
                        end = limit;
                    }
                }
            }

            int ptCnt = sliceArr.count() - 1;

            if (ptCnt > 0)
            {
                //who knows which way it actually goes
                CalculateHeadings(sliceArr);

                for (int i = 1; i < 30; i++)
                {
                    Vec3 pt(sliceArr[ptCnt]);
                    pt.easting += (sin(pt.heading) * i);
                    pt.northing += (cos(pt.heading) * i);
                    sliceArr.Add(pt);
                }

                Vec3 stat(sliceArr[0]);

                for (int i = 1; i < 30; i++)
                {
                    Vec3 pt(stat);
                    pt.easting -= (sin(pt.heading) * i);
                    pt.northing -= (cos(pt.heading) * i);
                    sliceArr.Insert(0, pt);
                }

                mode = (int)TrackMode.Curve;
            }
            else
            {
                start = 99999; end = 99999;
                return;
            }

            //update the arrays
            start = 99999; end = 99999;
        }
        else //straight line
        {
            if ((Math.Abs(start - end)) > (bnd->bndList[bndSelect].fenceLine.count() * 0.5))
            {
                if (start < end)
                {
                    (start, end) = (end, start);
                }
            }
            else
            {
                if (start > end)
                {
                    (start, end) = (end, start);
                }
            }

            Vec3 ptA(bnd->bndList[bndSelect].fenceLine[start]);
            Vec3 ptB(bnd->bndList[bndSelect].fenceLine[end]);

            //calculate the AB Heading
            double abHead = Math.Atan2(
                bnd->bndList[bndSelect].fenceLine[end].easting - bnd->bndList[bndSelect].fenceLine[start].easting,
                bnd->bndList[bndSelect].fenceLine[end].northing - bnd->bndList[bndSelect].fenceLine[start].northing);
            if (abHead < 0) abHead += glm.twoPI;

            sliceArr.clear();

            ptA.heading = abHead;
            ptB.heading = abHead;

            for (int i = 0; i <= (int)(glm.Distance(ptA, ptB)); i++)
            {
                Vec3 ptC(
                        easting = (sin(abHead) * i) + ptA.easting,
                        northing = (cos(abHead) * i) + ptA.northing,
                        heading = abHead
                    );
                sliceArr.Add(ptC);
            }

            int ptCnt = sliceArr.count() - 1;

            for (int i = 1; i < 30; i++)
            {
                Vec3 pt(sliceArr[ptCnt]);
                pt.easting += (sin(pt.heading) * i);
                pt.northing += (cos(pt.heading) * i);
                sliceArr.Add(pt);
            }

            Vec3 stat(sliceArr[0]);

            for (int i = 1; i < 30; i++)
            {
                Vec3 pt(stat);
                pt.easting -= (sin(pt.heading) * i);
                pt.northing -= (cos(pt.heading) * i);
                sliceArr.Insert(0, pt);
            }

            mode = (int)TrackMode.AB;

            start = 99999; end = 99999;
        }

        //Move the line
        if ((double)lineDistance != 0)
            SetLineDistance();

        //QML: btnSlice.Enabled = true;
    }
    */
}

void FormHeadland::ogl_initialize() {

}

void FormHeadland::ogl_paint() {

}

void FormHeadland::ogl_resize() {

}
