#include "cabline.h"
#include "vec2.h"
#include "glm.h"
#include "cnmea.h"

//#include <QtOpenGL>
#include <QSettings>
#include <QOpenGLFunctions>
#include <QColor>
#include "glutils.h"

CABLine::CABLine(CVehicle *v, CYouTurn *t)
    :vehicle(v), yt(t)
{

}

void CABLine::deleteAB() {
    refPoint1 = Vec2(0.0, 0.0);
    refPoint2 = Vec2(0.0, 1.0);

    refABLineP1 = Vec2(0.0, 0.0);
    refABLineP2 = Vec2(0.0, 1.0);

    currentABLineP1 = Vec2(0.0, 0.0);
    currentABLineP2 = Vec2(0.0, 1.0);

    abHeading = 0.0;

    passNumber = 0.0;

    howManyPathsAway = 0.0;

    isABLineSet = false;
}

void CABLine::setABLineByPoint()
{
    refPoint2.easting = vehicle->fixEasting;
    refPoint2.northing = vehicle->fixNorthing;
    //std::cout << refPoint2.easting << std::endl;
    //std::cout << refPoint2.northing << std::endl;

    //calculate the AB Heading
    abHeading = atan2(refPoint2.easting - refPoint1.easting, refPoint2.northing - refPoint1.northing);
    if (abHeading < 0) abHeading += twoPI;

    //sin x cos z for endpoints, opposite for additional lines
    refABLineP1.easting = refPoint1.easting - (sin(abHeading) * 4000.0);
    refABLineP1.northing = refPoint1.northing - (cos(abHeading) * 4000.0);

    refABLineP2.easting = refPoint1.easting + (sin(abHeading) * 4000.0);
    refABLineP2.northing = refPoint1.northing + (cos(abHeading) * 4000.0);

    isABLineSet = true;
}

void CABLine::setABLineByHeading()
{
    //heading is set in the AB Form
    refABLineP1.easting = refPoint1.easting - (sin(abHeading) * 4000.0);
    refABLineP1.northing = refPoint1.northing - (cos(abHeading) * 4000.0);

    refABLineP2.easting = refPoint1.easting + (sin(abHeading) * 4000.0);
    refABLineP2.northing = refPoint1.northing + (cos(abHeading) * 4000.0);

    refPoint2.easting = refABLineP2.easting;
    refPoint2.northing = refABLineP2.northing;

    isABLineSet = true;
}

void CABLine::snapABLine()
{
    double headingCalc;
    //calculate the heading 90 degrees to ref ABLine heading
    if (isOnRightSideCurrentLine) headingCalc = abHeading + PIBy2;
    else headingCalc = abHeading - PIBy2;

    //calculate the new points for the reference line and points
    refPoint1.easting = (sin(headingCalc) * fabs(distanceFromCurrentLine) * 0.001) + refPoint1.easting;
    refPoint1.northing = (cos(headingCalc) * fabs(distanceFromCurrentLine) * 0.001) + refPoint1.northing;

    refABLineP1.easting = refPoint1.easting - (sin(abHeading) * 4000.0);
    refABLineP1.northing = refPoint1.northing - (cos(abHeading) * 4000.0);

    refABLineP2.easting = refPoint1.easting + (sin(abHeading) * 4000.0);
    refABLineP2.northing = refPoint1.northing + (cos(abHeading) * 4000.0);

    refPoint2.easting = refABLineP2.easting;
    refPoint2.northing = refABLineP2.northing;
}

void CABLine::getCurrentABLine() {
    //grab a copy from main
    pivotAxlePosAB = vehicle->pivotAxlePos;

    //move the ABLine over based on the overlap amount set in vehicle
    double widthMinusOverlap = vehicle->toolWidth - vehicle->toolOverlap;

     //x2-x1
    double dx = refABLineP2.easting - refABLineP1.easting;
    //z2-z1
    double dy = refABLineP2.northing - refABLineP1.northing;

    //how far are we away from the reference line at 90 degrees
    distanceFromRefLine = ((dy * pivotAxlePosAB.easting) - (dx * pivotAxlePosAB.northing) + (refABLineP2.easting *
                            refABLineP1.northing) - (refABLineP2.northing * refABLineP1.easting)) /
                                sqrt((dy * dy) + (dx * dx));

    //sign of distance determines which side of line we are on
    if (distanceFromRefLine > 0) refLineSide = 1;
    else refLineSide = -1;

    //absolute the distance
    distanceFromRefLine = fabs(distanceFromRefLine);

    //Which ABLine is the vehicle on, negative is left and positive is right side
    //howManyPathsAway = Math.Round(distanceFromRefLine / widthMinusOverlap, 0, MidpointRounding.AwayFromZero);
    //midpoint away from zero is f(x) = (x < 0) ? floor(x) : ceil(x);
    howManyPathsAway = distanceFromRefLine / widthMinusOverlap;

    //generate that pass number as signed integer, with midpoint rounding away from zero
    passNumber = int((howManyPathsAway < 0) ? floor(howManyPathsAway) : ceil(howManyPathsAway));

    //calculate the new point that is number of implement widths over
    double toolOffset = vehicle->toolOffset;
    Vec2 point1;

    //depending which way you are going, the offset can be either side
    if (isABSameAsFixHeading)
    {
        point1 = Vec2((cos(-abHeading) * ((widthMinusOverlap * howManyPathsAway * refLineSide) - toolOffset)) + refPoint1.easting,
                      (sin(-abHeading) * ((widthMinusOverlap * howManyPathsAway * refLineSide) - toolOffset)) + refPoint1.northing);
    }
    else
    {
        point1 = Vec2((cos(-abHeading) * ((widthMinusOverlap * howManyPathsAway * refLineSide) + toolOffset)) + refPoint1.easting,
                      (sin(-abHeading) * ((widthMinusOverlap * howManyPathsAway * refLineSide) + toolOffset)) + refPoint1.northing);
    }

    //create the new line extent points for current ABLine based on original heading of AB line
    currentABLineP1.easting = point1.easting - (sin(abHeading) * 40000.0);
    currentABLineP1.northing = point1.northing - (cos(abHeading) * 40000.0);

    currentABLineP2.easting = point1.easting + (sin(abHeading) * 40000.0);
    currentABLineP2.northing = point1.northing + (cos(abHeading) * 40000.0);

    //get the distance from currently active AB line
    //x2-x1
    dx = currentABLineP2.easting - currentABLineP1.easting;
    //z2-z1
    dy = currentABLineP2.northing - currentABLineP1.northing;

    //save a copy of dx,dy in youTurn
    //vehicle->yt->dxAB = dx;
    //vehicle->yt->dyAB = dy;
    this->dx = dx;
    this->dy = dy;

    //how far from current AB Line is fix
    distanceFromCurrentLine = ((dy * pivotAxlePosAB.easting) - (dx * pivotAxlePosAB.northing)
                               + (currentABLineP2.easting * currentABLineP1.northing)
                               - (currentABLineP2.northing * currentABLineP1.easting))
            / sqrt((dy * dy) + (dx * dx));

    //are we on the right side or not
    isOnRightSideCurrentLine = distanceFromCurrentLine > 0;

    //absolute the distance
    distanceFromCurrentLine = fabs(distanceFromCurrentLine);

    //Subtract the two headings, if > 1.57 its going the opposite heading as refAB
    abFixHeadingDelta = (fabs(vehicle->fixHeading - abHeading));
    if (abFixHeadingDelta >= M_PI) abFixHeadingDelta = fabs(abFixHeadingDelta - twoPI);

    // ** Pure pursuit ** - calc point on ABLine closest to current position
    //if (currentABLineP1.easting == currentABLineP2.easting && currentABLineP1.northing == currentABLineP2.northing) currentABLineP1.easting -= 0.00001;

    double U = (((pivotAxlePosAB.easting - currentABLineP1.easting) * (dx))
                + ((pivotAxlePosAB.northing - currentABLineP1.northing) * (dy)))
            / ((dx * dx) + (dy * dy));

    //point on AB line closest to pivot axle point
    rEastAB = currentABLineP1.easting + (U * dx );
    rNorthAB = currentABLineP1.northing + (U * dy );

    //how far should goal point be away  - speed * seconds * kmph -> m/s + min value

    double goalPointDistance = (vehicle->speed * vehicle->goalPointLookAhead * 0.2777777777);
    if (goalPointDistance < minLookAheadDistance) goalPointDistance = minLookAheadDistance;

    if (abFixHeadingDelta >= PIBy2)
    {
        isABSameAsFixHeading = false;
        goalPointAB.easting = rEastAB - (sin(abHeading) * goalPointDistance);
        goalPointAB.northing = rNorthAB - (cos(abHeading) * goalPointDistance);
    }
    else
    {
        isABSameAsFixHeading = true;
        goalPointAB.easting = rEastAB + (sin(abHeading) * goalPointDistance);
        goalPointAB.northing = rNorthAB + (cos(abHeading) * goalPointDistance);
    }

    //calc "D" the distance from pivot axle to lookahead point
    double goalPointDistanceDSquared = CNMEA::distanceSquared(goalPointAB.northing, goalPointAB.easting, pivotAxlePosAB.northing, pivotAxlePosAB.easting);

    //calculate the the new x in local coordinates and steering angle degrees based on wheelbase
    double localHeading = twoPI - vehicle->fixHeading;
    ppRadiusAB = goalPointDistanceDSquared / (2 * (((goalPointAB.easting - pivotAxlePosAB.easting) * cos(localHeading)) + ((goalPointAB.northing - pivotAxlePosAB.northing) * sin(localHeading))));

    steerAngleAB = toDegrees(atan( 2 * (((goalPointAB.easting - pivotAxlePosAB.easting) * cos(localHeading))
                                        + ((goalPointAB.northing - pivotAxlePosAB.northing) * sin(localHeading)))
                                   * vehicle->wheelbase / goalPointDistanceDSquared)) ;
    if (steerAngleAB < -vehicle->maxSteerAngle) steerAngleAB = -vehicle->maxSteerAngle;
    if (steerAngleAB > vehicle->maxSteerAngle) steerAngleAB = vehicle->maxSteerAngle;

    //limit circle size for display purpose
    if (ppRadiusAB < -500) ppRadiusAB = -500;
    if (ppRadiusAB > 500) ppRadiusAB = 500;

    radiusPointAB.easting = pivotAxlePosAB.easting + (ppRadiusAB * cos(localHeading));
    radiusPointAB.northing = pivotAxlePosAB.northing + (ppRadiusAB * sin(localHeading));

    //Convert to millimeters
    distanceFromCurrentLine = distanceFromCurrentLine * 1000.0;
    distanceFromCurrentLine = (distanceFromCurrentLine < 0) ? floor(distanceFromCurrentLine) : ceil(distanceFromCurrentLine);
    //distanceFromCurrentLine = Math.Round(distanceFromCurrentLine * 1000.0, MidpointRounding.AwayFromZero);

    //angular velocity in rads/sec  = 2PI * m/sec * radians/meters
    angVel = twoPI * 0.277777 * vehicle->speed * (tan(toRadians(steerAngleAB)))/vehicle->wheelbase;

    //clamp the steering angle to not exceed safe angular velocity
    if (fabs(angVel) > vehicle->maxAngularVelocity)
    {
        steerAngleAB = toDegrees(steerAngleAB > 0 ? (atan((vehicle->wheelbase * vehicle->maxAngularVelocity) / (twoPI * vehicle->speed * 0.277777)))
            : (atan((vehicle->wheelbase * -vehicle->maxAngularVelocity) / (twoPI * vehicle->speed * 0.277777))));
    }

    //distance is negative if on left, positive if on right
    if (isABSameAsFixHeading)
    {
        if (!isOnRightSideCurrentLine) distanceFromCurrentLine *= -1.0;
    }

    //opposite way so right is left
    else
    {
        //double temp = (abHeading - Math.PI);
        //if (temp < 0) temp = (temp + twoPI);
        if (isOnRightSideCurrentLine) distanceFromCurrentLine *= -1.0;
    }

    //mf->guidanceLineHeadingDelta = (Int16)((atan2(sin(temp - mf->fixHeading), cos(temp - mf->fixHeading))) * 10000);
    vehicle->guidanceLineDistanceOff = int(distanceFromCurrentLine);
    vehicle->guidanceLineSteerAngle = int(steerAngleAB * 10);

    if(yt->isYouTurnOn) {
        //do the pure pursuit from youTurn
        yt->distanceFromYouTurnLine(vehicle);

        //now substitute what it thinks are AB line values with auto turn values
        steerAngleAB = yt->steerAngleYT;
        distanceFromCurrentLine = yt->distanceFromCurrentLine;

        goalPointAB = yt->goalPointYT;
        radiusPointAB.easting = yt->radiusPointYT.easting;
        radiusPointAB.northing = yt->radiusPointYT.northing;
        ppRadiusAB = yt->ppRadiusYT;
    }
}

void CABLine::drawABLines(QOpenGLFunctions *gl, const QMatrix4x4 &mvp) {
    QSettings settings;

	QOpenGLBuffer abBuffer;
	QColor color;


    /*
    gl->glMatrixMode(GL_MODELVIEW);
    gl->glPushMatrix();
    gl->glLoadMatrixf(modelview.constData());

    gl->glMatrixMode(GL_PROJECTION);
    gl->glPushMatrix();
    gl->glLoadMatrixf(projection.constData());
    */

    //Draw AB Points
    ColorVertex abpoints[2] = { { QVector3D(refPoint1.easting, refPoint1.northing, 0.0), 
                                  QVector4D(0.5f, 0.0f, 0.0f, 1.0f) },

				{ QVector3D(refPoint2.easting, refPoint2.northing, 0.0),
				  QVector4D(0.0f, 0.0f, 0.5f, 1.0f) } };

    abBuffer.create();
    abBuffer.bind();
    abBuffer.allocate(abpoints, 2 * sizeof(ColorVertex));
    abBuffer.release();

    glDrawArraysColors(gl, mvp,
                       GL_POINTS, abBuffer,
		       GL_FLOAT, 2, 8.0f);

    if (isABLineSet)
    {
        //Draw reference AB line

        QVector3D abline[2] = { QVector3D(refABLineP1.easting, refABLineP1.northing, 0),
	                        QVector3D(refABLineP2.easting, refABLineP2.northing, 0) };
	
		if (abBuffer.isCreated())
			abBuffer.destroy();
		abBuffer.create();
		abBuffer.bind();
		abBuffer.allocate(abline, 2 * sizeof(QVector3D));
		abBuffer.release();


		//TODO: make a dotted line in OpenGL ES
        //gl->glLineStipple(1, 0x07F0);
        //gl->glEnable(GL_LINE_STIPPLE);
		color = QColor::fromRgbF(0.9f, 0.5f, 0.7f, 1.0f);
		glDrawArraysColor(gl,mvp,
		                  GL_LINES, color, abBuffer,
						  GL_FLOAT, 2, 2.0f);
        //calculate if tram line is here
        if (tramPassEvery != 0)
        {
            int pass = (int)passNumber + (tramPassEvery*300) - passBasedOn;
            if (pass % tramPassEvery != 0) color = QColor::fromRgbF(0.9f, 0.0f, 0.0f, 1.0f);
            else color = QColor::fromRgbF(0, 0.9, 0, 1.0f);
        }

        //based on line pass, make ref purple
        if (fabs(passBasedOn - passNumber) < 0.0000000001 && tramPassEvery != 0) 
		    color = QColor::fromRgbF(0.990f, 0.190f, 0.990f, 1.0f);

		abline[0] = QVector3D(currentABLineP1.easting, currentABLineP1.northing, 0.0);
		abline[1] = QVector3D(currentABLineP2.easting, currentABLineP2.northing, 0.0);
		//buffer should still be active and pointing to this data structure so
		//we won't bother re-initializing it.
		glDrawArraysColor(gl,mvp,
		                  GL_LINES, color, abBuffer,
						  GL_FLOAT, 2, 3.0f);
	


        //gl->glEnd();

        if (settings.value("display/isSideGuideLines",true).toBool())
        {
            //get the tool offset and width
            double toolOffset = vehicle->toolOffset * 2;
            double toolWidth = vehicle->toolWidth - vehicle->toolOverlap;

			/*
            gl->glColor3f(0.0f, 0.90f, 0.50f);
            gl->glLineWidth(1);
            gl->glBegin(GL_LINES);
			*/

            //precalculate sin cos
            double cosHeading = cos(-abHeading);
            double sinHeading = sin(-abHeading);

			QVector<QVector3D> vertexList;

            if (isABSameAsFixHeading)
            {
				vertexList.append(QVector3D((cosHeading * (toolWidth + toolOffset)) + currentABLineP1.easting, (sinHeading * (toolWidth + toolOffset)) + currentABLineP1.northing, 0));
				vertexList.append(QVector3D((cosHeading * (toolWidth + toolOffset)) + currentABLineP2.easting, (sinHeading * (toolWidth + toolOffset)) + currentABLineP2.northing, 0));
				vertexList.append(QVector3D((cosHeading * (-toolWidth + toolOffset)) + currentABLineP1.easting, (sinHeading * (-toolWidth + toolOffset)) + currentABLineP1.northing, 0));
				vertexList.append(QVector3D((cosHeading * (-toolWidth + toolOffset)) + currentABLineP2.easting, (sinHeading * (-toolWidth + toolOffset)) + currentABLineP2.northing, 0));

                toolWidth *= 2;
				vertexList.append(QVector3D((cosHeading * (toolWidth)) + currentABLineP1.easting, (sinHeading * (toolWidth)) + currentABLineP1.northing, 0));
				vertexList.append(QVector3D((cosHeading * (toolWidth)) + currentABLineP2.easting, (sinHeading * (toolWidth)) + currentABLineP2.northing, 0));
				vertexList.append(QVector3D((cosHeading * (-toolWidth)) + currentABLineP1.easting, (sinHeading * (-toolWidth)) + currentABLineP1.northing, 0));
				vertexList.append(QVector3D((cosHeading * (-toolWidth)) + currentABLineP2.easting, (sinHeading * (-toolWidth)) + currentABLineP2.northing, 0));
            }
            else
            {
				vertexList.append(QVector3D((cosHeading * (toolWidth - toolOffset)) + currentABLineP1.easting, (sinHeading * (toolWidth - toolOffset)) + currentABLineP1.northing, 0));
				vertexList.append(QVector3D((cosHeading * (toolWidth - toolOffset)) + currentABLineP2.easting, (sinHeading * (toolWidth - toolOffset)) + currentABLineP2.northing, 0));
				vertexList.append(QVector3D((cosHeading * (-toolWidth - toolOffset)) + currentABLineP1.easting, (sinHeading * (-toolWidth - toolOffset)) + currentABLineP1.northing, 0));
				vertexList.append(QVector3D((cosHeading * (-toolWidth - toolOffset)) + currentABLineP2.easting, (sinHeading * (-toolWidth - toolOffset)) + currentABLineP2.northing, 0));

                toolWidth *= 2;
				vertexList.append(QVector3D((cosHeading * (toolWidth)) + currentABLineP1.easting, (sinHeading * (toolWidth)) + currentABLineP1.northing, 0));
				vertexList.append(QVector3D((cosHeading * (toolWidth)) + currentABLineP2.easting, (sinHeading * (toolWidth)) + currentABLineP2.northing, 0));
				vertexList.append(QVector3D((cosHeading * (-toolWidth)) + currentABLineP1.easting, (sinHeading * (-toolWidth)) + currentABLineP1.northing, 0));
				vertexList.append(QVector3D((cosHeading * (-toolWidth)) + currentABLineP2.easting, (sinHeading * (-toolWidth)) + currentABLineP2.northing, 0));
            }

			if (abBuffer.isCreated())
				abBuffer.destroy();
			abBuffer.create();
			abBuffer.bind();
			abBuffer.allocate(vertexList.constData(), vertexList.size() * sizeof(QVector3D));
			abBuffer.release();

			glDrawArraysColor(gl,mvp,
							  GL_LINES, color, abBuffer,
							  GL_FLOAT, vertexList.size(), 1.0f);
            //gl->glEnd();
        }

        if (settings.value("display/isPureDisplayOn",true).toBool())
        {
            //draw the guidance circle
			QVector<QVector3D> pointsList;

            const int numSegments = 100;
            {
				color = QColor::fromRgbF(0.95f, 0.30f, 0.950f, 1.0f);
                double theta = twoPI / numSegments;
                double c = cos(theta);//precalculate the sine and cosine
                double s = sin(theta);

                double x = ppRadiusAB;//we start at angle = 0
                double y = 0;

                //gl->glLineWidth(1);
                //gl->glBegin(GL_LINE_LOOP);
                for (int ii = 0; ii < numSegments; ii++)
                {
                    //output vertex
					pointsList.append(QVector3D(x + radiusPointAB.easting, y + radiusPointAB.northing, 0.0));

                    //apply the rotation matrix
                    double t = x;
                    x = (c * x) - (s * y);
                    y = (s * t) + (c * y);
                }
				if (abBuffer.isCreated())
					abBuffer.destroy();
				abBuffer.create();
				abBuffer.bind();
				abBuffer.allocate(pointsList.constData(), pointsList.size() * sizeof(QVector3D));
				abBuffer.release();

				glDrawArraysColor(gl,mvp,
								  GL_LINE_LOOP, color, abBuffer,
								  GL_FLOAT, pointsList.size(), 1.0f);

                //gl->glEnd();
            }
            //Draw lookahead Point
			QVector3D goalPoint = QVector3D(goalPointAB.easting, goalPointAB.northing, 0.0);
			if (abBuffer.isCreated())
				abBuffer.destroy();
			abBuffer.create();
			abBuffer.bind();
			abBuffer.allocate(&goalPoint, sizeof(QVector3D));
			abBuffer.release();

			color = QColor::fromRgbF(1.0f, 0.5f, 0.95f, 1.0f);
			glDrawArraysColor(gl,mvp,
							  GL_POINTS, color, abBuffer,
							  GL_FLOAT, 1, 4.0f);

			/*
            gl->glPointSize(4.0f);
            gl->glBegin(GL_POINTS);
            gl->glColor3f(1.0f, 0.5f, 0.95f);
            gl->glVertex3d(goalPointAB.easting, goalPointAB.northing, 0.0);
            //gl->glColor(0.6f, 0.95f, 0.95f);
            //gl->glVertex3d(radiusPointAB.easting, radiusPointAB.northing, 0.0);
            gl->glEnd();
            gl->glPointSize(1.0f);
			*/
        }

        //gl->glLineWidth(1);
    }

    //gl->glPopMatrix();
    //gl->glMatrixMode(GL_MODELVIEW);
    //gl->glPopMatrix();

}

void CABLine::resetABLine()
{
    refPoint1 = Vec2(0.2, 0.2);
    refPoint2 = Vec2(0.3, 0.3);

    refABLineP1 = Vec2(0.0, 0.0);
    refABLineP2 = Vec2(0.0, 1.0);

    currentABLineP1 = Vec2(0.0, 0.0);
    currentABLineP2 = Vec2(0.0, 1.0);

    abHeading = 0.0;
    isABLineSet = false;
    isABLineBeingSet = false;
    howManyPathsAway = 0.0;
    passNumber = 0;

}
