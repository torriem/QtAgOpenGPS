#ifndef CBOUNDARYLINES_H
#define CBOUNDARYLINES_H
#include "vec3.h"
#include "vec2.h"
#include <QVector>

class QOpenGLFunctions;
class QMatrix4x4;

class CBoundaryLines
{
public:
    //list of coordinates of boundary line
    QVector<Vec3> bndLine;
    //the list of constants and multiples of the boundary
    QVector<Vec2> calcList;

    double area;
    bool isSet, isDriveAround, isDriveThru;

    CBoundaryLines();
    void calculateBoundaryHeadings();
    void fixBoundaryLine(int bndNum, double spacing);
    void reverseWinding();
    void preCalcBoundaryLines();
    bool isPointInsideBoundary(Vec3 testPointv3) const;
    bool isPointInsideBoundary(Vec2 testPointv2) const;
    void drawBoundaryLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, QColor color);
    void calculateBoundaryArea();

};

#endif // CBOUNDARYLINES_H
