#ifndef CGEOFENCELINES_H
#define CGEOFENCELINES_H

#include <QVector>
#include "vec2.h"
#include "vec3.h"

class QOpenGLFunctions;
class QMatrix4x4;


class CGeoFenceLines
{
public:
    QVector<Vec2> geoFenceLine;
    QVector<Vec2> calcList;


    CGeoFenceLines();
    void reset();
    bool isPointInGeoFenceArea(Vec3 testPointv2);
    bool isPointInGeoFenceArea(Vec2 testPointv2);
    void drawGeoFenceLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
    void fixGeoFenceLine(double totalHeadWidth, QVector<Vec3> curBnd, double spacing);
    void preCalcTurnLines();



};

#endif // CGEOFENCELINES_H
