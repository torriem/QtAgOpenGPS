#include "cboundarylines.h"
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <math.h>
#include "glm.h"
#include "glutils.h"


CBoundaryLines::CBoundaryLines()
{
    area = 0;
    isDriveThru = false;
}

void CBoundaryLines::calculateFenceLineHeadings()
{
    //to calc heading based on next and previous points to give an average heading.
    int cnt = fenceLine.count();
    QVector<Vec3> arr = fenceLine; //make copy of list
    cnt--;
    fenceLine.clear();

    //first point needs last, first, second points
    Vec3 pt3 = arr[0];
    pt3.heading = atan2(arr[1].easting - arr[cnt].easting,
                        arr[1].northing - arr[cnt].northing);
    if (pt3.heading < 0) pt3.heading += glm::twoPI;
    fenceLine.append(pt3);

    //middle points
    for (int i = 1; i < cnt; i++)
    {
        pt3 = arr[i];
        pt3.heading = atan2(arr[i + 1].easting - arr[i - 1].easting,
                            arr[i + 1].northing - arr[i - 1].northing);
        if (pt3.heading < 0) pt3.heading += glm::twoPI;
        fenceLine.append(pt3);
    }

    //last and first point
    pt3 = arr[cnt];
    pt3.heading = atan2(arr[0].easting - arr[cnt - 1].easting,
                        arr[0].northing - arr[cnt - 1].northing);
    if (pt3.heading < 0) pt3.heading += glm::twoPI;
    fenceLine.append(pt3);
}

void CBoundaryLines::fixFenceLine(int bndNum)
{
    double spacing;
    //close if less than 20 ha, 40ha, more
    if (area < 200000) spacing = 1.1;
    else if (area < 400000) spacing = 2.2;
    else spacing = 3.3;

    if (bndNum > 0) spacing *= 0.5;

    int bndCount = fenceLine.count();
    double distance;

    //make sure distance isn't too big between points on headland
    for (int i = 0; i < bndCount - 1; i++)
    {
        int j = i + 1;

        if (j == bndCount) j = 0;
        distance = glm::distance(fenceLine[i], fenceLine[j]);
        if (distance > spacing * 1.5)
        {
            Vec3 pointB((fenceLine[i].easting + fenceLine[j].easting) / 2.0,
                        (fenceLine[i].northing + fenceLine[j].northing) / 2.0,
                        fenceLine[i].heading);

            fenceLine.insert(j, pointB);
            bndCount = fenceLine.count();
            i--;
        }
    }

    //make sure distance isn't too big between points on boundary
    bndCount = fenceLine.count();

    for (int i = 0; i < bndCount; i++)
    {
        int j = i + 1;

        if (j == bndCount) j = 0;
        distance = glm::distance(fenceLine[i], fenceLine[j]);
        if (distance > spacing * 1.6)
        {
            Vec3 pointB((fenceLine[i].easting + fenceLine[j].easting) / 2.0,
                        (fenceLine[i].northing + fenceLine[j].northing) / 2.0,
                        fenceLine[i].heading);

            fenceLine.insert(j, pointB);
            bndCount = fenceLine.count();
            i--;
        }
    }

    //make sure distance isn't too small between points on boundary
    spacing *= 0.9;
    bndCount = fenceLine.count();
    for (int i = 0; i < bndCount - 1; i++)
    {
        distance = glm::distance(fenceLine[i], fenceLine[i + 1]);
        if (distance < spacing)
        {
            fenceLine.removeAt(i + 1);
            bndCount = fenceLine.count();
            i--;
        }
    }

    //make sure headings are correct for calculated points
    calculateFenceLineHeadings();

    double delta = 0;
    fenceLineEar.clear();

    for (int i = 0; i < fenceLine.count(); i++)
    {
        if (i == 0)
        {
            fenceLineEar.append(Vec2(fenceLine[i].easting, fenceLine[i].northing));
            continue;
        }
        delta += (fenceLine[i - 1].heading - fenceLine[i].heading);
        if (fabs(delta) > 0.005)
        {
            fenceLineEar.append(Vec2(fenceLine[i].easting, fenceLine[i].northing));
            delta = 0;
        }
    }
}

void CBoundaryLines::reverseWinding()
{
    //reverse the boundary
    int cnt = fenceLine.size();
    QVector<Vec3> arr = fenceLine;
    cnt--;
    fenceLine.clear();
    for (int i = cnt; i >= 0; i--)
    {
        arr[i].heading -= M_PI;
        if (arr[i].heading < 0) arr[i].heading += glm::twoPI;
        fenceLine.append(arr[i]);
    }
}

//obvious
bool CBoundaryLines::calculateFenceArea(int idx)
{
    int ptCount = fenceLine.count();
    if (ptCount < 1) return false;

    area = 0;         // Accumulates area in the loop
    int j = ptCount - 1;  // The last vertex is the 'previous' one to the first

    for (int i = 0; i < ptCount; j = i++)
    {
        area += (fenceLine[j].easting + fenceLine[i].easting) *
                (fenceLine[j].northing - fenceLine[i].northing);
    }

    bool isClockwise = area >= 0;

    area = fabs(area / 2);

    //make sure is clockwise for outer counter clockwise for inner
    if ((idx == 0 && isClockwise) || (idx > 0 && !isClockwise))
    {
        reverseWinding();
    }

    return isClockwise;
}

void CBoundaryLines::calculateTurnHeadings()
{
    //to calc heading based on next and previous points to give an average heading.
    int cnt = turnLine.count();
    QVector<Vec3> arr = turnLine;
    cnt--;
    turnLine.clear();

    //first point needs last, first, second points
    Vec3 pt3(arr[0]);
    pt3.heading = atan2(arr[1].easting - arr[cnt].easting,
                        arr[1].northing - arr[cnt].northing);
    if (pt3.heading < 0) pt3.heading += glm::twoPI;
    turnLine.append(pt3);

    //middle points
    for (int i = 1; i < cnt; i++)
    {
        pt3 = arr[i];
        pt3.heading = atan2(arr[i + 1].easting - arr[i - 1].easting,
                            arr[i + 1].northing - arr[i - 1].northing);
        if (pt3.heading < 0) pt3.heading += glm::twoPI;
        turnLine.append(pt3);
    }

    //last and first point
    pt3 = arr[cnt];
    pt3.heading = atan2(arr[0].easting - arr[cnt - 1].easting,
                        arr[0].northing - arr[cnt - 1].northing);
    if (pt3.heading < 0) pt3.heading += glm::twoPI;
    turnLine.append(pt3);
}

void CBoundaryLines::fixTurnLine(double totalHeadWidth, double spacing)
{
    //count the points from the boundary
    int lineCount = turnLine.count();

    totalHeadWidth *= totalHeadWidth;
    spacing *= spacing;

    //int headCount = mf.bndArr[inTurnNum].bndLine.Count;
    double distance;

    //remove the points too close to boundary
    for (int i = 0; i < fenceLine.count(); i++)
    {
        for (int j = 0; j < lineCount; j++)
        {
            //make sure distance between headland and boundary is not less then width
            distance = glm::distanceSquared(fenceLine[i], turnLine[j]);
            if (distance < (totalHeadWidth * 0.99))
            {
                turnLine.removeAt(j);
                lineCount = turnLine.count();
                j = -1;
            }
        }
    }

    //make sure distance isn't too big between points on Turn
    int bndCount = turnLine.count();
    for (int i = 0; i < bndCount; i++)
    {
        int j = i + 1;
        if (j == bndCount) j = 0;
        distance = glm::distanceSquared(turnLine[i], turnLine[j]);
        if (distance > (spacing * 1.8))
        {
            Vec3 pointB((turnLine[i].easting + turnLine[j].easting) / 2.0,
                        (turnLine[i].northing + turnLine[j].northing) / 2.0,
                        turnLine[i].heading);

            turnLine.insert(j, pointB);
            bndCount = turnLine.count();
            i--;
        }
    }

    //make sure distance isn't too small between points on turnLine
    bndCount = turnLine.count();
    for (int i = 0; i < bndCount - 1; i++)
    {
        distance = glm::distanceSquared(turnLine[i], turnLine[i + 1]);
        if (distance < spacing)
        {
            turnLine.removeAt(i + 1);
            bndCount = turnLine.count();
            i--;
        }
    }

    //make sure headings are correct for calculated points
    if (turnLine.count() > 0)
    {
        calculateTurnHeadings();
    }

}

#if 0
void CBoundaryLines::preCalcBoundaryLines()
{
    int j = bndLine.size() - 1;
    //clear the list, constant is easting, multiple is northing
    calcList.clear();
    Vec2 constantMultiple(0, 0);

    for (int i = 0; i < bndLine.size(); j = i++)
    {
        //check for divide by zero
        if (fabs(bndLine[i].northing - bndLine[j].northing) < 0.00000000001)
        {
            constantMultiple.easting = bndLine[i].easting;
            constantMultiple.northing = 0;
            calcList.append(constantMultiple);
        }
        else
        {
            //determine constant and multiple and add to list
            constantMultiple.easting = bndLine[i].easting - ((bndLine[i].northing * bndLine[j].easting)
                            / (bndLine[j].northing - bndLine[i].northing)) + ((bndLine[i].northing * bndLine[i].easting)
                                / (bndLine[j].northing - bndLine[i].northing));
            constantMultiple.northing = (bndLine[j].easting - bndLine[i].easting) / (bndLine[j].northing - bndLine[i].northing);
            calcList.append(constantMultiple);
        }
    }
}

bool CBoundaryLines::isPointInsideBoundary(Vec3 testPointv3) const
{
    if (calcList.size() < 3) return false;
    int j = bndLine.size() - 1;
    bool oddNodes = false;

    //test against the constant and multiples list the test point
    for (int i = 0; i < bndLine.size(); j = i++)
    {
        if ((bndLine[i].northing < testPointv3.northing && bndLine[j].northing >= testPointv3.northing)
        || (bndLine[j].northing < testPointv3.northing && bndLine[i].northing >= testPointv3.northing))
        {
            oddNodes ^= ((testPointv3.northing * calcList[i].northing) + calcList[i].easting < testPointv3.easting);
        }
    }
    return oddNodes; //true means inside.
}

bool CBoundaryLines::isPointInsideBoundary(Vec2 testPointv2) const
{
    if (calcList.size() < 3) return false;
    int j = bndLine.size() - 1;
    bool oddNodes = false;

    //test against the constant and multiples list the test point
    for (int i = 0; i < bndLine.size(); j = i++)
    {
        if ((bndLine[i].northing < testPointv2.northing && bndLine[j].northing >= testPointv2.northing)
        || (bndLine[j].northing < testPointv2.northing && bndLine[i].northing >= testPointv2.northing))
        {
            oddNodes ^= ((testPointv2.northing * calcList[i].northing) + calcList[i].easting < testPointv2.easting);
        }
    }
    return oddNodes; //true means inside.
}

void CBoundaryLines::drawBoundaryLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, QColor color)
{
    ////draw the perimeter line so far
    if (bndLine.size() < 1) return;


    gl->glLineWidth(2);
    int ptCount = bndLine.size();

    GLHelperOneColor gldraw;

    for (int h = 0; h < ptCount; h++)
        gldraw.append(QVector3D(bndLine[h].easting, bndLine[h].northing, 0));

    gldraw.draw(gl,mvp,color,
                GL_LINES, 2.0);
}

void CBoundaryLines::calculateBoundaryArea()
{
    int ptCount = bndLine.size();
    if (ptCount < 1) return;

    area = 0;         // Accumulates area in the loop
    int j = ptCount - 1;  // The last vertex is the 'previous' one to the first

    for (int i = 0; i < ptCount; j = i++)
    {
        area += (bndLine[j].easting + bndLine[i].easting) * (bndLine[j].northing - bndLine[i].northing);
    }
    area = fabs(area / 2);
}
#endif
