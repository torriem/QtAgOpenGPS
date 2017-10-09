#ifndef CCONTOUR_H
#define CCONTOUR_H

#include <QVector>
#include <QSharedPointer>
#include "vec2.h"
#include "vec4.h"
#include "cvec.h"

class FormGPS;
class QOpenGLContext;

class CContour
{
private:
    FormGPS *mf;
public:
    bool isContourOn=false, isContourBtnOn=false;

    //used to determine if section was off and now is on or vice versa
    bool wasSectionOn;

    //generated box for finding closest point
    Vec2 boxA = Vec2(0, 0);
    Vec2 boxB = Vec2(0, 2);
    Vec2 boxC = Vec2(1, 1);
    Vec2 boxD = Vec2(2, 3);

    //current contour patch and point closest to current fix
    int closestRefPatch, closestRefPoint;

    //angle to path line closest point and fix
    double refHeading, ref2;

    // for closest line point to current fix
    double minDistance = 99999.0, refX, refZ;

    //generated reference line
    double refLineSide = 1.0;
    Vec2 refPoint1 = Vec2(1, 1), refPoint2 = Vec2(2, 2);

    double distanceFromRefLine;
    double distanceFromCurrentLine;

    int A, B, C;
    double abFixHeadingDelta, abHeading;

    bool isABSameAsFixHeading = true;
    bool isOnRightSideCurrentLine = true;

    //pure pursuit values
    Vec2 pivotAxlePosCT = Vec2(0, 0);
    Vec2 goalPointCT = Vec2(0, 0);
    Vec2 radiusPointCT = Vec2(0, 0);
    double steerAngleCT;
    double rEastCT, rNorthCT;
    double ppRadiusCT;

    double angVel;

    //list of strip data individual points
    QSharedPointer<QVector<Vec4>> ptList;

    //list of the list of individual Lines for entire field
    QVector<QSharedPointer<QVector<Vec4>>> stripList;

    //list of points for the new contour line
    QVector<Vec4> ctList;

    //list of points to determine position ofnew contour line
    QVector<CVec> conList;

    CContour(FormGPS *mf);
    void startContourLine();
    void addPoint();
    void stopContourLine();
    void buildContourGuidanceLine(double eastFix, double northFix);
    void distanceFromContourLine();
    void drawContourLine(QOpenGLContext *c);
    void resetContour();

};

#endif // CCONTOUR_H
