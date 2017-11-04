#include <QtGlobal>
#include "cvehicle.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions_2_1>
#include <QOpenGLFunctions>
#include <QRgb>
#include "aogsettings.h"
#include "csection.h"
#include "glm.h"
#include "glutils.h"

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

struct ColorVertex {
    QVector3D vertex;
    QVector4D color;
};



CVehicle::CVehicle()
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

    //hack to get around tight coupling for now.
    //TODO, implement wrapper methods and pass in the
    //variables the section methods need.
    for (int i=0; i <= MAXSECTIONS; i++) {
        section[i].set_vehicle(this);
    }
}

void CVehicle::makeBuffers()
{
    //requires a valid OpenGL context
    QSettings settings;

    QVector<QVector3D> v;
    if (buffersCurrent) return;

    double trailingTank, trailingTool;

    //settings doesn't change trailing hitch length if set to rigid, so do it here
    if (isToolTrailing)
    {
        trailingTank = tankTrailingHitchLength;
        trailingTool = toolTrailingHitchLength;
    }
    else { trailingTank = 0; trailingTool = 0; }

    //there is a trailing tow between hitch
    if (tankTrailingHitchLength < -2.0 && isToolTrailing)
    {
        v.clear();
        v.append(QVector3D(0,trailingTank,0));
        v.append(QVector3D(0,0,0));

        if (trailingTankHitchBuffer.isCreated() ){
            trailingTankHitchBuffer.destroy();
        }
        trailingTankHitchBuffer.create();
        trailingTankHitchBuffer.bind();
        trailingTankHitchBuffer.allocate(v.data(),v.count() * sizeof(QVector3D));
        trailingTankHitchBuffer.release();

        v.clear();
        v.append(QVector3D(0,trailingTank,0));
        if (tankDotBuffer.isCreated())
            tankDotBuffer.destroy();
        tankDotBuffer.create();
        tankDotBuffer.allocate(v.data(),v.count() * sizeof(QVector3D));
        tankDotBuffer.release();
    }

    //draw the hitch if trailing
    if (isToolTrailing)
    {
        v.clear();
        v.append(QVector3D(0,trailingTool,0));
        v.append(QVector3D(0,0,0));

        if(toolHitchBuffer.isCreated())
            toolHitchBuffer.destroy();
        toolHitchBuffer.create();
        toolHitchBuffer.bind();
        toolHitchBuffer.allocate(v.data(),v.count() * sizeof(QVector3D));
        toolHitchBuffer.release();
    }

    //super section
    v.clear();
    v.append(QVector3D(section[numOfSections].positionLeft, trailingTool,0));
    v.append(QVector3D(section[numOfSections].positionRight, trailingTool,0));

    if (superSectionBuffer.isCreated())
        superSectionBuffer.destroy();
    superSectionBuffer.create();
    superSectionBuffer.bind();
    superSectionBuffer.allocate(v.data(),v.count() * sizeof(QVector3D));
    superSectionBuffer.release();

    // individual sections
    for (int j = 0; j < numOfSections; j++)
    {
        v.clear();
        v.append(QVector3D(section[j].positionLeft, trailingTool,0));
        v.append(QVector3D(section[j].positionRight, trailingTool,0));

        if (sectionBuffer[j].isCreated())
            sectionBuffer[j].destroy();
        sectionBuffer[j].create();
        sectionBuffer[j].bind();
        sectionBuffer[j].allocate(v.data(), v.count() * sizeof(QVector3D));
        sectionBuffer[j].release();
    }

    //section dots
    v.clear();
    for (int j = 0; j < numOfSections - 1; j++)
        v.append(QVector3D(section[j].positionRight, trailingTool,0));

    if (sectionDotsBuffer.isCreated())
        sectionDotsBuffer.destroy();
    sectionDotsBuffer.create();
    sectionDotsBuffer.bind();
    sectionDotsBuffer.allocate(v.data(), v.count() * sizeof(QVector3D));
    sectionDotsBuffer.release();

    //draw the vehicle Body
    ColorVertex vb[] = {
        { QVector3D(0,0,-0.2),                QVector4D(0.9, 0.5, 0.30, 1.0) },
        { QVector3D(1.8, -antennaPivot, 0.0), QVector4D(0.9, 0.5, 0.30, 1.0) },
        { QVector3D(0, -antennaPivot + wheelbase, 0.0), QVector4D(0.9, 0.5, 0.30, 1.0) },
        { QVector3D(-1.8, -antennaPivot, 0.0), QVector4D(0.20, 0.0, 0.9, 1.0) },
        { QVector3D(1.8, -antennaPivot, 0.0),  QVector4D(0.20, 0.0, 0.9, 1.0) }
    };

    if (vehicleBodyBuffer.isCreated())
        vehicleBodyBuffer.destroy();
    vehicleBodyBuffer.create();
    vehicleBodyBuffer.bind();
    vehicleBodyBuffer.allocate(vb,5 * sizeof(ColorVertex));
    vehicleBodyBuffer.release();

    //draw the area side marker, antenna, hitch pin ,etc
    ColorVertex m[] = {
        //area side marker
        { QVector3D(), QVector4D(0.95f, 0.90f, 0.0f, 1.0) },
        //antenna
        { QVector3D(0,0,0), QVector4D(0.0f, 0.98f, 0.0f, 1.0) },
        //hitch pin
        { QVector3D(0, hitchLength - antennaPivot, 0), QVector4D(0.99f, 0.0f, 0.0f, 1.0) }
    };
    if (settings.value("vehicle/isAreaOnRight",true).toBool())
        m[0].vertex = QVector3D(2.0, -antennaPivot, 0);
    else
        m[0].vertex = QVector3D(-2.0, -antennaPivot, 0);

    if (pinsAndMarkersBuffer.isCreated())
        pinsAndMarkersBuffer.destroy();
    pinsAndMarkersBuffer.create();
    pinsAndMarkersBuffer.bind();
    pinsAndMarkersBuffer.allocate(m,sizeof(ColorVertex)*3);
    pinsAndMarkersBuffer.release();

    //yellow pointer
    v.clear();
    v.append(QVector3D(1.2, -antennaPivot + wheelbase + 5, 0.0));
    v.append(QVector3D(0, -antennaPivot + wheelbase + 10, 0.0));
    v.append(QVector3D(-1.2, -antennaPivot + wheelbase + 5, 0.0));

    if (pointerBuffer.isCreated())
        pointerBuffer.destroy();
    pointerBuffer.create();
    pointerBuffer.bind();
    pointerBuffer.allocate(v.data(),v.count()*sizeof(QVector3D));
    pointerBuffer.release();

    //draw the rigid hitch

    v.clear();
    v.append(QVector3D(0, hitchLength - antennaPivot, 0));
    v.append(QVector3D(0, -antennaPivot, 0));

    if (rigidHitchBuffer.isCreated())
        rigidHitchBuffer.destroy();
    rigidHitchBuffer.create();
    rigidHitchBuffer.bind();
    rigidHitchBuffer.allocate(v.data(),v.count()*sizeof(QVector3D));
    rigidHitchBuffer.release();


    buffersCurrent = true;
}

void CVehicle::drawVehicle(QOpenGLContext *glContext, QMatrix4x4 &modelview,
                           const QMatrix4x4 &projection, bool drawSectionMarkers) {
    QOpenGLFunctions *gl =glContext->functions();
    //QOpenGLFunctions_2_1 *gl21 = glContext->versionFunctions<QOpenGLFunctions_2_1>();

    QColor color;
    makeBuffers(); //create openGL buffers if necessary.

    //translate and rotate at pivot axle
    //this will change the modelview for our caller, which is what
    //was happening here.
    //gl21->glTranslated(mf->fixEasting, mf->fixNorthing, 0);
    modelview.translate(fixEasting, fixNorthing, 0);

    //gl->glPushMatrix();
    QMatrix4x4 mvTool = modelview;

    //most complicated translate ever!
    //gl->glTranslated((sin(mf->fixHeading) * (hitchLength - antennaPivot)),
    //                (cos(mf->fixHeading) * (hitchLength - antennaPivot)), 0);
    mvTool.translate((sin(fixHeading) * (hitchLength - antennaPivot)),
                     (cos(fixHeading) * (hitchLength - antennaPivot)), 0);

    //settings doesn't change trailing hitch length if set to rigid, so do it here
    double trailingTank;
    if (isToolTrailing)
    {
        trailingTank = tankTrailingHitchLength;
    }
    else { trailingTank = 0;  }

    //there is a trailing tow between hitch
    if (tankTrailingHitchLength < -2.0 && isToolTrailing)
    {
        //gl->glRotated(toDegrees(-mf->fixHeadingTank), 0.0, 0.0, 1.0);
        mvTool.rotate(toDegrees(-fixHeadingTank), 0.0, 0.0, 1.0);

        //draw the tank hitch
        gl->glLineWidth(2);

        //gl->glColor3f(0.7f, 0.7f, 0.97f);
        color.setRgbF(0.7f,0.7f, 0.97f);
        //gl->glBegin(GL_LINES);
        //gl->glVertex3d(0, trailingTank, 0);
        //gl->glVertex3d(0, 0, 0);
        //gl->glEnd();
        glDrawArraysColor(gl,projection*mvTool,
                          GL_LINES, color,
                          trailingTankHitchBuffer,GL_FLOAT,
                          2);


        //gl->glColor3f(0.95f, 0.950f, 0.0f);
        color.setRgbF(0.95f, 0.950f, 0.0f);
        //gl->glPointSize(6.0f);
        //gl->glBegin(GL_POINTS);
        //gl->glVertex3d(0, trailingTank, 0);
        //gl->glEnd();
        glDrawArraysColor(gl,projection*mvTool,
                          GL_POINTS,color,
                          tankDotBuffer, GL_FLOAT,
                          1, 6.0f);



        //move down the tank hitch, unwind, rotate to section heading
        //gl->glTranslated(0, trailingTank, 0);
        //gl->glRotated(toDegrees(mf->fixHeadingTank), 0.0, 0.0, 1.0);
        //gl->glRotated(toDegrees(-mf->fixHeadingSection), 0.0, 0.0, 1.0);

        mvTool.translate(0, trailingTank, 0);
        mvTool.rotate(toDegrees(fixHeadingTank), 0.0, 0.0, 1.0);
        mvTool.rotate(toDegrees(-fixHeadingSection), 0.0, 0.0, 1.0);
    }
    //no tow between hitch
    else {
        //gl->glRotated(toDegrees(-mf->fixHeadingSection), 0.0, 0.0, 1.0);
        mvTool.rotate(toDegrees(-fixHeadingSection), 0.0, 0.0, 1.0);
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
        glDrawArraysColor(gl,projection*mvTool,
                          GL_LINES, color,
                          toolHitchBuffer,GL_FLOAT,
                          2);

    }

    //draw the sections
    gl->glLineWidth(8);

    //gl->glBegin(GL_LINES);
    //draw section line.  This is the single big section
    if (section[numOfSections].isSectionOn)
    {
        if (section[0].manBtnState == btnStates::Auto)
            //gl->glColor3f(0.0f, 0.97f, 0.0f);
            color.setRgbF(0.0f,0.97f,0.0f);
        else
            //gl->glColor3f(0.99, 0.99, 0);
            color.setRgbF(0.99f, 0.99f, 0.0f);

        //gl->glVertex3d(mf->section[numOfSections].positionLeft, trailingTool, 0);
        //gl->glVertex3d(mf->section[numOfSections].positionRight, trailingTool, 0);
        glDrawArraysColor(gl,projection*mvTool,
                          GL_LINES,color,
                          superSectionBuffer,GL_FLOAT,
                          2);
    } else {
        for (int j = 0; j < numOfSections; j++)
        {
            //if section is on, green, if off, red color
            if (section[j].isSectionOn)
            {
                if (section[j].manBtnState == btnStates::Auto)
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
            glDrawArraysColor(gl,projection*mvTool,
                              GL_LINES,color,
                              sectionBuffer[j],GL_FLOAT,
                              2);
        }
    }
    //gl->glEnd();


    //draw section markers if close enough
    if (drawSectionMarkers)
    {
        //gl->glColor3f(0.0f, 0.0f, 0.0f);
        color.setRgbF(0.0f, 0.0f, 0.0f);

        //section markers
        //TODO: this needs to go in the shder
        //gl->glPointSize(4.0f);

        //gl->glBegin(GL_POINTS);
        glDrawArraysColor(gl,projection*mvTool,
                          GL_POINTS,color,
                          sectionDotsBuffer,GL_FLOAT,
                          numOfSections - 1,
                          4.0f);
        //gl->glEnd();
    }

    //draw vehicle
    //gl21->glPopMatrix();

    //gl21->glRotated(toDegrees(-mf->fixHeading), 0.0, 0.0, 1.0);
    modelview.rotate(toDegrees(-fixHeading), 0.0, 0.0, 1.0);

    //draw the vehicle Body
    //gl21->glColor3f(0.9, 0.5, 0.30);
    //gl21->glBegin(GL_TRIANGLE_FAN);

    //gl21->glVertex3d(0, 0, -0.2);
    //gl21->glVertex3d(1.8, -antennaPivot, 0.0);
    //gl21->glVertex3d(0, -antennaPivot + wheelbase, 0.0);
    //gl21->glColor3f(0.20, 0.0, 0.9);
    //gl21->glVertex3d(-1.8, -antennaPivot, 0.0);
    //gl21->glVertex3d(1.8, -antennaPivot, 0.0);
    //gl21->glEnd();
    glDrawArraysColors(gl,projection*modelview,
                       GL_TRIANGLE_FAN,
                       vehicleBodyBuffer,
                       GL_FLOAT,
                       5); //5 vertices in body

    /*
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
    */
    //draw the area side marker, antenna, hitch pin ,etc
    glDrawArraysColors(gl,projection*modelview,
                       GL_POINTS,
                       pinsAndMarkersBuffer,
                       GL_FLOAT,
                       3,
                       4.0f);

    //gl21->glColor3f(0.9, 0.95, 0.10);
    //gl21->glBegin(GL_LINE_STRIP);
    //{
    //    gl21->glVertex3d(1.2, -antennaPivot + wheelbase + 5, 0.0);
    //    gl21->glVertex3d(0, -antennaPivot + wheelbase + 10, 0.0);
    //    gl21->glVertex3d(-1.2, -antennaPivot + wheelbase + 5, 0.0);
    //}
    //gl21->glEnd();

    gl->glLineWidth(1);
    color.setRgbF(0.9, 0.95, 0.10);
    glDrawArraysColor(gl, projection * modelview,
                      GL_LINE_STRIP, color,
                      pointerBuffer,
                      GL_FLOAT,
                      3);



    //draw the rigid hitch
    //gl21->glColor3f(0.37f, 0.37f, 0.97f);
    //gl21->glBegin(GL_LINES);
    //gl21->glVertex3d(0, hitchLength - antennaPivot, 0);
    //gl21->glVertex3d(0, -antennaPivot, 0);
    //gl21->glEnd();
    color.setRgbF(0.37f, 0.37f, 0.97f);
    glDrawArraysColor(gl, projection * modelview,
                      GL_LINES, color,
                      rigidHitchBuffer,
                      GL_FLOAT,
                      2);
}

void CVehicle::destroyGLBuffers()
{
    //assume valid OpenGL context

    trailingTankHitchBuffer.destroy();
    tankDotBuffer.destroy();
    toolHitchBuffer.destroy();
    superSectionBuffer.destroy();

    for (int j = 0; j <= MAXSECTIONS; j++)
        sectionBuffer[j].destroy();

    sectionDotsBuffer.destroy();
    vehicleBodyBuffer.destroy();
    pinsAndMarkersBuffer.destroy();
    pointerBuffer.destroy();
    rigidHitchBuffer.destroy();
}

void CVehicle::settingsChanged()
{
    //regenerate buffers since settings have changed.
    buffersCurrent = false;
}

