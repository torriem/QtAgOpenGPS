#ifndef CTURN_H
#define CTURN_H

#include <QObject>
#include <QVector>
#include "cturnlines.h"
#include "vec4.h"
#include "vec2.h"
#include "vec3.h"

class QOpenGLFunctions;
class QMatrix4x4;
class CBoundary;
class CTool;
class CFieldData;

class CTurn: public QObject
{
    Q_OBJECT
private:
    double boxLength;
public:
    QVector<CTurnLines> turnArr;

    //the list of possible bounds points
    QVector<Vec4> turnClosestList;
    int turnSelected, closestTurnNum;

    //generated box for finding closest point
    Vec2 boxA, boxB;
    Vec2 boxC, boxD;

    //point at the farthest turn segment from pivotAxle
    Vec3 closestTurnPt;

    explicit CTurn(QObject *parent = NULL);

    void findClosestTurnPoint(const CBoundary &bnd, const CTool &tool, bool isYouTurnRight, Vec3 fromPt, double headAB);
    bool pointInsideWorkArea(const CBoundary &bnd, Vec2 pt);
    void resetTurnLines();
    void buildTurnLines(const CBoundary &bnd, const CTool &tool, CFieldData &fd, double youTurnTriggerDistanceOffset);
    void drawTurnLines(const CBoundary &bnd, QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
    void drawClosestPoint(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);

signals:
    void showMessage(int,QString,QString);
};

#endif // CTURN_H
