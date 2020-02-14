#include "formgps.h"
#include "ui_formgps.h"
#include "cworldgrid.h"
#include "csection.h"
#include "ccontour.h"
#include "cboundary.h"
#include "cvehicle.h"
#include "cyouturn.h"
#include "crate.h"
#include "aogsettings.h"
#include <QColor>
#include <QRgb>
#include "qmlutil.h"
#include "glm.h"
#include "toplinedisplay.h"
#include <QLocale>
#include <QLabel>

FormGPS::FormGPS(QWidget *parent) :
    QMainWindow(parent),timerFor200ms(0),
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
    worldGrid = new CWorldGrid();
    pn = new CNMEA(this); //can make this static now
    //section = new CSection[MAXSECTIONS];
    //for (int i = 0; i < MAXSECTIONS; i++)
    //    section[i].set_mainform(this);
    sim = new CSim();

    vehicle = new CVehicle();
    boundary = new CBoundary();
    yt = new CYouTurn();
    ABLine = new CABLine(vehicle,yt);
    ct = new CContour(vehicle);
    rc = new CRate();

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


    //TODO: put section widths in settings file
    /*
    vehicle->section[0].positionLeft = -18.288;
    vehicle->section[0].positionRight = -9.144;
    vehicle->section[1].positionLeft = -9.144;
    vehicle->section[1].positionRight = 0.0;
    vehicle->section[2].positionLeft = 0.0;
    vehicle->section[2].positionRight = 9.144;
    vehicle->section[3].positionLeft = 9.144;
    vehicle->section[3].positionRight = 18.288;
    */
    vehicle->section[0].positionLeft = -8;
    vehicle->section[0].positionRight = -4;
    vehicle->section[1].positionLeft = -4;
    vehicle->section[1].positionRight = 0.0;
    vehicle->section[2].positionLeft = 0.0;
    vehicle->section[2].positionRight = 4;
    vehicle->section[3].positionLeft = 4;
    vehicle->section[3].positionRight = 8;
    vehicle->numOfSections = 4; //0-3
    vehicle->numSuperSection = 5;
    vehicle->toolTrailingHitchLength = -10; //30 foot hitch to see following action better

    vehicle->sectionCalcWidths();
    //turn on the right number of section buttons.
    //we don't need to do this on resize, but we will need
    //to call it when settings change.
    lineUpManualBtns();

    //hard wire this on for testing
    isJobStarted = true;
    vehicle->section[0].isAllowedOn = true;
    vehicle->section[1].isAllowedOn = true;
    vehicle->section[2].isAllowedOn = true;
    vehicle->section[3].isAllowedOn = true;

    if (isUDPServerOn) startUDPServer();

    connect(sim, SIGNAL(new_position(QByteArray)), this, SLOT(on_csim_new_position(QByteArray)));

}

FormGPS::~FormGPS()
{
    /* clean up our dynamically-allocated
     * objects.
     */
    delete ui;
    delete worldGrid;
    delete pn;
    delete ABLine;
    delete ct;
    delete vehicle;
    delete yt;
    delete rc;

}

//This used to be part of openGLControlBack_Draw in the C# code, but
//because openGL rendering can potentially be in another thread, it's
//broken out here, and runs when the QOpenGLWidget that does the section
//lookahead rendering has finished a frame.  So the lookaheadPixels array has
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
        if (vehicle->section[j].sectionLookAhead > rpHeight) rpHeight = vehicle->section[j].sectionLookAhead;
        if (vehicle->section[j].manBtnState == btnStates::Off) vehicle->isSuperSectionAllowedOn = false;
        if (!vehicle->section[j].isInsideBoundary) vehicle->isSuperSectionAllowedOn = false;

        //check if any sections going backwards
        if (vehicle->section[j].sectionLookAhead < 0) vehicle->isSuperSectionAllowedOn = false;
    }

    //if only one section, or going slow no need for super section
    if (vehicle->numOfSections == 1 || pn->speed < vehicle->slowSpeedCutoff)
            vehicle->isSuperSectionAllowedOn = false;

    //clamp the height after looking way ahead, this is for switching off super section only
    rpHeight = fabs(rpHeight) * 2.0;
    if (rpHeight > 195) rpHeight = 195;
    if (rpHeight < 8) rpHeight = 8;
    //qDebug() << rpHeight;

    //lookaheadPixels was read in the OpenGL rendering routine.

    //10 % min is required for overlap, otherwise it never would be on.
    int pixLimit = (int)((double)(vehicle->rpWidth * rpHeight)/(double)(vehicle->numOfSections*1.5));

    //is applied area coming up?
    int totalPixs = 0;
    if (vehicle->isSuperSectionAllowedOn)
    {
        //look for anything applied coming up
        for (int a = 0; a < (vehicle->rpWidth * rpHeight); a++)
        {
            if (lookaheadPixels[a].green != 0) // && lookaheadPixels[a].red == 0 and lookaheadPixels[a].blue == 0)
            {
                if (lookaheadPixels[a].red != 0 || lookaheadPixels[a].blue != 0) {
                    qDebug() << "position " << a << "showed spurious pixel" << int(lookaheadPixels[a].green);
                }
                if (totalPixs++ > pixLimit)
                {
                    vehicle->isSuperSectionAllowedOn = false;
                    break;
                }

                //check for a boundary line
                if (lookaheadPixels[a].green > 200)
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
            if (vehicle->section[j].isSectionOn)
            {
                vehicle->section[j].sectionOffRequest = true;
                vehicle->section[j].sectionOnRequest = false;
                vehicle->section[j].sectionOffTimer = 0;
                vehicle->section[j].sectionOnTimer = 0;
            }
        }

        //turn on super section
        vehicle->section[vehicle->numOfSections].sectionOnRequest = true;
        vehicle->section[vehicle->numOfSections].sectionOffRequest = false;
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

        for (int j = 0; j < vehicle->numOfSections; j++)
        {
            //is section going backwards?
            if (vehicle->section[j].sectionLookAhead > 0)
            {
                //If any nowhere applied, send OnRequest, if its all green send an offRequest
                vehicle->section[j].isSectionRequiredOn = false;

                if (boundary->isSet)
                {

                    start = 0, end = 0; //, skip = 0;
                    start = vehicle->section[j].rpSectionPosition - vehicle->section[0].rpSectionPosition;
                    end = vehicle->section[j].rpSectionWidth - 1 + start;
                    if (end > vehicle->rpWidth - 1) end = vehicle->rpWidth - 1;
                    //skip = vehicle->rpWidth - (end - start);


                    tagged = 0;
                    for (int h = 0; h < (int)vehicle->section[j].sectionLookAhead; h++)
                    {
                        for (int a = start; a < end; a++)
                        {
                            if (lookaheadPixels[a].green == 0)
                            {
                                if (tagged++ > vehicle->toolMinUnappliedPixels)
                                {
                                    vehicle->section[j].isSectionRequiredOn = true;
                                    goto GetMeOutaHere;
                                }
                            }
                        }

                        start += vehicle->rpWidth;
                        end += vehicle->rpWidth;
                    }

                    //minimum apllied conditions met
GetMeOutaHere:

                    start = 0; end = 0; //skip = 0;
                    start = vehicle->section[j].rpSectionPosition - vehicle->section[0].rpSectionPosition;
                    end = vehicle->section[j].rpSectionWidth - 1 + start;
                    if (end > vehicle->rpWidth - 1) end = vehicle->rpWidth - 1;
                    //skip = vehicle->rpWidth - (end - start);

                    //looking for boundary line color, bright green
                    for (int h = 0; h < (int)vehicle->section[j].sectionLookAhead; h++)
                    {
                        for (int a = start; a < end; a++)
                        {
                            if (lookaheadPixels[a].green > 240) //&& )
                            {
                                vehicle->section[j].isSectionRequiredOn = false;
                                vehicle->section[j].sectionOffRequest = true;
                                vehicle->section[j].sectionOnRequest = false;
                                vehicle->section[j].sectionOffTimer = 0;
                                vehicle->section[j].sectionOnTimer = 0;

                                goto GetMeOutaHere1;
                            }
                        }

                        start += vehicle->rpWidth;
                        end += vehicle->rpWidth;
                    }

                    GetMeOutaHere1:

                    //if out of boundary, turn it off
                    if (!vehicle->section[j].isInsideBoundary)
                    {
                        vehicle->section[j].isSectionRequiredOn = false;
                        vehicle->section[j].sectionOffRequest = true;
                        vehicle->section[j].sectionOnRequest = false;
                        vehicle->section[j].sectionOffTimer = 0;
                        vehicle->section[j].sectionOnTimer = 0;
                    }
                }

                //no boundary set so ignore
                else
                {
                    vehicle->section[j].isSectionRequiredOn = false;

                    int start = 0, end = 0; //, skip = 0;
                    start = vehicle->section[j].rpSectionPosition - vehicle->section[0].rpSectionPosition;
                    end = vehicle->section[j].rpSectionWidth - 1 + start;
                    if (end > vehicle->rpWidth - 1) end = vehicle->rpWidth - 1;
                    //skip = vehicle->rpWidth - (end - start);


                    int tagged = 0;
                    for (int h = 0; h < (int)vehicle->section[j].sectionLookAhead; h++)
                    {
                        for (int a = start; a < end; a++)
                        {
                            if (lookaheadPixels[a].green == 0)
                            {
                                if (tagged++ > vehicle->toolMinUnappliedPixels)
                                {
                                    vehicle->section[j].isSectionRequiredOn = true;
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
            else vehicle->section[j].isSectionRequiredOn = false;

        }

        //if the superSection is on, turn it off
        if (vehicle->section[vehicle->numOfSections].isSectionOn)
        {
            vehicle->section[vehicle->numOfSections].sectionOffRequest = true;
            vehicle->section[vehicle->numOfSections].sectionOnRequest = false;
            vehicle->section[vehicle->numOfSections].sectionOffTimer = 0;
            vehicle->section[vehicle->numOfSections].sectionOnTimer = 0;
        }

        //if Master Auto is on
        for (int j = 0; j < vehicle->numOfSections; j++)
        {
            if (vehicle->section[j].isSectionRequiredOn && vehicle->section[j].isAllowedOn)
            {
                //global request to turn on section
                vehicle->section[j].sectionOnRequest = true;
                vehicle->section[j].sectionOffRequest = false;
            }

            else if (!vehicle->section[j].isSectionRequiredOn)
            {
                //qDebug() << "requesting off for section " << j;
                //global request to turn off section
                vehicle->section[j].sectionOffRequest = true;
                vehicle->section[j].sectionOnRequest = false;
            }

            // Manual on, force the section On and exit loop so digital is also overidden
            if (vehicle->section[j].manBtnState == btnStates::On)
            {
                vehicle->section[j].sectionOnRequest = true;
                vehicle->section[j].sectionOffRequest = false;
                continue;
            }

            if (vehicle->section[j].manBtnState == btnStates::Off)
            {
                vehicle->section[j].sectionOnRequest = false;
                vehicle->section[j].sectionOffRequest = true;
            }

            //if going too slow turn off sections
            if (pn->speed < vehicle->slowSpeedCutoff)
            {
                //qDebug() << "section speed too slow on "<< j;
                vehicle->section[j].sectionOnRequest = false;
                vehicle->section[j].sectionOffRequest = true;
            }

            //digital input Master control (WorkSwitch)
            if (isJobStarted && mc.isWorkSwitchEnabled)
            {
                //check condition of work switch
                if (mc.isWorkSwitchActiveLow)
                {
                    if (mc.workSwitchValue == 0)
                        { vehicle->section[j].sectionOnRequest = true; vehicle->section[j].sectionOffRequest = false; }
                    else { vehicle->section[j].sectionOnRequest = false; vehicle->section[j].sectionOffRequest = true; }
                }
                else
                {
                    if (mc.workSwitchValue == 1)
                        { vehicle->section[j].sectionOnRequest = true; vehicle->section[j].sectionOffRequest = false; }
                    else { vehicle->section[j].sectionOnRequest = false; vehicle->section[j].sectionOffRequest = true; }
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

void FormGPS::on_csim_new_position(QByteArray new_nmea_data)
{
    pn->rawBuffer.append(new_nmea_data);
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
            if (vehicle->section[j].sectionOnRequest && !vehicle->section[j].sectionOnOffCycle)
            {
                vehicle->section[j].sectionOnTimer = (int)(pn->speed * vehicle->toolLookAhead)+1;
                if (vehicle->section[j].sectionOnTimer > fixUpdateHz+3) vehicle->section[j].sectionOnTimer = fixUpdateHz+3;
                vehicle->section[j].sectionOnOffCycle = true;
            }

            //reset the ON request
            vehicle->section[j].sectionOnRequest = false;

            //decrement the timer if not zero
            if (vehicle->section[j].sectionOnTimer > 0)
            {
                //turn the section ON if not and decrement timer
                vehicle->section[j].sectionOnTimer--;
                if (!vehicle->section[j].isSectionOn) vehicle->section[j].turnSectionOn();

                //keep resetting the section OFF timer while the ON is active
                vehicle->section[j].sectionOffTimer = (int)(fixUpdateHz * vehicle->toolTurnOffDelay);
            }

            if (!vehicle->section[j].sectionOffRequest) vehicle->section[j].sectionOffTimer = (int)(fixUpdateHz * vehicle->toolTurnOffDelay);

            //decrement the off timer
            if (vehicle->section[j].sectionOffTimer > 0) vehicle->section[j].sectionOffTimer--;

            //Turn OFF
            //if Off section timer is zero, turn off the section
            if (vehicle->section[j].sectionOffTimer == 0 && vehicle->section[j].sectionOnTimer == 0 && vehicle->section[j].sectionOffRequest)
            {
                if (vehicle->section[j].isSectionOn) vehicle->section[j].turnSectionOff();
                vehicle->section[j].sectionOnOffCycle = false;
                vehicle->section[j].sectionOffRequest = false;
            }
        }
    }
}

void FormGPS::tmrWatchdog_timeout()
{


    // Avoiding lots timer that might be affects to target device
    timerFor200ms++;

    if(timerFor200ms == 4)
    {
        sim->DoSimTick( 5 * 0.01);
        timerFor200ms = 0;

    }

    scanForNMEA();
    statusUpdateCounter++;

    /* menu panel taken care of by qml */

   //every third of a second update all status
    if (statusUpdateCounter > 0)
    {
        //reset the counter
        statusUpdateCounter = 0;

        //counter used for saving field in background
        saveCounter++;

        double spd = 0;
        for (int c = 0; c < 10; c++)
            spd += vehicle->avgSpeed[c];

        //convert to kph
        spd *= 0.1;

        if (isMetric)  //metric or imperial
        {
            //Hectares on the master section soft control and sections
            btnSectionOffAutoOn->setProperty("buttonText",
                     (vehicle->totalSquareMeters < 999900 ?
                          locale.toString(vehicle->totalSquareMeters * 0.0001,'f',2) :
                          locale.toString(vehicle->totalSquareMeters * 0.0001,'f',1))
                                             + " " + tr("Ha"));
            btnPerimeter->setProperty("buttonText",
                     locale.toString(periArea.area * 0.0001,'f', 2) + " " + tr("Ha"));

            //status strip values

            /* TODO:
            stripDistance.Text = Convert.ToString((UInt16)(userDistance)) + " m";
            stripAreaUser.Text = HectaresUser;
            lblSpeed.Text = SpeedKPH;
            stripAreaRate.Text = (Math.Round(vehicle.toolWidth * pn.speed / 10,2)).ToString();
            stripEqWidth.Text = vehiclefileName + (Math.Round(vehicle.toolWidth,2)).ToString() + " m";
            toolStripStatusLabelBoundaryArea.Text = boundary.areaHectare;
            */
            qmlItem(qml_root,"stripBoundaryArea")->setProperty("text",tr("Bounded area:")+ " " +locale.toString(boundary->areaHectare,'f',1) + " " + tr("Ha"));
            qmlItem(qml_root,"stripAreaUser")->setProperty("text", locale.toString(vehicle->totalUserSquareMeters * 0.0001,'f',1) + " " + tr("Ha"));
            qmlItem(qml_root,"stripEqWidth")->setProperty("text", locale.toString(vehicle->toolWidth,'f',2) + " " + tr("M"));
            qmlItem(qml_root,"stripDistance")->setProperty("text", locale.toString(userDistance,'f',0)+" "+tr("M"));
            qmlItem(qml_root,"stripAreaRate")->setProperty("text", locale.toString(vehicle->toolWidth * spd / 10,'f',1) + " " + tr("Ha/hr"));
            tlDisp->lblSpeed->setText(locale.toString(spd,'f',1) + " "+tr("KPH"));

        }
        else
        {
            //acres on the master section soft control and sections
            btnSectionOffAutoOn->setProperty("buttonText",
                     (vehicle->totalSquareMeters < 404645 ?
                          QString::number(vehicle->totalSquareMeters * 0.00024710499815078974633856493327535, 'f',2) :
                          QString::number(vehicle->totalSquareMeters * 0.00024710499815078974633856493327535, 'f',1))
                                             + " " + tr("Ac"));
            btnPerimeter->setProperty("buttonText",
                     QString::number(periArea.area * 0.00024710499815078974633856493327535, 'f',2) +
                                      " " + tr("Ac"));

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
            qmlItem(qml_root,"stripBoundaryArea")->setProperty("text",tr("Bounded area:")+ " " +locale.toString(boundary->areaAcre,'f',1) + " " + tr("Ac"));
            qmlItem(qml_root,"stripAreaUser")->setProperty("text", locale.toString(vehicle->totalUserSquareMeters * 0.00024710499815078974633856493327535,'f',1) + " " + tr("Ac"));
            qmlItem(qml_root,"stripEqWidth")->setProperty("text", locale.toString(vehicle->toolWidth * m2ft,'f',1) + " " + tr("ft"));
            qmlItem(qml_root,"stripDistance")->setProperty("text", locale.toString(userDistance * 3.28084,'f',0)+" "+tr("ft"));
            qmlItem(qml_root,"stripAreaRate")->setProperty("text", locale.toString(vehicle->toolWidth * spd / 10 * 2.47,'f',1) + " " + tr("Ac/hr"));
            tlDisp->lblSpeed->setText(locale.toString(spd * 0.621371,'f',1) + " "+tr("MPH"));
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
        */

        qmlItem(qml_root,"stripHz")->setProperty("text",locale.toString(fixUpdateHz) + " " + tr("Hz"));
        tlDisp->lblHeading->setText("<small>Hdg:</small>"+locale.toString(toDegrees(vehicle->fixHeading),'f',1) + QChar(0x00b0));
        tlDisp->lblSteerAngle->setText("<small>Steer:</small>" + locale.toString((double)(vehicle->guidanceLineSteerAngle) / 10,'f',1) + QChar(0x00b0));

        /*
         * TODO

        if (Math.Abs(userSquareMetersAlarm) < 0.1) stripAreaUser.BackColor = SystemColors.ControlLightLight;
        else
        {
            stripAreaUser.BackColor = totalUserSquareMeters < userSquareMetersAlarm ? SystemColors.ControlLightLight
                                                                                        : Color.OrangeRed;
        }
        */

        //up in the menu a few pieces of info
        if (isJobStarted)
        {
            tlDisp->lblEasting->setText(tr("<small>UTM E:</small>")+ locale.toString(pn->easting,'f', 1));
            tlDisp->lblNorthing->setText(tr("<small>N:</small>") + locale.toString(pn->northing, 'f', 1));
        }
        else
        {
            tlDisp->lblEasting->setText(tr("<small>UTM E:</small>")+ locale.toString(pn->actualEasting,'f', 1));
            tlDisp->lblNorthing->setText(tr("<small>N:</small>") + locale.toString(pn->actualNorthing, 'f', 1));
        }

        tlDisp->lblZone->setText("<small>Zn:</small>"+locale.toString(pn->zone));

        //grab the Valid sentence
        //NMEASentence = recvSentenceSettings;// pn.currentNMEA_GGASentence + pn.currentNMEA_RMCSentence;
        /* TODO
        tboxSentence.Text = recvSentenceSettings;
        */
        //update the online indicator
        if (recvCounter > 50)
        {
            //stripOnlineGPS.Value = 1;
            //TODO turn off port light
            tlDisp->lblEasting->setText("-");
            tlDisp->lblNorthing->setText(tr("No GPS"));
            tlDisp->lblZone->setText("-");
            //tboxSentence.Text = "** No Sentence Data **";
        }
        //else  stripOnlineGPS.Value = 100;
        // turn on port light

        //LightBar if AB Line is set and turned on or contour
        if (isLightbarOn)
        {
            if (ct->isContourBtnOn)
            {
                QString dist;
                //turn on distance widget
                tlDisp->txtDistanceOffABLine->show();
                btnAutoSteer->setProperty("enabled",true);
                //lblDelta.Visible = true;
                if (ct->distanceFromCurrentLine == 32000) ct->distanceFromCurrentLine = 0;

                if ((ct->distanceFromCurrentLine) < 0.0) {
                    if (isMetric) dist = locale.toString((int)fabs(ct->distanceFromCurrentLine * 0.1)) + " " + QChar(0x2192);
                    else dist = locale.toString((int)fabs(ct->distanceFromCurrentLine / 2.54 * 0.1)) + " " + QChar(0x2192);
                    tlDisp->txtDistanceOffABLine->setStyleSheet("QLabel { color: green; }");
                    tlDisp->txtDistanceOffABLine->setText(dist);
                } else {
                    if (isMetric) dist = QString("") + QChar(0x2190) + " " + locale.toString((int)fabs(ct->distanceFromCurrentLine * 0.1));
                    else dist = QString("") + QChar(0x2190)+ " " + locale.toString((int)fabs(ct->distanceFromCurrentLine / 2.54 * 0.1));
                    tlDisp->txtDistanceOffABLine->setStyleSheet("QLabel { color: red; }");
                    tlDisp->txtDistanceOffABLine->setText(dist);
                }

                //qDebug() << vehicle->guidanceLineDistanceOff;
                if (vehicle->guidanceLineDistanceOff == 32020 || vehicle->guidanceLineDistanceOff == 32000) {
                    btnAutoSteer->setProperty("buttonText", QChar(0x2715));
                } else {
                    btnAutoSteer->setProperty("buttonText", QChar(0x2713));
                }
            } else if (ABLine->isABLineSet || ABLine->isABLineBeingSet) {
                QString dist;

                tlDisp->txtDistanceOffABLine->show();
                btnAutoSteer->setProperty("enabled",true);
                if ((ABLine->distanceFromCurrentLine) < 0.0) {
                    // --->
                    if (isMetric) dist = locale.toString((int)fabs(ABLine->distanceFromCurrentLine * 0.1)) + QChar(0x21D2);
                    else dist = locale.toString((int)fabs(ABLine->distanceFromCurrentLine / 2.54 * 0.1)) + QChar(0x21D2);
                    tlDisp->txtDistanceOffABLine->setStyleSheet("QLabel { color: green; }");
                    tlDisp->txtDistanceOffABLine->setText(dist);
                } else {
                    // <----
                    if (isMetric) dist = QChar(0x21D0) + locale.toString((int)fabs(ABLine->distanceFromCurrentLine * 0.1));
                    else dist = QChar(0x21D0) + locale.toString((int)fabs(ABLine->distanceFromCurrentLine / 2.54 * 0.1));
                    tlDisp->txtDistanceOffABLine->setStyleSheet("QLabel { color: red; }");
                    tlDisp->txtDistanceOffABLine->setText(dist);
                }

                if (vehicle->guidanceLineDistanceOff == 32020 || vehicle->guidanceLineDistanceOff == 32000) {
                    btnAutoSteer->setProperty("buttonText", QChar(0x2715));
                    //btnAutoSteer->setProperty("enabled",false);
                } else {
                    btnAutoSteer->setProperty("buttonText", QChar(0x2713));
                    //btnAutoSteer->setProperty("enabled",true);
                }
            }

            //AB line is not set so turn off numbers
            if (!ABLine->isABLineSet && !ABLine->isABLineBeingSet && !ct->isContourBtnOn)
            {
                tlDisp->txtDistanceOffABLine->hide();
                btnAutoSteer->setProperty("buttonText","-");
                //btnAutoSteer->setProperty("enabled",false);
            }
        } else {
            tlDisp->txtDistanceOffABLine->hide();
            btnAutoSteer->setProperty("buttonText","-");
            //btnAutoSteer->setProperty("enabled",false);
        }

        if (flagPts.size()) {
            btnDeleteAllFlags->setProperty("enabled",true);
            if(flagNumberPicked) {
                btnDeleteFlag->setProperty("enabled",true);
            } else {
                btnDeleteFlag->setProperty("enabled",false);
            }
        } else {
            btnDeleteAllFlags->setProperty("enabled",false);
            btnDeleteFlag->setProperty("enabled",false);
        }

        ui->menuBar->adjustSize();
    }
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
        if (b < vehicle->numOfSections) {
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

    switch(vehicle->section[sectNumber].manBtnState) {
    case btnStates::Off:
        vehicle->section[sectNumber].manBtnState = btnStates::Auto;
        button->setProperty("state","auto");
        break;
    case btnStates::Auto:
        vehicle->section[sectNumber].manBtnState = btnStates::On;
        button->setProperty("state","on");
        break;
    case btnStates::On:
        vehicle->section[sectNumber].manBtnState = btnStates::Off;
        button->setProperty("state","off");
        break;
    }
}

QString FormGPS::speedKPH() {
    double spd = 0;
    for (int c = 0; c < 10; c++)
        spd += vehicle->avgSpeed[c];

    //convert to kph
    spd *= 0.1;

    return locale.toString(spd,'f',1);
}

QString FormGPS::speedMPH() {
    double spd = 0;
    for (int c = 0; c < 10; c++)
        spd += vehicle->avgSpeed[c];

    //convert to mph
    spd *= 0.0621371;

    return locale.toString(spd,'f',1);
}
