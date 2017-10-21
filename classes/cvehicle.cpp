#include "cvehicle.h"
#include "formgps.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions_2_1>
#include <QOpenGLFunctions>
#include <QRgb>
#include "aogsettings.h"
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
    AOGSettings s;
    //from settings grab the vehicle specifics
    toolWidth = s.value("vehicle/toolWidth", DEFAULT_TOOLWIDTH).toDouble();
    toolOverlap = s.value("vehicle/toolOverlap", DEFAULT_TOOLOVERLAP).toDouble();
    toolTrailingHitchLength = s.value("vehicle/toolTrailingHitchLength",
                                      DEFAULT_TOOLTRAILINGHITCHLENGTH).toDouble();
    tankTrailingHitchLength = s.value("vehicle/tankTrailingHitchLength",
                                      DEFAULT_TANKTRAILINGHITCHLENGTH).toDouble();
    toolOffset = s.value("vehicle/toolOffset",DEFAULT_TOOLOFFSET).toDouble();

    isToolBehindPivot = s.value("vehicle/isToolBehindPivot",
                                DEFAULT_ISTOOLBEHINDPIVOT).toBool();
    isToolTrailing = s.value("vehicle/isToolTrailing",
                             DEFAULT_ISTOOLTRAILING).toBool();

    isPivotBehindAntenna = s.value("vehicle/isPivotBehindAntenna",
                                   DEFAULT_ISPIVOTBEHINDANTENNA).toBool();
    antennaHeight = s.value("vehicle/antennaHeight",DEFAULT_ANTENNAHEIGHT).toDouble();
    antennaPivot = s.value("vehicle/antennaPivot",DEFAULT_ANTENNAPIVOT).toDouble();
    hitchLength = s.value("vehicle/hitchLength",DEFAULT_HITCHLENGTH).toDouble();

    wheelbase = s.value("vehicle/wheelbase",DEFAULT_WHEELBASE).toDouble();
    isSteerAxleAhead = s.value("vehicle/isSteerAxleAhead",
                               DEFAULT_ISSTEERINGAXLEAHEAD).toBool();

    toolLookAhead = s.value("vehicle/lookAhead",DEFAULT_LOOKAHEAD).toDouble();
    toolTurnOffDelay = s.value("vehicle/turnOffDelay",DEFAULT_TURNOFFDELAY).toDouble();

    numOfSections = s.value("vehicle/numSections",DEFAULT_NUMSECTIONS).toInt();
    numSuperSection = numOfSections+1;

    slowSpeedCutoff = s.value("vehicle/slowSpeedCutoff",DEFAULT_SLOWSPEEDCUTOFF).toDouble();
    toolMinUnappliedPixels = s.value("vehicle/minApplied",DEFAULT_MINAPPLIED).toInt();

    goalPointLookAhead = s.value("vehicle/goalPointLookAhead",
                                 DEFAULT_GOALPOINTLOOKAHEAD).toDouble();
    maxAngularVelocity = s.value("vehicle/maxAngularVelocity",
                                 DEFAULT_MAXANGULARVELOCITY).toDouble();
    maxSteerAngle = s.value("vehicle/maxSteerAngle",
                            DEFAULT_MAXSTEERINGANGLE).toDouble();
}

void CVehicle::drawVehicle(QOpenGLContext *glContext, QMatrix4x4 &modelview,
                           const QMatrix4x4 &projection) {
    QOpenGLFunctions *gl =glContext->functions();
    QOpenGLFunctions_2_1 *gl21 = glContext->versionFunctions<QOpenGLFunctions_2_1>();

    QVector<QVector2D> v;
    QColor color;

    //translate and rotate at pivot axle
    //this will change the modelview for our caller, which is what
    //was happening here.
    gl21->glTranslated(mf->fixEasting, mf->fixNorthing, 0);
    modelview.translate(mf->fixEasting, mf->fixNorthing, 0);

    //gl->glPushMatrix();
    QMatrix4x4 mvTool = modelview;

    //most complicated translate ever!
    //gl->glTranslated((sin(mf->fixHeading) * (hitchLength - antennaPivot)),
    //                (cos(mf->fixHeading) * (hitchLength - antennaPivot)), 0);
    mvTool.translate((sin(mf->fixHeading) * (hitchLength - antennaPivot)),
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
        //gl->glRotated(toDegrees(-mf->fixHeadingTank), 0.0, 0.0, 1.0);
        mvTool.rotate(toDegrees(-mf->fixHeadingTank), 0.0, 0.0, 1.0);

        //draw the tank hitch
        gl->glLineWidth(2);

        //gl->glColor3f(0.7f, 0.7f, 0.97f);
        color.setRgbF(0.7f,0.7f, 0.97f);
        //gl->glBegin(GL_LINES);
        //gl->glVertex3d(0, trailingTank, 0);
        //gl->glVertex3d(0, 0, 0);
        //gl->glEnd();
        v.clear();
        v.append(QVector2D(0,trailingTank));
        v.append(QVector2D(0,0));
        mf->glDrawArraysWrapper(gl,projection*mvTool,
                                GL_LINES, color,
                                v.data(),GL_FLOAT,
                                2*sizeof(QVector2D),
                                2);


        //section markers
        //gl->glColor3f(0.95f, 0.950f, 0.0f);
        color.setRgbF(0.95f, 0.950f, 0.0f);
        //gl->glPointSize(6.0f);
        //gl->glBegin(GL_POINTS);
        //gl->glVertex3d(0, trailingTank, 0);
        //gl->glEnd();
        v.clear();
        v.append(QVector2D(0,trailingTank));
        mf->glDrawArraysWrapper(gl,projection*mvTool,
                                GL_POINTS,color,
                                v.data(),GL_FLOAT,
                                2*sizeof(QVector2D),
                                1);



        //move down the tank hitch, unwind, rotate to section heading
        //gl->glTranslated(0, trailingTank, 0);
        //gl->glRotated(toDegrees(mf->fixHeadingTank), 0.0, 0.0, 1.0);
        //gl->glRotated(toDegrees(-mf->fixHeadingSection), 0.0, 0.0, 1.0);

        mvTool.translate(0, trailingTank, 0);
        mvTool.rotate(toDegrees(mf->fixHeadingTank), 0.0, 0.0, 1.0);
        mvTool.rotate(toDegrees(-mf->fixHeadingSection), 0.0, 0.0, 1.0);
    }
    //no tow between hitch
    else {
        //gl->glRotated(toDegrees(-mf->fixHeadingSection), 0.0, 0.0, 1.0);
        mvTool.rotate(toDegrees(-mf->fixHeadingSection), 0.0, 0.0, 1.0);
    }

    //draw the hitch if trailing
    if (isToolTrailing)
    {
        gl->glLineWidth(2);
        //gl->glColor3f(0.7f, 0.7f, 0.97f);
        color.setRgbF(0.7f, 0.7f, 0.97f);
        //gl->glBegin(GL_LINES);
        //gl->glVertex3d(0, trailingTool, 0);
        //gl->glVertex3d(0, 0, 0);
        //gl->glEnd();
        v.clear();
        v.append(QVector2D(0,trailingTool));
        v.append(QVector2D(0,0));
        mf->glDrawArraysWrapper(gl,projection*mvTool,
                                GL_LINES, color,
                                v.data(),GL_FLOAT,
                                2*sizeof(QVector2D),
                                2);

    }

    //draw the sections
    gl->glLineWidth(8);

    //gl->glBegin(GL_LINES);
    //draw section line.  This is the single big section
    if (mf->section[numOfSections].isSectionOn)
    {
        if (mf->section[0].manBtnState == btnStates::Auto)
            //gl->glColor3f(0.0f, 0.97f, 0.0f);
            color.setRgbF(0.0f,0.97f,0.0f);
        else
            //gl->glColor3f(0.99, 0.99, 0);
            color.setRgbF(0.99f, 0.99f, 0.0f);

        //gl->glVertex3d(mf->section[numOfSections].positionLeft, trailingTool, 0);
        //gl->glVertex3d(mf->section[numOfSections].positionRight, trailingTool, 0);
        v.clear();
        v.append(QVector2D(mf->section[numOfSections].positionLeft, trailingTool));
        v.append(QVector2D(mf->section[numOfSections].positionRight, trailingTool));
        mf->glDrawArraysWrapper(gl,projection*mvTool,
                                GL_LINES,color,
                                v.data(),GL_FLOAT,
                                2*sizeof(QVector2D),
                                2);
    } else {
        for (int j = 0; j < numOfSections; j++)
        {
            //if section is on, green, if off, red color
            if (mf->section[j].isSectionOn)
            {
                if (mf->section[j].manBtnState == btnStates::Auto)
                    //gl->glColor3f(0.0f, 0.97f, 0.0f);
                    color.setRgbF(0.0f, 0.97f, 0.0f);
                else
                    //gl->glColor3f(0.97, 0.97, 0);
                    color.setRgbF(0.97f, 0.97f, 0.0f);
            } else
                //gl->glColor3f(0.97f, 0.2f, 0.2f);
                color.setRgbF(0.97f, 0.2f, 0.2f);

            //draw section line
            //gl->glVertex3d(mf->section[j].positionLeft, trailingTool, 0);
            //gl->glVertex3d(mf->section[j].positionRight, trailingTool, 0);
            v.clear();
            v.append(QVector2D(mf->section[j].positionLeft, trailingTool));
            v.append(QVector2D(mf->section[j].positionRight, trailingTool));
            mf->glDrawArraysWrapper(gl,projection*mvTool,
                                GL_LINES,color,
                                v.data(),GL_FLOAT,
                                2*sizeof(QVector2D),
                                2);
        }
    }
    //gl->glEnd();


    //draw section markers if close enough
    if (mf->camera.camSetDistance > -1500)
    {
        //gl->glColor3f(0.0f, 0.0f, 0.0f);
        color.setRgbF(0.0f, 0.0f, 0.0f);

        //section markers
        //TODO: this needs to go in the shder
        //gl->glPointSize(4.0f);

        //gl->glBegin(GL_POINTS);
        v.clear();
        for (int j = 0; j < numOfSections - 1; j++)
            //gl->glVertex3d(mf->section[j].positionRight, trailingTool, 0);
            v.append(QVector2D(mf->section[j].positionRight, trailingTool));

        mf->glDrawArraysWrapper(gl,projection*mvTool,
                            GL_POINTS,color,
                            v.data(),GL_FLOAT,
                            2*sizeof(QVector2D),
                            numOfSections - 1,
                            4.0f);
        //gl->glEnd();
    }

    //draw vehicle
    //gl21->glPopMatrix();

    gl21->glRotated(toDegrees(-mf->fixHeading), 0.0, 0.0, 1.0);
    modelview.rotate(toDegrees(-mf->fixHeading), 0.0, 0.0, 1.0);

    //draw the vehicle Body
    gl21->glColor3f(0.9, 0.5, 0.30);
    gl21->glBegin(GL_TRIANGLE_FAN);

    gl21->glVertex3d(0, 0, -0.2);
    gl21->glVertex3d(1.8, -antennaPivot, 0.0);
    gl21->glVertex3d(0, -antennaPivot + wheelbase, 0.0);
    gl21->glColor3f(0.20, 0.0, 0.9);
    gl21->glVertex3d(-1.8, -antennaPivot, 0.0);
    gl21->glVertex3d(1.8, -antennaPivot, 0.0);
    gl21->glEnd();

    //draw the area side marker
    gl21->glColor3f(0.95f, 0.90f, 0.0f);
    gl21->glPointSize(4.0f);
    gl21->glBegin(GL_POINTS);
    if (mf->isAreaOnRight) gl21->glVertex3d(2.0, -antennaPivot, 0);
    else gl21->glVertex3d(-2.0, -antennaPivot, 0);

    //antenna
    gl21->glColor3f(0.0f, 0.98f, 0.0f);
    gl21->glVertex3d(0, 0, 0);

    //hitch pin
    gl21->glColor3f(0.99f, 0.0f, 0.0f);
    gl21->glVertex3d(0, hitchLength - antennaPivot, 0);

    ////rear Tires
    //glPointSize(12.0f);
    //glColor3f(0, 0, 0);
    //glVertex3d(-1.8, 0, -antennaPivot);
    //glVertex3d(1.8, 0, -antennaPivot);
    gl21->glEnd();

    gl21->glLineWidth(1);
    gl21->glColor3f(0.9, 0.95, 0.10);
    gl21->glBegin(GL_LINE_STRIP);
    {
        gl21->glVertex3d(1.2, -antennaPivot + wheelbase + 5, 0.0);
        gl21->glVertex3d(0, -antennaPivot + wheelbase + 10, 0.0);
        gl21->glVertex3d(-1.2, -antennaPivot + wheelbase + 5, 0.0);
    }
    gl21->glEnd();

    //draw the rigid hitch
    gl21->glColor3f(0.37f, 0.37f, 0.97f);
    gl21->glBegin(GL_LINES);
    gl21->glVertex3d(0, hitchLength - antennaPivot, 0);
    gl21->glVertex3d(0, -antennaPivot, 0);
    gl21->glEnd();
}


