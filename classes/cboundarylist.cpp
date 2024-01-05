#include "cboundarylist.h"
#include "glm.h"

CBoundaryList::CBoundaryList()
{
    area = 0;
    isDriveThru = false;
}

void CBoundaryList::CalculateFenceLineHeadings()
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

void CBoundaryList::FixFenceLine(int bndNum)
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
        distance = glm::Distance(fenceLine[i], fenceLine[j]);
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
        distance = glm::Distance(fenceLine[i], fenceLine[j]);
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
        distance = glm::Distance(fenceLine[i], fenceLine[i + 1]);
        if (distance < spacing)
        {
            fenceLine.removeAt(i + 1);
            bndCount = fenceLine.count();
            i--;
        }
    }

    //make sure headings are correct for calculated points
    CalculateFenceLineHeadings();

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

void CBoundaryList::ReverseWinding()
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
bool CBoundaryList::CalculateFenceArea(int idx)
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
        ReverseWinding();
    }

    return isClockwise;
}

