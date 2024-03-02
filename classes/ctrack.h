#ifndef CTRACK_H
#define CTRACK_H

#include <QVector>
#include <QString>

#include "vec3.h"
#include "vec2.h"

class CVehicle;
class CABLine;
class CABCurve;

enum TrackMode { None = 0, AB = 2, Curve = 4, bndTrackOuter = 8, bndTrackInner = 16, bndCurve = 32};//, Heading, Circle, Spiral

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

    CTrk() :
        heading(3),
        name(QString("New Track")),
        isVisible(true),
        mode(0),
        nudgeDistance(0)
    { }

    CTrk(const CTrk& _trk) :
        curvePts(_trk.curvePts),
        heading(_trk.heading),
        name(_trk.name),
        isVisible(_trk.isVisible),
        ptA(_trk.ptA),
        ptB(_trk.ptB),
        endPtA(_trk.endPtA),
        endPtB(_trk.endPtB),
        mode(_trk.mode),
        nudgeDistance(_trk.nudgeDistance)
    { }
};

class CTrack
{
public:
    QVector<CTrk> gArr;

    int idx, autoTrack3SecTimer;

    bool isLine, isAutoTrack = false, isAutoSnapToPivot = false, isAutoSnapped;

    CTrack(): idx(-1) { }
    int FindClosestRefTrack(Vec3 pivot, const CVehicle& vehicle);
    void NudgeTrack(double dist, CABLine &ABLine, CABCurve &curve);
    void NudgeDistanceReset(CABLine &ABLine, CABCurve &curve);
    void SnapToPivot(CABLine &ABLine, CABCurve &curve);
    void NudgeRefTrack(double dist, CABLine &ABLine, CABCurve &curve);
    void NudgeRefABLine(double dist);
    void NudgeRefCurve(double dist, CABCurve &curve);
};

#endif // CTRACK_H
