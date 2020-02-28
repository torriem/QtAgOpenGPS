#include "csection.h"
#include "cvehicle.h"
#include "ctool.h"

#include <math.h>

CSection::CSection()
{
    triangleList = QSharedPointer<TriangleList>( new TriangleList);
}

void CSection::turnSectionOn(const CVehicle &vehicle) {
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
        triangleList->append(QVector3D((vehicle.cosSectionHeading * positionLeft) + vehicle.toolPos.easting,
                                       (vehicle.sinSectionHeading * positionLeft) + vehicle.toolPos.northing, 0));

        //Right side of triangle
        triangleList->append(QVector3D((vehicle.cosSectionHeading * positionRight) + vehicle.toolPos.easting,
                                       (vehicle.sinSectionHeading * positionRight) + vehicle.toolPos.northing, 0));
    }
}

void CSection::turnSectionOff(CVehicle &vehicle, CTool &tool) {
    //qDebug() << "section turned off";
    addPathPoint(vehicle, tool, vehicle.toolPos.northing, vehicle.toolPos.easting, vehicle.cosSectionHeading, vehicle.sinSectionHeading);

    isSectionOn = false;
    numTriangles = 0;

    //save the triangle list in a patch list to add to saving file
    //is this used anymore?
    tool.patchSaveList.append(triangleList);
}


//every time a new fix, a new patch point from last point to this point
//only need prev point on the first points of triangle strip that makes a box (2 triangles)

void CSection::addPathPoint(CVehicle &vehicle, CTool &tool, double northing, double easting, double cosHeading, double sinHeading)
{
    //add two triangles for next step.
    //left side and add the point to List

    QVector3D point((cosHeading * positionLeft) + easting,
                    (sinHeading * positionLeft) + northing,
                    0 );

    triangleList->append(point);

    //Right side add the point to the list

    QVector3D point2((cosHeading * positionRight) + easting,
                     (sinHeading * positionRight) + northing,
                     0 );
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
        //easting = x, northing = y!
        {
            double temp = ((*triangleList)[c].x() * ((*triangleList)[c - 1].y() - (*triangleList)[c - 2].y())) +
                          ((*triangleList)[c - 1].x() * ((*triangleList)[c - 2].y() - (*triangleList)[c].y())) +
                          ((*triangleList)[c - 2].x() * ((*triangleList)[c].y() - (*triangleList)[c - 1].y()));

            temp = fabs(temp / 2.0);
            vehicle.totalSquareMeters += temp;
            vehicle.totalUserSquareMeters += temp;

            //temp = 0;
            temp = ((*triangleList)[c - 1].x() * ((*triangleList)[c - 2].y() - (*triangleList)[c - 3].y())) +
                   ((*triangleList)[c - 2].x() * ((*triangleList)[c - 3].y() - (*triangleList)[c - 1].y())) +
                   ((*triangleList)[c - 3].x() * ((*triangleList)[c - 1].y() - (*triangleList)[c - 2].y()));

            temp = fabs(temp / 2.0);
            vehicle.totalSquareMeters += temp;
            vehicle.totalUserSquareMeters += temp;
        }
    }

    if (numTriangles > 36)
    {
        numTriangles = 0;

        //save the cutoff patch to be saved later
        tool.patchSaveList.append(triangleList);

        triangleList = QSharedPointer<TriangleList>( new TriangleList);
        patchList.append(triangleList);

        //add the points to List, yes its more points, but breaks up patches for culling
        triangleList->append(point);
        triangleList->append(point2);
    }
}
