#ifndef CPERIMETER_H
#define CPERIMETER_H

#include <QVector>
#include "vec2.h"

class CPerimeter
{
public:
    //private read only OpenGL gl

    //list of coordinates of perimter line
    QVector<Vec2> periPtList;
    QVector<Vec2> calcList;

    //list of the list of patch data individual triangles for that entire section activity
    QVector<QVector<Vec2>> periLineList;

    //button status
    bool isBtnPerimeterPn = false;

    //area variable
    double area = 0.0;

    CPerimeter();

    void drawPerimeterLine();

};

#endif // CPERIMETER_H
