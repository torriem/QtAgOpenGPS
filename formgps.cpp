// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Main class where everything is initialized
#include "formgps.h"
#include "aogproperty.h"
#include <QColor>
#include <QRgb>
#include "qmlutil.h"
#include "glm.h"
#include "cpgn.h"
#include <QLocale>
#include <QLabel>
#include <QQuickWindow>
#include "qmlsettings.h"

extern QLabel *grnPixelsWindow;
extern QMLSettings qml_settings;

FormGPS::FormGPS(QWidget *parent) : QQmlApplicationEngine(parent)
{


    connect_classes(); //make all the inter-class connections
    qml_settings.setupKeys();
    qml_settings.loadSettings();  //fetch everything from QSettings for QML to use

    /* Temporary test data to see if drawing routines are working. */

    /*
    if ((QString)property_setVehicle_vehicleName == "Default Vehicle") {
        //set up a default vehicle

        //fieldColor = QColor(s.value("display/fieldColor", "#82781E").toString());
        //sectionColor = QColor(s.value("display/sectionColor", "#32DCC8").toString());
        property_setMenu_isCompassOn = true;
        property_setMenu_isSpeedoOn = true;

        //just for debugging I think, force sections to be set
        property_setSection_position1 = -3.3528;
        property_setSection_position2 = -2.2352;
        property_setSection_position3 = -1.1176;
        property_setSection_position4 = 0;
        property_setSection_position5 = 1.1176;
        property_setSection_position6 = 2.2352;
        property_setSection_position7 = 3.3528;
        property_setVehicle_numSections = 6;
        property_setVehicle_toolWidth = 6.7056;


        property_setTool_zones = QVector<int>( { 6,2,4,6,8,10,12,0,0 }); //2 rows per zone
        property_setTool_numSectionsMulti = 12; //12 rows
        property_setTool_sectionWidthMulti = .5588; //22" rows
        property_setTool_isSectionsNotZones = true; //enable zones

        if (property_setTool_isSectionsNotZones) {
            tool.sectionCalcWidths();
        } else {
            tool.sectionCalcMulti();
        }



        property_setVehicle_wheelbase = 3.1496;
        property_setVehicle_trackWidth = 2.286;
        property_setVehicle_hitchLength = -2.54;
        property_setTool_isToolTBT=true;
        property_setVehicle_tankTrailingHitchLength = -3;

        property_setTool_isToolTrailing = true;
        property_setTool_toolTrailingHitchLength = -4.572;
        property_setVehicle_minTurningRadius = 8;
        property_setVehicle_maxSteerAngle = 30;
    } else {
        //reload our saved settings
        vehicle_load(property_setVehicle_vehicleName);
    }
    */
    setupGui();
    loadSettings();

    //QML does this now
    //LineUpIndividualSectionBtns();

    /*
    //hard wire this on for testing
    isJobStarted = true;
    //fileCreateField();
    if (! FileOpenField((QString)property_setF_CurrentDir))
    {
        //set up default field to play in for debugging purposes
        currentFieldDirectory = "TestField";
        property_setF_CurrentDir = currentFieldDirectory;
        pn.latStart = sim.latitude;
        pn.lonStart = sim.longitude;

        FileCreateField();

        ABLine.refPoint1.easting = 0;
        ABLine.refPoint1.easting = 0;
        ABLine.abHeading = glm::toRadians(5.0f);
        ABLine.SetABLineByHeading();

        CABLines line;
        line.origin = Vec2(0,0);
        line.origin = Vec2(0,0);
        line.heading = glm::toRadians(5.0f);
        line.Name = "Test AB Line";
        ABLine.lineArr.append( line );
        FileSaveABLines();

        CBoundaryList boundary;
        boundary.isDriveThru = true;

        boundary.fenceLine.append(Vec3(-100,0,0));
        boundary.fenceLine.append(Vec3(-100,250,glm::toRadians((double)90)));
        boundary.fenceLine.append(Vec3( 100,250,glm::toRadians((double)180)));
        boundary.fenceLine.append(Vec3( 100,0,glm::toRadians((double)270)));
        boundary.fenceLine.append(Vec3(-100,0,0));

        boundary.CalculateFenceArea(0);

        double delta = 0;
        boundary.fenceLineEar.clear();

        for (int i = 0; i < boundary.fenceLine.count(); i++)
        {
            if (i == 0)
            {
                boundary.fenceLineEar.append(Vec2(boundary.fenceLine[i].easting, boundary.fenceLine[i].northing));
                continue;
            }
            delta += (boundary.fenceLine[i - 1].heading - boundary.fenceLine[i].heading);
            if (fabs(delta) > 0.005)
            {
                boundary.fenceLineEar.append(Vec2(boundary.fenceLine[i].easting, boundary.fenceLine[i].northing));
                delta = 0;
            }
        }
        bnd.bndList.append(boundary);
        fd.UpdateFieldBoundaryGUIAreas(bnd.bndList);

        calculateMinMax();
        FileSaveBoundary();
        bnd.BuildTurnLines(fd);

        bootstrap_field=true;
        isJobStarted = true;
    }
    //ABLine.isABLineSet = true;
    //ABLine.isBtnABLineOn = true;
    */

    isJobStarted = false;

    StartLoopbackServer();
    if ((bool)property_setMenu_isSimulatorOn == false) {
        qDebug() << "Stopping simulator because it's off in settings.";
        timerSim.stop();
    }
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
    //qDebug() << "frame time before doing section lookahead " << swFrame.elapsed();
    lock.lockForWrite();
    //qDebug() << "frame time after getting lock  " << swFrame.elapsed();

    if (property_displayShowBack)
        grnPixelsWindow->setPixmap(QPixmap::fromImage(grnPix.mirrored()));

    //determine where the tool is wrt to headland
    if (bnd.isHeadlandOn) bnd.WhereAreToolCorners(tool);

    //set the look ahead for hyd Lift in pixels per second
    vehicle.hydLiftLookAheadDistanceLeft = tool.farLeftSpeed * vehicle.hydLiftLookAheadTime * 10;
    vehicle.hydLiftLookAheadDistanceRight = tool.farRightSpeed * vehicle.hydLiftLookAheadTime * 10;

    if (vehicle.hydLiftLookAheadDistanceLeft > 200) vehicle.hydLiftLookAheadDistanceLeft = 200;
    if (vehicle.hydLiftLookAheadDistanceRight > 200) vehicle.hydLiftLookAheadDistanceRight = 200;

    tool.lookAheadDistanceOnPixelsLeft = tool.farLeftSpeed * tool.lookAheadOnSetting * 10;
    tool.lookAheadDistanceOnPixelsRight = tool.farRightSpeed * tool.lookAheadOnSetting * 10;

    if (tool.lookAheadDistanceOnPixelsLeft > 200) tool.lookAheadDistanceOnPixelsLeft = 200;
    if (tool.lookAheadDistanceOnPixelsRight > 200) tool.lookAheadDistanceOnPixelsRight = 200;

    tool.lookAheadDistanceOffPixelsLeft = tool.farLeftSpeed * tool.lookAheadOffSetting * 10;
    tool.lookAheadDistanceOffPixelsRight = tool.farRightSpeed * tool.lookAheadOffSetting * 10;

    if (tool.lookAheadDistanceOffPixelsLeft > 160) tool.lookAheadDistanceOffPixelsLeft = 160;
    if (tool.lookAheadDistanceOffPixelsRight > 160) tool.lookAheadDistanceOffPixelsRight = 160;

    //determine if section is in boundary and headland using the section left/right positions
    bool isLeftIn = true, isRightIn = true;

    if (bnd.bndList.count() > 0)
    {
        for (int j = 0; j < tool.numOfSections; j++)
        {
            //only one first left point, the rest are all rights moved over to left
            isLeftIn = j == 0 ? bnd.IsPointInsideFenceArea(tool.section[j].leftPoint) : isRightIn;
            isRightIn = bnd.IsPointInsideFenceArea(tool.section[j].rightPoint);

            if (tool.isSectionOffWhenOut)
            {
                //merge the two sides into in or out
                if (isLeftIn || isRightIn) tool.section[j].isInBoundary = true;
                else tool.section[j].isInBoundary = false;
            }
            else
            {
                //merge the two sides into in or out
                if (!isLeftIn || !isRightIn) tool.section[j].isInBoundary = false;
                else tool.section[j].isInBoundary = true;
            }
        }
    }

    //determine farthest ahead lookahead - is the height of the readpixel line
    double rpHeight = 0;
    double rpOnHeight = 0;
    double rpToolHeight = 0;

    //pick the larger side
    if (vehicle.hydLiftLookAheadDistanceLeft > vehicle.hydLiftLookAheadDistanceRight) rpToolHeight = vehicle.hydLiftLookAheadDistanceLeft;
    else rpToolHeight = vehicle.hydLiftLookAheadDistanceRight;

    if (tool.lookAheadDistanceOnPixelsLeft > tool.lookAheadDistanceOnPixelsRight) rpOnHeight = tool.lookAheadDistanceOnPixelsLeft;
    else rpOnHeight = tool.lookAheadDistanceOnPixelsRight;

    isHeadlandClose = false;

    //clamp the height after looking way ahead, this is for switching off super section only
    rpOnHeight = fabs(rpOnHeight);
    rpToolHeight = fabs(rpToolHeight);

    //10 % min is required for overlap, otherwise it never would be on.
    int pixLimit = (int)((double)(tool.section[0].rpSectionWidth * rpOnHeight) / (double)(5.0));

    if ((rpOnHeight < rpToolHeight && bnd.isHeadlandOn && bnd.isSectionControlledByHeadland)) rpHeight = rpToolHeight + 2;
    else rpHeight = rpOnHeight + 2;

    if (rpHeight > 290) rpHeight = 290;
    if (rpHeight < 8) rpHeight = 8;

    //read the whole block of pixels up to max lookahead, one read only
    //pixels are already read in another thread.

    //determine if headland is in read pixel buffer left middle and right.
    int start = 0, end = 0, tagged = 0, totalPixel = 0;

    //slope of the look ahead line
    double mOn = 0, mOff = 0;

    //tram and hydraulics
    if (tram.displayMode > 0 && tool.width > vehicle.trackWidth)
    {
        tram.controlByte = 0;
        //1 pixels in is there a tram line?
        if (tram.isOuter)
        {
            if (grnPixels[(int)(tram.halfWheelTrack * 10)].green == 245) tram.controlByte += 2;
            if (grnPixels[tool.rpWidth - (int)(tram.halfWheelTrack * 10)].green == 245) tram.controlByte += 1;
        }
        else
        {
            if (grnPixels[tool.rpWidth / 2 - (int)(tram.halfWheelTrack * 10)].green == 245) tram.controlByte += 2;
            if (grnPixels[tool.rpWidth / 2 + (int)(tram.halfWheelTrack * 10)].green == 245) tram.controlByte += 1;
        }
    }
    else tram.controlByte = 0;

    //determine if in or out of headland, do hydraulics if on
    if (bnd.isHeadlandOn)
    {
        //calculate the slope
        double m = (vehicle.hydLiftLookAheadDistanceRight - vehicle.hydLiftLookAheadDistanceLeft) / tool.rpWidth;
        int height = 1;

        for (int pos = 0; pos < tool.rpWidth; pos++)
        {
            height = (int)(vehicle.hydLiftLookAheadDistanceLeft + (m * pos)) - 1;
            for (int a = pos; a < height * tool.rpWidth; a += tool.rpWidth)
            {
                if (grnPixels[a].green == 250)
                {
                    isHeadlandClose = true;
                    goto GetOutTool;
                }
            }
        }

    GetOutTool: //goto

        //is the tool completely in the headland or not
        bnd.isToolInHeadland = bnd.isToolOuterPointsInHeadland && !isHeadlandClose;

        //set hydraulics based on tool in headland or not
        bnd.SetHydPosition(autoBtnState, p_239, vehicle);

        //set hydraulics based on tool in headland or not
        bnd.SetHydPosition(autoBtnState, p_239, vehicle);

    }

    ///////////////////////////////////////////   Section control        ssssssssssssssssssssss

    int endHeight = 1, startHeight = 1;

    if (bnd.isHeadlandOn && bnd.isSectionControlledByHeadland) bnd.WhereAreToolLookOnPoints(vehicle, tool);

    for (int j = 0; j < tool.numOfSections; j++)
    {
        //Off or too slow or going backwards
        if (tool.sectionButtonState.get(j) == btnStates::Off || vehicle.avgSpeed < vehicle.slowSpeedCutoff || tool.section[j].speedPixels < 0)
        {
            tool.section[j].sectionOnRequest = false;
            tool.section[j].sectionOffRequest = true;

            // Manual on, force the section On
            if (tool.sectionButtonState.get(j) == btnStates::On)
            {
                tool.section[j].sectionOnRequest = true;
                tool.section[j].sectionOffRequest = false;
                continue;
            }
            continue;
        }

        // Manual on, force the section On
        if (tool.sectionButtonState.get(j) == btnStates::On)
        {
            tool.section[j].sectionOnRequest = true;
            tool.section[j].sectionOffRequest = false;
            continue;
        }


        //AutoSection - If any nowhere applied, send OnRequest, if its all green send an offRequest
        tool.section[j].isSectionRequiredOn = false;

        //calculate the slopes of the lines
        mOn = (tool.lookAheadDistanceOnPixelsRight - tool.lookAheadDistanceOnPixelsLeft) / tool.rpWidth;
        mOff = (tool.lookAheadDistanceOffPixelsRight - tool.lookAheadDistanceOffPixelsLeft) / tool.rpWidth;

        start = tool.section[j].rpSectionPosition - tool.section[0].rpSectionPosition;
        end = tool.section[j].rpSectionWidth - 1 + start;

        if (end >= tool.rpWidth)
            end = tool.rpWidth - 1;

        totalPixel = 1;
        tagged = 0;

        for (int pos = start; pos <= end; pos++)
        {
            startHeight = (int)(tool.lookAheadDistanceOffPixelsLeft + (mOff * pos)) * tool.rpWidth + pos;
            endHeight = (int)(tool.lookAheadDistanceOnPixelsLeft + (mOn * pos)) * tool.rpWidth + pos;

            for (int a = startHeight; a <= endHeight; a += tool.rpWidth)
            {
                totalPixel++;
                if (grnPixels[a].green == 0) tagged++;
            }
        }

        //determine if meeting minimum coverage
        tool.section[j].isSectionRequiredOn = ((tagged * 100) / totalPixel > (100 - tool.minCoverage));

        //logic if in or out of boundaries or headland
        if (bnd.bndList.count() > 0)
        {
            //if out of boundary, turn it off
            if (!tool.section[j].isInBoundary)
            {
                tool.section[j].isSectionRequiredOn = false;
                tool.section[j].sectionOffRequest = true;
                tool.section[j].sectionOnRequest = false;
                tool.section[j].sectionOffTimer = 0;
                tool.section[j].sectionOnTimer = 0;
                continue;
            }
            else
            {
                //is headland coming up
                if (bnd.isHeadlandOn && bnd.isSectionControlledByHeadland)
                {
                    bool isHeadlandInLookOn = false;

                    //is headline in off to on area
                    mOn = (tool.lookAheadDistanceOnPixelsRight - tool.lookAheadDistanceOnPixelsLeft) / tool.rpWidth;
                    mOff = (tool.lookAheadDistanceOffPixelsRight - tool.lookAheadDistanceOffPixelsLeft) / tool.rpWidth;

                    start = tool.section[j].rpSectionPosition - tool.section[0].rpSectionPosition;

                    end = tool.section[j].rpSectionWidth - 1 + start;

                    if (end >= tool.rpWidth)
                        end = tool.rpWidth - 1;

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
                                isHeadlandInLookOn = true;
                                goto GetOutHdOn;
                            }
                        }
                    }
                GetOutHdOn:

                    //determine if look ahead points are completely in headland
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


        //global request to turn on section
        tool.section[j].sectionOnRequest = tool.section[j].isSectionRequiredOn;
        tool.section[j].sectionOffRequest = !tool.section[j].sectionOnRequest;

    }  // end of go thru all sections "for"

    //Set all the on and off times based from on off section requests
    for (int j = 0; j < tool.numOfSections; j++)
    {
        //SECTION timers

        if (tool.section[j].sectionOnRequest)
            tool.section[j].isSectionOn = true;

        //turn off delay
        if (tool.turnOffDelay > 0)
        {
            if (!tool.section[j].sectionOffRequest) tool.section[j].sectionOffTimer = (int)(gpsHz / 2.0 * tool.turnOffDelay);

            if (tool.section[j].sectionOffTimer > 0) tool.section[j].sectionOffTimer--;

            if (tool.section[j].sectionOffRequest && tool.section[j].sectionOffTimer == 0)
            {
                if (tool.section[j].isSectionOn) tool.section[j].isSectionOn = false;
            }
        }
        else
        {
            if (tool.section[j].sectionOffRequest)
                tool.section[j].isSectionOn = false;
        }

        //Mapping timers
        if (tool.section[j].sectionOnRequest && !tool.section[j].isMappingOn && tool.section[j].mappingOnTimer == 0)
        {
            tool.section[j].mappingOnTimer = (int)(tool.lookAheadOnSetting * (gpsHz / 2) - 1);
        }
        else if (tool.section[j].sectionOnRequest && tool.section[j].isMappingOn && tool.section[j].mappingOffTimer > 1)
        {
            tool.section[j].mappingOffTimer = 0;
            tool.section[j].mappingOnTimer = (int)(tool.lookAheadOnSetting * (gpsHz / 2) - 1);
        }

        if (tool.lookAheadOffSetting > 0)
        {
            if (tool.section[j].sectionOffRequest && tool.section[j].isMappingOn && tool.section[j].mappingOffTimer == 0)
            {
                tool.section[j].mappingOffTimer = (int)(tool.lookAheadOffSetting * (gpsHz / 2) + 4);
            }
        }
        else if (tool.turnOffDelay > 0)
        {
            if (tool.section[j].sectionOffRequest && tool.section[j].isMappingOn && tool.section[j].mappingOffTimer == 0)
                tool.section[j].mappingOffTimer = (int)(tool.turnOffDelay * gpsHz / 2);
        }
        else
        {
            tool.section[j].mappingOffTimer = 0;
        }

        //MAPPING - Not the making of triangle patches - only status - on or off
        if (tool.section[j].sectionOnRequest)
        {
            tool.section[j].mappingOffTimer = 0;
            if (tool.section[j].mappingOnTimer > 1)
                tool.section[j].mappingOnTimer--;
            else
            {
                tool.section[j].isMappingOn = true;
            }
        }

        if (tool.section[j].sectionOffRequest)
        {
            tool.section[j].mappingOnTimer = 0;
            if (tool.section[j].mappingOffTimer > 1)
                tool.section[j].mappingOffTimer--;
            else
            {
                tool.section[j].isMappingOn = false;
            }
        }
    }

    //Checks the workswitch or steerSwitch if required
    if (ahrs.isAutoSteerAuto || mc.isRemoteWorkSystemOn)
        mc.CheckWorkAndSteerSwitch(ahrs,isAutoSteerBtnOn);

    // check if any sections have changed status
    number = 0;

    for (int j = 0; j < tool.numOfSections; j++)
    {
        if (tool.section[j].isMappingOn)
        {
            number |= 1ul << j;
        }
    }

    //there has been a status change of section on/off
    if (number != lastNumber)
    {
        int sectionOnOffZones = 0, patchingZones = 0;

        //everything off
        if (number == 0)
        {
            for (int j = 0; j < triStrip.count(); j++)
            {
                if (triStrip[j].isDrawing)
                    triStrip[j].TurnMappingOff(tool, fd);
            }
        }
        else if (!tool.isMultiColoredSections)
        {
            //set the start and end positions from section points
            for (int j = 0; j < tool.numOfSections; j++)
            {
                //skip till first mapping section
                if (!tool.section[j].isMappingOn) continue;

                //do we need more patches created
                if (triStrip.count() < sectionOnOffZones + 1)
                    triStrip.append(CPatches());

                //set this strip start edge to edge of this section
                triStrip[sectionOnOffZones].newStartSectionNum = j;

                while ((j + 1) < tool.numOfSections && tool.section[j + 1].isMappingOn)
                {
                    j++;
                }

                //set the edge of this section to be end edge of strp
                triStrip[sectionOnOffZones].newEndSectionNum = j;
                sectionOnOffZones++;
            }

            //count current patch strips being made
            for (int j = 0; j < triStrip.count(); j++)
            {
                if (triStrip[j].isDrawing) patchingZones++;
            }

            //tests for creating new strips or continuing
            bool isOk = (patchingZones == sectionOnOffZones && sectionOnOffZones < 3);

            if (isOk)
            {
                for (int j = 0; j < sectionOnOffZones; j++)
                {
                    if (triStrip[j].newStartSectionNum > triStrip[j].currentEndSectionNum
                        || triStrip[j].newEndSectionNum < triStrip[j].currentStartSectionNum)
                        isOk = false;
                }
            }

            if (isOk)
            {
                for (int j = 0; j < sectionOnOffZones; j++)
                {
                    if (triStrip[j].newStartSectionNum != triStrip[j].currentStartSectionNum
                        || triStrip[j].newEndSectionNum != triStrip[j].currentEndSectionNum)
                    {
                        //if (tool.isSectionsNotZones)
                        {
                            triStrip[j].AddMappingPoint(tool,fd, 0);
                        }

                        triStrip[j].currentStartSectionNum = triStrip[j].newStartSectionNum;
                        triStrip[j].currentEndSectionNum = triStrip[j].newEndSectionNum;
                        triStrip[j].AddMappingPoint(tool,fd, 0);
                    }
                }
            }
            else
            {
                //too complicated, just make new strips
                for (int j = 0; j < triStrip.count(); j++)
                {
                    if (triStrip[j].isDrawing)
                        triStrip[j].TurnMappingOff(tool, fd);
                }

                for (int j = 0; j < sectionOnOffZones; j++)
                {
                    triStrip[j].currentStartSectionNum = triStrip[j].newStartSectionNum;
                    triStrip[j].currentEndSectionNum = triStrip[j].newEndSectionNum;
                    triStrip[j].TurnMappingOn(tool, 0);
                }
            }
        }
        else if (tool.isMultiColoredSections) //could be else only but this is more clear
        {
            //set the start and end positions from section points
            for (int j = 0; j < tool.numOfSections; j++)
            {
                //do we need more patches created
                if (triStrip.count() < sectionOnOffZones + 1)
                    triStrip.append(CPatches());

                //set this strip start edge to edge of this section
                triStrip[sectionOnOffZones].newStartSectionNum = j;

                //set the edge of this section to be end edge of strp
                triStrip[sectionOnOffZones].newEndSectionNum = j;
                sectionOnOffZones++;

                if (!tool.section[j].isMappingOn)
                {
                    if (triStrip[j].isDrawing)
                        triStrip[j].TurnMappingOff(tool, fd);
                }
                else
                {
                    triStrip[j].currentStartSectionNum = triStrip[j].newStartSectionNum;
                    triStrip[j].currentEndSectionNum = triStrip[j].newEndSectionNum;
                    triStrip[j].TurnMappingOn(tool,j);
                }
            }
        }


        lastNumber = number;
    }

    //send the byte out to section machines
    BuildMachineByte();

    //if a minute has elapsed save the field in case of crash and to be able to resume
    if (minuteCounter > 30 && (uint)sentenceCounter < 20)
    {
        tmrWatchdog->stop();

        //don't save if no gps
        if (isJobStarted)
        {
            //auto save the field patches, contours accumulated so far
            FileSaveSections();
            FileSaveContour();

            //NMEA log file
            //TODO: if (isLogElevation) FileSaveElevation();
            //ExportFieldAs_KML();
        }

        //if its the next day, calc sunrise sunset for next day
        minuteCounter = 0;

        //set saving flag off
        isSavingFile = false;

        //go see if data ready for draw and position updates
        tmrWatchdog->start();

        //calc overlap
        //oglZoom.Refresh();

    }

    //stop the timer and calc how long it took to do calcs and draw
    frameTimeRough = swFrame.elapsed();
    //qDebug() << "frame time after finishing section lookahead " << frameTimeRough ;

    if (frameTimeRough > 50) frameTimeRough = 50;
    frameTime = frameTime * 0.90 + frameTimeRough * 0.1;

    QObject *aog = qmlItem(qml_root, "aog");
    aog->setProperty("frameTime", frameTime);

    //TODO 5 hz sections
    //if (bbCounter++ > 0)
    //    bbCounter = 0;

    //draw the section control window off screen buffer
    //if (bbCounter == 0)
    //{
    if (isJobStarted)
    {
        p_239.pgn[p_239.geoStop] = mc.isOutOfBounds ? 1 : 0;

        SendPgnToLoop(p_239.pgn);

        SendPgnToLoop(p_229.pgn);
    }


    lock.unlock();

    //this is the end of the "frame". Now we wait for next NMEA sentence with a valid fix.
}

void FormGPS::tmrWatchdog_timeout()
{
    //TODO: replace all this with individual timers for cleaner

    if (! (bool)property_setMenu_isSimulatorOn && timerSim.isActive()) {
        qDebug() << "Shutting down simulator.";
        timerSim.stop();
    } else if ( (bool)property_setMenu_isSimulatorOn && ! timerSim.isActive() ) {
        qDebug() << "Starting up simulator.";
        pn.latitude = sim.latitude;
        pn.longitude = sim.longitude;
        pn.headingTrue = 0;

        timerSim.start(100); //fire simulator every 100 ms.
        gpsHz = 10;
    }


    // This is done in QML
//    if ((uint)sentenceCounter++ > 20)
//    {
//        //TODO: ShowNoGPSWarning();
//        return;
//    }
    sentenceCounter += 1;


    if (tenSecondCounter++ >= 40)
    {
        tenSecondCounter = 0;
        tenSeconds++;
    }
    if (threeSecondCounter++ >= 12)
    {
        threeSecondCounter = 0;
        threeSeconds++;
    }
    if (oneSecondCounter++ >= 4)
    {
        oneSecondCounter = 0;
        oneSecond++;
    }
    if (oneHalfSecondCounter++ >= 2)
    {
        oneHalfSecondCounter = 0;
        oneHalfSecond++;
    }
    if (oneFifthSecondCounter++ >= 0)
    {
        oneFifthSecondCounter = 0;
        oneFifthSecond++;
    }

    ////////////////////////////////////////////// 10 second ///////////////////////////////////////////////////////
    //every 10 second update status
    if (tenSeconds != 0)
    {
        //reset the counter
        tenSeconds = 0;
    }
    /////////////////////////////////////////////////////////   333333333333333  ////////////////////////////////////////
    //every 3 second update status
    if (displayUpdateThreeSecondCounter != threeSeconds)
    {
        //reset the counter
        displayUpdateThreeSecondCounter = threeSeconds;

        //check to make sure the grid is big enough
        //worldGrid.checkZoomWorldGrid(pn.fix.northing, pn.fix.easting);

        //hide the NAv panel in 6  secs
        /* TODO:
        if (panelNavigation.Visible)
        {
            if (navPanelCounter-- < 2) panelNavigation.Visible = false;
        }

        if (panelNavigation.Visible)
            lblHz.Text = gpsHz.ToString("N1") + " ~ " + (frameTime.ToString("N1")) + " " + FixQuality;

        lblFix.Text = FixQuality + pn.age.ToString("N1");

        lblTime.Text = DateTime.Now.ToString("T");
        */

        //save nmea log file
        //TODO: if (isLogNMEA) FileSaveNMEA();

        //update button lines numbers
        //TODO: UpdateGuidanceLineButtonNumbers();

    }//end every 3 seconds

    //every second update all status ///////////////////////////   1 1 1 1 1 1 ////////////////////////////
    if (displayUpdateOneSecondCounter != oneSecond)
    {
        //reset the counter
        displayUpdateOneSecondCounter = oneSecond;

        //counter used for saving field in background
        minuteCounter++;
        tenMinuteCounter++;
    }

    //every half of a second update all status  ////////////////    0.5  0.5   0.5    0.5    /////////////////
    if (displayUpdateHalfSecondCounter != oneHalfSecond)
    {
        //reset the counter
        displayUpdateHalfSecondCounter = oneHalfSecond;

        isFlashOnOff = !isFlashOnOff;

        //the ratemap trigger
        worldGrid.isRateTrigger = true;

        //Make sure it is off when it should
        if ((!ct.isContourBtnOn && trk.idx == -1 && isAutoSteerBtnOn)
            ) onStopAutoSteer();

    } //end every 1/2 second

    //every fourth second update  ///////////////////////////   Fourth  ////////////////////////////
    {
        //reset the counter
        oneHalfSecondCounter++;
        oneSecondCounter++;
        makeUTurnCounter++;

        secondsSinceStart = stopwatch.elapsed() / 1000.0;
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

void FormGPS::SwapDirection() {
    if (!yt.isYouTurnTriggered)
    {
        yt.isYouTurnRight = ! yt.isYouTurnRight;
        yt.ResetCreatedYouTurn(makeUTurnCounter);
    }
    else if (yt.isYouTurnBtnOn)
    {
        yt.isYouTurnBtnOn = false;
    }
}


void FormGPS::JobClose()
{
    recPath.resumeState = 0;
    recPath.currentPositonIndex = 0;

    sbGrid.clear();

    //reset field offsets
    if (!isKeepOffsetsOn)
    {
        pn.fixOffset.easting = 0;
        pn.fixOffset.northing = 0;
    }

    //turn off headland
    bnd.isHeadlandOn = false; //this turns off the button

    recPath.recList.clear();
    recPath.StopDrivingRecordedPath();

    //make sure hydraulic lift is off
    p_239.pgn[p_239.hydLift] = 0;
    vehicle.isHydLiftOn = false; //this turns off the button also

    //oglZoom.SendToBack();

    //clean all the lines
    bnd.bndList.clear();
    //TODO: bnd.shpList.clear();


    isJobStarted = false;

    //fix ManualOffOnAuto buttons
    manualBtnState = btnStates::Off;

    //fix auto button
    autoBtnState = btnStates::Off;

    /*
    btnZone1.BackColor = Color.Silver;
    btnZone2.BackColor = Color.Silver;
    btnZone3.BackColor = Color.Silver;
    btnZone4.BackColor = Color.Silver;
    btnZone5.BackColor = Color.Silver;
    btnZone6.BackColor = Color.Silver;
    btnZone7.BackColor = Color.Silver;
    btnZone8.BackColor = Color.Silver;

    btnZone1.Enabled = false;
    btnZone2.Enabled = false;
    btnZone3.Enabled = false;
    btnZone4.Enabled = false;
    btnZone5.Enabled = false;
    btnZone6.Enabled = false;
    btnZone7.Enabled = false;
    btnZone8.Enabled = false;

    btnSection1Man.Enabled = false;
    btnSection2Man.Enabled = false;
    btnSection3Man.Enabled = false;
    btnSection4Man.Enabled = false;
    btnSection5Man.Enabled = false;
    btnSection6Man.Enabled = false;
    btnSection7Man.Enabled = false;
    btnSection8Man.Enabled = false;
    btnSection9Man.Enabled = false;
    btnSection10Man.Enabled = false;
    btnSection11Man.Enabled = false;
    btnSection12Man.Enabled = false;
    btnSection13Man.Enabled = false;
    btnSection14Man.Enabled = false;
    btnSection15Man.Enabled = false;
    btnSection16Man.Enabled = false;

    btnSection1Man.BackColor = Color.Silver;
    btnSection2Man.BackColor = Color.Silver;
    btnSection3Man.BackColor = Color.Silver;
    btnSection4Man.BackColor = Color.Silver;
    btnSection5Man.BackColor = Color.Silver;
    btnSection6Man.BackColor = Color.Silver;
    btnSection7Man.BackColor = Color.Silver;
    btnSection8Man.BackColor = Color.Silver;
    btnSection9Man.BackColor = Color.Silver;
    btnSection10Man.BackColor = Color.Silver;
    btnSection11Man.BackColor = Color.Silver;
    btnSection12Man.BackColor = Color.Silver;
    btnSection13Man.BackColor = Color.Silver;
    btnSection14Man.BackColor = Color.Silver;
    btnSection15Man.BackColor = Color.Silver;
    btnSection16Man.BackColor = Color.Silver;
    */

    //clear the section lists
    for (int j = 0; j < triStrip.count(); j++)
    {
        //clean out the lists
        triStrip[j].patchList.clear();
        triStrip[j].triangleList.clear();
    }

    triStrip.clear();
    triStrip.append(CPatches());

    //clear the flags
    flagPts.clear();

    //ABLine
    tram.tramList.clear();

    curve.ResetCurveLine(trk);

    //tracks
    trk.gArr.clear();
    trk.idx = -1;

    //clean up tram
    tram.displayMode = 0;
    tram.generateMode = 0;
    tram.tramBndInnerArr.clear();
    tram.tramBndOuterArr.clear();

    //clear out contour and Lists
    ct.ResetContour();
    ct.isContourBtnOn = false; //turns off button in gui
    ct.isContourOn = false;

    //btnABDraw.Enabled = false;
    //btnCycleLines.Image = Properties.Resources.ABLineCycle;
    //btnCycleLines.Enabled = false;
    //btnCycleLinesBk.Image = Properties.Resources.ABLineCycleBk;
    //btnCycleLinesBk.Enabled = false;

    //AutoSteer
    //btnAutoSteer.Enabled = false;
    isAutoSteerBtnOn = false;

    //auto YouTurn shutdown
    yt.isYouTurnBtnOn = false;

    yt.ResetYouTurn(makeUTurnCounter);

    //reset acre and distance counters
    fd.workedAreaTotal = 0;

    //reset GUI areas
    fd.UpdateFieldBoundaryGUIAreas(bnd.bndList);

    displayFieldName = tr("None");

    recPath.recList.clear();
    recPath.shortestDubinsList.clear();
    recPath.shuttleDubinsList.clear();

    //FixPanelsAndMenus();
    SetZoom();
    worldGrid.isGeoMap = false;
    worldGrid.isRateMap = false;

    //release Bing texture

}

void FormGPS::JobNew()
{
    isJobStarted = true;
    startCounter = 0;

    //btnSectionMasterManual.Enabled = true;
    manualBtnState = btnStates::Off;
    //btnSectionMasterManual.Image = Properties.Resources.ManualOff;

    //btnSectionMasterAuto.Enabled = true;
    autoBtnState = btnStates::Off;
    //btnSectionMasterAuto.Image = Properties.Resources.SectionMasterOff;

    ABLine.abHeading = 0.00;

    SetZoom();
    fileSaveCounter = 25;
    trk.isAutoTrack = false;
}

void FormGPS::FileSaveEverythingBeforeClosingField()
{
    qDebug() << "shutting down, saving field items.";

    //update our settings to the vehicle as well
    if((QString)property_setVehicle_vehicleName != "Default Vehicle") {
        vehicle_saveas(property_setVehicle_vehicleName);
    }

    if (! isJobStarted) return;

    //turn off contour line if on
    if (ct.isContourOn) ct.StopContourLine(contourSaveList);

    //turn off all the sections
    for (int j = 0; j < tool.numOfSections; j++)
    {
        tool.section[j].sectionOnOffCycle = false;
        tool.section[j].sectionOffRequest = false;
    }

    //turn off patching
    for (int j = 0; j < triStrip.count(); j++)
    {
        if (triStrip[j].isDrawing) triStrip[j].TurnMappingOff(tool, fd);
    }

    //FileSaveHeadland();
    FileSaveBoundary();
    FileSaveSections();
    FileSaveContour();

    ExportFieldAs_KML();
    //ExportFieldAs_ISOXMLv3()
    //ExportFieldAs_ISOXMLv4()

    //property_setF_CurrentDir = tr("None");
    //currentFieldDirectory = (QString)property_setF_CurrentDir;
    displayFieldName = tr("None");

    JobClose();
    //Text = "AgOpenGPS";

}
