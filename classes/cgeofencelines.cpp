#include "cgeofencelines.h"
#include "glm.h"
#include "glutils.h"

CGeoFenceLines::CGeoFenceLines()
{

}

void CGeoFenceLines::reset()
{
    calcList.clear();
    geoFenceLine.clear();
}

bool CGeoFenceLines::isPointInGeoFenceArea(Vec3 testPointv2)
{
    if (calcList.size() < 3) return false;
    int j = geoFenceLine.size() - 1;
    bool oddNodes = false;

    //test against the constant and multiples list the test point
    for (int i = 0; i < geoFenceLine.size(); j = i++)
    {
        if ((geoFenceLine[i].northing < testPointv2.northing && geoFenceLine[j].northing >= testPointv2.northing)
        || (geoFenceLine[j].northing < testPointv2.northing && geoFenceLine[i].northing >= testPointv2.northing))
        {
            oddNodes ^= ((testPointv2.northing * calcList[i].northing) + calcList[i].easting < testPointv2.easting);
        }
    }
    return oddNodes; //true means inside.
}

bool CGeoFenceLines::isPointInGeoFenceArea(Vec2 testPointv2)
{
    if (calcList.size() < 3) return false;
    int j = geoFenceLine.size() - 1;
    bool oddNodes = false;

    //test against the constant and multiples list the test point
    for (int i = 0; i < geoFenceLine.size(); j = i++)
    {
        if ((geoFenceLine[i].northing < testPointv2.northing && geoFenceLine[j].northing >= testPointv2.northing)
        || (geoFenceLine[j].northing < testPointv2.northing && geoFenceLine[i].northing >= testPointv2.northing))
        {
            oddNodes ^= ((testPointv2.northing * calcList[i].northing) + calcList[i].easting < testPointv2.easting);
        }
    }
    return oddNodes; //true means inside.

}

void CGeoFenceLines::drawGeoFenceLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    ////draw the turn line oject
    if (geoFenceLine.size() < 1) return;
    int ptCount = geoFenceLine.size();

    GLHelperOneColor gldraw;


    gl->glLineWidth(3);
    for (int h = 0; h < ptCount; h++)
        gldraw.append(QVector3D(geoFenceLine[h].easting, geoFenceLine[h].northing, 0));

    gldraw.append(QVector3D(geoFenceLine[0].easting, geoFenceLine[0].northing, 0));

    gldraw.draw(gl, mvp, QColor::fromRgbF(0.96555f, 0.1232f, 0.50f),
                GL_LINE_STRIP, 3);

}

void CGeoFenceLines::fixGeoFenceLine(double totalHeadWidth, QVector<Vec3> curBnd, double spacing)
{
    //count the points from the boundary
    int lineCount = geoFenceLine.size();
    double distance = 0;

    //int headCount = mf.bndArr[inTurnNum].bndLine.size();
    int bndCount = curBnd.size();

    //remove the points too close to boundary
    for (int i = 0; i < bndCount; i++)
    {
        for (int j = 0; j < lineCount; j++)
        {
            //make sure distance between headland and boundary is not less then width
            distance = glm::distance(curBnd[i], geoFenceLine[j]);
            if (distance < (totalHeadWidth * 0.96))
            {
                geoFenceLine.removeAt(j);
                lineCount = geoFenceLine.size();
                j = -1;
            }
        }
    }

    //make sure distance isn't too small between points on turnLine
    bndCount = geoFenceLine.size();

    //double spacing = mf.tool.toolWidth * 0.25;
    for (int i = 0; i < bndCount - 1; i++)
    {
        distance = glm::distance(geoFenceLine[i], geoFenceLine[i + 1]);
        if (distance < spacing)
        {
            geoFenceLine.removeAt(i + 1);
            bndCount = geoFenceLine.size();
            i--;
        }
    }

    //make sure distance isn't too big between points on Turn
    bndCount = geoFenceLine.size();
    for (int i = 0; i < bndCount; i++)
    {
        int j = i + 1;
        if (j == bndCount) j = 0;
        distance = glm::distance(geoFenceLine[i], geoFenceLine[j]);
        if (distance > (spacing * 1.25))
        {
            Vec2 pointB = Vec2((geoFenceLine[i].easting + geoFenceLine[j].easting) / 2.0, (geoFenceLine[i].northing + geoFenceLine[j].northing) / 2.0);

            geoFenceLine.insert(j, pointB);
            bndCount = geoFenceLine.size();
            i--;
        }
    }
}

void CGeoFenceLines::preCalcTurnLines()
{
    int j = geoFenceLine.size() - 1;
    //clear the list, constant is easting, multiple is northing
    calcList.clear();
    Vec2 constantMultiple = Vec2(0, 0);

    for (int i = 0; i < geoFenceLine.size(); j = i++)
    {
        //check for divide by zero
        if (fabs(geoFenceLine[i].northing - geoFenceLine[j].northing) < glm::DOUBLE_EPSILON)
        {
            constantMultiple.easting = geoFenceLine[i].easting;
            constantMultiple.northing = 0;
            calcList.append(constantMultiple);
        }
        else
        {
            //determine constant and multiple and add to list
            constantMultiple.easting = geoFenceLine[i].easting - ((geoFenceLine[i].northing * geoFenceLine[j].easting)
                            / (geoFenceLine[j].northing - geoFenceLine[i].northing)) + ((geoFenceLine[i].northing * geoFenceLine[i].easting)
                                / (geoFenceLine[j].northing - geoFenceLine[i].northing));
            constantMultiple.northing = (geoFenceLine[j].easting - geoFenceLine[i].easting) / (geoFenceLine[j].northing - geoFenceLine[i].northing);
            calcList.append(constantMultiple);
        }
    }

}
