#ifndef CPERIMETER_H
#define CPERIMETER_H

#include <QVector>
#include <QVector3D>
#include <QOpenGLBuffer>

#include "vec2.h"

class QOpenGLContext;

class CPerimeter
{
private:
   QOpenGLBuffer perimBuffer;
   QOpenGLBuffer perimCloseLoopBuffer;

   int lastListSize = -1;

public:
    //private read only OpenGL gl

    //list of coordinates of perimter line
    QVector<QVector3D> periPtList;
    //QVector<QVector3D> calcList;

    //list of the list of patch data individual triangles for that entire section activity
    //QVector<QVector<QVector3D>> periLineList;

    //button status
    bool isBtnPerimeterOn = false;

    //area variable
    double area = 0.0;

    CPerimeter();

    void drawPerimeterLine(QOpenGLContext *c, const QMatrix4x4 &mvp);

};

#endif // CPERIMETER_H
