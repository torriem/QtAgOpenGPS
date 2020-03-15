#ifndef CCAMERA_H
#define CCAMERA_H

#include <QMatrix4x4>

class QOpenGLContext;
class CCamera
{
private:
    double camPosX = 0;
    double camPosY = 0;
    const double camPosZ; //readonly in C# code
public:
    double camYaw = 0;

    double offset = 0;
    double camSetDistance = -75; //put in setttings?
    //double camDelta = 0;
    double gridZoom;
    double zoomValue = 15;
    double previousZoom = 25;

    double camHeading = 0.0; //moved from main form to here

    bool camFollowing;
    int camMode = 0;

    CCamera();

    void setWorldCam(QMatrix4x4 &modelview, double _fixPosX, double _fixPosY, double _fixHeading);

};

#endif // CCAMERA_H
