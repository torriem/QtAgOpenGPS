#ifndef CCAMERA_H
#define CCAMERA_H

#include <QMatrix4x4>

class QOpenGLContext;
class QOpenGLFunctions_2_1;
class CCamera
{
private:
    double camPosX = 0;
    double camPosY = 0;
    const double camPosZ; //readonly in C# code
public:
    double fixHeading = 0;
    double camYaw = 0;

    double camPitch = 0;
    double offset = 0;
    double camSetDistance = -100; //put in setttings?
    //double camDelta = 0;

    CCamera();
    void setWorldCam(QMatrix4x4 &mvp, double _fixPosX, double _fixPosY, double _fixHeading);

};

#endif // CCAMERA_H
