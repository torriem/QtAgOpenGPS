#ifndef CBOUNDARY_H
#define CBOUNDARY_H

#include "vec2.h"
#include "vec3.h"
#include <QVector>
#include <QVector3D>
#include <QSharedPointer>
#include <QOpenGLBuffer>
#include "cboundarylines.h"
#include "btnenum.h"


class QOpenGLFunctions;
class QMatrix4x4;
class CVehicle;
class CABLine;
class CYouTurn;
class CModuleComm;
class CFieldData;
class CTool;

class CBoundary
{
private:
    const double scanWidth = 1.0;
    const double boxLength = 2000;


    bool bufferCurrent = false;
    bool backBufferCurrent = false;

public:
    //area of boundaries
    QVector<CBoundaryLines> bndList;

    QVector<Vec3> bndBeingMadePts;

    double createBndOffset;
    bool isBndBeingMade;

    bool isDrawRightSide = true;
    bool isOkToAddPoints = false;

    int closestFenceNum;

    //point at the farthest boundary segment from pivotAxle
    Vec3 closestFencePt = Vec3(-10000,-10000, 9);

    // the list of possible bounds points
    QVector<Vec3> turnClosestList;
    int turnSelected, closestTurnNum;
    double iE = 0, iN = 0;

    //point at the farthest turn segment from pivotAxle
    Vec3 closestTurnPt = Vec3(-10000, -10000, 9);
    Vec3 closePt;

    bool isHeadlandOn;
    bool isToolInHeadland, isToolOuterPointsInHeadland, isSectionControlledByHeadland;


    CBoundary();

    //CFence.cs
    bool isPointInsideFenceArea(Vec3 testPoint) const ;
    bool isPointInsideFenceArea(Vec2 testPoint) const;
    void drawFenceLines(const CVehicle &v, const CModuleComm &mc, QOpenGLFunctions *g, const QMatrix4x4 &mvp);

    //CTurn.sh
    int isPointInsideTurnArea(Vec3 pt) const;
    void findClosestTurnPoint(const CABLine &abline, Vec3 fromPt);
    void buildTurnLines(CFieldData &fd);

    //CHead.cs
    void SetHydPosition(btnStates autoBtnState, CVehicle &vehicle); //TODO sounds, p_239
    void WhereAreToolCorners(CTool &tool);
    void WhereAreToolLookOnPoints(CVehicle &vehicle, CTool &tool);
    bool IsPointInsideHeadArea(Vec2 pt);




    /*
    void findClosestBoundaryPoint(Vec2 fromPt, double headAB);
    void resetBoundaries();
    */
    //void drawClosestPoint(QOpenGLFunctions *g, const QMatrix4x4 &mvp);
    //void drawBoundaryLineOnBackBuffer(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
};

#endif // CBOUNDARY_H
