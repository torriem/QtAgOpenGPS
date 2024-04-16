#ifndef CTRACK_H
#define CTRACK_H

#include <QVector>
#include "vec3.h"
#include "vec2.h"

enum TrackMode {
    None = 0,
    AB = 2,
    Curve = 4,
    bndTrackOuter = 8,
    bndTrackInner = 16,
    bndCurve = 32,
    waterPivot = 64
};//, Heading, Circle, Spiral

class CTrk
{
public:
    QVector<Vec3> curvePts;
    double heading;
    QString name;
    bool isVisible;
    Vec2 ptA;
    Vec2 ptB;
    Vec2 endPtA;
    Vec2 endPtB;
    int mode;
    double nudgeDistance;
};

class CTrack
{
public:
    QVector<CTrk> gArr;

    int idx, autoTrack3SecTimer;

    bool isLine, isAutoTrack = false, isAutoSnapToPivot = false, isAutoSnapped;

    CTrack();

    int FindClosestRefTrack(Vec3 pivot);
    void NudgeTrack(double dist);
    void NudgeDistanceReset();
    void SnapToPivot();
    void NudgeRefTrack(double dist);
    void NudgeRefABLine(double dist);
    void NudgeRefCurve(double distAway);
};

#endif // CTRACK_H
