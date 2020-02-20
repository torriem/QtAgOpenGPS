#include "ctool.h"
#include "cvehicle.h"
#include "glm.h"
#include "aogsettings.h"
#include "glutils.h"
#include "ccamera.h"

CTool::CTool()
{
    USE_SETTINGS;

    //from settings grab the vehicle specifics
    toolWidth = SETTINGS_TOOLWIDTH;
    toolOverlap = SETTINGS_TOOLOVERLAP;
    toolOffset = SETTINGS_TOOLOFFSET;

    toolTrailingHitchLength = SETTINGS_TOOLTRAILINGHITCHLENGTH;
    toolTankTrailingHitchLength = SETTINGS_TOOLTANKTRAILINGHITCHLENGTH;
    hitchLength = SETTINGS_HITCHLENGTH;

    isToolBehindPivot = SETTINGS_TOOLISBEHINDPIVOT;
    isToolTrailing = SETTINGS_TOOLISTRAILING;
    isToolTBT = SETTINGS_TOOLISTBT;

    toolLookAhead = SETTINGS_TOOLLOOKAHEAD;
    toolTurnOffDelay = SETTINGS_TOOLTURNOFFDELAY;

    numOfSections = SETTINGS_NUMSECTIONS;
    numSuperSection = numOfSections + 1;

    toolMinUnappliedPixels = SETTINGS_TOOLMINAPPLIED;

    //hack to get around tight coupling for now.
    //TODO, implement wrapper methods and pass in the
    //variables the section methods need.
    //for (int i=0; i <= MAXSECTIONS; i++) {
    //    section[i].set_vehicle(vehicle);
    //}
}

void CTool::drawTool(CVehicle &v, CCamera &camera, QOpenGLFunctions *gl, QMatrix4x4 &mvp)
{
    //translate and rotate at pivot axle, caller's mvp will be changed
    mvp.translate(v.pivotAxlePos.easting, v.pivotAxlePos.northing, 0);

    QMatrix4x4 mvp1 = mvp; //make a copy, simulate PushMatrix
    GLHelperOneColor gldraw;

    //translate down to the hitch pin
    mvp1.translate(sin(v.fixHeading) * hitchLength,
                            cos(v.fixHeading) * hitchLength, 0);

    //settings doesn't change trailing hitch length if set to rigid, so do it here
    double trailingTank, trailingTool;
    if (isToolTrailing)
    {
        trailingTank = toolTankTrailingHitchLength;
        trailingTool = toolTrailingHitchLength;
    }
    else { trailingTank = 0; trailingTool = 0; }

    //there is a trailing tow between hitch
    if (isToolTBT && isToolTrailing)
    {
        //rotate to tank heading
        mvp1.rotate(glm::toDegrees(-v.tankPos.heading), 0.0, 0.0, 1.0);


        //draw the tank hitch
        gldraw.append(QVector3D(0.0, trailingTank, 0.0));
        gldraw.append(QVector3D(0, 0, 0));
        gldraw.draw(gl,mvp1,QColor::fromRgbF(0.7f, 0.7f, 0.97f),GL_LINES, 2.0f);

        //section markers
        gldraw.clear();
        gldraw.append(QVector3D(0.0, trailingTank, 0.0));
        gldraw.draw(gl,mvp1,QColor::fromRgbF(0.95f, 0.95f, 0.0f),GL_POINTS, 6.0f);

        //move down the tank hitch, unwind, rotate to section heading
        mvp1.translate(0.0, trailingTank, 0.0);
        mvp1.rotate(glm::toDegrees(v.tankPos.heading), 0.0, 0.0, 1.0);
        mvp1.rotate(glm::toDegrees(v.toolPos.heading), 0.0, 0.0, 1.0);
    }

    //no tow between hitch
    else
    {
        mvp1.rotate(glm::toDegrees(-v.toolPos.heading), 0.0, 0.0, 1.0);
    }

    //draw the hitch if trailing
    if (isToolTrailing)
    {
        gldraw.clear();
        gldraw.append(QVector3D(0.0, trailingTool, 0.0));
        gldraw.append(QVector3D(0,0,0));
        gldraw.draw(gl,mvp1,QColor::fromRgbF(0.7f, 0.7f, 0.97f),GL_LINES, 2.0f);
    }

    //draw the sections
    GLHelperColors gldrawcolors;
    QColor color;

    //draw section line
    if (section[numOfSections].isSectionOn)
    {
        ColorVertex cv;
        if (section[0].manBtnState == btnStates::Auto) cv.color=QVector4D(0.0f, 0.97f, 0.0f, 1.0);
        else cv.color = QVector4D(0.99, 0.99, 0, 1.0);

        cv.vertex = QVector3D( section[numOfSections].positionLeft, trailingTool, 0);
        gldrawcolors.append(cv);

        cv.vertex = QVector3D( section[numOfSections].positionRight, trailingTool, 0);
        gldrawcolors.append(cv);
    }
    else
    {
        for (int j = 0; j < numOfSections; j++)
        {
            ColorVertex cv;

            //if section is on, green, if off, red color
            if (section[j].isSectionOn)
            {
                if (section[j].manBtnState == btnStates::Auto) cv.color = QVector4D(0.0f, 0.97f, 0.0f, 1.0f);
                else cv.color = QVector4D(0.97, 0.97, 0, 1.0f);
            }
            else
            {
                cv.color = QVector4D(0.97f, 0.2f, 0.2f, 1.0f);
            }

            //draw section line
            cv.vertex = QVector3D(section[j].positionLeft, trailingTool, 0);
            gldrawcolors.append(cv);
            cv.vertex = QVector3D(section[j].positionRight, trailingTool, 0);
            gldrawcolors.append(cv);
        }
    }

    gldrawcolors.draw(gl,mvp1,GL_LINES,8.0f);

    //draw section markers if close enough
    if (camera.camSetDistance > -250)
    {
        gldraw.clear();
        //section markers
        for (int j = 0; j < numOfSections - 1; j++)
            gldraw.append(QVector3D(section[j].positionRight, trailingTool, 0));

        gldraw.draw(gl,mvp1,QColor::fromRgbF(0,0,0),GL_POINTS,3.0f);
    }

    //GL.PopMatrix();
}

//function to calculate the width of each section and update
void CTool::sectionCalcWidths()
{
    for (int j = 0; j < MAXSECTIONS; j++)
    {
        section[j].sectionWidth = (section[j].positionRight - section[j].positionLeft);
        section[j].rpSectionPosition = 200 + (int)(glm::roundAwayFromZero(section[j].positionLeft * 10));
        section[j].rpSectionWidth = (int)(glm::roundAwayFromZero(section[j].sectionWidth * 10));
    }

    //calculate tool width based on extreme right and left values
    toolWidth = fabs(section[0].positionLeft) + fabs(section[numOfSections - 1].positionRight);

    //left and right tool position
    toolFarLeftPosition = section[0].positionLeft;
    toolFarRightPosition = section[numOfSections - 1].positionRight;

    //now do the full width section
    section[numOfSections].sectionWidth = toolWidth;
    section[numOfSections].positionLeft = toolFarLeftPosition;
    section[numOfSections].positionRight = toolFarRightPosition;

    //find the right side pixel position
    rpXPosition = 200 + (int)(glm::roundAwayFromZero(toolFarLeftPosition * 10));
    rpWidth = (int)(glm::roundAwayFromZero(toolWidth * 10));
}
