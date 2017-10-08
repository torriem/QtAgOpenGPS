#include "cvehicle.h"
#include "formgps.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions_1_1>
#include <QSettings>
#include "csection.h"
#include "glm.h"

//TODO: move these to a central location
#define DEFAULT_TOOLWIDTH 16.0
#define DEFAULT_TOOLOVERLAP 0.1
#define DEFAULT_TOOLTRAILINGHITCHLENGTH -2.0
#define DEFAULT_TANKTRAILINGHITCHLENGTH -1.5
#define DEFAULT_TOOLOFFSET 0.0
#define DEFAULT_ISTOOLBEHINDPIVOT true
#define DEFAULT_ISTOOLTRAILING true
#define DEFAULT_ISPIVOTBEHINDANTENNA true
#define DEFAULT_ANTENNAHEIGHT 3.0
#define DEFAULT_ANTENNAPIVOT 1.1
#define DEFAULT_HITCHLENGTH -0.5
#define DEFAULT_WHEELBASE 5.0
#define DEFAULT_ISSTEERINGAXLEAHEAD true
#define DEFAULT_LOOKAHEAD 2.0
#define DEFAULT_TURNOFFDELAY 1.0
#define DEFAULT_NUMSECTIONS 3
#define DEFAULT_SLOWSPEEDCUTOFF 2.0
#define DEFAULT_MINAPPLIED 30
#define DEFAULT_GOALPOINTLOOKAHEAD 3.0
#define DEFAULT_MAXANGULARVELOCITY 1.0
#define DEFAULT_MAXSTEERINGANGLE 20.0


CVehicle::CVehicle(FormGPS *mf)
    :mf(mf)
{
    QSettings s;
    //from settings grab the vehicle specifics
    toolWidth = s.value("setVehicle_toolWidth", DEFAULT_TOOLWIDTH).toDouble();
    toolOverlap = s.value("setVehicle_toolOverlap", DEFAULT_TOOLOVERLAP).toDouble();
    toolTrailingHitchLength = s.value("setVehicle_toolTrailingHitchLength",
                                      DEFAULT_TOOLTRAILINGHITCHLENGTH).toDouble();
    tankTrailingHitchLength = s.value("setVehicle_tankTrailingHitchLength",
                                      DEFAULT_TANKTRAILINGHITCHLENGTH).toDouble();
    toolOffset = s.value("setVehicle_toolOffset",DEFAULT_TOOLOFFSET).toDouble();

    isToolBehindPivot = s.value("setVehicle_isToolBehindPivot",
                                DEFAULT_ISTOOLBEHINDPIVOT).toBool();
    isToolTrailing = s.value("setVehicle_isToolTrailing",
                             DEFAULT_ISTOOLTRAILING).toBool();

    isPivotBehindAntenna = s.value("setVehicle_isPivotBehindAntenna",
                                   DEFAULT_ISPIVOTBEHINDANTENNA).toBool();
    antennaHeight = s.value("setVehicle_antennaHeight",DEFAULT_ANTENNAHEIGHT).toDouble();
    antennaPivot = s.value("setVehicle_antennaPivot",DEFAULT_ANTENNAPIVOT).toDouble();
    hitchLength = s.value("setVehicle_hitchLength",DEFAULT_HITCHLENGTH).toDouble();

    wheelbase = s.value("setVehicle_wheelbase",DEFAULT_WHEELBASE).toDouble();
    isSteerAxleAhead = s.value("setVehicle_isSteerAxleAhead",
                               DEFAULT_ISSTEERINGAXLEAHEAD).toBool();

    toolLookAhead = s.value("setVehicle_lookAhead",DEFAULT_LOOKAHEAD).toDouble();
    toolTurnOffDelay = s.value("setVehicle_turnOffDelay",DEFAULT_TURNOFFDELAY).toDouble();

    numOfSections = s.value("setVehicle_numSections",DEFAULT_NUMSECTIONS).toInt();
    numSuperSection = numOfSections+1;

    slowSpeedCutoff = s.value("setVehicle_slowSpeedCutoff",DEFAULT_SLOWSPEEDCUTOFF).toDouble();
    toolMinUnappliedPixels = s.value("setVehicle_minApplied",DEFAULT_MINAPPLIED).toInt();

    goalPointLookAhead = s.value("setVehicle_goalPointLookAhead",
                                 DEFAULT_GOALPOINTLOOKAHEAD).toDouble();
    maxAngularVelocity = s.value("setVehicle_maxAngularVelocity",
                                 DEFAULT_MAXANGULARVELOCITY).toDouble();
    maxSteerAngle = s.value("setVehicle_maxSteerAngle",
                            DEFAULT_MAXSTEERINGANGLE).toDouble();
}

void CVehicle::drawVehicle(QOpenGLContext *glContext) {
    QOpenGLFunctions_1_1 *gl = glContext->versionFunctions<QOpenGLFunctions_1_1>();

    //translate and rotate at pivot axle
    gl->glTranslated(mf->fixEasting, mf->fixNorthing, 0);
    gl->glPushMatrix();

    //most complicated translate ever!
    gl->glTranslated((sin(mf->fixHeading) * (hitchLength - antennaPivot)),
                    (cos(mf->fixHeading) * (hitchLength - antennaPivot)), 0);

    //settings doesn't change trailing hitch length if set to rigid, so do it here
    double trailingTank, trailingTool;
    if (isToolTrailing)
    {
        trailingTank = tankTrailingHitchLength;
        trailingTool = toolTrailingHitchLength;
    }
    else { trailingTank = 0; trailingTool = 0; }

    //there is a trailing tow between hitch
    if (tankTrailingHitchLength < -2.0 && isToolTrailing)
    {
        gl->glRotated(toDegrees(-mf->fixHeadingTank), 0.0, 0.0, 1.0);

        //draw the tank hitch
        gl->glLineWidth(2);
        gl->glColor3f(0.7f, 0.7f, 0.97f);

        gl->glBegin(GL_LINES);
        gl->glVertex3d(0, trailingTank, 0);
        gl->glVertex3d(0, 0, 0);
        gl->glEnd();

        //section markers
        gl->glColor3f(0.95f, 0.950f, 0.0f);
        gl->glPointSize(6.0f);
        gl->glBegin(GL_POINTS);
        gl->glVertex3d(0, trailingTank, 0);
        gl->glEnd();

        //move down the tank hitch, unwind, rotate to section heading
        gl->glTranslated(0, trailingTank, 0);
        gl->glRotated(toDegrees(mf->fixHeadingTank), 0.0, 0.0, 1.0);
        gl->glRotated(toDegrees(-mf->fixHeadingSection), 0.0, 0.0, 1.0);
    }

    //no tow between hitch
    else gl->glRotated(toDegrees(-mf->fixHeadingSection), 0.0, 0.0, 1.0);

    //draw the hitch if trailing
    if (isToolTrailing)
    {
        gl->glLineWidth(2);
        gl->glColor3f(0.7f, 0.7f, 0.97f);

        gl->glBegin(GL_LINES);
        gl->glVertex3d(0, trailingTool, 0);
        gl->glVertex3d(0, 0, 0);
        gl->glEnd();
    }

    //draw the sections
    gl->glLineWidth(8);
    gl->glBegin(GL_LINES);

    //draw section line
    if (mf->section[numOfSections].isSectionOn)
    {
        if (mf->section[0].manBtnState == manBtn::Auto) gl->glColor3f(0.0f, 0.97f, 0.0f);
        else gl->glColor3f(0.99, 0.99, 0);
        gl->glVertex3d(mf->section[numOfSections].positionLeft, trailingTool, 0);
        gl->glVertex3d(mf->section[numOfSections].positionRight, trailingTool, 0);
    }
    else
        for (int j = 0; j < numOfSections; j++)
        {
            //if section is on, green, if off, red color
            if (mf->section[j].isSectionOn)
            {
                if (mf->section[j].manBtnState == manBtn::Auto) gl->glColor3f(0.0f, 0.97f, 0.0f);
                else gl->glColor3f(0.97, 0.97, 0);
            }
            else gl->glColor3f(0.97f, 0.2f, 0.2f);

            //draw section line
            gl->glVertex3d(mf->section[j].positionLeft, trailingTool, 0);
            gl->glVertex3d(mf->section[j].positionRight, trailingTool, 0);
        }
    gl->glEnd();

    //draw section markers if close enough
    if (mf->camera.camSetDistance > -1500)
    {
        gl->glColor3f(0.0f, 0.0f, 0.0f);
        //section markers
        gl->glPointSize(4.0f);
        gl->glBegin(GL_POINTS);
        for (int j = 0; j < numOfSections - 1; j++)
            gl->glVertex3d(mf->section[j].positionRight, trailingTool, 0);
        gl->glEnd();
    }

    //draw vehicle
    gl->glPopMatrix();
    gl->glRotated(toDegrees(-mf->fixHeading), 0.0, 0.0, 1.0);

    //draw the vehicle Body
    gl->glColor3f(0.9, 0.5, 0.30);
    gl->glBegin(GL_TRIANGLE_FAN);

    gl->glVertex3d(0, 0, -0.2);
    gl->glVertex3d(1.8, -antennaPivot, 0.0);
    gl->glVertex3d(0, -antennaPivot + wheelbase, 0.0);
    gl->glColor3f(0.20, 0.0, 0.9);
    gl->glVertex3d(-1.8, -antennaPivot, 0.0);
    gl->glVertex3d(1.8, -antennaPivot, 0.0);
    gl->glEnd();

    //draw the area side marker
    gl->glColor3f(0.95f, 0.90f, 0.0f);
    gl->glPointSize(4.0f);
    gl->glBegin(GL_POINTS);
    if (mf->isAreaOnRight) gl->glVertex3d(2.0, -antennaPivot, 0);
    else gl->glVertex3d(-2.0, -antennaPivot, 0);

    //antenna
    gl->glColor3f(0.0f, 0.98f, 0.0f);
    gl->glVertex3d(0, 0, 0);

    //hitch pin
    gl->glColor3f(0.99f, 0.0f, 0.0f);
    gl->glVertex3d(0, hitchLength - antennaPivot, 0);

    ////rear Tires
    //glPointSize(12.0f);
    //glColor3f(0, 0, 0);
    //glVertex3d(-1.8, 0, -antennaPivot);
    //glVertex3d(1.8, 0, -antennaPivot);
    gl->glEnd();

    gl->glLineWidth(1);
    gl->glColor3f(0.9, 0.95, 0.10);
    gl->glBegin(GL_LINE_STRIP);
    {
        gl->glVertex3d(1.2, -antennaPivot + wheelbase + 5, 0.0);
        gl->glVertex3d(0, -antennaPivot + wheelbase + 10, 0.0);
        gl->glVertex3d(-1.2, -antennaPivot + wheelbase + 5, 0.0);
    }
    gl->glEnd();

    //draw the rigid hitch
    gl->glColor3f(0.37f, 0.37f, 0.97f);
    gl->glBegin(GL_LINES);
    gl->glVertex3d(0, hitchLength - antennaPivot, 0);
    gl->glVertex3d(0, -antennaPivot, 0);
    gl->glEnd();
}
