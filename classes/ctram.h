#ifndef CTRAM_H
#define CTRAM_H

#include <QVector>
#include <QMatrix4x4>
#include "vec2.h"
#include "vec3.h"

class CBoundary;
class QOpenGLFunctions;
class CCamera;

class CTram
{
private:

public:

    //the list of constants and multiples of the boundary
    QVector<Vec2> calcList;

    //the triangle strip of the outer tram highlight
    QVector<Vec2> tramBndOuterArr;

    QVector<Vec2> tramBndInnerArr;

    //tram settings
    /* these are in properties
    double tramWidth;
    double halfWheelTrack;
    int passes;
     */

    bool isOuter;

    //tramlines
    QVector<Vec2> tramArr;
    QVector<QVector<Vec2>> tramList;

    // 0 off, 1 All, 2, Lines, 3 Outer
    int displayMode, generateMode = 0;

    int controlByte;

    CTram();
    void isTramOuterOrInner();
    void drawTram(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, CCamera &camera);
    void buildTramBnd(const CBoundary &bnd);
    void createBndInnerTramTrack(const CBoundary &bnd);
    void createBndOuterTramTrack(const CBoundary &bnd);
};

#endif // CTRAM_H
