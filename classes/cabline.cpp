#include "cabline.h"
#include "vec2.h"
#include "glm.h"
#include "cvehicle.h"
#include "cyouturn.h"
#include "ctram.h"
#include "ccamera.h"
#include "aogsettings.h"
#include <QOpenGLFunctions>
#include <QColor>
#include "glutils.h"
#include "ctool.h"
#include "cnmea.h"

//??? why does CABLine refer to mf.ABLine? Isn't there only one instance and
//thus was can just use "this."  If this is wrong, we'll remove this and fix it.

CABLine::CABLine(QObject *parent) : QObject(parent)
{
    USE_SETTINGS;
    lineWidth = SETTINGS_DISPLAY_LINEWIDTH;

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

//called from Main form
void CABLine::setABLineByBPoint(const CVehicle &vehicle)
{
    refPoint2.easting = vehicle.fixEasting;
    refPoint2.northing = vehicle.fixNorthing;

    //calculate the AB Heading
    abHeading = atan2(refPoint2.easting - refPoint1.easting, refPoint2.northing - refPoint1.northing);
    if (abHeading < 0) abHeading += glm::twoPI;

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
    if (isOnRightSideCurrentLine) headingCalc = abHeading + glm::PIBy2;
    else headingCalc = abHeading - glm::PIBy2;

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

//called from main form
void CABLine::getCurrentABLine(Vec3 pivot, Vec3 steer, CVehicle &vehicle, CYouTurn &yt, const CTool &tool, CNMEA &pn, double speed)
{
    if (vehicle.isStanleyUsed) {
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
        howManyPathsAway = glm::roundAwayFromZero(distanceFromRefLine / widthMinusOverlap);

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
        currentABLineP1.easting = point1.easting - (sin(abHeading) *   1600.0);
        currentABLineP1.northing = point1.northing - (cos(abHeading) * 1600.0);

        currentABLineP2.easting = point1.easting + (sin(abHeading) *   1600.0);
        currentABLineP2.northing = point1.northing + (cos(abHeading) * 1600.0);

        //get the distance from currently active AB line
        //x2-x1
        dx = currentABLineP2.easting - currentABLineP1.easting;
        //z2-z1
        dy = currentABLineP2.northing - currentABLineP1.northing;

        //save a copy of dx,dy in youTurn
        yt.dxAB = dx; yt.dyAB = dy;

        //how far from current AB Line is fix
        distanceFromCurrentLine = ((dy * steer.easting) - (dx * steer.northing) + (currentABLineP2.easting
                    * currentABLineP1.northing) - (currentABLineP2.northing * currentABLineP1.easting))
                    / sqrt((dy * dy) + (dx * dx));

        //are we on the right side or not
        isOnRightSideCurrentLine = distanceFromCurrentLine > 0;

        //absolute the distance
        distanceFromCurrentLine = fabs(distanceFromCurrentLine);

        //Subtract the two headings, if > 1.57 its going the opposite heading as refAB
        abFixHeadingDelta = (fabs(vehicle.fixHeading - abHeading));
        if (abFixHeadingDelta >= M_PI) abFixHeadingDelta = fabs(abFixHeadingDelta - glm::twoPI);

        isABSameAsVehicleHeading = abFixHeadingDelta < glm::PIBy2;

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
            abFixHeadingDelta = (steer.heading - abHeading + M_PI);
        }

        //Fix the circular error
        if (abFixHeadingDelta > M_PI) abFixHeadingDelta -= M_PI;
        else if (abFixHeadingDelta < M_PI) abFixHeadingDelta += M_PI;

        if (abFixHeadingDelta > glm::PIBy2) abFixHeadingDelta -= M_PI;
        else if (abFixHeadingDelta < -glm::PIBy2) abFixHeadingDelta += M_PI;

        abFixHeadingDelta *= vehicle.stanleyHeadingErrorGain;
        if (abFixHeadingDelta > 0.4) abFixHeadingDelta = 0.4;
        if (abFixHeadingDelta < -0.4) abFixHeadingDelta = -0.4;

        steerAngleAB = atan((distanceFromCurrentLine * vehicle.stanleyGain) / ((speed * 0.277777) + 1));

        if (steerAngleAB > 0.4) steerAngleAB = 0.4;
        if (steerAngleAB < -0.4) steerAngleAB = -0.4;

        steerAngleAB = glm::toDegrees((steerAngleAB + abFixHeadingDelta) * -1.0);

        if (steerAngleAB < -vehicle.maxSteerAngle) steerAngleAB = -vehicle.maxSteerAngle;
        if (steerAngleAB > vehicle.maxSteerAngle) steerAngleAB = vehicle.maxSteerAngle;

        //Convert to millimeters
        distanceFromCurrentLine = glm::roundAwayFromZero(distanceFromCurrentLine * 1000.0);
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
        yt.dxAB = dx;
        yt.dyAB = dy;

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
        double goalPointDistance = vehicle.updateGoalPointDistance(pn, distanceFromCurrentLine);

        //mf.lookaheadActual = goalPointDistance;
        //not needed; vehicle.updateGoalPointDistance does this for us

        //Subtract the two headings, if > 1.57 its going the opposite heading as refAB
        abFixHeadingDelta = (fabs(vehicle.fixHeading - abHeading));
        if (abFixHeadingDelta >= M_PI) abFixHeadingDelta = fabs(abFixHeadingDelta - glm::twoPI);

        // ** Pure pursuit ** - calc point on ABLine closest to current position
        double U = (((pivot.easting - currentABLineP1.easting) * (dx))
                    + ((pivot.northing - currentABLineP1.northing) * (dy)))
                / ((dx * dx) + (dy * dy));

        //point on AB line closest to pivot axle point
        rEastAB = currentABLineP1.easting + (U * dx );
        rNorthAB = currentABLineP1.northing + (U * dy );

        if (abFixHeadingDelta >= glm::PIBy2)
        {
            isABSameAsVehicleHeading = false;
            goalPointAB.easting = rEastAB - (sin(abHeading) * goalPointDistance);
            goalPointAB.northing = rNorthAB - (cos(abHeading) * goalPointDistance);
        }
        else
        {
            isABSameAsVehicleHeading = true;
            goalPointAB.easting = rEastAB + (sin(abHeading) * goalPointDistance);
            goalPointAB.northing = rNorthAB + (cos(abHeading) * goalPointDistance);
        }

        //calc "D" the distance from pivot axle to lookahead point
        double goalPointDistanceDSquared = glm::distanceSquared(goalPointAB.northing, goalPointAB.easting, pivot.northing, pivot.easting);

        //calculate the the new x in local coordinates and steering angle degrees based on wheelbase
        double localHeading = glm::twoPI - vehicle.fixHeading;
        ppRadiusAB = goalPointDistanceDSquared / (2 * (((goalPointAB.easting - pivot.easting) * cos(localHeading)) +
                                                       ((goalPointAB.northing - pivot.northing) * sin(localHeading))));

        steerAngleAB = glm::toDegrees(atan( 2 * (((goalPointAB.easting - pivot.easting) * cos(localHeading))
                                            + ((goalPointAB.northing - pivot.northing) * sin(localHeading)))
                                       * vehicle.wheelbase / goalPointDistanceDSquared)) ;
        if (steerAngleAB < -vehicle.maxSteerAngle) steerAngleAB = -vehicle.maxSteerAngle;
        if (steerAngleAB > vehicle.maxSteerAngle) steerAngleAB = vehicle.maxSteerAngle;

        //limit circle size for display purpose
        if (ppRadiusAB < -500) ppRadiusAB = -500;
        if (ppRadiusAB > 500) ppRadiusAB = 500;

        radiusPointAB.easting = pivot.easting + (ppRadiusAB * cos(localHeading));
        radiusPointAB.northing = pivot.northing + (ppRadiusAB * sin(localHeading));

        //Convert to millimeters
        distanceFromCurrentLine = glm::roundAwayFromZero(distanceFromCurrentLine * 1000.0);

        //angular velocity in rads/sec  = 2PI * m/sec * radians/meters
        angVel = glm::twoPI * 0.277777 * vehicle.speed * (tan(glm::toRadians(steerAngleAB)))/vehicle.wheelbase;

        //clamp the steering angle to not exceed safe angular velocity
        if (fabs(angVel) > vehicle.maxAngularVelocity)
        {
            steerAngleAB = glm::toDegrees(steerAngleAB > 0 ? (atan((vehicle.wheelbase * vehicle.maxAngularVelocity) / (glm::twoPI * vehicle.speed * 0.277777)))
                : (atan((vehicle.wheelbase * -vehicle.maxAngularVelocity) / (glm::twoPI * vehicle.speed * 0.277777))));
        }

        //distance is negative if on left, positive if on right
        if (isABSameAsVehicleHeading)
        {
            if (!isOnRightSideCurrentLine) distanceFromCurrentLine *= -1.0;
        }

        //opposite way so right is left
        else
        {
            if (isOnRightSideCurrentLine) distanceFromCurrentLine *= -1.0;
        }

        //TODO: make these CYouTurn &ytignal?
        vehicle.guidanceLineDistanceOff = int(distanceFromCurrentLine);
        vehicle.guidanceLineSteerAngle = int(steerAngleAB * 100);

        if(yt.isYouTurnTriggered) {
            //do the pure pursuit from youTurn
            yt.distanceFromYouTurnLine(vehicle,pn);
            //mf.seq.doSequenceEvent(); //TODO:?
            emit doSequence(yt);

            //now substitute what it thinks are AB line values with auto turn values
            steerAngleAB = yt.steerAngleYT;
            distanceFromCurrentLine = yt.distanceFromCurrentLine;

            goalPointAB = yt.goalPointYT;
            radiusPointAB.easting = yt.radiusPointYT.easting;
            radiusPointAB.northing = yt.radiusPointYT.northing;
            ppRadiusAB = yt.ppRadiusYT;
        }
    }
}

void CABLine::drawABLines(QOpenGLFunctions *gl, const QMatrix4x4 &mvp,
                          const CVehicle &vehicle, const CTool &tool,
                          CYouTurn &yt, CTram &tram, const CCamera &camera,
                          bool isSideGuideLines) {
    USE_SETTINGS;

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
        if (isSideGuideLines && camera.camSetDistance > tool.toolWidth * -120) {
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
            abBuffer.allocate(vertices.data(), vertices.length() * sizeof(QVector3D));
            abBuffer.release();

            color = QColor::fromRgbF(0.56f, 0.65f, 0.65f, 1.0f);
            glDrawArraysColor(gl,mvp,
                              GL_LINES, color, abBuffer,
                              GL_FLOAT, vertices.length(), 1.0f);
        }
    }
    if(isEditing) {
        double toolWidth2 = tool.toolWidth - tool.toolOverlap;
        double cosHeading2 = cos(-abHeading);
        double sinHeading2 = sin(-abHeading);

        if (camera.camSetDistance > -200)
        {
            QVector<QVector3D> vertices;

            for (int i = 1; i <= 6; i++)
            {
                vertices.append(QVector3D((cosHeading2 * toolWidth2) + refABLineP1.easting, (sinHeading2 * toolWidth2) + refABLineP1.northing, 0));
                vertices.append(QVector3D((cosHeading2 * toolWidth2) + refABLineP2.easting, (sinHeading2 * toolWidth2) + refABLineP2.northing, 0));
                toolWidth2 = toolWidth2 + tool.toolWidth - tool.toolOverlap;
            }

            //reuse abBuffer
            if (abBuffer.isCreated())
                abBuffer.destroy();
            abBuffer.create();
            abBuffer.bind();
            abBuffer.allocate(vertices.data(), vertices.length() * sizeof(QVector3D));
            abBuffer.release();

            color = QColor::fromRgbF(0.9630f, 0.2f, 0.2f, 1.0f);
            //TODO: dotted line with shader
            //GL.Enable(EnableCap.LineStipple);
            gl->glLineWidth(SETTINGS_DISPLAY_LINEWIDTH);
            glDrawArraysColor(gl,mvp,
                              GL_LINES, color, abBuffer,
                              GL_FLOAT, vertices.length(),
                              SETTINGS_DISPLAY_LINEWIDTH);
        }

    }

    if (SETTINGS_DISPLAY_ISPUREON && !vehicle.isStanleyUsed) {
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

    yt.drawYouTurn(gl, mvp);

    if (yt.isRecordingCustomYouTurn) {

        int ptCount = yt.youFileList.length();
        if (ptCount > 1)
        {
            QVector<QVector3D> points;
            for (int i = 1; i < ptCount; i++)
            {
                points.append(QVector3D(yt.youFileList[i].easting + yt.youFileList[0].easting, yt.youFileList[i].northing + yt.youFileList[0].northing, 0));
            }

            //reuse abBuffer
            if (abBuffer.isCreated())
                abBuffer.destroy();
            abBuffer.create();
            abBuffer.bind();
            abBuffer.allocate(points.data(), points.length() * sizeof(QVector3D));
            abBuffer.release();

            color = QColor::fromRgbF(0.05f, 0.05f, 0.95f, 1.0f);

            glDrawArraysColor(gl, mvp,
                              GL_POINTS, color, abBuffer,
                              GL_FLOAT, points.length(), 2.0f);
        }
    }

    if (tram.displayMode == 1 || tram.displayMode == 2 )
        //draw normal tram line
        drawTram(gl, mvp);
    if (tram.displayMode == 1 || tram.displayMode == 3)
        tram.drawTramBnd(gl,mvp);


}

void CABLine::drawTram(QOpenGLFunctions *gl, const QMatrix4x4 &mvp)
{
    QVector<QVector3D> vertices;
    QOpenGLBuffer tramBuffer;
    QColor color;
    USE_SETTINGS;

    for (int i = 0; i < tramList.size(); i++)
    {
        vertices.clear();
        for (int h = 0; h < tramList[i].size(); h++)
            vertices.append(QVector3D(tramList[i][h].easting, tramList[i][h].northing, 0));

        if (tramBuffer.isCreated())
            tramBuffer.destroy();
        tramBuffer.create();
        tramBuffer.bind();
        tramBuffer.allocate(vertices.data(), vertices.length() * sizeof(QVector3D));
        tramBuffer.release();

        color = QColor::fromRgbF(0.8630f, 0.93692f, 0.3260f, 0.22f);
        glDrawArraysColor(gl, mvp,
                          GL_TRIANGLE_STRIP, color,
                          tramBuffer,
                          GL_FLOAT, vertices.length(), 1.0f);
    }

    //draw tram numbers at end and beggining of line
    if (SETTINGS_DISPLAY_ISFONTON)
    {
        //TODO: not implemented yet
        qDebug() << "Font rendering not implemented yet.";

        /*
        for (int i = 0; i < tramList.size(); i++)
        {
            int middle = 0;
            //GL.Color4(0.8630f, 0.93692f, 0.8260f, 0.752);
            if (tramList[i].size() > 1)
            {
                middle = tramList[i].size() - 1;
                mf.font.DrawText3D(tramList[i][middle].easting, tramList[i][middle].northing, QString(i + 1));
                mf.font.DrawText3D(tramList[i][0].easting, tramList[i][0].northing, QString(i + 1));

            }
        }
        */
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
