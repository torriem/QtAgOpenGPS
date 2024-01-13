#include "cabcurve.h"
#include "glutils.h"
#include "glm.h"
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector>
#include "vec3.h"
#include "vec2.h"
#include "cvehicle.h"
#include "cyouturn.h"
#include "cboundary.h"
#include "cyouturn.h"
#include "ctram.h"
#include "ccamera.h"
#include "cnmea.h"
#include "cahrs.h"
#include "cguidance.h"
#include "aogproperty.h"

CABCurve::CABCurve(QObject *parent) : QObject(parent)
{

}
void CABCurve::BuildCurveCurrentList(Vec3 pivot,
                                     double secondsSinceStart,
                                     const CVehicle &vehicle,
                                     const CBoundary &bnd,
                                     const CYouTurn &yt)
{
    double minDistA = 1000000, minDistB;
    //move the ABLine over based on the overlap amount set in vehicle

    double tool_width = property_setVehicle_toolWidth;
    double tool_overlap = property_setVehicle_toolOverlap;
    double tool_offset = property_setVehicle_toolOffset;

    double widthMinusOverlap = tool_width - tool_overlap;

    int refCount = refList.count();
    if (refCount < 5)
    {
        curList.clear();
        return;
    }

    //close call hit
    int cc = 0, dd;

    for (int j = 0; j < refCount; j += 10)
    {
        double dist = ((vehicle.guidanceLookPos.easting - refList[j].easting) * (vehicle.guidanceLookPos.easting - refList[j].easting))
                      + ((vehicle.guidanceLookPos.northing - refList[j].northing) * (vehicle.guidanceLookPos.northing - refList[j].northing));
        if (dist < minDistA)
        {
            minDistA = dist;
            cc = j;
        }
    }

    minDistA = minDistB = 1000000;

    dd = cc + 7; if (dd > refCount - 1) dd = refCount;
    cc -= 7; if (cc < 0) cc = 0;

    //find the closest 2 points to current close call
    for (int j = cc; j < dd; j++)
    {
        double dist = ((vehicle.guidanceLookPos.easting - refList[j].easting) * (vehicle.guidanceLookPos.easting - refList[j].easting))
                      + ((vehicle.guidanceLookPos.northing - refList[j].northing) * (vehicle.guidanceLookPos.northing - refList[j].northing));
        if (dist < minDistA)
        {
            minDistB = minDistA;
            rB = rA;
            minDistA = dist;
            rA = j;
        }
        else if (dist < minDistB)
        {
            minDistB = dist;
            rB = j;
        }
    }

    //reset the line over jump
    isLateralTriggered = false;

    if (rA > rB) { C = rA; rA = rB; rB = C; }

    //same way as line creation or not
    isHeadingSameWay = M_PI - fabs(fabs(pivot.heading - refList[rA].heading) - M_PI) < glm::PIBy2;

    if (yt.isYouTurnTriggered) isHeadingSameWay = !isHeadingSameWay;

    //which side of the closest point are we on is next
    //calculate endpoints of reference line based on closest point
    refPoint1.easting = refList[rA].easting - (sin(refList[rA].heading) * 300.0);
    refPoint1.northing = refList[rA].northing - (cos(refList[rA].heading) * 300.0);

    refPoint2.easting = refList[rA].easting + (sin(refList[rA].heading) * 300.0);
    refPoint2.northing = refList[rA].northing + (cos(refList[rA].heading) * 300.0);

    //x2-x1
    double dx = refPoint2.easting - refPoint1.easting;
    //z2-z1
    double dz = refPoint2.northing - refPoint1.northing;

    //how far are we away from the reference line at 90 degrees - 2D cross product and distance
    distanceFromRefLine = ((dz * vehicle.guidanceLookPos.easting) -
                           (dx * vehicle.guidanceLookPos.northing) +
                           (refPoint2.easting * refPoint1.northing) -
                           (refPoint2.northing * refPoint1.easting))
                          / sqrt((dz * dz) + (dx * dx));

    double RefDist = (distanceFromRefLine +
                      (isHeadingSameWay ? tool_offset : tool_offset)) / widthMinusOverlap;
    if (RefDist < 0) howManyPathsAway = (int)(RefDist - 0.5);
    else howManyPathsAway = (int)(RefDist + 0.5);

    //build current list
    isCurveValid = true;

    //build the current line
    curList.clear();

    double distAway = widthMinusOverlap * howManyPathsAway +
                      (isHeadingSameWay ? -tool_offset : tool_offset);

    if (howManyPathsAway > -1) howManyPathsAway += 1;

    double distSqAway = (distAway * distAway) - 0.01;
    Vec3 point;
    for (int i = 0; i < refCount; i++)
    {
        point = Vec3(
            refList[i].easting + (sin(glm::PIBy2 + refList[i].heading) * distAway),
            refList[i].northing + (cos(glm::PIBy2 + refList[i].heading) * distAway),
            refList[i].heading);
        bool Add = true;

        for (int t = 0; t < refCount; t++)
        {
            double dist = ((point.easting - refList[t].easting) *
                           (point.easting - refList[t].easting))
                          + ((point.northing - refList[t].northing) *
                             (point.northing - refList[t].northing));
            if (dist < distSqAway)
            {
                Add = false;
                break;
            }
        }

        if (Add)
        {
            if (curList.count() > 0)
            {
                double dist = ((point.easting - curList[curList.count() - 1].easting) *
                               (point.easting - curList[curList.count() - 1].easting))
                              + ((point.northing - curList[curList.count() - 1].northing) *
                                 (point.northing - curList[curList.count() - 1].northing));
                if (dist > 1.5)
                    curList.append(point);
            }
            else curList.append(point);
        }
    }

    int cnt = curList.count();
    if (cnt > 6)
    {
        QVector<Vec3> arr(curList);

        curList.clear();

        for (int i = 0; i < (arr.count() - 1); i++)
        {
            arr[i].heading = atan2(arr[i + 1].easting - arr[i].easting,
                                   arr[i + 1].northing - arr[i].northing);
            if (arr[i].heading < 0) arr[i].heading += glm::twoPI;
            if (arr[i].heading >= glm::twoPI) arr[i].heading -= glm::twoPI;
        }

        arr[arr.length() - 1].heading = arr[arr.length() - 2].heading;

        cnt = arr.length();
        double distance;
        double spacing = 1.5;

        //add the first point of loop - it will be p1
        curList.append(arr[0]);
        //curList.append(arr[1]);

        for (int i = 0; i < cnt - 3; i++)
        {
            // add p1
            curList.append(arr[i + 1]);

            distance = glm::Distance(arr[i + 1], arr[i + 2]);

            if (distance > spacing)
            {
                int loopTimes = (int)(distance / spacing + 1);
                for (int j = 1; j < loopTimes; j++)
                {
                    Vec3 pos(glm::Catmull(j / (double)(loopTimes), arr[i], arr[i + 1], arr[i + 2], arr[i + 3]));
                    curList.append(pos);
                }
            }
        }

        curList.append(arr[cnt - 2]);
        curList.append(arr[cnt - 1]);

        //to calc heading based on next and previous points to give an average heading.
        cnt = curList.count();
        arr = QVector<Vec3>(curList);
        cnt--;
        curList.clear();

        curList.append(Vec3(arr[0]));

        //middle points
        for (int i = 1; i < cnt; i++)
        {
            Vec3 pt3(arr[i]);
            pt3.heading = atan2(arr[i + 1].easting - arr[i - 1].easting,
                                arr[i + 1].northing - arr[i - 1].northing);
            if (pt3.heading < 0) pt3.heading += glm::twoPI;
            curList.append(pt3);
        }

        int k = arr.length() - 1;
        Vec3 pt33(arr[k]);
        pt33.heading = atan2(arr[k].easting - arr[k - 1].easting,
                             arr[k].northing - arr[k - 1].northing);
        if (pt33.heading < 0) pt33.heading += glm::twoPI;
        curList.append(pt33);

        if (curveArr.count() == 0) return;
        if (bnd.bndList.count() > 0 && !(curveArr[numCurveLineSelected - 1].Name == "Boundary Curve"))
        {
            int ptCnt = curList.count() - 1;

            //end
            while (glm::IsPointInPolygon(bnd.bndList[0].fenceLineEar,curList[curList.count() - 1]))
            {
                for (int i = 1; i < 10; i++)
                {
                    Vec3 pt(curList[ptCnt]);
                    pt.easting += (sin(pt.heading) * i);
                    pt.northing += (cos(pt.heading) * i);
                    curList.append(pt);
                }
                ptCnt = curList.count() - 1;
            }

            //and the beginning
            pt33 = Vec3(curList[0]);

            while (glm::IsPointInPolygon(bnd.bndList[0].fenceLineEar,curList[0]))
            {
                pt33 = Vec3(curList[0]);

                for (int i = 1; i < 10; i++)
                {
                    Vec3 pt(pt33);
                    pt.easting -= (sin(pt.heading) * i);
                    pt.northing -= (cos(pt.heading) * i);
                    curList.insert(0, pt);
                }
            }
        }
    }

    lastSecond = secondsSinceStart;

}

void CABCurve::GetCurrentCurveLine(Vec3 pivot,
                                   Vec3 steer,
                                   double secondsSinceStart,
                                   bool isAutoSteerBtnOn,
                                   bool steerSwitchHigh,
                                   CVehicle &vehicle,
                                   const CBoundary &bnd,
                                   CYouTurn &yt,
                                   const CAHRS &ahrs,
                                   CGuidance &gyd,
                                   CNMEA &pn)
{
    double purePursuitGain = property_purePursuitIntegralGainAB;
    double wheelBase = property_setVehicle_wheelbase;
    double maxSteerAngle = property_setVehicle_maxSteerAngle;

    if (refList.count() < 5) return;

    //build new current ref line if required
    if (!isCurveValid || ((secondsSinceStart - lastSecond) > 0.66
                          && (!isAutoSteerBtnOn || steerSwitchHigh)))
        BuildCurveCurrentList(pivot,secondsSinceStart,vehicle,bnd,yt);

    double dist, dx, dz;
    double minDistA = 1000000, minDistB = 1000000;
    int ptCount = curList.count();

    if (ptCount > 0)
    {
        if (yt.isYouTurnTriggered && yt.DistanceFromYouTurnLine(vehicle,pn))//do the pure pursuit from youTurn
        {
            //now substitute what it thinks are AB line values with auto turn values
            steerAngleCu = yt.steerAngleYT;
            distanceFromCurrentLinePivot = yt.distanceFromCurrentLine;

            goalPointCu = yt.goalPointYT;
            radiusPointCu.easting = yt.radiusPointYT.easting;
            radiusPointCu.northing = yt.radiusPointYT.northing;
            ppRadiusCu = yt.ppRadiusYT;
            vehicle.modeActualXTE = (distanceFromCurrentLinePivot);
        }
        else if (property_setVehicle_isStanleyUsed)//Stanley
        {
            gyd.StanleyGuidanceCurve(pivot, steer, curList, isAutoSteerBtnOn, vehicle, *this, ahrs);
        }
        else// Pure Pursuit ------------------------------------------
        {
            minDistA = minDistB = glm::DOUBLE_MAX;
            //close call hit
            int cc = 0, dd;

            for (int j = 0; j < curList.count(); j += 10)
            {
                dist = glm::DistanceSquared(pivot, curList[j]);
                if (dist < minDistA)
                {
                    minDistA = dist;
                    cc = j;
                }
            }

            minDistA = glm::DOUBLE_MAX;

            dd = cc + 8; if (dd > curList.count() - 1) dd = curList.count();
            cc -= 8; if (cc < 0) cc = 0;

            //find the closest 2 points to current close call
            for (int j = cc; j < dd; j++)
            {
                dist = glm::DistanceSquared(pivot, curList[j]);
                if (dist < minDistA)
                {
                    minDistB = minDistA;
                    B = A;
                    minDistA = dist;
                    A = j;
                }
                else if (dist < minDistB)
                {
                    minDistB = dist;
                    B = j;
                }
            }

            //just need to make sure the points continue ascending or heading switches all over the place
            if (A > B) { C = A; A = B; B = C; }

            currentLocationIndex = A;

            //get the distance from currently active AB line

            if (A > curList.count() - 1 || B > curList.count() - 1)
                return;
            dx = curList[B].easting - curList[A].easting;
            dz = curList[B].northing - curList[A].northing;

            if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dz) < glm::DOUBLE_EPSILON) return;

            //abHeading = atan2(dz, dx);

            //how far from current AB Line is fix
            distanceFromCurrentLinePivot = ((dz * pivot.easting) - (dx * pivot.northing) + (curList[B].easting
                                                                                            * curList[A].northing) - (curList[B].northing * curList[A].easting))
                                           / sqrt((dz * dz) + (dx * dx));

            //integral slider is set to 0
            if (purePursuitGain != 0 && !vehicle.isReverse)
            {
                pivotDistanceError = distanceFromCurrentLinePivot * 0.2 + pivotDistanceError * 0.8;

                if (counter2++ > 4)
                {
                    pivotDerivative = pivotDistanceError - pivotDistanceErrorLast;
                    pivotDistanceErrorLast = pivotDistanceError;
                    counter2 = 0;
                    pivotDerivative *= 2;

                    //limit the derivative
                    //if (pivotDerivative > 0.03) pivotDerivative = 0.03;
                    //if (pivotDerivative < -0.03) pivotDerivative = -0.03;
                    //if (fabs(pivotDerivative) < 0.01) pivotDerivative = 0;
                }

                //pivotErrorTotal = pivotDistanceError + pivotDerivative;

                if (isAutoSteerBtnOn && vehicle.avgSpeed > 2.5 && fabs(pivotDerivative) < 0.1)
                {
                    //if over the line heading wrong way, rapidly decrease integral
                    if ((inty < 0 && distanceFromCurrentLinePivot < 0) || (inty > 0 && distanceFromCurrentLinePivot > 0))
                    {
                        inty += pivotDistanceError * purePursuitGain * -0.04;
                    }
                    else
                    {
                        if (fabs(distanceFromCurrentLinePivot) > 0.02)
                        {
                            inty += pivotDistanceError * purePursuitGain * -0.02;
                            if (inty > 0.2) inty = 0.2;
                            else if (inty < -0.2) inty = -0.2;
                        }
                    }
                }
                else inty *= 0.95;
            }
            else inty = 0;

            // ** Pure pursuit ** - calc point on ABLine closest to current position
            double U = (((pivot.easting - curList[A].easting) * dx)
                        + ((pivot.northing - curList[A].northing) * dz))
                       / ((dx * dx) + (dz * dz));

            rEastCu = curList[A].easting + (U * dx);
            rNorthCu = curList[A].northing + (U * dz);
            manualUturnHeading = curList[A].heading;

            //update base on autosteer settings and distance from line
            double goalPointDistance = vehicle.UpdateGoalPointDistance();

            bool ReverseHeading = vehicle.isReverse ? !isHeadingSameWay : isHeadingSameWay;

            int count = ReverseHeading ? 1 : -1;
            Vec3 start(rEastCu, rNorthCu, 0);
            double distSoFar = 0;

            for (int i = ReverseHeading ? B : A; i < ptCount && i >= 0; i += count)
            {
                // used for calculating the length squared of next segment.
                double tempDist = glm::Distance(start, curList[i]);

                //will we go too far?
                if ((tempDist + distSoFar) > goalPointDistance)
                {
                    double j = (goalPointDistance - distSoFar) / tempDist; // the remainder to yet travel

                    goalPointCu.easting = (((1 - j) * start.easting) + (j * curList[i].easting));
                    goalPointCu.northing = (((1 - j) * start.northing) + (j * curList[i].northing));
                    break;
                }
                else distSoFar += tempDist;
                start = curList[i];
            }

            if (isAutoSteerBtnOn && !vehicle.isReverse)
            {
                if (isHeadingSameWay)
                {
                    if (glm::Distance(goalPointCu, curList[(curList.count() - 1)]) < 0.5)
                    {
                        emit TimedMessage(2000,tr("Guidance Stopped"), tr("Past end of curve"));
                        emit stopAutoSteer();
                    }
                }
                else
                {
                    if (glm::Distance(goalPointCu, curList[0]) < 0.5)
                    {
                        emit TimedMessage(2000,tr("Guidance Stopped"), tr("Past end of curve"));
                        emit stopAutoSteer();
                    }
                }
            }

            //calc "D" the distance from pivot axle to lookahead point
            double goalPointDistanceSquared = glm::DistanceSquared(goalPointCu.northing, goalPointCu.easting, pivot.northing, pivot.easting);

            //calculate the the delta x in local coordinates and steering angle degrees based on wheelbase
            //double localHeading = glm::twoPI - mf.fixHeading;

            double localHeading;
            if (ReverseHeading) localHeading = glm::twoPI - vehicle.fixHeading + inty;
            else localHeading = glm::twoPI - vehicle.fixHeading - inty;

            ppRadiusCu = goalPointDistanceSquared / (2 * (((goalPointCu.easting - pivot.easting) * cos(localHeading)) + ((goalPointCu.northing - pivot.northing) * sin(localHeading))));

            steerAngleCu = glm::toDegrees(atan(2 * (((goalPointCu.easting - pivot.easting) * cos(localHeading))
                                                        + ((goalPointCu.northing - pivot.northing) * sin(localHeading))) * wheelBase / goalPointDistanceSquared));

            if (ahrs.imuRoll != 88888)
                steerAngleCu += ahrs.imuRoll * -(double)property_setAS_sideHillComp; /* gyd.sideHillCompFactor*/

            if (steerAngleCu < -maxSteerAngle) steerAngleCu = -maxSteerAngle;
            if (steerAngleCu > maxSteerAngle) steerAngleCu = maxSteerAngle;

            if (!isHeadingSameWay)
                distanceFromCurrentLinePivot *= -1.0;

            //used for acquire/hold mode
            vehicle.modeActualXTE = (distanceFromCurrentLinePivot);

            double steerHeadingError = (pivot.heading - curList[A].heading);
            //Fix the circular error
            if (steerHeadingError > M_PI)
                steerHeadingError -= M_PI;
            else if (steerHeadingError < -M_PI)
                steerHeadingError += M_PI;

            if (steerHeadingError > glm::PIBy2)
                steerHeadingError -= M_PI;
            else if (steerHeadingError < -glm::PIBy2)
                steerHeadingError += M_PI;

            vehicle.modeActualHeadingError = glm::toDegrees(steerHeadingError);

            //Convert to centimeters
            vehicle.guidanceLineDistanceOff = (short)glm::roundMidAwayFromZero(distanceFromCurrentLinePivot * 1000.0);
            vehicle.guidanceLineSteerAngle = (short)(steerAngleCu * 100);
        }
    }
    else
    {
        //invalid distance so tell AS module
        distanceFromCurrentLinePivot = 32000;
        vehicle.guidanceLineDistanceOff = 32000;
    }
}

void CABCurve::DrawCurve(QOpenGLFunctions *gl, const QMatrix4x4 &mvp,
                         bool isFontOn,
                         const CVehicle &vehicle,
                         CYouTurn &yt, const CCamera &camera)
{
    //double tool_toolWidth = property_setVehicle_toolWidth;
    //double tool_toolOverlap = property_setVehicle_toolOverlap;


    GLHelperColors gldraw_colors;
    GLHelperOneColor gldraw;
    ColorVertex cv;
    QColor color;

    double lineWidth = property_setDisplay_lineWidth;

    if (desList.count() > 0)
    {
        for (int h = 0; h < desList.count(); h++)
            gldraw.append(QVector3D(desList[h].easting, desList[h].northing, 0));

        gldraw.draw(gl,mvp,color.fromRgbF(0.95f, 0.42f, 0.750f),GL_LINE_STRIP,lineWidth);
    }

    int ptCount = refList.count();
    if (refList.count() == 0) return;

    cv.color = QVector4D(0.96, 0.2f, 0.2f, 1.0f);
    for (int h = 0; h < ptCount; h++) {
        cv.vertex = QVector3D(refList[h].easting, refList[h].northing, 0);
        gldraw_colors.append(cv);
    }

    if (!isCurveSet)
    {
        cv.color = QVector4D(0.930f, 0.0692f, 0.260f, 1.0f);
        ptCount--;
        cv.vertex = QVector3D(refList[ptCount].easting, refList[ptCount].northing, 0);
        gldraw_colors.append(cv);
        cv.vertex = QVector3D(vehicle.pivotAxlePos.easting, vehicle.pivotAxlePos.northing, 0);
        gldraw_colors.append(cv);
    }
    gldraw_colors.draw(gl,mvp,GL_LINES,lineWidth);

    if (isFontOn)
    {
        color.setRgbF(0.40f, 0.90f, 0.95f);
        drawText3D(camera, gl, mvp, refList[0].easting, refList[0].northing, "&A", 1.0, true, color);
        drawText3D(camera, gl, mvp, refList[refList.count() - 1].easting, refList[refList.count() - 1].northing, "&B", 1.0, true, color);
    }

    //just draw ref and smoothed line if smoothing window is open
    if (isSmoothWindowOpen)
    {
        if (smooList.count() == 0) return;

        gldraw.clear();

        for (int h = 0; h < smooList.count(); h++)
            gldraw.append(QVector3D(smooList[h].easting, smooList[h].northing, 0));

        gldraw.draw(gl,mvp,color,GL_LINES,lineWidth);
    }
    else //normal. Smoothing window is not open.
    {
        if (curList.count() > 0 && isCurveSet)
        {
            color.setRgbF(0.95f, 0.2f, 0.95f);
            gldraw.clear();
            for (int h = 0; h < curList.count(); h++)
                gldraw.append(QVector3D(curList[h].easting, curList[h].northing, 0));

            gldraw.draw(gl,mvp,color,GL_LINE_STRIP,lineWidth);

            //GL.PointSize(8);
            //GL.Color3(0.95f, 0.82f, 0.95f);
            //GL.Begin(PrimitiveType.Points);

            //GL.Vertex3(refList[refList.count() - 1].easting, refList[refList.count() - 1].northing, 0);
            //GL.Vertex3(refList[0].easting, refList[0].northing, 0);
            ////GL.Vertex3(refList[refList.count() - 1].easting, curList[curList.count() - 1].northing, 0);
            ////GL.Vertex3(refList[refList.count() - 1].easting, curList[curList.count() - 1].northing, 0);
            //GL.End();

            if (!(bool)property_setVehicle_isStanleyUsed && camera.camSetDistance > -200)
            {
                gldraw.clear();
                //Draw lookahead Point
                color.setRgbF(1.0f, 0.95f, 0.195f);

                gldraw.append(QVector3D(goalPointCu.easting, goalPointCu.northing, 0.0));
                gldraw.draw(gl,mvp,color,GL_POINTS,8.0f);
            }

            yt.DrawYouTurn(gl,mvp);
        }
    }
}

void CABCurve::BuildTram(CBoundary &bnd, CTram &tram)
{
    double tool_halfWidth = ((double)property_setVehicle_toolWidth - (double)property_setVehicle_toolOverlap) / 2.0;
    //if all or bnd only then make outer loop pass
    if (tram.generateMode != 1)
    {
        tram.BuildTramBnd(bnd);
    }
    else
    {
        tram.tramBndOuterArr.clear();
        tram.tramBndInnerArr.clear();
    }

    tram.tramList.clear();
    tram.tramArr->clear();

    if (tram.generateMode == 2) return;

    bool isBndExist = bnd.bndList.count() != 0;

    int refCount = refList.count();

    int cntr = 0;
    if (isBndExist)
    {
        if (tram.generateMode == 1)
            cntr = 0;
        else
            cntr = 1;
    }
    double widd = 0;

    for (int i = cntr; i <= (int)property_setTram_passes; i++)
    {
        tram.tramArr = QSharedPointer<QVector<Vec2>>(new QVector<Vec2>());
        tram.tramList.append(tram.tramArr);

        widd = (double)property_setTram_tramWidth * 0.5 - tool_halfWidth - (double)property_setVehicle_trackWidth*0.5;
        widd += ((double)property_setTram_tramWidth * i);

        double distSqAway = widd * widd * 0.999999;

        for (int j = 0; j < refCount; j += 1)
        {
            Vec2 point(
                (sin(glm::PIBy2 + refList[j].heading) *
                 widd) + refList[j].easting,
                (cos(glm::PIBy2 + refList[j].heading) *
                 widd) + refList[j].northing
                );

            bool Add = true;
            for (int t = 0; t < refCount; t++)
            {
                //distance check to be not too close to ref line
                double dist = ((point.easting - refList[t].easting) * (point.easting - refList[t].easting))
                              + ((point.northing - refList[t].northing) * (point.northing - refList[t].northing));
                if (dist < distSqAway)
                {
                    Add = false;
                    break;
                }
            }
            if (Add)
            {
                //a new point only every 2 meters
                double dist = (tram.tramArr->count() > 0 ? ((point.easting - (*tram.tramArr)[tram.tramArr->count() - 1].easting) * (point.easting - (*tram.tramArr)[tram.tramArr->count() - 1].easting))
                                                               + ((point.northing - (*tram.tramArr)[tram.tramArr->count() - 1].northing) * (point.northing - (*tram.tramArr)[tram.tramArr->count() - 1].northing)) : 3.0);
                if (dist > 2)
                {
                    //if inside the boundary, add
                    if (!isBndExist || glm::IsPointInPolygon(bnd.bndList[0].fenceLineEar,point))
                    {
                        tram.tramArr->append(point);
                    }
                }
            }
        }
    }

    for (int i = cntr; i <= (int)property_setTram_passes; i++)
    {
        tram.tramArr = QSharedPointer<QVector<Vec2>>(new QVector<Vec2>());
        tram.tramList.append(tram.tramArr);

        widd = (double)property_setTram_tramWidth * 0.5 - tool_halfWidth - (double)property_setVehicle_trackWidth*0.5;
        widd += ((double)property_setTram_tramWidth * i);

        double distSqAway = widd * widd * 0.999999;

        for (int j = 0; j < refCount; j += 1)
        {
            Vec2 point(
                sin(glm::PIBy2 + refList[j].heading) *
                        widd + refList[j].easting,
                cos(glm::PIBy2 + refList[j].heading) *
                        widd + refList[j].northing
                );

            bool Add = true;
            for (int t = 0; t < refCount; t++)
            {
                //distance check to be not too close to ref line
                double dist = ((point.easting - refList[t].easting) * (point.easting - refList[t].easting))
                              + ((point.northing - refList[t].northing) * (point.northing - refList[t].northing));
                if (dist < distSqAway)
                {
                    Add = false;
                    break;
                }
            }
            if (Add)
            {
                //a new point only every 2 meters
                double dist = tram.tramArr->count() > 0 ? ((point.easting - (*tram.tramArr)[tram.tramArr->count() - 1].easting) * (point.easting - (*tram.tramArr)[tram.tramArr->count() - 1].easting))
                                                              + ((point.northing - (*tram.tramArr)[tram.tramArr->count() - 1].northing) * (point.northing - (*tram.tramArr)[tram.tramArr->count() - 1].northing)) : 3.0;
                if (dist > 2)
                {
                    //if inside the boundary, add
                    if (!isBndExist || glm::IsPointInPolygon(bnd.bndList[0].fenceLineEar,point))
                    {
                        tram.tramArr->append(point);
                    }
                }
            }
        }
    }
}

void CABCurve::SmoothAB(int smPts)
{
    //count the reference list of original curve
    int cnt = refList.count();

    //just go back if not very long
    if (!isCurveSet || cnt < 200) return;

    //the temp array
    Vec3 arr[cnt];

    //read the points before and after the setpoint
    for (int s = 0; s < smPts / 2; s++)
    {
        arr[s].easting = refList[s].easting;
        arr[s].northing = refList[s].northing;
        arr[s].heading = refList[s].heading;
    }

    for (int s = cnt - (smPts / 2); s < cnt; s++)
    {
        arr[s].easting = refList[s].easting;
        arr[s].northing = refList[s].northing;
        arr[s].heading = refList[s].heading;
    }

    //average them - center weighted average
    for (int i = smPts / 2; i < cnt - (smPts / 2); i++)
    {
        for (int j = -smPts / 2; j < smPts / 2; j++)
        {
            arr[i].easting += refList[j + i].easting;
            arr[i].northing += refList[j + i].northing;
        }
        arr[i].easting /= smPts;
        arr[i].northing /= smPts;
        arr[i].heading = refList[i].heading;
    }

    //make a list to draw
    smooList.clear();
    for (int i = 0; i < cnt; i++)
    {
        smooList.append(arr[i]);
    }
}

void CABCurve::CalculateTurnHeadings()
{
    //to calc heading based on next and previous points to give an average heading.
    int cnt = refList.size();
    if (cnt > 0)
    {
        QVector<Vec3> arr = refList;
        cnt--;
        refList.clear();

        //middle points
        for (int i = 1; i < cnt; i++)
        {
            Vec3 pt3 = arr[i];
            pt3.heading = atan2(arr[i + 1].easting - arr[i - 1].easting, arr[i + 1].northing - arr[i - 1].northing);
            if (pt3.heading < 0) pt3.heading += glm::twoPI;
            refList.append(pt3);
        }
    }
}

void CABCurve::SaveSmoothAsRefList()
{
    //oops no smooth list generated
    int cnt = smooList.size();
    if (cnt == 0) return;

    //eek
    refList.clear();

    //copy to an array to calculate all the new headings
    QVector<Vec3> arr = smooList;

    //calculate new headings on smoothed line
    for (int i = 1; i < cnt - 1; i++)
    {
        arr[i].heading = atan2(arr[i + 1].easting - arr[i].easting, arr[i + 1].northing - arr[i].northing);
        if (arr[i].heading < 0) arr[i].heading += glm::twoPI;
        refList.append(arr[i]);
    }
}

void CABCurve::MoveABCurve(double dist)
{
    isCurveValid = false;
    lastSecond = 0;

    int cnt = refList.count();
    QVector<Vec3> arr = refList;
    refList.clear();

    moveDistance += isHeadingSameWay ? dist : -dist;

    for (int i = 0; i < cnt; i++)
    {
        arr[i].easting += cos(arr[i].heading) * (isHeadingSameWay ? dist : -dist);
        arr[i].northing -= sin(arr[i].heading) * (isHeadingSameWay ? dist : -dist);
        refList.append(arr[i]);
    }

}

bool CABCurve::PointOnLine(Vec3 pt1, Vec3 pt2, Vec3 pt)
{
    Vec2 r(0, 0);
    if (pt1.northing == pt2.northing && pt1.easting == pt2.easting) { pt1.northing -= 0.00001; }

    double U = ((pt.northing - pt1.northing) * (pt2.northing - pt1.northing)) + ((pt.easting - pt1.easting) * (pt2.easting - pt1.easting));

    double Udenom = pow(pt2.northing - pt1.northing, 2) + pow(pt2.easting - pt1.easting, 2);

    U /= Udenom;

    r.northing = pt1.northing + (U * (pt2.northing - pt1.northing));
    r.easting = pt1.easting + (U * (pt2.easting - pt1.easting));

    double minx, maxx, miny, maxy;

    minx = fmin(pt1.northing, pt2.northing);
    maxx = fmax(pt1.northing, pt2.northing);

    miny = fmin(pt1.easting, pt2.easting);
    maxy = fmax(pt1.easting, pt2.easting);
    return (r.northing >= minx && r.northing <= maxx && (r.easting >= miny && r.easting <= maxy));

}

void CABCurve::AddFirstLastPoints(QVector<Vec3> &xList,
                                  const CBoundary &bnd)
{
    int ptCnt = xList.count() - 1;
    Vec3 start(xList[0]);

    if (bnd.bndList.count() > 0)
    {
        //end
        while (glm::IsPointInPolygon(bnd.bndList[0].fenceLineEar, xList[xList.count() - 1]))
        {
            for (int i = 1; i < 10; i++)
            {
                Vec3 pt(xList[ptCnt]);
                pt.easting += (sin(pt.heading) * i);
                pt.northing += (cos(pt.heading) * i);
                xList.append(pt);
            }
            ptCnt = xList.count() - 1;
        }

        //and the beginning
        start = Vec3(xList[0]);

        while (glm::IsPointInPolygon(bnd.bndList[0].fenceLineEar,xList[0]))
        {
            for (int i = 1; i < 10; i++)
            {
                Vec3 pt(start);
                pt.easting -= (sin(pt.heading) * i);
                pt.northing -= (cos(pt.heading) * i);
                xList.insert(0, pt);
            }
            start = Vec3(xList[0]);
        }
    }
    else
    {
        for (int i = 1; i < 300; i++)
        {
            Vec3 pt(xList[ptCnt]);
            pt.easting += (sin(pt.heading) * i);
            pt.northing += (cos(pt.heading) * i);
            xList.append(pt);
        }

        //and the beginning
        start = Vec3(xList[0]);

        for (int i = 1; i < 300; i++)
        {
            Vec3 pt(start);
            pt.easting -= (sin(pt.heading) * i);
            pt.northing -= (cos(pt.heading) * i);
            xList.insert(0, pt);
        }
    }
}

void CABCurve::ResetCurveLine()
{
    curList.clear();
    refList.clear();
    isCurveSet = false;
}

