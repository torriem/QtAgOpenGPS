#include "cpatches.h"
#include "cfielddata.h"
#include "ctool.h"
#include "aogproperty.h"

CPatches::CPatches() {
   triangleList = QSharedPointer<PatchTriangleList>( new PatchTriangleList);
}

/* torriem: modifications. Passing in left, right points, and color, rather than
 * accessing the CSection objects themselves.
 */
void CPatches::TurnMappingOn(CTool &tool, int j)
{
    QColor color_prop;
    numTriangles = 0;

    //do not tally square meters on inital point, that would be silly
    if (!isDrawing)   {
        //set the section bool to on
        isDrawing = true;

        //starting a new patch chunk so create a new triangle list
        triangleList = QSharedPointer<PatchTriangleList>( new PatchTriangleList);

        patchList.append(triangleList);

        //Add Patch colour
        if (!tool.isMultiColoredSections)
        {
            color_prop = property_setDisplay_colorSectionsDay;
        }
        else
        {
            if (tool.isSectionsNotZones)
                color_prop = tool.secColors[j];
            else
                color_prop = property_setDisplay_colorSectionsDay;
        }

        triangleList->append(QVector3D(color_prop.redF(), color_prop.greenF(), color_prop.blueF()));

        leftPoint = tool.section[currentStartSectionNum].leftPoint;
        rightPoint = tool.section[currentEndSectionNum].rightPoint;

        //left side of triangle
        triangleList->append(leftPoint);

        //Right side of triangle
        triangleList->append(rightPoint);
    }
}

void CPatches::TurnMappingOff(CTool &tool,
                              CFieldData &fd)
{
   AddMappingPoint(tool, fd, 0);

   isDrawing = false;
   numTriangles = 0;

   if (triangleList->count() > 4)
   {
       //save the triangle list in a patch list to add to saving file
       tool.patchSaveList.append(triangleList);
   }
   else
   {
       //torriem: patch strip is too small to keep, so get rid of it
       triangleList->clear();
       if (patchList.count() > 0) patchList.removeAt(patchList.count() - 1);
   }
}

void CPatches::AddMappingPoint(CTool &tool,
                               CFieldData &fd,
                               int j)
{
   Vec2 vleftPoint = tool.section[currentStartSectionNum].leftPoint;
   Vec2 vrightPoint = tool.section[currentEndSectionNum].rightPoint;
   QVector3D leftPoint(vleftPoint.easting,vleftPoint.northing,0);
   QVector3D rightPoint(vrightPoint.easting,vrightPoint.northing,0);
   QVector3D color;
   QColor color_prop;


   //add two triangles for next step.
   //left side

   //add the point to List
   triangleList->append(leftPoint);

   //Right side
   triangleList->append(rightPoint);

   //count the triangle pairs
   numTriangles++;

   //quick count
   int c = triangleList->count() - 1;

   //when closing a job the triangle patches all are emptied but the section delay keeps going.
   //Prevented by quick check. 4 points plus colour

   //torriem: easting is .x(), northing is .y() when using QVector3D
   double temp = fabs(((*triangleList)[c].x() * ((*triangleList)[c - 1].y() - (*triangleList)[c - 2].y()))
          + ((*triangleList)[c - 1].x() * ((*triangleList)[c - 2].y() - (*triangleList)[c].y()))
          + ((*triangleList)[c - 2].x() * ((*triangleList)[c].y() - (*triangleList)[c - 1].y())));

   temp += fabs(((*triangleList)[c - 1].x() * ((*triangleList)[c - 2].y() - (*triangleList)[c - 3].y()))
                + ((*triangleList)[c - 2].x() * ((*triangleList)[c - 3].y() - (*triangleList)[c - 1].y()))
                + ((*triangleList)[c - 3].x() * ((*triangleList)[c - 1].y() - (*triangleList)[c - 2].y())));

   temp *= 0.5;

   //TODO,
   fd.workedAreaTotal += temp;
   fd.workedAreaTotalUser += temp;

   if (numTriangles > 61)
   {
       numTriangles = 0;

       //save the cutoff patch to be saved later
       tool.patchSaveList.append(triangleList);

       triangleList = QSharedPointer<PatchTriangleList>(new PatchTriangleList);

       patchList.append(triangleList);

       //Add Patch colour
       if (!tool.isMultiColoredSections)
           color_prop = property_setDisplay_colorSectionsDay;
       else
           color_prop = tool.secColors[j];

       color = QVector3D(color_prop.redF(), color_prop.greenF(), color_prop.blueF());
       //add the points to List, yes its more points, but breaks up patches for culling
       triangleList->append(color);

       triangleList->append(leftPoint);
       triangleList->append(rightPoint);
   }
}
