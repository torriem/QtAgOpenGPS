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
    QVector<Vec3> fenceLine;
    QVector<Vec2> fenceLineEar;
    QVector<Vec3> hdLine;
    QVector<Vec3> turnLine;


    //the list of constants and multiples of the boundary
    //QVector<Vec2> calcList;

    //area variable
    double area;

    //boundary variables
    bool isDriveThru;

    //bool isSet, isDriveThru;

    CBoundaryLines();
    void calculateFenceLineHeadings();
    void fixFenceLine(int bndNum);
    void reverseWinding();
    bool calculateFenceArea(int idx);

    /* CTurnLines.cs*/
    void calculateTurnHeadings();
    void fixTurnLine(double totalHeadWidth, double spacing);

   /*
    bool isPointInsideBoundary(Vec3 testPointv3) const;
    bool isPointInsideBoundary(Vec2 testPointv2) const;
    void drawBoundaryLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, QColor color);
    void calculateBoundaryArea();
*/

};

#endif // CBOUNDARYLINES_H
