#include "ccamera.h"
#include <QOpenGLContext>
#include "aogsettings.h"
#include <math.h>
#include "glm.h"

//TODO: move to own .h file
#define DEFAULT_SETCAM_PITCH -64

CCamera::CCamera()
    :camPosZ(0.0)
{
    AOGSettings s;
    camPitch = s.value("camera/pitch", DEFAULT_SETCAM_PITCH).toDouble();
}

/* why are we saving all these values */
void CCamera::setWorldCam(QMatrix4x4 &modelview,
                          double _fixPosX, double _fixPosY,
                          double _fixHeading) {

    camPosX = _fixPosX;
    camPosY = _fixPosY;
    fixHeading = _fixHeading;
    camYaw = _fixHeading;

    //back the camera up
    modelview.translate(0,0,camSetDistance * 0.5);

    ////draw the guide
    //glBegin(GL_TRIANGLES);
    //glColor3f(0.98f, 0.0f, 0.0f);
    //glVertex(0.0f, -2.0f, 0.0f);
    //glColor3f(0.0f, 0.98f, 0.0f);
    //glVertex3f(-2.0f, -3.0f, 0.0f);
    //glColor3f(0.98f, 0.98f, 0.0f);
    //glVertex3f(2.0f, -3.0f, 0.0f);
    //glEnd();						// Done Drawing Reticle

    modelview.rotate(camPitch, 1, 0, 0);
    modelview.rotate(camYaw, 0,0,1);

    double fixHeadingRadians = toRadians(fixHeading);

    if (camPitch > -45) {
        offset = (45.0 + camPitch) / 45.0;

        offset = (offset * offset * offset * offset * 0.015) + 0.02;

        modelview.translate(-camPosX + (offset * camSetDistance * sin(fixHeadingRadians)),
                      -camPosY + (offset * camSetDistance * cos(fixHeadingRadians)),
                      -camPosZ);
    } else {
        modelview.translate(-camPosX + (0.02 * camSetDistance * sin(fixHeadingRadians)),
                      -camPosY + (0.02 * camSetDistance * cos(fixHeadingRadians)),
                      -camPosZ);
    }
}
