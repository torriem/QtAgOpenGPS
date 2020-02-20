#ifndef CTRAM_H
#define CTRAM_H

#include <QVector>
#include <QMatrix4x4>
#include "vec2.h"
#include "vec3.h"

class CTool;
class CBoundary;
class QOpenGLFunctions;

class CTram
{
private:
    CTool *tool;
    CBoundary *bnd;

public:
    CTram(CTool *tool, CBoundary *bnd);

    //the list of constants and multiples of the boundary
    QVector<Vec2> calcList;

    //the outer ring of boundary tram - also used for clipping
    QVector<Vec3> outArr;

    //the triangle strip of the outer tram highlight
    QVector<Vec2> tramBndArr;

    //tram settings
    double wheelTrack;
    double tramWidth, abOffset;
    double halfWheelTrack;
    int passes;

    // 0 off, 1 All, 2, Lines, 3 Outer
    int displayMode;

    void drawTramBnd(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
    void buildTramBnd();
    void createBndTramRef();
    void createOuterTram();
    bool isPointInTramBndArea(Vec2 testPointv2);
    void preCalcTurnLines();
};

#endif // CTRAM_H
