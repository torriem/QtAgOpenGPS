#ifndef CTRAM_H
#define CTRAM_H

#include <QVector>
#include <QMatrix4x4>
#include "vec2.h"

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
    double tramWidth;
    double halfWheelTrack;
    int passes;

    bool isOuter;

    bool isLeftManualOn, isRightManualOn;

    //tramlines
    QSharedPointer<QVector<Vec2>> tramArr;
    QVector<QSharedPointer<QVector<Vec2>>> tramList;

    // 0 off, 1 All, 2, Lines, 3 Outer
    int displayMode, generateMode = 0;

    int controlByte;

    CTram();
    void loadSettings();
    void IsTramOuterOrInner();
    void DrawTram(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, CCamera &camera);
    void BuildTramBnd(const CBoundary &bnd);
    void CreateBndInnerTramTrack(const CBoundary &bnd);
    void CreateBndOuterTramTrack(const CBoundary &bnd);
};

#endif // CTRAM_H
