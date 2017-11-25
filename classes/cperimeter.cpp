#include "cperimeter.h"
#include <math.h>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include "glutils.h"

CPerimeter::CPerimeter()
{
}

void CPerimeter::drawPerimeterLine(QOpenGLContext *glContext, const QMatrix4x4 &mvp)
{
    ////draw the perimeter line so far
    int ptCount = periPtList.size();
    if (ptCount < 1) return;

    if (lastListSize != ptCount) {
        if(perimBuffer.isCreated())
            perimBuffer.destroy();
        if(perimCloseLoopBuffer.isCreated())
            perimCloseLoopBuffer.destroy();
        perimBuffer.create();
        perimBuffer.bind();
        perimBuffer.allocate(periPtList.constData(),ptCount*sizeof(QVector3D));
        perimBuffer.release();

        //close the loop by joining the first and last coordinates into a line
        QVector<QVector3D> closeLoopList;
        closeLoopList.append(periPtList[0]);
        closeLoopList.append(periPtList[ptCount-1]);
        perimCloseLoopBuffer.create();
        perimCloseLoopBuffer.bind();
        perimCloseLoopBuffer.allocate(closeLoopList.constData(),closeLoopList.size()*sizeof(QVector3D));
        perimCloseLoopBuffer.release();

        lastListSize = ptCount;
    }

    QOpenGLFunctions *gl = glContext->functions();

    QColor perimColor = QColor(50,250,0);

    glDrawArraysColor(gl,mvp,
                      GL_LINE_STRIP, perimColor,
                      perimBuffer,GL_FLOAT,
                      ptCount,2.0f);

    perimColor = QColor(250,204,0);
    glDrawArraysColor(gl,mvp,
                      GL_LINE_STRIP, perimColor,
                      perimCloseLoopBuffer,GL_FLOAT,
                      2, 1.0f);

    area = 0; //accumulates area in the loop;

    int j = ptCount -1; //The last vertex is the 'previous' one to the first

    for (int i=0; i<ptCount; j = i++) {
        area += (periPtList[j].x() + periPtList[i].x()) *
                (periPtList[j].y() - periPtList[i].y());
    }

    area = fabs(area/2);
}
