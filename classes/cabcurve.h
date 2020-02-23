#ifndef CABCURVE_H
#define CABCURVE_H

#include <QObject>
#include <QVector>
#include "vec2.h"
#include "vec3.h"

class QOpenGLFunctions;
class QMatrix4x4;
class CVehicle;
class CYouTurn;
class CTram;
class CTool;
class CCamera;
class CBoundary;
class CNMEA;


class CCurveLines
{
    QVector<Vec3> curvePts;
    double aveHeading = 3;
    QString Name = "aa";
};



class CABCurve : public QObject
{
    Q_OBJECT
private:
    int closestRefIndex = 0;
    int A, B, C;

public:
    //flag for starting stop adding points
    bool isBtnCurveOn, isOkToAddPoints, isCurveSet;

    double distanceFromCurrentLine;
    bool isABSameAsVehicleHeading = true;
    bool isOnRightSideCurrentLine = true;

    double howManyPathsAway, curveNumber;
    Vec2 refPoint1 = Vec2(1, 1), refPoint2 = Vec2(2, 2);

    bool isSameWay;
    double refHeading, moveDistance;
    double deltaOfRefAndAveHeadings;

    //generated box for finding closest point
    Vec2 boxA = Vec2(0, 0), boxB = Vec2(0, 2);

    Vec2 boxC = Vec2(1, 1), boxD = Vec2(2, 3);
    int currentLocationIndex;

    double aveLineHeading;

    //pure pursuit values
    Vec2 goalPointCu = Vec2(0, 0);

    Vec2 radiusPointCu = Vec2(0, 0);
    double steerAngleCu, rEastCu, rNorthCu, ppRadiusCu;

    //the list of points of the ref line.
    QVector<Vec3> refList;
    //the list of points of curve to drive on
    QVector<Vec3> curList;

    bool isSmoothWindowOpen;
    QVector<Vec3> smooList;

    QVector<CCurveLines> curveArr;
    int numCurveLines, numCurveLineSelected;

    bool isEditing;
    QVector<Vec2> tramArr;
    QVector<QVector<Vec2>> tramList;

    explicit CABCurve(QObject *parent = 0);
    void drawCurve(QOpenGLFunctions *gl, const QMatrix4x4 &mvp,
                   const CVehicle &vehicle, const CTool &tool,
                   CYouTurn &yt, CTram &tram, const CCamera &camera
                   );

    void drawTram(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);

    void buildTram(CBoundary &bnd, CTram &tram);
    void smoothAB(int smPts);
    void calculateTurnHeadings();
    void saveSmoothAsRefList();
    void getCurrentCurveLine(Vec3 pivot, Vec3 steer,
                             CVehicle &vehicle, CYouTurn &yt,
                             const CTool &tool, CNMEA &pn,
                             double speed);
    void snapABCurve();
    void moveABCurve(double dist);
    bool pointOnLine(Vec3 pt1, Vec3 pt2, Vec3 pt);
    void addFirstLastPoints();
    void resetCurveLine();


signals:
    void doSequence();

public slots:
};

#endif // CABCURVE_H
