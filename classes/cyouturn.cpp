#include <limits>
#include <math.h>
#include <QFile>
#include "cyouturn.h"
#include "aogsettings.h"
#include "cabline.h"
#include "cvehicle.h"
#include "cnmea.h"
#include "glm.h"

//constructor
CYouTurn::CYouTurn()
{
        AOGSettings s;
        //how far before or after boundary line should turn happen
        //TODO: Specify a path for this value, and a default
        startYouTurnAt = s.value("",0).toInt(); //Properties.Settings.Default.setAS_startYouTurnAt;
}

void CYouTurn::cancelYouTurn()
{
        isYouTurnOn = false;
        isAutoTriggered = false;
        isAutoPointSet = false;
        if (ytList.size() > 0) ytList.clear();
        //mf.AutoYouTurnButtonsReset();
        //Maybe main form can poll isYouTurnOn.
}

bool CYouTurn::loadYouTurnShapeFromFile(QString filename)
{
    //if an error occurs, this returns false, and caller should check
    //QIODevice::errorString()

    //if there is existing shape, delete it
    if (youFileList.size() > 0) youFileList.clear();

    QFile File(filename);

    if (!File.exists())
    {
        //TODO: popup message
        //var form = new FormTimedMessage(4000, "Missing Youturn File", "Fix the thing!");
        //form.Show();

        return false;

    } else {
        if (!File.open(QIODevice::ReadOnly | QIODevice::Text)) {
            //popup error message about not being able to read file
            return false;
        }

        QTextStream reader(&File);

        QString line = reader.readLine();
        if(line.isNull()) return false;

        int points = line.toInt();

        if (points) {
            QVector2D coords;

            for (int v=0; v < points; v++) {
                line = reader.readLine();

                if(line.isNull()) return false;

                QStringList words = line.split(',');

                coords.setX(words[0].toDouble());
                coords.setY(words[1].toDouble());
                youFileList.append(coords);
            }
        }
    }

    return true;
}

//build the points and path
void CYouTurn::buildYouTurnListToRight(CABLine *ABLine, bool isTurnRight)
{
    isYouTurnOn = true;

    //grab the Lookahead that ABLine uses
    minLookAheadDistance = ABLine->minLookAheadDistance;

    //point on AB line closest to pivot axle point from ABLine PurePursuit
    rEastYT = ABLine->rEastAB;
    rNorthYT = ABLine->rNorthAB;
    isABSameAsFixHeading = ABLine->isABSameAsFixHeading;
    pivotAxlePosYT = ABLine->vehicle->pivotAxlePos;

    //grab the vehicle widths and offsets
    double widthMinusOverlap = ABLine->vehicle->toolWidth - ABLine->vehicle->toolOverlap;
    double toolOffset = ABLine->vehicle->toolOffset * 2.0;
    double turnOffset = 0;
    abHeading = ABLine->abHeading;

    //turning right same as AB line
    if ((isTurnRight && isABSameAsFixHeading) || (isTurnRight && !isABSameAsFixHeading))
    {
        turnOffset = (widthMinusOverlap + toolOffset);
    }

    //turning left same way as AB line
    if ((!isTurnRight && isABSameAsFixHeading) || (!isTurnRight && !isABSameAsFixHeading))
    {
        turnOffset = (widthMinusOverlap - toolOffset);
    }

    numShapePoints = youFileList.size();
    QVector4D pt[numShapePoints];

    //Now put the shape into an array since lists are immutable
    for (int i = 0; i < numShapePoints; i++)
    {
        pt[i].setX(youFileList[i].x());
        pt[i].setZ(youFileList[i].y());
    }

    //start of path on the origin. Mirror the shape if left turn
    if (!isTurnRight)
    {
        for (int i = 0; i < numShapePoints; i++)
            pt[i].setX( pt[i].x() * -1 );
    }

    //scaling - Drawing is 10m wide so find ratio of tool width
    double scale = turnOffset * 0.1;
    for (int i = 0; i < numShapePoints; i++)
    {
        pt[i].setX( pt[i].x() * scale );
        pt[i].setZ( pt[i].x() * scale );
    }

    //rotate pattern to match AB Line heading
    for (int i = 0; i < numShapePoints; i++)
    {
        double xr, yr;
        if (isABSameAsFixHeading)
        {
            xr = (cos(-abHeading) * pt[i].x()) - (sin(-abHeading) * pt[i].z());
            yr = (sin(-abHeading) * pt[i].x()) + (cos(-abHeading) * pt[i].z());
        }
        else
        {
            xr = (cos(-abHeading+M_PI) * pt[i].x()) - (sin(-abHeading+M_PI) * pt[i].z());
            yr = (sin(-abHeading+M_PI) * pt[i].x()) + (cos(-abHeading+M_PI) * pt[i].z());
        }

        pt[i].setX(xr+rEastYT);
        pt[i].setZ(yr+rNorthYT);
        pt[i].setW(0);
        pt[i].setY(atan2(pt[i].z(), pt[i].x()));

        ytList.append(pt[i]);
    }
}

//determine distance from youTurn guidance line
void CYouTurn::distanceFromYouTurnLine(CVehicle *vehicle)
{
    //grab a copy from main
    pivotAxlePosYT = vehicle->pivotAxlePos;
    double minDistA = 1000000, minDistB = 1000000;
    int ptCount = ytList.size();

    if (ptCount > 0)
    {
        //find the closest 2 points to current fix
        for (int t = 0; t < ptCount; t++)
        {
            double dist = ((pivotAxlePosYT.easting - ytList[t].x()) * (pivotAxlePosYT.easting - ytList[t].x()))
                            + ((pivotAxlePosYT.northing - ytList[t].z()) * (pivotAxlePosYT.northing - ytList[t].z()));
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
        double dx = ytList[B].x() - ytList[A].x();
        double dz = ytList[B].z() - ytList[A].z();
        if (fabs(dx) < std::numeric_limits<double>::epsilon() && fabs(dz) < std::numeric_limits<double>::epsilon()) return;

        //abHeading = atan2(dz, dx);
        abHeading = ytList[A].y();

        //how far from current AB Line is fix
        distanceFromCurrentLine = ((dz * vehicle->fixEasting) - (dx * vehicle->fixNorthing) + (ytList[B].x()
                    * ytList[A].z()) - (ytList[B].z() * ytList[A].x()))
                        / sqrt((dz * dz) + (dx * dx));

        //are we on the right side or not
        isOnRightSideCurrentLine = distanceFromCurrentLine > 0;

        //absolute the distance
        distanceFromCurrentLine = fabs(distanceFromCurrentLine);

        //return and reset if too far away or end of the line
        if (distanceFromCurrentLine > 5 || B >= ptCount - 3)
        {
            cancelYouTurn();
            //TODO: how to communicate with main form. Probably with a signal.
            return;
        }

        // ** Pure pursuit ** - calc point on ABLine closest to current position
        double U = (((pivotAxlePosYT.easting - ytList[A].x()) * (dx))
                    + ((pivotAxlePosYT.northing - ytList[A].z()) * (dz)))
                    / ((dx * dx) + (dz * dz));

        rEastYT = ytList[A].x() + (U * (dx));
        rNorthYT = ytList[A].z() + (U * (dz));

        //used for accumulating distance to find goal point
        double distSoFar;

        //how far should goal point be away  - speed * seconds * kmph -> m/s + min value
        double goalPointDistance = vehicle->speed * vehicle->goalPointLookAhead * 0.27777777;

        //minimum of Whatever AB Line is meters look ahead
        if (goalPointDistance < minLookAheadDistance) goalPointDistance = minLookAheadDistance;

        // used for calculating the length squared of next segment.
        double tempDist = 0.0;

        isABSameAsFixHeading = true;
        distSoFar = CNMEA::distance(ytList[B].z(), ytList[B].x(), rNorthYT, rEastYT);

        //Is this segment long enough to contain the full lookahead distance?
        if (distSoFar > goalPointDistance)
        {
            //treat current segment like an AB Line
            goalPointYT.easting = rEastYT + (sin(ytList[A].y()) * goalPointDistance);
            goalPointYT.northing = rNorthYT + (cos(ytList[A].y()) * goalPointDistance);
        }

        //multiple segments required
        else
        {
            //cycle thru segments and keep adding lengths. check if end and break if so.
            // ReSharper disable once LoopVariableIsNeverChangedInsideLoop
            while (B < ptCount - 1)
            {
                B++; A++;
                tempDist = CNMEA::distance(ytList[B].z(), ytList[B].x(), ytList[A].z(), ytList[A].x());
                if ((tempDist + distSoFar) > goalPointDistance) break; //will we go too far?
                distSoFar += tempDist;
            }

            double t = (goalPointDistance - distSoFar); // the remainder to yet travel
            t /= tempDist;
            goalPointYT.easting = (((1 - t) * ytList[A].x()) + (t * ytList[B].x()));
            goalPointYT.northing = (((1 - t) * ytList[A].z()) + (t * ytList[B].z()));
        }

        //calc "D" the distance from pivot axle to lookahead point
        double goalPointDistanceSquared = CNMEA::distanceSquared(goalPointYT.northing, goalPointYT.easting, pivotAxlePosYT.northing, pivotAxlePosYT.easting);

        //calculate the the delta x in local coordinates and steering angle degrees based on wheelbase
        double localHeading = twoPI - vehicle->fixHeading;
        ppRadiusYT = goalPointDistanceSquared / (2 * (((goalPointYT.easting - pivotAxlePosYT.easting) * cos(localHeading)) + ((goalPointYT.northing - pivotAxlePosYT.northing) * sin(localHeading))));

        steerAngleYT = toDegrees(atan(2 * (((goalPointYT.easting - pivotAxlePosYT.easting) * cos(localHeading))
            + ((goalPointYT.northing - pivotAxlePosYT.northing) * sin(localHeading))) * vehicle->wheelbase / goalPointDistanceSquared));

        if (steerAngleYT < -vehicle->maxSteerAngle) steerAngleYT = -vehicle->maxSteerAngle;
        if (steerAngleYT > vehicle->maxSteerAngle) steerAngleYT = vehicle->maxSteerAngle;

        if (ppRadiusYT < -500) ppRadiusYT = -500;
        if (ppRadiusYT > 500) ppRadiusYT = 500;

        radiusPointYT.easting = pivotAxlePosYT.easting + (ppRadiusYT * cos(localHeading));
        radiusPointYT.northing = pivotAxlePosYT.northing + (ppRadiusYT * sin(localHeading));

        //angular velocity in rads/sec  = 2PI * m/sec * radians/meters
        double angVel = twoPI * 0.277777 * vehicle->speed * (tan(toRadians(steerAngleYT))) / vehicle->wheelbase;

        //clamp the steering angle to not exceed safe angular velocity
        if (fabs(angVel) > vehicle->maxAngularVelocity)
        {
            steerAngleYT = toDegrees(steerAngleYT > 0 ?
                    (atan((vehicle->wheelbase * vehicle->maxAngularVelocity) / (twoPI * vehicle->speed * 0.277777)))
                : (atan((vehicle->wheelbase * -vehicle->maxAngularVelocity) / (twoPI * vehicle->speed * 0.277777))));
        }
        //Convert to centimeters
        distanceFromCurrentLine = roundAwayFromZero(distanceFromCurrentLine * 1000.0);

        //distance is negative if on left, positive if on right
        //if you're going the opposite direction left is right and right is left
        if (isABSameAsFixHeading)
        {
            if (!isOnRightSideCurrentLine) distanceFromCurrentLine *= -1.0;
        }

        //opposite way so right is left
        else
        {
            if (isOnRightSideCurrentLine) distanceFromCurrentLine *= -1.0;
        }

        vehicle->guidanceLineDistanceOff = (short)distanceFromCurrentLine;
        vehicle->guidanceLineSteerAngle = (short)(steerAngleYT * 10);
    }
    else
    {
        cancelYouTurn();
    }
}
