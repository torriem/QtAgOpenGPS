#include "csection.h"
#include "cvehicle.h"
#include "ctool.h"
#include "aogsettings.h"

#include <math.h>

CSection::CSection(QObject *parent) : QObject(parent)
{
    triangleList = QSharedPointer<TriangleList>( new TriangleList);
}

void CSection::turnMappingOn() {
    USE_SETTINGS;
    numTriangles = 0;

    //do not tally square meters on inital point, that would be silly
    if (!isMappingOn)
    {
        qDebug() << "turn section mapping on";
        //set the section bool to on
        isMappingOn = true;

        //starting a new patch chunk so create a new triangle list
        //and add the previous triangle list to the list of paths
        // start a brand new triangle list
        triangleList = QSharedPointer<TriangleList>( new TriangleList);
        patchList.append(triangleList);

        //vec3 colur = new vec3(mf.sectionColorDay.R, mf.sectionColorDay.G, mf.sectionColorDay.B);
        QVector3D colur = parseColorVector(SETTINGS_DISPLAY_SECTIONSCOLORDAY);
        triangleList->append(colur);


        //left side of triangle
        triangleList->append(QVector3D(leftPoint.easting, leftPoint.northing,0));

        //Right side of triangle
        triangleList->append(QVector3D(rightPoint.easting, rightPoint.northing,0));
    }
}

void CSection::turnMappingOff(CTool &tool) {
    //qDebug() << "section turned off";
    addMappingPoint(tool);

    isMappingOn = false;
    numTriangles = 0;
    if (triangleList->count() > 4)
    {
        //save the triangle list in a patch list to add to saving file
        tool.patchSaveList.append(triangleList);
    }
    else
    {
        triangleList.clear();
        if (patchList.count() > 0) patchList.removeAt(patchList.count() - 1);
    }
}


//every time a new fix, a new patch point from last point to this point
//only need prev point on the first points of triangle strip that makes a box (2 triangles)

void CSection::addMappingPoint(CTool &tool)
{
    USE_SETTINGS;
    //add two triangles for next step.
    //left side and add the point to List

    QVector3D point(leftPoint.easting, leftPoint.northing, 0);

    //add the point to the list
    triangleList->append(point);

    //Right side add the point to the list

    QVector3D point2(rightPoint.easting, rightPoint.northing, 0);

    //add the point to the list
    triangleList->append(point2);

    //count the triangle pairs
    numTriangles++;

    //quick count
    int c = triangleList->size()-1;

    //when closing a job the triangle patches all are emptied but the section delay keeps going.
    //Prevented by quick check. 4 points plus colour (which we aren't implementing at this yet. TODO
    if (c >= 5) //would be 5 if using colur thing
    {
        //calculate area of these 2 new triangles - AbsoluteValue of (Ax(By-Cy) + Bx(Cy-Ay) + Cx(Ay-By)/2)
        //easting = x, northing = y!
        {
            double temp = ((*triangleList)[c].x() * ((*triangleList)[c - 1].y() - (*triangleList)[c - 2].y())) +
                          ((*triangleList)[c - 1].x() * ((*triangleList)[c - 2].y() - (*triangleList)[c].y())) +
                          ((*triangleList)[c - 2].x() * ((*triangleList)[c].y() - (*triangleList)[c - 1].y()));

            temp = fabs(temp / 2.0);
            emit addToTotalArea(temp);
            emit addToUserArea(temp);

            //temp = 0;
            temp = ((*triangleList)[c - 1].x() * ((*triangleList)[c - 2].y() - (*triangleList)[c - 3].y())) +
                   ((*triangleList)[c - 2].x() * ((*triangleList)[c - 3].y() - (*triangleList)[c - 1].y())) +
                   ((*triangleList)[c - 3].x() * ((*triangleList)[c - 1].y() - (*triangleList)[c - 2].y()));

            temp = fabs(temp / 2.0);
            emit addToTotalArea(temp);
            emit addToUserArea(temp);
        }
    }

    if (numTriangles > 36)
    {
        numTriangles = 0;

        //save the cutoff patch to be saved later
        tool.patchSaveList.append(triangleList);

        triangleList = QSharedPointer<TriangleList>( new TriangleList);
        patchList.append(triangleList);

        //add patch color
        QVector3D colur = parseColorVector(SETTINGS_DISPLAY_SECTIONSCOLORDAY);
        triangleList->append(colur);

        //add the points to List, yes its more points, but breaks up patches for culling
        triangleList->append(point);
        triangleList->append(point2);
    }
}
