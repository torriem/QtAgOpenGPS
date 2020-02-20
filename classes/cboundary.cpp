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
    boundarySelected = 0;
}


void CBoundary::findClosestBoundaryPoint(Vec2 fromPt, double headAB)
{
    //heading is based on ABLine, average Curve, and going same direction as AB or not
    //Draw a bounding box to determine if points are in it
    boxA.easting = fromPt.easting + (sin(headAB + glm::PIBy2) * (scanWidth - 2));
    boxA.northing = fromPt.northing + (cos(headAB + glm::PIBy2) * (scanWidth - 2));

    boxB.easting = fromPt.easting + (sin(headAB + glm::PIBy2) * scanWidth);
    boxB.northing = fromPt.northing + (cos(headAB + glm::PIBy2) * scanWidth);

    boxC.easting = boxB.easting + (sin(headAB) * boxLength);
    boxC.northing = boxB.northing + (cos(headAB) * boxLength);

    boxD.easting = boxA.easting + (sin(headAB) * boxLength);
    boxD.northing = boxA.northing + (cos(headAB) * boxLength);

    int ptCount;

    //determine if point is inside bounding box
    bndClosestList.clear();
    Vec4 inBox;
    for (int i = 0; i < bndArr.size(); i++)
    {
        //skip the drive thru
        if (bndArr[i].isDriveThru) continue;

        ptCount = bndArr[i].bndLine.size();
        for (int p = 0; p < ptCount; p++)
        {
            if ((((boxB.easting - boxA.easting) * (bndArr[i].bndLine[p].northing - boxA.northing))
                    - ((boxB.northing - boxA.northing) * (bndArr[i].bndLine[p].easting - boxA.easting))) < 0) { continue; }

            if ((((boxD.easting - boxC.easting) * (bndArr[i].bndLine[p].northing - boxC.northing))
                    - ((boxD.northing - boxC.northing) * (bndArr[i].bndLine[p].easting - boxC.easting))) < 0) { continue; }

            if ((((boxC.easting - boxB.easting) * (bndArr[i].bndLine[p].northing - boxB.northing))
                    - ((boxC.northing - boxB.northing) * (bndArr[i].bndLine[p].easting - boxB.easting))) < 0) { continue; }

            if ((((boxA.easting - boxD.easting) * (bndArr[i].bndLine[p].northing - boxD.northing))
                    - ((boxA.northing - boxD.northing) * (bndArr[i].bndLine[p].easting - boxD.easting))) < 0) { continue; }

            //it's in the box, so add to list
            inBox.easting = bndArr[i].bndLine[p].easting;
            inBox.northing = bndArr[i].bndLine[p].northing;
            inBox.heading = bndArr[i].bndLine[p].heading;
            inBox.index = i;

            //which boundary/headland is it from
            bndClosestList.append(inBox);
        }
    }

    //which of the points is closest
    closestBoundaryPt.easting = -20000; closestBoundaryPt.northing = -20000;
    ptCount = bndClosestList.size();
    if (ptCount != 0)
    {
        //determine closest point
        double minDistance = 9999999;
        for (int i = 0; i < ptCount; i++)
        {
            double dist = ((fromPt.easting - bndClosestList[i].easting) * (fromPt.easting - bndClosestList[i].easting))
                            + ((fromPt.northing - bndClosestList[i].northing) * (fromPt.northing - bndClosestList[i].northing));
            if (minDistance >= dist)
            {
                minDistance = dist;

                closestBoundaryPt.easting = bndClosestList[i].easting;
                closestBoundaryPt.northing = bndClosestList[i].northing;
                closestBoundaryPt.heading = bndClosestList[i].heading;
                closestBoundaryNum = bndClosestList[i].index;
            }
        }
        if (closestBoundaryPt.heading < 0) closestBoundaryPt.heading += glm::twoPI;
    }
}

void CBoundary::resetBoundaries()
{
    bndArr.clear();
}

void CBoundary::drawBoundaryLines(const CVehicle &v, QOpenGLFunctions *gl,
                                 const QMatrix4x4 &mvp)
{
    //draw the boundaries
    QColor color;
    color.setRgbF(0.95f, 0.5f, 0.250f);

    for (int i = 0; i < bndArr.size(); i++)
    {
        bndArr[i].drawBoundaryLine(gl,mvp,color);
    }

    if (bndBeingMadePts.size() > 0)
    {
        //the boundary so far
        Vec3 pivot = v.pivotAxlePos;
        gl->glLineWidth(1);

        GLHelperColors gldraw;

        ColorVertex cv;

        cv.color = QVector4D(0.825f, 0.22f, 0.90f, 1.0f);

        for (int h = 0; h < bndBeingMadePts.size(); h++) {
            cv.vertex = QVector3D(bndBeingMadePts[h].easting, bndBeingMadePts[h].northing, 0);
            gldraw.append(cv);
        }

        cv.color = QVector4D(0.295f, 0.972f, 0.290f, 1.0f);
        cv.vertex = QVector3D(bndBeingMadePts[0].easting, bndBeingMadePts[0].northing, 0);
        gldraw.append(cv);

        gldraw.draw(gl,mvp, GL_LINE_STRIP, 1.0f);

        //line from last point to pivot marker

        GLHelperOneColor gldraw1;

        //GL.Enable(EnableCap.LineStipple);
        //GL.LineStipple(1, 0x0700);
        //TODO: implement stippled line with shader
        if (isDrawRightSide)
        {
            gldraw1.append(QVector3D(bndBeingMadePts[0].easting, bndBeingMadePts[0].northing, 0));
            gldraw1.append(QVector3D(pivot.easting + (sin(pivot.heading - glm::PIBy2) *
                                                      -createBndOffset),
                                     pivot.northing + (cos(pivot.heading - glm::PIBy2) *
                                                       -createBndOffset), 0));
            gldraw1.append(QVector3D(bndBeingMadePts[bndBeingMadePts.size() - 1].easting, bndBeingMadePts[bndBeingMadePts.size() - 1].northing, 0));
        }
        else
        {
            gldraw1.append(QVector3D(bndBeingMadePts[0].easting, bndBeingMadePts[0].northing, 0));
            gldraw1.append(QVector3D(pivot.easting + (sin(pivot.heading - glm::PIBy2) *
                                                      createBndOffset),
                                     pivot.northing + (cos(pivot.heading - glm::PIBy2) *
                                                       createBndOffset), 0));
            gldraw1.append(QVector3D(bndBeingMadePts[bndBeingMadePts.size() - 1].easting, bndBeingMadePts[bndBeingMadePts.size() - 1].northing, 0));
        }

        gldraw1.draw(gl,mvp,QColor::fromRgbF(0.825f, 0.842f, 0.0f),
                     GL_LINE_STRIP,1.0f);

        //boundary points

        gldraw1.clear();
        for (int h = 0; h < bndBeingMadePts.size(); h++)
            gldraw1.append(QVector3D(bndBeingMadePts[h].easting, bndBeingMadePts[h].northing, 0));

        gldraw1.draw(gl,mvp,QColor::fromRgbF(0.0f, 0.95f, 0.95f),
                     GL_POINTS, 6.0f);
    }

}

/*
//draw a blue line in the back buffer for section control over boundary line
void CBoundary::drawBoundaryLineOnBackBuffer(QOpenGLFunctions *gl,
                                             const QMatrix4x4 &mvp)
{
    if (ptList.size() > 1) {
        if (!backBufferCurrent) {
            if (ptListBackBuffer.isCreated())
                ptListBackBuffer.destroy();
            ptListBackBuffer.create();
            ptListBackBuffer.bind();
	    //Here we copy the vector data rather than using it directly
	    //because we need to close the loop by adding the first point
	    //at the end
            ptListBackBuffer.allocate(ptList.size() + 1);
            ptListBackBuffer.write(0,ptList.constData(),ptList.size() * sizeof(QVector3D));
            //the "close the loop" line
            ptListBackBuffer.write(ptList.size() * sizeof(QVector3D),&(ptList[0]), sizeof(QVector3D));
            ptListBackBuffer.release();
	    //TODO: convert this to use GL_LINE_LOOP instead of GL_LINE_STRIP
	    //that we we don't need to do all this copying and replicate
	    //the first point at the end.
        }

        QColor color = QColor::fromRgbF(0.0f, 0.99f, 0.0f);

        ////draw the perimeter line so far
        glDrawArraysColor(gl, mvp,
                          GL_LINE_STRIP, color,
                          ptListBackBuffer, GL_FLOAT,
                          ptList.size() + 1, 4.0f);
    }
}
*/
