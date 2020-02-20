#include "cturnlines.h"
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector>
#include <math.h>
#include "glm.h"
#include "glutils.h"

CTurnLines::CTurnLines()
{

}

void CTurnLines::calculateTurnHeadings()
{
    //to calc heading based on next and previous points to give an average heading.
    int cnt = turnLine.size();
    QVector<Vec3> arr;

    arr = turnLine; //copy the list
    cnt--; //should point to last item

    turnLine.clear();

    //first point needs last, first, second points
    Vec3 pt3 = arr[0];
    pt3.heading = atan2(arr[1].easting - arr[cnt].easting, arr[1].northing - arr[cnt].northing);
    if (pt3.heading < 0) pt3.heading += glm::twoPI;
    turnLine.append(pt3);

    //middle points
    for (int i = 1; i < cnt; i++)
    {
        pt3 = arr[i];
        pt3.heading = atan2(arr[i + 1].easting - arr[i - 1].easting, arr[i + 1].northing - arr[i - 1].northing);
        if (pt3.heading < 0) pt3.heading += glm::twoPI;
        turnLine.append(pt3);
    }

    //last and first point
    pt3 = arr[cnt];
    pt3.heading = atan2(arr[0].easting - arr[cnt - 1].easting, arr[0].northing - arr[cnt - 1].northing);
    if (pt3.heading < 0) pt3.heading += glm::twoPI;
    turnLine.append(pt3);
}

void CTurnLines::resetTurn()
{
    calcList.clear();
    turnLine.clear();
}

void CTurnLines::fixTurnLine(double totalHeadWidth, const QVector<Vec3> &curBnd, double spacing)
{
    //count the points from the boundary
    int lineCount = turnLine.size();
    double distance = 0;

    //int headCount = mf.bndArr[inTurnNum].bndLine.size();
    int bndCount = curBnd.size();

    //remove the points too close to boundary
    for (int i = 0; i < bndCount; i++)
    {
        for (int j = 0; j < lineCount; j++)
        {
            //make sure distance between headland and boundary is not less then width
            distance = glm::distance(curBnd[i], turnLine[j]);
            if (distance < (totalHeadWidth * 0.96))
            {
                turnLine.removeAt(j);
                lineCount = turnLine.size();
                j = -1;
            }
        }
    }

    //make sure distance isn't too small between points on turnLine
    bndCount = turnLine.size();

    //double spacing = mf.tool.toolWidth * 0.25;
    for (int i = 0; i < bndCount - 1; i++)
    {
        distance = glm::distance(turnLine[i], turnLine[i + 1]);
        if (distance < spacing)
        {
            turnLine.removeAt(i + 1);
            bndCount = turnLine.size();
            i--;
        }
    }

    //make sure distance isn't too big between points on Turn
    bndCount = turnLine.size();
    for (int i = 0; i < bndCount; i++)
    {
        int j = i + 1;
        if (j == bndCount) j = 0;
        distance = glm::distance(turnLine[i], turnLine[j]);
        if (distance > (spacing * 1.25))
        {
            Vec3 pointB = Vec3((turnLine[i].easting + turnLine[j].easting) / 2.0, (turnLine[i].northing + turnLine[j].northing) / 2.0, turnLine[i].heading);

            turnLine.insert(j, pointB);
            bndCount = turnLine.size();
            i--;
        }
    }

    //make sure headings are correct for calculated points

    //Tate Button contributed this bug fix.
    if (turnLine.size() > 0)
    {
        calculateTurnHeadings();
    }

}


void CTurnLines::preCalcTurnLines()
{
    int j = turnLine.size() - 1;
    //clear the list, constant is easting, multiple is northing
    calcList.clear();
    Vec2 constantMultiple(0, 0);

    for (int i = 0; i < turnLine.size(); j = i++)
    {
        //check for divide by zero
        if (fabs(turnLine[i].northing - turnLine[j].northing) < 0.00000000001)
        {
            constantMultiple.easting = turnLine[i].easting;
            constantMultiple.northing = 0;
            calcList.append(constantMultiple);
        }
        else
        {
            //determine constant and multiple and add to list
            constantMultiple.easting = turnLine[i].easting - ((turnLine[i].northing * turnLine[j].easting)
                            / (turnLine[j].northing - turnLine[i].northing)) + ((turnLine[i].northing * turnLine[i].easting)
                                / (turnLine[j].northing - turnLine[i].northing));
            constantMultiple.northing = (turnLine[j].easting - turnLine[i].easting) / (turnLine[j].northing - turnLine[i].northing);
            calcList.append(constantMultiple);
        }
    }
}

bool CTurnLines::isPointInTurnWorkArea(Vec3 testPointv3)
{
    if (calcList.size() < 3) return false;
    int j = turnLine.size() - 1;
    bool oddNodes = false;

    //test against the constant and multiples list the test point
    for (int i = 0; i < turnLine.size(); j = i++)
    {
        if ((turnLine[i].northing < testPointv3.northing && turnLine[j].northing >= testPointv3.northing)
        || (turnLine[j].northing < testPointv3.northing && turnLine[i].northing >= testPointv3.northing))
        {
            oddNodes ^= ((testPointv3.northing * calcList[i].northing) + calcList[i].easting < testPointv3.easting);
        }
    }
    return oddNodes; //true means inside.
}

bool CTurnLines::isPointInTurnWorkArea(Vec2 testPointv2)
{
    if (calcList.size() < 3) return false;
    int j = turnLine.size() - 1;
    bool oddNodes = false;

    //test against the constant and multiples list the test point
    for (int i = 0; i < turnLine.size(); j = i++)
    {
        if ((turnLine[i].northing < testPointv2.northing && turnLine[j].northing >= testPointv2.northing)
        || (turnLine[j].northing < testPointv2.northing && turnLine[i].northing >= testPointv2.northing))
        {
            oddNodes ^= ((testPointv2.northing * calcList[i].northing) + calcList[i].easting < testPointv2.easting);
        }
    }
    return oddNodes; //true means inside.
}

void CTurnLines::drawTurnLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    ////draw the turn line oject
    int ptCount = turnLine.size();
    if (ptCount < 1) return;

    GLHelperOneColor gldraw;


    for (int h = 0; h < ptCount; h++)
        gldraw.append(QVector3D(turnLine[h].easting, turnLine[h].northing, 0));

    gldraw.append(QVector3D(turnLine[0].easting, turnLine[0].northing, 0));

    gldraw.draw(gl,mvp,QColor::fromRgbF(0.8555f, 0.9232f, 0.60f),GL_POINTS,2.0f);
}
