#ifndef CCAMERA_H
#define CCAMERA_H


class QOpenGLContext;
class QOpenGLFunctions_2_1;
class CCamera
{
private:
    double camPosX;
    double camPosY;
    const double camPosZ; //readonly in C# code
public:
    double fixHeading;
    double camYaw;

    double camPitch;
    double offset;
    double camSetDistance = -100; //put in setttings?
    //double camDelta = 0;

    CCamera();
    void setWorldCam(QOpenGLFunctions_2_1 *c, double _fixPosX, double _fixPosY, double _fixHeading);

};

#endif // CCAMERA_H
