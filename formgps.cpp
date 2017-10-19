#include "formgps.h"
#include "ui_formgps.h"
#include "cworldgrid.h"
#include "cnmea.h"
#include "csection.h"
#include "cabline.h"
#include "ccontour.h"
#include "cboundary.h"
#include "cvehicle.h"
#include "mainoverlay.h"
#include "aogsettings.h"
#include <QColor>
#include <QRgb>
#include "qmlutil.h"
#include "glm.h"

FormGPS::FormGPS(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FormGPS)
{
    setupGui();
    AOGSettings s;

    /* initialize child objects */
    /* these objects need pointers to this form
     * to retrieve information. they are tightly
     * coupled!  So we have to create them here, since
     * the header file has only incomplete types.
     */
    worldGrid = new CWorldGrid(this);
    pn = new CNMEA(this);
    section = new CSection[MAXSECTIONS];
    for (int i = 0; i < MAXSECTIONS; i++)
        section[i].set_mainform(this);

    ABLine = new CABLine(this);
    ct = new CContour(this);
    vehicle = new CVehicle(this);
    boundary = new CBoundary(this);

    isUDPServerOn = s.value("port/udp_on", false).toBool();


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

    isGridOn = true;

    //TODO: move all the settings loading/setting of defaults to a separate
    //function.
    minFixStepDist = s.value("position/minFixStep",1.0).toDouble(); //minimum distance between fixes.
    isAtanCam = s.value("camera/useCalculatedHeading", true).toBool();

    //fieldColor = QColor(s.value("display/fieldColor", "#82781E").toString());
    //sectionColor = QColor(s.value("display/sectionColor", "#32DCC8").toString());


    section[0].positionLeft = -8.0;
    section[0].positionRight = -4.0;
    section[1].positionLeft = -4.0;
    section[1].positionRight = 0.0;
    section[2].positionLeft = 0.0;
    section[2].positionRight = 4.0;
    section[3].positionLeft = 4.0;
    section[3].positionRight = 8.0;

    sectionCalcWidths();
    //turn on the right number of section buttons.
    //we don't need to do this on resize, but we will need
    //to call it when settings change.
    lineUpManualBtns();

    //hard wire this on for testing
    isJobStarted = true;
    section[0].isAllowedOn = true;
    section[1].isAllowedOn = true;
    section[2].isAllowedOn = true;
    section[3].isAllowedOn = true;

    if (isUDPServerOn) startUDPServer();

}

FormGPS::~FormGPS()
{
    /* clean up our dynamically-allocated
     * objects.
     */
    delete ui;
    delete worldGrid;
    delete pn;
    delete[] section;
    delete ABLine;
    delete ct;
    delete vehicle;

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
        if (b < vehicle->numOfSections) {
            button->setProperty("visible","true");
            if (isJobStarted)
                button->setProperty("enabled", "true");
        } else {
            button->setProperty("visible","false");
        }
    }
}

//function to calculate teh width of each section and update
void FormGPS::sectionCalcWidths()
{
    for (int j = 0; j < MAXSECTIONS; j++)
    {
        section[j].sectionWidth = (section[j].positionRight - section[j].positionLeft);
        section[j].rpSectionPosition = 200 + (int)(roundAwayFromZero(section[j].positionLeft * 10));
        section[j].rpSectionWidth = (int)(roundAwayFromZero(section[j].sectionWidth * 10));
    }

    //calculate tool width based on extreme right and left values
    vehicle->toolWidth = fabs(section[0].positionLeft) + fabs(section[vehicle->numOfSections - 1].positionRight);

    //left and right tool position
    vehicle->toolFarLeftPosition = section[0].positionLeft;
    vehicle->toolFarRightPosition = section[vehicle->numOfSections - 1].positionRight;

    //now do the full width section
    section[vehicle->numOfSections].sectionWidth = vehicle->toolWidth;
    section[vehicle->numOfSections].positionLeft = vehicle->toolFarLeftPosition;
    section[vehicle->numOfSections].positionRight = vehicle->toolFarRightPosition;

    //find the right side pixel position
    vehicle->rpXPosition = 200 + (int)(roundAwayFromZero(vehicle->toolFarLeftPosition * 10));
    vehicle->rpWidth = (int)(roundAwayFromZero(vehicle->toolWidth * 10));
}

//udate individual btn based on state after push.
void FormGPS::manualBtnUpdate(int sectNumber)
{
    QObject *button = qmlItem(qml_root,QString("section")+QString::number(sectNumber));

    switch(section[sectNumber].manBtnState) {
    case btnStates::Off:
        section[sectNumber].manBtnState = btnStates::Auto;
        button->setProperty("state","auto");
        break;
    case btnStates::Auto:
        section[sectNumber].manBtnState = btnStates::On;
        button->setProperty("state","on");
        break;
    case btnStates::On:
        section[sectNumber].manBtnState = btnStates::Off;
        button->setProperty("state","off");
        break;
    }
}

//This used to be part of openGLControlBack_Draw in the C# code, but
//because openGL rendering can potentially be in another thread, it's
//broken out here, and runs when the QOpenGLWidget that does the section
//lookahead rendering has finished a frame.  So the grnPixels array has
//been populated already by the rendering routine.
void FormGPS::processSectionLookahead() {

    ui->grnPixels->setPixmap(QPixmap::fromImage(grnPix.mirrored()));
    //determine farthest ahead lookahead - is the height of the readpixel line
    double rpHeight = 0;

    //assume all sections are on and super can be on, if not set false to turn off.
    vehicle->isSuperSectionAllowedOn = true;

    //find any off buttons, any outside of boundary, going backwards, and the farthest lookahead
    for (int j = 0; j < vehicle->numOfSections; j++)
    {
        if (section[j].sectionLookAhead > rpHeight) rpHeight = section[j].sectionLookAhead;
        if (section[j].manBtnState == btnStates::Off) vehicle->isSuperSectionAllowedOn = false;
        if (!section[j].isInsideBoundary) vehicle->isSuperSectionAllowedOn = false;

        //check if any sections going backwards
        if (section[j].sectionLookAhead < 0) vehicle->isSuperSectionAllowedOn = false;
    }

    //if only one section, or going slow no need for super section
    if (vehicle->numOfSections == 1 || pn->speed < vehicle->slowSpeedCutoff)
            vehicle->isSuperSectionAllowedOn = false;

    //clamp the height after looking way ahead, this is for switching off super section only
    rpHeight = fabs(rpHeight) * 2.0;
    if (rpHeight > 195) rpHeight = 195;
    if (rpHeight < 8) rpHeight = 8;

    //grnPixels was read in the OpenGL rendering routine.

    //10 % min is required for overlap, otherwise it never would be on.
    int pixLimit = (int)((double)(vehicle->rpWidth * rpHeight)/(double)(vehicle->numOfSections*1.5));

    //is applied area coming up?
    int totalPixs = 0;
    if (vehicle->isSuperSectionAllowedOn)
    {
        //look for anything applied coming up
        for (int a = 0; a < (vehicle->rpWidth * rpHeight); a++)
        {
            if (grnPixels[a] != 0 )
            {
                if (totalPixs++ > pixLimit)
                {
                    vehicle->isSuperSectionAllowedOn = false;
                    break;
                }

                //check for a boundary line
                if (grnPixels[a] > 200)
                {
                    vehicle->isSuperSectionAllowedOn = false;
                    break;
                }
            }
        }
    }


    // If ALL sections are required on, No buttons are off, within boundary, turn super section on, normal sections off
    if (vehicle->isSuperSectionAllowedOn)
    {
        for (int j = 0; j < vehicle->numOfSections; j++)
        {
            if (section[j].isSectionOn)
            {
                section[j].sectionOffRequest = true;
                section[j].sectionOnRequest = false;
                section[j].sectionOffTimer = 0;
                section[j].sectionOnTimer = 0;
            }
        }

        //turn on super section
        section[vehicle->numOfSections].sectionOnRequest = true;
        section[vehicle->numOfSections].sectionOffRequest = false;
    }

    /* Below is priority based. The last if statement is the one that is
        * applied and takes the highest priority. Digital input controls
        * have the highest priority and overide all buttons except
        * the manual button which exits the loop and just turns sections on....
        * Because isn't that what manual means! */

    //turn on indivdual sections as super section turn off
    else
    {
        //Read the pixels ahead of tool a normal section at a time. Each section can have its own lookahead manipulated.

        //moved variable declarations here because compiler complained about how the goto
        //statements bypassed the initializers.
        int start, end, skip;
        int tagged;

        for (int j = 0; j < vehicle->numOfSections; j++)
        {
            //is section going backwards?
            if (section[j].sectionLookAhead > 0)
            {
                //If any nowhere applied, send OnRequest, if its all green send an offRequest
                section[j].isSectionRequiredOn = false;

                if (boundary->isSet)
                {

                    start = 0, end = 0, skip = 0;
                    start = section[j].rpSectionPosition - section[0].rpSectionPosition;
                    end = section[j].rpSectionWidth - 1 + start;
                    if (end > vehicle->rpWidth - 1) end = vehicle->rpWidth - 1;
                    skip = vehicle->rpWidth - (end - start);


                    tagged = 0;
                    for (int h = 0; h < (int)section[j].sectionLookAhead; h++)
                    {
                        for (int a = start; a < end; a++)
                        {
                            if (grnPixels[a] == 0)
                            {
                                if (tagged++ > vehicle->toolMinUnappliedPixels)
                                {
                                    section[j].isSectionRequiredOn = true;
                                    goto GetMeOutaHere;
                                }
                            }
                        }

                        start += vehicle->rpWidth;
                        end += vehicle->rpWidth;
                    }

                    //minimum apllied conditions met
GetMeOutaHere:

                    start = 0; end = 0; skip = 0;
                    start = section[j].rpSectionPosition - section[0].rpSectionPosition;
                    end = section[j].rpSectionWidth - 1 + start;
                    if (end > vehicle->rpWidth - 1) end = vehicle->rpWidth - 1;
                    skip = vehicle->rpWidth - (end - start);

                    //looking for boundary line color, bright green
                    for (int h = 0; h < (int)section[j].sectionLookAhead; h++)
                    {
                        for (int a = start; a < end; a++)
                        {
                            if (grnPixels[a] > 240) //&& )
                            {
                                section[j].isSectionRequiredOn = false;
                                section[j].sectionOffRequest = true;
                                section[j].sectionOnRequest = false;
                                section[j].sectionOffTimer = 0;
                                section[j].sectionOnTimer = 0;

                                goto GetMeOutaHere1;
                            }
                        }

                        start += vehicle->rpWidth;
                        end += vehicle->rpWidth;
                    }

                    GetMeOutaHere1:

                    //if out of boundary, turn it off
                    if (!section[j].isInsideBoundary)
                    {
                        section[j].isSectionRequiredOn = false;
                        section[j].sectionOffRequest = true;
                        section[j].sectionOnRequest = false;
                        section[j].sectionOffTimer = 0;
                        section[j].sectionOnTimer = 0;
                    }
                }

                //no boundary set so ignore
                else
                {
                    section[j].isSectionRequiredOn = false;

                    int start = 0, end = 0, skip = 0;
                    start = section[j].rpSectionPosition - section[0].rpSectionPosition;
                    end = section[j].rpSectionWidth - 1 + start;
                    if (end > vehicle->rpWidth - 1) end = vehicle->rpWidth - 1;
                    skip = vehicle->rpWidth - (end - start);


                    int tagged = 0;
                    for (int h = 0; h < (int)section[j].sectionLookAhead; h++)
                    {
                        for (int a = start; a < end; a++)
                        {
                            if (grnPixels[a] == 0)
                            {
                                if (tagged++ > vehicle->toolMinUnappliedPixels)
                                {
                                    section[j].isSectionRequiredOn = true;
                                    goto GetMeOutaHere2;
                                }
                            }
                        }

                        start += vehicle->rpWidth;
                        end += vehicle->rpWidth;
                    }

                    //minimum apllied conditions met
                GetMeOutaHere2:
                    start = 0;
                }
            }

            //if section going backwards turn it off
            else section[j].isSectionRequiredOn = false;

        }

        //if the superSection is on, turn it off
        if (section[vehicle->numOfSections].isSectionOn)
        {
            section[vehicle->numOfSections].sectionOffRequest = true;
            section[vehicle->numOfSections].sectionOnRequest = false;
            section[vehicle->numOfSections].sectionOffTimer = 0;
            section[vehicle->numOfSections].sectionOnTimer = 0;
        }

        //if Master Auto is on
        for (int j = 0; j < vehicle->numOfSections; j++)
        {
            if (section[j].isSectionRequiredOn && section[j].isAllowedOn)
            {
                //global request to turn on section
                section[j].sectionOnRequest = true;
                section[j].sectionOffRequest = false;
            }

            else if (!section[j].isSectionRequiredOn)
            {
                //global request to turn off section
                section[j].sectionOffRequest = true;
                section[j].sectionOnRequest = false;
            }

            // Manual on, force the section On and exit loop so digital is also overidden
            if (section[j].manBtnState == btnStates::On)
            {
                section[j].sectionOnRequest = true;
                section[j].sectionOffRequest = false;
                continue;
            }

            if (section[j].manBtnState == btnStates::Off)
            {
                section[j].sectionOnRequest = false;
                section[j].sectionOffRequest = true;
            }

            //if going too slow turn off sections
            if (pn->speed < vehicle->slowSpeedCutoff)
            {
                section[j].sectionOnRequest = false;
                section[j].sectionOffRequest = true;
            }

            //digital input Master control (WorkSwitch)
            if (isJobStarted && mc.isWorkSwitchEnabled)
            {
                //check condition of work switch
                if (mc.isWorkSwitchActiveLow)
                {
                    if (mc.workSwitchValue == 0)
                        { section[j].sectionOnRequest = true; section[j].sectionOffRequest = false; }
                    else { section[j].sectionOnRequest = false; section[j].sectionOffRequest = true; }
                }
                else
                {
                    if (mc.workSwitchValue == 1)
                        { section[j].sectionOnRequest = true; section[j].sectionOffRequest = false; }
                    else { section[j].sectionOnRequest = false; section[j].sectionOffRequest = true; }
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
    sectionControlOutToPort();

    //System.Threading.Thread.Sleep(400);
    //stop the timer and calc how long it took to do calcs and draw
    frameTime = (double)swFrame.elapsed() / 1000; //QElapsedTimer is in milliseconds

    //if a minute has elapsed save the field in case of crash and to be able to resume
    if (saveCounter > 180)       //3 counts per second X 60 seconds = 180 counts per minute.
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
    //if (pn->speed > 0.2)
    {
        for (int j = 0; j < vehicle->numOfSections+1; j++)
        {
            //Turn ON
            //if requested to be on, set the timer to Max 10 (1 seconds) = 10 frames per second
            if (section[j].sectionOnRequest && !section[j].sectionOnOffCycle)
            {
                section[j].sectionOnTimer = (int)(pn->speed * vehicle->toolLookAhead)+1;
                if (section[j].sectionOnTimer > fixUpdateHz+3) section[j].sectionOnTimer = fixUpdateHz+3;
                section[j].sectionOnOffCycle = true;
            }

            //reset the ON request
            section[j].sectionOnRequest = false;

            //decrement the timer if not zero
            if (section[j].sectionOnTimer > 0)
            {
                //turn the section ON if not and decrement timer
                section[j].sectionOnTimer--;
                if (!section[j].isSectionOn) section[j].turnSectionOn();

                //keep resetting the section OFF timer while the ON is active
                section[j].sectionOffTimer = (int)(fixUpdateHz * vehicle->toolTurnOffDelay);
            }

            if (!section[j].sectionOffRequest) section[j].sectionOffTimer = (int)(fixUpdateHz * vehicle->toolTurnOffDelay);

            //decrement the off timer
            if (section[j].sectionOffTimer > 0) section[j].sectionOffTimer--;

            //Turn OFF
            //if Off section timer is zero, turn off the section
            if (section[j].sectionOffTimer == 0 && section[j].sectionOnTimer == 0 && section[j].sectionOffRequest)
            {
                if (section[j].isSectionOn) section[j].turnSectionOff();
                section[j].sectionOnOffCycle = false;
                section[j].sectionOffRequest = false;
            }
        }
    }
}

void FormGPS::tmrWatchdog_timeout()
{
    //tmrWatchdog->stop();
    scanForNMEA();
    //tmrWatchdog->start();
    statusUpdateCounter++;

    /* menu panel taken care of by qml */

   //every third of a second update all status
    if (statusUpdateCounter > 0)
    {
        //reset the counter
        statusUpdateCounter = 0;

        //counter used for saving field in background
        saveCounter++;

        if (isMetric)  //metric or imperial
        {
            //Hectares on the master section soft control and sections
            btnSectionOffAutoOn->setProperty("buttonText",
                     (totalSquareMeters < 999900 ?
                          QString::number(totalSquareMeters * 0.0001,'g',2) :
                          QString::number(totalSquareMeters * 0.0001,'g',1)));
            btnPerimeter->setProperty("buttonText",
                     QString::number(periArea.area * 0.0001,'g', 2));

            //status strip values
            /* TODO:
            stripDistance.Text = Convert.ToString((UInt16)(userDistance)) + " m";
            stripAreaUser.Text = HectaresUser;
            lblSpeed.Text = SpeedKPH;
            stripAreaRate.Text = (Math.Round(vehicle.toolWidth * pn.speed / 10,2)).ToString();
            stripEqWidth.Text = vehiclefileName + (Math.Round(vehicle.toolWidth,2)).ToString() + " m";
            toolStripStatusLabelBoundaryArea.Text = boundary.areaHectare;
            */
        }
        else
        {
            //acres on the master section soft control and sections
            btnSectionOffAutoOn->setProperty("buttonText",
                     (totalSquareMeters < 404645 ?
                          QString::number(totalSquareMeters * 0.00024710499815078974633856493327535, 'g',2) :
                          QString::number(totalSquareMeters * 0.00024710499815078974633856493327535, 'g',1)));
            btnPerimeter->setProperty("buttonText",
                     QString::number(periArea.area * 0.00024710499815078974633856493327535, 'g',2));

            //status strip values
            /* TODO:
            stripDistance.Text = Convert.ToString((UInt16)(userDistance * 3.28084)) + " ft";
            stripAreaUser.Text = AcresUser;
            lblSpeed.Text = SpeedMPH;
            //stripGridZoom.Text = "Grid: " + GridFeet + " '";
            stripAreaRate.Text = ((int)((vehicle.toolWidth * pn.speed / 10) * 2.47)).ToString();
            stripEqWidth.Text = vehiclefileName + (Math.Round(vehicle.toolWidth * glm.m2ft, 2)).ToString() + " ft";
            toolStripStatusLabelBoundaryArea.Text = boundary.areaAcre;
            */
        }

        //lblDelta.Text = guidanceLineHeadingDelta.ToString();

        //non metric or imp fields
        /*TODO
        stripHz.Text = NMEAHz+"Hz "+ (int)(frameTime);
        lblHeading.Text = Heading;
        btnABLine.Text = PassNumber;
        lblSteerAngle.Text = Convert.ToString((double)(guidanceLineSteerAngle) / 10);

        //stripRoll.Text = avgRoll + "\u00B0";
        //stripPitch.Text = avgPitch + "\u00B0";
        //stripAngularVel.Text = avgAngVel.ToString();
        //lblIMUHeading.Text = Math.Round(modcom.imuHeading, 1) + "\u00B0";

        //lblFix.Text = FixQuality;
        //lblAgeDiff.Text = AgeDiff;

        if (Math.Abs(userSquareMetersAlarm) < 0.1) stripAreaUser.BackColor = SystemColors.ControlLightLight;
        else
        {
            stripAreaUser.BackColor = totalUserSquareMeters < userSquareMetersAlarm ? SystemColors.ControlLightLight
                                                                                        : Color.OrangeRed;
        }

        //up in the menu a few pieces of info
        if (isJobStarted)
        {
            lblEasting.Text = "E: "+ Math.Round(pn.easting, 1).ToString();
            lblNorthing.Text = "N: " + Math.Round(pn.northing, 1).ToString();
        }
        else
        {
            lblEasting.Text = "E: " + ((int)pn.actualEasting).ToString();
            lblNorthing.Text = "N: " + ((int)pn.actualNorthing).ToString();
        }

        lblZone.Text = pn.zone.ToString();

        //grab the Valid sentence
        //NMEASentence = recvSentenceSettings;// pn.currentNMEA_GGASentence + pn.currentNMEA_RMCSentence;

        tboxSentence.Text = recvSentenceSettings;
        //update the online indicator
        if (recvCounter > 50)
        {
            stripOnlineGPS.Value = 1;
            lblEasting.Text = "-";
            lblNorthing.Text ="No GPS";
            lblZone.Text = "-";
            tboxSentence.Text = "** No Sentence Data **";
        }
        else  stripOnlineGPS.Value = 100;
        */
    }
    //wait till timer fires again.
}
