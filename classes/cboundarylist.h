#ifndef CBOUNDARYLIST_H
#define CBOUNDARYLIST_H
#include "vec3.h"
#include "vec2.h"
#include <QVector>

class QOpenGLFunctions;
class QMatrix4x4;

class CBoundaryList
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

    CBoundaryList();
    void CalculateFenceLineHeadings();
    void FixFenceLine(int bndNum);
    void ReverseWinding();
    bool CalculateFenceArea(int idx);

    /* CTurnLines.cs*/
    void CalculateTurnHeadings();
    void FixTurnLine(double totalHeadWidth, double spacing);

   /*
    bool isPointInsideBoundary(Vec3 testPointv3) const;
    bool isPointInsideBoundary(Vec2 testPointv2) const;
    void drawBoundaryLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, QColor color);
    void calculateBoundaryArea();
*/

};

#endif // CBOUNDARYLIST_H
