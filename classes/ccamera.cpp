#include "ccamera.h"
#include <QOpenGLContext>
#include "aogsettings.h"
#include <math.h>
#include "glm.h"

CCamera::CCamera()
    :camPosZ(0.0)
{
    USE_SETTINGS;
    camPitch = SETTINGS_DISPLAY_CAMPITCH;
    camFollowing = true;
}

void CCamera::setWorldCam(QMatrix4x4 &modelview,
                          double _fixPosX, double _fixPosY,
                          double _fixHeading) {

    camPosX = _fixPosX;
    camPosY = _fixPosY;
    camYaw = _fixHeading;

    //back the camera up
    modelview.translate(0,0,camSetDistance * 0.5);

    modelview.rotate(camPitch, 1, 0, 0);

    if(camFollowing) {
        modelview.rotate(camYaw, 0,0,1);
        modelview.translate(-camPosX, -camPosY, -camPosZ);
    } else {
        modelview.translate(-camPosX, -camPosY, -camPosZ);
    }
}

