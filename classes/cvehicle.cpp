#include <QtGlobal>
#include "cvehicle.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QRgb>
#include "aogsettings.h"
#include "ccamera.h"
#include "ctool.h"
#include "cboundary.h"
#include "glm.h"
#include "glutils.h"
#include "cnmea.h"
#include "chead.h"

CVehicle::CVehicle()
{
    USE_SETTINGS;

    isPivotBehindAntenna = SETTINGS_VEHICLE_ISPIVOTBEHINDANTENNA;
    antennaHeight = SETTINGS_VEHICLE_ANTENNAHEIGHT;
    antennaPivot = SETTINGS_VEHICLE_ANTENNAPIVOT;
    antennaOffset = SETTINGS_VEHICLE_ANTENNAOFFSET;

    wheelbase = SETTINGS_VEHICLE_WHEELBASE;
    minTurningRadius = SETTINGS_VEHICLE_MINTURNINGRADIUS;
    isSteerAxleAhead = SETTINGS_VEHICLE_ISSTEERAXLEAHEAD;

    slowSpeedCutoff = SETTINGS_VEHICLE_SLOWSPEEDCUTOFF;

    goalPointLookAheadSeconds = SETTINGS_VEHICLE_LOOKAHEAD;
    goalPointLookAheadMinimumDistance = SETTINGS_VEHICLE_LOOKAHEADMINIMUM;
    goalPointDistanceMultiplier = SETTINGS_VEHICLE_DISTANCEMULTIPLIER;
    goalPointLookAheadUturnMult = SETTINGS_VEHICLE_LOOKAHEADUTURNMULT;

    isStanleyUsed = SETTINGS_VEHICLE_ISSTANLEYUSED;
    stanleyGain = SETTINGS_VEHICLE_STANLEY_GAIN;
    stanleyHeadingErrorGain = SETTINGS_VEHICLE_STANLEYHEADINGERRORGAIN;

    maxAngularVelocity = SETTINGS_VEHICLE_MAXANGULARVELOCITY;
    maxSteerAngle = SETTINGS_VEHICLE_MAXSTEERANGLE;

    isHydLiftOn = SETTINGS_VEHICLE_ISHYDLIFTON;

    //treeSpacing = Properties.Settings.Default.setDistance_TreeSpacing;
    treeSpacing = 0;

    vehicleType = SETTINGS_VEHICLE_VEHICLETYPE;

    hydLiftLookAhead = SETTINGS_VEHICLE_HYDLIFTLOOKAHEAD;

    //zero out all ten slots of avgSpeed[]
    memset(avgSpeed,0,sizeof(double)*10);
}

//called from various Classes, always needs current speed
double CVehicle::updateGoalPointDistance(CNMEA &pn, double distanceFromCurrentLine)
{
    //how far should goal point be away  - speed * seconds * kmph -> m/s then limit min value
    double goalPointDistance = pn.speed * goalPointLookAheadSeconds * 0.27777777;

    if (distanceFromCurrentLine < 1.0)
        goalPointDistance += distanceFromCurrentLine * goalPointDistance * goalPointDistanceMultiplier;
    else
        goalPointDistance += goalPointDistance * goalPointDistanceMultiplier;

    if (goalPointDistance < goalPointLookAheadMinimumDistance) goalPointDistance = goalPointLookAheadMinimumDistance;

    emit lookaheadGoal(goalPointDistance);
    //mf.lookaheadActual = goalPointDistance;

    return goalPointDistance;

}

void CVehicle::drawVehicle(QOpenGLFunctions *gl, QMatrix4x4 &mvp,
                           CCamera &camera, CTool &tool,
                           CBoundary &bnd, CHead &hd)
{
    //draw vehicle
    mvp.rotate(glm::toDegrees(-fixHeading), 0.0, 0.0, 1.0);

    GLHelperColors glcolors;
    GLHelperOneColor gldraw;
    ColorVertex cv;


    //antenna
    cv.color = QVector4D(0.0f, 0.95f, 0.95f, 1.0f);
    cv.vertex = QVector3D(0, antennaPivot, 0);
    glcolors.append(cv);

    //hitch pin
    cv.color = QVector4D(0.95f, 0.0f, 0.0f, 1.0f);
    cv.vertex = QVector3D(0, tool.hitchLength, 0);
    glcolors.append(cv);

    glcolors.draw(gl,mvp, GL_POINTS, 6.0f);

    ////draw the vehicle Body

    if (!hd.isOn)
    {
        glcolors.clear();

        //GL.Begin(PrimitiveType.TriangleFan);
        cv.color = QVector4D(0.9, 0.90, 0.0,1.0f);
        cv.vertex = QVector3D(0, antennaPivot, -0.0);
        glcolors.append(cv);
        cv.vertex = QVector3D(1.0, -0, 0.0);
        glcolors.append(cv);
        cv.color = QVector4D(0.0, 0.90, 0.92, 1.0f);
        cv.vertex = QVector3D(0, wheelbase, 0.0);
        glcolors.append(cv);
        cv.color = QVector4D(0.920, 0.0, 0.9, 1.0f);
        cv.vertex = QVector3D(-1.0, -0, 0.0);
        glcolors.append(cv);
        cv.vertex = QVector3D(1.0, -0, 0.0);
        glcolors.append(cv);
        glcolors.draw(gl,mvp,GL_TRIANGLE_FAN,1.0f);
    }
    else
    {
        if (hd.isToolUp)
        {
            gldraw.append(QVector3D(0, antennaPivot, -0.0));
            gldraw.append(QVector3D(1.0, -0, 0.0));
            gldraw.append(QVector3D(0, wheelbase, 0.0));
            gldraw.append(QVector3D(-1.0, -0, 0.0));
            gldraw.append(QVector3D(1.0, -0, 0.0));
            gldraw.draw(gl,mvp,QColor::fromRgbF(0.95, 0.0, 0.0),
                        GL_TRIANGLE_FAN,1.0f);
        }
        else
        {
            gldraw.append(QVector3D(0, antennaPivot, -0.0));
            gldraw.append(QVector3D(1.0, -0, 0.0));
            gldraw.append(QVector3D(0, wheelbase, 0.0));
            gldraw.append(QVector3D(-1.0, -0, 0.0));
            gldraw.append(QVector3D(1.0, -0, 0.0));
            gldraw.draw(gl,mvp,QColor::fromRgbF(0.0, 0.950, 0.0),
                        GL_TRIANGLE_FAN,1.0f);
        }
    }

    gl->glLineWidth(3);
    gldraw.clear();

    gldraw.append(QVector3D(-1.0, 0, 0));
    gldraw.append(QVector3D(1.0, 0, 0));
    gldraw.append(QVector3D(0, wheelbase, 0));

    gldraw.draw(gl,mvp,QColor::fromRgbF(0,0,0),GL_LINE_LOOP,3);

    if (bnd.isBndBeingMade)
    {
        glcolors.clear();

        gl->glLineWidth(2);
        cv.color = QVector4D(0.0, 0.970, 0.0, 1.0);

        if (bnd.isDrawRightSide)
        {
            cv.vertex = QVector3D(0.0, 0, 0);
            glcolors.append(cv);

            cv.color = QVector4D(0.970, 0.920, 0.0, 1.0);
            cv.vertex = QVector3D(bnd.createBndOffset, 0, 0);
            glcolors.append(cv);

            cv.vertex = QVector3D(bnd.createBndOffset*0.75, 0.25, 0);
            glcolors.append(cv);

            glcolors.draw(gl,mvp,GL_LINE_STRIP, 2);
        }

        //draw on left side
        else
        {
            cv.vertex = QVector3D(0.0, 0, 0);
            glcolors.append(cv);

            cv.color = QVector4D(0.970, 0.920, 0.0, 1.0);
            cv.vertex = QVector3D(-bnd.createBndOffset, 0, 0);
            glcolors.append(cv);

            cv.vertex = QVector3D(-bnd.createBndOffset*0.75, 0.25, 0);
            glcolors.append(cv);

            glcolors.draw(gl,mvp,GL_LINE_STRIP, 2);
        }
    }

    /* TODO: implement font  texture and subroutines for text
    if (mf.curve.isBtnCurveOn && !mf.ct.isContourBtnOn)
    {
        GL.Color4(0.969, 0.95, 0.9510, 0.87);
        mf.font.DrawTextVehicle(0, wheelbase, mf.curve.curveNumber.ToString(), 1.5);
    }
    else if (mf.ABLine.isBtnABLineOn && !mf.ct.isContourBtnOn)
    {
        GL.Color4(0.96, 0.95, 0.9510, 0.87);
        mf.font.DrawTextVehicle(0, wheelbase, mf.ABLine.passNumber.ToString(), 1.5);
    }
    */

    //draw the rigid hitch
    gldraw.clear();
    gldraw.append(QVector3D(0, tool.hitchLength, 0));
    gldraw.append(QVector3D(0, 0, 0));
    gldraw.draw(gl,mvp,QColor::fromRgbF(0.37f, 0.37f, 0.97f),
                GL_LINES, 2);

    gl->glLineWidth(1);

    if (camera.camSetDistance < -1000)
    {
        double theta = glm::twoPI / 20;
        double c = cos(theta);//precalculate the sine and cosine
        double s = sin(theta);

        double x = camera.camSetDistance * -.015;//we start at angle = 0
        double y = 0;

        gldraw.clear();
        gldraw.append(QVector3D(x, y, 0.0));
        for (int ii = 0; ii < 20; ii++)
        {
            //output vertex
            gldraw.append(QVector3D(x, y, 0.0));

            //apply the rotation matrix
            double t = x;
            x = (c * x) - (s * y);
            y = (s * t) + (c * y);
            // GL.Vertex3(x, y, 0.0);
        }
        gldraw.draw(gl,mvp,QColor::fromRgbF(0.5f, 0.5f, 0.9f, 0.20),
                    GL_TRIANGLE_FAN, 1);

        gldraw.clear();

        gl->glLineWidth(1);

        for (int ii = 0; ii < 20; ii++)
        {
            //output vertex
            gldraw.append(QVector3D(x, y, 0.0));

            //apply the rotation matrix
            double t = x;
            x = (c * x) - (s * y);
            y = (s * t) + (c * y);
            // GL.Vertex3(x, y, 0.0);
        }
        gldraw.draw(gl, mvp, QColor::fromRgbF(0.5f, 0.9f, 0.2f),
                    GL_LINE_LOOP, 1);
    }
}

