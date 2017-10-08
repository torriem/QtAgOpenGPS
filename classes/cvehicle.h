#ifndef CVEHICLE_H
#define CVEHICLE_H

class FormGPS;
class QOpenGLContext;

class CVehicle
{
private:
    FormGPS *mf;
public:
    double toolWidth;
    double toolFarLeftPosition = 0;
    double toolFarLeftSpeed = 0;
    double toolFarRightPosition = 0;
    double toolFarRightSpeed = 0;

    double toolOverlap;
    double toolTrailingHitchLength, tankTrailingHitchLength;
    double toolOffset;

    double toolTurnOffDelay;
    double toolLookAhead;

    bool isToolTrailing;
    bool isToolBehindPivot;
    bool isSteerAxleAhead;

    //vehicle specific
    bool isPivotBehindAntenna;

    double antennaHeight;
    double antennaPivot;
    double wheelbase;
    double hitchLength;

    //how many individual sections
    int numOfSections;
    int numSuperSection;

    //used for super section off on
    int toolMinUnappliedPixels;
    bool isSuperSectionAllowedOn;
    bool areAllSectionBtnsOn = true;

    //read pixel values
    int rpXPosition;
    int rpWidth;

    //min vehicle speed allowed before turning shit off
    double slowSpeedCutoff = 0;

    //autosteer values
    double goalPointLookAhead;
    double maxSteerAngle;
    double maxAngularVelocity;


    CVehicle(FormGPS *mf);
    void drawVehicle(QOpenGLContext *c);

};

#endif // CVEHICLE_H
