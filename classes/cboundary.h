#ifndef CBOUNDARY_H
#define CBOUNDARY_H

#include "vec4.h"
#include "vec3.h"
#include "vec2.h"
#include <QVector>
#include <QVector3D>
#include <QSharedPointer>
#include <QOpenGLBuffer>
#include "cboundarylines.h"


class QOpenGLFunctions;
class QMatrix4x4;
class CVehicle;

class CBoundary
{
private:
    const double scanWidth = 1.0;
    const double boxLength = 2000;


    bool bufferCurrent = false;
    bool backBufferCurrent = false;

public:
    //area of boundaries
    QVector<CBoundaryLines> bndArr;
    QVector<Vec3> bndBeingMadePts;

    double createBndOffset;
    bool isBndBeingMade;

    bool isDrawRightSide = true;
    bool isOkToAddPoints = false;

    // the list of possible bounds points
    QVector<Vec4> bndClosestList;
    int boundarySelected;
    int closestBoundaryNum;

    //generated box for finding closest point
    Vec2 boxA = Vec2(9000, 9000);
    Vec2 boxB = Vec2(9000, 9002);

    Vec2 boxC = Vec2(9001, 9001);
    Vec2 boxD = Vec2(9002, 9003);

    //point at the farthest boundary segment from pivotAxle
    Vec3 closestBoundaryPt = Vec3(-10000, -10000, 9);

    CBoundary();

    void findClosestBoundaryPoint(Vec2 fromPt, double headAB);
    void resetBoundaries();

    void drawBoundaryLines(const CVehicle &v, QOpenGLFunctions *g, const QMatrix4x4 &mvp);
    void drawClosestPoint(QOpenGLFunctions *g, const QMatrix4x4 &mvp);
    //void drawBoundaryLineOnBackBuffer(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
};

#endif // CBOUNDARY_H
