#include "cboundarylines.h"
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <math.h>
#include "glm.h"
#include "glutils.h"


CBoundaryLines::CBoundaryLines()
{
    area = 0;
    isSet = false;
    isDriveAround = false;
    isDriveThru = false;
}

void CBoundaryLines::calculateBoundaryHeadings()
{
    //to calc heading based on next and previous points to give an average heading.
    int cnt = bndLine.size();
    QVector<Vec3> arr = bndLine; //make copy of list
    cnt--;
    bndLine.clear();

    //first point needs last, first, second points
    Vec3 pt3 = arr[0];
    pt3.heading = atan2(arr[1].easting - arr[cnt].easting, arr[1].northing - arr[cnt].northing);
    if (pt3.heading < 0) pt3.heading += glm::twoPI;
    bndLine.append(pt3);

    //middle points
    for (int i = 1; i < cnt; i++)
    {
        pt3 = arr[i];
        pt3.heading = atan2(arr[i + 1].easting - arr[i - 1].easting, arr[i + 1].northing - arr[i - 1].northing);
        if (pt3.heading < 0) pt3.heading += glm::twoPI;
        bndLine.append(pt3);
    }

    //last and first point
    pt3 = arr[cnt];
    pt3.heading = atan2(arr[0].easting - arr[cnt - 1].easting, arr[0].northing - arr[cnt - 1].northing);
    if (pt3.heading < 0) pt3.heading += glm::twoPI;
    bndLine.append(pt3);
}

void CBoundaryLines::fixBoundaryLine(int bndNum, double spacing)
{
    //boundary point spacing based on eq width
    spacing *= 0.25;

    if (spacing < 1) spacing = 1;
    if (spacing > 3) spacing = 3;

    //first find out which side is inside the boundary
    double oneSide = glm::PIBy2;
    Vec3 point(bndLine[2].easting - (sin(oneSide + bndLine[2].heading) * 2.0),
    bndLine[2].northing - (cos(oneSide + bndLine[2].heading) * 2.0), 0.0);

    //make sure boundaries are wound correctly
    if (bndNum == 0)
    {
        //outside an outer boundary means its wound clockwise
        if (!isPointInsideBoundary(point)) reverseWinding();
    }
    else
    {
        //inside an inner boundary means its wound clockwise
        if (isPointInsideBoundary(point)) reverseWinding();
        spacing *= 0.66;
    }

    //make sure distance isn't too small between points on headland
    int bndCount = bndLine.size();
    double distance;
    for (int i = 0; i < bndCount - 1; i++)
    {
        distance = glm::distance(bndLine[i], bndLine[i + 1]);
        if (distance < spacing)
        {
            bndLine.removeAt(i + 1);
            bndCount = bndLine.size();
            i--;
        }
    }

    //make sure distance isn't too big between points on boundary
    bndCount = bndLine.size();
    spacing *= 1.33;

    for (int i = 0; i < bndCount; i++)
    {
        int j = i + 1;

        if (j == bndCount) j = 0;
        distance = glm::distance(bndLine[i], bndLine[j]);
        if (distance > spacing)
        {
            Vec3 pointB((bndLine[i].easting + bndLine[j].easting) / 2.0,
                (bndLine[i].northing + bndLine[j].northing) / 2.0, bndLine[i].heading);

            bndLine.insert(j, pointB);
            bndCount = bndLine.size();
            i--;
        }
    }

    //make sure distance isn't too big between points on boundary
    bndCount = bndLine.size();
    spacing *= 1.33;

    for (int i = 0; i < bndCount; i++)
    {
        int j = i + 1;

        if (j == bndCount) j = 0;
        distance = glm::distance(bndLine[i], bndLine[j]);
        if (distance > spacing)
        {
            Vec3 pointB((bndLine[i].easting + bndLine[j].easting) / 2.0,
                (bndLine[i].northing + bndLine[j].northing) / 2.0, bndLine[i].heading);

            bndLine.insert(j, pointB);
            bndCount = bndLine.size();
            i--;
        }
    }

    //make sure headings are correct for calculated points
    calculateBoundaryHeadings();

}

void CBoundaryLines::reverseWinding()
{
    //reverse the boundary
    int cnt = bndLine.size();
    QVector<Vec3> arr = bndLine;
    cnt--;
    bndLine.clear();
    for (int i = cnt; i >= 0; i--)
    {
        arr[i].heading -= M_PI;
        if (arr[i].heading < 0) arr[i].heading += glm::twoPI;
        bndLine.append(arr[i]);
    }
}

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

bool CBoundaryLines::isPointInsideBoundary(Vec3 testPointv3)
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

bool CBoundaryLines::isPointInsideBoundary(Vec2 testPointv2)
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

void CBoundaryLines::drawBoundaryLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    ////draw the perimeter line so far
    if (bndLine.size() < 1) return;


    gl->glLineWidth(2);
    int ptCount = bndLine.size();

    GLHelperOneColor gldraw;

    for (int h = 0; h < ptCount; h++)
        gldraw.append(QVector3D(bndLine[h].easting, bndLine[h].northing, 0));

    gldraw.draw(gl,mvp,QColor::fromRgbF(1,1,1),
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
