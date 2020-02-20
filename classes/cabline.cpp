#include "cabline.h"
#include "vec2.h"
#include "glm.h"
#include "cvehicle.h"
#include "cyouturn.h"
#include "cnmea.h"
#include "aogsettings.h"

//#include <QtOpenGL>
#include <QSettings>
#include <QOpenGLFunctions>
#include <QColor>
#include "glutils.h"
#include "ctool.h"
#include "runstate.h"

CABLine::CABLine(CVehicle *v, CYouTurn *t)
    :vehicle(v), yt(t)
{
    //TODO: properties.Settings.Default.setDisplay_lineWidth
    lineWidth = 4;

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
    isABLineLoaded = false;
}

void CABLine::setABLineByPoint()
{
    refPoint2.easting = vehicle->fixEasting;
    refPoint2.northing = vehicle->fixNorthing;

    //calculate the AB Heading
    abHeading = atan2(refPoint2.easting - refPoint1.easting, refPoint2.northing - refPoint1.northing);
    if (abHeading < 0) abHeading += twoPI;

    //sin x cos z for endpoints, opposite for additional lines
    refABLineP1.easting = refPoint1.easting - (sin(abHeading) * 4000.0);
    refABLineP1.northing = refPoint1.northing - (cos(abHeading) * 4000.0);

    refABLineP2.easting = refPoint1.easting + (sin(abHeading) * 4000.0);
    refABLineP2.northing = refPoint1.northing + (cos(abHeading) * 4000.0);

    isABLineSet = true;
    isABLineLoaded = true;
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
    isABLineLoaded = true;
}

void CABLine::snapABLine()
{
    double headingCalc;
    //calculate the heading 90 degrees to ref ABLine heading
    if (isOnRightSideCurrentLine) headingCalc = abHeading + PIBy2;
    else headingCalc = abHeading - PIBy2;

    if (isABSameAsVehicleHeading)
        moveDistance += (distanceFromCurrentLine * 0.001);
    else
        moveDistance -= (distanceFromCurrentLine * 0.001);

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

void CABLine::getCurrentABLine(Vec3 pivot, Vec3 steer, CTool tool) {
    //TODO: tool is an instance of CTool, pass it in?
    //TODO: isStanley used put in vehicle? get from settings?
    AOGSettings a;

    if (vehicle->isStanleyUsed) {
        //move the ABLine over based on the overlap amount set in vehicle
        double widthMinusOverlap = tool.toolWidth - tool.toolOverlap;

        //x2-x1
        double dx = refABLineP2.easting - refABLineP1.easting;
        //z2-z1
        double dy = refABLineP2.northing - refABLineP1.northing;

        //how far are we away from the reference line at 90 degrees
        distanceFromRefLine = ((dy * pivot.easting) - (dx * pivot.northing) + (refABLineP2.easting
                                * refABLineP1.northing) - (refABLineP2.northing * refABLineP1.easting))
                                    / sqrt((dy * dy) + (dx * dx));

        //sign of distance determines which side of line we are on
        if (distanceFromRefLine > 0) refLineSide = 1;
        else refLineSide = -1;

        //absolute the distance
        distanceFromRefLine = fabs(distanceFromRefLine);

        //Which ABLine is the vehicle on, negative is left and positive is right side
        howManyPathsAway = roundAwayFromZero(distanceFromRefLine / widthMinusOverlap);

        //generate that pass number as signed integer
        passNumber = int(refLineSide * howManyPathsAway);

        //calculate the new point that is number of implement widths over
        double toolOffset = tool.toolOffset;
        Vec2 point1;

        //depending which way you are going, the offset can be either side
        if (isABSameAsVehicleHeading)
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
        currentABLineP1.easting = point1.easting - (Math.Sin(abHeading) *   1600.0);
        currentABLineP1.northing = point1.northing - (Math.Cos(abHeading) * 1600.0);

        currentABLineP2.easting = point1.easting + (Math.Sin(abHeading) *   1600.0);
        currentABLineP2.northing = point1.northing + (Math.Cos(abHeading) * 1600.0);

        //get the distance from currently active AB line
        //x2-x1
        dx = currentABLineP2.easting - currentABLineP1.easting;
        //z2-z1
        dy = currentABLineP2.northing - currentABLineP1.northing;

        //save a copy of dx,dy in youTurn
        yt->dxAB = dx; yt->dyAB = dy;

        //how far from current AB Line is fix
        distanceFromCurrentLine = ((dy * steer.easting) - (dx * steer.northing) + (currentABLineP2.easting
                    * currentABLineP1.northing) - (currentABLineP2.northing * currentABLineP1.easting))
                    / sqrt((dy * dy) + (dx * dx));

        //are we on the right side or not
        isOnRightSideCurrentLine = distanceFromCurrentLine > 0;

        //absolute the distance
        distanceFromCurrentLine = fabs(distanceFromCurrentLine);

        //Subtract the two headings, if > 1.57 its going the opposite heading as refAB
        abFixHeadingDelta = (fabs(vehicle->fixHeading - abHeading));
        if (abFixHeadingDelta >= M_PI) abFixHeadingDelta = fabs(abFixHeadingDelta - twoPI);

        isABSameAsVehicleHeading = abFixHeadingDelta < PIBy2;

        // **Stanley Point ** - calc point on ABLine closest to current steer position
        double U = (((steer.easting - currentABLineP1.easting) * dx)
                    + ((steer.northing - currentABLineP1.northing) * dy))
                    / ((dx * dx) + (dy * dy));

        //point on AB line closest to pivot axle point
        rEastAB = currentABLineP1.easting + (U * dx);
        rNorthAB = currentABLineP1.northing + (U * dy);

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
            abFixHeadingDelta = (steer.heading - abHeading + Math.PI);
        }

        //Fix the circular error
        if (abFixHeadingDelta > Math.PI) abFixHeadingDelta -= Math.PI;
        else if (abFixHeadingDelta < Math.PI) abFixHeadingDelta += Math.PI;
n
        if (abFixHeadingDelta > glm.PIBy2) abFixHeadingDelta -= Math.PI;
        else if (abFixHeadingDelta < -glm.PIBy2) abFixHeadingDelta += Math.PI;

        abFixHeadingDelta *= vehicle->stanleyHeadingErrorGain;
        if (abFixHeadingDelta > 0.4) abFixHeadingDelta = 0.4;
        if (abFixHeadingDelta < -0.4) abFixHeadingDelta = -0.4;

        steerAngleAB = Math.Atan((distanceFromCurrentLine * vehicle->stanleyGain) / ((mf.pn.speed * 0.277777) + 1));

        if (steerAngleAB > 0.4) steerAngleAB = 0.4;
        if (steerAngleAB < -0.4) steerAngleAB = -0.4;

        steerAngleAB = glm.toDegrees((steerAngleAB + abFixHeadingDelta) * -1.0);

        if (steerAngleAB < -vehicle->maxSteerAngle) steerAngleAB = -vehicle->maxSteerAngle;
        if (steerAngleAB > vehicle->maxSteerAngle) steerAngleAB = vehicle->maxSteerAngle;

        //Convert to millimeters
        distanceFromCurrentLine = roundAwayFromZero(distanceFromCurrentLine * 1000.0);
    }
    else
    {
        //move the ABLine over based on the overlap amount set in vehicle
        double widthMinusOverlap = tool.toolWidth - tool.toolOverlap;

         //x2-x1
        double dx = refABLineP2.easting - refABLineP1.easting;
        //z2-z1
        double dy = refABLineP2.northing - refABLineP1.northing;

        //how far are we away from the reference line at 90 degrees
        distanceFromRefLine = ((dy * pivot.easting) - (dx * pivot.northing) + (refABLineP2.easting *
                                refABLineP1.northing) - (refABLineP2.northing * refABLineP1.easting)) /
                                    sqrt((dy * dy) + (dx * dx));

        //sign of distance determines which side of line we are on
        if (distanceFromRefLine > 0) refLineSide = 1;
        else refLineSide = -1;

        //absolute the distance
        distanceFromRefLine = fabs(distanceFromRefLine);

        //Which ABLine is the vehicle on, negative is left and positive is right side
        howManyPathsAway = distanceFromRefLine / widthMinusOverlap;

        //generate that pass number as signed integer
        passNumber = int(refLineSide * howManyPathsAway);

        //calculate the new point that is number of implement widths over
        double toolOffset = tool.toolOffset;
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
        currentABLineP1.easting = point1.easting - (sin(abHeading) * 1600.0);
        currentABLineP1.northing = point1.northing - (cos(abHeading) * 1600.0);

        currentABLineP2.easting = point1.easting + (sin(abHeading) * 1600.0);
        currentABLineP2.northing = point1.northing + (cos(abHeading) * 1600.0);

        //get the distance from currently active AB line
        //x2-x1
        dx = currentABLineP2.easting - currentABLineP1.easting;
        //z2-z1
        dy = currentABLineP2.northing - currentABLineP1.northing;

        //save a copy of dx,dy in youTurn
        vehicle->yt->dxAB = dx;
        vehicle->yt->dyAB = dy;

        //how far from current AB Line is fix
        distanceFromCurrentLine = ((dy * pivot.easting) - (dx * pivot.northing)
                                   + (currentABLineP2.easting * currentABLineP1.northing)
                                   - (currentABLineP2.northing * currentABLineP1.easting))
                / sqrt((dy * dy) + (dx * dx));

        //are we on the right side or not
        isOnRightSideCurrentLine = distanceFromCurrentLine > 0;

        //absolute the distance
        distanceFromCurrentLine = fabs(distanceFromCurrentLine);

        //update base on autosteer settings and distance from line
        double goalPointDistance = vehicle->updateGoalPointDistance(distanceFromCurrentLine);
        //TODO: emit a signal?
        mf.lookaheadActual = goalPointDistance;

        //Subtract the two headings, if > 1.57 its going the opposite heading as refAB
        abFixHeadingDelta = (fabs(vehicle->fixHeading - abHeading));
        if (abFixHeadingDelta >= M_PI) abFixHeadingDelta = fabs(abFixHeadingDelta - twoPI);

        // ** Pure pursuit ** - calc point on ABLine closest to current position
        double U = (((pivot.easting - currentABLineP1.easting) * (dx))
                    + ((pivot.northing - currentABLineP1.northing) * (dy)))
                / ((dx * dx) + (dy * dy));

        //point on AB line closest to pivot axle point
        rEastAB = currentABLineP1.easting + (U * dx );
        rNorthAB = currentABLineP1.northing + (U * dy );

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
        double goalPointDistanceDSquared = CNMEA::distanceSquared(goalPointAB.northing, goalPointAB.easting, pivot.northing, pivot.easting);

        //calculate the the new x in local coordinates and steering angle degrees based on wheelbase
        double localHeading = twoPI - vehicle->fixHeading;
        ppRadiusAB = goalPointDistanceDSquared / (2 * (((goalPointAB.easting - pivot.easting) * cos(localHeading)) +
                                                       ((goalPointAB.northing - pivot.northing) * sin(localHeading))));

        steerAngleAB = toDegrees(atan( 2 * (((goalPointAB.easting - pivot.easting) * cos(localHeading))
                                            + ((goalPointAB.northing - pivot.northing) * sin(localHeading)))
                                       * vehicle->wheelbase / goalPointDistanceDSquared)) ;
        if (steerAngleAB < -vehicle->maxSteerAngle) steerAngleAB = -vehicle->maxSteerAngle;
        if (steerAngleAB > vehicle->maxSteerAngle) steerAngleAB = vehicle->maxSteerAngle;

        //limit circle size for display purpose
        if (ppRadiusAB < -500) ppRadiusAB = -500;
        if (ppRadiusAB > 500) ppRadiusAB = 500;

        radiusPointAB.easting = pivot.easting + (ppRadiusAB * cos(localHeading));
        radiusPointAB.northing = pivot.northing + (ppRadiusAB * sin(localHeading));

        //Convert to millimeters
        distanceFromCurrentLine = roundAwayFromZero(distanceFromCurrentLine * 1000.0);

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
            if (isOnRightSideCurrentLine) distanceFromCurrentLine *= -1.0;
        }

        //TODO: make these emitted with a signal?
        vehicle->guidanceLineDistanceOff = int(distanceFromCurrentLine);
        vehicle->guidanceLineSteerAngle = int(steerAngleAB * 10);

        if(yt->isYouTurnTriggered) {
            //do the pure pursuit from youTurn
            yt->distanceFromYouTurnLine(vehicle);
            mf.seq.doSequenceEvent(); //TODO:?

            //now substitute what it thinks are AB line values with auto turn values
            steerAngleAB = yt->steerAngleYT;
            distanceFromCurrentLine = yt->distanceFromCurrentLine;

            goalPointAB = yt->goalPointYT;
            radiusPointAB.easting = yt->radiusPointYT.easting;
            radiusPointAB.northing = yt->radiusPointYT.northing;
            ppRadiusAB = yt->ppRadiusYT;
        }
    }
}

void CABLine::drawABLines(QOpenGLFunctions *gl, const QMatrix4x4 &mvp) {
    QSettings settings;

	QOpenGLBuffer abBuffer;
	QColor color;


    //Draw AB Points
    ColorVertex abpoints[2] = { { QVector3D(refPoint1.easting, refPoint1.northing, 0.0), 
                                  QVector4D(0.95f, 0.0f, 0.0f, 1.0f) },

                                { QVector3D(refPoint2.easting, refPoint2.northing, 0.0),
                                  QVector4D(0.0f, 0.9f, 0.95f, 1.0f) } };

    abBuffer.create();
    abBuffer.bind();
    abBuffer.allocate(abpoints, 2 * sizeof(ColorVertex));
    abBuffer.release();

    glDrawArraysColors(gl, mvp,
                       GL_POINTS, abBuffer,
                       GL_FLOAT, 2, 8.0f);

    //TODO: draw font
    //mf.font.DrawText3D(refPoint1.easting, refPoint1.northing, "&A");
    //mf.font.DrawText3D(refPoint2.easting, refPoint2.northing, "&B");

    //Draw reference AB line

    QVector3D abline[2] = { QVector3D(refABLineP1.easting, refABLineP1.northing, 0),
                            QVector3D(refABLineP2.easting, refABLineP2.northing, 0) };

    if (abBuffer.isCreated())
        abBuffer.destroy();
    abBuffer.create();
    abBuffer.bind();
    abBuffer.allocate(abline, 2 * sizeof(QVector3D));
    abBuffer.release();


    //TODO: make a dotted line in OpenGL ES with shader
    //gl->glLineStipple(1, 0x07F0);
    color = QColor::fromRgbF(0.93f, 0.2f, 0.2f, 1.0f);
    glDrawArraysColor(gl,mvp,
                      GL_LINES, color, abBuffer,
                      GL_FLOAT, 2, 1.0f);

    //draw current AB Line, reuse old array and buffer
    abline[0] = QVector3D(currentABLineP1.easting, currentABLineP1.northing, 0.0);
    abline[1] = QVector3D(currentABLineP2.easting, currentABLineP2.northing, 0.0);

    if (abBuffer.isCreated())
        abBuffer.destroy();
    abBuffer.create();
    abBuffer.bind();
    abBuffer.allocate(abline, 2 * sizeof(QVector3D));
    abBuffer.release();

    color = QColor::fromRgbF(0.95f, 0.0f, 0.95f, 1.0f);
    glDrawArraysColor(gl,mvp,
                      GL_LINES, color, abBuffer,
                      GL_FLOAT, 2, 1.0f);

    if (!isEditing) {
        if (mf.isSideGuideLines && mf.camera.camSetDistance > tool.toolWidth * -120) {
            //get the tool offset and width
            double toolOffset = tool.toolOffset * 2;
            double toolWidth = tool.toolWidth - tool.toolOverlap;
            double cosHeading = cos(-abHeading);
            double sinHeading = sin(-abHeading);

            QVector<QVector3D> vertices;
            if (isABSameAsVehicleHeading) {
                vertices.append(QVector3D((cosHeading * (toolWidth + toolOffset)) + currentABLineP1.easting, (sinHeading * (toolWidth + toolOffset)) + currentABLineP1.northing, 0));
                vertices.append(QVector3D((cosHeading * (toolWidth + toolOffset)) + currentABLineP2.easting, (sinHeading * (toolWidth + toolOffset)) + currentABLineP2.northing, 0));
                vertices.append(QVector3D((cosHeading * (-toolWidth + toolOffset)) + currentABLineP1.easting, (sinHeading * (-toolWidth + toolOffset)) + currentABLineP1.northing, 0));
                vertices.append(QVector3D((cosHeading * (-toolWidth + toolOffset)) + currentABLineP2.easting, (sinHeading * (-toolWidth + toolOffset)) + currentABLineP2.northing, 0));

                toolWidth *= 2;
                vertices.append(QVector3D((cosHeading * toolWidth) + currentABLineP1.easting, (sinHeading * toolWidth) + currentABLineP1.northing, 0));
                vertices.append(QVector3D((cosHeading * toolWidth) + currentABLineP2.easting, (sinHeading * toolWidth) + currentABLineP2.northing, 0));
                vertices.append(QVector3D((cosHeading * (-toolWidth)) + currentABLineP1.easting, (sinHeading * (-toolWidth)) + currentABLineP1.northing, 0));
                vertices.append(QVector3D((cosHeading * (-toolWidth)) + currentABLineP2.easting, (sinHeading * (-toolWidth)) + currentABLineP2.northing, 0));
            } else {
                vertices.append(QVector3D((cosHeading * (toolWidth - toolOffset)) + currentABLineP1.easting, (sinHeading * (toolWidth - toolOffset)) + currentABLineP1.northing, 0));
                vertices.append(QVector3D((cosHeading * (toolWidth - toolOffset)) + currentABLineP2.easting, (sinHeading * (toolWidth - toolOffset)) + currentABLineP2.northing, 0));
                vertices.append(QVector3D((cosHeading * (-toolWidth - toolOffset)) + currentABLineP1.easting, (sinHeading * (-toolWidth - toolOffset)) + currentABLineP1.northing, 0));
                vertices.append(QVector3D((cosHeading * (-toolWidth - toolOffset)) + currentABLineP2.easting, (sinHeading * (-toolWidth - toolOffset)) + currentABLineP2.northing, 0));

                toolWidth *= 2;
                vertices.append(QVector3D((cosHeading * toolWidth) + currentABLineP1.easting, (sinHeading * toolWidth) + currentABLineP1.northing, 0));
                vertices.append(QVector3D((cosHeading * toolWidth) + currentABLineP2.easting, (sinHeading * toolWidth) + currentABLineP2.northing, 0));
                vertices.append(QVector3D((cosHeading * (-toolWidth)) + currentABLineP1.easting, (sinHeading * (-toolWidth)) + currentABLineP1.northing, 0));
                vertices.append(QVector3D((cosHeading * (-toolWidth)) + currentABLineP2.easting, (sinHeading * (-toolWidth)) + currentABLineP2.northing, 0));
            }

            //reuse abBuffer
            if (abBuffer.isCreated())
                abBuffer.destroy();
            abBuffer.create();
            abBuffer.bind();
            abBuffer.allocate(verticies, vertices.length() * sizeof(QVector3D));
            abBuffer.release();

            color = QColor::fromRgbF(0.56f, 0.65f, 0.65f, 1.0f);
            glDrawArraysColor(gl,mvp,
                              GL_LINES, color, abBuffer,
                              GL_FLOAT, vertices.length(), 1.0f);
        }
    }
    if(isEditing) {
        double toolWidth2 = tool.toolWidth - tool.toolOverlap;
        double cosHeading2 = Math.Cos(-mf.ABLine.abHeading);
        double sinHeading2 = Math.Sin(-mf.ABLine.abHeading);

        if (mf.camera.camSetDistance > -200)
        {
            QVector<QVector3D> vertices;

            for (int i = 1; i <= 6; i++)
            {
                vertices.append(QVector3D((cosHeading2 * toolWidth2) + mf.ABLine.refABLineP1.easting, (sinHeading2 * toolWidth2) + mf.ABLine.refABLineP1.northing, 0));
                vertices.append(QVector3D((cosHeading2 * toolWidth2) + mf.ABLine.refABLineP2.easting, (sinHeading2 * toolWidth2) + mf.ABLine.refABLineP2.northing, 0));
                toolWidth2 = toolWidth2 + tool.toolWidth - tool.toolOverlap;
            }

            //reuse abBuffer
            if (abBuffer.isCreated())
                abBuffer.destroy();
            abBuffer.create();
            abBuffer.bind();
            abBuffer.allocate(verticies, vertices.length() * sizeof(QVector3D));
            abBuffer.release();

            color = QColor::fromRgbF(0.9630f, 0.2f, 0.2f, 1.0f);
            //TODO: dotted line with shader
            //GL.Enable(EnableCap.LineStipple);
            glDrawArraysColor(gl,mvp,
                              GL_LINES, color, abBuffer,
                              GL_FLOAT, vertices.length(), mf.ABLine.lineWidth); //master instance of CABLine in main form
        }

    }

    if (mf.isPureDisplayOn && !mf.isStanleyUsed) {
        //Draw lookahead Point
        abline[0] = QVector3D( goalPointAB.easting, goalPointAB.northing, 0.0 );

        //reuse abBuffer
        if (abBuffer.isCreated())
            abBuffer.destroy();
        abBuffer.create();
        abBuffer.bind();
        abBuffer.allocate(abline, 1 * sizeof(QVector3D));
        abBuffer.release();

        color = QColor::fromRgbF(1.0f, 1.0f, 0.0f, 1.0f);

        glDrawArraysColor(gl, mvp,
                          GL_POINTS, color, abBuffer,
                          GL_FLOAT, 2, 1.0f);
    }

    yt->drawYouTurn(gl, mvp);

    if (yt->isRecordingCustomYouTurn) {

        int ptCount = yt->youFileList.length();
        if (ptCount > 1)
        {
            QVector<QVector3d> points;
            for (int i = 1; i < ptCount; i++)
            {
                points.append(QVector3D(mf.yt.youFileList[i].easting + mf.yt.youFileList[0].easting, mf.yt.youFileList[i].northing + mf.yt.youFileList[0].northing, 0));
            }

            //reuse abBuffer
            if (abBuffer.isCreated())
                abBuffer.destroy();
            abBuffer.create();
            abBuffer.bind();
            abBuffer.allocate(points, points.length() * sizeof(QVector3D));
            abBuffer.release();

            color = QColor::fromRgbF(0.05f, 0.05f, 0.95f, 1.0f);

            glDrawArraysColor(gl, mvp,
                              GL_POINTS, color, abBuffer,
                              GL_FLOAT, points.length(), 2.0f);
        }
    }

    if (mf.tram.displayMode == 1 || mf.tram.displayMode == 2 )
        //draw normal tram line
        drawTram(gl, mvp);
    if (mf.tram.displayMode == 1 || mf.tram.displayMode == 3)
        mf.tram.DrawTramBnd();


}

void CABLine::drawTram(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    QVector<QVector3D> vertices;
    QOpenGLBuffer tramBuffer;

    for (int i = 0; i < tramList.Count; i++)
    {
        vertices.clear();
        for (int h = 0; h < tramList[i].Count; h++)
            vertices.append(QVector3D(tramList[i][h].easting, tramList[i][h].northing, 0));

        if (tramBuffer.isCreated())
            tramBuffer.destroy();
        tramBuffer.create();
        tramBuffer.bind();
        tramBuffer.allocate(vertices, vertices.length() * sizeof(QVector3D));
        abBuffer.release();

        color = QColor::fromRgbF(0.8630f, 0.93692f, 0.3260f, 0.22f);
        glDrawArraysColor(gl, mvp,
                          GL_TRIANGLE_STRIP, tramBuffer,
                          GL_FLOAT, vertices.length(), 1.0f);
    }

    //draw tram numbers at end and beggining of line
    if (mf.font.isFontOn)
    {
        //TODO: not implemented yet
        qDebug() << "Font rendering not implemented yet.";

        for (int i = 0; i < tramList.Count; i++)
        {
            int middle = 0;
            /*
            GL.Color4(0.8630f, 0.93692f, 0.8260f, 0.752);
            if (tramList[i].Count > 1)
            {
                middle = tramList[i].Count - 1;
                mf.font.DrawText3D(tramList[i][middle].easting, tramList[i][middle].northing, (i + 1).ToString());
                mf.font.DrawText3D(tramList[i][0].easting, tramList[i][0].northing, (i + 1).ToString());

            }
            */
        }
    }
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
