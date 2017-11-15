#include "ccontour.h"
#include "cnmea.h"
#include "cvehicle.h"
#include "cvec.h"
#include <math.h>
#include <limits>
#include <QOpenGLContext>
#include "glm.h"
#include "cvehicle.h"
#include <QSettings>
#include "glutils.h"

const double DOUBLE_EPSILON=std::numeric_limits<double>::epsilon();

CContour::CContour(CVehicle *v)
    : vehicle(v), ptList(new QVector<Vec4>)
{

}

//start stop and add points to list
void CContour::startContourLine() {
    qDebug() << "starting contour line.";
    isContourOn = true;
    if (!ptList.isNull() && ptList->size() == 1)
    {
        //reuse ptList
        ptList->clear();
    }
    else
    {
        //make new ptList
        ptList = QSharedPointer<QVector<Vec4>>(new QVector<Vec4>);
        stripList.append(ptList);
    }
    //grab a copy from main
    pivotAxlePosCT = vehicle->pivotAxlePos;

    pivotAxlePosCT.easting -= (sin(vehicle->fixHeading) * 5.0);
    pivotAxlePosCT.northing -= (cos(vehicle->fixHeading) * 5.0);

    Vec4 point = Vec4(pivotAxlePosCT.easting, vehicle->fixHeading,
                      pivotAxlePosCT.northing, vehicle->altitude);
    ptList->append(point);
}

//Add current position to stripList
void CContour::addPoint() {
    Vec4 point = Vec4(vehicle->pivotAxlePos.easting, vehicle->fixHeading,
                      vehicle->pivotAxlePos.northing, vehicle->altitude);
    ptList->append(point);
}

//End the strip
void CContour::stopContourLine()
{
    qDebug() << ptList->size() << "Stopping contour line.";
    //make sure its long enough to bother
    if (ptList->size() > 10)
    {
        //grab a copy from main
        pivotAxlePosCT = vehicle->pivotAxlePos;

        pivotAxlePosCT.easting += (sin(vehicle->fixHeading) * 5.0);
        pivotAxlePosCT.northing += (cos(vehicle->fixHeading) * 5.0);

        Vec4 point = Vec4(pivotAxlePosCT.easting, vehicle->fixHeading,
                          pivotAxlePosCT.northing, vehicle->altitude);
        ptList->append(point);

        //add the point list to the save list for appending to contour file
        contourSaveList.append(ptList);
    }

    //delete ptList
    else
    {
        ptList->clear();
        stripList.pop_back(); //remove the last list in the list
    }

    //turn it off
    isContourOn = false;
}

//determine closest point on applied
void CContour::buildContourGuidanceLine(double eastFix, double northFix)
{
    //build a frustum box ahead of fix to find adjacent paths and points
    //double startX = eastFix + sin(mf->fixHeading)* 0;
    //double startY = northFix + cos(mf->fixHeading) * 0;

    boxA.easting = eastFix - (sin(vehicle->fixHeading + PIBy2) *  2.0 * vehicle->toolWidth);
    boxA.northing = northFix - (cos(vehicle->fixHeading + PIBy2) * 2.0 * vehicle->toolWidth);

    boxB.easting = eastFix + (sin(vehicle->fixHeading + PIBy2) *  2.0 * vehicle->toolWidth);
    boxB.northing = northFix + (cos(vehicle->fixHeading + PIBy2) * 2.0 * vehicle->toolWidth);

    boxC.easting = boxB.easting + (sin(vehicle->fixHeading) * 13.0);
    boxC.northing = boxB.northing + (cos(vehicle->fixHeading) * 13.0);

    boxD.easting = boxA.easting + (sin(vehicle->fixHeading) * 13.0);
    boxD.northing = boxA.northing + (cos(vehicle->fixHeading) * 13.0);

    conList.clear();
    ctList.clear();
    int ptCount;

    //check if no strips yet, return
    int stripCount = stripList.size();
    if (stripCount == 0) return;

    CVec pointC;

    //determine if points are in frustum box
    for (int s = 0; s < stripCount; s++)
    {
        ptCount = stripList[s]->size();
        for (int p = 0; p < ptCount; p++)
        {
            if ((((boxB.easting - boxA.easting) * ((*stripList[s])[p].z - boxA.northing))
                    - ((boxB.northing - boxA.northing) * ((*stripList[s])[p].x - boxA.easting))) < 0) { continue; }

            if ((((boxD.easting - boxC.easting) * ((*stripList[s])[p].z - boxC.northing))
                    - ((boxD.northing - boxC.northing) * ((*stripList[s])[p].x - boxC.easting))) < 0) { continue; }

            if ((((boxC.easting - boxB.easting) * ((*stripList[s])[p].z - boxB.northing))
                    - ((boxC.northing - boxB.northing) * ((*stripList[s])[p].x - boxB.easting))) < 0) { continue; }

            if ((((boxA.easting - boxD.easting) * ((*stripList[s])[p].z - boxD.northing))
                    - ((boxA.northing - boxD.northing) * ((*stripList[s])[p].x - boxD.easting))) < 0) { continue; }

            //in the box so is it parallelish or perpedicularish to current heading
            ref2 = M_PI - fabs(fabs(vehicle->fixHeading - (*stripList[s])[p].y) - M_PI);
            if (ref2 < 1.2 || ref2 > 1.9)
            {
                //it's in the box and parallelish so add to list
                pointC.x = (*stripList[s])[p].x;
                pointC.z = (*stripList[s])[p].z;
                pointC.h = (*stripList[s])[p].y;
                pointC.strip = s;
                pointC.pt = p;
                conList.append(pointC);
            }
        }
    }

    //no points in the box, exit
    ptCount = conList.size();
    if (ptCount == 0)
    {
        distanceFromCurrentLine = 9999;
        return;
    }

    //determine closest point
    minDistance = 99999;
    for (int i = 0; i < ptCount; i++)
    {
        double dist = ((eastFix - conList[i].x) * (eastFix - conList[i].x))
                        + ((northFix - conList[i].z) * (northFix - conList[i].z));
        if (minDistance >= dist)
        {
            minDistance = dist;
            closestRefPoint = i;
        }
    }

    //now we have closest point, the distance squared from it, and which patch and point its from
    int strip = conList[closestRefPoint].strip;
    int pt = conList[closestRefPoint].pt;
    refX = (*stripList[strip])[pt].x;
    refZ = (*stripList[strip])[pt].z;
    refHeading = (*stripList[strip])[pt].y;

    //are we going same direction as stripList was created?
    bool isSameWay = M_PI - fabs(fabs(vehicle->fixHeading - refHeading) - M_PI) < 1.4;

    //which side of the patch are we on is next
    //calculate endpoints of reference line based on closest point
    refPoint1.easting = refX - (sin(refHeading) * 50.0);
    refPoint1.northing = refZ - (cos(refHeading) * 50.0);

    refPoint2.easting = refX + (sin(refHeading) * 50.0);
    refPoint2.northing = refZ + (cos(refHeading) * 50.0);

    //x2-x1
    double dx = refPoint2.easting - refPoint1.easting;
    //z2-z1
    double dz = refPoint2.northing - refPoint1.northing;

    //how far are we away from the reference line at 90 degrees - 2D cross product and distance
    distanceFromRefLine = ((dz * vehicle->fixEasting) - (dx * vehicle->fixNorthing) + (refPoint2.easting
                            * refPoint1.northing) - (refPoint2.northing * refPoint1.easting))
                                / sqrt((dz * dz) + (dx * dx));

    //add or subtract pi by 2 depending on which side of ref line
    double piSide;

    //sign of distance determines which side of line we are on
    if (distanceFromRefLine > 0) piSide = PIBy2;
    else piSide = -PIBy2;

    //offset calcs
    double toolOffset = vehicle->toolOffset;
    if (isSameWay) toolOffset = 0.0;
    else
    {
        if (distanceFromRefLine > 0) toolOffset *= 2.0;
        else toolOffset *= -2.0;
    }

    //move the Guidance Line over based on the overlap, width, and offset amount set in vehicle
    double widthMinusOverlap = vehicle->toolWidth - vehicle->toolOverlap + toolOffset;

    //absolute the distance
    distanceFromRefLine = fabs(distanceFromRefLine);

    //make the new guidance line list called guideList
    ptCount = stripList[strip]->size()-1;
    int start, stop;

    if (isSameWay)
    {
        start = pt - 25;
        if (start < 0) start = 0;
        stop = pt + 60;
        if (stop > ptCount) stop = ptCount+1;
    }
    else
    {
        start = pt - 60;
        if (start < 0) start = 0;
        stop = pt + 25;
        if (stop > ptCount) stop = ptCount+1;
    }

    for (int i = start; i < stop; i++)
    {
        QVector3D point = QVector3D(
            (*stripList[strip])[i].x + (sin(piSide + (*stripList[strip])[i].y) * widthMinusOverlap),
            (*stripList[strip])[i].y,
            (*stripList[strip])[i].z + (cos(piSide + (*stripList[strip])[i].y) * widthMinusOverlap));

        ctList.append(point);
    }

    //we'll have to rebuild the OpenGL buffer for the guidance line.
    ctListBufferCurrent = false;
}

//determine distance from contour guidance line
void CContour::distanceFromContourLine()
{
    //grab a copy from main
    pivotAxlePosCT = vehicle->pivotAxlePos;

    double minDistA = 1000000, minDistB = 1000000;
    int ptCount = ctList.size();
    //distanceFromCurrentLine = 9999;
    if (ptCount > 0)
    {
        //find the closest 2 points to current fix
        for (int t = 0; t < ptCount; t++)
        {
            double dist = ((vehicle->fixEasting - ctList[t].x()) * (vehicle->fixEasting - ctList[t].x()))
                            + ((vehicle->fixNorthing - ctList[t].z()) * (vehicle->fixNorthing - ctList[t].z()));
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

        //get the distance from currently active AB line
        //x2-x1
        double dx = ctList[B].x() - ctList[A].x();
        //z2-z1
        double dz = ctList[B].z() - ctList[A].z();

        if (fabs(dx) < DOUBLE_EPSILON && fabs(dz) < DOUBLE_EPSILON) return;

        //abHeading = atan2(dz, dx);
        abHeading = ctList[A].y();

        //how far from current AB Line is fix
        distanceFromCurrentLine = ((dz * vehicle->fixEasting) - (dx * vehicle->fixNorthing) + (ctList[B].x()
                    * ctList[A].z()) - (ctList[B].z() * ctList[A].x()))
                        / sqrt((dz * dz) + (dx * dx));

        //are we on the right side or not
        isOnRightSideCurrentLine = distanceFromCurrentLine > 0;

        //absolute the distance
        distanceFromCurrentLine = fabs(distanceFromCurrentLine);

        // ** Pure pursuit ** - calc point on ABLine closest to current position
        double U = (((pivotAxlePosCT.easting - ctList[A].x()) * (dx))
                    + ((pivotAxlePosCT.northing - ctList[A].z()) * (dz)))
                    / ((dx * dx) + (dz * dz));

        rEastCT = ctList[A].x() + (U * (dx));
        rNorthCT = ctList[A].z() + (U * (dz));

        //Subtract the two headings, if > 1.57 its going the opposite heading as refAB
        abFixHeadingDelta = (fabs(vehicle->fixHeading - abHeading));
        if (abFixHeadingDelta >= M_PI) abFixHeadingDelta = fabs(abFixHeadingDelta - twoPI);

        //used for accumulating distance to find goal point
        double distSoFar;

        //how far should goal point be away  - speed * seconds * kmph -> m/s + min value
        double goalPointDistance = vehicle->speed * vehicle->goalPointLookAhead * 0.27777777;

        //minimum of 4.0 meters look ahead
        if (goalPointDistance < 3.0) goalPointDistance = 3.0;

        // used for calculating the length squared of next segment.
        double tempDist = 0.0;

        if (abFixHeadingDelta >= PIBy2)
        {
            //counting down
            isABSameAsFixHeading = false;
            distSoFar = CNMEA::distance(ctList[A].z(), ctList[A].x(), rNorthCT, rEastCT);
            //Is this segment long enough to contain the full lookahead distance?
            if (distSoFar > goalPointDistance)
            {
                //treat current segment like an AB Line
                goalPointCT.easting = rEastCT - (sin(ctList[A].y()) * goalPointDistance);
                goalPointCT.northing = rNorthCT - (cos(ctList[A].y()) * goalPointDistance);
            }

            //multiple segments required
            else
            {
                //cycle thru segments and keep adding lengths. check if start and break if so.
                while (A > 0)
                {
                    B--; A--;
                    tempDist = CNMEA::distance(ctList[B].z(), ctList[B].x(), ctList[A].z(), ctList[A].x());

                    //will we go too far?
                    if ((tempDist + distSoFar) > goalPointDistance)
                    {
                        //A++; B++;
                        break; //tempDist contains the full length of next segment
                    }
                    else distSoFar += tempDist;
                }

                double t = (goalPointDistance - distSoFar); // the remainder to yet travel
                t /= tempDist;

                goalPointCT.easting = (((1 - t) * ctList[B].x()) + (t * ctList[A].x()));
                goalPointCT.northing = (((1 - t) * ctList[B].z()) + (t * ctList[A].z()));
            }
        }
        else
        {
            //counting up
            isABSameAsFixHeading = true;
            distSoFar = CNMEA::distance(ctList[B].z(), ctList[B].x(), rNorthCT, rEastCT);

            //Is this segment long enough to contain the full lookahead distance?
            if (distSoFar > goalPointDistance)
            {
                //treat current segment like an AB Line
                goalPointCT.easting = rEastCT + (sin(ctList[A].y()) * goalPointDistance);
                goalPointCT.northing = rNorthCT + (cos(ctList[A].y()) * goalPointDistance);
            }

            //multiple segments required
            else
            {
                //cycle thru segments and keep adding lengths. check if end and break if so.
                // ReSharper disable once LoopVariableIsNeverChangedInsideLoop
                while (B < ptCount - 1)
                {
                    B++; A++;
                    tempDist = CNMEA::distance(ctList[B].z(), ctList[B].x(), ctList[A].z(), ctList[A].x());

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

                goalPointCT.easting = (((1 - t) * ctList[A].x()) + (t * ctList[B].x()));
                goalPointCT.northing = (((1 - t) * ctList[A].z()) + (t * ctList[B].z()));
            }
        }

        //calc "D" the distance from pivot axle to lookahead point
        double goalPointDistanceSquared = CNMEA::distanceSquared(goalPointCT.northing, goalPointCT.easting, pivotAxlePosCT.northing, pivotAxlePosCT.easting);

        //calculate the the delta x in local coordinates and steering angle degrees based on wheelbase
        double localHeading = twoPI - vehicle->fixHeading;
        ppRadiusCT = goalPointDistanceSquared / (2 * (((goalPointCT.easting - pivotAxlePosCT.easting) * cos(localHeading)) + ((goalPointCT.northing - pivotAxlePosCT.northing) * sin(localHeading))));

        steerAngleCT = toDegrees(atan(2 * (((goalPointCT.easting - pivotAxlePosCT.easting) * cos(localHeading))
            + ((goalPointCT.northing - pivotAxlePosCT.northing) * sin(localHeading))) * vehicle->wheelbase / goalPointDistanceSquared));

        if (steerAngleCT < -vehicle->maxSteerAngle) steerAngleCT = -vehicle->maxSteerAngle;
        if (steerAngleCT > vehicle->maxSteerAngle) steerAngleCT = vehicle->maxSteerAngle;

        if (ppRadiusCT < -500) ppRadiusCT = -500;
        if (ppRadiusCT > 500) ppRadiusCT = 500;

        radiusPointCT.easting = pivotAxlePosCT.easting + (ppRadiusCT * cos(localHeading));
        radiusPointCT.northing = pivotAxlePosCT.northing + (ppRadiusCT * sin(localHeading));

        //angular velocity in rads/sec  = 2PI * m/sec * radians/meters
        angVel = twoPI * 0.277777 * vehicle->speed * (tan(toRadians(steerAngleCT))) / vehicle->wheelbase;

        //clamp the steering angle to not exceed safe angular velocity
        if (fabs(angVel) > vehicle->maxAngularVelocity)
        {
            steerAngleCT = toDegrees(steerAngleCT > 0 ?
                    (atan((vehicle->wheelbase * vehicle->maxAngularVelocity) / (twoPI * vehicle->speed * 0.277777)))
                : (atan((vehicle->wheelbase * -vehicle->maxAngularVelocity) / (twoPI * vehicle->speed * 0.277777))));
        }
        //Convert to centimeters
        distanceFromCurrentLine = distanceFromCurrentLine * 1000.0;
        distanceFromCurrentLine = (distanceFromCurrentLine < 0) ? floor(distanceFromCurrentLine) : ceil(distanceFromCurrentLine);

        //distance is negative if on left, positive if on right
        //if you're going the opposite direction left is right and right is left
        //double temp;
        if (isABSameAsFixHeading)
        {
            //temp = (abHeading);
            if (!isOnRightSideCurrentLine) distanceFromCurrentLine *= -1.0;
        }

        //opposite way so right is left
        else
        {
            //temp = (abHeading - M_PI);
            //if (temp < 0) temp = (temp + twoPI);
            //temp = toDegrees(temp);
            if (isOnRightSideCurrentLine) distanceFromCurrentLine *= -1.0;
        }

        vehicle->guidanceLineDistanceOff = int(distanceFromCurrentLine);
        vehicle->guidanceLineSteerAngle = int(steerAngleCT*10);
        //mf->guidanceLineHeadingDelta = (Int16)((atan2(sin(temp - mf->fixHeading),
        //                                    cos(temp - mf->fixHeading))) * 10000);
   }
    else
    {
        //invalid distance so tell AS module
        distanceFromCurrentLine = 32000;
        vehicle->guidanceLineDistanceOff = 32000;
    }
}

//draw the red follow me line
void CContour::drawContourLine(QOpenGLContext *glContext, const QMatrix4x4 &modelview, const QMatrix4x4 &projection)
{
    QSettings settings;
    QOpenGLFunctions *gles = glContext->functions();

    /* generate ctList OpenGL buffer */
    if (! ctListBufferCurrent) {
        if (ctListBuffer.isCreated())
            ctListBuffer.destroy();
        ctListBuffer.create();
        ctListBuffer.bind();

        QVector<QVector3D> temp;
        foreach(QVector3D i, ctList) {
            temp.append(QVector3D(i.x(), i.z(), 0));
        }

        ctListBuffer.allocate(temp.constData(), temp.size() * sizeof(QVector3D));
        ctListBuffer.release();

        ctListBufferCurrent = true;
    }

    ////draw the guidance line
    QColor color = QColor::fromRgbF(0.98f, 0.2f, 0.0f);
    glDrawArraysColor(gles, projection * modelview,
                      GL_LINE_STRIP, color,
                      ctListBuffer, GL_FLOAT,
                      ctList.size(),2);


    //draw points on line
    color = QColor::fromRgbF(0.7f, 0.7f, 0.25f);
    glDrawArraysColor(gles, projection * modelview,
                      GL_POINTS, color,
                      ctListBuffer, GL_FLOAT,
                      ctList.size(), 2);

    gl->glPointSize(1.0f);

    if (settings.value("display/isPureOn",true).toBool())
    {
        const int numSegments = 100;
        {
            double theta = twoPI / (numSegments);
            double c = cos(theta);//precalculate the sine and cosine
            double s = sin(theta);

            double x = ppRadiusCT;//we start at angle = 0
            double y = 0;

            QVector<QVector3D> circle;

            for (int ii = 0; ii < numSegments; ii++)
            {
                circle.append(QVector3D(x + radiusPointCT.easting,
                                        y + radiusPointCT.northing,
                                        0));
                //apply the rotation matrix
                double t = x;
                x = (c * x) - (s * y);
                y = (s * t) + (c * y);
            }

            //Build or reload openGL buffer with points
            if (purePursuitBuffer.isCreated()) {
                purePursuitBuffer.bind();
                purePursuitBuffer.write(0,circle.constData(),
                                        numSegments * sizeof(QVector3D));
                purePursuitBuffer.release();
            } else {
                purePursuitBuffer.create();
                purePursuitBuffer.bind();
                purePursuitBuffer.allocate(circle.constData(),
                                           numSegments * sizeof(QVector3D));
                purePursuitBuffer.release();
            }
            QColor color = QColor::fromRgbF(0.95f, 0.30f, 0.950f);
            glDrawArraysColor(gles, projection * modelview,
                              GL_LINE_LOOP, color,
                              purePursuitBuffer, GL_FLOAT,
                              numSegments);


            QVector3D lookAheadPoint = QVector3D(goalPointCT.easting,
                                                 goalPointCT.northing, 0.0);

            if (lookAheadPointBuffer.isCreated()) {
                lookAheadPointBuffer.bind();
                lookAheadPointBuffer.write(0,&lookAheadPoint, sizeof(QVector3D));
                lookAheadPointBuffer.release();
            } else {
                lookAheadPointBuffer.create();
                lookAheadPointBuffer.bind();
                lookAheadPointBuffer.allocate(&lookAheadPoint, sizeof(QVector3D));
                lookAheadPointBuffer.release();
            }
            //Draw lookahead Point
            color = QColor::fromRgbF(1.0f, 0.5f, 0.95f);
            glDrawArraysColor(gles, projection * modelview,
                              GL_POINTS, color,
                              lookAheadPointBuffer, GL_FLOAT,
                              1,4.0f);
        }
    }
}

//Reset the contour to zip
void CContour::resetContour()
{
    stripList.clear();

    if (!ptList.isNull()) ptList->clear();
    ctList.clear();
    ctListBufferCurrent = false;
}
