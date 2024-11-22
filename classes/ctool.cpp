#include "ctool.h"
#include "cvehicle.h"
#include "glm.h"
#include "aogproperty.h"
#include "glutils.h"
#include "ccamera.h"
#include "ctram.h"

void CTool::loadSettings()
{
    //from settings grab the vehicle specifics
    trailingToolToPivotLength = property_setTool_trailingToolToPivotLength;

    width = property_setVehicle_toolWidth;
    overlap = property_setVehicle_toolOverlap;

    offset = property_setVehicle_toolOffset;

    trailingHitchLength = property_setTool_toolTrailingHitchLength;
    tankTrailingHitchLength = property_setVehicle_tankTrailingHitchLength;
    hitchLength = property_setVehicle_hitchLength;

    isToolRearFixed = property_setTool_isToolRearFixed;
    isToolTrailing = property_setTool_isToolTrailing;
    isToolTBT = property_setTool_isToolTBT;
    isToolFrontFixed = property_setTool_isToolFront;

    lookAheadOnSetting = property_setVehicle_toolLookAheadOn;
    lookAheadOffSetting = property_setVehicle_toolLookAheadOff;
    turnOffDelay = property_setVehicle_toolOffDelay;

    isSectionOffWhenOut = property_setTool_isSectionOffWhenOut;

    isSectionsNotZones = property_setTool_isSectionsNotZones;

    if (isSectionsNotZones)
        numOfSections = property_setVehicle_numSections;
    else
        numOfSections = property_setTool_numSectionsMulti;

    minCoverage = property_setVehicle_minCoverage;
    isMultiColoredSections = property_setColor_isMultiColorSections;

    secColors[0] = property_setColor_sec01;
    secColors[1] = property_setColor_sec02;
    secColors[2] = property_setColor_sec03;
    secColors[3] = property_setColor_sec04;
    secColors[4] = property_setColor_sec05;
    secColors[5] = property_setColor_sec06;
    secColors[6] = property_setColor_sec07;
    secColors[7] = property_setColor_sec08;
    secColors[8] = property_setColor_sec09;
    secColors[9] = property_setColor_sec10;
    secColors[10] = property_setColor_sec11;
    secColors[11] = property_setColor_sec12;
    secColors[12] = property_setColor_sec13;
    secColors[13] = property_setColor_sec14;
    secColors[14] = property_setColor_sec15;
    secColors[15] = property_setColor_sec16;

    for (int c=0 ; c < 16; c++) {
        //check setColor[C] to make sure there's nothing over 254
    }

    zoneRanges = property_setTool_zones;
    zones = zoneRanges[0];
    //zoneRanges.removeAt(0); //remove first element since it was a count

    isDisplayTramControl = property_setTool_isDisplayTramControl;

}


CTool::CTool()
{
    loadSettings();
}

void CTool::DrawTool(QOpenGLFunctions *gl, QMatrix4x4 &modelview, QMatrix4x4 projection,
                     bool isJobStarted,
                     CVehicle &v, CCamera &camera, CTram &tram)
{
    double tram_halfWheelTrack = (double)property_setVehicle_trackWidth * 0.5;

    //translate and rotate at pivot axle, caller's mvp will be changed
    //all subsequent draws will be based on this point
    modelview.translate(v.pivotAxlePos.easting, v.pivotAxlePos.northing, 0);

    GLHelperOneColor gldraw;

    QMatrix4x4 mv = modelview; //push matrix (just have to save it)

    //translate down to the hitch pin
    mv.translate(sin(v.fixHeading) * hitchLength,
                            cos(v.fixHeading) * hitchLength, 0);

    //settings doesn't change trailing hitch length if set to rigid, so do it here
    double trailingTank, trailingTool;
    if (isToolTrailing)
    {
        trailingTank = tankTrailingHitchLength;
        trailingTool = trailingHitchLength;
    }
    else { trailingTank = 0; trailingTool = 0; }

    //there is a trailing tow between hitch
    if (isToolTBT && isToolTrailing)
    {
        //rotate to tank heading
        mv.rotate(glm::toDegrees(-v.tankPos.heading), 0.0, 0.0, 1.0);


        //draw the tank hitch
        //draw the rigid hitch
        gldraw.append(QVector3D(-0.57, trailingTank, 0.0));
        gldraw.append(QVector3D(0, 0, 0));
        gldraw.append(QVector3D(0.57,trailingTank, 0.0));
        gldraw.draw(gl,projection*mv,QColor::fromRgbF(0.0, 0.0, 0.0),GL_LINE_LOOP, 6.0f);

        //draw the rigid hitch
        gldraw.draw(gl,projection*mv,QColor::fromRgbF(0.765f, 0.76f, 0.32f),GL_LINE_LOOP, 6.0f);

        //move down the tank hitch, unwind, rotate to section heading
        mv.translate(0.0, trailingTank, 0.0);
        mv.rotate(glm::toDegrees(v.tankPos.heading), 0.0, 0.0, 1.0);
        mv.rotate(glm::toDegrees(-v.toolPos.heading), 0.0, 0.0, 1.0);
    }

    //no tow between hitch
    else
    {
        mv.rotate(glm::toDegrees(-v.toolPos.heading), 0.0, 0.0, 1.0);
    }

    //draw the hitch if trailing
    if (isToolTrailing)
    {
        gldraw.clear();
        gldraw.append(QVector3D(-0.4 + offset, trailingTool, 0.0));
        gldraw.append(QVector3D(0,0,0));
        gldraw.append(QVector3D(0.4 + offset, trailingTool, 0.0));
        gldraw.draw(gl,projection*mv,QColor::fromRgbF(0.0f, 0.0f, 0.0f),GL_LINE_STRIP, 6.0f);

        gldraw.draw(gl,projection*mv,QColor::fromRgbF(0.7f, 0.4f, 0.2f),GL_LINE_STRIP, 1.0f);

        GLHelperTexture gldrawtex;

        gldrawtex.append( { QVector3D(1.5 + offset, trailingTool + 1, 0), QVector2D(1,0) } ); //Top Right
        gldrawtex.append( { QVector3D(-1.5 + offset, trailingTool + 1, 0), QVector2D(0,0) } ); //Top Left
        gldrawtex.append( { QVector3D(1.5 + offset, trailingTool - 1, 0), QVector2D(1,1) } ); //Bottom Right
        gldrawtex.append( { QVector3D(-1.5 + offset, trailingTool - 1, 0), QVector2D(0,1) } ); //Bottom LEft
        gldrawtex.draw(gl, projection*mv,Textures::TOOLWHEELS, GL_TRIANGLE_STRIP, true, QColor::fromRgbF(1,1,1,0.75));
    }

    trailingTool -= trailingToolToPivotLength;

    //look ahead lines
    GLHelperColors gldrawcolors;
    ColorVertex cv;
    QColor color;


    if (isJobStarted)
    {
        //lookahead section on
        cv.color = QVector4D(0.20f, 0.7f, 0.2f, 1);
        cv.vertex = QVector3D(farLeftPosition, (lookAheadDistanceOnPixelsLeft) * 0.1 + trailingTool, 0);
        gldrawcolors.append(cv);
        cv.vertex = QVector3D(farRightPosition, (lookAheadDistanceOnPixelsRight) * 0.1 + trailingTool, 0);
        gldrawcolors.append(cv);

        //lookahead section off
        cv.color = QVector4D(0.70f, 0.2f, 0.2f, 1);
        cv.vertex = QVector3D(farLeftPosition, (lookAheadDistanceOffPixelsLeft) * 0.1 + trailingTool, 0);
        gldrawcolors.append(cv);
        cv.vertex = QVector3D(farRightPosition, (lookAheadDistanceOffPixelsRight) * 0.1 + trailingTool, 0);
        gldrawcolors.append(cv);


        if (v.isHydLiftOn)
        {
            cv.color = QVector4D(0.70f, 0.2f, 0.72f, 1);
            cv.vertex = QVector3D(section[0].positionLeft, (v.hydLiftLookAheadDistanceLeft * 0.1) + trailingTool, 0);
            gldrawcolors.append(cv);
            cv.vertex = QVector3D(section[numOfSections - 1].positionRight, (v.hydLiftLookAheadDistanceRight * 0.1) + trailingTool, 0);
            gldrawcolors.append(cv);
        }

        gldrawcolors.draw(gl, projection * mv, GL_LINES, 1.0);
    }
    //draw the sections
    //line width 2 now

    double hite = camera.camSetDistance / -150;
    if (hite > 12) hite = 12;
    if (hite < 1) hite = 1;

    for (int j = 0; j < numOfSections; j++)
    {
        //if section is on, green, if off, red color
        if (section[j].isSectionOn)
        {
            if (sectionButtonState.get(j) == btnStates::Auto)
            {
                if (section[j].isMappingOn) color.setRgbF(0.0f, 0.95f, 0.0f, 1.0f);
                else color.setRgbF(0.970f, 0.30f, 0.970f);
            }
            else color.setRgbF(0.97, 0.97, 0, 1.0f);
        }
        else
        {
            if (!section[j].isMappingOn)
                color.setRgbF(0.950f, 0.2f, 0.2f, 1.0f);
            else
                color.setRgbF(0.0f, 0.250f, 0.97f, 1.0f);
        }

        double mid = (section[j].positionRight - section[j].positionLeft) / 2 + section[j].positionLeft;

        gldraw.clear();
        gldraw.append(QVector3D(section[j].positionLeft, trailingTool, 0));
        gldraw.append(QVector3D(section[j].positionLeft, trailingTool - hite, 0));

        gldraw.append(QVector3D(mid, trailingTool - hite * 1.5, 0));

        gldraw.append(QVector3D(section[j].positionRight, trailingTool - hite, 0));
        gldraw.append(QVector3D(section[j].positionRight, trailingTool, 0));

        gldraw.draw(gl, projection * mv, color, GL_TRIANGLE_FAN, 2.0f);

        if (camera.camSetDistance > -width * 200)
        {
            color.setRgbF(0.0, 0.0, 0.0);
            gldraw.draw(gl,projection * mv, color, GL_LINE_LOOP, 1.0);
        }
    }

    //zones
    if (!isSectionsNotZones && zones > 0 && camera.camSetDistance > -150)
    {
        gldraw.clear();
        color.setRgbF(0.5f, 0.80f, 0.950f);
        for (int i = 1; i < zones; i++)
        {
            gldraw.append(QVector3D(section[zoneRanges[i]].positionLeft, trailingTool - 0.4, 0));
            gldraw.append(QVector3D(section[zoneRanges[i]].positionLeft, trailingTool + 0.2, 0));
        }
        gldraw.draw(gl, projection * mv, color, GL_LINES, 2.0f);
    }

    float pointSize;

    //tram Dots
    if (property_setTool_isDisplayTramControl && tram.displayMode != 0)
    {
        if (camera.camSetDistance > -300)
        {
            if (camera.camSetDistance > -100)
                pointSize = 16;
            else pointSize = 12;

            if (tram.isOuter)
            {
                //section markers
                gldrawcolors.clear();
                //right side
                if (((tram.controlByte) & 1) == 1) cv.color = QVector4D(0.0f, 0.900f, 0.39630f,1.0f);
                else cv.color = QVector4D(0, 0, 0, 1.0f);
                cv.vertex = QVector3D(farRightPosition - tram_halfWheelTrack, trailingTool, 0);
                gldrawcolors.append(cv);

                //left side
                if ((tram.controlByte & 2) == 2) cv.color = QVector4D(0.0f, 0.900f, 0.3930f, 1.0f);
                else cv.color = QVector4D(0, 0, 0, 1.0f);
                cv.vertex = QVector3D(farLeftPosition + tram_halfWheelTrack, trailingTool, 0);
                gldrawcolors.append(cv);

                gldrawcolors.draw(gl, projection * mv,GL_POINTS, pointSize);
            }
            else
            {
                gldrawcolors.clear();

                //right side
                if (((tram.controlByte) & 1) == 1) cv.color = QVector4D(0.0f, 0.900f, 0.39630f, 1.0f);
                else cv.color = QVector4D(0, 0, 0, 1.0f);
                cv.vertex = QVector3D(tram_halfWheelTrack, trailingTool, 0);
                gldrawcolors.append(cv);

                //left side
                if ((tram.controlByte & 2) == 2) cv.color = QVector4D(0.0f, 0.900f, 0.3930f, 1.0f);
                else cv.color = QVector4D(0, 0, 0, 1.0f);
                cv.vertex = QVector3D(-tram_halfWheelTrack, trailingTool, 0);
                gldrawcolors.append(cv);
                gldrawcolors.draw(gl, projection * mv, GL_POINTS, pointSize);
            }
        }
    }
}

//function to calculate the width of each section and update
void CTool::sectionCalcWidths()
{
    if (isSectionsNotZones)
    {
        for (int j = 0; j < MAXSECTIONS; j++)
        {
            section[j].sectionWidth = (section[j].positionRight - section[j].positionLeft);
            section[j].rpSectionPosition = 250 + (int)(glm::roundMidAwayFromZero(section[j].positionLeft * 10));
            section[j].rpSectionWidth = (int)(glm::roundMidAwayFromZero(section[j].sectionWidth * 10));
        }

        //calculate tool width based on extreme right and left values
        double width = fabs(section[0].positionLeft) + fabs(section[numOfSections - 1].positionRight);
        property_setVehicle_toolWidth = width; //save it in settings

        //left and right tool position
        farLeftPosition = section[0].positionLeft;
        farRightPosition = section[numOfSections - 1].positionRight;

        //find the right side pixel position
        rpXPosition = 250 + (int)(glm::roundMidAwayFromZero(farLeftPosition * 10));
        rpWidth = (int)(glm::roundMidAwayFromZero(width * 10));
    }
}

//moved from main form to here
void CTool::sectionCalcMulti()
{
    double leftside = width / -2.0;
    double defaultSectionWidth = property_setTool_sectionWidthMulti;
    double offset = property_setVehicle_toolOffset;
    section[0].positionLeft = leftside+offset;

    for (int i = 0; i < numOfSections - 1; i++)
    {
        leftside += defaultSectionWidth;

        section[i].positionRight = leftside + offset;
        section[i + 1].positionLeft = leftside + offset;
        section[i].sectionWidth = defaultSectionWidth;
        section[i].rpSectionPosition = 250 + (int)(glm::roundMidAwayFromZero(section[i].positionLeft * 10));
        section[i].rpSectionWidth = (int)(glm::roundMidAwayFromZero(section[i].sectionWidth * 10));
    }

    leftside += defaultSectionWidth;
    section[numOfSections - 1].positionRight = leftside + offset;
    section[numOfSections - 1].sectionWidth = defaultSectionWidth;
    section[numOfSections - 1].rpSectionPosition = 250 + (int)(glm::roundMidAwayFromZero(section[numOfSections - 1].positionLeft * 10));
    section[numOfSections - 1].rpSectionWidth = (int)(glm::roundMidAwayFromZero(section[numOfSections - 1].sectionWidth * 10));

    //calculate tool width based on extreme right and left values
    width = (section[numOfSections - 1].positionRight) - (section[0].positionLeft);
    property_setVehicle_toolWidth = width; //save it in settings


    //left and right tool position
    farLeftPosition = section[0].positionLeft;
    farRightPosition = section[numOfSections - 1].positionRight;

    //find the right side pixel position
    rpXPosition = 250 + (int)(glm::roundMidAwayFromZero(farLeftPosition * 10));
    rpWidth = (int)(glm::roundMidAwayFromZero(width * 10));

}


void CTool::sectionSetPositions()
{
    section[0].positionLeft = (double)property_setSection_position1 + (double)property_setVehicle_toolOffset;
    section[0].positionRight = (double)property_setSection_position2 + (double)property_setVehicle_toolOffset;

    section[1].positionLeft = (double)property_setSection_position2 + (double)property_setVehicle_toolOffset;
    section[1].positionRight = (double)property_setSection_position3 + (double)property_setVehicle_toolOffset;

    section[2].positionLeft = (double)property_setSection_position3 + (double)property_setVehicle_toolOffset;
    section[2].positionRight = (double)property_setSection_position4 + (double)property_setVehicle_toolOffset;

    section[3].positionLeft = (double)property_setSection_position4 + (double)property_setVehicle_toolOffset;
    section[3].positionRight = (double)property_setSection_position5 + (double)property_setVehicle_toolOffset;

    section[4].positionLeft = (double)property_setSection_position5 + (double)property_setVehicle_toolOffset;
    section[4].positionRight = (double)property_setSection_position6 + (double)property_setVehicle_toolOffset;

    section[5].positionLeft = (double)property_setSection_position6 + (double)property_setVehicle_toolOffset;
    section[5].positionRight = (double)property_setSection_position7 + (double)property_setVehicle_toolOffset;

    section[6].positionLeft = (double)property_setSection_position7 + (double)property_setVehicle_toolOffset;
    section[6].positionRight = (double)property_setSection_position8 + (double)property_setVehicle_toolOffset;

    section[7].positionLeft = (double)property_setSection_position8 + (double)property_setVehicle_toolOffset;
    section[7].positionRight = (double)property_setSection_position9 + (double)property_setVehicle_toolOffset;

    section[8].positionLeft = (double)property_setSection_position9 + (double)property_setVehicle_toolOffset;
    section[8].positionRight = (double)property_setSection_position10 + (double)property_setVehicle_toolOffset;

    section[9].positionLeft = (double)property_setSection_position10 + (double)property_setVehicle_toolOffset;
    section[9].positionRight = (double)property_setSection_position11 + (double)property_setVehicle_toolOffset;

    section[10].positionLeft = (double)property_setSection_position11 + (double)property_setVehicle_toolOffset;
    section[10].positionRight = (double)property_setSection_position12 + (double)property_setVehicle_toolOffset;

    section[11].positionLeft = (double)property_setSection_position12 + (double)property_setVehicle_toolOffset;
    section[11].positionRight = (double)property_setSection_position13 + (double)property_setVehicle_toolOffset;

    section[12].positionLeft = (double)property_setSection_position13 + (double)property_setVehicle_toolOffset;
    section[12].positionRight = (double)property_setSection_position14 + (double)property_setVehicle_toolOffset;

    section[13].positionLeft = (double)property_setSection_position14 + (double)property_setVehicle_toolOffset;
    section[13].positionRight = (double)property_setSection_position15 + (double)property_setVehicle_toolOffset;

    section[14].positionLeft = (double)property_setSection_position15 + (double)property_setVehicle_toolOffset;
    section[14].positionRight = (double)property_setSection_position16 + (double)property_setVehicle_toolOffset;

    section[15].positionLeft = (double)property_setSection_position16 + (double)property_setVehicle_toolOffset;
    section[15].positionRight = (double)property_setSection_position17 + (double)property_setVehicle_toolOffset;
}
