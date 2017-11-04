#ifndef CBOUNDARY_H
#define CBOUNDARY_H

#include "vec2.h"
#include <QVector>
#include <QSharedPointer>
#include <QString>


class QOpenGLFunctions_2_1;
class QOpenGLContext;
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
    double area = 0 ;
    double areaHectare = 0;
    double areaAcre = 0;

    //boundary variables
    bool isOkToAddPoints = false;
    bool isSet = false;
    bool isDrawRightSide = false;

    CBoundary(FormGPS *mf);

    void resetBoundary();
    void preCalcBoundaryLines();
    bool isPrePointInPolygon(Vec2 testPoint);

    void drawBoundaryLine(QOpenGLContext *glContext);
    void drawBoundaryLineOnBackBuffer(QOpenGLFunctions_2_1 *gl);
    void calculateBoundaryArea();
    bool isPointInPolygon();
};

#endif // CBOUNDARY_H
