#ifndef CBOUNDARY_H
#define CBOUNDARY_H

#include "vec2.h"
#include "vec3.h"
#include <QVector>
#include <QVector3D>
#include <QSharedPointer>
#include <QOpenGLBuffer>
#include "cboundarylist.h"
#include "btnenum.h"
#include <QObject>
#include "interfaceproperty.h"


class QOpenGLFunctions;
class QMatrix4x4;
class CVehicle;
class CABLine;
class CYouTurn;
class CModuleComm;
class CFieldData;
class CTool;
class CPGN_EF;

class CBoundary : public QObject
{
    Q_OBJECT
private:
    const double scanWidth = 1.0;
    const double boxLength = 2000;


    bool bufferCurrent = false;
    bool backBufferCurrent = false;

public:
    //area of boundaries
    QVector<CBoundaryList> bndList;

    QVector<Vec3> bndBeingMadePts;

    InterfaceProperty<BoundaryInterface,double> createBndOffset = InterfaceProperty<BoundaryInterface,double>("createBndOffset");
    //InterfaceProperty<BoundaryInterface,bool> isBndBeingMade = InterfaceProperty<BoundaryInterface,bool>("isBndBeingMade");
    bool isBndBeingMade = false;

    InterfaceProperty<BoundaryInterface,bool> isDrawRightSide = InterfaceProperty<BoundaryInterface,bool>("isDrawRightSide");
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


    CBoundary(QObject *parent = 0);

    //CFence.cs
    bool IsPointInsideFenceArea(Vec3 testPoint) const ;
    bool IsPointInsideFenceArea(Vec2 testPoint) const;
    void DrawFenceLines(const CVehicle &v, const CModuleComm &mc, QOpenGLFunctions *g, const QMatrix4x4 &mvp);

    //CTurn.sh
    int IsPointInsideTurnArea(Vec3 pt) const;
    void FindClosestTurnPoint(const CABLine &abline, Vec3 fromPt);
    void BuildTurnLines(CFieldData &fd);

    //CHead.cs
    void SetHydPosition(btnStates autoBtnState, CPGN_EF &p_239, CVehicle &vehicle); //TODO sounds, p_239
    void WhereAreToolCorners(CTool &tool);
    void WhereAreToolLookOnPoints(CVehicle &vehicle, CTool &tool);
    bool IsPointInsideHeadArea(Vec2 pt);




    /*
    void findClosestBoundaryPoint(Vec2 fromPt, double headAB);
    void resetBoundaries();
    */
    //void drawClosestPoint(QOpenGLFunctions *g, const QMatrix4x4 &mvp);
    //void drawBoundaryLineOnBackBuffer(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);

signals:
    void TimedMessage(int timeout, QString title, QString message);
    void soundHydLiftChange(bool);

};

#endif // CBOUNDARY_H
