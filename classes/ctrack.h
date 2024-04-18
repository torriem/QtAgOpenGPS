#ifndef CTRACK_H
#define CTRACK_H

#include <QVector>
#include "vec3.h"
#include "vec2.h"

class CVehicle;
class CABLine;
class CABCurve;


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

    CTrk();
};

class CTrack
{
public:
    QVector<CTrk> gArr;

    int idx, autoTrack3SecTimer;

    bool isLine, isAutoTrack = false, isAutoSnapToPivot = false, isAutoSnapped;

    CTrack();

    int FindClosestRefTrack(Vec3 pivot, const CVehicle &vehicle);
    void NudgeTrack(double dist, CABLine &ABLine, CABCurve &curve);
    void NudgeDistanceReset(CABLine &ABLine, CABCurve &curve);
    void SnapToPivot(CABLine &ABLine, CABCurve &curve);
    void NudgeRefTrack(double dist, CABLine &ABLine, CABCurve &curve);
    void NudgeRefABLine(double dist);
    void NudgeRefCurve(double distAway, CABCurve &curve);
};

#endif // CTRACK_H
