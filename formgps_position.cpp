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
#include "aogproperty.h"
#include "cpgn.h"

//called for every new GPS or simulator position
void FormGPS::UpdateFixPosition()
{
    QLocale locale;

    //swFrame.Stop();
    //Measure the frequency of the GPS updates
    //timeSliceOfLastFix = (double)(swFrame.elapsed()) / 1000;
    swFrame.restart();
    lock.lockForWrite(); //stop GL from updating while we calculate a new position

    //get Hz from timeslice
    //nowHz = 1 / timeSliceOfLastFix;
    nowHz = 1000.0 / swFrame.elapsed(); //convert ms into hz
    if (nowHz > 20) nowHz = 20;
    if (nowHz < 3) nowHz = 3;

    //simple comp filter
    gpsHz = 0.98 * gpsHz + 0.02 * nowHz;

    //Initialization counter
    startCounter++;

    if (!isGPSPositionInitialized)
    {
        InitializeFirstFewGPSPositions();
        lock.unlock();
        return;
    }

    pn.speed = pn.vtgSpeed;
    vehicle.AverageTheSpeed(pn.speed);

    /*
    //GPS is valid, let's bootstrap the demo field if needed
    if(bootstrap_field)
    {
        fileCreateField();
        fileSaveABLines();
        bootstrap_field = false;
    }
    */

    //#region Heading
    //calculate current heading only when moving, otherwise use last
    if (headingFromSource == "Fix")
    {
        //#region Start

        distanceCurrentStepFixDisplay = glm::Distance(prevDistFix, pn.fix);
        if ((fd.distanceUser += distanceCurrentStepFixDisplay) > 999) fd.distanceUser = 0;
        distanceCurrentStepFixDisplay *= 100;

        prevDistFix = pn.fix;

        if (fabs(vehicle.avgSpeed) < 1.5 && !isFirstHeadingSet)
            goto byPass;

        if (!isFirstHeadingSet) //set in steer settings, Stanley
        {
            prevFix.easting = stepFixPts[0].easting; prevFix.northing = stepFixPts[0].northing;

            if (stepFixPts[2].isSet == 0)
            {
                //this is the first position no roll or offset correction
                if (stepFixPts[0].isSet == 0)
                {
                    stepFixPts[0].easting = pn.fix.easting;
                    stepFixPts[0].northing = pn.fix.northing;
                    stepFixPts[0].isSet = 1;
                    lock.unlock();
                    return;
                }

                //and the second
                if (stepFixPts[1].isSet == 0)
                {
                    for (int i = totalFixSteps - 1; i > 0; i--) stepFixPts[i] = stepFixPts[i - 1];
                    stepFixPts[0].easting = pn.fix.easting;
                    stepFixPts[0].northing = pn.fix.northing;
                    stepFixPts[0].isSet = 1;
                    lock.unlock();
                    return;
                }

                //the critcal moment for checking initial direction/heading.
                for (int i = totalFixSteps - 1; i > 0; i--) stepFixPts[i] = stepFixPts[i - 1];
                stepFixPts[0].easting = pn.fix.easting;
                stepFixPts[0].northing = pn.fix.northing;
                stepFixPts[0].isSet = 1;

                gpsHeading = atan2(pn.fix.easting - stepFixPts[2].easting,
                                        pn.fix.northing - stepFixPts[2].northing);

                if (gpsHeading < 0) gpsHeading += glm::twoPI;
                else if (gpsHeading > glm::twoPI) gpsHeading -= glm::twoPI;

                vehicle.fixHeading = gpsHeading;

                //set the imu to gps heading offset
                if (ahrs.imuHeading != 99999)
                {
                    double imuHeading = (glm::toRadians(ahrs.imuHeading));
                    imuGPS_Offset = 0;

                    //Difference between the IMU heading and the GPS heading
                    double gyroDelta = (imuHeading + imuGPS_Offset) - gpsHeading;

                    if (gyroDelta < 0) gyroDelta += glm::twoPI;
                    else if (gyroDelta > glm::twoPI) gyroDelta -= glm::twoPI;

                    //calculate delta based on circular data problem 0 to 360 to 0, clamp to +- 2 Pi
                    if (gyroDelta >= -glm::PIBy2 && gyroDelta <= glm::PIBy2) gyroDelta *= -1.0;
                    else
                    {
                        if (gyroDelta > glm::PIBy2) { gyroDelta = glm::twoPI - gyroDelta; }
                        else { gyroDelta = (glm::twoPI + gyroDelta) * -1.0; }
                    }
                    if (gyroDelta > glm::twoPI) gyroDelta -= glm::twoPI;
                    else if (gyroDelta < -glm::twoPI) gyroDelta += glm::twoPI;

                    //moe the offset to line up imu with gps
                    imuGPS_Offset = (gyroDelta);
                    //rounding a floating point number doesn't make sense.
                    //imuGPS_Offset = Math.Round(imuGPS_Offset, 6);

                    if (imuGPS_Offset >= glm::twoPI) imuGPS_Offset -= glm::twoPI;
                    else if (imuGPS_Offset <= 0) imuGPS_Offset += glm::twoPI;

                    //determine the Corrected heading based on gyro and GPS
                    imuCorrected = imuHeading + imuGPS_Offset;
                    if (imuCorrected > glm::twoPI) imuCorrected -= glm::twoPI;
                    else if (imuCorrected < 0) imuCorrected += glm::twoPI;

                    vehicle.fixHeading = imuCorrected;
                }

                //set the camera
                camera.camHeading = glm::toDegrees(gpsHeading);

                //now we have a heading, fix the first 3
                if (vehicle.antennaOffset != 0)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        stepFixPts[i].easting = (cos(-gpsHeading) * vehicle.antennaOffset) + stepFixPts[i].easting;
                        stepFixPts[i].northing = (sin(-gpsHeading) * vehicle.antennaOffset) + stepFixPts[i].northing;
                    }
                }

                if (ahrs.imuRoll != 88888)
                {
                    //change for roll to the right is positive times -1
                    rollCorrectionDistance = tan(glm::toRadians((ahrs.imuRoll))) * -vehicle.antennaHeight;

                    // roll to left is positive  **** important!!
                    // not any more - April 30, 2019 - roll to right is positive Now! Still Important
                    for (int i = 0; i < 3; i++)
                    {
                        stepFixPts[i].easting = (cos(-gpsHeading) * rollCorrectionDistance) + stepFixPts[i].easting;
                        stepFixPts[i].northing = (sin(-gpsHeading) * rollCorrectionDistance) + stepFixPts[i].northing;
                    }
                }

                //get the distance from first to 2nd point, update fix with new offset/roll
                stepFixPts[0].distance = glm::Distance(stepFixPts[1], stepFixPts[0]);
                pn.fix.easting = stepFixPts[0].easting;
                pn.fix.northing = stepFixPts[0].northing;

                isFirstHeadingSet = true;
                TimedMessageBox(2000, "Direction Reset", "Forward is Set");

                lastGPS = pn.fix;

                lock.unlock();
                return;
            }
        }
        //#endregion

        //#region Offset Roll
        if (vehicle.antennaOffset != 0)
        {
            pn.fix.easting = (cos(-gpsHeading) * vehicle.antennaOffset) + pn.fix.easting;
            pn.fix.northing = (sin(-gpsHeading) * vehicle.antennaOffset) + pn.fix.northing;
        }

        uncorrectedEastingGraph = pn.fix.easting;

        //originalEasting = pn.fix.easting;
        if (ahrs.imuRoll != 88888)
        {
            //change for roll to the right is positive times -1
            rollCorrectionDistance = sin(glm::toRadians((ahrs.imuRoll))) * -vehicle.antennaHeight;
            correctionDistanceGraph = rollCorrectionDistance;

            pn.fix.easting = (cos(-gpsHeading) * rollCorrectionDistance) + pn.fix.easting;
            pn.fix.northing = (sin(-gpsHeading) * rollCorrectionDistance) + pn.fix.northing;
        }

        //#endregion

        //#region Fix Heading

        double minFixHeadingDistSquared;
        double newGPSHeading;
        double imuHeading;
        double camDelta;
        double gyroDelta;

        //imu on board
        if (ahrs.imuHeading != 99999)
        {
            //how far since last fix
            distanceCurrentStepFix = glm::Distance(stepFixPts[0], pn.fix);

            if (distanceCurrentStepFix < gpsMinimumStepDistance)
            {
                goto byPass;
            }

            //userDistance can be reset

            minFixHeadingDistSquared = minHeadingStepDist * minHeadingStepDist;
            fixToFixHeadingDistance = 0;

            for (int i = 0; i < totalFixSteps; i++)
            {
                fixToFixHeadingDistance = glm::DistanceSquared(stepFixPts[i], pn.fix);
                currentStepFix = i;

                if (fixToFixHeadingDistance > minFixHeadingDistSquared)
                {
                    break;
                }
            }

            if (fixToFixHeadingDistance < (minFixHeadingDistSquared * 0.5))
                goto byPass;

            newGPSHeading = atan2(pn.fix.easting - stepFixPts[currentStepFix].easting,
                                              pn.fix.northing - stepFixPts[currentStepFix].northing);
            if (newGPSHeading < 0) newGPSHeading += glm::twoPI;

            ////what is angle between the last valid heading before stopping and one just now
            delta = fabs(M_PI - fabs(fabs(newGPSHeading - imuCorrected) - M_PI));

            //ie change in direction
            if (delta > 1.57) //
            {
                vehicle.isReverse = true;
                newGPSHeading += M_PI;
                if (newGPSHeading < 0) newGPSHeading += glm::twoPI;
                else if (newGPSHeading >= glm::twoPI) newGPSHeading -= glm::twoPI;
                isReverseWithIMU = true;
            }
            else
            {
                vehicle.isReverse = false;
                isReverseWithIMU = false;
            }

            if (vehicle.isReverse)
                newGPSHeading -= glm::toRadians(vehicle.antennaPivot / 1
                                               * mc.actualSteerAngleDegrees * ahrs.reverseComp);
            else
                newGPSHeading -= glm::toRadians(vehicle.antennaPivot / 1
                                               * mc.actualSteerAngleDegrees * ahrs.forwardComp);

            if (newGPSHeading < 0) newGPSHeading += glm::twoPI;
            else if (newGPSHeading >= glm::twoPI) newGPSHeading -= glm::twoPI;

            gpsHeading = newGPSHeading;

            //#region IMU Fusion

            // IMU Fusion with heading correction, add the correction
            //current gyro angle in radians
            imuHeading = (glm::toRadians(ahrs.imuHeading));

            //Difference between the IMU heading and the GPS heading
            gyroDelta = 0;

            if (!isReverseWithIMU)
                gyroDelta = (imuHeading + imuGPS_Offset) - gpsHeading;
            else
            {
                gyroDelta = 0;
            }

            if (gyroDelta < 0) gyroDelta += glm::twoPI;
            else if (gyroDelta > glm::twoPI) gyroDelta -= glm::twoPI;

            //calculate delta based on circular data problem 0 to 360 to 0, clamp to +- 2 Pi
            if (gyroDelta >= -glm::PIBy2 && gyroDelta <= glm::PIBy2) gyroDelta *= -1.0;
            else
            {
                if (gyroDelta > glm::PIBy2) { gyroDelta = glm::twoPI - gyroDelta; }
                else { gyroDelta = (glm::twoPI + gyroDelta) * -1.0; }
            }
            if (gyroDelta > glm::twoPI) gyroDelta -= glm::twoPI;
            else if (gyroDelta < -glm::twoPI) gyroDelta += glm::twoPI;

            //moe the offset to line up imu with gps
            imuGPS_Offset += (gyroDelta * (ahrs.fusionWeight));
            //imuGPS_Offset += (gyroDelta * (0.06));

            if (imuGPS_Offset > glm::twoPI) imuGPS_Offset -= glm::twoPI;
            else if (imuGPS_Offset < 0) imuGPS_Offset += glm::twoPI;

            //determine the Corrected heading based on gyro and GPS
            imuCorrected = imuHeading + imuGPS_Offset;
            if (imuCorrected > glm::twoPI) imuCorrected -= glm::twoPI;
            else if (imuCorrected < 0) imuCorrected += glm::twoPI;

            //use imu as heading when going slow
            vehicle.fixHeading = imuCorrected;

            //#endregion
        }
        else
        {
            //how far since last fix
            distanceCurrentStepFix = glm::Distance(stepFixPts[0], pn.fix);

            if (distanceCurrentStepFix < (gpsMinimumStepDistance))
                goto byPass;

            minFixHeadingDistSquared = minHeadingStepDist * minHeadingStepDist;
            fixToFixHeadingDistance = 0;

            for (int i = 0; i < totalFixSteps; i++)
            {
                fixToFixHeadingDistance = glm::DistanceSquared(stepFixPts[i], pn.fix);
                currentStepFix = i;

                if (fixToFixHeadingDistance > minFixHeadingDistSquared)
                {
                    break;
                }
            }

            if (fixToFixHeadingDistance < minFixHeadingDistSquared * 0.5)
                goto byPass;

            newGPSHeading = atan2(pn.fix.easting - stepFixPts[currentStepFix].easting,
                                              pn.fix.northing - stepFixPts[currentStepFix].northing);
            if (newGPSHeading < 0) newGPSHeading += glm::twoPI;

            ////what is angle between the last valid heading before stopping and one just now
            delta = fabs(M_PI - fabs(fabs(newGPSHeading - gpsHeading) - M_PI));

            filteredDelta = delta * 0.2 + filteredDelta * 0.8;

            //filtered delta different then delta
            if (fabs(filteredDelta - delta) > 0.5)
            {
                isChangingDirection = true;
            }
            else
            {
                isChangingDirection = false;
            }

            //we can't be sure if changing direction so do nothing
            if (isChangingDirection)
                goto byPass;

            //ie change in direction
            if (filteredDelta > 1.57) //
            {
                vehicle.isReverse = true;
                newGPSHeading += M_PI;
                if (newGPSHeading < 0) newGPSHeading += glm::twoPI;
                else if (newGPSHeading >= glm::twoPI) newGPSHeading -= glm::twoPI;
            }
            else
                vehicle.isReverse = false;

            if (vehicle.isReverse)
                newGPSHeading -= glm::toRadians(vehicle.antennaPivot / 1
                                               * mc.actualSteerAngleDegrees * ahrs.reverseComp);
            else
                newGPSHeading -= glm::toRadians(vehicle.antennaPivot / 1
                                               * mc.actualSteerAngleDegrees * ahrs.forwardComp);

            if (newGPSHeading < 0) newGPSHeading += glm::twoPI;
            else if (newGPSHeading >= glm::twoPI) newGPSHeading -= glm::twoPI;

            //set the headings
            vehicle.fixHeading = gpsHeading = newGPSHeading;
        }

        //save current fix and set as valid
        for (int i = totalFixSteps - 1; i > 0; i--) stepFixPts[i] = stepFixPts[i - 1];
        stepFixPts[0].easting = pn.fix.easting;
        stepFixPts[0].northing = pn.fix.northing;
        stepFixPts[0].isSet = 1;

        //#endregion

        //#region Camera

        camDelta = vehicle.fixHeading - smoothCamHeading;

        if (camDelta < 0) camDelta += glm::twoPI;
        else if (camDelta > glm::twoPI) camDelta -= glm::twoPI;

        //calculate delta based on circular data problem 0 to 360 to 0, clamp to +- 2 Pi
        if (camDelta >= -glm::PIBy2 && camDelta <= glm::PIBy2) camDelta *= -1.0;
        else
        {
            if (camDelta > glm::PIBy2) { camDelta = glm::twoPI - camDelta; }
            else { camDelta = (glm::twoPI + camDelta) * -1.0; }
        }
        if (camDelta > glm::twoPI) camDelta -= glm::twoPI;
        else if (camDelta < -glm::twoPI) camDelta += glm::twoPI;

        smoothCamHeading -= camDelta * camera.camSmoothFactor;

        if (smoothCamHeading > glm::twoPI) smoothCamHeading -= glm::twoPI;
        else if (smoothCamHeading < -glm::twoPI) smoothCamHeading += glm::twoPI;

        camera.camHeading = glm::toDegrees(smoothCamHeading);

        //#endregion


        //Calculate a million other things
    byPass:
        if (ahrs.imuHeading != 99999)
        {
            imuCorrected = (glm::toRadians(ahrs.imuHeading)) + imuGPS_Offset;
            if (imuCorrected > glm::twoPI) imuCorrected -= glm::twoPI;
            else if (imuCorrected < 0) imuCorrected += glm::twoPI;

            //use imu as heading when going slow
            vehicle.fixHeading = imuCorrected;
        }

        camDelta = vehicle.fixHeading - smoothCamHeading;

        if (camDelta < 0) camDelta += glm::twoPI;
        else if (camDelta > glm::twoPI) camDelta -= glm::twoPI;

        //calculate delta based on circular data problem 0 to 360 to 0, clamp to +- 2 Pi
        if (camDelta >= -glm::PIBy2 && camDelta <= glm::PIBy2) camDelta *= -1.0;
        else
        {
            if (camDelta > glm::PIBy2) { camDelta = glm::twoPI - camDelta; }
            else { camDelta = (glm::twoPI + camDelta) * -1.0; }
        }
        if (camDelta > glm::twoPI) camDelta -= glm::twoPI;
        else if (camDelta < -glm::twoPI) camDelta += glm::twoPI;

        smoothCamHeading -= camDelta * camera.camSmoothFactor;

        if (smoothCamHeading > glm::twoPI) smoothCamHeading -= glm::twoPI;
        else if (smoothCamHeading < -glm::twoPI) smoothCamHeading += glm::twoPI;

        camera.camHeading = glm::toDegrees(smoothCamHeading);

        TheRest();
    } else if (headingFromSource == "VTG")
    {
        isFirstHeadingSet = true;
        if (vehicle.avgSpeed > 1)
        {
            //use NMEA headings for camera and tractor graphic
            vehicle.fixHeading = glm::toRadians(pn.headingTrue);
            camera.camHeading = pn.headingTrue;
            gpsHeading = vehicle.fixHeading;
        }

        //grab the most current fix to last fix distance
        distanceCurrentStepFix = glm::Distance(pn.fix, prevFix);

        //#region Antenna Offset

        if (vehicle.antennaOffset != 0)
        {
            pn.fix.easting = (cos(-vehicle.fixHeading) * vehicle.antennaOffset) + pn.fix.easting;
            pn.fix.northing = (sin(-vehicle.fixHeading) * vehicle.antennaOffset) + pn.fix.northing;
        }
        //#endregion

        uncorrectedEastingGraph = pn.fix.easting;

        //an IMU with heading correction, add the correction
        if (ahrs.imuHeading != 99999)
        {
            //current gyro angle in radians
            double correctionHeading = (glm::toRadians(ahrs.imuHeading));

            //Difference between the IMU heading and the GPS heading
            double gyroDelta = (correctionHeading + imuGPS_Offset) - gpsHeading;
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
                imuGPS_Offset += (gyroDelta * (0.1));
                if (imuGPS_Offset > glm::twoPI) imuGPS_Offset -= glm::twoPI;
                if (imuGPS_Offset < -glm::twoPI) imuGPS_Offset += glm::twoPI;
            }
            else
            {
                //a bit of delta and add to correction to current gyro
                imuGPS_Offset += (gyroDelta * (0.2));
                if (imuGPS_Offset > glm::twoPI) imuGPS_Offset -= glm::twoPI;
                if (imuGPS_Offset < -glm::twoPI) imuGPS_Offset += glm::twoPI;
            }

            //determine the Corrected heading based on gyro and GPS
            imuCorrected = correctionHeading + imuGPS_Offset;
            if (imuCorrected > glm::twoPI) imuCorrected -= glm::twoPI;
            if (imuCorrected < 0) imuCorrected += glm::twoPI;

            vehicle.fixHeading = imuCorrected;

            camera.camHeading = vehicle.fixHeading;
            if (camera.camHeading > glm::twoPI) camera.camHeading -= glm::twoPI;
            camera.camHeading = glm::toDegrees(camera.camHeading);
        }


        //#region Roll

        if (ahrs.imuRoll != 88888)
        {
            //change for roll to the right is positive times -1
            rollCorrectionDistance = sin(glm::toRadians((ahrs.imuRoll))) * -vehicle.antennaHeight;
            correctionDistanceGraph = rollCorrectionDistance;

            // roll to left is positive  **** important!!
            // not any more - April 30, 2019 - roll to right is positive Now! Still Important
            pn.fix.easting = (cos(-vehicle.fixHeading) * rollCorrectionDistance) + pn.fix.easting;
            pn.fix.northing = (sin(-vehicle.fixHeading) * rollCorrectionDistance) + pn.fix.northing;
        }

        //#endregion Roll

        TheRest();

        //most recent fixes are now the prev ones
        prevFix.easting = pn.fix.easting; prevFix.northing = pn.fix.northing;

    } else if (headingFromSource == "Dual")
    {
        isFirstHeadingSet = true;
        //use Dual Antenna heading for camera and tractor graphic
        vehicle.fixHeading = glm::toRadians(pn.headingTrueDual);
        gpsHeading = vehicle.fixHeading;

        uncorrectedEastingGraph = pn.fix.easting;

        if (vehicle.antennaOffset != 0)
        {
            pn.fix.easting = (cos(-vehicle.fixHeading) * vehicle.antennaOffset) + pn.fix.easting;
            pn.fix.northing = (sin(-vehicle.fixHeading) * vehicle.antennaOffset) + pn.fix.northing;
        }

        if (ahrs.imuRoll != 88888 && vehicle.antennaHeight != 0)
        {

            //change for roll to the right is positive times -1
            rollCorrectionDistance = sin(glm::toRadians((ahrs.imuRoll))) * -vehicle.antennaHeight;
            correctionDistanceGraph = rollCorrectionDistance;

            pn.fix.easting = (cos(-gpsHeading) * rollCorrectionDistance) + pn.fix.easting;
            pn.fix.northing = (sin(-gpsHeading) * rollCorrectionDistance) + pn.fix.northing;
        }

        //grab the most current fix and save the distance from the last fix
        distanceCurrentStepFix = glm::Distance(pn.fix, prevDistFix);

        //userDistance can be reset
        if ((fd.distanceUser += distanceCurrentStepFix) > 999) fd.distanceUser = 0;

        distanceCurrentStepFixDisplay = distanceCurrentStepFix * 100;
        prevDistFix = pn.fix;

        if (glm::DistanceSquared(lastReverseFix, pn.fix) > 0.20)
        {
            //most recent heading
            double newHeading = atan2(pn.fix.easting - lastReverseFix.easting,
                                           pn.fix.northing - lastReverseFix.northing);

            if (newHeading < 0) newHeading += glm::twoPI;


            //what is angle between the last reverse heading and current dual heading
            double delta = fabs(M_PI - fabs(fabs(newHeading - vehicle.fixHeading) - M_PI));

            //are we going backwards
            vehicle.isReverse = delta > 2 ? true : false;

            //save for next meter check
            lastReverseFix = pn.fix;
        }

        double camDelta = vehicle.fixHeading - smoothCamHeading;

        if (camDelta < 0) camDelta += glm::twoPI;
        else if (camDelta > glm::twoPI) camDelta -= glm::twoPI;

        //calculate delta based on circular data problem 0 to 360 to 0, clamp to +- 2 Pi
        if (camDelta >= -glm::PIBy2 && camDelta <= glm::PIBy2) camDelta *= -1.0;
        else
        {
            if (camDelta > glm::PIBy2) { camDelta = glm::twoPI - camDelta; }
            else { camDelta = (glm::twoPI + camDelta) * -1.0; }
        }
        if (camDelta > glm::twoPI) camDelta -= glm::twoPI;
        else if (camDelta < -glm::twoPI) camDelta += glm::twoPI;

        smoothCamHeading -= camDelta * camera.camSmoothFactor;

        if (smoothCamHeading > glm::twoPI) smoothCamHeading -= glm::twoPI;
        else if (smoothCamHeading < -glm::twoPI) smoothCamHeading += glm::twoPI;

        camera.camHeading = glm::toDegrees(smoothCamHeading);

        TheRest();
    }
    //else {
    //}

    //preset the values
    vehicle.guidanceLineDistanceOff = 32000;

    if (ct.isContourBtnOn)
    {
        ct.DistanceFromContourLine(isAutoSteerBtnOn, vehicle, yt, ahrs, pn, vehicle.pivotAxlePos, vehicle.steerAxlePos);
    }
    else
    {
        if (curve.isCurveSet && curve.isBtnCurveOn)
        {
            //do the calcs for AB Curve
            curve.GetCurrentCurveLine(vehicle.pivotAxlePos, vehicle.steerAxlePos, secondsSinceStart, isAutoSteerBtnOn, mc.steerSwitchHigh, vehicle, bnd,yt,ahrs,gyd,pn);
        }

        if (ABLine.isABLineSet && ABLine.isBtnABLineOn)
        {
            ABLine.GetCurrentABLine(vehicle.pivotAxlePos, vehicle.steerAxlePos, secondsSinceStart, isAutoSteerBtnOn, mc.steerSwitchHigh, vehicle, yt, ahrs, gyd, pn);
        }
    }

    // autosteer at full speed of updates

    //if the whole path driving driving process is green
    if (recPath.isDrivingRecordedPath) recPath.UpdatePosition(vehicle, yt, isAutoSteerBtnOn);

    // If Drive button off - normal autosteer
    if (!vehicle.isInFreeDriveMode)
    {
        //fill up0 the appropriate arrays with new values
        p_254.pgn[p_254.speedHi] = (char)((int)(fabs(vehicle.avgSpeed) * 10.0) >> 8);
        p_254.pgn[p_254.speedLo] = (char)((int)(fabs(vehicle.avgSpeed) * 10.0));
        //mc.machineControlData[mc.cnSpeed] = mc.autoSteerData[mc.sdSpeed];

        //save distance for display
        //lightbarDistance = vehicle.guidanceLineDistanceOff;

        if (!isAutoSteerBtnOn) //32020 means auto steer is off
        {
            vehicle.guidanceLineDistanceOff = 32020;
            p_254.pgn[p_254.status] = 0;
        }

        else p_254.pgn[p_254.status] = 1;

        if (recPath.isDrivingRecordedPath || recPath.isFollowingDubinsToPath) p_254.pgn[p_254.status] = 1;

        //mc.autoSteerData[7] = unchecked((byte)(vehicle.guidanceLineDistanceOff >> 8));
        //mc.autoSteerData[8] = unchecked((byte)(vehicle.guidanceLineDistanceOff));

        //convert to cm from mm and divide by 2 - lightbar
        int distanceX2;
        if (vehicle.guidanceLineDistanceOff == 32020 || vehicle.guidanceLineDistanceOff == 32000)
            distanceX2 = 255;

        else
        {
            distanceX2 = (int)(vehicle.guidanceLineDistanceOff * 0.05);

            if (distanceX2 < -127) distanceX2 = -127;
            else if (distanceX2 > 127) distanceX2 = 127;
            distanceX2 += 127;
        }

        p_254.pgn[p_254.lineDistance] = (char)distanceX2;

        if (!timerSim.isActive())
        {
            if (isAutoSteerBtnOn && vehicle.avgSpeed > vehicle.maxSteerSpeed)
            {
                onStopAutoSteer();
                if (isMetric)
                    TimedMessageBox(3000, tr("AutoSteer Disabled"), tr("Above Maximum Safe Steering Speed: ") + locale.toString(vehicle.maxSteerSpeed, 'g', 1) + tr(" Kmh"));
                else
                    TimedMessageBox(3000, tr("AutoSteer Disabled"), tr("Above Maximum Safe Steering Speed: ") + locale.toString(vehicle.maxSteerSpeed * 0.621371, 'g', 1) + tr(" MPH"));
            }

            if (isAutoSteerBtnOn && vehicle.avgSpeed < vehicle.minSteerSpeed)
            {
                minSteerSpeedTimer++;
                if (minSteerSpeedTimer > 80)
                {
                    onStopAutoSteer();
                    if (isMetric)
                        TimedMessageBox(3000, tr("AutoSteer Disabled"), tr("Below Minimum Safe Steering Speed: ") + locale.toString(vehicle.maxSteerSpeed, 'g', 1) + tr(" Kmh"));
                    else
                        TimedMessageBox(3000, tr("AutoSteer Disabled"), tr("Below Minimum Safe Steering Speed: ") + locale.toString(vehicle.maxSteerSpeed * 0.621371, 'g', 1) + tr(" MPH"));
                }
            }
            else
            {
                minSteerSpeedTimer = 0;
            }
        }

        double tanSteerAngle = tan(glm::toRadians(((double)(vehicle.guidanceLineSteerAngle)) * 0.01));
        double tanActSteerAngle = tan(glm::toRadians(mc.actualSteerAngleDegrees));

        setAngVel = 0.277777 * vehicle.avgSpeed * tanSteerAngle / vehicle.wheelbase;
        actAngVel = glm::toDegrees(0.277777 * vehicle.avgSpeed * tanActSteerAngle / vehicle.wheelbase);


        isMaxAngularVelocity = false;
        //greater then settings rads/sec limit steer angle
        if (fabs(setAngVel) > vehicle.maxAngularVelocity)
        {
            setAngVel = vehicle.maxAngularVelocity;
            tanSteerAngle = 3.6 * setAngVel * vehicle.wheelbase / vehicle.avgSpeed;
            if (vehicle.guidanceLineSteerAngle < 0)
                vehicle.guidanceLineSteerAngle = (short)(glm::toDegrees(atan(tanSteerAngle)) * -100);
            else
                vehicle.guidanceLineSteerAngle = (short)(glm::toDegrees(atan(tanSteerAngle)) * 100);
            isMaxAngularVelocity = true;
        }

        setAngVel = glm::toDegrees(setAngVel);

        p_254.pgn[p_254.steerAngleHi] = (char)(vehicle.guidanceLineSteerAngle >> 8);
        p_254.pgn[p_254.steerAngleLo] = (char)(vehicle.guidanceLineSteerAngle);

        if (isChangingDirection && ahrs.imuHeading == 99999)
            p_254.pgn[p_254.status] = 0;

        //for now if backing up, turn off autosteer
        if (!isSteerInReverse)
        {
            if (vehicle.isReverse) p_254.pgn[p_254.status] = 0;
        }
    }

    else //Drive button is on
    {
        //fill up the auto steer array with free drive values
        p_254.pgn[p_254.speedHi] = (char)((int)(80) >> 8);
        p_254.pgn[p_254.speedLo] = (char)((int)(80));

        //turn on status to operate
        p_254.pgn[p_254.status] = 1;

        //send the steer angle
        vehicle.guidanceLineSteerAngle = (qint16)(vehicle.driveFreeSteerAngle * 100);

        p_254.pgn[p_254.steerAngleHi] = (char)(vehicle.guidanceLineSteerAngle >> 8);
        p_254.pgn[p_254.steerAngleLo] = (char)(vehicle.guidanceLineSteerAngle);


    }

    //out serial to autosteer module  //indivdual classes load the distance and heading deltas
    SendPgnToLoop(p_254.pgn);

    //for average cross track error
    if (vehicle.guidanceLineDistanceOff < 29000)
    {
        crossTrackError = (int)((double)crossTrackError * 0.90 + fabs((double)vehicle.guidanceLineDistanceOff) * 0.1);
    }
    else
    {
        crossTrackError = 0;
    }

    //#endregion

    //#region AutoSteer

    //preset the values
    vehicle.guidanceLineDistanceOff = 32000;

    if (ct.isContourBtnOn)
    {
        ct.DistanceFromContourLine(isAutoSteerBtnOn, vehicle, yt, ahrs, pn, vehicle.pivotAxlePos, vehicle.steerAxlePos);
    }
    else
    {
        if (curve.isCurveSet && curve.isBtnCurveOn)
        {
            //do the calcs for AB Curve
            curve.GetCurrentCurveLine(vehicle.pivotAxlePos, vehicle.steerAxlePos, secondsSinceStart, isAutoSteerBtnOn, mc.steerSwitchHigh, vehicle, bnd, yt, ahrs, gyd, pn);
        }

        if (ABLine.isABLineSet && ABLine.isBtnABLineOn)
        {
            ABLine.GetCurrentABLine(vehicle.pivotAxlePos, vehicle.steerAxlePos, secondsSinceStart, isAutoSteerBtnOn, mc.steerSwitchHigh, vehicle, yt, ahrs, gyd, pn);
        }
    }

    // autosteer at full speed of updates

    //if the whole path driving driving process is green
    if (recPath.isDrivingRecordedPath) recPath.UpdatePosition(vehicle, yt, isAutoSteerBtnOn);

    // If Drive button off - normal autosteer
    if (!vehicle.isInFreeDriveMode)
    {
        //fill up0 the appropriate arrays with new values
        p_254.pgn[p_254.speedHi] = (char)((int)(fabs(vehicle.avgSpeed) * 10.0) >> 8);
        p_254.pgn[p_254.speedLo] = (char)((int)(fabs(vehicle.avgSpeed) * 10.0));
        //mc.machineControlData[mc.cnSpeed] = mc.autoSteerData[mc.sdSpeed];

        //save distance for display
        lightbarDistance = vehicle.guidanceLineDistanceOff;

        if (!isAutoSteerBtnOn) //32020 means auto steer is off
        {
            vehicle.guidanceLineDistanceOff = 32020;
            p_254.pgn[p_254.status] = 0;
        }

        else p_254.pgn[p_254.status] = 1;

        if (recPath.isDrivingRecordedPath || recPath.isFollowingDubinsToPath) p_254.pgn[p_254.status] = 1;

        //mc.autoSteerData[7] = unchecked((byte)(vehicle.guidanceLineDistanceOff >> 8));
        //mc.autoSteerData[8] = unchecked((byte)(vehicle.guidanceLineDistanceOff));

        //convert to cm from mm and divide by 2 - lightbar
        int distanceX2;
        if (vehicle.guidanceLineDistanceOff == 32020 || vehicle.guidanceLineDistanceOff == 32000)
            distanceX2 = 255;

        else
        {
            distanceX2 = (int)(vehicle.guidanceLineDistanceOff * 0.05);

            if (distanceX2 < -127) distanceX2 = -127;
            else if (distanceX2 > 127) distanceX2 = 127;
            distanceX2 += 127;
        }

        p_254.pgn[p_254.lineDistance] = (char)distanceX2;

        if (!timerSim.isActive())
        {
            if (isAutoSteerBtnOn && vehicle.avgSpeed > vehicle.maxSteerSpeed)
            {
                onStopAutoSteer();

                if (isMetric)
                    TimedMessageBox(3000, tr("AutoSteer Disabled"), tr("Above Maximum Safe Steering Speed: ") + locale.toString(vehicle.maxSteerSpeed, 'g', 1) + tr(" Kmh"));
                else
                    TimedMessageBox(3000, tr("AutoSteer Disabled"), tr("Above Maximum Safe Steering Speed: ") + locale.toString(vehicle.maxSteerSpeed * 0.621371, 'g', 1) + tr(" MPH"));
            }

            if (isAutoSteerBtnOn && vehicle.avgSpeed < vehicle.minSteerSpeed)
            {
                minSteerSpeedTimer++;
                if (minSteerSpeedTimer > 80)
                {
                    onStopAutoSteer();
                    if (isMetric)
                        TimedMessageBox(3000, tr("AutoSteer Disabled"), tr("Below Minimum Safe Steering Speed: ") + locale.toString(vehicle.maxSteerSpeed, 'g', 1) + tr(" Kmh"));
                    else
                        TimedMessageBox(3000, tr("AutoSteer Disabled"), tr("Below Minimum Safe Steering Speed: ") + locale.toString(vehicle.maxSteerSpeed * 0.621371, 'g', 1) + tr(" MPH"));
                }
            }
            else
            {
                minSteerSpeedTimer = 0;
            }
        }

        double tanSteerAngle = tan(glm::toRadians(((double)(vehicle.guidanceLineSteerAngle)) * 0.01));
        double tanActSteerAngle = tan(glm::toRadians(mc.actualSteerAngleDegrees));

        setAngVel = 0.277777 * vehicle.avgSpeed * tanSteerAngle / vehicle.wheelbase;
        actAngVel = glm::toDegrees(0.277777 * vehicle.avgSpeed * tanActSteerAngle / vehicle.wheelbase);


        isMaxAngularVelocity = false;
        //greater then settings rads/sec limit steer angle
        if (fabs(setAngVel) > vehicle.maxAngularVelocity)
        {
            setAngVel = vehicle.maxAngularVelocity;
            tanSteerAngle = 3.6 * setAngVel * vehicle.wheelbase / vehicle.avgSpeed;
            if (vehicle.guidanceLineSteerAngle < 0)
                vehicle.guidanceLineSteerAngle = (short)(glm::toDegrees(atan(tanSteerAngle)) * -100);
            else
                vehicle.guidanceLineSteerAngle = (short)(glm::toDegrees(atan(tanSteerAngle)) * 100);
            isMaxAngularVelocity = true;
        }

        setAngVel = glm::toDegrees(setAngVel);

        p_254.pgn[p_254.steerAngleHi] = (char)(vehicle.guidanceLineSteerAngle >> 8);
        p_254.pgn[p_254.steerAngleLo] = (char)(vehicle.guidanceLineSteerAngle);

        if (isChangingDirection && ahrs.imuHeading == 99999)
            p_254.pgn[p_254.status] = 0;

        //for now if backing up, turn off autosteer
        if (!isSteerInReverse)
        {
            if (vehicle.isReverse) p_254.pgn[p_254.status] = 0;
        }
    }

    else //Drive button is on
    {
        //fill up the auto steer array with free drive values
        p_254.pgn[p_254.speedHi] = (char)((int)(80) >> 8);
        p_254.pgn[p_254.speedLo] = (char)((int)(80));

        //turn on status to operate
        p_254.pgn[p_254.status] = 1;

        //send the steer angle
        vehicle.guidanceLineSteerAngle = (qint16)(vehicle.driveFreeSteerAngle * 100);

        p_254.pgn[p_254.steerAngleHi] = (char)(vehicle.guidanceLineSteerAngle >> 8);
        p_254.pgn[p_254.steerAngleLo] = (char)(vehicle.guidanceLineSteerAngle);


    }

    //out serial to autosteer module  //indivdual classes load the distance and heading deltas
    SendPgnToLoop(p_254.pgn);

    //for average cross track error
    if (vehicle.guidanceLineDistanceOff < 29000)
    {
        crossTrackError = (int)((double)crossTrackError * 0.90 + fabs((double)vehicle.guidanceLineDistanceOff) * 0.1);
    }
    else
    {
        crossTrackError = 0;
    }

    //#endregion

    //#region Youturn

    //reset the fault distance to an appropriate weird number
    //-2222 means it fell out of the loop completely
    //-3333 means unable to find a nearest point at all even though inside the work area of field
    // -4444 means cross trac error too high
    distancePivotToTurnLine = -4444;

    //if an outer boundary is set, then apply critical stop logic
    if (bnd.bndList.count() > 0)
    {
        //check if inside all fence
        if (!yt.isYouTurnBtnOn)
        {
            mc.isOutOfBounds = !bnd.IsPointInsideFenceArea(vehicle.pivotAxlePos);
        }
        else //Youturn is on
        {
            bool isInTurnBounds = bnd.IsPointInsideTurnArea(vehicle.pivotAxlePos) != -1;
            //Are we inside outer and outside inner all turn boundaries, no turn creation problems
            //if we are too much off track > 1.3m, kill the diagnostic creation, start again
            //if (!yt.isYouTurnTriggered)
            if (isInTurnBounds)
            {
                mc.isOutOfBounds = false;
                //now check to make sure we are not in an inner turn boundary - drive thru is ok
                if (yt.youTurnPhase != 3)
                {
                    if (crossTrackError > 500)
                    {
                        yt.ResetCreatedYouTurn();
                    }
                    else
                    {
                        if (ABLine.isABLineSet)
                        {
                            yt.BuildABLineDubinsYouTurn(vehicle, bnd, ABLine, yt.isYouTurnRight);
                        }
                        else yt.BuildCurveDubinsYouTurn(vehicle, bnd, curve, yt.isYouTurnRight, vehicle.pivotAxlePos);
                    }

                    if (yt.uTurnStyle == 0 && yt.youTurnPhase == 3) yt.SmoothYouTurn(yt.uTurnSmoothing);
                }
                else //wait to trigger the actual turn since its made and waiting
                {
                    //distance from current pivot to first point of youturn pattern
                    distancePivotToTurnLine = glm::Distance(yt.ytList[5], vehicle.pivotAxlePos);

                    if ((distancePivotToTurnLine <= 20.0) && (distancePivotToTurnLine >= 18.0) && !yt.isYouTurnTriggered)

                    /* TODO: sounds
                    if (!sounds.isBoundAlarming)
                    {
                        if (sounds.isTurnSoundOn) sounds.sndBoundaryAlarm.Play();
                        sounds.isBoundAlarming = true;
                    }*/

                    //if we are close enough to pattern, trigger.
                    if ((distancePivotToTurnLine <= 1.0) && (distancePivotToTurnLine >= 0) && !yt.isYouTurnTriggered)
                    {
                        yt.YouTurnTrigger(vehicle, ABLine, curve);
                        //TODO: sounds
                        //sounds.isBoundAlarming = false;
                    }

                    if (isAutoSteerBtnOn && vehicle.guidanceLineDistanceOff > 300 && !yt.isYouTurnTriggered)
                    {
                        yt.ResetCreatedYouTurn();
                    }
                }
            }
            else
            {
                if (!yt.isYouTurnTriggered)
                {
                    yt.ResetCreatedYouTurn();
                    mc.isOutOfBounds = !bnd.IsPointInsideFenceArea(vehicle.pivotAxlePos);
                }

            }

            //}
            //// here is stop logic for out of bounds - in an inner or out the outer turn border.
            //else
            //{
            //    //mc.isOutOfBounds = true;
            //    if (isAutoSteerBtnOn)
            //    {
            //        if (yt.isYouTurnBtnOn)
            //        {
            //            yt.ResetCreatedYouTurn();
            //            //sim.stepDistance = 0 / 17.86;
            //        }
            //    }
            //    else
            //    {
            //        yt.isTurnCreationTooClose = false;
            //    }

            //}
        }
    }
    else
    {
        mc.isOutOfBounds = false;
    }

    //#endregion

    //update main window
    //oglMain.MakeCurrent();
    //oglMain.Refresh();

    AOGRendererInSG *renderer = qml_root->findChild<AOGRendererInSG *>("openglcontrol");
    renderer->update();

    //since we're in the main thread we can directly call processSectionLookahead()
    //the code in processSectionLookahead was moved from AOG's oglBack_Paint because
    //opengl drawing occurs in a separate thread in Qt.  So I broke the code out into
    //its own function so we can call it from here.
    processSectionLookahead();


    //end of UppdateFixPosition

    //stop the timer and calc how long it took to do calcs and draw
    frameTimeRough = swFrame.elapsed();

    if (frameTimeRough > 50) frameTimeRough = 50;
    frameTime = frameTime * 0.90 + frameTimeRough * 0.1;
    lock.unlock();
    newframe = true;

}

void FormGPS::TheRest()
{
    //positions and headings
    CalculatePositionHeading();

    //calculate lookahead at full speed, no sentence misses
    CalculateSectionLookAhead(vehicle.toolPos.northing, vehicle.toolPos.easting, vehicle.cosSectionHeading, vehicle.sinSectionHeading);

    //To prevent drawing high numbers of triangles, determine and test before drawing vertex
    sectionTriggerDistance = glm::Distance(pn.fix, prevSectionPos);
    contourTriggerDistance = glm::Distance(pn.fix, prevContourPos);

    //contour points
    if (isJobStarted &&(contourTriggerDistance > tool.contourWidth
                         || contourTriggerDistance > sectionTriggerStepDistance))
    {
        AddContourPoints();
    }

    //section on off and points
    if (sectionTriggerDistance > sectionTriggerStepDistance && isJobStarted)
    {
        AddSectionOrPathPoints();

        //grab fix and elevation
        if (isLogElevation) sbFix.append(QString("%1, %2, %3, %4, %5").arg(pn.fix.easting,0,'g',3)
                             .arg(pn.fix.northing,0,'g',3)
                             .arg(pn.fix.northing,0,'g',3)
                             .arg(pn.latitude,0,'g',7)
                             .arg(pn.longitude,0,'g',7).toUtf8());
    }

    //test if travelled far enough for new boundary point
    if (bnd.isOkToAddPoints)
    {
        double boundaryDistance = glm::Distance(pn.fix, prevBoundaryPos);
        if (boundaryDistance > 1) AddBoundaryPoint();
    }

    //calc distance travelled since last GPS fix
    //distance = glm::distance(pn.fix, prevFix);
    //if (vehicle.avgSpeed > 1)

    if ((vehicle.avgSpeed - previousSpeed  ) < -vehicle.panicStopSpeed && vehicle.panicStopSpeed != 0)
    {
        if (isAutoSteerBtnOn) onStopAutoSteer();
    }

    previousSpeed = vehicle.avgSpeed;
}



void FormGPS::CalculatePositionHeading()
{
    // #region pivot hitch trail

    //translate from pivot position to steer axle and pivot axle position
    //translate world to the pivot axle
    vehicle.pivotAxlePos.easting = pn.fix.easting - (sin(vehicle.fixHeading) * vehicle.antennaPivot);
    vehicle.pivotAxlePos.northing = pn.fix.northing - (cos(vehicle.fixHeading) * vehicle.antennaPivot);
    vehicle.pivotAxlePos.heading = vehicle.fixHeading;

    vehicle.steerAxlePos.easting = vehicle.pivotAxlePos.easting + (sin(vehicle.fixHeading) * vehicle.wheelbase);
    vehicle.steerAxlePos.northing = vehicle.pivotAxlePos.northing + (cos(vehicle.fixHeading) * vehicle.wheelbase);
    vehicle.steerAxlePos.heading = vehicle.fixHeading;

    if (!ABLine.isLateralTriggered && !curve.isLateralTriggered)
    {
        double speed1 = tool.width * 0.5;
        double speed2 = vehicle.avgSpeed * 0.277777 * guidanceLookAheadTime;
        double guidanceLookDist = (speed1 < speed2 ? speed2 : speed1);
        vehicle.guidanceLookPos.easting = vehicle.pivotAxlePos.easting + (sin(vehicle.fixHeading) * guidanceLookDist);
        vehicle.guidanceLookPos.northing = vehicle.pivotAxlePos.northing + (cos(vehicle.fixHeading) * guidanceLookDist);
    }

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
                double t = (tool.tankTrailingHitchLength) / distanceCurrentStepFix;
                vehicle.tankPos.easting = vehicle.hitchPos.easting + t * (vehicle.hitchPos.easting - vehicle.tankPos.easting);
                vehicle.tankPos.northing = vehicle.hitchPos.northing + t * (vehicle.hitchPos.northing - vehicle.tankPos.northing);
                vehicle.tankPos.heading = atan2(vehicle.hitchPos.easting - vehicle.tankPos.easting, vehicle.hitchPos.northing - vehicle.tankPos.northing);
                if (vehicle.tankPos.heading < 0) vehicle.tankPos.heading += glm::twoPI;

            }

            ////the tool is seriously jacknifed or just starting out so just spring it back.
            over = fabs(M_PI - fabs(fabs(vehicle.tankPos.heading - vehicle.fixHeading) - M_PI));

            if ((over < 2.0) && (startCounter > 50))
            {
                vehicle.tankPos.easting = vehicle.hitchPos.easting + (sin(vehicle.tankPos.heading) * (tool.tankTrailingHitchLength));
                vehicle.tankPos.northing = vehicle.hitchPos.northing + (cos(vehicle.tankPos.heading) * (tool.tankTrailingHitchLength));
            }

            //criteria for a forced reset to put tool directly behind vehicle
            if (over > 2.0 || startCounter < 51 )
            {
                vehicle.tankPos.heading = vehicle.fixHeading;
                vehicle.tankPos.easting = vehicle.hitchPos.easting + (sin(vehicle.tankPos.heading) * (tool.tankTrailingHitchLength));
                vehicle.tankPos.northing = vehicle.hitchPos.northing + (cos(vehicle.tankPos.heading) * (tool.tankTrailingHitchLength));
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
            double t = (tool.trailingHitchLength) / distanceCurrentStepFix;
            vehicle.toolPivotPos.easting = vehicle.tankPos.easting + t * (vehicle.tankPos.easting - vehicle.toolPivotPos.easting);
            vehicle.toolPivotPos.northing = vehicle.tankPos.northing + t * (vehicle.tankPos.northing - vehicle.toolPivotPos.northing);
            vehicle.toolPivotPos.heading = atan2(vehicle.tankPos.easting - vehicle.toolPivotPos.easting, vehicle.tankPos.northing - vehicle.toolPivotPos.northing);
            if (vehicle.toolPivotPos.heading < 0) vehicle.toolPivotPos.heading += glm::twoPI;
        }

        ////the tool is seriously jacknifed or just starting out so just spring it back.
        over = fabs(M_PI - fabs(fabs(vehicle.toolPivotPos.heading - vehicle.tankPos.heading) - M_PI));

        if ((over < 1.9) && (startCounter > 50))
        {
            vehicle.toolPivotPos.easting = vehicle.tankPos.easting + (sin(vehicle.toolPivotPos.heading) * (tool.trailingHitchLength));
            vehicle.toolPivotPos.northing = vehicle.tankPos.northing + (cos(vehicle.toolPivotPos.heading) * (tool.trailingHitchLength));
        }

        //criteria for a forced reset to put tool directly behind vehicle
        if (over > 1.9 || startCounter < 51 )
        {
            vehicle.toolPivotPos.heading = vehicle.tankPos.heading;
            vehicle.toolPivotPos.easting = vehicle.tankPos.easting + (sin(vehicle.toolPivotPos.heading) * (tool.trailingHitchLength));
            vehicle.toolPivotPos.northing = vehicle.tankPos.northing + (cos(vehicle.toolPivotPos.heading) * (tool.trailingHitchLength));
        }

        vehicle.toolPos.heading = vehicle.toolPivotPos.heading;
        vehicle.toolPos.easting = vehicle.tankPos.easting +
                          (sin(vehicle.toolPivotPos.heading) * (tool.trailingHitchLength - tool.trailingToolToPivotLength));
        vehicle.toolPos.northing = vehicle.tankPos.northing +
                           (cos(vehicle.toolPivotPos.heading) * (tool.trailingHitchLength - tool.trailingToolToPivotLength));

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
    double distance = tool.width * 0.5;
    if (distance > 5) distance = 5;

    //whichever is less
    if (tool.farLeftSpeed < tool.farRightSpeed)
    {
        double twist = tool.farLeftSpeed / tool.farRightSpeed;
        twist *= twist;
        if (twist < 0.2) twist = 0.2;
        vehicle.sectionTriggerStepDistance = distance * twist * twist;
    }
    else
    {
        double twist = tool.farRightSpeed / tool.farLeftSpeed;
        //twist *= twist;
        if (twist < 0.2) twist = 0.2;

        vehicle.sectionTriggerStepDistance = distance * twist * twist;
    }

    //finally fixed distance for making a curve line
    if (!curve.isOkToAddDesPoints) vehicle.sectionTriggerStepDistance = vehicle.sectionTriggerStepDistance + 0.5;
    //if (ct.isContourBtnOn) vehicle.sectionTriggerStepDistance *=0.5;

    //precalc the sin and cos of heading * -1
    vehicle.sinSectionHeading = sin(-vehicle.toolPos.heading);
    vehicle.cosSectionHeading = cos(-vehicle.toolPos.heading);
}

void FormGPS::AddBoundaryPoint()
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

void FormGPS::AddContourPoints()
{
    //if (isConstantContourOn)
    {
        //record contour all the time
        //Contour Base Track.... At least One section on, turn on if not
        if (patchCounter != 0)
        {
            //keep the line going, everything is on for recording path
            if (ct.isContourOn) ct.AddPoint(vehicle.pivotAxlePos);
            else
            {
                ct.StartContourLine();
                ct.AddPoint(vehicle.pivotAxlePos);
            }
        }

        //All sections OFF so if on, turn off
        else
        {
            if (ct.isContourOn)
            { ct.StopContourLine(contourSaveList); }
        }

        //Build contour line if close enough to a patch
        if (ct.isContourBtnOn) ct.BuildContourGuidanceLine(secondsSinceStart, vehicle, vehicle.pivotAxlePos);
    }
    //save the north & east as previous
    prevContourPos.northing = vehicle.pivotAxlePos.northing;
    prevContourPos.easting = vehicle.pivotAxlePos.easting;
}

//add the points for section, contour line points, Area Calc feature
void FormGPS::AddSectionOrPathPoints()
{
    if (recPath.isRecordOn)
    {
        //keep minimum speed of 1.0
        double speed = vehicle.avgSpeed;
        if (vehicle.avgSpeed < 1.0) speed = 1.0;
        bool autoBtn = (autoBtnState == btnStates::Auto);

        recPath.recList.append(CRecPathPt(vehicle.pivotAxlePos.easting, vehicle.pivotAxlePos.northing, vehicle.pivotAxlePos.heading, speed, autoBtn));
    }

    if (curve.isOkToAddDesPoints)
    {
        curve.refList.append(Vec3(vehicle.pivotAxlePos.easting, vehicle.pivotAxlePos.northing, vehicle.pivotAxlePos.heading));
    }

    //save the north & east as previous
    prevSectionPos.northing = pn.fix.northing;
    prevSectionPos.easting = pn.fix.easting;

    // if non zero, at least one section is on.
    int patchCounter = 0;

    //send the current and previous GPS fore/aft corrected fix to each section
    for (int j = 0; j < triStrip.count(); j++)
    {
        if (triStrip[j].isDrawing)
        {
            triStrip[j].AddMappingPoint(tool, fd, j);
            patchCounter++;
        }
    }
}

//calculate the extreme tool left, right velocities, each section lookahead, and whether or not its going backwards
void FormGPS::CalculateSectionLookAhead(double northing, double easting, double cosHeading, double sinHeading)
{
    //calculate left side of section 1
    Vec2 left;
    Vec2 right = left;
    double leftSpeed = 0, rightSpeed = 0;

    //speed max for section kmh*0.277 to m/s * 10 cm per pixel * 1.7 max speed
    double meterPerSecPerPixel = fabs(vehicle.avgSpeed) * 4.5;
    //qDebug() << pn.speed << ", m/s per pixel is " << meterPerSecPerPixel;

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

            leftSpeed = left.getLength() * gpsHz * 10;
            //qDebug() << leftSpeed << " - left speed";
            if (leftSpeed > meterPerSecPerPixel) leftSpeed = meterPerSecPerPixel;

        }
        else
        {
            //right point from last section becomes this left one
            tool.section[j].leftPoint = tool.section[j - 1].rightPoint;
            left = tool.section[j].leftPoint - tool.section[j].lastLeftPoint;

            //save a copy for next time
            tool.section[j].lastLeftPoint = tool.section[j].leftPoint;

            //save the slower of the 2
            if (leftSpeed > rightSpeed) leftSpeed = rightSpeed;
        }

        tool.section[j].rightPoint = Vec2(cosHeading * (tool.section[j].positionRight) + easting,
                            sinHeading * (tool.section[j].positionRight) + northing);
        /*
        qDebug() << j << ": " << tool.section[j].leftPoint.easting << "," <<
                                 tool.section[j].leftPoint.northing <<" " <<
                                 tool.section[j].rightPoint.easting << ", " <<
                                 tool.section[j].rightPoint.northing;
                                 */


        //now we have left and right for this section
        right = tool.section[j].rightPoint - tool.section[j].lastRightPoint;

        //save a copy for next time
        tool.section[j].lastRightPoint = tool.section[j].rightPoint;

        //grab vector length and convert to meters/sec/10 pixels per meter
        rightSpeed = right.getLength() * gpsHz * 10;
        if (rightSpeed > meterPerSecPerPixel) rightSpeed = meterPerSecPerPixel;

        //Is section outer going forward or backward
        double head = left.headingXZ();

        if (head < 0) head += glm::twoPI;

        if (M_PI - fabs(fabs(head - vehicle.toolPos.heading) - M_PI) > glm::PIBy2)
        {
            if (leftSpeed > 0) leftSpeed *= -1;
        }

        head = right.headingXZ();
        if (head < 0) head += glm::twoPI;
        if (M_PI - fabs(fabs(head - vehicle.toolPos.heading) - M_PI) > glm::PIBy2)
        {
            if (rightSpeed > 0) rightSpeed *= -1;
        }

        double sped = 0;
        //save the far left and right speed in m/sec averaged over 20%
        if (j==0)
        {
            sped = (leftSpeed * 0.1);
            if (sped < 0.1) sped = 0.1;
            tool.farLeftSpeed = tool.farLeftSpeed * 0.7 + sped * 0.3;
        }
        if (j == tool.numOfSections - 1)
        {
            sped = (rightSpeed * 0.1);
            if(sped < 0.1) sped = 0.1;
            tool.farRightSpeed = tool.farRightSpeed * 0.7 + sped * 0.3;
        }

        //choose fastest speed
        if (leftSpeed > rightSpeed)
        {
            sped = leftSpeed;
            leftSpeed = rightSpeed;
        }
        else sped = rightSpeed;
        tool.section[j].speedPixels = tool.section[j].speedPixels * 0.7 + sped * 0.3;
    }
}

//the start of first few frames to initialize entire program
void FormGPS::InitializeFirstFewGPSPositions()
{
    if (!isFirstFixPositionSet)
    {
        if (!isJobStarted)
        {
            pn.latStart = pn.latitude;
            pn.lonStart = pn.longitude;
            pn.SetLocalMetersPerDegree();
        }

        pn.ConvertWGS84ToLocal(pn.latitude, pn.longitude, pn.fix.northing, pn.fix.easting);

        //Draw a grid once we know where in the world we are.
        isFirstFixPositionSet = true;

        //most recent fixes
        prevFix.easting = pn.fix.easting;
        prevFix.northing = pn.fix.northing;

        //run once and return
        isFirstFixPositionSet = true;

        return;
    }

    else
    {

        //most recent fixes
        prevFix.easting = pn.fix.easting; prevFix.northing = pn.fix.northing;

        //keep here till valid data
        if (startCounter > (20))
        {
            isGPSPositionInitialized = true;
            lastReverseFix = pn.fix;
        }

        //in radians
        vehicle.fixHeading = 0;
        vehicle.toolPos.heading = vehicle.fixHeading;

        //send out initial zero settings
        if (isGPSPositionInitialized)
        {
            //TODO determine if it is day from wall clock and date
            isDayTime = true;

            SetZoom();
        }
        return;
    }
}
