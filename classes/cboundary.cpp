#include "cboundary.h"
#include "cnmea.h"
#include "glm.h"
#include "cvehicle.h"
#include <math.h>
#include <QLocale>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include "glutils.h"

CBoundary::CBoundary()
{

}

void CBoundary::findClosestBoundaryPoint(CVehicle *vehicle, Vec2 fromPt)
{
    boxA.easting = fromPt.easting - (sin(vehicle->fixHeading + PIBy2) *  vehicle->toolWidth * 0.5);
    boxA.northing = fromPt.northing - (cos(vehicle->fixHeading + PIBy2)  * vehicle->toolWidth * 0.5);

    boxB.easting = fromPt.easting + (sin(vehicle->fixHeading + PIBy2) *  vehicle->toolWidth * 0.5);
    boxB.northing = fromPt.northing + (cos(vehicle->fixHeading + PIBy2)  * vehicle->toolWidth * 0.5);

    boxC.easting = boxB.easting + (sin(vehicle->fixHeading) * 2000.0);
    boxC.northing = boxB.northing + (cos(vehicle->fixHeading) * 2000.0);

    boxD.easting = boxA.easting + (sin(vehicle->fixHeading) * 2000.0);
    boxD.northing = boxA.northing + (cos(vehicle->fixHeading) * 2000.0);

    //determine if point is inside bounding box
    bdList.clear();
    int ptCount = ptList.size();
    for (int p = 0; p < ptCount; p++) {
        if ((((boxB.easting - boxA.easting) * (ptList[p].y() - boxA.northing))
             - ((boxB.northing - boxA.northing) * (ptList[p].x() - boxA.easting))) < 0) {
            continue;
        }
        if ((((boxD.easting - boxC.easting) * (ptList[p].y() - boxC.northing))
             - ((boxD.northing - boxC.northing) * (ptList[p].x() - boxC.easting))) < 0) {
            continue;
        }
        if ((((boxC.easting - boxB.easting) * (ptList[p].y() - boxB.northing))
             - ((boxC.northing - boxB.northing) * (ptList[p].x() - boxB.easting))) < 0) {
            continue;
        }
        if ((((boxA.easting - boxD.easting) * (ptList[p].y() - boxD.northing))
             - ((boxA.northing - boxD.northing) * (ptList[p].x() - boxD.easting))) < 0) {
            continue;
        }

        //it's in the box, so add to list
        closestBoundaryPt.easting = ptList[p].x();
        closestBoundaryPt.northing = ptList[p].y();
        bdList.append(closestBoundaryPt);
    }

    //which of the points is closest
    closestBoundaryPt.easting = -1; closestBoundaryPt.northing = -1;
    ptCount = bdList.count();
    if (ptCount == 0) {
        return;
    } else {
        //determine closest point
        double minDistance = 9999999;
        for (int i = 0; i < ptCount; i++) {
            double dist = ((fromPt.easting - bdList[i].easting) * (fromPt.easting - bdList[i].easting))
                    + ((fromPt.northing - bdList[i].northing) * (fromPt.northing - bdList[i].northing));
            if (minDistance >= dist) {
                minDistance = dist;
                closestBoundaryPt = bdList[i];
            }
        }
    }
}

void CBoundary::resetBoundary()
{
    calcList.clear();
    ptList.clear();
    area = 0;
    areaAcre = 0;
    areaHectare = 0;

    isDrawRightSide = true;
    isSet = false;
    isOkToAddPoints = false;

    bufferCurrent = false;
}

void CBoundary::preCalcBoundaryLines()
{
    int j = ptList.size() - 1;
    //clear the list, constant is easting, multiple is northing
    calcList.clear();
    Vec2 constantMultiple(0, 0);

    for (int i = 0; i < ptList.size(); j = i++)
    {
        //check for divide by zero
        if (fabs(ptList[i].y() - ptList[j].y()) < 0.00000000001)
        {
            constantMultiple.easting = ptList[i].x();
            constantMultiple.northing = 0;
        }
        else
        {
            //determine constant and multiple and add to list
            constantMultiple.easting = ptList[i].x() - (ptList[i].y() * ptList[j].x())
                    / (ptList[j].y() - ptList[i].y()) + (ptList[i].y() * ptList[i].x())
                    / (ptList[j].y() - ptList[i].y());
            constantMultiple.northing = (ptList[j].x() - ptList[i].x()) / (ptList[j].y() - ptList[i].y());
            calcList.append(constantMultiple);
        }
    }

    areaHectare = area * 0.0001;
    areaAcre = area * 0.000247105;
}

bool CBoundary::isPrePointInPolygon(Vec2 testPoint)
{
    if (calcList.size() < 10) return false;
    int j = ptList.size() - 1;
    bool oddNodes = false;

    //test against the constant and multiples list the test point
    for (int i = 0; i < ptList.size(); j = i++)
    {
        if ( ((ptList[i].y() < testPoint.northing) && (ptList[j].y() >= testPoint.northing)) ||
             ((ptList[j].y() < testPoint.northing) && (ptList[i].y() >= testPoint.northing)) )
        {
            oddNodes ^= (testPoint.northing * calcList[i].northing + calcList[i].easting < testPoint.easting);
        }
    }
    return oddNodes; //true means inside.
}

void CBoundary::drawBoundaryLine(QOpenGLContext *glContext,
                                 const QMatrix4x4 &modelview,
                                 const QMatrix4x4 &projection)
{
    if (!bufferCurrent) {
        ColorVertex bline[ptList.size() + 1];

        for(int i=0; i < ptList.size(); i++) {
            bline[i].vertex = ptList[i];
            bline[i].color = QVector4D(0.98f, 0.2f, 0.60f, 1.0f);
        }
        //the "close the loop" line
        //not sure how this will render, honestly.  Might end up
        //being a blended color
        bline[ptList.size()].vertex = ptList[0];
        bline[ptList.size()].color = QVector4D(0.9f, 0.32f, 0.70f,1.0f);

        if (ptListBuffer.isCreated())
            ptListBuffer.destroy();
        ptListBuffer.create();
        ptListBuffer.bind();
        ptListBuffer.allocate(bline, (ptList.size() + 1) * sizeof(ColorVertex));
        ptListBuffer.release();

        bufferCurrent = true;
    }

    QOpenGLFunctions *gl=glContext->functions();

    ////draw the perimeter line so far
    glDrawArraysColors(gl, projection * modelview,
                       GL_LINE_STRIP, ptListBuffer,
                       GL_FLOAT, ptList.size() + 1,
                       2.0f);
}

//draw a blue line in the back buffer for section control over boundary line
void CBoundary::drawBoundaryLineOnBackBuffer(QOpenGLContext *glContext,
                                             const QMatrix4x4 &modelview,
                                             const QMatrix4x4 &projection)
{
    if (!backBufferCurrent) {
        if (ptListBackBuffer.isCreated())
            ptListBackBuffer.destroy();
        ptListBackBuffer.create();
        ptListBackBuffer.bind();
        ptListBackBuffer.allocate(ptList.size() + 1);
        ptListBackBuffer.write(0,ptList.constData(),ptList.size() * sizeof(QVector3D));

        //the "close the loop" line
        ptListBackBuffer.write(ptList.size() * sizeof(QVector3D),&(ptList[0]), sizeof(QVector3D));
        ptListBackBuffer.release();
    }

    QOpenGLFunctions *gl=glContext->functions();
    QColor color = QColor::fromRgbF(0.0f, 0.99f, 0.0f);

    ////draw the perimeter line so far
    glDrawArraysColor(gl, projection*modelview,
                      GL_LINE_STRIP, color,
                      ptListBackBuffer, GL_FLOAT,
                      ptList.size() + 1, 4.0f);
}

void CBoundary::calculateBoundaryArea()
{
    int ptCount = ptList.size();
    if (ptCount < 1) return;

    area = 0;         // Accumulates area in the loop
    int j = ptCount - 1;  // The last vertex is the 'previous' one to the first

    for (int i = 0; i < ptCount; j = i++)
    {
        area += (ptList[j].x() + ptList[i].x()) * (ptList[j].y() - ptList[i].y());
    }
    area = fabs(area / 2);
}

void CBoundary::addPoint(Vec2 point)
{
    ptList.append(QVector3D(point.easting, point.northing, 0));
    bufferCurrent = false;
    backBufferCurrent = false;
}
