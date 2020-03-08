#ifndef CHEADLINES_H
#define CHEADLINES_H

#include <QVector>
#include "vec3.h"
#include "vec2.h"

class QOpenGLFunctions;
class QMatrix4x4;

class CHeadLines
{
public:
    QVector<Vec3> hdLine;
    QVector<Vec2> calcList;
    QVector<bool> isDrawList;

    CHeadLines();
    void resetHead();
    bool isPointInHeadArea(Vec3 testPointv2);
    bool isPointInHeadArea(Vec2 testPointv2);
    void drawHeadLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, int linewidth);
    void drawHeadLineBackBuffer(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);

    void preCalcHeadLines();
};

#endif // CHEADLINES_H
