#include <QtGlobal>
#include "cvehicle.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QRgb>
#include "aogsettings.h"
#include "ccamera.h"
#include "cboundary.h"
#include "glm.h"
#include "glutils.h"
#include "cnmea.h"
#include "chead.h"
#include "ccamera.h"
#include "cabline.h"
#include "cabcurve.h"
#include "ccontour.h"

CVehicle::CVehicle(QObject *parent) : QObject(parent)
{
    treeSpacing = 0;
}

//called from various Classes, always needs current speed
double CVehicle::updateGoalPointDistance(CNMEA &pn, double distanceFromCurrentLine)
{
    USE_SETTINGS;
    //how far should goal point be away  - speed * seconds * kmph -> m/s then limit min value
    double goalPointDistance = pn.speed * SETTINGS_VEHICLE_GOALPOINTLOOKAHEAD * 0.27777777;

    if (distanceFromCurrentLine < 1.0)
        goalPointDistance += distanceFromCurrentLine * goalPointDistance * SETTINGS_VEHICLE_DISTANCEMULTIPLIER;
    else
        goalPointDistance += goalPointDistance * SETTINGS_VEHICLE_DISTANCEMULTIPLIER;

    if (goalPointDistance < SETTINGS_VEHICLE_LOOKAHEADMINIMUM) goalPointDistance = SETTINGS_VEHICLE_LOOKAHEADMINIMUM;

    //emit setLookAheadGoal(goalPointDistance);
    //mf.lookaheadActual = goalPointDistance; //mf.lookaheadActual is unused

    return goalPointDistance;

}

void CVehicle::drawVehicle(QOpenGLFunctions *gl, QMatrix4x4 modelview,
                           QMatrix4x4 projection,
                           const CCamera &camera,
                           CBoundary &bnd, CHead &hd,
                           const CContour &ct,
                           const CABCurve &curve, const CABLine &ABLine)
{
    USE_SETTINGS;

    //draw vehicle
    modelview.rotate(glm::toDegrees(-fixHeading), 0.0, 0.0, 1.0);

    GLHelperColors glcolors;
    GLHelperOneColor gldraw;
    ColorVertex cv;
    QLocale locale;

    QMatrix4x4 mvp = projection*modelview;


    //antenna
    cv.color = QVector4D(0.0f, 0.95f, 0.95f, 1.0f);
    cv.vertex = QVector3D(0, SETTINGS_VEHICLE_ANTENNAPIVOT, 0);
    glcolors.append(cv);

    //hitch pin
    cv.color = QVector4D(0.95f, 0.0f, 0.0f, 1.0f);
    cv.vertex = QVector3D(0, SETTINGS_TOOL_HITCHLENGTH, 0);
    glcolors.append(cv);

    glcolors.draw(gl,mvp, GL_POINTS, 6.0f);

    ////draw the vehicle Body

    if (!SETTINGS_VEHICLE_ISHYDLIFTON)
    {
        glcolors.clear();

        //GL.Begin(PrimitiveType.TriangleFan);
        cv.color = QVector4D(0.9, 0.90, 0.0,1.0f);
        cv.vertex = QVector3D(0, SETTINGS_VEHICLE_ANTENNAPIVOT, -0.0);
        glcolors.append(cv);
        cv.vertex = QVector3D(1.0, -0, 0.0);
        glcolors.append(cv);
        cv.color = QVector4D(0.0, 0.90, 0.92, 1.0f);
        cv.vertex = QVector3D(0, SETTINGS_VEHICLE_WHEELBASE, 0.0);
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
            gldraw.append(QVector3D(0, SETTINGS_VEHICLE_ANTENNAPIVOT, -0.0));
            gldraw.append(QVector3D(1.0, -0, 0.0));
            gldraw.append(QVector3D(0, SETTINGS_VEHICLE_WHEELBASE, 0.0));
            gldraw.append(QVector3D(-1.0, -0, 0.0));
            gldraw.append(QVector3D(1.0, -0, 0.0));
            gldraw.draw(gl,mvp,QColor::fromRgbF(0.0, 0.95, 0.0),
                        GL_TRIANGLE_FAN,1.0f);
        }
        else
        {
            gldraw.append(QVector3D(0, SETTINGS_VEHICLE_ANTENNAPIVOT, -0.0));
            gldraw.append(QVector3D(1.0, -0, 0.0));
            gldraw.append(QVector3D(0, SETTINGS_VEHICLE_WHEELBASE, 0.0));
            gldraw.append(QVector3D(-1.0, -0, 0.0));
            gldraw.append(QVector3D(1.0, -0, 0.0));
            gldraw.draw(gl,mvp,QColor::fromRgbF(0.950, 0.0, 0.0),
                        GL_TRIANGLE_FAN,1.0f);
        }
    }

    gl->glLineWidth(3);
    gldraw.clear();

    gldraw.append(QVector3D(-1.0, 0, 0));
    gldraw.append(QVector3D(1.0, 0, 0));
    gldraw.append(QVector3D(0, SETTINGS_VEHICLE_WHEELBASE, 0));

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

    if (curve.isBtnCurveOn && !ct.isContourBtnOn)
    {
        drawTextVehicle(camera, gl, mvp, 0, SETTINGS_VEHICLE_WHEELBASE,
                        locale.toString(curve.curveNumber), 1.5,
                        true, QColor::fromRgbF(0.969, 0.95, 0.9510, 0.87));
    }
    else if (ABLine.isBtnABLineOn && !ct.isContourBtnOn)
    {
        drawTextVehicle(camera, gl, mvp, 0, SETTINGS_VEHICLE_WHEELBASE,
                        locale.toString(ABLine.passNumber), 1.5,
                        true, QColor::fromRgbF(0.969, 0.95, 0.9510, 0.87));
    }

    //draw the rigid hitch
    gldraw.clear();
    gldraw.append(QVector3D(0, SETTINGS_TOOL_HITCHLENGTH, 0));
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

void CVehicle::onNewSpeed(double _speed)
{
    avgSpeed = _speed * 0.9 + avgSpeed * 0.1;

    //TODO: complementary filter instead of ring buffer?
}

