#include "cabcurve.h"
#include "glutils.h"
#include "glm.h"
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QVector>
#include "vec3.h"
#include "vec2.h"
#include "cvehicle.h"
#include "ctool.h"
#include "cyouturn.h"
#include "cboundary.h"
#include "ctram.h"
#include "ccamera.h"
#include "aogsettings.h"

CABCurve::CABCurve(QObject *parent) : QObject(parent)
{

}

void CABCurve::drawCurve(QOpenGLFunctions *gl, const QMatrix4x4 &mvp,
                         const CVehicle &vehicle, const CTool &tool,
                         CYouTurn &yt, CTram &tram, const CCamera &camera)
{
    USE_SETTINGS;

    GLHelperColors gldraw_colors;
    GLHelperOneColor gldraw;
    ColorVertex cv;

    if (!isEditing)
    {
        int ptCount = refList.size();
        if (refList.size() == 0) return;

        gl->glLineWidth(SETTINGS_DISPLAY_LINEWIDTH);

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
        }

        gldraw_colors.draw(gl, mvp, GL_LINES, SETTINGS_DISPLAY_LINEWIDTH);

        /*
         * TODO: implement GL font textures
        if (mf.font.isFontOn && refList.size() > 410)
        {
            GL.Color3(0.40f, 0.90f, 0.95f);
            mf.font.DrawText3D(refList[201].easting, refList[201].northing, "&A");
            mf.font.DrawText3D(refList[refList.size() - 200].easting, refList[refList.size() - 200].northing, "&B");
        }
        */

        //just draw ref and smoothed line if smoothing window is open
        if (isSmoothWindowOpen)
        {
            ptCount = smooList.size();
            if (smooList.size() == 0) return;

            gl->glLineWidth(SETTINGS_DISPLAY_LINEWIDTH);
            gldraw.clear();

            for (int h = 0; h < ptCount; h++)
                gldraw.append(QVector3D(smooList[h].easting, smooList[h].northing, 0));

            gldraw.draw(gl, mvp, QColor::fromRgbF(0.930f, 0.92f, 0.260f),
                        GL_LINES, SETTINGS_DISPLAY_LINEWIDTH);

        }
        else //normal. Smoothing window is not open.
        {
            ptCount = curList.size();
            if (ptCount > 0 && isCurveSet)
            {
                gldraw.clear();

                for (int h = 0; h < ptCount; h++)
                    gldraw.append(QVector3D(curList[h].easting, curList[h].northing, 0));

                gldraw.draw(gl, mvp, QColor::fromRgbF(0.95f, 0.2f, 0.95f),
                            GL_LINE_STRIP, 2.0f);

                if (SETTINGS_DISPLAY_ISPUREON && !vehicle.isStanleyUsed)
                {
                    if (ppRadiusCu < 100 && ppRadiusCu > -100)
                    {
                        const int numSegments = 100;
                        double theta = glm::twoPI / numSegments;
                        double c = cos(theta);//precalculate the sine and cosine
                        double s = sin(theta);
                        double x = ppRadiusCu;//we start at angle = 0
                        double y = 0;

                        gl->glLineWidth(1);
                        gldraw.clear();
                        for (int ii = 0; ii < numSegments; ii++)
                        {
                            gldraw.append(QVector3D(x + radiusPointCu.easting, y + radiusPointCu.northing, 0));//output vertex
                            double t = x;//apply the rotation matrix
                            x = (c * x) - (s * y);
                            y = (s * t) + (c * y);
                        }
                        gldraw.draw(gl, mvp, QColor::fromRgbF(0.95f, 0.30f, 0.950f),
                                    GL_LINE_LOOP, 1.0f);
                    }

                    //Draw lookahead Point
                    gldraw.clear();
                    gldraw.append(QVector3D(goalPointCu.easting, goalPointCu.northing, 0.0));
                    gldraw.draw(gl, mvp, QColor::fromRgbF(1.0f, 0.5f, 0.95f),
                                GL_POINTS, 4.0f);
                }

                yt.drawYouTurn(gl, mvp);

                if (yt.isYouTurnTriggered)
                {
                    ptCount = yt.ytList.size();
                    if (ptCount > 0)
                    {
                        gldraw.clear();
                        for (int i = 0; i < ptCount; i++)
                        {
                            gldraw.append(QVector3D(yt.ytList[i].easting, yt.ytList[i].northing, 0));
                        }
                        gldraw.draw(gl, mvp, QColor::fromRgbF(0.95f, 0.95f, 0.25f),
                                    GL_POINTS, 4.0f);
                    }
                    //GL.Color3(0.95f, 0.05f, 0.05f);
                }
            }
        }
    }

    if (isEditing)
    {
        int ptCount = refList.size();
        if (refList.size() == 0) return;

        gl->glLineWidth(SETTINGS_DISPLAY_LINEWIDTH);
        gldraw.clear();
        for (int h = 0; h < ptCount; h++)
            gldraw.append(QVector3D(refList[h].easting, refList[h].northing, 0));

        gldraw.draw(gl, mvp, QColor::fromRgbF(0.930f, 0.2f, 0.260f),
                    GL_LINES, SETTINGS_DISPLAY_LINEWIDTH);

        //current line
        if (curList.size() > 0 && isCurveSet)
        {
            ptCount = curList.size();
            gldraw.clear();

            for (int h = 0; h < ptCount; h++)
                gldraw.append(QVector3D(curList[h].easting, curList[h].northing, 0));

            gldraw.draw(gl, mvp, QColor::fromRgbF(0.95f, 0.2f, 0.950f),
                        GL_LINE_STRIP, SETTINGS_DISPLAY_LINEWIDTH);
        }


        if (camera.camSetDistance > -200)
        {
            double toolWidth2 = tool.toolWidth - tool.toolOverlap;
            double cosHeading2 = cos(-aveLineHeading);
            double sinHeading2 = sin(-aveLineHeading);

            gldraw.clear();
            for (int i = 1; i <= 6; i++)
            {
                for (int h = 0; h < ptCount; h++)
                    gldraw.append(QVector3D((cosHeading2 * toolWidth2) + refList[h].easting,
                                            (sinHeading2 * toolWidth2) + refList[h].northing, 0));
                toolWidth2 = toolWidth2 + tool.toolWidth - tool.toolOverlap;
            }
            gldraw.draw(gl, mvp, QColor::fromRgbF(0.8f, 0.3f, 0.2f),
                        GL_POINTS, 2.0f);

        }
    }

    if (tram.displayMode == 1 || tram.displayMode == 2) drawTram(gl, mvp);
    if (tram.displayMode == 1 || tram.displayMode == 3) tram.drawTramBnd(gl, mvp);

}

void CABCurve::drawTram(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    USE_SETTINGS;

    GLHelperColors gldraw_colors;
    ColorVertex cv;

    cv.color = QVector4D(0.8630f, 0.93692f, 0.3260f, 0.22);

    for (int i = 0; i < tramList.size(); i++)
    {
        gldraw_colors.clear();
        for (int h = 0; h < tramList[i].size(); h++) {
            cv.vertex = QVector3D(tramList[i][h].easting, tramList[i][h].northing, 0);
            gldraw_colors.append(cv);
        }

        gldraw_colors.draw(gl, mvp, GL_TRIANGLE_STRIP, SETTINGS_DISPLAY_LINEWIDTH);
    }

    /*TODO implement font drawing
    if (mf.font.isFontOn)
    {
        for (int i = 0; i < tramList.size(); i++)
        {
            int middle = 0;
            GL.Color4(0.8630f, 0.93692f, 0.8260f, 0.752);
            if (tramList[i].size() > 0)
            {
                middle = tramList[i].size() - 1;
                mf.font.DrawText3D(tramList[i][middle].easting, tramList[i][middle].northing, (i + 1).ToString());
                mf.font.DrawText3D(tramList[i][0].easting, tramList[i][0].northing, (i + 1).ToString());
            }
        }
    }
    */

}

void CABCurve::buildTram(CBoundary &bnd, CTram &tram)
{
    tram.buildTramBnd(bnd);
    tramList.clear();

    Vec2 tramLineP1;

    bool isBndExist = bnd.bndArr.size() != 0;

    double pass = 0.5;
    double headingCalc = aveLineHeading + glm::PIBy2;

    double hsin = sin(headingCalc);
    double hcos = cos(headingCalc);

    for (int i = 0; i < tram.passes; i++)
    {
        tramArr.clear(); //appending to the tramList copies the list anyway.
        //TODO: is my logic okay about appending below
        for (int j = 0; j < refList.size(); j += 4)
        {
            tramLineP1.easting = (hsin * ((tram.tramWidth * (pass + i)) - tram.halfWheelTrack + tram.abOffset)) + refList[j].easting;
            tramLineP1.northing = (hcos * ((tram.tramWidth * (pass + i)) -tram.halfWheelTrack + tram.abOffset)) + refList[j].northing;

            if (isBndExist)
            {
                if (bnd.bndArr[0].isPointInsideBoundary(tramLineP1))
                {
                    tramArr.append(tramLineP1);

                    tramLineP1.easting =  (hsin * tram.wheelTrack) + tramLineP1.easting;
                    tramLineP1.northing = (hcos * tram.wheelTrack) + tramLineP1.northing;
                    tramArr.append(tramLineP1);
                }
            }
            else
            {
                tramArr.append(tramLineP1);

                tramLineP1.easting =  (hsin * tram.wheelTrack) + tramLineP1.easting;
                tramLineP1.northing = (hcos * tram.wheelTrack) + tramLineP1.northing;
                tramArr.append(tramLineP1);
            }
        }
        tramList.append(tramArr); //TODO: not sure about this translation, might have to play with SharedPointers
    }
}

void CABCurve::smoothAB(int smPts)
{
    //count the reference list of original curve
    int cnt = refList.size();

    //just go back if not very long
    if (!isCurveSet || cnt < 400) return;

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

void CABCurve::calculateTurnHeadings()
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

void CABCurve::saveSmoothAsRefList()
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

void CABCurve::getCurrentCurveLine(Vec3 pivot, Vec3 steer,
                                   CVehicle &vehicle, CYouTurn &yt,
                                   const CTool &tool, CNMEA &pn,
                                   double speed)
{
    int ptCount = refList.size();
    int ptCnt = ptCount - 1;
    if (ptCount < 5) return;

    boxA.easting = pivot.easting - (sin(aveLineHeading + glm::PIBy2) * 2000);
    boxA.northing = pivot.northing - (cos(aveLineHeading + glm::PIBy2) * 2000);

    boxB.easting = pivot.easting + (sin(aveLineHeading + glm::PIBy2) * 2000);
    boxB.northing = pivot.northing + (cos(aveLineHeading + glm::PIBy2) * 2000);

    boxC.easting = boxB.easting + (sin(aveLineHeading) * 1.0);
    boxC.northing = boxB.northing + (cos(aveLineHeading) * 1.0);

    boxD.easting = boxA.easting + (sin(aveLineHeading) * 1.0);
    boxD.northing = boxA.northing + (cos(aveLineHeading) * 1.0);

    boxA.easting -= (sin(aveLineHeading) * 1.0);
    boxA.northing -= (cos(aveLineHeading) * 1.0);

    boxB.easting -= (sin(aveLineHeading) * 1.0);
    boxB.northing -= (cos(aveLineHeading) * 1.0);

    //determine if point are in frustum box
    for (int s = 0; s < ptCnt; s++)
    {
        if ((((boxB.easting - boxA.easting) * (refList[s].northing - boxA.northing))
                - ((boxB.northing - boxA.northing) * (refList[s].easting - boxA.easting))) < 0) { continue; }

        if ((((boxD.easting - boxC.easting) * (refList[s].northing - boxC.northing))
                - ((boxD.northing - boxC.northing) * (refList[s].easting - boxC.easting))) < 0) { continue; }

        closestRefIndex = s;
        break;
    }

    double dist = ((pivot.easting - refList[closestRefIndex].easting) * (pivot.easting - refList[closestRefIndex].easting))
                    + ((pivot.northing - refList[closestRefIndex].northing) * (pivot.northing - refList[closestRefIndex].northing));

    //determine closest point
    double minDistance = sqrt(dist);

    //grab the heading at the closest point
    refHeading = refList[closestRefIndex].heading;

    //which side of the patch are we on is next
    //calculate endpoints of reference line based on closest point
    refPoint1.easting = refList[closestRefIndex].easting - (sin(refHeading) * 50.0);
    refPoint1.northing = refList[closestRefIndex].northing - (cos(refHeading) * 50.0);

    refPoint2.easting = refList[closestRefIndex].easting + (sin(refHeading) * 50.0);
    refPoint2.northing = refList[closestRefIndex].northing + (cos(refHeading) * 50.0);

    //x2-x1
    double dx = refPoint2.easting - refPoint1.easting;
    //z2-z1
    double dz = refPoint2.northing - refPoint1.northing;

    //how far are we away from the reference line at 90 degrees - 2D cross product and distance
    double distanceFromRefLine = ((dz * pivot.easting) - (dx * pivot.northing) + (refPoint2.easting
                            * refPoint1.northing) - (refPoint2.northing * refPoint1.easting));
    //   / sqrt((dz * dz) + (dx * dx));
    //are we going same direction as stripList was created?
    isSameWay = M_PI - fabs(fabs(pivot.heading - refHeading) - M_PI) < glm::PIBy2;
    deltaOfRefAndAveHeadings = M_PI - fabs(fabs(aveLineHeading - refHeading) - M_PI);
    deltaOfRefAndAveHeadings = cos(deltaOfRefAndAveHeadings);

    //add or subtract pi by 2 depending on which side of ref line
    double piSide;

    //sign of distance determines which side of line we are on
    if (distanceFromRefLine > 0) piSide = glm::PIBy2;
    else piSide = -glm::PIBy2;
    double widthMinusOverlap;
    //move the ABLine over based on the overlap amount set in vehicle
    if (tool.toolOffset != 0) {
        widthMinusOverlap = tool.toolWidth / 2 - tool.toolOverlap;
    }
    else
    {
         widthMinusOverlap = tool.toolWidth  - tool.toolOverlap;
    }


    howManyPathsAway = glm::roundAwayFromZero(minDistance / widthMinusOverlap);

    curveNumber = howManyPathsAway;
    if (distanceFromRefLine < 0) curveNumber = -curveNumber;

    //double toolOffset = tool.toolOffset;

    //build the current line
    curList.clear();
    for (int i = 0; i < ptCount; i++)
    {
        Vec3 point = Vec3(
            refList[i].easting + (sin(piSide + aveLineHeading) * ((widthMinusOverlap * howManyPathsAway))),
            refList[i].northing + (cos(piSide + aveLineHeading) * ((widthMinusOverlap * howManyPathsAway))),
            refList[i].heading);
        curList.append(point);
    }

    double minDistA = 1000000, minDistB = 1000000;

    ptCount = curList.size();

    if (ptCount > 0)
    {
        if (vehicle.isStanleyUsed)
        {
            //find the closest 2 points to current fix
            for (int t = 0; t < ptCount; t++)
            {
                dist = ((steer.easting - curList[t].easting) * (steer.easting - curList[t].easting))
                                + ((steer.northing - curList[t].northing) * (steer.northing - curList[t].northing));
                if (dist < minDistA)
                {
                    minDistB = minDistA;
                    B = A;
                    minDistA = dist;
                    A = t;
                }
                else if (dist < minDistB)
                {
                    minDistB = dist;
                    B = t;
                }
            }

            //just need to make sure the points continue ascending or heading switches all over the place
            if (A > B) { C = A; A = B; B = C; }

            currentLocationIndex = A;

            //get the distance from currently active AB line
            dx = curList[B].easting - curList[A].easting;
            dz = curList[B].northing - curList[A].northing;

            if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dz) < glm::DOUBLE_EPSILON) return;

            //abHeading = atan2(dz, dx);
            double abHeading = curList[A].heading;

            //how far from current AB Line is fix
            distanceFromCurrentLine = ((dz * steer.easting) - (dx * steer.northing) + (curList[B].easting
                        * curList[A].northing) - (curList[B].northing * curList[A].easting))
                            / sqrt((dz * dz) + (dx * dx));

            //are we on the right side or not
            isOnRightSideCurrentLine = distanceFromCurrentLine > 0;

            //absolute the distance
            distanceFromCurrentLine = fabs(distanceFromCurrentLine);

            //Subtract the two headings, if > 1.57 its going the opposite heading as refAB
            double abFixHeadingDelta = (fabs(vehicle.fixHeading - abHeading));
            if (abFixHeadingDelta >= M_PI) abFixHeadingDelta = fabs(abFixHeadingDelta - glm::twoPI);
            isABSameAsVehicleHeading = abFixHeadingDelta < glm::PIBy2;

            // calc point on ABLine closest to current position
            double U = (((steer.easting - curList[A].easting) * dx)
                        + ((steer.northing - curList[A].northing) * dz))
                        / ((dx * dx) + (dz * dz));

            rEastCu = curList[A].easting + (U * dx);
            rNorthCu = curList[A].northing + (U * dz);

            //distance is negative if on left, positive if on right
            if (isABSameAsVehicleHeading)
            {
                if (!isOnRightSideCurrentLine)
                {
                    distanceFromCurrentLine *= -1.0;
                }
                abFixHeadingDelta = (steer.heading - abHeading);
            }

            //opposite way so right is left
            else
            {
                if (isOnRightSideCurrentLine)
                {
                    distanceFromCurrentLine *= -1.0;
                }
                abFixHeadingDelta = (steer.heading - abHeading + M_PI);
            }

            //Fix the circular error
            if (abFixHeadingDelta > M_PI) abFixHeadingDelta -= M_PI;
            else if (abFixHeadingDelta < M_PI) abFixHeadingDelta += M_PI;

            if (abFixHeadingDelta > glm::PIBy2) abFixHeadingDelta -= M_PI;
            else if (abFixHeadingDelta < -glm::PIBy2) abFixHeadingDelta += M_PI;

            abFixHeadingDelta *= vehicle.stanleyHeadingErrorGain;
            if (abFixHeadingDelta > 0.74) abFixHeadingDelta = 0.74;
            if (abFixHeadingDelta < -0.74) abFixHeadingDelta = -0.74;

            steerAngleCu = atan((distanceFromCurrentLine * vehicle.stanleyGain) / ((speed * 0.277777) + 1));

            if (steerAngleCu > 0.74) steerAngleCu = 0.74;
            if (steerAngleCu < -0.74) steerAngleCu = -0.74;

            steerAngleCu = glm::toDegrees((steerAngleCu + abFixHeadingDelta) * -1.0);

            if (steerAngleCu < -vehicle.maxSteerAngle) steerAngleCu = -vehicle.maxSteerAngle;
            if (steerAngleCu > vehicle.maxSteerAngle) steerAngleCu = vehicle.maxSteerAngle;

            //Convert to millimeters
            distanceFromCurrentLine = glm::roundAwayFromZero(distanceFromCurrentLine * 1000.0);
        }
        else
        {
            //find the closest 2 points to current fix
            for (int t = 0; t < ptCount; t++)
            {
                dist = ((pivot.easting - curList[t].easting) * (pivot.easting - curList[t].easting))
                                + ((pivot.northing - curList[t].northing) * (pivot.northing - curList[t].northing));
                if (dist < minDistA)
                {
                    minDistB = minDistA;
                    B = A;
                    minDistA = dist;
                    A = t;
                }
                else if (dist < minDistB)
                {
                    minDistB = dist;
                    B = t;
                }
            }

            //just need to make sure the points continue ascending or heading switches all over the place
            if (A > B) { C = A; A = B; B = C; }

            currentLocationIndex = A;

            //get the distance from currently active AB line
            dx = curList[B].easting - curList[A].easting;
            dz = curList[B].northing - curList[A].northing;

            if (fabs(dx) < glm::DOUBLE_EPSILON && fabs(dz) < glm::DOUBLE_EPSILON) return;

            //abHeading = atan2(dz, dx);

            //how far from current AB Line is fix
            distanceFromCurrentLine = ((dz * pivot.easting) - (dx * pivot.northing) + (curList[B].easting
                        * curList[A].northing) - (curList[B].northing * curList[A].easting))
                            / sqrt((dz * dz) + (dx * dx));

            //are we on the right side or not
            isOnRightSideCurrentLine = distanceFromCurrentLine > 0;

            //absolute the distance
            distanceFromCurrentLine = fabs(distanceFromCurrentLine);

            // ** Pure pursuit ** - calc point on ABLine closest to current position
            double U = (((pivot.easting - curList[A].easting) * dx)
                        + ((pivot.northing - curList[A].northing) * dz))
                        / ((dx * dx) + (dz * dz));

            rEastCu = curList[A].easting + (U * dx);
            rNorthCu = curList[A].northing + (U * dz);

            //double minx, maxx, miny, maxy;

            //minx = fmin(curList[A].northing, curList[B].northing);
            //maxx = fmax(curList[A].northing, curList[B].northing);

            //miny = fmin(curList[A].easting, curList[B].easting);
            //maxy = fmax(curList[A].easting, curList[B].easting);

            //isValid = rNorthCu >= minx && rNorthCu <= maxx && (rEastCu >= miny && rEastCu <= maxy);

            //if (!isValid)
            //{
            //    //invalid distance so tell AS module
            //    distanceFromCurrentLine = 32000;
            //    mf.guidanceLineDistanceOff = 32000;
            //    return;
            //}

            //used for accumulating distance to find goal point
            double distSoFar;

            //update base on autosteer settings and distance from line
            double goalPointDistance = vehicle.updateGoalPointDistance(pn, distanceFromCurrentLine);
            //mf.lookaheadActual = goalPointDistance; //already updated in above updateGoalPointDistance call

            // used for calculating the length squared of next segment.
            double tempDist = 0.0;

            if (!isSameWay)
            {
                //counting down
                isABSameAsVehicleHeading = false;
                distSoFar = glm::distance(curList[A], rEastCu, rNorthCu);
                //Is this segment long enough to contain the full lookahead distance?
                if (distSoFar > goalPointDistance)
                {
                    //treat current segment like an AB Line
                    goalPointCu.easting = rEastCu - (sin(curList[A].heading) * goalPointDistance);
                    goalPointCu.northing = rNorthCu - (cos(curList[A].heading) * goalPointDistance);
                }

                //multiple segments required
                else
                {
                    //cycle thru segments and keep adding lengths. check if start and break if so.
                    while (A > 0)
                    {
                        B--; A--;
                        tempDist = glm::distance(curList[B], curList[A]);

                        //will we go too far?
                        if ((tempDist + distSoFar) > goalPointDistance) break; //tempDist contains the full length of next segment
                        else distSoFar += tempDist;
                    }

                    double t = (goalPointDistance - distSoFar); // the remainder to yet travel
                    t /= tempDist;

                    goalPointCu.easting = (((1 - t) * curList[B].easting) + (t * curList[A].easting));
                    goalPointCu.northing = (((1 - t) * curList[B].northing) + (t * curList[A].northing));
                }
            }
            else
            {
                //counting up
                isABSameAsVehicleHeading = true;
                distSoFar = glm::distance(curList[B], rEastCu, rNorthCu);

                //Is this segment long enough to contain the full lookahead distance?
                if (distSoFar > goalPointDistance)
                {
                    //treat current segment like an AB Line
                    goalPointCu.easting = rEastCu + (sin(curList[A].heading) * goalPointDistance);
                    goalPointCu.northing = rNorthCu + (cos(curList[A].heading) * goalPointDistance);
                }

                //multiple segments required
                else
                {
                    //cycle thru segments and keep adding lengths. check if end and break if so.
                    // ReSharper disable once LoopVariableIsNeverChangedInsideLoop
                    while (B < ptCount - 1)
                    {
                        B++; A++;
                        tempDist = glm::distance(curList[B], curList[A]);

                        //will we go too far?
                        if ((tempDist + distSoFar) > goalPointDistance)
                        {
                            //A--; B--;
                            break; //tempDist contains the full length of next segment
                        }

                        distSoFar += tempDist;
                    }

                    //xt = (((1 - t) * x0 + t * x1)
                    //yt = ((1 - t) * y0 + t * y1))

                    double t = (goalPointDistance - distSoFar); // the remainder to yet travel
                    t /= tempDist;

                    goalPointCu.easting = (((1 - t) * curList[A].easting) + (t * curList[B].easting));
                    goalPointCu.northing = (((1 - t) * curList[A].northing) + (t * curList[B].northing));
                }
            }

            //calc "D" the distance from pivot axle to lookahead point
            double goalPointDistanceSquared = glm::distanceSquared(goalPointCu.northing, goalPointCu.easting, pivot.northing, pivot.easting);

            //calculate the the delta x in local coordinates and steering angle degrees based on wheelbase
            double localHeading = glm::twoPI - vehicle.fixHeading;
            ppRadiusCu = goalPointDistanceSquared / (2 * (((goalPointCu.easting - pivot.easting) * cos(localHeading)) + ((goalPointCu.northing - pivot.northing) * sin(localHeading))));

            steerAngleCu = glm::toDegrees(atan(2 * (((goalPointCu.easting - pivot.easting) * cos(localHeading))
                + ((goalPointCu.northing - pivot.northing) * sin(localHeading))) * vehicle.wheelbase / goalPointDistanceSquared));

            if (steerAngleCu < -vehicle.maxSteerAngle) steerAngleCu = -vehicle.maxSteerAngle;
            if (steerAngleCu > vehicle.maxSteerAngle) steerAngleCu = vehicle.maxSteerAngle;

            if (ppRadiusCu < -500) ppRadiusCu = -500;
            if (ppRadiusCu > 500) ppRadiusCu = 500;

            radiusPointCu.easting = pivot.easting + (ppRadiusCu * cos(localHeading));
            radiusPointCu.northing = pivot.northing + (ppRadiusCu * sin(localHeading));

            //angular velocity in rads/sec  = 2PI * m/sec * radians/meters
            double angVel = glm::twoPI * 0.277777 * speed * (tan(glm::toRadians(steerAngleCu))) / vehicle.wheelbase;

            //clamp the steering angle to not exceed safe angular velocity
            if (fabs(angVel) > vehicle.maxAngularVelocity)
            {
                steerAngleCu = glm::toDegrees(steerAngleCu > 0 ?
                        (atan((vehicle.wheelbase * vehicle.maxAngularVelocity) / (glm::twoPI * speed * 0.277777)))
                    : (atan((vehicle.wheelbase * -vehicle.maxAngularVelocity) / (glm::twoPI * speed * 0.277777))));
            }
            //Convert to centimeters
            distanceFromCurrentLine = glm::roundAwayFromZero(distanceFromCurrentLine * 1000.0);

            //distance is negative if on left, positive if on right
            //if you're going the opposite direction left is right and right is left
            //double temp;
            if (isABSameAsVehicleHeading)
            {
                //temp = (abHeading);
                if (!isOnRightSideCurrentLine) distanceFromCurrentLine *= -1.0;
            }

            //opposite way so right is left
            else
            {
                //temp = (abHeading - M_PI);
                //if (temp < 0) temp = (temp + glm::twoPI);
                //temp = glm::toDegrees(temp);
                if (isOnRightSideCurrentLine) distanceFromCurrentLine *= -1.0;
            }
        }

        vehicle.guidanceLineDistanceOff = (int)distanceFromCurrentLine;
        //mf.distanceDisplay?
        vehicle.guidanceLineSteerAngle = (int)(steerAngleCu * 100);

        if (yt.isYouTurnTriggered)
        {
            //do the pure pursuit from youTurn
            yt.distanceFromYouTurnLine(vehicle, pn);
            emit doSequence(vehicle);
            //mf.seq.DoSequenceEvent();

            //now substitute what it thinks are AB line values with auto turn values
            steerAngleCu = yt.steerAngleYT;
            distanceFromCurrentLine = yt.distanceFromCurrentLine;

            goalPointCu = yt.goalPointYT;
            radiusPointCu.easting = yt.radiusPointYT.easting;
            radiusPointCu.northing = yt.radiusPointYT.northing;
            ppRadiusCu = yt.ppRadiusYT;
        }
    }
    else
    {
        //invalid distance so tell AS module
        distanceFromCurrentLine = 32000;
        vehicle.guidanceLineDistanceOff = 32000;
    }
}

void CABCurve::snapABCurve()
{
    double headingAt90;

    //calculate the heading 90 degrees to ref ABLine heading
    if (isOnRightSideCurrentLine)
    {
        headingAt90 = glm::PIBy2;
    }
    else
    {
        headingAt90 = -glm::PIBy2;
    }

    if (isABSameAsVehicleHeading)
    {
        moveDistance += distanceFromCurrentLine * 0.001;
    }
    else
    {
        moveDistance -= distanceFromCurrentLine * 0.001;
    }


    int cnt = refList.size();
    QVector<Vec3> arr = refList;
    refList.clear();

    for (int i = 0; i < cnt; i++)
    {
        arr[i].easting = (sin(headingAt90 + arr[i].heading) * fabs(distanceFromCurrentLine) * 0.001) + arr[i].easting;
        arr[i].northing = (cos(headingAt90 + arr[i].heading) * fabs(distanceFromCurrentLine) * 0.001) + arr[i].northing;
        refList.append(arr[i]);
    }
}

void CABCurve::moveABCurve(double dist)
{
    double headingAt90;

    //calculate the heading 90 degrees to ref ABLine heading

    if (isABSameAsVehicleHeading)
    {
        headingAt90 = glm::PIBy2;
        moveDistance += dist;
    }
    else
    {
        headingAt90 = -glm::PIBy2;
        moveDistance -= dist;
    }

    int cnt = refList.size();
    QVector<Vec3> arr = refList;
    refList.clear();

    for (int i = 0; i < cnt; i++)
    {
        arr[i].easting = (sin(headingAt90 + arr[i].heading) * dist) + arr[i].easting;
        arr[i].northing = (cos(headingAt90 + arr[i].heading) * dist) + arr[i].northing;
        refList.append(arr[i]);
    }

}

bool CABCurve::pointOnLine(Vec3 pt1, Vec3 pt2, Vec3 pt)
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
    return r.northing >= minx && r.northing <= maxx && (r.easting >= miny && r.easting <= maxy);

}

void CABCurve::addFirstLastPoints()
{
    int ptCnt = refList.size() - 1;
    for (int i = 1; i < 200; i++)
    {
        Vec3 pt(refList[ptCnt]);
        pt.easting += (sin(pt.heading) * i);
        pt.northing += (cos(pt.heading) * i);
        refList.append(pt);
    }

    //and the beginning
    Vec3 start(refList[0]);
    for (int i = 1; i < 200; i++)
    {
        Vec3 pt(start);
        pt.easting -= (sin(pt.heading) * i);
        pt.northing -= (cos(pt.heading) * i);
        refList.insert(0, pt);
    }

}

void CABCurve::resetCurveLine()
{
    curList.clear();
    refList.clear();
    isCurveSet = false;
    isOkToAddPoints = false;
    closestRefIndex = 0;

}

