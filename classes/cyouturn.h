#ifndef YOUTURN_H
#define YOUTURN_H

#include <QString>
#include <QVector>
#include <QVector2D>
#include <QVector4D>
#include "vec2.h"

class CABLine;
class CVehicle;

class CYouTurn
{
private:
    int A, B, C;

public:
    bool isYouTurnOn, isAutoTriggered, isAutoPointSet, isAutoTurnRight, isLastAutoTurnRight;
    bool isRecordingYouTurn, isAutoYouTurnEnabled;
    int startYouTurnAt;

    //guidance values
    double distanceFromCurrentLine;
    double dxAB, dyAB;
    double abHeading;
    bool isABSameAsFixHeading = true;
    bool isOnRightSideCurrentLine = true;

    //pure pursuit values
    Vec2 pivotAxlePosYT;
    Vec2 goalPointYT;
    Vec2 radiusPointYT;
    double steerAngleYT;
    double rEastYT, rNorthYT;
    double ppRadiusYT;
    double minLookAheadDistance;
    int numShapePoints;

    //list of points for scaled and rotated YouTurn line
    QVector<QVector4D> ytList;

    //list of points read from file, this is the actual pattern from a bunch of sources possible
    QVector<QVector2D> youFileList;

    QVector2D autoYouTurnTriggerPoint;

    //constructor
    CYouTurn();

    void cancelYouTurn();

    bool loadYouTurnShapeFromFile(QString filename);

    //build the points and path
    void buildYouTurnListToRight(CABLine *ABLine, bool isTurnRight);
    void distanceFromYouTurnLine(CVehicle *vehicle);

};

#endif // YOUTURN_H
