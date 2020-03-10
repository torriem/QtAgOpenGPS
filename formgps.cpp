#include "formgps.h"
#include "aogsettings.h"
#include <QColor>
#include <QRgb>
#include "qmlutil.h"
#include "glm.h"
#include <QLocale>
#include <QLabel>

extern QLabel *grnPixels;



FormGPS::FormGPS(QWidget *parent) :
    QQuickView(qobject_cast<QWindow *>(parent))
{
    USE_SETTINGS;
    setupGui();
    AOGSettings s;

    isUDPServerOn = s.value("port/udp_on", true).toBool();


    /* test data to see if drawing routines are working. */

    isAreaOnRight = s.value("vehicle/isAreaOnRight",false).toBool();

    isGridOn = s.value("display/showGrid",true).toBool();
    isMetric = s.value("display/isMetric", true).toBool();

    //TODO: move all the settings loading/setting of defaults to a separate
    //function.
    minFixStepDist = s.value("position/minFixStep",1.0).toDouble(); //minimum distance between fixes.
    //isAtanCam = s.value("camera/useCalculatedHeading", true).toBool();

    //fieldColor = QColor(s.value("display/fieldColor", "#82781E").toString());
    //sectionColor = QColor(s.value("display/sectionColor", "#32DCC8").toString());
    SETTINGS_SET_DISPLAY_COMPASS(true);
    SETTINGS_SET_DISPLAY_SPEEDO(true);

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
    tool.toolTrailingHitchLength = -4; //30 foot hitch to see following action better
    vehicle.minTurningRadius = 5;
    vehicle.maxSteerAngle = 45;
    tool.sectionCalcWidths();

    CBoundaryLines boundary;
    boundary.bndLine.append(Vec3(-100,0,0));
    boundary.bndLine.append(Vec3( 100,0,0));
    boundary.bndLine.append(Vec3( 100,250,0));
    boundary.bndLine.append(Vec3(-100,250,0));
    boundary.bndLine.append(Vec3(-100,0,0));
    boundary.isSet = true;
    boundary.calculateBoundaryArea();
    boundary.calculateBoundaryHeadings();
    boundary.preCalcBoundaryLines();

    bnd.bndArr.append(boundary);

    gf.geoFenceArr.append(CGeoFenceLines());
    gf.buildGeoFenceLines(bnd, tool.toolWidth, SETTINGS_VEHICLE_GEOFENCEDIST);

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

    ABLine.refPoint1.easting = 0;
    ABLine.refPoint1.easting = 0;
    ABLine.setABLineByHeading(glm::toRadians(5.0f));
    ABLine.isBtnABLineOn = true;
    //isAutoSteerBtnOn = true;

    if (isUDPServerOn) startUDPServer();

    //TODO: connect signals from various classes
    connect(&pn, SIGNAL(setRollX16(int)), &ahrs, SLOT(setRollX16(int)));
    connect(&pn, SIGNAL(setCorrectionHeadingX16(int)), &ahrs,SLOT(setCorrectionHeadingX16(int)));
    connect(&pn, SIGNAL(clearRecvCounter()), this, SLOT(onClearRecvCounter()));
    connect(&pn, SIGNAL(newSpeed(double)), &vehicle, SLOT(onNewSpeed(double)));
    connect(&pn, SIGNAL(headingSource(int)), this, SLOT(onHeadingSource(int)));

    connect(&curve, SIGNAL(doSequence(CYouTurn&)), &seq, SLOT(DoSequenceEvent(CYouTurn&)));

    connect(&ABLine, SIGNAL(doSequence(CYouTurn&)), &seq, SLOT(DoSequenceEvent(CYouTurn&)));
    //connect(&ABLine,SIGNAL(showMessage(int,QString,QString)),...

    //connnect(&ct, SIGNAL(showMessage(int,QString,QString))

    for(int i=0; i < MAXSECTIONS+1 ; i++) {
        //connect sections so they can increment area counters
        connect(&tool.section[i], SIGNAL(addToTotalArea(double)), &fd, SLOT(addToTotalArea(double)));
        connect(&tool.section[i], SIGNAL(addToUserArea(double)), &fd, SLOT(addToUserArea(double)));
    }

    connect(&hd, SIGNAL(moveHydraulics(int)), &mc, SLOT(setHydLift(int)));

    //connect(&mc, SIGNAL(sendOutUSBAutoSteerPort(uchar*,int)),
    //connect(&mc, SIGNAL(sendOutUSBMachinePort(uchar*,int)),

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
    USE_SETTINGS;
    //not using regular Qt Widgets in the main window anymore.  For
    //debugging purposes, this could go in another popup window
    //grnPixelsWindow->setPixmap(QPixmap::fromImage(grnPix.mirrored()));

    bool isHeadlandClose = false, isBoundaryClose = false, isMapping = true;

    //determine farthest ahead lookahead - is the height of the readpixel line
    double rpHeight = 0;

    //assume all sections are on and super can be on, if not set false to turn off.
    tool.isSuperSectionAllowedOn = true;
    double rpOnHeight = 0;
    double rpToolHeight = 0;

    //pick the larger side
    if (vehicle.hydLiftLookAheadDistanceLeft > vehicle.hydLiftLookAheadDistanceRight)
        rpToolHeight = vehicle.hydLiftLookAheadDistanceLeft;
    else rpToolHeight = vehicle.hydLiftLookAheadDistanceRight;

    if (tool.lookAheadDistanceOnPixelsLeft > tool.lookAheadDistanceOnPixelsRight) rpOnHeight = tool.lookAheadDistanceOnPixelsLeft;
    else rpOnHeight = tool.lookAheadDistanceOnPixelsRight;

    //assume all sections are on and super can be on, if not set false to turn off.
    tool.isSuperSectionAllowedOn = true;
    isHeadlandClose = false;
    isBoundaryClose = false;

    //find any off buttons, any outside of boundary, going backwards, and the farthest lookahead
    for (int j = 0; j < tool.numOfSections; j++)
    {
        if (tool.section[j].manBtnState == btnStates::Off) tool.isSuperSectionAllowedOn = false;
        if (!tool.section[j].isInBoundary) tool.isSuperSectionAllowedOn = false;

        //check if any sections going backwards
        if (tool.section[j].speedPixels < 0) tool.isSuperSectionAllowedOn = false;
    }

    //if only one section, or going slow no need for super section
    if (tool.numOfSections == 1 || pn.speed <= SETTINGS_VEHICLE_SLOWSPEEDCUTOFF)
            tool.isSuperSectionAllowedOn = false;

    //clamp the height after looking way ahead, this is for switching off super section only
    rpOnHeight = fabs(rpOnHeight);
    rpToolHeight = fabs(rpToolHeight);

    //10 % min is required for overlap, otherwise it never would be on.
    int pixLimit = (int)((double)(tool.section[0].rpSectionWidth * rpOnHeight) / (double)(5.0));

    if ((rpOnHeight < rpToolHeight && hd.isOn)) rpHeight = rpToolHeight + 2;
    else rpHeight = rpOnHeight + 2;

    if (rpHeight > 240) rpHeight = 240;
    if (rpHeight < 8) rpHeight = 8;
    //qDebug() << rpHeight;

    //is applied area coming up?
    int totalPixs = 0;

    //determine if headland is in read pixel buffer left middle and right.
    int start = 0, end = 0, tagged = 0;

    //slope of the look ahead line
    double mOn = 0, mOff=0;

    if (bnd.bndArr.count() > 0)
    {
        //are there enough pixels in buffer array to warrant turning off supersection
        for (int a = 0; a < (tool.rpWidth * rpOnHeight); a++)
        {
            if (grnPixels[a].green != 0)
            {
                if (tool.isSuperSectionAllowedOn && totalPixs++ > pixLimit)
                {
                    tool.isSuperSectionAllowedOn = false;
                    break;
                }
            }
        }

        //5 pixels in is there a boundary line?
        for (int a = 0; a < (tool.rpWidth * 5); a++)
        {
            if (grnPixels[a].green == 240)
            {
                tool.isSuperSectionAllowedOn = false;
                isBoundaryClose = true;
                break;
            }
        }

        //determine if in or out of headland, do hydraulics if on
        if (hd.isOn)
        {
            //calculate the slope
            double m = (vehicle.hydLiftLookAheadDistanceRight - vehicle.hydLiftLookAheadDistanceLeft) / tool.rpWidth;
            int height = 1;

            for (int pos = 0; pos < tool.rpWidth; pos++)
            {
                height = (int)(vehicle.hydLiftLookAheadDistanceLeft + (m * pos)) - 1;
                for (int a = pos; a < height*tool.rpWidth; a+=tool.rpWidth)
                {
                    if (grnPixels[a].green == 250)
                    {
                        isHeadlandClose = true;
                        goto GetOutTool;
                    }
                }
            }
GetOutTool:

            //is the tool completely in the headland or not
            hd.isToolInHeadland = hd.isToolOuterPointsInHeadland && !isHeadlandClose;

            if (isHeadlandClose || hd.isToolInHeadland) tool.isSuperSectionAllowedOn = false;

            //set hydraulics based on tool in headland or not
            hd.setHydPosition(vehicle, pn.speed);

            ////
            //if (hd.isToolInHeadland) lblInHead.Text = "Headland";
            //else lblInHead.Text = "Work";
            //if (hd.isToolUp) lblIsHdClose.Text = "Up";
            //else lblIsHdClose.Text = "Down";
        }
    }
    else  //supersection check by applied only
    {
        for (int a = 0; a < (tool.rpWidth * rpOnHeight); a++)
        {
            if (grnPixels[a].green != 0)
            {
                if (tool.isSuperSectionAllowedOn && totalPixs++ > pixLimit)
                {
                    tool.isSuperSectionAllowedOn = false;
                    break;
                }
            }
        }
    }

    //if all manual and all on go supersection
    if (manualBtnState == btnStates::On)
    {
        tool.isSuperSectionAllowedOn = true;
        for (int j = 0; j < tool.numOfSections; j++)
        {
            if (tool.section[j].manBtnState == btnStates::Off) tool.isSuperSectionAllowedOn = false;
        }
    }

    // If ALL sections are required on, No buttons are off, within boundary, turn super section on, normal sections off
    if (tool.isSuperSectionAllowedOn)
    {
        for (int j = 0; j < tool.numOfSections; j++)
        {
            if (tool.section[j].isMappingOn)
            {
                tool.section[j].mappingOffRequest = true;
                tool.section[j].mappingOnRequest = false;
                tool.section[j].mappingOffTimer = 0;
                tool.section[j].mappingOnTimer = 0;
            }
            if (tool.section[j].isSectionOn)
            {
                tool.section[j].sectionOffRequest = true;
                tool.section[j].sectionOnRequest = false;
                tool.section[j].sectionOffTimer = 0;
                tool.section[j].sectionOnTimer = 0;
            }
        }

        //turn on super section
        tool.section[tool.numOfSections].mappingOnRequest = true;
        tool.section[tool.numOfSections].mappingOffRequest = false;

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
        //if the superSection is on, turn it off
        if (tool.section[tool.numOfSections].isSectionOn)
        {
            tool.section[tool.numOfSections].sectionOffRequest = true;
            tool.section[tool.numOfSections].sectionOnRequest = false;
            tool.section[tool.numOfSections].sectionOffTimer = 0;
            tool.section[tool.numOfSections].sectionOnTimer = 0;
        }

        //if the superSection is on, turn it off
        if (tool.section[tool.numOfSections].isMappingOn)
        {
            tool.section[tool.numOfSections].mappingOffRequest = true;
            tool.section[tool.numOfSections].mappingOnRequest = false;
            tool.section[tool.numOfSections].mappingOffTimer = 0;
            tool.section[tool.numOfSections].mappingOnTimer = 0;
        }

        //Mapping   ---------------------------------------------------------------------------------------------------
        int endHeight = 1, startHeight =1;

        for (int j = 0; j < tool.numOfSections; j++)
        {
            tool.section[j].isMappingRequiredOn = false;

            //calculate slope
            mOn = (tool.lookAheadDistanceOnPixelsRight - tool.lookAheadDistanceOnPixelsLeft) / tool.rpWidth;

            //start and end point to scan across buffer
            start = tool.section[j].rpSectionPosition - tool.section[0].rpSectionPosition;
            end = tool.section[j].rpSectionWidth - 1 + start;
            tagged = 0;

            for (int pos = start; pos <= end; pos++)
            {
                //block 5 pixels high (50 cm look ahead)
                    endHeight = 5 * tool.rpWidth + pos;

                for (int a = pos; a <= endHeight; a += tool.rpWidth)
                {
                    if (grnPixels[a].green == 0)
                    {
                        tagged++;
                        if (tagged > tool.toolMinUnappliedPixels)
                        {
                            tool.section[j].isMappingRequiredOn = true;
                            goto GetOutMappingOn;
                        }
                    }
                }
            }
GetOutMappingOn:
            start = 0;

            if (bnd.bndArr.count() > 0)
            {
                //if out of boundary, turn it off
                if (!tool.section[j].isInBoundary)
                {
                    tool.section[j].isMappingRequiredOn = false;
                    tool.section[j].mappingOffRequest = true;
                    tool.section[j].mappingOnRequest = false;
                    tool.section[j].mappingOffTimer = 0;
                    tool.section[j].mappingOnTimer = 0;
                }

                else if (tool.section[j].isInHeadlandArea & hd.isOn)
                {
                    // if headland is on and out, turn off
                        tool.section[j].isMappingRequiredOn = false;
                        tool.section[j].mappingOffRequest = true;
                        tool.section[j].mappingOnRequest = false;
                        tool.section[j].mappingOffTimer = 0;
                        tool.section[j].mappingOnTimer = 0;
                }
            }
        }

        ///////////////////////////////////////////   Section control        ssssssssssssssssssssss
        for (int j = 0; j < tool.numOfSections; j++)
        {
            //If any nowhere applied, send OnRequest, if its all green send an offRequest
            //ensure it starts off
            tool.section[j].isSectionRequiredOn = false;

            if (bnd.bndArr.count() > 0)
            {
                //if out of boundary, turn it off
                if (!tool.section[j].isInBoundary)
                {
                    tool.section[j].isSectionRequiredOn = false;
                    tool.section[j].sectionOffRequest = true;
                    tool.section[j].sectionOnRequest = false;
                    tool.section[j].sectionOffTimer = 0;
                    tool.section[j].sectionOnTimer = 0;
                }
                else
                {
                    mOn = (tool.lookAheadDistanceOnPixelsRight - tool.lookAheadDistanceOnPixelsLeft) / tool.rpWidth;
                    start = tool.section[j].rpSectionPosition - tool.section[0].rpSectionPosition;
                    end = tool.section[j].rpSectionWidth - 1 + start;
                    tagged = 0;

                    for (int pos = start; pos <= end; pos++)
                    {
                        if (isBoundaryClose) endHeight = tool.rpWidth + pos;
                        else endHeight = (int)(tool.lookAheadDistanceOnPixelsLeft + (mOn * pos)) * tool.rpWidth + pos;

                        for (int a = pos; a <= endHeight; a += tool.rpWidth)
                        {
                            if (grnPixels[a].green == 0)
                            {
                                tagged++;
                                if (tagged > tool.toolMinUnappliedPixels)
                                {
                                    tool.section[j].isSectionRequiredOn = true;
                                    goto GetOutSectionOn;
                                }
                            }
                        }

                    }
GetOutSectionOn:
                    tagged = 0;

                //only turn off if on
                if (tool.section[j].isSectionRequiredOn == true)
                {
                    tool.section[j].isSectionRequiredOn = false;
                    //calculate the slopes of the lines
                    mOn = (tool.lookAheadDistanceOnPixelsRight - tool.lookAheadDistanceOnPixelsLeft) / tool.rpWidth;
                    mOff = (tool.lookAheadDistanceOffPixelsRight - tool.lookAheadDistanceOffPixelsLeft) / tool.rpWidth;
                    start = tool.section[j].rpSectionPosition - tool.section[0].rpSectionPosition;
                    end = tool.section[j].rpSectionWidth - 1 + start;
                    tagged = 0;

                    for (int pos = start; pos <= end; pos++)
                    {
                        if (isBoundaryClose)
                        {
                            endHeight = tool.rpWidth + pos;
                            startHeight = pos;
                        }
                        else
                        {
                            startHeight = (int)(tool.lookAheadDistanceOffPixelsLeft + (mOff * pos)) * tool.rpWidth + pos;
                            endHeight = (int)(tool.lookAheadDistanceOnPixelsLeft + (mOn * pos)) * tool.rpWidth + pos;
                        }

                        for (int a = startHeight; a <= endHeight; a += tool.rpWidth)
                        {
                            if (grnPixels[a].green == 0)
                            {
                                tagged++;
                                if (tagged > tool.toolMinUnappliedPixels)
                                {
                                    tool.section[j].isSectionRequiredOn = true;
                                    goto GetOutSectionOff;
                                }
                            }
                        }
                    }
GetOutSectionOff:
                    start = 0;
                }

                //is headland coming up
                if (hd.isOn)
                {
                    bool isHeadlandInLookOn = false;

                    //is headline in off to on area
                    mOn = (tool.lookAheadDistanceOnPixelsRight - tool.lookAheadDistanceOnPixelsLeft) / tool.rpWidth;
                    mOff = (tool.lookAheadDistanceOffPixelsRight - tool.lookAheadDistanceOffPixelsLeft) / tool.rpWidth;

                    start = tool.section[j].rpSectionPosition - tool.section[0].rpSectionPosition;

                    end = tool.section[j].rpSectionWidth - 1 + start;
                    tagged = 0;

                    for (int pos = start; pos <= end; pos++)
                    {
                        startHeight = (int)(tool.lookAheadDistanceOffPixelsLeft + (mOff * pos)) * tool.rpWidth + pos;
                        endHeight = (int)(tool.lookAheadDistanceOnPixelsLeft + (mOn * pos)) * tool.rpWidth + pos;

                        for (int a = startHeight; a <= endHeight; a += tool.rpWidth)
                        {
                            if (a < 0)
                                mOn = 0;
                            if (grnPixels[a].green == 250)
                            {
                                //tagged++;
                                //if (tagged > tool.toolMinUnappliedPixels)
                                {
                                    isHeadlandInLookOn = true;
                                    goto GetOutHdOn;
                                }
                            }
                        }
                    }
GetOutHdOn:
                    //    //is headline in base to off area
                    //    tagged = 0;
                    //    mOn = (tool.lookAheadDistanceOffPixelsRight - tool.lookAheadDistanceOffPixelsLeft) / tool.rpWidth;

                    //    start = tool.section[j].rpSectionPosition - tool.section[0].rpSectionPosition;
                    //    end = tool.section[j].rpSectionWidth - 1 + start;

                    //    for (int pos = start; pos <= end; pos++)
                    //    {
                    //        endHeight = (int)(tool.lookAheadDistanceOffPixelsLeft + (mOn * pos)) * tool.rpWidth + pos;

                    //        for (int a = pos; a <= endHeight; a += tool.rpWidth)
                    //        {
                    //            if (grnPixels[a].green == 250)
                    //            {
                    //                //tagged++;
                    //                //if (tagged > tool.toolMinUnappliedPixels)
                    //                {
                    //                    isHeadlandInLookOff = true;
                    //                    goto GetOutHdOff;
                    //                }
                    //            }
                    //        }
                    //    }
                    //GetOutHdOff:

                        //determine if look ahead points are completely in headland
                        hd.whereAreToolLookOnPoints(vehicle, tool);

                        if (tool.section[j].isSectionRequiredOn && tool.section[j].isLookOnInHeadland && !isHeadlandInLookOn)
                        {
                            tool.section[j].isSectionRequiredOn = false;
                            tool.section[j].sectionOffRequest = true;
                            tool.section[j].sectionOnRequest = false;
                        }

                        if (tool.section[j].isSectionRequiredOn && !tool.section[j].isLookOnInHeadland && isHeadlandInLookOn)
                        {
                            tool.section[j].isSectionRequiredOn = true;
                            tool.section[j].sectionOffRequest = false;
                            tool.section[j].sectionOnRequest = true;
                        }
                    }
                }
            }
            else  //Section Control in no boundary field
            {
                tagged = 0;
                mOn = (tool.lookAheadDistanceOnPixelsRight - tool.lookAheadDistanceOnPixelsLeft) / tool.rpWidth;

                start = tool.section[j].rpSectionPosition - tool.section[0].rpSectionPosition;
                end = tool.section[j].rpSectionWidth - 1 + start;

                for (int pos = start; pos <= end; pos++)
                {
                    endHeight = (int)(tool.lookAheadDistanceOnPixelsLeft + (mOn * pos)) * tool.rpWidth + pos;

                    for (int a = pos; a <= endHeight; a += tool.rpWidth)
                    {
                        if (grnPixels[a].green == 0)
                        {
                            tagged++;
                            if (tagged > tool.toolMinUnappliedPixels)
                            {
                                tool.section[j].isSectionRequiredOn = true;
                                goto GetOutSectionOn1;
                            }
                        }
                    }
                }
GetOutSectionOn1:
                //only turn off if on
                if (tool.section[j].isSectionRequiredOn == true)
                {
                    tool.section[j].isSectionRequiredOn = false;
                    //calculate the slopes of the lines
                    mOn = (tool.lookAheadDistanceOnPixelsRight - tool.lookAheadDistanceOnPixelsLeft) / tool.rpWidth;
                    mOff = (tool.lookAheadDistanceOffPixelsRight - tool.lookAheadDistanceOffPixelsLeft) / tool.rpWidth;

                    start = tool.section[j].rpSectionPosition - tool.section[0].rpSectionPosition;

                    end = tool.section[j].rpSectionWidth - 1 + start;
                    tagged = 0;

                    for (int pos = start; pos <= end; pos++)
                    {
                        startHeight = (int)(tool.lookAheadDistanceOffPixelsLeft + (mOff * pos)) * tool.rpWidth + pos;
                        endHeight = (int)(tool.lookAheadDistanceOnPixelsLeft + (mOn * pos)) * tool.rpWidth + pos;

                        for (int a = startHeight; a <= endHeight; a += tool.rpWidth)
                        {
                            if (a < 0)
                                mOn = 0;
                            if (grnPixels[a].green == 0)
                            {
                                tagged++;
                                if (tagged > tool.toolMinUnappliedPixels)
                                {
                                    tool.section[j].isSectionRequiredOn = true;
                                    goto GetOutSectionOff1;
                                }
                            }
                        }
                    }
GetOutSectionOff1:
                    start = 0;
                }

                start = 0; end = 0;
            }

            if (tool.section[j].speedPixels < 0)
            {
                tool.section[j].isSectionRequiredOn = false;

                tool.section[j].isMappingRequiredOn = false;
                tool.section[j].mappingOffRequest = true;
                tool.section[j].mappingOnRequest = false;
            }
        }  // end of go thru all sections "for"
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
            if (pn.speed < SETTINGS_VEHICLE_SLOWSPEEDCUTOFF)
            {
                tool.section[j].sectionOnRequest = false;
                tool.section[j].sectionOffRequest = true;
            }
        }

        for (int j = 0; j < tool.numOfSections; j++)
        {
            //now for  mapping
            if (tool.section[j].isMappingRequiredOn && tool.section[j].isAllowedOn)
            {
                //global request to turn on section
                tool.section[j].mappingOnRequest = true;
                tool.section[j].mappingOffRequest = false;
            }

            else if (!tool.section[j].isMappingRequiredOn)
            {
                //global request to turn off section
                tool.section[j].mappingOffRequest = true;
                tool.section[j].mappingOnRequest = false;
            }

            // Manual on, force the section On and exit loop so digital is also overidden
            if (tool.section[j].manBtnState == btnStates::On)
            {
                tool.section[j].mappingOnRequest = true;
                tool.section[j].mappingOffRequest = false;
                continue;
            }

            if (tool.section[j].manBtnState == btnStates::Off)
            {
                tool.section[j].mappingOnRequest = false;
                tool.section[j].mappingOffRequest = true;
            }

            //if going too slow turn off sections
            if (pn.speed < SETTINGS_VEHICLE_SLOWSPEEDCUTOFF)
            {
                tool.section[j].mappingOnRequest = false;
                tool.section[j].mappingOffRequest = true;
            }

        }
    } // end of supersection is off
    //Checks the workswitch if required
    if (isJobStarted && mc.isWorkSwitchEnabled)
    {
        //TODO: workSwitch.checkWorkSwitch();
    }

    //Determine if sections want to be on or off
    processSectionOnOffRequests(isMapping);

    //send the byte out to section machines
    //TODO: buildMachineByte();

    //send the machine out to port
    //TODO: sendOutUSBMachinePort(mc.machineData, CModuleComm.pgnSentenceLength);

    ////send machine data to autosteer if checked
    if (mc.isMachineDataSentToAutoSteer)
        //TODO: SendOutUSBAutoSteerPort(mc.machineData, CModuleComm.pgnSentenceLength);


    //if a minute has elapsed save the field in case of crash and to be able to resume
    if (minuteCounter > 60 && sentenceCounter < 20)
    {
        //tmrWatchdog->stop();

        //don't save if no gps
        if (isJobStarted )
        {
            //auto save the field patches, contours accumulated so far
            //TODO:FileSaveSections();
            //TODO: FileSaveContour();

            //NMEA log file
            //TODO: if (isLogNMEA) FileSaveNMEA();
            //TODO: if (isLogElevation) FileSaveElevation();
            //FileSaveFieldKML();
        }

        /*
         TODO:
        if (isAutoDayNight && tenMinuteCounter > 600)
        {
            tenMinuteCounter = 0;
            isDayTime = (DateTime.Now.Ticks < sunset.Ticks && DateTime.Now.Ticks > sunrise.Ticks);

            if (isDayTime != isDay)
            {
                isDay = isDayTime;
                isDay = !isDay;
                SwapDayNightMode();
            }

            if (sunrise.Date != DateTime.Today)
            {
                IsBetweenSunriseSunset(pn.latitude, pn.longitude);

                //set display accordingly
                isDayTime = (DateTime.Now.Ticks < sunset.Ticks && DateTime.Now.Ticks > sunrise.Ticks);

                lblSunrise.Text = sunrise.ToString("HH:mm");
                lblSunset.Text = sunset.ToString("HH:mm");
            }
        }

        //if its the next day, calc sunrise sunset for next day
        minuteCounter = 0;
        */

        //set saving flag off
        isSavingFile = false;

        //go see if data ready for draw and position updates
        //tmrWatchdog->start();

    }
    //this is the end of the "frame". Now we wait for next NMEA sentence with a valid fix.
}

//Does the logic to process section on off requests
void FormGPS::processSectionOnOffRequests(bool isMapping)
{
    for (int j = 0; j < tool.numOfSections + 1; j++)
    {
        //SECTIONS -


        if (tool.section[j].sectionOnRequest)
            tool.section[j].isSectionOn = true;

        if (!tool.section[j].sectionOffRequest) tool.section[j].sectionOffTimer = (int)((double)fixUpdateHz * tool.turnOffDelay);

        if (tool.section[j].sectionOffTimer > 0) tool.section[j].sectionOffTimer--;

        if (tool.section[j].sectionOffRequest && tool.section[j].sectionOffTimer == 0)
        {
            if (tool.section[j].isSectionOn) tool.section[j].isSectionOn = false;
        }

        //MAPPING -

        //easy just turn it on
        if (tool.section[j].mappingOnRequest)
        {
            if (!tool.section[j].isMappingOn && isMapping) tool.section[j].turnMappingOn(); //**************************************** un comment to enable mappping again
        }

        //turn off
        double sped = 1 / ((pn.speed+5) * 0.2);
        if (sped < 0.2) sped = 0.2;

        //keep setting the timer so full when ready to turn off
        if (!tool.section[j].mappingOffRequest)
            tool.section[j].mappingOffTimer = (int)(fixUpdateHz * 1 * sped + ((double)fixUpdateHz * tool.turnOffDelay));

        //decrement the off timer
        if (tool.section[j].mappingOffTimer > 0) tool.section[j].mappingOffTimer--;

        //if Off mapping timer is zero, turn off the section, reset everything
        if (tool.section[j].mappingOffTimer == 0 && tool.section[j].mappingOffRequest)
        {
            if (tool.section[j].isMappingOn) tool.section[j].turnMappingOff(tool);
            tool.section[j].mappingOffRequest = false;
        }

        //#region notes
        //Turn ON
        //if requested to be on, set the timer to Max 10 (1 seconds) = 10 frames per second
        //if (tool.section[j].sectionOnRequest && !tool.section[j].sectionOnOffCycle)
        //{
        //    tool.section[j].sectionOnTimer = (int)(pn.speed * tool.section[j].lookAheadOn) + 1;
        //    if (tool.section[j].sectionOnTimer > fixUpdateHz + 3) tool.section[j].sectionOnTimer = fixUpdateHz + 3;
        //    tool.section[j].sectionOnOffCycle = true;
        //}

        ////reset the ON request
        //section[j].sectionOnRequest = false;

        ////decrement the timer if not zero
        //if (tool.section[j].sectionOnTimer > 0)
        //{
        //    //turn the section ON if not and decrement timer
        //    tool.section[j].sectionOnTimer--;
        //    if (!tool.section[j].isSectionOn) tool.section[j].isSectionOn = true;

        //    //keep resetting the section OFF timer while the ON is active
        //    //section[j].sectionOffTimer = (int)(fixUpdateHz * tool.toolTurnOffDelay);
        //}
        //if (!tool.section[j].sectionOffRequest)
        //    tool.section[j].sectionOffTimer = (int)(fixUpdateHz * tool.turnOffDelay);

        ////decrement the off timer
        //if (tool.section[j].sectionOffTimer > 0 && tool.section[j].sectionOnTimer == 0) tool.section[j].sectionOffTimer--;

        ////Turn OFF
        ////if Off section timer is zero, turn off the section
        //if (tool.section[j].sectionOffTimer == 0 && tool.section[j].sectionOnTimer == 0 && tool.section[j].sectionOffRequest)
        //{
        //    if (tool.section[j].isSectionOn) tool.section[j].isSectionOn = false;
        //    //section[j].sectionOnOffCycle = false;
        //    tool.section[j].sectionOffRequest = false;
        //    //}
        //}
        //Turn ON
        //if requested to be on, set the timer to Max 10 (1 seconds) = 10 frames per second
        //if (tool.section[j].mappingOnRequest && !tool.section[j].mappingOnOffCycle)
        //{
        //    tool.section[j].mappingOnTimer = (int)(fixUpdateHz * 1) + 1;
        //    tool.section[j].mappingOnOffCycle = true;
        //}

        ////reset the ON request
        //section[j].mappingOnRequest = false;

        //decrement the timer if not zero
        //#endregion notes
    }
}

void FormGPS::tmrWatchdog_timeout()
{
    //Check for a newline char, if none then just return
    sentenceCounter++;
    int cr = pn.rawBuffer.indexOf("\n");
    if (sentenceCounter > 80)
        sentenceCounter = 100; //show no GPS warning

    if (cr == -1) return;

    //tmrWatchdog->stop();

    //did we get a new fix position?
    if (scanForNMEA())
    {
        //reset the dead GPS counter
        if (sentenceCounter > 98)
        {
            camera.camSetDistance = -200;
            setZoom();
        }

        sentenceCounter = 0;


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
            minuteCounter++;

            qmlItem(qml_root,"btnPerimeter")->setProperty("buttonText", locale.toString(vehicle.totalSquareMeters / 1000, 'f', 2));

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

           //lblTrigger.Text = sectionTriggerStepDistance.ToString("N2");
            //lblLift.Text = mc.pgn[mc.azRelayData][mc.rdHydLift].ToString();

        } //end every 1/2 second

        //every fifth second update  ///////////////////////////   FIFTH Fifth ////////////////////////////
        if (displayUpdateOneFifthCounter != oneFifthSecond)
        {
            //reset the counter
            displayUpdateOneFifthCounter = oneFifthSecond;

            if (hd.isOn)
            {
            }

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

    //Do these updates every cycle

    //we cannot manipulate the GUI from any of the OpenGL drawing methods as they run in a
    //different thread.
    if (isAutoSteerBtnOn && !ct.isContourBtnOn) {
        qmlItem(qml_root, "btnManUturnLeft")->setProperty("visible", true);
        qmlItem(qml_root, "btnManUturnRight")->setProperty("visible", true);
    } else {
        qmlItem(qml_root, "btnManUturnLeft")->setProperty("visible", false);
        qmlItem(qml_root, "btnManUturnRight")->setProperty("visible", false);
    }

    if (ct.isContourBtnOn || ABLine.isBtnABLineOn || curve.isBtnCurveOn)
    {
        double dist = vehicle.distanceDisplay * 0.1;
        qmlItem(qml_root, "btnAutoSteer")->setProperty("buttonText", locale.toString(dist,'f',1));
    } else {
        qmlItem(qml_root, "btnAutoSteer")->setProperty("buttonText", "");
    }


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
    double spd = vehicle.avgSpeed;

    //convert to kph
    spd *= 0.1;

    return locale.toString(spd,'f',1);
}

QString FormGPS::speedMPH() {
    double spd = vehicle.avgSpeed;

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

void FormGPS::onClearRecvCounter()
{
    recvCounter = 0;
}

void FormGPS::onHeadingSource(int tempInt)
{
    if (tempInt == 0)//0: no heading, no roll 1: heading ok, no roll 2: roll+heading ok
    {
        if (headingFromSource != "Fix") { headingFromSourceBak = headingFromSource; }
        headingFromSource = "Fix";
    }
    else { headingFromSource = headingFromSourceBak; }
}
