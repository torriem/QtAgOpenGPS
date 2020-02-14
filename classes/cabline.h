#ifndef CABLINE_H
#define CABLINE_H

#include "vec2.h"
#include "cvehicle.h"
#include "cyouturn.h"

class QOpenGLFunctions;
//namespace AgOpenGPS

class CABLine
{
private:

public:
    double abHeading = 0.0;
    double abFixHeadingDelta = 0;

    bool isABSameAsFixHeading = true;
    bool isOnRightSideCurrentLine = true;

    double refLineSide = 1.0;

    double distanceFromRefLine = 0.0;
    double distanceFromCurrentLine = 0.0;
    double snapDistance = 0;

    bool isABLineSet = false;
    bool isABLineBeingSet = false;

    double passNumber = 0.0;

    double howManyPathsAway = 0.0;

    //tramlines
    //Color tramColor = Color.YellowGreen;
    int tramPassEvery = 0;
    int passBasedOn = 0;

    //the two inital A and B points
    Vec2 refPoint1 = Vec2(0.2, 0.2);
    Vec2 refPoint2 = Vec2(0.3, 0.3);

    //the reference line endpoints
    Vec2 refABLineP1 = Vec2(0.0, 0.0);
    Vec2 refABLineP2 = Vec2(0.0, 1.0);

    //the current AB guidance line
    Vec2 currentABLineP1 = Vec2(0.0, 0.0);
    Vec2 currentABLineP2 = Vec2(0.0, 1.0);

    //pure pursuit values
    Vec2 pivotAxlePosAB = Vec2(0, 0);
    Vec2 goalPointAB = Vec2(0, 0);
    Vec2 radiusPointAB = Vec2(0, 0);
    double steerAngleAB = 0;
    double rEastAB = 0, rNorthAB = 0;
    double ppRadiusAB = 0;
    double minLookAheadDistance = 8.0;
    double dx, dy;

    double angVel = 0;

    CVehicle *vehicle;
    CYouTurn *yt;


private:
    //pointers to mainform controls
    //OpenGL gl;


public:
    CABLine(CVehicle *v, CYouTurn *t);
    void deleteAB();
    void setABLineByPoint();
    void setABLineByHeading(); //do we need to pass in heading somewhere from the main form?
    void snapABLine();
    void getCurrentABLine();

    void drawABLines(QOpenGLFunctions *g, const QMatrix4x4 &mvp);

    void resetABLine();
};

#endif // CABLINE_H
