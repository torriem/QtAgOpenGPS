#ifndef CABLINE_H
#define CABLINE_H

#include <QObject>
#include <QVector>
#include "vec2.h"
#include "vec3.h"
#include <QString>
#include "ctrk.h"

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
class CTrack;
class CWorldGrid;

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
    Vec3 currentLinePtA = Vec3(0.0, 0.0, 0.0);
    Vec3 currentLinePtB = Vec3(0.0, 1.0, 0.0);

    double distanceFromCurrentLinePivot;
    double distanceFromRefLine = 0.0;

    //pure pursuit values
    Vec2 goalPointAB = Vec2(0, 0);

    //List of all available ABLines
    CTrk refLine;

    double howManyPathsAway;
    bool isMakingABLine;
    bool isHeadingSameWay = true;

    double ppRadiusAB;
    Vec2 radiusPointAB;
    double rEastAB, rNorthAB;

    double snapDistance, lastSecond = 0;
    double steerAngleAB;
    int lineWidth;

    //design
    Vec2 desPtA = Vec2(0.2, 0.15);
    Vec2 desPtB = Vec2(0.3, 0.3);

    Vec2 desLineEndA = Vec2(0.0, 0.0);
    Vec2 desLineEndB = Vec2(999997, 1.0);

    Vec2 refNudgePtA = Vec2(1, 1), refNudgePtB = Vec2(2, 2);

    double desHeading = 0;

    QString desName = "";

    //autosteer errors
    double pivotDistanceError, pivotDistanceErrorLast, pivotDerivative, pivotDerivativeSmoothed;

    //derivative counters
    double inty;
    double steerAngleSmoothed, pivotErrorTotal;
    double distSteerError, lastDistSteerError, derivativeDistError;

    //Color tramColor = Color.YellowGreen;
    int tramPassEvery;

public:
    explicit CABLine(QObject *parent = 0);

    void BuildCurrentABLineList(Vec3 pivot,
                                double secondsSinceStart,
                                CTrack &trk,
                                const CYouTurn &yt,
                                const CVehicle &vehicle);

    void GetCurrentABLine(Vec3 pivot, Vec3 steer,
                          double secondsSinceStart,
                          bool isAutoSteerBtnOn,
                          bool steerSwitchHigh,
                          CTrack &trk,
                          CVehicle &vehicle,
                          CYouTurn &yt,
                          const CAHRS &ahrs,
                          CGuidance &gyd,
                          CNMEA &pn);

    void DrawABLines(QOpenGLFunctions *g, const QMatrix4x4 &mvp,
                     bool isFontOn,
                     CTrack &trk,
                     CYouTurn &yt,
                     const CWorldGrid &worldGrid,
                     const CCamera &camera,
                     const CGuidance &gyd);

    void DrawABLineNew(QOpenGLFunctions *g, const QMatrix4x4 &mvp, const CCamera &camera);

    void BuildTram(CTrack &trk, CBoundary &bnd, CTram &tram);

signals:
    void stopAutosteer();
    void TimedMessage(int,QString,QString);
};

#endif // CABLINE_H
