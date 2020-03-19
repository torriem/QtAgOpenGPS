#ifndef CGEOFENCE_H
#define CGEOFENCE_H

#include <QVector>
#include "vec3.h"
#include "vec2.h"
#include "cgeofencelines.h"

class CBoundary;
class CTurn;
class CMazeGrid;
class QOpenGLFunctions;
class QMatrix4x4;

class CGeoFence
{

public:
    QVector<CGeoFenceLines> geoFenceArr;
    CGeoFence();
    void findPointsDriveAround(CTurn &turn, const CBoundary &bnd,
                               const CMazeGrid &mazeGrid,
                               double minFieldX, double minFieldY,
                               Vec3 fromPt, double headAB,
                               Vec3 &start, Vec3 &stop);

    bool isPointInsideGeoFences(const CBoundary &bnd, const Vec3 pt);
    bool isPointInsideGeoFences(const CBoundary &bnd, const Vec2 pt);

    void buildGeoFenceLines(CBoundary &bnd);

    void drawGeoFenceLines(const CBoundary &bnd, QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
};

#endif // CGEOFENCE_H
