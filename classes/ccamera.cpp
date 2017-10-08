#include "ccamera.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions_1_1>
#include <QSettings>
#include <math.h>
#include "glm.h"

//TODO: move to own .h file
#define DEFAULT_SETCAM_PITCH -64

CCamera::CCamera()
    :camPosZ(0.0)
{
    QSettings s;
    camPitch = s.value("setCam_pitch",QVariant((double) DEFAULT_SETCAM_PITCH)).toDouble();
}

void CCamera::setWorldCam(QOpenGLContext *glContext,
                          double _fixPosX, double _fixPosY,
                          double _fixHeading) {
    QOpenGLFunctions_1_1 *gl = glContext->versionFunctions<QOpenGLFunctions_1_1>();

    camPosX = _fixPosX;
    camPosY = _fixPosY;
    fixHeading = _fixHeading;
    camYaw = _fixHeading;

    //back the camera up
    gl->glTranslatef(0,0,camSetDistance * 0.5);

    ////draw the guide
    //glBegin(GL_TRIANGLES);
    //glColor3f(0.98f, 0.0f, 0.0f);
    //glVertex(0.0f, -2.0f, 0.0f);
    //glColor3f(0.0f, 0.98f, 0.0f);
    //glVertex3f(-2.0f, -3.0f, 0.0f);
    //glColor3f(0.98f, 0.98f, 0.0f);
    //glVertex3f(2.0f, -3.0f, 0.0f);
    //glEnd();						// Done Drawing Reticle

    gl->glRotated(camPitch, 1, 0, 0);
    gl->glRotated(camYaw, 0, 0, 1);

    double fixHeadingRadians = toRadians(fixHeading);

    if (camPitch > -45) {
        offset = (45.0 + camPitch) / 45.0;

        offset = (offset * offset * offset * offset * 0.015) + 0.02;

        gl->glTranslated(-camPosX + (offset * camSetDistance * sin(fixHeadingRadians)),
                         -camPosY + (offset * camSetDistance * cos(fixHeadingRadians)),
                         -camPosZ);
    } else {
        gl->glTranslated(-camPosX + (0.02 * camSetDistance * sin(fixHeadingRadians)),
                         -camPosY + (0.02 * camSetDistance * cos(fixHeadingRadians)),
                         -camPosZ);
    }
}
