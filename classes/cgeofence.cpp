#include "cgeofence.h"
#include "cgeofencelines.h"
#include "vec3.h"
#include "vec2.h"
#include "cturn.h"
#include "cboundary.h"
#include "cmazegrid.h"
#include "glm.h"
#include "glutils.h"

CGeoFence::CGeoFence()
{

}

void CGeoFence::findPointsDriveAround(CTurn &turn, const CBoundary &bnd,
                                      const CMazeGrid &mazeGrid,
                                      double minFieldX, double minFieldY,
                                      Vec3 fromPt, double headAB, Vec3 &start, Vec3 &stop)
{
    //initial scan is straight ahead of pivot point of vehicle to find the right turnLine/boundary
    Vec3 pt;

    bool isFound = false;
    int closestTurnNum = 99;
    int closestTurnIndex = 99999;
    int mazeDim = mazeGrid.mazeColXDim * mazeGrid.mazeRowYDim;

    double cosHead = cos(headAB);
    double sinHead = sin(headAB);

    for (int b = 1; b < 1600; b += 2)
    {
        pt.easting = fromPt.easting + (sinHead * b);
        pt.northing = fromPt.northing + (cosHead * b);

        if (turn.turnArr[0].isPointInTurnWorkArea(pt))
        {
            for (int t = 1; t < bnd.bndArr.size(); t++)
            {
                if (!bnd.bndArr[t].isSet || bnd.bndArr[t].isDriveThru) continue;

                if (bnd.bndArr[t].isDriveAround)
                {
                    if (geoFenceArr[t].isPointInGeoFenceArea(pt))
                    {
                        isFound = true;
                        closestTurnNum = t;
                        closestTurnIndex = b;

                        start.easting = fromPt.easting + (sinHead * b);
                        start.northing = fromPt.northing + (cosHead * b);
                        start.heading = headAB;
                        break;
                    }
                }
                else
                {
                    //its a uturn obstacle
                    if (turn.turnArr[t].isPointInTurnWorkArea(pt))
                    {
                        start.easting = 88888;
                        return;
                    }
                }
            }
        }
        else
        {
            isFound = true;
            start.easting = 88888;
            closestTurnNum = 0;
            return;
        }
        if (isFound) break;
    }

    isFound = false;

    for (int b = closestTurnIndex + 200; b > closestTurnIndex; b--)
    {
        pt.easting = fromPt.easting + (sinHead * b);
        pt.northing = fromPt.northing + (cosHead * b);

        if (geoFenceArr[closestTurnNum].isPointInGeoFenceArea(pt))
        {
            isFound = true;

            stop.easting = fromPt.easting + (sinHead * b);
            stop.northing = fromPt.northing + (cosHead * b);
            stop.heading = headAB;
        }

        if (isFound) break;
    }

    for (int i = 0; i < 30; i++)
    {
        start.easting -= sinHead;
        start.northing -= cosHead;
        start.heading = headAB;

        int iStart = (int)((((int)((start.northing - minFieldY) / mazeGrid.mazeScale)) * mazeGrid.mazeColXDim)
            + (int)((start.easting - minFieldX) / mazeGrid.mazeScale));

        if (iStart >= mazeDim)
        {
            isFound = true;
            start.easting = 88888;
            closestTurnNum = 0;
            return;
        }

        if (mazeGrid.mazeArr[iStart] == 0) break;
    }

    for (int i = 0; i < 30; i++)
    {
        stop.easting += sinHead;
        stop.northing += cosHead;
        stop.heading = headAB;

        int iStop = (int)((((int)((stop.northing - minFieldY) / mazeGrid.mazeScale)) * mazeGrid.mazeColXDim)
            + (int)((stop.easting - minFieldX) / mazeGrid.mazeScale));

        if (iStop >= mazeDim)
        {
            isFound = true;
            start.easting = 88888;
            closestTurnNum = 0;
            return;
        }

        if (mazeGrid.mazeArr[iStop] == 0) break;
    }

}

bool CGeoFence::isPointInsideGeoFences(const CBoundary &bnd, const Vec3 pt)
{
    //if inside outer boundary, then potentially add
    if (geoFenceArr.size() > 0 && geoFenceArr[0].isPointInGeoFenceArea(pt))
    {
        for (int b = 1; b < bnd.bndArr.size(); b++)
        {
            if (bnd.bndArr[b].isSet)
            {
                if (geoFenceArr[b].isPointInGeoFenceArea(pt))
                {
                    //point is in an inner turn area but inside outer
                    return false;
                }
            }
        }
        return true;
    }
    else
    {
        return false;
    }

}

bool CGeoFence::isPointInsideGeoFences(const CBoundary &bnd, const Vec2 pt)
{

    //if inside outer boundary, then potentially add
    if (geoFenceArr.size() > 0 && geoFenceArr[0].isPointInGeoFenceArea(pt))
    {
        for (int b = 1; b < bnd.bndArr.size(); b++)
        {
            if (bnd.bndArr[b].isSet)
            {
                if (geoFenceArr[b].isPointInGeoFenceArea(pt))
                {
                    //point is in an inner turn area but inside outer
                    return false;
                }
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

void CGeoFence::buildGeoFenceLines(CBoundary &bnd, double toolWidth, double ytGeoFenceDistance)
{
    if (bnd.bndArr.size() == 0)
    {
        return;
    }

    //to fill the list of line points
    Vec3 point;

    //determine how wide a headland space
    double totalHeadWidth = ytGeoFenceDistance;

    //outside boundary - count the points from the boundary
    geoFenceArr[0].geoFenceLine.clear();
    int ptCount = bnd.bndArr[0].bndLine.size();
    for (int i = ptCount - 1; i >= 0; i--)
    {
        //calculate the point inside the boundary
        point.easting = bnd.bndArr[0].bndLine[i].easting + (-sin(glm::PIBy2 + bnd.bndArr[0].bndLine[i].heading) * totalHeadWidth);
        point.northing = bnd.bndArr[0].bndLine[i].northing + (-cos(glm::PIBy2 + bnd.bndArr[0].bndLine[i].heading) * totalHeadWidth);
        point.heading = bnd.bndArr[0].bndLine[i].heading;
        if (point.heading < -glm::twoPI) point.heading += glm::twoPI;

        //only add if inside actual field boundary
        if (bnd.bndArr[0].isPointInsideBoundary(point))
        {
            Vec2 tPnt(point.easting, point.northing);
            geoFenceArr[0].geoFenceLine.append(tPnt);
        }
    }
    geoFenceArr[0].fixGeoFenceLine(totalHeadWidth, bnd.bndArr[0].bndLine, toolWidth);
    geoFenceArr[0].preCalcTurnLines();

    //inside boundaries
    for (int j = 1; j < bnd.bndArr.size(); j++)
    {
        geoFenceArr[j].geoFenceLine.clear();
        if (!bnd.bndArr[j].isSet || bnd.bndArr[j].isDriveThru) continue;

        ptCount = bnd.bndArr[j].bndLine.size();

        for (int i = ptCount - 1; i >= 0; i--)
        {
            //calculate the point outside the boundary
            point.easting = bnd.bndArr[j].bndLine[i].easting + (-sin(glm::PIBy2 + bnd.bndArr[j].bndLine[i].heading) * totalHeadWidth);
            point.northing = bnd.bndArr[j].bndLine[i].northing + (-cos(glm::PIBy2 + bnd.bndArr[j].bndLine[i].heading) * totalHeadWidth);
            point.heading = bnd.bndArr[j].bndLine[i].heading;
            if (point.heading < -glm::twoPI) point.heading += glm::twoPI;

            //only add if outside actual field boundary
            if (!bnd.bndArr[j].isPointInsideBoundary(point))
            {
                Vec2 tPnt(point.easting, point.northing);
                geoFenceArr[j].geoFenceLine.append(tPnt);
            }
        }
        geoFenceArr[j].fixGeoFenceLine(totalHeadWidth, bnd.bndArr[j].bndLine, toolWidth * 0.5);
        geoFenceArr[j].preCalcTurnLines();
    }
}

void CGeoFence::drawGeoFenceLines(const CBoundary &bnd, QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    for (int i = 0; i < bnd.bndArr.size(); i++)
    {
        if (bnd.bndArr[i].isSet)
            geoFenceArr[i].drawGeoFenceLine(gl, mvp);
    }
}
