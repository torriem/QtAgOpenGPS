#ifndef CTRAM_H
#define CTRAM_H

#include <QVector>
#include <QMatrix4x4>
#include <QObject>
#include "vec2.h"
#include "setter.h"

class CBoundary;
class QOpenGLFunctions;
class CCamera;

class CTram : public QObject
{
    Q_OBJECT
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

    Q_PROPERTY (bool isLeftManualOn MEMBER isLeftManualOn NOTIFY isLeftManualOnChanged)
    bool isLeftManualOn;

    Q_PROPERTY (bool isRightManualOn MEMBER isRightManualOn NOTIFY isRightManualOnChanged)
    bool isRightManualOn;

    //tramlines
    QSharedPointer<QVector<Vec2>> tramArr;
    QVector<QSharedPointer<QVector<Vec2>>> tramList;

    // 0 off, 1 All, 2, Lines, 3 Outer
    int displayMode, generateMode = 0;

    int controlByte;

    explicit CTram(QObject *parent = 0);
    void loadSettings();
    void IsTramOuterOrInner();
    void DrawTram(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, CCamera &camera);
    void BuildTramBnd(const CBoundary &bnd);
    void CreateBndInnerTramTrack(const CBoundary &bnd);
    void CreateBndOuterTramTrack(const CBoundary &bnd);

    SETTER(bool, isLeftManualOn, setIsLeftManualOn)
    SETTER(bool, isRightManualOn, setIsRightManualOn)

signals:
    void isLeftManualOnChanged();
    void isRightManualOnChanged();
};

#endif // CTRAM_H
