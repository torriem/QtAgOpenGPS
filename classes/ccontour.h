#ifndef CCONTOUR_H
#define CCONTOUR_H
#include <QObject>
#include <QVector>
#include <QSharedPointer>
#include "vec2.h"
#include "vec4.h"
#include "cvec.h"

class QOpenGLFunctions;
class QMatrix4x4;
class CVehicle;
class CBoundary;
class CTool;
class CNMEA;

class CContour: public QObject
{
    Q_OBJECT
private:
    bool ctListBufferCurrent = false;
    int A = 0, B = 0, C = 0;

public:
    bool isContourOn=false, isContourBtnOn=false, isRightPriority = true;

    //used to determine if section was off and now is on or vice versa
    bool wasSectionOn = false;

    //generated box for finding closest point
    Vec2 boxA = Vec2(0, 0);
    Vec2 boxB = Vec2(0, 2);
    Vec2 boxC = Vec2(1, 1);
    Vec2 boxD = Vec2(2, 3);
    Vec2 boxE = Vec2(3, 4), boxF = Vec2(4, 5);
    Vec2 boxG = Vec2(6, 6), boxH = Vec2(7, 7);


    //current contour patch and point closest to current fix
    int closestRefPatch = 0, closestRefPoint = 0;

    //angle to path line closest point and fix
    double refHeading = 0, ref2 = 0;

    // for closest line point to current fix
    double minDistance = 99999.0, refX, refZ;

    //generated reference line
    double refLineSide = 1.0;
    Vec2 refPoint1 = Vec2(1, 1), refPoint2 = Vec2(2, 2);

    double distanceFromRefLine = 0;
    double distanceFromCurrentLine = 0;

    double abFixHeadingDelta = 0, abHeading = 0;

    bool isABSameAsVehicleHeading = true;
    bool isOnRightSideCurrentLine = true;

    //pure pursuit values
    bool isValid;

    Vec2 goalPointCT = Vec2(0, 0);
    Vec2 radiusPointCT = Vec2(0, 0);
    double steerAngleCT = 0;
    double rEastCT = 0, rNorthCT = 0;
    double ppRadiusCT = 0;

    //list of strip data individual points
    QSharedPointer<QVector<Vec3>> ptList;

    //list of the list of individual Lines for entire field
    QVector<QSharedPointer<QVector<Vec3>>> stripList;

    //list of points for the new contour line
    QVector<Vec3> ctList;

    //list of points to determine position ofnew contour line
    QVector<CVec> conList;

    explicit CContour(QObject *parent = 0);
    void startContourLine(Vec3 pivot);
    void addPoint(Vec3 pivot);
    void stopContourLine(Vec3 pivot, QVector<QSharedPointer<QVector<Vec3>>> &contourSaveList);
    void buildBoundaryContours(CBoundary &bnd, int pass, int spacingInt);
    void buildContourGuidanceLine(CVehicle &vehicle, CNMEA &pn, Vec3 pivot);
    void calculateContourHeadings();
    void distanceFromContourLine(CVehicle &vehicle, CNMEA &pn, Vec3 pivot, Vec3 steer);
    void drawContourLine(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
    void resetContour();
signals:
    //void guidanceLineDistanceOff(int);
    //void distanceDisplay(int);
    //void guidanceLineSteerAngle(int);
    void showMessage(int,QString,QString);
};

#endif // CCONTOUR_H
