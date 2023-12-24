#ifndef CABLINE_H
#define CABLINE_H

#include <QObject>
#include <QVector>
#include "vec2.h"
#include "vec3.h"
#include <QString>

class QOpenGLFunctions;
//namespace AgOpenGPS

class CVehicle;
class CBoundary;
class CYouTurn;
class CTool;
class CNMEA;
class CTram;
class CCamera;
class CAHRS;
class CGuidance;

class CABLines
{
public:
    Vec2 origin;
    double heading = 0;
    QString Name = "aa";
    bool isVisible = true;
};

class CABLine: public QObject
{
    Q_OBJECT
private:
    int counter2;
    double shadowOffset = 0;
    double widthMinusOverlap = 0;

public:
    double abFixHeadingDelta = 0;
    double abHeading = 0.0, abLength;
    double angVel = 0;

    bool isABValid, isLateralTriggered;


    //the current AB guidance line
    Vec3 currentABLineP1 = Vec3(0.0, 0.0, 0.0);
    Vec3 currentABLineP2 = Vec3(0.0, 1.0, 0.0);

    double distanceFromCurrentLinePivot;
    double distanceFromRefLine = 0.0;

    //pure pursuit values
    Vec2 goalPointAB = Vec2(0, 0);

    //List of all available ABLines
    QVector<CABLines> lineArr;
    int numABLines, numABLineSelected;

    double howManyPathsAway = 0.0, moveDistance;
    bool isABLineBeingSet;
    bool isABLineSet, isABLineLoaded;
    bool isHeadingSameWay = true;
    bool isBtnABLineOn;

    double ppRadiusAB;
    Vec2 radiusPointAB;
    double rEastAB, rNorthAB;

    //the reference line endpoints
    Vec2 refABLineP1 = Vec2(0.0, 0.0);
    Vec2 refABLineP2 = Vec2(0.0, 1.0);
    double refLineSide = 1.0;

    //the two inital A and B points
    Vec2 refPoint1 = Vec2(0.2, 0.15);
    Vec2 refPoint2 = Vec2(0.3, 0.3);
    double snapDistance, lastSecond = 0;
    double steerAngleAB;
    int lineWidth;

    //design
    Vec2 desPoint1 = Vec2(0.2, 0.15);

    Vec2 desPoint2 = Vec2(0.3, 0.3);
    double desHeading = 0;
    Vec2 desP1 = Vec2(0.0, 0.0);
    Vec2 desP2 = Vec2(999997, 1.0);
    QString desName = "";

    double pivotDistanceError, pivotDistanceErrorLast, pivotDerivative, pivotDerivativeSmoothed;

    //derivative counters
    double inty;
    double steerAngleSmoothed, pivotErrorTotal;
    double distSteerError, lastDistSteerError, derivativeDistError;

    //Color tramColor = Color.YellowGreen;
    int tramPassEvery;

public:
    explicit CABLine(QObject *parent = 0);

    void buildCurrentABLineList(Vec3 pivot,
                                double secondsSinceStart,
                                const CYouTurn &yt);
    void getCurrentABLine(Vec3 pivot, Vec3 steer,
                          double secondsSinceStart,
                          bool isAutoSteerBtnOn,
                          bool steerSwitchHigh,
                          CVehicle &vehicle,
                          CYouTurn &yt,
                          const CAHRS &ahrs,
                          CGuidance &gyd,
                          CNMEA &pn);
    void drawABLines(QOpenGLFunctions *g, const QMatrix4x4 &mvp,
                     bool isFontOn,
                     CBoundary &bnd,
                     CYouTurn &yt,
                     const CCamera &camera,
                     const CGuidance &gyd);
    void buildTram(CBoundary &bnd, CTram &tram);
    void deleteAB();
    void setABLineByBPoint(const CVehicle &vehicle);
    void setABLineByHeading(double heading); //do we need to pass in heading somewhere from the main form?
    void moveABLine(double dist);

signals:
    void stopAutosteer();
    void timedMessage(int,QString,QString);
};

#endif // CABLINE_H
