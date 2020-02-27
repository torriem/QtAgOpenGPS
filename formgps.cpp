#include "formgps.h"
#include "aogsettings.h"
#include <QColor>
#include <QRgb>
#include "qmlutil.h"
#include "glm.h"
#include "toplinedisplay.h"
#include <QLocale>
#include <QLabel>

FormGPS::FormGPS(QWidget *parent) :
    QQuickView(qobject_cast<QWindow *>(parent))
{
    USE_SETTINGS;
    setupGui();
    AOGSettings s;

    isUDPServerOn = s.value("port/udp_on", true).toBool();


    /* test data to see if drawing routines are working. */
    /*
    isGPSPositionInitialized = true;
    pivotAxlePos.easting = 5005;
    pivotAxlePos.northing = 5005;
    fixEasting = 5005;
    fixNorthing = 5000;
    fixHeadingSection = 0;
    fixHeading = 0;
    fixHeadingCam = 0;
    fixHeadingSection = 0;
    worldGrid->createWorldGrid(5005,5005);
    */

    isAreaOnRight = s.value("vehicle/isAreaOnRight",false).toBool();

    isGridOn = s.value("display/showGrid",true).toBool();
    isMetric = s.value("display/isMetric", true).toBool();

    //TODO: move all the settings loading/setting of defaults to a separate
    //function.
    minFixStepDist = s.value("position/minFixStep",1.0).toDouble(); //minimum distance between fixes.
    isAtanCam = s.value("camera/useCalculatedHeading", true).toBool();

    //fieldColor = QColor(s.value("display/fieldColor", "#82781E").toString());
    //sectionColor = QColor(s.value("display/sectionColor", "#32DCC8").toString());


    tool.section[0].positionLeft = -8;
    tool.section[0].positionRight = -4;
    tool.section[1].positionLeft = -4;
    tool.section[1].positionRight = 0.0;
    tool.section[2].positionLeft = 0.0;
    tool.section[2].positionRight = 4;
    tool.section[3].positionLeft = 4;
    tool.section[3].positionRight = 8;
    tool.numOfSections = 4; //0-3
    tool.numSuperSection = 5;
    tool.toolTrailingHitchLength = -10; //30 foot hitch to see following action better

    tool.sectionCalcWidths();
    //turn on the right number of section buttons.
    //we don't need to do this on resize, but we will need
    //to call it when settings change.
    lineUpManualBtns();

    //hard wire this on for testing
    isJobStarted = true;
    tool.section[0].isAllowedOn = true;
    tool.section[1].isAllowedOn = true;
    tool.section[2].isAllowedOn = true;
    tool.section[3].isAllowedOn = true;

    if (isUDPServerOn) startUDPServer();

    //TODO: connect signals from various classes
    connect(&pn, SIGNAL(setRollX16(int)), &ahrs, SLOT(setRollX16(int)));
    connect(&pn, SIGNAL(setCorrectionHeadingX16(int)), &ahrs,SLOT(setCorrectionHeadingX16(int)));
    connect(&pn, SIGNAL(clearRecvCounter()), this, SLOT(onClearRecvCounter()));
    connect(&pn, SIGNAL(newSpeed(double)), &vehicle, SLOT(onNewSpeed(double)));

    connect(&curve, SIGNAL(doSequence(CYouTurn&)), &seq, SLOT(DoSequenceEvent(CYouTurn&)));

    connect(&ABLine, SIGNAL(doSequence(CYouTurn&)), &seq, SLOT(DoSequenceEvent(CYouTurn&)));
    //connect(&ABLine,SIGNAL(showMessage(int,QString,QString)),...

    //connnect(&ct, SIGNAL(showMessage(int,QString,QString))

    //connect(&hd, SIGNAL(moveHydraulics(int))..

    //connect(&mc, SIGNAL(sendAutoSteerDataOutToPort())
    //connect(&mc, SIGNAL(sendAutoSteerSettingsOutToPort())
    //connect(&mc, SIGNAL(sendRelayOutToPort(uchar*,int))

    connect(&yt, SIGNAL(outOfBounds()), &mc, SLOT(setOutOfBounds()));
    connect(&yt, SIGNAL(resetSequenceEventTriggers()), &seq, SLOT(ResetSequenceEventTriggers()));
    //connect(&yt, SIGNAL(showMessage(int,QString,QString))
    connect(&yt, SIGNAL(swapDirection()), this, SLOT(swapDirection()));
    connect(&yt, SIGNAL(setTriggerSequence(bool)), &seq, SLOT(setIsSequenceTriggered(bool)));
    connect(&yt, SIGNAL(turnOffBoundAlarm()), this, SLOT(turnOffBoundAlarm()));

    connect(&recPath, SIGNAL(setSimStepDistance(double)),&sim,SLOT(setSimStepDistance(double)));
    //connect(&recPath,SIGNAL(stoppedDriving())

    connect(&seq, SIGNAL(doYouTurnSequenceEvent(int,int)), this, SLOT(DoYouTurnSequenceEvent(int,int)));
    //connect(&seq, SIGNAL(setDistanceToolToTurnLine(double)) //unused

    connect(&sim, SIGNAL(new_position(QByteArray)), this, SLOT(onSimNewPosition(QByteArray)));


}

FormGPS::~FormGPS()
{
    /* clean up our dynamically-allocated
     * objects.
     */
}

//This used to be part of oglBack_paint in the C# code, but
//because openGL rendering can potentially be in another thread here, it's
//broken out here.  So the lookaheadPixels array has been populated already
//by the rendering routine.
void FormGPS::processSectionLookahead() {

    //not using regular Qt Widgets in the main window anymore.  For
    //debugging purposes, this could go in another popup window
    //ui->grnPixels->setPixmap(QPixmap::fromImage(grnPix.mirrored()));

    //determine farthest ahead lookahead - is the height of the readpixel line
    double rpHeight = 0;

    //assume all sections are on and super can be on, if not set false to turn off.
    tool.isSuperSectionAllowedOn = true;

    //find any off buttons, any outside of boundary, going backwards, and the farthest lookahead
    for (int j = 0; j < tool.numOfSections; j++)
    {
        if (tool.section[j].sectionLookAhead > rpHeight) rpHeight = tool.section[j].sectionLookAhead;
        if (tool.section[j].manBtnState == btnStates::Off) tool.isSuperSectionAllowedOn = false;
        if (!tool.section[j].isInsideBoundary) tool.isSuperSectionAllowedOn = false;

        //check if any sections going backwards
        if (tool.section[j].sectionLookAhead < 0) tool.isSuperSectionAllowedOn = false;
    }

    //if only one section, or going slow no need for super section
    if (tool.numOfSections == 1 || pn.speed < tool.slowSpeedCutoff)
            tool.isSuperSectionAllowedOn = false;

    //clamp the height after looking way ahead, this is for switching off super section only
    rpHeight = fabs(rpHeight) * 2.0;
    if (rpHeight > 195) rpHeight = 195;
    if (rpHeight < 8) rpHeight = 8;
    //qDebug() << rpHeight;

    //lookaheadPixels was read in the OpenGL rendering routine.

    //10 % min is required for overlap, otherwise it never would be on.
    int pixLimit = (int)((double)(tool.rpWidth * rpHeight)/(double)(tool.numOfSections*1.5));

    //is applied area coming up?
    int totalPixs = 0;
    if (tool.isSuperSectionAllowedOn)
    {
        //look for anything applied coming up
        for (int a = 0; a < (tool.rpWidth * rpHeight); a++)
        {
            if (lookaheadPixels[a].green != 0) // && lookaheadPixels[a].red == 0 and lookaheadPixels[a].blue == 0)
            {
                if (lookaheadPixels[a].red != 0 || lookaheadPixels[a].blue != 0) {
                    qDebug() << "position " << a << "showed spurious pixel" << int(lookaheadPixels[a].green);
                }
                if (totalPixs++ > pixLimit)
                {
                    tool.isSuperSectionAllowedOn = false;
                    break;
                }

                //check for a boundary line
                if (lookaheadPixels[a].green > 200)
                {
                    tool.isSuperSectionAllowedOn = false;
                    break;
                }
            }
        }
    }


    // If ALL sections are required on, No buttons are off, within boundary, turn super section on, normal sections off
    if (tool.isSuperSectionAllowedOn)
    {
        for (int j = 0; j < tool.numOfSections; j++)
        {
            if (tool.section[j].isSectionOn)
            {
                tool.section[j].sectionOffRequest = true;
                tool.section[j].sectionOnRequest = false;
                tool.section[j].sectionOffTimer = 0;
                tool.section[j].sectionOnTimer = 0;
            }
        }

        //turn on super section
        tool.section[tool.numOfSections].sectionOnRequest = true;
        tool.section[tool.numOfSections].sectionOffRequest = false;
    }

    /* Below is priority based. The last if statement is the one that is
        * applied and takes the highest priority. Digital input controls
        * have the highest priority and overide all buttons except
        * the manual button which exits the loop and just turns sections on....
        * Because isn't that what manual means! */

    //turn on indivdual sections as super section turn off
    else
    {
        //qDebug() << "super section not allowed on.";
        //Read the pixels ahead of tool a normal section at a time. Each section can have its own lookahead manipulated.

        //moved variable declarations here because compiler complained about how the goto
        //statements bypassed the initializers.
        int start, end; //, skip;
        int tagged;

        for (int j = 0; j < tool.numOfSections; j++)
        {
            //is section going backwards?
            if (tool.section[j].sectionLookAhead > 0)
            {
                //If any nowhere applied, send OnRequest, if its all green send an offRequest
                tool.section[j].isSectionRequiredOn = false;

                if (bnd.bndArr.size() > 0)
                {

                    start = 0, end = 0; //, skip = 0;
                    start = tool.section[j].rpSectionPosition - tool.section[0].rpSectionPosition;
                    end = tool.section[j].rpSectionWidth - 1 + start;
                    if (end > tool.rpWidth - 1) end = tool.rpWidth - 1;
                    //skip = tool.rpWidth - (end - start);


                    tagged = 0;
                    for (int h = 0; h < (int)tool.section[j].sectionLookAhead; h++)
                    {
                        for (int a = start; a < end; a++)
                        {
                            if (lookaheadPixels[a].green == 0)
                            {
                                if (tagged++ > tool.toolMinUnappliedPixels)
                                {
                                    tool.section[j].isSectionRequiredOn = true;
                                    goto GetMeOutaHere;
                                }
                            }
                        }

                        start += tool.rpWidth;
                        end += tool.rpWidth;
                    }

                    //minimum apllied conditions met
GetMeOutaHere:

                    start = 0; end = 0; //skip = 0;
                    start = tool.section[j].rpSectionPosition - tool.section[0].rpSectionPosition;
                    end = tool.section[j].rpSectionWidth - 1 + start;
                    if (end > tool.rpWidth - 1) end = tool.rpWidth - 1;
                    //skip = tool.rpWidth - (end - start);

                    //looking for boundary line color, bright green
                    for (int h = 0; h < (int)tool.section[j].sectionLookAhead; h++)
                    {
                        for (int a = start; a < end; a++)
                        {
                            if (lookaheadPixels[a].green > 240) //&& )
                            {
                                tool.section[j].isSectionRequiredOn = false;
                                tool.section[j].sectionOffRequest = true;
                                tool.section[j].sectionOnRequest = false;
                                tool.section[j].sectionOffTimer = 0;
                                tool.section[j].sectionOnTimer = 0;

                                goto GetMeOutaHere1;
                            }
                        }

                        start += tool.rpWidth;
                        end += tool.rpWidth;
                    }

                    GetMeOutaHere1:

                    //if out of boundary, turn it off
                    if (!tool.section[j].isInsideBoundary)
                    {
                        tool.section[j].isSectionRequiredOn = false;
                        tool.section[j].sectionOffRequest = true;
                        tool.section[j].sectionOnRequest = false;
                        tool.section[j].sectionOffTimer = 0;
                        tool.section[j].sectionOnTimer = 0;
                    }
                }

                //no boundary set so ignore
                else
                {
                    tool.section[j].isSectionRequiredOn = false;

                    int start = 0, end = 0; //, skip = 0;
                    start = tool.section[j].rpSectionPosition - tool.section[0].rpSectionPosition;
                    end = tool.section[j].rpSectionWidth - 1 + start;
                    if (end > tool.rpWidth - 1) end = tool.rpWidth - 1;
                    //skip = tool.rpWidth - (end - start);


                    int tagged = 0;
                    for (int h = 0; h < (int)tool.section[j].sectionLookAhead; h++)
                    {
                        for (int a = start; a < end; a++)
                        {
                            if (lookaheadPixels[a].green == 0)
                            {
                                if (tagged++ > tool.toolMinUnappliedPixels)
                                {
                                    tool.section[j].isSectionRequiredOn = true;
                                    goto GetMeOutaHere2;
                                }
                            }
                        }

                        start += tool.rpWidth;
                        end += tool.rpWidth;
                    }

                    //minimum apllied conditions met
                GetMeOutaHere2:
                    start = 0;
                }
            }

            //if section going backwards turn it off
            else tool.section[j].isSectionRequiredOn = false;

        }

        //if the superSection is on, turn it off
        if (tool.section[tool.numOfSections].isSectionOn)
        {
            tool.section[tool.numOfSections].sectionOffRequest = true;
            tool.section[tool.numOfSections].sectionOnRequest = false;
            tool.section[tool.numOfSections].sectionOffTimer = 0;
            tool.section[tool.numOfSections].sectionOnTimer = 0;
        }

        //if Master Auto is on
        for (int j = 0; j < tool.numOfSections; j++)
        {
            if (tool.section[j].isSectionRequiredOn && tool.section[j].isAllowedOn)
            {
                //global request to turn on section
                tool.section[j].sectionOnRequest = true;
                tool.section[j].sectionOffRequest = false;
            }

            else if (!tool.section[j].isSectionRequiredOn)
            {
                //qDebug() << "requesting off for section " << j;
                //global request to turn off section
                tool.section[j].sectionOffRequest = true;
                tool.section[j].sectionOnRequest = false;
            }

            // Manual on, force the section On and exit loop so digital is also overidden
            if (tool.section[j].manBtnState == btnStates::On)
            {
                tool.section[j].sectionOnRequest = true;
                tool.section[j].sectionOffRequest = false;
                continue;
            }

            if (tool.section[j].manBtnState == btnStates::Off)
            {
                tool.section[j].sectionOnRequest = false;
                tool.section[j].sectionOffRequest = true;
            }

            //if going too slow turn off sections
            if (pn.speed < tool.slowSpeedCutoff)
            {
                //qDebug() << "section speed too slow on "<< j;
                tool.section[j].sectionOnRequest = false;
                tool.section[j].sectionOffRequest = true;
            }

            //digital input Master control (WorkSwitch)
            if (isJobStarted && mc.isWorkSwitchEnabled)
            {
                //TODO implement work switch logic.
                //check condition of work switch
                if (mc.isWorkSwitchActiveLow)
                {
                    if (mc.workSwitchValue == 0)
                        { tool.section[j].sectionOnRequest = true; tool.section[j].sectionOffRequest = false; }
                    else { tool.section[j].sectionOnRequest = false; tool.section[j].sectionOffRequest = true; }
                }
                else
                {
                    if (mc.workSwitchValue == 1)
                        { tool.section[j].sectionOnRequest = true; tool.section[j].sectionOffRequest = false; }
                    else { tool.section[j].sectionOnRequest = false; tool.section[j].sectionOffRequest = true; }
                }
            }
        }
    }

    //double check the work switch to enable/disable auto section button
    if (isJobStarted)
    {
        if (!mc.isWorkSwitchEnabled) {
            btnSectionOffAutoOn->setProperty("enabled", "true");
        } else {
            btnSectionOffAutoOn->setProperty("enabled", "false");
        }
    }


    //Determine if sections want to be on or off
    processSectionOnOffRequests();
    //send the byte out to section relays
    //TODO:buildSectionRelayByte();
    //sectionControlOutToPort(mc.relayData, pgnSentenceLength);

    frameTime = (double)swFrame.elapsed() / 1000; //QElapsedTimer is in milliseconds

    //if a minute has elapsed save the field in case of crash and to be able to resume
    if (saveCounter > 59)       //3 counts per second X 60 seconds = 180 counts per minute.
    {
        if (isJobStarted && qmlItem(qml_root, "stripOnlineGPS")->property("state").toString() == "ok")
        {
            //auto save the field patches, contours accumulated so far
            //TODO: fileSaveField();
            //TODO: fileSaveContour();

            //NMEA log file
            //TODO: if (isLogNMEA) fileSaveNMEA();
        }
        saveCounter = 0;
    }
    //this is the end of the "frame". Now we wait for next NMEA sentence.
}

//Does the logic to process section on off requests
void FormGPS::processSectionOnOffRequests()
{
    //if (pn.speed > 0.2)
    {
        for (int j = 0; j < tool.numOfSections+1; j++)
        {
            //Turn ON
            //if requested to be on, set the timer to Max 10 (1 seconds) = 10 frames per second
            if (tool.section[j].sectionOnRequest && !tool.section[j].sectionOnOffCycle)
            {
                tool.section[j].sectionOnTimer = (int)(pn.speed * tool.toolLookAhead)+1;
                if (tool.section[j].sectionOnTimer > fixUpdateHz+3) tool.section[j].sectionOnTimer = fixUpdateHz+3;
                tool.section[j].sectionOnOffCycle = true;
            }

            //reset the ON request
            tool.section[j].sectionOnRequest = false;

            //decrement the timer if not zero
            if (tool.section[j].sectionOnTimer > 0)
            {
                //turn the section ON if not and decrement timer
                tool.section[j].sectionOnTimer--;
                if (!tool.section[j].isSectionOn) tool.section[j].turnSectionOn(vehicle);

                //keep resetting the section OFF timer while the ON is active
                tool.section[j].sectionOffTimer = (int)(fixUpdateHz * tool.toolTurnOffDelay);
            }

            if (!tool.section[j].sectionOffRequest) tool.section[j].sectionOffTimer = (int)(fixUpdateHz * tool.toolTurnOffDelay);

            //decrement the off timer
            if (tool.section[j].sectionOffTimer > 0) tool.section[j].sectionOffTimer--;

            //Turn OFF
            //if Off section timer is zero, turn off the section
            if (tool.section[j].sectionOffTimer == 0 && tool.section[j].sectionOnTimer == 0 && tool.section[j].sectionOffRequest)
            {
                if (tool.section[j].isSectionOn) tool.section[j].turnSectionOff(vehicle);
                tool.section[j].sectionOnOffCycle = false;
                tool.section[j].sectionOffRequest = false;
            }
        }
    }
}

void FormGPS::tmrWatchdog_timeout()
{
    if(pn.rawBuffer.indexOf('\n') < 0 )
        return; //No sentence available

    //tmrWatchdog->stop();
    //did we get a new fix position?
    if (scanForNMEA())
    {
        if (threeSecondCounter++ >= fixUpdateHz * 2)
        {
            threeSecondCounter = 0;
            threeSeconds++;
        }
        if (oneSecondCounter++ >= fixUpdateHz)
        {
            oneSecondCounter = 0;
            oneSecond++;
        }
        if (oneHalfSecondCounter++ >= fixUpdateHz / 2)
        {
            oneHalfSecondCounter = 0;
            oneHalfSecond++;
        }
        if (oneFifthSecondCounter++ >= fixUpdateHz / 5)
        {
            oneFifthSecondCounter = 0;
            oneFifthSecond++;
        }

        /////////////////////////////////////////////////////////   333333333333333  ////////////////////////////////////////
        //every 3 second update status
        if (displayUpdateThreeSecondCounter != threeSeconds)
        {
            //reset the counter
            displayUpdateThreeSecondCounter = threeSeconds;

            //check to make sure the grid is big enough
            worldGrid.checkZoomWorldGrid(pn.fix.northing, pn.fix.easting);

            //TODO: batman panel

            if (isMetric)
            {
                //TODO: status bar updates
                /*
                lblTotalFieldArea.Text = fd.AreaBoundaryLessInnersHectares;
                lblTotalAppliedArea.Text = fd.WorkedHectares;
                lblWorkRemaining.Text = fd.WorkedAreaRemainHectares;
                lblPercentRemaining.Text = fd.WorkedAreaRemainPercentage;
                lblTimeRemaining.Text = fd.TimeTillFinished;

                lblAreaAppliedMinusOverlap.Text = ((fd.actualAreaCovered * glm.m2ha).ToString("N2"));
                lblAreaMinusActualApplied.Text = (((fd.areaBoundaryOuterLessInner - fd.actualAreaCovered) * glm.m2ha).ToString("N2"));
                lblOverlapPercent.Text = (fd.overlapPercent.ToString("N2")) + "%";
                lblAreaOverlapped.Text = (((fd.workedAreaTotal - fd.actualAreaCovered) * glm.m2ha).ToString("N3"));

                btnManualOffOn.Text = fd.AreaBoundaryLessInnersHectares;
                lblEqSpec.Text = (Math.Round(tool.toolWidth, 2)).ToString() + " m  " + vehicleFileName + toolFileName;
                */
            }
            else //imperial
            {
                /*
                lblTotalFieldArea.Text = fd.AreaBoundaryLessInnersAcres;
                lblTotalAppliedArea.Text = fd.WorkedAcres;
                lblWorkRemaining.Text = fd.WorkedAreaRemainAcres;
                lblPercentRemaining.Text = fd.WorkedAreaRemainPercentage;
                lblTimeRemaining.Text = fd.TimeTillFinished;

                lblAreaAppliedMinusOverlap.Text = ((fd.actualAreaCovered * glm.m2ac).ToString("N2"));
                lblAreaMinusActualApplied.Text = (((fd.areaBoundaryOuterLessInner - fd.actualAreaCovered) * glm.m2ac).ToString("N2"));
                lblOverlapPercent.Text = (fd.overlapPercent.ToString("N2")) + "%";
                lblAreaOverlapped.Text = (((fd.workedAreaTotal - fd.actualAreaCovered) * glm.m2ac).ToString("N3"));

                btnManualOffOn.Text = fd.AreaBoundaryLessInnersAcres;
                lblEqSpec.Text =  (Math.Round(tool.toolWidth * glm.m2ft, 2)).ToString() + " ft  " + vehicleFileName + toolFileName;
                */
            }

            //not Metric/Standard units sensitive
            //TODO: line button updates
            /*
            if (ABLine.isBtnABLineOn) btnABLine.Text = "# " + PassNumber;
            else btnABLine.Text = "";

            if (curve.isBtnCurveOn) btnCurve.Text = "# " + CurveNumber;
            else btnCurve.Text = "";

            //update the online indicator 63 green red 64
            if (recvCounter > 20 && toolStripBtnGPSStength.Image.Height != 64)
            {
                //stripOnlineGPS.Value = 1;
                lblEasting.Text = "-";
                lblNorthing.Text = gStr.gsNoGPS;
                //lblZone.Text = "-";
                toolStripBtnGPSStength.Image = Resources.GPSSignalPoor;
            }
            else if (recvCounter < 20 && toolStripBtnGPSStength.Image.Height != 63)
            {
                //stripOnlineGPS.Value = 100;
                toolStripBtnGPSStength.Image = Resources.GPSSignalGood;
            }

            lblDateTime.Text = DateTime.Now.ToString("HH:mm:ss") + "\n\r" + DateTime.Now.ToString("ddd MMM yyyy");
            */
        }//end every 3 seconds



        //every second update all status ///////////////////////////   1 1 1 1 1 1 ////////////////////////////
        if (displayUpdateOneSecondCounter != oneSecond)
        {
            //reset the counter
            displayUpdateOneSecondCounter = oneSecond;

            //counter used for saving field in background
            saveCounter++;

            /*
            if (panelBatman.Visible)
            {
                //both
                lblLatitude.Text = Latitude;
                lblLongitude.Text = Longitude;

                pbarRelayComm.Value = pbarRelay;

                lblRoll.Text = RollInDegrees;
                lblYawHeading.Text = GyroInDegrees;
                lblGPSHeading.Text = GPSHeading;

                //up in the menu a few pieces of info
                if (isJobStarted)
                {
                    lblEasting.Text = "E:" + (pn.fix.easting).ToString("N2");
                    lblNorthing.Text = "N:" + (pn.fix.northing).ToString("N2");
                }
                else
                {
                    lblEasting.Text = "E:" + (pn.actualEasting).ToString("N2");
                    lblNorthing.Text = "N:" + (pn.actualNorthing).ToString("N2");
                }

                lblUturnByte.Text = Convert.ToString(mc.autoSteerData[mc.sdYouTurnByte], 2).PadLeft(6, '0');
            }

            if (ABLine.isBtnABLineOn && !ct.isContourBtnOn)
            {
                btnEditHeadingB.Text = ((int)(ABLine.moveDistance * 100)).ToString();
            }
            if (curve.isBtnCurveOn && !ct.isContourBtnOn)
            {
                btnEditHeadingB.Text = ((int)(curve.moveDistance * 100)).ToString();
            }

            pbarAutoSteerComm.Value = pbarSteer;
            pbarUDPComm.Value = pbarUDP;
            */

            if (mc.steerSwitchValue == 0)
            {
                //this.AutoSteerToolBtn.BackColor = System.Drawing.Color.LightBlue;
            }
            else
            {
                //this.AutoSteerToolBtn.BackColor = System.Drawing.Color.Transparent;
            }


            //AutoSteerAuto button enable - Ray Bear inspired code - Thx Ray!
            if (isJobStarted && ahrs.isAutoSteerAuto && !recPath.isDrivingRecordedPath &&
                (ABLine.isBtnABLineOn || ct.isContourBtnOn || curve.isBtnCurveOn))
            {
                if (mc.steerSwitchValue == 0)
                {
                    //if (!isAutoSteerBtnOn) btnAutoSteer.PerformClick();
                }
                else
                {
                    //if ( isAutoSteerBtnOn) btnAutoSteer.PerformClick();
                }
            }

            //Make sure it is off when it should
            //if ((!ABLine.isBtnABLineOn && !ct.isContourBtnOn && !curve.isBtnCurveOn && isAutoSteerBtnOn) || (recPath.isDrivingRecordedPath && isAutoSteerBtnOn)) btnAutoSteer.PerformClick();

            //do all the NTRIP routines
            //DoNTRIPSecondRoutine();

            //the main formgps window
            /*
            if (isMetric)  //metric or imperial
            {
                //Hectares on the master section soft control and sections
                btnSectionOffAutoOn.Text = fd.WorkedHectares;
                lblSpeed.Text = SpeedKPH;

                //status strip values
                distanceToolBtn.Text = fd.DistanceUserMeters + "\r\n" + fd.WorkedUserHectares2;

                btnContour.Text = XTE; //cross track error

            }
            else  //Imperial Measurements
            {
                //acres on the master section soft control and sections
                btnSectionOffAutoOn.Text = fd.WorkedAcres;
                lblSpeed.Text = SpeedMPH;

                //status strip values
                distanceToolBtn.Text = fd.DistanceUserFeet + "\r\n" + fd.WorkedUserAcres2;
                btnContour.Text = InchXTE; //cross track error
            }

            //statusbar flash red undefined headland
            if (mc.isOutOfBounds && statusStripBottom.BackColor == Color.Transparent
                || !mc.isOutOfBounds && statusStripBottom.BackColor == Color.Tomato)
            {
                if (!mc.isOutOfBounds)
                {
                    statusStripBottom.BackColor = Color.Transparent;
                }
                else
                {
                    statusStripBottom.BackColor = Color.Tomato;
                }
            }
            */
        }

        //every half of a second update all status  ////////////////    0.5  0.5   0.5    0.5    /////////////////
        if (displayUpdateHalfSecondCounter != oneHalfSecond)
        {
            //reset the counter
            displayUpdateHalfSecondCounter = oneHalfSecond;

            if (hd.isOn)
            {
                //if (hd.isOn)
                //lblUpDown.Text = hd.FindHeadlandDistance().ToString();
                hd.findHeadlandDistance(vehicle, tool);
                if (hd.isToolUp)
                {
                    //lblHeadLeftDist.BackColor = Color.Salmon;
                    //lblHeadRightDist.BackColor = Color.Salmon;
                }
                else
                {
                    //lblHeadLeftDist.BackColor = Color.LightSeaGreen;
                    //lblHeadRightDist.BackColor = Color.LightSeaGreen;
                }

                //lblHeadLeftDist.Text = hd.leftToolDistance.ToString("N2");
                //lblHeadRightDist.Text = hd.rightToolDistance.ToString("N2");
            }

            //lblTrigger.Text = sectionTriggerStepDistance.ToString("N2");
            //lblLift.Text = mc.pgn[mc.azRelayData][mc.rdHydLift].ToString();

        } //end every 1/2 second

        //every fifth second update  ///////////////////////////   FIFTH Fifth ////////////////////////////
        if (displayUpdateOneFifthCounter != oneFifthSecond)
        {
            //reset the counter
            displayUpdateOneFifthCounter = oneFifthSecond;

            if ((vehicle.guidanceLineDistanceOff == 32020) | (vehicle.guidanceLineDistanceOff == 32000))
            {
                //steerAnglesToolStripDropDownButton1.Text = "Off \r\n" + ActualSteerAngle;
            }
            else
            {
                //steerAnglesToolStripDropDownButton1.Text = SetSteerAngle + "\r\n" + ActualSteerAngle;
            }

            //lblHz.Text = NMEAHz + "Hz " + (int)(frameTime) + "\r\n" + FixQuality + HzTime.ToString("N1") + " Hz";
        }

    } //there was a new GPS update

    //start timer again and wait for new fix
    //qmlItem(qml_root,"stripAreaUser")->setProperty("text", locale.toString(vehicle.totalUserSquareMeters * 0.0001,'f',1) + " " + tr("Ha"));
    //tmrWatchdog->start();

    //wait till timer fires again.
}

//force all the buttons same according to two main buttons
void FormGPS::manualAllBtnsUpdate()
{
    manualBtnUpdate(0);
    manualBtnUpdate(1);
    manualBtnUpdate(2);
    manualBtnUpdate(3);
    manualBtnUpdate(4);
    manualBtnUpdate(5);
    manualBtnUpdate(6);
    manualBtnUpdate(7);
}

//line up section On Off Auto buttons based on how many there are
//in QML they are always centered.
void FormGPS::lineUpManualBtns()
{
    QObject* button;
    for (int b=0; b< MAXSECTIONS-1; b++ ) {
        button = qmlItem(qml_root,QString("section")+QString::number(b));

        //temporarily enable them so we can test them
        button->setProperty("enabled", "true");
        if (b < tool.numOfSections) {
            button->setProperty("visible","true");
            if (isJobStarted)
                button->setProperty("enabled", "true");
        } else {
            button->setProperty("visible","false");
        }
    }
}

//udate individual btn based on state after push.
void FormGPS::manualBtnUpdate(int sectNumber)
{
    QObject *button = qmlItem(qml_root,QString("section")+QString::number(sectNumber));

    switch(tool.section[sectNumber].manBtnState) {
    case btnStates::Off:
        tool.section[sectNumber].manBtnState = btnStates::Auto;
        button->setProperty("state","auto");
        break;
    case btnStates::Auto:
        tool.section[sectNumber].manBtnState = btnStates::On;
        button->setProperty("state","on");
        break;
    case btnStates::On:
        tool.section[sectNumber].manBtnState = btnStates::Off;
        button->setProperty("state","off");
        break;
    }
}

QString FormGPS::speedKPH() {
    double spd = 0;
    for (int c = 0; c < 10; c++)
        spd += vehicle.avgSpeed[c];

    //convert to kph
    spd *= 0.1;

    return locale.toString(spd,'f',1);
}

QString FormGPS::speedMPH() {
    double spd = 0;
    for (int c = 0; c < 10; c++)
        spd += vehicle.avgSpeed[c];

    //convert to mph
    spd *= 0.0621371;

    return locale.toString(spd,'f',1);
}

void FormGPS::swapDirection() {
    if (!yt.isYouTurnTriggered)
    {
        //is it turning right already?
        if (yt.isYouTurnRight)
        {
            yt.isYouTurnRight = false;
            yt.isLastYouTurnRight = !yt.isLastYouTurnRight;
            yt.resetCreatedYouTurn();
        }
        else
        {
            //make it turn the other way
            yt.isYouTurnRight = true;
            yt.isLastYouTurnRight = !yt.isLastYouTurnRight;
            yt.resetCreatedYouTurn();
        }
    }
    else
    {
        //if (yt.isYouTurnBtnOn) btnAutoYouTurn.PerformClick();
    }
}
