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
#include "ui_formgps.h"

/*
static const double sm_a = 6378137.0;
static const double sm_b = 6356752.314;
static const double UTMScaleFactor2 = 1.0004001600640256102440976390556;

//helper functions
QPair<double, double> UTMToLatLon(double X, double Y);
double FootpointLatitude(double y);
*/

//called by watchdog timer every 50 ms
void FormGPS::scanForNMEA()
{
    //parse any data from pn->rawBuffer
    //qDebug() << stopwatch.restart();
    pn->parseNMEA();

    //time for a frame update with new valid nmea data
    if (pn->updatedGGA || pn->updatedRMC)
    {
        recvCounter = 0;
        vehicle->avgSpeed[vehicle->ringCounter] = pn->speed;
        if (vehicle->ringCounter++ > 8) vehicle->ringCounter = 0;
        /*
        qDebug() << qSetRealNumberPrecision(15) <<
                    pn->headingTrue << ", " <<
                    //pn->longitude <<", " <<pn->latitude << " => " <<
                    pn->easting << ", " << pn->northing;
        */

        //if saving a file ignore any movement
        if (isSavingFile) return;

        //accumulate time over multiple frames
        double wallclock = swFrame.restart();
        //qDebug() << wallclock << "ms";
        hzTime += wallclock; //get elapsed time and reset in one operation

        //now calculate NMEA rate
        if (timerPn++ == 36)
        {
            et = (1 / (hzTime * 0.000025));
            if (et > 4 && et < 8) fixUpdateHz = 5;
            if (et > 9 && et < 13) fixUpdateHz = 10;
            if (et > 1.2 && et < 3) fixUpdateHz = 2;
            if (et > 0.8 && et < 1.2) fixUpdateHz = 1;
            fixUpdateTime = 1 / (double)fixUpdateHz;
            timerPn = 0;
            hzTime = 0;
            //fixUpdateHz = 5;
            //fixUpdateTime = 0.2;
        }

        //start the watch and time till it gets back here
        //swFrame.Start();

        //reset both flags
        pn->updatedGGA = false;
        pn->updatedRMC = false;

        //update all data for new frame
        updateFixPosition();
    }

    //must make sure arduinos are kept off
    else
    {
        if (!isGPSPositionInitialized)
        {
            //relay port
            mc.relaySectionControl[0] = (uchar)0;
            //TODO: not implemented yet
            sectionControlOutToPort();

            //autosteer port
            mc.autoSteerData[mc.sdHeaderHi] = (uchar)127; //32766
            mc.autoSteerData[mc.sdHeaderLo] = (uchar)(254);
            mc.autoSteerData[mc.sdRelay] = (uchar)0;
            mc.autoSteerData[mc.sdSpeed] = (uchar)(0);
            mc.autoSteerData[mc.sdDistanceHi] = (uchar)(125); //32020
            mc.autoSteerData[mc.sdDistanceLo] = (uchar)20;
            mc.autoSteerData[mc.sdSteerAngleHi] = (uchar)(125); //32020
            mc.autoSteerData[mc.sdSteerAngleLo] = (uchar)20;


            autoSteerDataOutToPort();
        }
    }

    //Update the port connecition counter - is reset every time new sentence is valid and ready
    if (recvCounter < 60) recvCounter++;
}

//call for position update after valid NMEA sentence
void FormGPS::updateFixPosition()
{
    startCounter++;
    totalFixSteps = fixUpdateHz * 4;
    if (!isGPSPositionInitialized) {  initializeFirstFewGPSPositions();   return;  }

    //TILT stuff

    //REGION STEP FIX
    //grab the most current fix and save the distance from the last fix
    distanceCurrentStepFix = pn->distance(pn->northing, pn->easting, stepFixPts[0].northing, stepFixPts[0].easting);
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
        stepFixPts[0].heading = pn->distance(pn->northing, pn->easting, stepFixPts[0].northing, stepFixPts[0].easting);
        stepFixPts[0].easting = pn->easting;
        stepFixPts[0].northing = pn->northing;

        //reload the last position that was triggered.
        stepFixPts[(totalFixSteps - 1)].heading = pn->distance(vHold.northing, vHold.easting, stepFixPts[(totalFixSteps - 1)].northing, stepFixPts[(totalFixSteps - 1)].easting);
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
            if (ct->isContourOn)
            {
                //TODO implement with serial code
                //TODO: pn->logNMEASentence.append(recvSentenceSettings);
            }
        }

        //To prevent drawing high numbers of triangles, determine and test before drawing vertex
        vehicle->sectionTriggerDistance = pn->distance(pn->northing, pn->easting, vehicle->prevSectionPos.northing, vehicle->prevSectionPos.easting);
        /*
        qDebug() << sectionTriggerDistance << "\t" << sectionTriggerStepDistance << "\t"
                 << pn->northing <<",\t"<< pn->easting << "\told: "
                 << prevSectionPos.northing << ", " << prevSectionPos.easting;
        */
        //section on off and points, contour points
        if (vehicle->sectionTriggerDistance > vehicle->sectionTriggerStepDistance)
                addSectionContourPathPoints();

        //test if travelled far enough for new boundary point
        double boundaryDistance = pn->distance(pn->northing, pn->easting, prevBoundaryPos.northing, prevBoundaryPos.easting);
        if (boundaryDistance > boundaryTriggerDistance) addBoundaryPoint();

        //calc distance travelled since last GPS fix
        distance = pn->distance(pn->northing, pn->easting, prevFix.northing, prevFix.easting);
        userDistance += distance;//userDistance can be reset

       //most recent fixes are now the prev ones
        prevFix.easting = pn->easting; prevFix.northing = pn->northing;

        //load up history with valid data
        for (int i = totalFixSteps - 1; i > 0; i--) stepFixPts[i] = stepFixPts[i - 1];
        stepFixPts[0].heading = pn->distance(pn->northing, pn->easting, stepFixPts[0].northing, stepFixPts[0].easting);
        stepFixPts[0].easting = pn->easting;
        stepFixPts[0].northing = pn->northing;
    }

    //region AutoSteer
    //preset the values
    vehicle->guidanceLineDistanceOff = 32000;

    //do the distance from line calculations for contour and AB
    if (ct->isContourBtnOn) ct->distanceFromContourLine();
    if (ABLine->isABLineSet && !ct->isContourBtnOn) ABLine->getCurrentABLine();

    // autosteer at full speed of updates
    if (!isAutoSteerBtnOn) //32020 means auto steer is off
    {
        vehicle->guidanceLineDistanceOff = 32020;
    }

    if (!isInFreeDriveMode)
    {

        //fill up0 the auto steer array with new values
        mc.autoSteerData[mc.sdSpeed] = (uchar)(pn->speed * 4.0);

        mc.autoSteerData[mc.sdDistanceHi] = (uchar)(vehicle->guidanceLineDistanceOff >> 8);
        mc.autoSteerData[mc.sdDistanceLo] = (uchar)vehicle->guidanceLineDistanceOff;

        mc.autoSteerData[mc.sdSteerAngleHi] = (uchar)(vehicle->guidanceLineSteerAngle >> 8);
        mc.autoSteerData[mc.sdSteerAngleLo] = (uchar)vehicle->guidanceLineSteerAngle;

        //out serial to autosteer module  //indivdual classes load the distance and heading deltas
        autoSteerDataOutToPort();
    }

    else
    {
        //fill up the auto steer array with free drive values
        mc.autoSteerData[mc.sdSpeed] = (uchar)(pn->speed * 4.0 + 8);

        //make steer module think everything is normal
        mc.autoSteerData[mc.sdDistanceHi] = (uchar)(0);
        mc.autoSteerData[mc.sdDistanceLo] = (uchar)0;

        //mc.autoSteerData[mc.sdSteerAngleHi] = (uchar)(guidanceLineSteerAngle >> 8);
        //mc.autoSteerData[mc.sdSteerAngleLo] = (uchar)guidanceLineSteerAngle;

        //out serial to autosteer module  //indivdual classes load the distance and heading deltas

        autoSteerDataOutToPort();

    }

    //calculate lookahead at full speed, no sentence misses
    calculateSectionLookAhead(vehicle->toolPos.northing, vehicle->toolPos.easting, vehicle->cosSectionHeading, vehicle->sinSectionHeading);

    //openGLControl_Draw routine triggered manually
    qmlview->update();

    //LightBar if AB Line is set and turned on or contour
    if (isLightbarOn)
    {
        if (ct->isContourBtnOn)
        {
            QString dist;
            //turn on distance widget
            //txtDistanceOffABLine.Visible = true;
            //lblDelta.Visible = true;
            if (ct->distanceFromCurrentLine == 32000) ct->distanceFromCurrentLine = 0;

            if ((ct->distanceFromCurrentLine) < 0.0) {
                txtDistanceOffABLine->setProperty("color","green");

                if (isMetric) dist = QString((int)fabs(ct->distanceFromCurrentLine * 0.1)) + " " + QChar(0x2192);
                else dist = QString((int)fabs(ct->distanceFromCurrentLine / 2.54 * 0.1)) + " " + QChar(0x2192);
                txtDistanceOffABLine->setProperty("text", dist);
            } else {
                txtDistanceOffABLine->setProperty("color", "red");
                if (isMetric) dist = QString("") + QChar(0x2190) + " " + ((int)fabs(ct->distanceFromCurrentLine * 0.1));
                else dist = QString("") + QChar(0x2190)+ " " + ((int)fabs(ct->distanceFromCurrentLine / 2.54 * 0.1));
                txtDistanceOffABLine->setProperty("text", dist);
            }

            //if (guidanceLineHeadingDelta < 0) lblDelta.ForeColor = Color.Red;
            //else lblDelta.ForeColor = Color.Green;

            if (vehicle->guidanceLineDistanceOff == 32020 || vehicle->guidanceLineDistanceOff == 32000)
                btnAutoSteer->setProperty("buttonText", QChar(0x2715));
            else
                btnAutoSteer->setProperty("buttonText", QChar(0x2713));
        } else if (ABLine->isABLineSet || ABLine->isABLineBeingSet) {
            QString dist;

            txtDistanceOffABLine->setProperty("visible", "true");
            //lblDelta.Visible = true;
            if ((ABLine->distanceFromCurrentLine) < 0.0) {
                // --->
                txtDistanceOffABLine->setProperty("color","green");
                if (isMetric) dist = QString((int)fabs(ABLine->distanceFromCurrentLine * 0.1)) + " \u21D2";
                else dist = QString((int)fabs(ABLine->distanceFromCurrentLine / 2.54 * 0.1)) + " \u21D2";
                txtDistanceOffABLine->setProperty("text", dist);
            } else {
                // <----
                txtDistanceOffABLine->setProperty("color", "red");
                if (isMetric) dist = QChar(0x21D0) + QString((int)fabs(ABLine->distanceFromCurrentLine * 0.1));
                else dist = QChar(0x21D0) + QString((int)fabs(ABLine->distanceFromCurrentLine / 2.54 * 0.1));
                txtDistanceOffABLine->setProperty("text", dist);
            }

            //if (guidanceLineHeadingDelta < 0) lblDelta.ForeColor = Color.Red;
            //else lblDelta.ForeColor = Color.Green;
            if (vehicle->guidanceLineDistanceOff == 32020 || vehicle->guidanceLineDistanceOff == 32000)
                btnAutoSteer->setProperty("buttonText", QChar(0x2715));
            else
                btnAutoSteer->setProperty("buttonText", QChar(0x2713));
        }

        //AB line is not set so turn off numbers
        if (!ABLine->isABLineSet && !ABLine->isABLineBeingSet && !ct->isContourBtnOn)
        {
            txtDistanceOffABLine->setProperty("visible", "false");
            //lblDelta.Visible = false;
            btnAutoSteer->setProperty("buttonText","-");
        }
    } else {
        txtDistanceOffABLine->setProperty("visible", "false");
        //lblDelta.Visible = false;
        btnAutoSteer->setProperty("buttonText","-");
    }


    /*
    //go to our offscreen context and do the section lookahead
    //drawing.

    if (!backOpenGLContext.isValid()) {
        //we're on a single-threaded implementation, so we'll
        //have to use the QML context for our rendering.
        qmlview->resetOpenGLState();
        QOpenGLContext *glContext = QOpenGLContext::currentContext();
        if (!backFBO ) {
            QOpenGLFramebufferObjectFormat format;
            format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
            //TODO: backFBO is leaking... delete it in the destructor?
            //I think context has to be active to delete it...
            backFBO = new QOpenGLFramebufferObject(QSize(400,400),format);
        }
        glContext->makeCurrent(&backSurface);
        backFBO->bind();
        glContext->functions()->glViewport(0,0,400,400);
        //openGLControlBack_Draw();
        glContext->functions()->glFlush();
        backFBO->bindDefault();
        glContext->doneCurrent();
        qmlview->resetOpenGLState();
    } else {
        backOpenGLContext.makeCurrent(&backSurface);
        if (!backFBO ) {
            QOpenGLFramebufferObjectFormat format;
            format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
            //TODO: backFBO is leaking... delete it in the destructor?
            //I think context has to be active to delete it...
            backFBO = new QOpenGLFramebufferObject(QSize(400,400),format);
        }

        backFBO->bind();
        backOpenGLContext.functions()->glViewport(0,0,400,400);
        openGLControlBack_Draw();
        backOpenGLContext.functions()->glFlush();
        backFBO->bindDefault();
        backOpenGLContext.doneCurrent();
    }
    */

    //since we're in the main thread we can directly call processSectionLookahead()
    processSectionLookahead();


//end of UppdateFixPosition
}

void FormGPS::calculatePositionHeading()
{
    //in radians
    vehicle->fixHeading = atan2(pn->easting - stepFixPts[currentStepFix].easting, pn->northing - stepFixPts[currentStepFix].northing);
    if (vehicle->fixHeading < 0) vehicle->fixHeading += twoPI;

    //qDebug() << qSetRealNumberPrecision(15) << currentStepFix << pn->easting << stepFixPts[currentStepFix].easting << pn->northing << stepFixPts[currentStepFix].northing << fixHeading;


    //determine fix positions and heading
    vehicle->fixEasting = (pn->easting);
    vehicle->fixNorthing = (pn->northing);
    vehicle->speed = pn->speed;
    vehicle->altitude = pn->altitude;

   //in degrees for glRotate opengl methods.
    int camStep = currentStepFix*2;
    if (camStep > (totalFixSteps - 1)) camStep = (totalFixSteps - 1);

    //mf.guidanceLineHeadingDelta = (Int16)((atan2(sin(temp - mf.fixHeading),
    //                                    cos(temp - mf.fixHeading))) * 10000);


    fixHeadingCam = atan2(pn->easting - stepFixPts[camStep].easting, pn->northing - stepFixPts[camStep].northing);
    if (fixHeadingCam < 0) fixHeadingCam += twoPI;
    fixHeadingCam = toDegrees(fixHeadingCam);

    //testDouble = fixHeadingCam;
    //testInt = camStep;

    //// kalman process

    //const double varHead = 0.06; // variance
    //const double varProcess = .002;

    //Pc = P + varProcess;
    //G = Pc / (Pc + varHead);    // kalman gain
    //P = (1 - G) * Pc;
    //Xp = Xe;
    //Zp = Xp;
    //Xe = G * (fixHeadingCam - Zp) + Xp;

    ////if (Xe < 0) Xe += twoPI;

    ////to degrees for openGL camera
    //fixHeadingCam = (Xe);
    //if (fixHeadingCam > twoPI) fixHeadingCam -= twoPI;

    //#region pivot hitch trail

    //translate world to the pivot axle
    vehicle->pivotAxlePos.easting = pn->easting - (sin(vehicle->fixHeading) * vehicle->antennaPivot);
    vehicle->pivotAxlePos.northing = pn->northing - (cos(vehicle->fixHeading) * vehicle->antennaPivot);

    //determine where the rigid vehicle hitch ends
    vehicle->hitchPos.easting = pn->easting + (sin(vehicle->fixHeading) * (vehicle->hitchLength - vehicle->antennaPivot));
    vehicle->hitchPos.northing = pn->northing + (cos(vehicle->fixHeading) * (vehicle->hitchLength - vehicle->antennaPivot));

    //tool attached via a trailing hitch
    if (vehicle->isToolTrailing)
    {
        double over;
        if (vehicle->tankTrailingHitchLength < -2.0)
        {
            //Torriem rules!!!!! Oh yes, this is all his. Thank-you
            if (distanceCurrentStepFix != 0)
            {
                double t = (vehicle->tankTrailingHitchLength) / distanceCurrentStepFix;
                vehicle->tankPos.easting = vehicle->hitchPos.easting + t * (vehicle->hitchPos.easting - vehicle->tankPos.easting);
                vehicle->tankPos.northing = vehicle->hitchPos.northing + t * (vehicle->hitchPos.northing - vehicle->tankPos.northing);
                vehicle->fixHeadingTank = atan2(vehicle->hitchPos.easting - vehicle->tankPos.easting, vehicle->hitchPos.northing - vehicle->tankPos.northing);
            }


            ////the tool is seriously jacknifed or just starting out so just spring it back.
            over = fabs(M_PI - fabs(fabs(vehicle->fixHeadingTank - vehicle->fixHeading) - M_PI));

            if (over < 2.0 && startCounter > 50)
            {
                vehicle->tankPos.easting = vehicle->hitchPos.easting + (sin(vehicle->fixHeadingTank) * (vehicle->tankTrailingHitchLength));
                vehicle->tankPos.northing = vehicle->hitchPos.northing + (cos(vehicle->fixHeadingTank) * (vehicle->tankTrailingHitchLength));
            }

            //criteria for a forced reset to put tool directly behind vehicle
            if (over > 2.0 || startCounter < 51 )
            {
                vehicle->fixHeadingTank = vehicle->fixHeading;
                vehicle->tankPos.easting = vehicle->hitchPos.easting + (sin(vehicle->fixHeadingTank) * (vehicle->tankTrailingHitchLength));
                vehicle->tankPos.northing = vehicle->hitchPos.northing + (cos(vehicle->fixHeadingTank) * (vehicle->tankTrailingHitchLength));
            }

        }

        else
        {
            vehicle->fixHeadingTank = vehicle->fixHeading;
            vehicle->tankPos.easting = vehicle->hitchPos.easting;
            vehicle->tankPos.northing = vehicle->hitchPos.northing;
        }

        //Torriem rules!!!!! Oh yes, this is all his. Thank-you
        if (distanceCurrentStepFix != 0)
        {
            double t = (vehicle->toolTrailingHitchLength) / distanceCurrentStepFix;
            vehicle->toolPos.easting = vehicle->tankPos.easting + t * (vehicle->tankPos.easting - vehicle->toolPos.easting);
            vehicle->toolPos.northing = vehicle->tankPos.northing + t * (vehicle->tankPos.northing - vehicle->toolPos.northing);
            vehicle->fixHeadingSection = atan2(vehicle->tankPos.easting - vehicle->toolPos.easting, vehicle->tankPos.northing - vehicle->toolPos.northing);
        }


        ////the tool is seriously jacknifed or just starting out so just spring it back.
        over = fabs(M_PI - fabs(fabs(vehicle->fixHeadingSection - vehicle->fixHeadingTank) - M_PI));

        if (over < 1.9 && startCounter > 50)
        {
            vehicle->toolPos.easting = vehicle->tankPos.easting + (sin(vehicle->fixHeadingSection) * (vehicle->toolTrailingHitchLength));
            vehicle->toolPos.northing = vehicle->tankPos.northing + (cos(vehicle->fixHeadingSection) * (vehicle->toolTrailingHitchLength));
        }

        //criteria for a forced reset to put tool directly behind vehicle
        if (over > 1.9 || startCounter < 51 )
        {
            vehicle->fixHeadingSection = vehicle->fixHeadingTank;
            vehicle->toolPos.easting = vehicle->tankPos.easting + (sin(vehicle->fixHeadingSection) * (vehicle->toolTrailingHitchLength));
            vehicle->toolPos.northing = vehicle->tankPos.northing + (cos(vehicle->fixHeadingSection) * (vehicle->toolTrailingHitchLength));
        }
    }

    //rigidly connected to vehicle
    else
    {
        vehicle->fixHeadingSection = vehicle->fixHeading;
        vehicle->toolPos.easting = vehicle->hitchPos.easting;
        vehicle->toolPos.northing = vehicle->hitchPos.northing;
    }

    //#endregion
        //used to increase triangle count when going around corners, less on straight
        //pick the slow moving side edge of tool
        double metersPerSec = pn->speed * 0.277777777;

    //whichever is less
    if (vehicle->toolFarLeftSpeed < vehicle->toolFarRightSpeed)
        vehicle->sectionTriggerStepDistance = vehicle->toolFarLeftSpeed / metersPerSec;
    else vehicle->sectionTriggerStepDistance = vehicle->toolFarRightSpeed / metersPerSec;

    //finally determine distance
    vehicle->sectionTriggerStepDistance = vehicle->sectionTriggerStepDistance * vehicle->sectionTriggerStepDistance *
        metersPerSec * triangleResolution*2.0 + 1.0;
    //qDebug() << metersPerSec << triangleResolution << sectionTriggerStepDistance;

    //  ** Torriem Cam   *****
    //Default using fix Atan2 to calc cam, if unchecked in display settings use True Heading from NMEA
    if (!isAtanCam)
    {
        //use NMEA headings for camera and tractor graphic
        //qDebug() << "fixHeadingCam: " << fixHeadingCam << ", headingTrue: " << pn->headingTrue;
        //qDebug() << "fixHeading: " << fixHeading << ", radians headingTrue: " << toRadians(pn->headingTrue);
        //fixHeading = toRadians(pn->headingTrue);
        fixHeadingCam = pn->headingTrue;
    }
    //qDebug() << "fixHeading degrees: " << toDegrees(fixHeading) << ", northing: " << pn->northing << ", easting: " <<pn->easting;

    //check to make sure the grid is big enough
    worldGrid->checkZoomWorldGrid(pn->northing, pn->easting);

    //precalc the sin and cos of heading * -1
    vehicle->sinSectionHeading = sin(-vehicle->fixHeadingSection);
    vehicle->cosSectionHeading = cos(-vehicle->fixHeadingSection);
}

void FormGPS::addBoundaryPoint()
{
    //save the north & east as previous
    prevBoundaryPos.easting = pn->easting;
    prevBoundaryPos.northing = pn->northing;
    Vec2 point;

    //build the boundary line
    if (boundary->isOkToAddPoints)
    {
        if (boundary->isDrawRightSide)
        {
            //Right side
            point = Vec2(vehicle->cosSectionHeading * (vehicle->section[vehicle->numOfSections - 1].positionRight) + vehicle->toolPos.easting,
                vehicle->sinSectionHeading * (vehicle->section[vehicle->numOfSections - 1].positionRight) + vehicle->toolPos.northing);
            boundary->ptList.append(point);
        }

            //draw on left side
        else
        {
            //Right side
            point = Vec2(vehicle->cosSectionHeading * (vehicle->section[0].positionLeft) + vehicle->toolPos.easting,
                vehicle->sinSectionHeading * (vehicle->section[0].positionLeft) + vehicle->toolPos.northing);
            boundary->ptList.append(point);
        }

    }

    //build the polygon to calculate area
    if (periArea.isBtnPerimeterOn)
    {
        if (isAreaOnRight)
        {
            //Right side
            point = Vec2(vehicle->cosSectionHeading * (vehicle->section[vehicle->numOfSections - 1].positionRight) + vehicle->toolPos.easting,
                vehicle->sinSectionHeading * (vehicle->section[vehicle->numOfSections - 1].positionRight) + vehicle->toolPos.northing);
            periArea.periPtList.append(point);
        }

            //draw on left side
        else
        {
            //Right side
            point = Vec2(vehicle->cosSectionHeading * (vehicle->section[0].positionLeft) + vehicle->toolPos.easting,
                vehicle->sinSectionHeading * (vehicle->section[0].positionLeft) + vehicle->toolPos.northing);
            periArea.periPtList.append(point);
        }

    }
}

//add the points for section, contour line points, Area Calc feature
void FormGPS::addSectionContourPathPoints()
{
    //save the north & east as previous
    vehicle->prevSectionPos.northing = pn->northing;
    vehicle->prevSectionPos.easting = pn->easting;

    if (isJobStarted)//add the pathpoint
    {
        // if non zero, at least one section is on.
        int sectionCounter = 0;

        //send the current and previous GPS fore/aft corrected fix to each section
        for (int j = 0; j < vehicle->numOfSections+1; j++)
        {
            if (vehicle->section[j].isSectionOn)
            {
                vehicle->section[j].addPathPoint(vehicle->toolPos.northing, vehicle->toolPos.easting, vehicle->cosSectionHeading, vehicle->sinSectionHeading);
                sectionCounter++;
            }
        }

        //Contour Base Track.... At least One section on, turn on if not
        if (sectionCounter != 0)
        {
            //keep the line going, everything is on for recording path
            if (ct->isContourOn)
                ct->addPoint();
            else {
                ct->startContourLine();
                ct->addPoint();
            }
        } else { //All sections OFF so if on, turn off
            if (ct->isContourOn) {
                ct->stopContourLine();
            }
        }

        //Build contour line if close enough to a patch
        if (ct->isContourBtnOn)
            ct->buildContourGuidanceLine(pn->easting, pn->northing);
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
    for (int j = 0; j < vehicle->numOfSections; j++)
    {
        if (j == 0)
        {
            //only one first left point, the rest are all rights moved over to left
            vehicle->section[j].leftPoint = Vec2(cosHeading * (vehicle->section[j].positionLeft) + easting,
                               sinHeading * (vehicle->section[j].positionLeft) + northing);

            left = vehicle->section[j].leftPoint - vehicle->section[j].lastLeftPoint;

            //save a copy for next time
            vehicle->section[j].lastLeftPoint = vehicle->section[j].leftPoint;

            //get the speed for left side only once
            leftSpeed = left.getLength() / fixUpdateTime * 10;
            leftLook = leftSpeed * vehicle->toolLookAhead;

            //save the far left speed
            vehicle->toolFarLeftSpeed = leftSpeed;


        }
        else
        {
            //right point from last section becomes this left one
            vehicle->section[j].leftPoint = vehicle->section[j-1].rightPoint;
            left = vehicle->section[j].leftPoint - vehicle->section[j].lastLeftPoint;

            //save a copy for next time
            vehicle->section[j].lastLeftPoint = vehicle->section[j].leftPoint;
            leftSpeed = rightSpeed;

        }


        vehicle->section[j].rightPoint = Vec2(cosHeading * (vehicle->section[j].positionRight) + easting,
                            sinHeading * (vehicle->section[j].positionRight) + northing);

        //now we have left and right for this section
        right = vehicle->section[j].rightPoint - vehicle->section[j].lastRightPoint;

        //save a copy for next time
        vehicle->section[j].lastRightPoint = vehicle->section[j].rightPoint;

        //grab vector length and convert to meters/sec/10 pixels per meter
        rightSpeed = right.getLength() / fixUpdateTime * 10;
        rightLook = rightSpeed * vehicle->toolLookAhead;

        //Is section outer going forward or backward
        double head = left.headingXZ();
        if (M_PI - fabs(fabs(head - vehicle->fixHeadingSection) - M_PI) > PIBy2) leftLook *= -1;

        head = right.headingXZ();
        if (M_PI - fabs(fabs(head - vehicle->fixHeadingSection) - M_PI) > PIBy2) rightLook *= -1;

        //choose fastest speed
        if (fabs(leftLook) > 0.000001 and fabs(rightLook) > 0.000001) {
            if (leftLook > rightLook)  vehicle->section[j].sectionLookAhead = leftLook;
            else vehicle->section[j].sectionLookAhead = rightLook;

            if (vehicle->section[j].sectionLookAhead > 190) vehicle->section[j].sectionLookAhead = 190;
        }
        //if (vehicle->section[j].sectionLookAhead == 0)
        //    qDebug() << j << vehicle->section[j].sectionLookAhead;


        //Doing the slow mo, exceeding buffer so just set as minimum 0.5 meter
        if (currentStepFix >= totalFixSteps - 1) vehicle->section[j].sectionLookAhead = 5;

    }//endfor

    //determine if section is in boundary using the section left/right positions
    for (int j = 0; j < vehicle->numOfSections; j++)
    {
        bool isLeftIn = true, isRightIn = true;

        if (boundary->isSet)
        {
            if (j == 0)
            {
                //only one first left point, the rest are all rights moved over to left
                isLeftIn = boundary->isPrePointInPolygon(vehicle->section[j].leftPoint);
                isRightIn = boundary->isPrePointInPolygon(vehicle->section[j].rightPoint);

                if (isLeftIn && isRightIn) vehicle->section[j].isInsideBoundary = true;
                else vehicle->section[j].isInsideBoundary = false;
            }

            else
            {
                //grab the right of previous section, its the left of this section
                isLeftIn = isRightIn;
                isRightIn = boundary->isPrePointInPolygon(vehicle->section[j].rightPoint);
                if (isLeftIn && isRightIn) vehicle->section[j].isInsideBoundary = true;
                else vehicle->section[j].isInsideBoundary = false;
            }
        }

        //no boundary created so always inside
        else vehicle->section[j].isInsideBoundary = true;
    }

    //with left and right tool velocity to determine rate of triangle generation, corners are more
    //save far right speed, 0 if going backwards, in meters/sec
    if (vehicle->section[vehicle->numOfSections - 1].sectionLookAhead > 0) vehicle->toolFarRightSpeed = rightSpeed * 0.05;
    else vehicle->toolFarRightSpeed = 0;

    //safe left side, 0 if going backwards, in meters/sec convert back from pixels/m
    if (vehicle->section[0].sectionLookAhead > 0) vehicle->toolFarLeftSpeed = vehicle->toolFarLeftSpeed * 0.05;
    else vehicle->toolFarLeftSpeed = 0;
}

//the start of first few frames to initialize entire program
void FormGPS::initializeFirstFewGPSPositions()
{
    if (!isFirstFixPositionSet)
    {
        //reduce the huge utm coordinates
        pn->utmEast = (int)(pn->easting);
        pn->utmNorth = (int)(pn->northing);
        //qDebug() << pn->utmEast <<" from " << pn->easting;
        pn->easting = pn->easting - pn->utmEast;
        pn->northing = pn->northing - pn->utmNorth;
        //qDebug() << "pn easting is now " << pn->easting;
        //Draw a grid once we know where in the world we are.
        isFirstFixPositionSet = true;
        worldGrid->createWorldGrid(pn->northing, pn->easting);

        //most recent fixes
        prevFix.easting = pn->easting;
        prevFix.northing = pn->northing;

        stepFixPts[0].easting = pn->easting;
        stepFixPts[0].northing = pn->northing;
        stepFixPts[0].heading = 0;

        //run once and return
        isFirstFixPositionSet = true;
        return;
    }

    else
    {

        //most recent fixes
        prevFix.easting = pn->easting; prevFix.northing = pn->northing;

        //load up history with valid data
        for (int i = totalFixSteps - 1; i > 0; i--)
        {
            stepFixPts[i].easting = stepFixPts[i - 1].easting;
            stepFixPts[i].northing = stepFixPts[i - 1].northing;
            stepFixPts[i].heading = stepFixPts[i - 1].heading;
        }

        stepFixPts[0].heading = pn->distance(pn->northing, pn->easting, stepFixPts[0].northing, stepFixPts[0].easting);
        stepFixPts[0].easting = pn->easting;
        stepFixPts[0].northing = pn->northing;

        //keep here till valid data
        if (startCounter > totalFixSteps) isGPSPositionInitialized = true;

        //in radians
        vehicle->fixHeading = atan2(pn->easting - stepFixPts[totalFixSteps - 1].easting, pn->northing - stepFixPts[totalFixSteps - 1].northing);
        if (vehicle->fixHeading < 0) vehicle->fixHeading += twoPI;
        vehicle->fixHeadingSection = vehicle->fixHeading;

        //send out initial zero settings
        if (isGPSPositionInitialized) autoSteerSettingsOutToPort();

        return;
    }
}

