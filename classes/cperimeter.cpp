#include "cperimeter.h"
#include <math.h>
#include <QOpenGLContext>
#include <QOpenGLFunctions_1_1>

CPerimeter::CPerimeter()
{
}

void CPerimeter::drawPerimeterLine(QOpenGLContext *glContext)
{
    QOpenGLFunctions_1_1 *gl = glContext->versionFunctions<QOpenGLFunctions_1_1>();

    ////draw the perimeter line so far
    int ptCount = periPtList.size();
    if (ptCount < 1) return;

    //I'm assuming that the GL context is already set up by the caller, so
    //these calls will just work.
    gl->glLineWidth(2);
    gl->glColor3f(0.2f, 0.98f, 0.0f);
    gl->glBegin(GL_LINE_STRIP);
    for (int h = 0; h < ptCount; h++)
        gl->glVertex2d(periPtList[h].easting, periPtList[h].northing);
    gl->glEnd();

    //the "close the loop" line
    gl->glLineWidth(1);
    gl->glColor3f(0.98f, 0.8f, 0.0f);
    gl->glBegin(GL_LINE_STRIP);
    gl->glVertex2d(periPtList[ptCount-1].easting, periPtList[ptCount-1].northing);
    gl->glVertex2d(periPtList[0].easting, periPtList[0].northing);
    gl->glEnd();

    area = 0; //accumulates area in the loop;

    int j = ptCount -1; //The last vertex is the 'previous' one to the first

    for (int i=0; i<ptCount; j = i++) {
        area += (periPtList[j].easting + periPtList[i].easting) *
                (periPtList[j].northing - periPtList[i].northing);
    }

    area = fabs(area/2);
}
