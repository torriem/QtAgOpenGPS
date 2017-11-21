#ifndef CBOUNDARY_H
#define CBOUNDARY_H

#include "vec2.h"
#include <QVector>
#include <QSharedPointer>
#include <QString>


class QOpenGLFunctions_2_1;
class QOpenGLContext;

class CBoundary
{
private:
public:
    //list of coordinates of boundary line
    QVector<Vec2> ptList;
    //the list of constants and multiples of the boundary
    QVector<Vec2> calcList;
    //the list of possible bounds points
    QVector<Vec2> bdList;

    //area variable
    double area = 0 ;
    double areaHectare = 0;
    double areaAcre = 0;

    //boundary variables
    bool isOkToAddPoints = false;
    bool isSet = false;
    bool isDrawRightSide = false;

    //generated box for finding closest point
    Vec2 boxA = Vec2(0, 0), boxB = Vec2(0, 2);
    Vec2 boxC = Vec2(1, 1), boxD = Vec2(2, 3);

    //point at the farthest boundary segment from pivotAxle
    Vec2 closestBoundaryPt = Vec2(-1,-1);

    CBoundary();

    void findClosestBoundaryPoint(Vec2 fromPt);
    void resetBoundary();
    void preCalcBoundaryLines();
    bool isPrePointInPolygon(Vec2 testPoint);

    void drawBoundaryLine(QOpenGLContext *glContext);
    void drawBoundaryLineOnBackBuffer(QOpenGLFunctions_2_1 *gl);
    void calculateBoundaryArea();
    //bool isPointInPolygon();
};

#endif // CBOUNDARY_H
