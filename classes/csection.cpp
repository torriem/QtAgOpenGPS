#include "csection.h"
#include <math.h>

CSection::CSection()
{
    triangleList = QSharedPointer<TriangleList>( new TriangleList);

}

void CSection::turnSectionOn(FormGPS *mf) {
    numTriangles = 0;

    //do not tally square meters on inital point, that would be silly
    if (!isSectionOn)
    {
        //set the section bool to on
        isSectionOn = true;

        //starting a new patch chunk so create a new triangle list
        //and add the previous triangle list to the list of paths

        // start a brand new triangle list
        triangleList = QSharedPointer<TriangleList>( new TriangleList);
        patchList.append(triangleList);

        //left side of triangle
        triangleList->append(Vec2((mf->cosSectionHeading * positionLeft) + mf->toolPos.easting,
                                  (mf->sinSectionHeading * positionLeft) + mf->toolPos.northing));

        //Right side of triangle
        triangleList->append(Vec2((mf->cosSectionHeading * positionRight) + mf->toolPos.easting,
                                  (mf->sinSectionHeading * positionRight) + mf->toolPos.northing));
    }
}

void CSection::turnSectionOff(FormGPS *mf) {
    addPathPoint(mf, mf->toolPos.northing, mf->toolPos.easting, mf->cosSectionHeading, mf->sinSectionHeading);

    isSectionOn = false;
    numTriangles = 0;

    //save the triangle list in a patch list to add to saving file
    mf->patchSaveList.append(triangleList);
}


//every time a new fix, a new patch point from last point to this point
//only need prev point on the first points of triangle strip that makes a box (2 triangles)

void CSection::addPathPoint(FormGPS *mf, double northing, double easting, double cosHeading, double sinHeading)
{
    //add two triangles for next step.
    //left side and add the point to List

    Vec2 point((cosHeading * positionLeft) + easting,
               (sinHeading * positionLeft) + northing);

    triangleList->append(point);

    //Right side add the point to the list

    Vec2 point2((cosHeading * positionRight) + easting,
                (sinHeading * positionRight) + northing);
    triangleList->append(point2);

    //count the triangle pairs
    numTriangles++;

    //quick count
    int c = triangleList->size()-1;

    //when closing a job the triangle patches all are emptied but the section delay keeps going.
    //Prevented by quick check.
    if (c >= 3)
    {
        //calculate area of these 2 new triangles - AbsoluteValue of (Ax(By-Cy) + Bx(Cy-Ay) + Cx(Ay-By)/2)
        {
            double temp = ((*triangleList)[c].easting * ((*triangleList)[c - 1].northing - (*triangleList)[c - 2].northing)) +
                          ((*triangleList)[c - 1].easting * ((*triangleList)[c - 2].northing - (*triangleList)[c].northing)) +
                          ((*triangleList)[c - 2].easting * ((*triangleList)[c].northing - (*triangleList)[c - 1].northing));

            temp = fabs(temp / 2.0);
            mf->totalSquareMeters += temp;
            mf->totalUserSquareMeters += temp;

            //temp = 0;
            temp = ((*triangleList)[c - 1].easting * ((*triangleList)[c - 2].northing - (*triangleList)[c - 3].northing)) +
                   ((*triangleList)[c - 2].easting * ((*triangleList)[c - 3].northing - (*triangleList)[c - 1].northing)) +
                   ((*triangleList)[c - 3].easting * ((*triangleList)[c - 1].northing - (*triangleList)[c - 2].northing));

            temp = fabs(temp / 2.0);
            mf->totalSquareMeters += temp;
            mf->totalUserSquareMeters += temp;
        }
    }

    if (numTriangles > 36)
    {
        numTriangles = 0;

        //save the cutoff patch to be saved later
        mf->patchSaveList.append(triangleList);

        triangleList = QSharedPointer<TriangleList>( new TriangleList);
        patchList.append(triangleList);

        //add the points to List, yes its more points, but breaks up patches for culling
        triangleList->append(point);
        triangleList->append(point2);
    }
}
