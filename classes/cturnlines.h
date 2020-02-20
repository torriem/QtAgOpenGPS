#ifndef CTURNLINES_H
#define CTURNLINES_H

#include <QVector>
#include "vec3.h"
#include "vec2.h"

class QOpenGLFunctions;
class QMatrix4x4;

class CTurnLines
{
public:
    //list of coordinates of boundary line
    QVector<Vec3> turnLine;

    //the list of constants and multiples of the boundary
    QVector<Vec2> calcList;

    CTurnLines();

    void calculateTurnHeadings();
    void resetTurn();
    void fixTurnLine(double totalHeadWidth, const QVector<Vec3> &curBnd, double spacing);
    void preCalcTurnLines();
    bool isPointInTurnWorkArea(Vec3 testPointv3);
    bool isPointInTurnWorkArea(Vec2 testPointv2);
    void drawTurnLine();
    void drawTurnLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
};

#endif // CTURNLINES_H
