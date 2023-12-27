#include "formgps.h"

/***************************************
 * callbacks for classes to connect to *
 ***************************************/

//TimedMessageBox defined in ??????, not here

void FormGPS::connect_classes()
{
    simConnectSlots();

    connect(&curve,SIGNAL(stopAutosteer()),this,SLOT(onStopAutoSteer()));
    connect(&curve,SIGNAL(TimedMessage(int,QString,QString)),this,SLOT(TimedMessageBox(int,QString,QString)));

    connect(&ct,SIGNAL(TimedMessage(int,QString,QString)),this,SLOT(TimedMessageBox(int,QString,QString)));

    connect(&mc, SIGNAL(stopAutoSteer()),this,SLOT(onStopAutoSteer()));
    connect(&mc, SIGNAL(turnOffAutoSections()),this,SLOT(onSectionMasterAutoOff()));
    connect(&mc, SIGNAL(turnOffManulSections()),this,SLOT(onSectionMasterManualOff()));

    connect(&pn, SIGNAL(checkZoomWorldGrid(double,double)),&worldGrid,SLOT(checkZoomWorldGrid(double,double)));

    connect(&recPath, SIGNAL(setSimStepDistance(double)),&sim,SLOT(setSimStepDistance(double)));
    connect(&recPath,SIGNAL(turnOffSectionMasterAuto()),this,SLOT(onSectionMasterAutoOff()));
    connect(&recPath, SIGNAL(stoppedDriving()),this,SLOT(onStoppedDriving()));

    connect(&bnd,SIGNAL(TimedMessage(int,QString,QString)),this,SLOT(TimedMessageBox(int,QString,QString)));
    //connect(&bnd, SIGNAL(soundHydLiftChange(bool)),sounds,SLOT(onHydLiftChange(bool)));

    connect(&yt, SIGNAL(outOfBounds()),&mc,SLOT(setOutOfBounds()));
    //TODO: connect(&yt,SIGNAL(turnOffBoundAlarm()),&sounds,SLOT(onTurnOffBoundAlarm()));

}

void FormGPS::onStopAutoSteer()
{
    //TODO
    //turn off auto steer
    //reset gui buttons
}

void FormGPS::onSectionMasterAutoOff()
{
    //turn off sections
    //reset GUI buttons
}

void FormGPS::onSectionMasterManualOff()
{
    //turn off sections
    //reset GUI buttons
}

void FormGPS::onStoppedDriving()
{
    //reset btnPathGoStop
    //reset btnPathRecordStop
    //reset btnPickPath
    //reset btnResumePath
}
