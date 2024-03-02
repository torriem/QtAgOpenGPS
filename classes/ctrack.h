#ifndef CTRACK_H
#define CTRACK_H

#include <QVector>
#include <QString>

#include "vec3.h"
#include "vec2.h"
#include "ctrk.h"

class CVehicle;
class CABLine;
class CABCurve;

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
