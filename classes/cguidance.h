#ifndef CGUIDANCE_H
#define CGUIDANCE_H

#include <QVector>
#include "vec3.h"

class CVehicle;
class CAHRS;
class CYouTurn;
class CABLine;
class CABCurve;

class CGuidance
{
private:
    int sA, sB, C, pA, pB;

public:
    double distanceFromCurrentLineSteer, distanceFromCurrentLinePivot;
    double steerAngleGu, rEastSteer, rNorthSteer, rEastPivot, rNorthPivot;

    double inty, xTrackSteerCorrection = 0;
    double steerHeadingError, steerHeadingErrorDegrees;

    double distSteerError, lastDistSteerError, derivativeDistError;

    double pivotDistanceError, stanleyModeMultiplier;

    //public int modeTimeCounter = 0;

    //for adding steering angle based on side slope hill
    /*property_setAS_sideHillComp
    double sideHillCompFactor;
    */

    //derivative counter
    int counter;

    CGuidance();

    void doSteerAngleCalc(bool isAutoSteerBtnOn,
                          CVehicle &vehicle,
                          const CAHRS &ahrs
                         );

    void StanleyGuidanceABLine(Vec3 curPtA, Vec3 curPtB,
                               Vec3 pivot, Vec3 steer,
                               bool isAutoSteerBtnOn,
                               CVehicle &vehicle,
                               CABLine &CABLine,
                               const CAHRS &ahrs,
                               CYouTurn &yt);


    void StanleyGuidanceCurve(Vec3 pivot, Vec3 steer,
                              QVector<Vec3> &curList,
                              bool isAutoSteerBtnOn,
                              CVehicle &vehicle,
                              CABCurve &curve,
                              const CAHRS &ahrs);
};

#endif // CGUIDANCE_H
