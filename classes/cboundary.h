#ifndef CBOUNDARY_H
#define CBOUNDARY_H

#include "vec2.h"
#include <QVector>
#include <QSharedPointer>
#include <QString>

class FormGPS;

class CBoundary
{
private:
    FormGPS *mf;
public:
    //list of coordinates of boundary line
    QVector<Vec2> ptList;
    //the list of constants and multiples of the boundary
    QVector<Vec2> calcList;

    //area variable
    double area;
    QString areaHectare = "";
    QString areaAcre = "";

    //boundary variables
    bool isOkToAddPoints;
    bool isSet;
    bool isDrawRightSide;

    CBoundary(FormGPS *mf);

    void resetBoundary();
    void preCalcBoundaryLines();
    bool isPrePointInPolygon(Vec2 testPoint);

    void drawBoundaryLine();
    void drawBoundaryLineOnBackBuffer();
    void calculateBoundaryArea();
    bool isPointInPolygon();
};

#endif // CBOUNDARY_H
