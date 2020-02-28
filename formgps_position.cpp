#include "formgps.h"
#include "cnmea.h"
#include "cmodulecomm.h"
#include "cabline.h"
#include "ccontour.h"
#include "cworldgrid.h"
#include "cvehicle.h"
#include "csection.h"
#include "cboundary.h"
#include <QQuickView>
#include <QOpenGLContext>
#include <QPair>
#include "glm.h"
#include <string>
#include "qmlutil.h"
#include "aogrenderer.h"

//called by watchdog timer every 50 ms
bool FormGPS::scanForNMEA()
{
    double nowHz;
    //parse any data from pn.rawBuffer
    //qDebug() << stopwatch.restart();
    pn.parseNMEA();

    //time for a frame update with new valid nmea data
    if (pn.updatedGGA || pn.updatedRMC || pn.updatedOGI)
    {
        //Measure the frequency of the GPS updates
        nowHz = 1000.0 / swFrame.restart(); //millis

        //simple comp filter
        if (nowHz < 20)
            HzTime = 0.95 * HzTime + 0.05 * nowHz;


        //reset  flags
        pn.updatedGGA = false;
        pn.updatedOGI = false;
        pn.updatedRMC = false;

        //update all data for new frame
        updateFixPosition();

        //Update the port connection counter - is reset every time new sentence is valid and ready
        recvCounter++;

        //new position updated
        return true;
    }

    return false;
}

//call for position update after valid NMEA sentence
void FormGPS::updateFixPosition()
{
    startCounter++;
    totalFixSteps = fixUpdateHz * 6;
    if (!isGPSPositionInitialized) {  initializeFirstFewGPSPositions();   return;  }

    //#region Antenna Offset
    if (vehicle.antennaOffset != 0)
    {
        if (vehicle.antennaOffset < 0)
        {
            offset -= 0.01;
            if (offset < vehicle.antennaOffset) offset = vehicle.antennaOffset;
            pn.fix.easting = (cos(-vehicle.fixHeading) * offset) + pn.fix.easting;
            pn.fix.northing = (sin(-vehicle.fixHeading) * offset) + pn.fix.northing;

        }
        else
        {
            offset += 0.01;
            if (offset > vehicle.antennaOffset) offset = vehicle.antennaOffset;
            pn.fix.easting = (cos(-vehicle.fixHeading) * offset) + pn.fix.easting;
            pn.fix.northing = (sin(-vehicle.fixHeading) * offset) + pn.fix.northing;
        }
    }
    //#endregion

    //region Roll
    rollUsed = 0;

    if (ahrs.isRollFromBrick | ahrs.isRollFromAutoSteer | ahrs.isRollFromGPS | ahrs.isRollFromExtUDP)
    {
        rollUsed = ((double)(ahrs.rollX16 - ahrs.rollZeroX16)) * 0.0625;

        //change for roll to the right is positive times -1
        rollCorrectionDistance = sin(glm::toRadians((rollUsed))) * -vehicle.antennaHeight;

        // roll to right is positive Now! Still Important
        pn.fix.easting = (cos(-vehicle.fixHeading) * rollCorrectionDistance) + pn.fix.easting;
        pn.fix.northing = (sin(-vehicle.fixHeading) * rollCorrectionDistance) + pn.fix.northing;
    }

    //region Step Fix

    //**** heading of the vec3 structure is used for distance in Step fix!!!!!

    //grab the most current fix and save the distance from the last fix
    distanceCurrentStepFix = glm::distance(pn.fix, stepFixPts[0]);

    //tree spacing
    if (vehicle.treeSpacing != 0 && tool.section[0].isSectionOn) treeSpacingCounter += (distanceCurrentStepFix * 100);

    //keep the distance below spacing
    if (treeSpacingCounter > vehicle.treeSpacing && vehicle.treeSpacing != 0)
    {
        if (treeTrigger == 0) treeTrigger = 1;
        else treeTrigger = 0;
        while (treeSpacingCounter > vehicle.treeSpacing) treeSpacingCounter -= vehicle.treeSpacing;
    }

    fixStepDist = distanceCurrentStepFix;

    //if  min distance isn't exceeded, keep adding old fixes till it does
    if (distanceCurrentStepFix <= minFixStepDist)
    {
        for (currentStepFix = 0; currentStepFix < totalFixSteps; currentStepFix++)
        {
            fixStepDist += stepFixPts[currentStepFix].heading;
            if (fixStepDist > minFixStepDist)
            {
                //if we reached end, keep the oldest and stay till distance is exceeded
                if (currentStepFix < (totalFixSteps-1) ) currentStepFix++;
                isFixHolding = false;
                break;
            }
            else isFixHolding = true;
        }
    }

    // only takes a single fix to exceeed min distance
    else currentStepFix = 0;

    //if total distance is less then the addition of all the fixes, keep last one as reference
    if (isFixHolding)
    {
        if (isFixHoldLoaded == false)
        {
            vHold = stepFixPts[(totalFixSteps - 1)];
            isFixHoldLoaded = true;
        }

        //cycle thru like normal
        for (int i = totalFixSteps - 1; i > 0; i--) stepFixPts[i] = stepFixPts[i - 1];

        //fill in the latest distance and fix
        stepFixPts[0].heading = glm::distance(pn.fix, stepFixPts[0]);
        stepFixPts[0].easting = pn.fix.easting;
        stepFixPts[0].northing = pn.fix.northing;

        //reload the last position that was triggered.
        stepFixPts[(totalFixSteps - 1)].heading = glm::distance(vHold, stepFixPts[(totalFixSteps - 1)]);
        stepFixPts[(totalFixSteps - 1)].easting = vHold.easting;
        stepFixPts[(totalFixSteps - 1)].northing = vHold.northing;
    }

    else //distance is exceeded, time to do all calcs and next frame
    {
        //positions and headings
        calculatePositionHeading();

        //get rid of hold position
        isFixHoldLoaded = false;

        //don't add the total distance again
        stepFixPts[(totalFixSteps - 1)].heading = 0;

        //grab sentences for logging
        if (isLogNMEA)
        {
            //TODO implement logging
            //logNMEASentence.append(recvSentenceSettings);
        }

        //To prevent drawing high numbers of triangles, determine and test before drawing vertex
        vehicle.sectionTriggerDistance = glm::distance(pn.fix, vehicle.prevSectionPos);
        if (vehicle.sectionTriggerDistance > vehicle.sectionTriggerStepDistance && isJobStarted)
        {
            addSectionContourPathPoints();

            //grab fix and elevation
            /*
            if (isLogElevation) sbFix.append(pn.fix.easting.ToString("N2") + "," + pn.fix.northing.ToString("N2") + ","
                                                + pn.altitude.ToString("N2") + ","
                                                + pn.latitude + "," + pn.longitude + "\r\n");
                                                */
        }
        //test if travelled far enough for new boundary point
        double boundaryDistance = glm::distance(pn.fix, prevBoundaryPos);
        if (boundaryDistance > 1) addBoundaryAndPerimiterPoint();

        //calc distance travelled since last GPS fix
        distance = glm::distance(pn.fix, prevFix);
        if ((fd.distanceUser += distance) > 3000) fd.distanceUser = 0; ;//userDistance can be reset

        //most recent fixes are now the prev ones
        prevFix.easting = pn.fix.easting; prevFix.northing = pn.fix.northing;

        //load up history with valid data
        for (int i = totalFixSteps - 1; i > 0; i--) stepFixPts[i] = stepFixPts[i - 1];
        stepFixPts[0].heading = glm::distance(pn.fix, stepFixPts[0]);
        stepFixPts[0].easting = pn.fix.easting;
        stepFixPts[0].northing = pn.fix.northing;
    }
    //endregion fix

    //region AutoSteer

    //preset the values
    vehicle.guidanceLineDistanceOff = 32000;

    //do the distance from line calculations for contour and AB
    if (ct.isContourBtnOn)
    {
        ct.distanceFromContourLine(vehicle, pn, vehicle.pivotAxlePos, vehicle.steerAxlePos);
    }
    else
    {
        if (curve.isCurveSet)
        {
            curve.getCurrentCurveLine(vehicle.pivotAxlePos, vehicle.steerAxlePos,vehicle, yt, tool, pn);
        }

        if (ABLine.isABLineSet)
        {
            ABLine.getCurrentABLine(vehicle.pivotAxlePos, vehicle.steerAxlePos, vehicle, yt, tool, pn);
            if (yt.isRecordingCustomYouTurn)
            {
                //save reference of first point
                if (yt.youFileList.size() == 0)
                {
                    Vec2 start(vehicle.steerAxlePos.easting, vehicle.steerAxlePos.northing);
                    yt.youFileList.append(start);
                }
                else
                {
                    //keep adding points
                    Vec2 point(vehicle.steerAxlePos.easting - yt.youFileList[0].easting, vehicle.steerAxlePos.northing - yt.youFileList[0].northing);
                    yt.youFileList.append(point);
                }
            }
        }
    }

    // autosteer at full speed of updates
    if (!isAutoSteerBtnOn) //32020 means auto steer is off
    {
        vehicle.guidanceLineDistanceOff = 32020;
    }

    //if the whole path driving driving process is green
    if (recPath.isDrivingRecordedPath) recPath.UpdatePosition(vehicle, bnd, pn, gf, mazeGrid, minFieldX, minFieldY);

    // If Drive button enabled be normal, or just fool the autosteer and fill values
    if (!isInFreeDriveMode)
    {
        if (rollUsed != 0)
        {
            vehicle.guidanceLineSteerAngle = (int)(vehicle.guidanceLineSteerAngle +
                ((-rollUsed) * ((double)mc.autoSteerSettings[mc.ssKd] / 50)) * 500);
        }

        //fill up0 the appropriate arrays with new values
        mc.autoSteerData[mc.sdSpeed] = (char)(pn.speed * 4.0);
        mc.machineControlData[mc.cnSpeed] = mc.autoSteerData[mc.sdSpeed];

        mc.autoSteerData[mc.sdDistanceHi] = (char)(vehicle.guidanceLineDistanceOff >> 8);
        mc.autoSteerData[mc.sdDistanceLo] = (char)(vehicle.guidanceLineDistanceOff);

        mc.autoSteerData[mc.sdSteerAngleHi] = (char)(vehicle.guidanceLineSteerAngle >> 8);
        mc.autoSteerData[mc.sdSteerAngleLo] = (char)(vehicle.guidanceLineSteerAngle);

        //out serial to autosteer module  //indivdual classes load the distance and heading deltas
        autoSteerDataOutToPort();
    }

    else
    {
        //fill up the auto steer array with free drive values
        //fill up the auto steer array with free drive values
        mc.autoSteerData[mc.sdSpeed] = (char)(pn.speed * 4.0 + 16);
        mc.machineControlData[mc.cnSpeed] = mc.autoSteerData[mc.sdSpeed];

        //make steer module think everything is normal
        vehicle.guidanceLineDistanceOff = 0;
        mc.autoSteerData[mc.sdDistanceHi] = (char)(0);
        mc.autoSteerData[mc.sdDistanceLo] = (char)0;

        vehicle.guidanceLineSteerAngle = (int)(driveFreeSteerAngle * 100);
        mc.autoSteerData[mc.sdSteerAngleHi] = (char)(vehicle.guidanceLineSteerAngle >> 8);
        mc.autoSteerData[mc.sdSteerAngleLo] = (char)(vehicle.guidanceLineSteerAngle);

        //out serial to autosteer module  //indivdual classes load the distance and heading deltas
        autoSteerDataOutToPort();
    }

    //for average cross track error
    if (vehicle.guidanceLineDistanceOff < 29000)
    {
        avgXTE[avgXTECntr] = fabs(vehicle.guidanceLineDistanceOff);
        if (avgXTECntr++ > 10) avgXTECntr = 0;
        crossTrackError = 0;
        for (int i = 0; i < 11; i++)
        {
            crossTrackError += (int)avgXTE[i];
        }
        crossTrackError /= 10;
    }
    else
    {
        avgXTE[avgXTECntr] = 0;
        if (avgXTECntr++ > 10) avgXTECntr = 0;
        crossTrackError = 0;
    }

    //#endregion autosteer
    //#region Youturn

    //reset the fault distance to an appropriate weird number
    //-2222 means it fell out of the loop completely
    //-3333 means unable to find a nearest point at all even though inside the work area of field
    // -4444 means cross trac error too high
    distancePivotToTurnLine = -4444;

    //always force out of bounds and change only if in bounds after proven so
    mc.isOutOfBounds = true;

    //if an outer boundary is set, then apply critical stop logic
    if (bnd.bndArr.count() > 0)
    {
        //Are we inside outer and outside inner all turn boundaries, no turn creation problems
        if (isInsideGeoFence() && !yt.isTurnCreationTooClose && !yt.isTurnCreationNotCrossingError)
        {
            //reset critical stop for bounds violation
            mc.isOutOfBounds = false;

            //do the auto youturn logic if everything is on.
            if (yt.isYouTurnBtnOn && isAutoSteerBtnOn)
            {
                //if we are too much off track > 1.3m, kill the diagnostic creation, start again
                if (crossTrackError > 1300 && !yt.isYouTurnTriggered)
                {
                    yt.resetCreatedYouTurn();
                }
                else
                {
                    //now check to make sure we are not in an inner turn boundary - drive thru is ok
                    if (yt.youTurnPhase != 3)
                    {
                        if (crossTrackError > 500)
                        {
                            yt.resetCreatedYouTurn();
                        }
                        else
                        {
                            if (yt.isUsingDubinsTurn)
                            {
                                if (ABLine.isABLineSet) yt.buildABLineDubinsYouTurn(vehicle, tool, bnd, gf,
                                                                                    ABLine, turn, mazeGrid,
                                                                                    minFieldX, minFieldY,
                                                                                    yt.isYouTurnRight);
                                else yt.buildCurveDubinsYouTurn(vehicle, tool, bnd, curve, turn,
                                                                yt.isYouTurnRight, vehicle.pivotAxlePos);
                            }
                            else
                            {
                                if (ABLine.isABLineSet) yt.buildABLinePatternYouTurn(vehicle, tool, bnd,
                                                                                     ABLine, turn, yt.isYouTurnRight);
                                else yt.buildCurvePatternYouTurn(vehicle, tool, bnd, curve, turn,
                                                                 yt.isYouTurnRight, vehicle.pivotAxlePos);
                            }
                        }
                    }
                    else //wait to trigger the actual turn since its made and waiting
                    {
                        //distance from current pivot to first point of youturn pattern
                        distancePivotToTurnLine = glm::distance(yt.ytList[0], vehicle.steerAxlePos);

                        if ((distancePivotToTurnLine <= 20.0) && (distancePivotToTurnLine >= 18.0) && !yt.isYouTurnTriggered)

                            if (!isBoundAlarming)
                            {
                                //sndBoundaryAlarm.Play();
                                isBoundAlarming = true;
                            }

                        //if we are close enough to pattern, trigger.
                        if ((distancePivotToTurnLine <= 1.0) && (distancePivotToTurnLine >= 0) && !yt.isYouTurnTriggered)
                        {
                            yt.youTurnTrigger();
                            isBoundAlarming = false;
                        }
                    }
                }
            } // end of isInWorkingArea
        }
        // here is stop logic for out of bounds - in an inner or out the outer turn border.
        else
        {
            mc.isOutOfBounds = true;
            if (yt.isYouTurnBtnOn)
            {
                yt.resetCreatedYouTurn();
                sim.setSimStepDistance(0 / 17.86);
            }
        }
    }
    else
    {
        mc.isOutOfBounds = false;
    }

    //#endregion

    //openGLControl_Draw routine triggered manually
    update();
    AOGRendererInSG *renderer = qml_root->findChild<AOGRendererInSG *>("openglcontrol");
    renderer->update();

    //since we're in the main thread we can directly call processSectionLookahead()
    //Do we need to move this somewhere else? I don't think so.
    processSectionLookahead();

    //calculate lookahead at full speed, no sentence misses
    //do after the call to processSectionLookahead above.
    calculateSectionLookAhead(vehicle.toolPos.northing, vehicle.toolPos.easting, vehicle.cosSectionHeading, vehicle.sinSectionHeading);

}

void FormGPS::calculatePositionHeading()
{
    if  (headingFromSource == "GPS") {
        vehicle.fixHeading = glm::toRadians(pn.headingTrue);
        camera.camHeading = pn.headingTrue;
        gpsHeading = glm::toRadians(pn.headingTrue);
    } else if (headingFromSource == "HDT") {
            //use NMEA headings for camera and tractor graphic
            vehicle.fixHeading = glm::toRadians(pn.headingHDT);
            camera.camHeading = pn.headingHDT;
            gpsHeading = glm::toRadians(pn.headingHDT);
    } else {
        //default to using "Fix"
        gpsHeading = atan2(pn.fix.easting - stepFixPts[currentStepFix].easting, pn.fix.northing - stepFixPts[currentStepFix].northing);
        if (gpsHeading < 0) gpsHeading += glm::twoPI;
        vehicle.fixHeading = gpsHeading;

        //determine fix positions and heading in degrees for glRotate opengl methods.
        int camStep = currentStepFix * 4;
        if (camStep > (totalFixSteps - 1)) camStep = (totalFixSteps - 1);
        camera.camHeading = atan2(pn.fix.easting - stepFixPts[camStep].easting, pn.fix.northing - stepFixPts[camStep].northing);
        if (camera.camHeading < 0) camera.camHeading += glm::twoPI;
        camera.camHeading = glm::toDegrees(camera.camHeading);
    }

    //an IMU with heading correction, add the correction
    if (ahrs.isHeadingFromBrick || ahrs.isHeadingFromAutoSteer || ahrs.isHeadingFromPAOGI || ahrs.isHeadingFromExtUDP)
    {
        //current gyro angle in radians
        double correctionHeading = (glm::toRadians((double)ahrs.correctionHeadingX16 * 0.0625));

        //Difference between the IMU heading and the GPS heading
        double gyroDelta = (correctionHeading + gyroCorrection) - gpsHeading;
        if (gyroDelta < 0) gyroDelta += glm::twoPI;

        //calculate delta based on circular data problem 0 to 360 to 0, clamp to +- 2 Pi
        if (gyroDelta >= -glm::PIBy2 && gyroDelta <= glm::PIBy2) gyroDelta *= -1.0;
        else
        {
            if (gyroDelta > glm::PIBy2) { gyroDelta = glm::twoPI - gyroDelta; }
            else { gyroDelta = (glm::twoPI + gyroDelta) * -1.0; }
        }
        if (gyroDelta > glm::twoPI) gyroDelta -= glm::twoPI;
        if (gyroDelta < -glm::twoPI) gyroDelta += glm::twoPI;

        //if the gyro and last corrected fix is < 10 degrees, super low pass for gps
        if (fabs(gyroDelta) < 0.18)
        {
            //a bit of delta and add to correction to current gyro
            gyroCorrection += (gyroDelta * (0.25 / fixUpdateHz));
            if (gyroCorrection > glm::twoPI) gyroCorrection -= glm::twoPI;
            if (gyroCorrection < -glm::twoPI) gyroCorrection += glm::twoPI;
        }
        else
        {
            //a bit of delta and add to correction to current gyro
            gyroCorrection += (gyroDelta * (2.0 / fixUpdateHz));
            if (gyroCorrection > glm::twoPI) gyroCorrection -= glm::twoPI;
            if (gyroCorrection < -glm::twoPI) gyroCorrection += glm::twoPI;
        }

        //determine the Corrected heading based on gyro and GPS
        gyroCorrected = correctionHeading + gyroCorrection;
        if (gyroCorrected > glm::twoPI) gyroCorrected -= glm::twoPI;
        if (gyroCorrected < 0) gyroCorrected += glm::twoPI;

        vehicle.fixHeading = gyroCorrected;
    }

    // #region pivot hitch trail

    //translate world to the pivot axle
    vehicle.pivotAxlePos.easting = pn.fix.easting - (sin(vehicle.fixHeading) * vehicle.antennaPivot);
    vehicle.pivotAxlePos.northing = pn.fix.northing - (cos(vehicle.fixHeading) * vehicle.antennaPivot);
    vehicle.pivotAxlePos.heading = vehicle.fixHeading;

    //translate from pivot position to steer axle position
    vehicle.steerAxlePos.easting = vehicle.pivotAxlePos.easting + (sin(vehicle.fixHeading) * vehicle.wheelbase);
    vehicle.steerAxlePos.northing = vehicle.pivotAxlePos.northing + (cos(vehicle.fixHeading) * vehicle.wheelbase);
    vehicle.steerAxlePos.heading = vehicle.fixHeading;

    //determine where the rigid vehicle hitch ends
    vehicle.hitchPos.easting = pn.fix.easting + (sin(vehicle.fixHeading) * (tool.hitchLength - vehicle.antennaPivot));
    vehicle.hitchPos.northing = pn.fix.northing + (cos(vehicle.fixHeading) * (tool.hitchLength - vehicle.antennaPivot));

    //tool attached via a trailing hitch
    if (tool.isToolTrailing)
    {
        double over;

        if (tool.isToolTBT)
        {
            //Torriem rules!!!!! Oh yes, this is all his. Thank-you
            if (distanceCurrentStepFix != 0)
            {
                double t = (tool.toolTankTrailingHitchLength) / distanceCurrentStepFix;
                vehicle.tankPos.easting = vehicle.hitchPos.easting + t * (vehicle.hitchPos.easting - vehicle.tankPos.easting);
                vehicle.tankPos.northing = vehicle.hitchPos.northing + t * (vehicle.hitchPos.northing - vehicle.tankPos.northing);
                vehicle.fixHeadingTank = atan2(vehicle.hitchPos.easting - vehicle.tankPos.easting, vehicle.hitchPos.northing - vehicle.tankPos.northing);
            }

            ////the tool is seriously jacknifed or just starting out so just spring it back.
            over = fabs(M_PI - fabs(fabs(vehicle.fixHeadingTank - vehicle.fixHeading) - M_PI));

            if (over < 2.0 && startCounter > 50)
            {
                vehicle.tankPos.easting = vehicle.hitchPos.easting + (sin(vehicle.fixHeadingTank) * (tool.toolTankTrailingHitchLength));
                vehicle.tankPos.northing = vehicle.hitchPos.northing + (cos(vehicle.fixHeadingTank) * (tool.toolTankTrailingHitchLength));
            }

            //criteria for a forced reset to put tool directly behind vehicle
            if (over > 2.0 || startCounter < 51 )
            {
                vehicle.fixHeadingTank = vehicle.fixHeading;
                vehicle.tankPos.easting = vehicle.hitchPos.easting + (sin(vehicle.tankPos.heading) * (tool.toolTankTrailingHitchLength));
                vehicle.tankPos.northing = vehicle.hitchPos.northing + (cos(vehicle.tankPos.heading) * (tool.toolTankTrailingHitchLength));
            }

        }

        else
        {
            vehicle.tankPos.heading = vehicle.fixHeading;
            vehicle.tankPos.easting = vehicle.hitchPos.easting;
            vehicle.tankPos.northing = vehicle.hitchPos.northing;
        }

        //Torriem rules!!!!! Oh yes, this is all his. Thank-you
        if (distanceCurrentStepFix != 0)
        {
            double t = (tool.toolTrailingHitchLength) / distanceCurrentStepFix;
            vehicle.toolPos.easting = vehicle.tankPos.easting + t * (vehicle.tankPos.easting - vehicle.toolPos.easting);
            vehicle.toolPos.northing = vehicle.tankPos.northing + t * (vehicle.tankPos.northing - vehicle.toolPos.northing);
            vehicle.toolPos.heading = atan2(vehicle.tankPos.easting - vehicle.toolPos.easting, vehicle.tankPos.northing - vehicle.toolPos.northing);
        }

        ////the tool is seriously jacknifed or just starting out so just spring it back.
        over = fabs(M_PI - fabs(fabs(vehicle.toolPos.heading - vehicle.tankPos.heading) - M_PI));

        if (over < 1.9 && startCounter > 50)
        {
            vehicle.toolPos.easting = vehicle.tankPos.easting + (sin(vehicle.toolPos.heading) * (tool.toolTrailingHitchLength));
            vehicle.toolPos.northing = vehicle.tankPos.northing + (cos(vehicle.toolPos.heading) * (tool.toolTrailingHitchLength));
        }

        //criteria for a forced reset to put tool directly behind vehicle
        if (over > 1.9 || startCounter < 51 )
        {
            vehicle.toolPos.heading = vehicle.tankPos.heading;
            vehicle.toolPos.easting = vehicle.tankPos.easting + (sin(vehicle.toolPos.heading) * (tool.toolTrailingHitchLength));
            vehicle.toolPos.northing = vehicle.tankPos.northing + (cos(vehicle.toolPos.heading) * (tool.toolTrailingHitchLength));
        }
    }

    //rigidly connected to vehicle
    else
    {
        vehicle.toolPos.heading = vehicle.fixHeading;
        vehicle.toolPos.easting = vehicle.hitchPos.easting;
        vehicle.toolPos.northing = vehicle.hitchPos.northing;
    }

    //#endregion

    //used to increase triangle count when going around corners, less on straight
    //pick the slow moving side edge of tool
    double distance = tool.toolWidth * 0.5;
    if (distance > 8) distance = 8;

    //whichever is less
    if (tool.toolFarLeftSpeed < tool.toolFarRightSpeed)
    {
        double twist = tool.toolFarLeftSpeed / tool.toolFarRightSpeed;
        //twist *= twist;
        if (twist < 0.2) twist = 0.2;
        vehicle.sectionTriggerStepDistance = distance * twist* twist;
    }
    else
    {
        double twist = tool.toolFarRightSpeed / tool.toolFarLeftSpeed;
        //twist *= twist;
        if (twist < 0.2) twist = 0.2;

        vehicle.sectionTriggerStepDistance = distance * twist*twist;
    }

    //finally determine distance
    if (!curve.isOkToAddPoints) vehicle.sectionTriggerStepDistance = vehicle.sectionTriggerStepDistance + 0.5;
    else vehicle.sectionTriggerStepDistance = 1.0;

    //precalc the sin and cos of heading * -1
    vehicle.sinSectionHeading = sin(-vehicle.toolPos.heading);
    vehicle.cosSectionHeading = cos(-vehicle.toolPos.heading);
}

void FormGPS::addBoundaryAndPerimiterPoint()
{
    //save the north & east as previous
    prevBoundaryPos.easting = pn.fix.easting;
    prevBoundaryPos.northing = pn.fix.northing;

    //build the boundary line
    if (bnd.isOkToAddPoints)
    {
        if (bnd.isDrawRightSide)
        {
            //Right side
            Vec3 point(vehicle.pivotAxlePos.easting + sin(vehicle.pivotAxlePos.heading - glm::PIBy2) * -bnd.createBndOffset,
                       vehicle.pivotAxlePos.northing + cos(vehicle.pivotAxlePos.heading - glm::PIBy2) * -bnd.createBndOffset,
                       vehicle.pivotAxlePos.heading);
            bnd.bndBeingMadePts.append(point);
        }

        //draw on left side
        else
        {
            //Right side
            Vec3 point(vehicle.pivotAxlePos.easting + sin(vehicle.pivotAxlePos.heading - glm::PIBy2) * bnd.createBndOffset,
                       vehicle.pivotAxlePos.northing + cos(vehicle.pivotAxlePos.heading - glm::PIBy2) * bnd.createBndOffset,
                       vehicle.pivotAxlePos.heading);
            bnd.bndBeingMadePts.append(point);
        }

    }
}

//add the points for section, contour line points, Area Calc feature
void FormGPS::addSectionContourPathPoints()
{
    if (recPath.isRecordOn)
    {
        //keep minimum speed of 1.0
        double speed = pn.speed;
        if (pn.speed < 1.0) speed = 1.0;
        bool autoBtn = (autoBtnState == btnStates::Auto);

        CRecPathPt pt(vehicle.steerAxlePos.easting, vehicle.steerAxlePos.northing, vehicle.steerAxlePos.heading, speed, autoBtn);
        recPath.recList.append(pt);
    }

    if (curve.isOkToAddPoints)
    {
        Vec3 pt(vehicle.pivotAxlePos.easting, vehicle.pivotAxlePos.northing, vehicle.pivotAxlePos.heading);
        curve.refList.append(pt);
    }

    //save the north & east as previous
    prevSectionPos.northing = pn.fix.northing;
    prevSectionPos.easting = pn.fix.easting;

    // if non zero, at least one section is on.
    int sectionCounter = 0;

    //send the current and previous GPS fore/aft corrected fix to each section
    for (int j = 0; j < tool.numOfSections + 1; j++)
    {
        if (tool.section[j].isSectionOn)
        {
            tool.section[j].addPathPoint(vehicle, vehicle.toolPos.northing, vehicle.toolPos.easting, vehicle.cosSectionHeading, vehicle.sinSectionHeading);
            sectionCounter++;
        }
    }
    if ((ABLine.isBtnABLineOn && !ct.isContourBtnOn && ABLine.isABLineSet && isAutoSteerBtnOn) ||
                (!ct.isContourBtnOn && curve.isBtnCurveOn && curve.isCurveSet && isAutoSteerBtnOn))
    {
        //no contour recorded
        if (ct.isContourOn) { ct.stopContourLine(vehicle.steerAxlePos, contourSaveList); }
    }
    else
    {
        //Contour Base Track.... At least One section on, turn on if not
        if (sectionCounter != 0)
        {
            //keep the line going, everything is on for recording path
            if (ct.isContourOn) ct.addPoint(vehicle.pivotAxlePos);
            else
            {
                ct.startContourLine(vehicle.pivotAxlePos);
                ct.addPoint(vehicle.pivotAxlePos);
            }
        }

        //All sections OFF so if on, turn off
        else { if (ct.isContourOn) { ct.stopContourLine(vehicle.pivotAxlePos, contourSaveList); } }

        //Build contour line if close enough to a patch
        if (ct.isContourBtnOn) ct.buildContourGuidanceLine(vehicle, tool, pn, vehicle.pivotAxlePos);
    }
}

//calculate the extreme tool left, right velocities, each section lookahead, and whether or not its going backwards
void FormGPS::calculateSectionLookAhead(double northing, double easting, double cosHeading, double sinHeading)
{
    //calculate left side of section 1
    Vec2 left(0, 0);
    Vec2 right = left;
    double leftSpeed = 0, rightSpeed = 0, leftLook = 0, rightLook = 0;

    //now loop all the section rights and the one extreme left
    for (int j = 0; j < tool.numOfSections; j++)
    {
        if (j == 0)
        {
            //only one first left point, the rest are all rights moved over to left
            tool.section[j].leftPoint = Vec2(cosHeading * (tool.section[j].positionLeft) + easting,
                               sinHeading * (tool.section[j].positionLeft) + northing);

            left = tool.section[j].leftPoint - tool.section[j].lastLeftPoint;

            //save a copy for next time
            tool.section[j].lastLeftPoint = tool.section[j].leftPoint;

            //get the speed for left side only once
            leftSpeed = left.getLength() / fixUpdateTime * 10;
            leftLook = leftSpeed * tool.toolLookAhead;

            //save the far left speed
            tool.toolFarLeftSpeed = leftSpeed;


        }
        else
        {
            //right point from last section becomes this left one
            tool.section[j].leftPoint = tool.section[j-1].rightPoint;
            left = tool.section[j].leftPoint - tool.section[j].lastLeftPoint;

            //save a copy for next time
            tool.section[j].lastLeftPoint = tool.section[j].leftPoint;
            leftSpeed = rightSpeed;

        }


        tool.section[j].rightPoint = Vec2(cosHeading * (tool.section[j].positionRight) + easting,
                            sinHeading * (tool.section[j].positionRight) + northing);

        //now we have left and right for this section
        right = tool.section[j].rightPoint - tool.section[j].lastRightPoint;

        //save a copy for next time
        tool.section[j].lastRightPoint = tool.section[j].rightPoint;

        //grab vector length and convert to meters/sec/10 pixels per meter
        rightSpeed = right.getLength() / fixUpdateTime * 10;
        rightLook = rightSpeed * tool.toolLookAhead;

        //Is section outer going forward or backward
        double head = left.headingXZ();
        if (M_PI - fabs(fabs(head - vehicle.toolPos.heading) - M_PI) > glm::PIBy2) leftLook *= -1;

        head = right.headingXZ();
        if (M_PI - fabs(fabs(head - vehicle.toolPos.heading) - M_PI) > glm::PIBy2) rightLook *= -1;

        //choose fastest speed
        if (leftLook > rightLook)  tool.section[j].sectionLookAhead = leftLook;
        else tool.section[j].sectionLookAhead = rightLook;

        if (tool.section[j].sectionLookAhead > 190) tool.section[j].sectionLookAhead = 190;
        //if (tool.section[j].sectionLookAhead == 0)
        //    qDebug() << j << tool.section[j].sectionLookAhead;


        //Doing the slow mo, exceeding buffer so just set as minimum 0.5 meter
        if (currentStepFix >= totalFixSteps - 1) tool.section[j].sectionLookAhead = 5;

    }//endfor

    //set up the super for youturn
    tool.section[tool.numOfSections].isInsideBoundary = true;

    //determine if section is in boundary using the section left/right positions
    bool isLeftIn = true, isRightIn = true;
    for (int j = 0; j < tool.numOfSections; j++)
    {
        if (bnd.bndArr.count())
        {
            //is a headland
            if ( hd.isOn)
            {
                if (j == 0)
                {
                    //only one first left point, the rest are all rights moved over to left
                    isLeftIn = hd.headArr[0].isPointInHeadArea(tool.section[j].leftPoint);
                    isRightIn = hd.headArr[0].isPointInHeadArea(tool.section[j].rightPoint);

                    //merge the two sides into in or out
                    if (!isLeftIn && !isRightIn) tool.section[j].isInsideBoundary = false;
                    else tool.section[j].isInsideBoundary = true;
                }

                else
                {
                    //grab the right of previous section, its the left of this section
                    isLeftIn = isRightIn;
                    isRightIn = hd.headArr[0].isPointInHeadArea(tool.section[j].rightPoint);
                    for (int i = 1; i < hd.headArr.count(); i++)
                    {
                        //inner boundaries should normally NOT have point inside
                        //if (hd.headArr[i].isSet) isRightIn &= !hd.headArr[i].IsPointInHeadArea(tool.section[j].rightPoint);
                    }

                    if (!isLeftIn && !isRightIn) tool.section[j].isInsideBoundary = false;
                    else tool.section[j].isInsideBoundary = true;
                }
                //no boundary created so always inside
                tool.section[tool.numOfSections].isInsideBoundary &= tool.section[j].isInsideBoundary;
            }

            //only outside boundary
            else
            {
                if (j == 0)
                {
                    //only one first left point, the rest are all rights moved over to left
                    isLeftIn = bnd.bndArr[0].isPointInsideBoundary(tool.section[j].leftPoint);
                    isRightIn = bnd.bndArr[0].isPointInsideBoundary(tool.section[j].rightPoint);

                    for (int i = 1; i < bnd.bndArr.count(); i++)
                    {
                        //inner boundaries should normally NOT have point inside
                        if (bnd.bndArr[i].isSet)
                        {
                            isLeftIn &= !bnd.bndArr[i].isPointInsideBoundary(tool.section[j].leftPoint);
                            isRightIn &= !bnd.bndArr[i].isPointInsideBoundary(tool.section[j].rightPoint);
                        }
                    }

                    //merge the two sides into in or out
                    if (isLeftIn && isRightIn) tool.section[j].isInsideBoundary = true;
                    else tool.section[j].isInsideBoundary = false;
                }

                else
                {
                    //grab the right of previous section, its the left of this section
                    isLeftIn = isRightIn;
                    isRightIn = bnd.bndArr[0].isPointInsideBoundary(tool.section[j].rightPoint);
                    for (int i = 1; i < bnd.bndArr.count(); i++)
                    {
                        //inner boundaries should normally NOT have point inside
                        if (bnd.bndArr[i].isSet) isRightIn &= !bnd.bndArr[i].isPointInsideBoundary(tool.section[j].rightPoint);
                    }

                    if (isLeftIn && isRightIn) tool.section[j].isInsideBoundary = true;
                    else tool.section[j].isInsideBoundary = false;
                }
                tool.section[tool.numOfSections].isInsideBoundary &= tool.section[j].isInsideBoundary;
            }
        }
        else
            //no boundary created so always inside
        {
            tool.section[j].isInsideBoundary = true;
            tool.section[tool.numOfSections].isInsideBoundary = false;
        }
    }

    //with left and right tool velocity to determine rate of triangle generation, corners are more
    //save far right speed, 0 if going backwards, in meters/sec
    if (tool.section[tool.numOfSections - 1].sectionLookAhead > 0) tool.toolFarRightSpeed = rightSpeed * 0.1;
    else tool.toolFarRightSpeed = 0;

    //safe left side, 0 if going backwards, in meters/sec convert back from pixels/m
    if (tool.section[0].sectionLookAhead > 0) tool.toolFarLeftSpeed = tool.toolFarLeftSpeed * 0.1;
    else tool.toolFarLeftSpeed = 0;
}

//the start of first few frames to initialize entire program
void FormGPS::initializeFirstFewGPSPositions()
{
    if (!isFirstFixPositionSet)
    {
        //reduce the huge utm coordinates
        pn.utmEast = (int)(pn.fix.easting);
        pn.utmNorth = (int)(pn.fix.northing);
        //qDebug() << pn.utmEast <<" from " << pn.fix.easting;
        pn.fix.easting = pn.fix.easting - pn.utmEast;
        pn.fix.northing = pn.fix.northing - pn.utmNorth;
        //qDebug() << "pn.fix. is now " << pn.fix.easting;
        //Draw a grid once we know where in the world we are.

        //calculate the central meridian of current zone
        pn.centralMeridian = -177 + ((pn.zone - 1) * 6);

        //Azimuth Error - utm declination
        pn.convergenceAngle = atan(sin(glm::toRadians(pn.latitude)) * tan(glm::toRadians(pn.longitude - pn.centralMeridian)));
        //TODO: lblConvergenceAngle.Text = Math.Round(glm::toDegrees(pn.convergenceAngle), 2).ToString();

        //Draw a grid once we know where in the world we are.
        isFirstFixPositionSet = true;
        worldGrid.createWorldGrid(pn.fix.northing, pn.fix.easting);

        //most recent fixes
        prevFix.easting = pn.fix.easting;
        prevFix.northing = pn.fix.northing;

        stepFixPts[0].easting = pn.fix.easting;
        stepFixPts[0].northing = pn.fix.northing;
        stepFixPts[0].heading = 0;

        //run once and return
        isFirstFixPositionSet = true;

        //set up the modules
        mc.resetAllModuleCommValues();

        autoSteerSettingsOutToPort();

        return;
    }

    else
    {

        //most recent fixes
        prevFix.easting = pn.fix.easting; prevFix.northing = pn.fix.northing;

        //load up history with valid data
        for (int i = totalFixSteps - 1; i > 0; i--)
        {
            stepFixPts[i].easting = stepFixPts[i - 1].easting;
            stepFixPts[i].northing = stepFixPts[i - 1].northing;
            stepFixPts[i].heading = stepFixPts[i - 1].heading;
        }

        stepFixPts[0].heading = glm::distance(pn.fix, stepFixPts[0]);
        stepFixPts[0].easting = pn.fix.easting;
        stepFixPts[0].northing = pn.fix.northing;

        //keep here till valid data
        if (startCounter > (totalFixSteps/2.0)) isGPSPositionInitialized = true;

        //in radians
        vehicle.fixHeading = atan2(pn.fix.easting - stepFixPts[totalFixSteps - 1].easting,
                                   pn.fix.northing - stepFixPts[totalFixSteps - 1].northing);
        if (vehicle.fixHeading < 0) vehicle.fixHeading += glm::twoPI;
        vehicle.toolPos.heading = vehicle.fixHeading;

        //send out initial zero settings
        if (isGPSPositionInitialized)
        {
            //set up the modules
            mc.resetAllModuleCommValues();

            autoSteerSettingsOutToPort();

            //TODO: day/night stuff
            /*
            isBetweenSunriseSunset(pn.latitude, pn.longitude);

            //set display accordingly
            isDayTime = (DateTime.Now.Ticks < sunset.Ticks && DateTime.Now.Ticks > sunrise.Ticks);

            lblSunrise.Text = sunrise.ToString("HH:mm");
            lblSunset.Text = sunset.ToString("HH:mm");

            if (isAutoDayNight)
            {
                isDay = isDayTime;
                isDay = !isDay;
                SwapDayNightMode();
            }
            */
        }
        return;
    }
}

bool FormGPS::isInsideGeoFence()
{
    //first where are we, must be inside outer and outside of inner geofence non drive thru turn borders
    if (gf.geoFenceArr[0].isPointInGeoFenceArea(vehicle.pivotAxlePos))
    {
        for (int i = 1; i < bnd.bndArr.count(); i++)
        {
            //make sure not inside a non drivethru boundary
            if (!bnd.bndArr[i].isSet) continue;
            if (bnd.bndArr[i].isDriveThru) continue;
            if (gf.geoFenceArr[i].isPointInGeoFenceArea(vehicle.pivotAxlePos))
            {
                distancePivotToTurnLine = -3333;
                return false;
            }
        }
    }
    else
    {
        distancePivotToTurnLine = -3333;
        return false;
    }
    //we are safely inside outer, outside inner boundaries
    return true;

}
