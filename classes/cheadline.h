#ifndef CHEADLINE_H
#define CHEADLINE_H

#include <QVector>
#include "vec3.h"

class CBoundary;


/*
 * defined in ctrack.h
enum TrackMode {
    None = 0,
    AB = 2,
    Curve = 4,
    bndTrackOuter = 8,
    bndTrackInner = 16,
    bndCurve = 32
};
*/

class CHeadPath
{
public:
    QVector<Vec3> trackPts;
    QString name = "aa";
    double moveDistance = 0;
    int mode = 0;
    int a_point = 0;

    bool operator< (const CHeadPath &rhs) const {
        return a_point < rhs.a_point;
    }
};

class CHeadLine
{
public:
    QVector<CHeadPath> tracksArr;
    int idx;
    QVector<Vec3> desList;

    CHeadLine();
    void CalculateHeadings(QVector<Vec3> &xList);
    void AddFirstLastPoints(QVector<Vec3> &xList, const CBoundary &bnd);
};

#endif // CHEADLINE_H
