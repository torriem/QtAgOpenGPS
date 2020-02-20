#include "cturn.h"
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <math.h>
#include "glm.h"
#include "glutils.h"
#include "vec3.h"
#include "vec2.h"
#include "aogsettings.h"
#include "cboundary.h"
#include "ctool.h"

CTurn::CTurn()
{
    //box for finding closest point
    boxA = Vec2(9000, 9000);
    boxB = Vec2(9000, 9002);
    boxC = Vec2(9001, 9001);
    boxD = Vec2(9002, 9003);

    //point at the farthest turn segment from pivotAxle
    closestTurnPt = Vec3(-10000, -10000, 9);

    turnSelected = 0;
    boxLength = 2000;
}

//called only by CYouTurn
//requires mf.bnd (CBoundary), mf.turn?, CTool
void CTurn::findClosestTurnPoint(const CBoundary &bnd, const CTool &tool, bool isYouTurnRight, Vec3 fromPt, double headAB)
{
    //initial scan is straight ahead of pivot point of vehicle to find the right turnLine/boundary
    Vec3 pt;
    Vec3 rayPt;

    bool isFound = false;
    int closestTurnNum = 99;

    double cosHead = cos(headAB);
    double sinHead = sin(headAB);

    for (int b = 1; b < 1500; b += 2)
    {
        pt.easting = fromPt.easting + (sinHead * b);
        pt.northing = fromPt.northing + (cosHead * b);

        if (turnArr[0].isPointInTurnWorkArea(pt))
        {
            for (int t = 1; t < bnd.bndArr.size(); t++)
            {
                if (!bnd.bndArr[t].isSet) continue;
                if (bnd.bndArr[t].isDriveThru) continue;
                if (bnd.bndArr[t].isDriveAround) continue;
                if (mf.turn.turnArr[t].isPointInTurnWorkArea(pt))
                {
                    isFound = true;
                    closestTurnNum = t;
                    rayPt.easting = pt.easting;
                    rayPt.northing = pt.northing;
                    break;
                }
            }
            if (isFound) break;
        }
        else
        {
            closestTurnNum = 0;
            rayPt.easting = pt.easting;
            rayPt.northing = pt.northing;
            break;
        }
    }

    //second scan is straight ahead of outside of tool based on turn direction
    double scanWidthL, scanWidthR;
    if (isYouTurnRight) //its actually left
    {
        scanWidthL = -(tool.toolWidth * 0.25) - (tool.toolWidth * 0.5);
        scanWidthR = (tool.toolWidth * 0.25) - (tool.toolWidth * 0.5);
    }
    else
    {
        scanWidthL = -(tool.toolWidth * 0.25) + (tool.toolWidth * 0.5);
        scanWidthR = (tool.toolWidth * 0.25) + (tool.toolWidth * 0.5);
    }

    //isYouTurnRight actuall means turning left - Painful, but it switches later
    boxA.easting = fromPt.easting + (sin(headAB + glm::PIBy2) * scanWidthL);
    boxA.northing = fromPt.northing + (cos(headAB + glm::PIBy2) * scanWidthL);

    boxB.easting = fromPt.easting + (sin(headAB + glm::PIBy2) * scanWidthR);
    boxB.northing = fromPt.northing + (cos(headAB + glm::PIBy2) * scanWidthR);

    boxC.easting = boxB.easting + (sin(headAB) * boxLength);
    boxC.northing = boxB.northing + (cos(headAB) * boxLength);

    boxD.easting = boxA.easting + (sin(headAB) * boxLength);
    boxD.northing = boxA.northing + (cos(headAB) * boxLength);

    //determine if point is inside bounding box of the 1 turn chosen above
    turnClosestList.clear();

    //I can only find one instance of CTurn in AOG.  So isn't
    //mf.turn always this?
    mf.turn.closestTurnNum = closestTurnNum;
    Vec4 inBox;

    int ptCount = turnArr[closestTurnNum].turnLine.size();
    for (int p = 0; p < ptCount; p++)
    {
        if ((((boxB.easting - boxA.easting) * (turnArr[closestTurnNum].turnLine[p].northing - boxA.northing))
                - ((boxB.northing - boxA.northing) * (turnArr[closestTurnNum].turnLine[p].easting - boxA.easting))) < 0) { continue; }

        if ((((boxD.easting - boxC.easting) * (turnArr[closestTurnNum].turnLine[p].northing - boxC.northing))
                - ((boxD.northing - boxC.northing) * (turnArr[closestTurnNum].turnLine[p].easting - boxC.easting))) < 0) { continue; }

        if ((((boxC.easting - boxB.easting) * (turnArr[closestTurnNum].turnLine[p].northing - boxB.northing))
                - ((boxC.northing - boxB.northing) * (turnArr[closestTurnNum].turnLine[p].easting - boxB.easting))) < 0) { continue; }

        if ((((boxA.easting - boxD.easting) * (turnArr[closestTurnNum].turnLine[p].northing - boxD.northing))
                - ((boxA.northing - boxD.northing) * (turnArr[closestTurnNum].turnLine[p].easting - boxD.easting))) < 0) { continue; }

        //it's in the box, so add to list
        inBox.easting = turnArr[closestTurnNum].turnLine[p].easting;
        inBox.northing = turnArr[closestTurnNum].turnLine[p].northing;
        inBox.heading = turnArr[closestTurnNum].turnLine[p].heading;
        inBox.index = closestTurnNum;

        //which turn/headland is it from
        turnClosestList.append(inBox);
    }

    if (turnClosestList.size() == 0)
    {
        if (isYouTurnRight) //its actually left
        {
            scanWidthL = -(tool.toolWidth * 0.5);
            scanWidthR = 0;
        }
        else
        {
            scanWidthL = 0;
            scanWidthR = (tool.toolWidth * 0.5);
        }

        //isYouTurnRight actuall means turning left - Painful, but it switches later
        boxA.easting = fromPt.easting + (sin(headAB + glm::PIBy2) * scanWidthL);
        boxA.northing = fromPt.northing + (cos(headAB + glm::PIBy2) * scanWidthL);

        boxB.easting = fromPt.easting + (sin(headAB + glm::PIBy2) * scanWidthR);
        boxB.northing = fromPt.northing + (cos(headAB + glm::PIBy2) * scanWidthR);

        boxC.easting = boxB.easting + (sin(headAB) * boxLength);
        boxC.northing = boxB.northing + (cos(headAB) * boxLength);

        boxD.easting = boxA.easting + (sin(headAB) * boxLength);
        boxD.northing = boxA.northing + (cos(headAB) * boxLength);

        //determine if point is inside bounding box of the 1 turn chosen above
        turnClosestList.clear();

        mf.turn.closestTurnNum = closestTurnNum;

        ptCount = turnArr[closestTurnNum].turnLine.size();
        for (int p = 0; p < ptCount; p++)
        {
            if ((((boxB.easting - boxA.easting) * (turnArr[closestTurnNum].turnLine[p].northing - boxA.northing))
                    - ((boxB.northing - boxA.northing) * (turnArr[closestTurnNum].turnLine[p].easting - boxA.easting))) < 0) { continue; }

            if ((((boxD.easting - boxC.easting) * (turnArr[closestTurnNum].turnLine[p].northing - boxC.northing))
                    - ((boxD.northing - boxC.northing) * (turnArr[closestTurnNum].turnLine[p].easting - boxC.easting))) < 0) { continue; }

            if ((((boxC.easting - boxB.easting) * (turnArr[closestTurnNum].turnLine[p].northing - boxB.northing))
                    - ((boxC.northing - boxB.northing) * (turnArr[closestTurnNum].turnLine[p].easting - boxB.easting))) < 0) { continue; }

            if ((((boxA.easting - boxD.easting) * (turnArr[closestTurnNum].turnLine[p].northing - boxD.northing))
                    - ((boxA.northing - boxD.northing) * (turnArr[closestTurnNum].turnLine[p].easting - boxD.easting))) < 0) { continue; }

            //it's in the box, so add to list
            inBox.easting = turnArr[closestTurnNum].turnLine[p].easting;
            inBox.northing = turnArr[closestTurnNum].turnLine[p].northing;
            inBox.heading = turnArr[closestTurnNum].turnLine[p].heading;
            inBox.index = closestTurnNum;

            //which turn/headland is it from
            turnClosestList.append(inBox);
        }
    }
    //which of the points is closest
    //closestTurnPt.easting = -20000; closestTurnPt.northing = -20000;
    ptCount = turnClosestList.size();
    if (ptCount != 0)
    {
        double totalDist = 0.75 * sqrt(((fromPt.easting - rayPt.easting) * (fromPt.easting - rayPt.easting))
        + ((fromPt.northing - rayPt.northing) * (fromPt.northing - rayPt.northing)));

        //determine closest point
        double minDistance = 9999999;
        for (int i = 0; i < ptCount; i++)
        {
            double dist = sqrt(((fromPt.easting - turnClosestList[i].easting) * (fromPt.easting - turnClosestList[i].easting))
                            + ((fromPt.northing - turnClosestList[i].northing) * (fromPt.northing - turnClosestList[i].northing)));

            //double distRay = ((rayPt.easting - turnClosestList[i].easting) * (rayPt.easting - turnClosestList[i].easting))
            //                + ((rayPt.northing - turnClosestList[i].northing) * (rayPt.northing - turnClosestList[i].northing));

            if (minDistance >= dist && dist > totalDist)
            {
                minDistance = dist;
                closestTurnPt.easting = turnClosestList[i].easting;
                closestTurnPt.northing = turnClosestList[i].northing;
                closestTurnPt.heading = turnClosestList[i].heading;
            }
        }
        if (closestTurnPt.heading < 0) closestTurnPt.heading += glm::twoPI;
    }
}

//not currently called from anywhere
bool CTurn::pointInsideWorkArea(const CBoundary &bnd, Vec2 pt)
{
    //if inside outer boundary, then potentially add
    if (turnArr[0].isPointInTurnWorkArea(pt))
    {
        for (int b = 1; b < bnd.bndArr.size(); b++)
        {
            if (bnd.bndArr[b].isSet)
            {
                if (turnArr[b].isPointInTurnWorkArea(pt))
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

void CTurn::resetTurnLines()
{
    turnArr.clear();
}

//called by main forms
void CTurn::buildTurnLines(const CBoundary &bnd, const CTool &tool)
{
    //update the GUI values for boundaries
    mf.fd.UpdateFieldBoundaryGUIAreas();

    if (bnd.bndArr.size() == 0)
    {
        mf.TimedMessageBox(1500, " No Boundaries", "No Turn Lines Made");
        return;
    }

    //to fill the list of line points
    Vec3 point;

    //determine how wide a headland space
    double totalHeadWidth = mf.yt.triggerDistanceOffset;

    //outside boundary - count the points from the boundary
    turnArr[0].turnLine.clear();
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
            Vec3 tPnt(point.easting, point.northing, point.heading);
            turnArr[0].turnLine.append(tPnt);
        }
    }
    turnArr[0].fixTurnLine(totalHeadWidth, bnd.bndArr[0].bndLine, tool.toolWidth * 0.25);
    turnArr[0].preCalcTurnLines();

    //inside boundaries
    for (int j = 1; j < bnd.bndArr.size(); j++)
    {
        turnArr[j].turnLine.clear();
        if (!bnd.bndArr[j].isSet || bnd.bndArr[j].isDriveThru || bnd.bndArr[j].isDriveAround) continue;

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
                Vec3 tPnt(point.easting, point.northing, point.heading);
                turnArr[j].turnLine.append(tPnt);
            }
        }
        turnArr[j].fixTurnLine(totalHeadWidth, bnd.bndArr[j].bndLine, tool.toolWidth * 0.4);
        turnArr[j].preCalcTurnLines();
    }

    //mf.TimedMessageBox(800, "Turn Lines", "Turn limits Created");
}

//called by main form
void CTurn::drawTurnLines(const CBoundary &bnd, QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    USE_SETTINGS;

    for (int i = 0; i < bnd.bndArr.size(); i++)
    {
        if (!bnd.bndArr[i].isSet && bnd.bndArr[i].isDriveAround) continue;
        //turnArr[i].DrawTurnLine();
        {
            ////draw the turn line oject
            int ptCount = mf.turn.turnArr[i].turnLine.size();
            if (ptCount < 1) continue;

            GLHelperOneColor gldraw;
            for (int h = 0; h < ptCount; h++)
                gldraw.append(QVector3D(mf.turn.turnArr[i].turnLine[h].easting, mf.turn.turnArr[i].turnLine[h].northing, 0));

            gldraw.append(QVector3D(mf.turn.turnArr[i].turnLine[0].easting, mf.turn.turnArr[i].turnLine[0].northing, 0));

            gldraw.draw(gl,mvp,QColor::fromRgbF(0.3555f, 0.6232f, 0.20f),GL_LINE_STRIP,
                        SETTINGS_DISPLAY_LINEWIDTH);
        }
    }
}

/*
//unused currently
void CTurn::drawClosestPoint(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
}
*/
