#include "cpatches.h"
#include "cfielddata.h"
#include "aogproperty.h"

CPatches::CPatches() {
   // triangleList = QSharedPointer<TriangleList>( new TriangleList);
}

/* torriem: modifications. Passing in left, right points, and color, rather than
 * accessing the CSection objects themselves.
 */
void CPatches::turnMappingOn(QVector3D newLeftPoint, QVector3D newRightPoint, QVector3D color)
{
   //bool isMultiColoredSections = property_setColor_isMultiColorSections;


   //do not tally square meters on inital point, that would be silly
   if (!isDrawing)
   {
       //set the section bool to on
       isDrawing = true;

       //starting a new patch chunk so create a new triangle list
       triangleList = QSharedPointer<PatchTriangleList>( new PatchTriangleList);

       patchList.append(triangleList);

       //torriem: multicolored sections dealt with by the caller who
       //specifies what color to use here.
       /*
       if (!isMultiColoredSections)
       {
           triangleList.Add(new vec3(mf.sectionColorDay.R, mf.sectionColorDay.G, mf.sectionColorDay.B));
       }
       else
       {
           if (mf.tool.isSectionsNotZones)
               triangleList.Add(new vec3(mf.tool.secColors[j].R, mf.tool.secColors[j].G, mf.tool.secColors[j].B));
           else
               triangleList.Add(new vec3(mf.sectionColorDay.R, mf.sectionColorDay.G, mf.sectionColorDay.B));
       }
       */

       //first Vector is actually a color, determined by the caller
       triangleList->append(color);


       leftPoint = newLeftPoint;
       rightPoint = newRightPoint;

       //left side of triangle
       triangleList->append(leftPoint);

       //Right side of triangle
       triangleList->append(rightPoint);
   }
}

void CPatches::turnMappingOff(QVector<QSharedPointer<PatchTriangleList> > &patchSaveList,
                              QVector3D newLeftPoint,
                              QVector3D newRightPoint,
                              QVector3D color,
                              CFieldData &fd)
{
   addMappingPoint(patchSaveList,newLeftPoint,newRightPoint,color,fd);

   isDrawing = false;
   numTriangles = 0;

   if (triangleList->count() > 4)
   {
       //save the triangle list in a patch list to add to saving file
       patchSaveList.append(triangleList);
   }
   else
   {
       //torriem: patch strip is too small to keep, so get rid of it
       triangleList->clear();
       if (patchList.count() > 0) patchList.removeAt(patchList.count() - 1);
   }
}

void CPatches::addMappingPoint(QVector<QSharedPointer<PatchTriangleList> > &patchSaveList,
                               QVector3D newLeftPoint,
                               QVector3D newRightPoint,
                               QVector3D color,
                               CFieldData &fd)
{
   leftPoint = newLeftPoint;
   rightPoint = newRightPoint;

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
       patchSaveList.append(triangleList);

       triangleList = QSharedPointer<PatchTriangleList>(new PatchTriangleList);

       patchList.append(triangleList);

       //Add Patch colour
       triangleList->append(color);
       /* put this in the caller
       if (!mf.tool.isMultiColoredSections)
           triangleList.Add(new vec3(mf.sectionColorDay.R, mf.sectionColorDay.G, mf.sectionColorDay.B));
       else
           triangleList.Add(new vec3(mf.tool.secColors[j].R, mf.tool.secColors[j].G, mf.tool.secColors[j].B));

       */
       //add the points to List, yes its more points, but breaks up patches for culling

       triangleList->append(leftPoint);
       triangleList->append(rightPoint);
   }
}
