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
class CYouTurn;
class CTool;
class CNMEA;
class CTram;
class CCamera;

class CABLines
{
public:
    Vec2 ref1;
    Vec2 ref2;
    Vec2 origin;
    double heading = 0;
    QString Name = "aa";
};

class CABLine: public QObject
{
    Q_OBJECT
private:

public:
    double abFixHeadingDelta = 0;
    double abHeading = 0.0;
    double angVel = 0;


    //the current AB guidance line
    Vec2 currentABLineP1 = Vec2(0.0, 0.0);
    Vec2 currentABLineP2 = Vec2(0.0, 1.0);
    double distanceFromCurrentLine = 0.0;
    double distanceFromRefLine = 0.0;

    //pure pursuit values
    Vec2 goalPointAB = Vec2(0, 0);

    //List of all available ABLines
    QVector<CABLines> lineArr;
    int numABLines, numABLineSelected;

    double howManyPathsAway = 0.0, moveDistance;
    bool isABLineBeingSet, isEditing;
    bool isABLineSet, isABLineLoaded;
    bool isABSameAsVehicleHeading = true;
    bool isBtnABLineOn;
    bool isOnRightSideCurrentLine = true;

    double passNumber;
    double ppRadiusAB;
    Vec2 radiusPointAB;
    double rEastAB, rNorthAB;

    //the reference line endpoints
    Vec2 refABLineP1 = Vec2(0.0, 0.0);
    Vec2 refABLineP2 = Vec2(0.0, 1.0);
    double refLineSide = 1.0;

    //the two inital A and B points
    Vec2 refPoint1 = Vec2(0.2, 0.2);
    Vec2 refPoint2 = Vec2(0.3, 0.3);
    double snapDistance;
    double steerAngleAB;
    int lineWidth;


    //tramlines
    QVector<Vec2> tramArr;
    QVector<QVector<Vec2>> tramList;

    //Color tramColor = Color.YellowGreen;
    int tramPassEvery = 0;

private:
    //pointers to mainform controls
    //OpenGL gl;


public:
    CABLine();
    void deleteAB();
    void setABLineByBPoint(const CVehicle &vehicle);
    void setABLineByHeading(); //do we need to pass in heading somewhere from the main form?
    void snapABLine();
    void getCurrentABLine(Vec3 pivot, Vec3 steer, CVehicle &vehicle, CYouTurn &yt, const CTool &tool, CNMEA &pn, double speed);
    void drawABLines(QOpenGLFunctions *g, const QMatrix4x4 &mvp, const CVehicle &vehicle, const CTool &tool, CYouTurn &yt, CTram &tram, const CCamera &camera, bool isSideGuideLines);
    void drawTram(QOpenGLFunctions *g, const QMatrix4x4 &mvp);
    void buildTram();
    void moveABLine(double dist);
    void resetABLine();

signals:
    void doSequence();
    void showMessage(int,QString,QString);
};

#endif // CABLINE_H
