#ifndef CCAMERA_H
#define CCAMERA_H


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
    double camSetDistance = -75;
    //double camDelta = 0;

    CCamera();
    void setWorldCam(double _fixPosX, double _fixPosY, double _fixHeading);

};

#endif // CCAMERA_H
