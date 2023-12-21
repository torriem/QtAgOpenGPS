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
class CCamera;
class CBoundary;
class CNMEA;
class CAHRS;


class CCurveLines
{
public:
    QVector<Vec3> curvePts;
    double aveHeading = 3;
    QString Name = "aa";
    bool isVisible = true;
};



class CABCurve : public QObject
{
    Q_OBJECT
private:
    int A, B, C;
    int rA, rB;

public:
    //flag for starting stop adding points
    bool isBtnCurveOn, isOkToAddDesPoints, isCurveSet;

    double distanceFromCurrentLinePivot;
    double distanceFromRefLine;

    bool isHeadingSameWay = true;

    double howManyPathsAway;

    Vec2 refPoint1 = Vec2(1, 1), refPoint2 = Vec2(2, 2);

    double refHeading, moveDistance;

    Vec2 boxC = Vec2(1, 1), boxD = Vec2(2, 3);
    int currentLocationIndex;

    double aveLineHeading;

    //pure pursuit values
    Vec2 goalPointCu = Vec2(0, 0);

    Vec2 radiusPointCu = Vec2(0, 0);
    double steerAngleCu, rEastCu, rNorthCu, ppRadiusCu, manualUturnHeading;

    //the list of points of the ref line.
    QVector<Vec3> refList;
    //the list of points of curve to drive on
    QVector<Vec3> curList;

    bool isSmoothWindowOpen;
    QVector<Vec3> smooList;

    QVector<CCurveLines> curveArr;
    int numCurveLines, numCurveLineSelected;

    bool isCurveValid, isLateralTriggered;

    double lastSecond = 0;

    QVector<Vec3> desList;
    QString desName = "**";

    explicit CABCurve(QObject *parent = 0);

    void buildCurveCurrentList(Vec3 pivot,
                               const CVehicle &vehicle,
                               const CBoundary &bnd,
                               const CYouTurn &yt);
    void getCurrentCurveLine(Vec3 pivot,
                             CVehicle &vehicle,
                             const CYouTurn &yt,
                             const CAHRS &ahrs,
                             const CNMEA &pn);


    void drawCurve(QOpenGLFunctions *gl, const QMatrix4x4 &mvp,
                   const CVehicle &vehicle,
                   CYouTurn &yt, CTram &tram, const CCamera &camera
                   );

    //void drawTram(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);

    void buildTram(CBoundary &bnd, CTram &tram);
    void smoothAB(int smPts);
    void calculateTurnHeadings();
    void saveSmoothAsRefList();
    void moveABCurve(double dist);
    bool pointOnLine(Vec3 pt1, Vec3 pt2, Vec3 pt);
    void addFirstLastPoints(QVector<Vec3> &xList);
    void resetCurveLine();


signals:
    void doSequence(CYouTurn &yt);
    void timedMessage(QString title, QString message, int timeout);
    void stopAutosteer();

public slots:
};

#endif // CABCURVE_H
