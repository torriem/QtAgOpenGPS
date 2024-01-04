#include <QQuickView>
#include <QQuickWidget>
#include <QQmlContext>
#include <QScreen>
#include "formgps.h"
#include "qmlutil.h"
#include <QTimer>
#include "cnmea.h"
#include "cvehicle.h"
#include "csection.h"
#include "ccontour.h"
#include "cabline.h"
#include "aogproperty.h"

#include <QGuiApplication>
#include <QQmlEngine>

//#include <QtGui/private/qguiapplication_p.h>
//#include <QtGui/qpa/qplatformintegration.h>
//hello

#include <functional>
#include <assert.h>
#include "aogrenderer.h"

void FormGPS::setupGui()
{
    /* Load the QML UI and display it in the main area of the GUI */
    setProperty("title","QtAgOpenGPS");

    //Load the QML into a view
    rootContext()->setContextProperty("screenPixelDensity",QGuiApplication::primaryScreen()->physicalDotsPerInch() * QGuiApplication::primaryScreen()->devicePixelRatio());
    rootContext()->setContextProperty("mainForm", this);
    load(QUrl("qrc:/qml/MainWindow.qml"));
    //setColor(Qt::transparent);

    //get pointer to root QML object, which is the OpenGLControl,
    //store in a member variable for future use.
    qml_root = rootObjects().first();

    qml_root->setProperty("visible",true);


    connect(qml_root,SIGNAL(closing(QQuickCloseEvent *)), this, SLOT(fileSaveEverythingBeforeClosingField(QQuickCloseEvent *)));

    //connect qml button signals to callbacks (it's not automatic with qml)
    //btnMenuDrawer = qmlItem(qml_root, "btnMenuDrawer");


    /*btnPerimeter = qmlItem(qml_root,"btnPerimeter");
    connect(btnPerimeter,SIGNAL(clicked()),this,
            SLOT(onBtnPerimeter_clicked()));
*/
    contextArea = qmlItem(qml_root, "contextArea");

    btnnavigationSettings = qmlItem(qml_root, "btnNavigationSettings");
    connect(btnnavigationSettings,SIGNAL(clicked()),this,
            SLOT(onBtnNavigationSettings_clicked()));
    btnAutoSteer = qmlItem(qml_root,"btnAutoSteer");
    connect(btnAutoSteer,SIGNAL(clicked()),this,
            SLOT(onBtnAutoSteer_clicked()));

    btnFlag = qmlItem(qml_root,"btnFlag");
    connect(btnFlag,SIGNAL(clicked()),this,
            SLOT(onBtnFlag_clicked()));


    btnABLine = qmlItem(qml_root,"btnABLine");
    connect(btnABLine,SIGNAL(clicked()),this,
            SLOT(onBtnABLine_clicked()));

    btnCurve = qmlItem(qml_root,"btnABCurve");
    connect(btnCurve,SIGNAL(clicked()),this,
            SLOT(onBtnABCurve_clicked()));

    btnContour = qmlItem(qml_root,"btnContour");
    connect(btnContour,SIGNAL(clicked()),this,
            SLOT(onBtnContour_clicked()));

    btnContourPriority = qmlItem(qml_root,"btnContourPriority");
    connect(btnContourPriority,SIGNAL(clicked()),this,
            SLOT(onBtnContourPriority_clicked()));

    btnManualOffOn = qmlItem(qml_root,"btnManualOffOn");
    connect(btnManualOffOn,SIGNAL(clicked()),this,
            SLOT(btnSectionMasterManual_Click()));

    btnSectionOffAutoOn = qmlItem(qml_root,"btnSectionOffAutoOn");
    connect(btnSectionOffAutoOn,SIGNAL(clicked()),this,
            SLOT(btnSectionMasterAuto_Click()));


    btnTiltDown = qmlItem(qml_root,"btnTiltDown");
    connect(btnTiltDown,SIGNAL(clicked()),this,
            SLOT(onBtnTiltDown_clicked()));

    btnTiltUp = qmlItem(qml_root,"btnTiltUp");
    connect(btnTiltUp,SIGNAL(clicked()),this,
            SLOT(onBtnTiltUp_clicked()));

    btnZoomIn = qmlItem(qml_root,"btnZoomIn");
    connect(btnZoomIn,SIGNAL(clicked()),this,
            SLOT(onBtnZoomIn_clicked()));

    btnZoomOut = qmlItem(qml_root,"btnZoomOut");
    connect(btnZoomOut,SIGNAL(clicked()),this,
            SLOT(onBtnZoomOut_clicked()));

    //icon palette


    btnSettings = qmlItem(qml_root,"btnSettings");
    connect(btnSettings,SIGNAL(clicked()),this,
            SLOT(onBtnSettings_clicked()));

  /*  btnJob = qmlItem(qml_root,"btnJob");
    connect(btnJob,SIGNAL(clicked()),this,
            SLOT(onBtnJob_clicked()));

    btnBoundaryMenu = qmlItem(qml_root,"btnBoundaryMenu");
    connect(btnBoundaryMenu,SIGNAL(clicked()),this,
            SLOT(onBtnBoundaryMenu_clicked()));

    btnComm = qmlItem(qml_root,"btnComm");
    connect(btnComm,SIGNAL(clicked()),this,
            SLOT(onBtnComm_clicked()));

    btnUnits = qmlItem(qml_root,"btnUnits");
    connect(btnUnits,SIGNAL(clicked()),this,
            SLOT(onBtnUnits_clicked()));

    btnFileExplorer = qmlItem(qml_root,"btnFileExplorer");
    connect(btnFileExplorer,SIGNAL(clicked()),this,
            SLOT(onBtnFileExplorer_clicked()));

    btnAutoSteerConfig = qmlItem(qml_root,"btnAutoSteerConfig");
    connect(btnAutoSteerConfig,SIGNAL(clicked()),this,
            SLOT(onBtnAutoSteerConfig_clicked()));
*/
    //Any objects we don't need to access later we can just store
    //temporarily
    QObject *temp;
    temp = qmlItem(qml_root,"btnRedFlag");
    connect(temp,SIGNAL(clicked()),this,SLOT(onBtnRedFlag_clicked()));
    temp = qmlItem(qml_root,"btnGreenFlag");
    connect(temp,SIGNAL(clicked()),this,SLOT(onBtnGreenFlag_clicked()));
    temp = qmlItem(qml_root,"btnYellowFlag");
    connect(temp,SIGNAL(clicked()),this,SLOT(onBtnYellowFlag_clicked()));

    btnDeleteFlag = qmlItem(qml_root,"btnDeleteFlag");
    connect(btnDeleteFlag,SIGNAL(clicked()),this,SLOT(onBtnDeleteFlag_clicked()));
    btnDeleteAllFlags = qmlItem(qml_root,"btnDeleteAllFlags");
    connect(btnDeleteAllFlags,SIGNAL(clicked()),this,SLOT(onBtnDeleteAllFlags_clicked()));
    contextFlag = qmlItem(qml_root, "contextFlag");

    //connnect section buttons to callbacks
    sectionButtonsSignalMapper = new QSignalMapper(this);
    for(int i=0; i < 16; i++){ //16 total onscreen buttons
        sectionButton[i] = qmlItem(qml_root,QString("section")+QString::number(i));
        sectionButton[i]->setProperty("state","off");
        connect(sectionButton[i],SIGNAL(clicked()),
                sectionButtonsSignalMapper,
                SLOT(map()));
        sectionButtonsSignalMapper->setMapping(sectionButton[i], i);
    }
    connect(sectionButtonsSignalMapper,SIGNAL(mapped(int)),this,
            SLOT(btnSectionMan_Click(int)));

    txtDistanceOffABLine = qmlItem(qml_root,"txtDistanceOffABLine");

    temp = qmlItem(qml_root,"btnAreaSide");
    connect(temp,SIGNAL(clicked()), this, SLOT(onBtnAreaSide_clicked()));

    temp = qmlItem(qml_root, "btnManUturnLeft");
    connect(temp,SIGNAL(clicked()), this, SLOT(onBtnManUTurnLeft_clicked()));

    temp = qmlItem(qml_root, "btnManUturnRight");
    connect(temp,SIGNAL(clicked()), this, SLOT(onBtnManUTurnRight_clicked()));

    openGLControl = qml_root->findChild<AOGRendererInSG *>("openglcontrol");
    //This is a bit hackish, but all rendering is done in this item, so
    //we have to give it a way of calling our initialize and draw functions
    openGLControl->setProperty("callbackObject",QVariant::fromValue((void *) this));
    openGLControl->setProperty("initCallback",QVariant::fromValue((void *) &FormGPS::openGLControl_Initialized));
    openGLControl->setProperty("paintCallback",QVariant::fromValue((void *) &FormGPS::oglMain_Paint));

    openGLControl->setProperty("samples",settings.value("display/antiAliasSamples", 0));
    openGLControl->setMirrorVertically(true);
    connect(openGLControl,SIGNAL(clicked(QVariant)),this,SLOT(onGLControl_clicked(QVariant)));

    //TODO: save and restore these numbers from settings
    qml_root->setProperty("width",1000);
    qml_root->setProperty("height",700);

    tmrWatchdog = new QTimer(this);
    connect (tmrWatchdog, SIGNAL(timeout()),this,SLOT(tmrWatchdog_timeout()));
    tmrWatchdog->start(250); //fire every 50ms.

    //SIM on
    connect_classes();


    swFrame.start();

    stopwatch.start();

}

void FormGPS::onGLControl_clicked(const QVariant &event)
{
    QObject *m = event.value<QObject *>();

    //cancel click if there are menus to close
    if (closeAllMenus()) return;

    //Pass the click on to the rendering routine.
    //make the bottom left be 0,0
    mouseX = m->property("x").toInt();
    mouseY = m->property("height").toInt() - m->property("y").toInt();
    leftMouseDownOnOpenGL = true;
    openGLControl->update();
}

void FormGPS::onBtnAcres_clicked(){
    qDebug()<<"AcresButton";
}
void FormGPS::onBtnNavigationSettings_clicked(){
    qDebug()<<"Navigation Settings";
}
void FormGPS::onBtnSettings_clicked(){
    qDebug()<<"Settings";
}
void FormGPS::onBtnAgIO_clicked(){
    qDebug()<<"AgIO";
}
void FormGPS::onBtnSteerConfig_clicked(){
    qDebug()<<"Steer config`";
}
void FormGPS::onBtnSteerMode_clicked(){
    qDebug()<<"Steer mode`";
}
void FormGPS::onBtnToggleAB_clicked(){
    qDebug()<<"Toggle AB";
}
void FormGPS::onBtnToggleABBack_clicked(){
    qDebug()<<"toggle AB back";
}
void FormGPS::onBtnAutoYouTurn_clicked(){
    qDebug()<<"activate youturn";
}
void FormGPS::onBtnResetTool_clicked(){
    qDebug()<<"REset tool";
}
void FormGPS::onBtnHeadland_clicked(){
    qDebug()<<"Headland";
}
void FormGPS::onBtnHydLift_clicked(){
    qDebug()<<"Hyd";
}
void FormGPS::onBtnTramlines_clicked(){
    qDebug()<<"tramline";
}
void FormGPS::onBtnSectionColor_clicked(){
    qDebug()<<"Section color";
}
void FormGPS::onBtnLinePicker_clicked(){
    qDebug()<<"Line picker";
}
void FormGPS::onBtnSnapToPivot_clicked(){
    qDebug()<<"snap to pivot";
}
void FormGPS::onBtnYouSkip_clicked(){
    qDebug()<<"you skip";
}


void FormGPS::onBtnAreaSide_clicked() {
    isAreaOnRight = !isAreaOnRight;
    settings.setValue("vehicle/isAreaOnRight", isAreaOnRight);
    contextArea->setProperty("visible",false);
    //vehicle->settingsChanged();
}

void FormGPS::onBtnAutoSteer_clicked(){
    if (isAutoSteerBtnOn) {
        isAutoSteerBtnOn = false;
        btnAutoSteer->setProperty("icon","/images/AutoSteerOff.png");

    } else {
        if (ABLine.isABLineSet || ct.isContourBtnOn) {
            isAutoSteerBtnOn = true;
            btnAutoSteer->setProperty("icon","/images/AutoSteerOn.png");
        } else {
            //show a timed error message
            //No Guidance Lines; turn on contour or set AB line"
        }
    }
    closeAllMenus();
}

void FormGPS::onBtnFlag_clicked() {
    if (closeAllMenus()) return;

    //TODO if this button is disabled until field is started, we won't
    //need this check.

    if(isGPSPositionInitialized) {
        int nextflag = flagPts.size() + 1;
        //CFlag flagPt(pn.latitude, pn.longitude, pn.easting, pn.northing, flagColor, nextflag);
        //flagPts.append(flagPt);
        flagsBufferCurrent = false;
        //TODO: FileSaveFlags();
    }
}


void FormGPS::onBtnABLine_clicked(){
    if (closeAllMenus()) return;
    qDebug()<<"abline button clicked." ;

    ABLine.isBtnABLineOn = !ABLine.isBtnABLineOn;
    if (ABLine.isBtnABLineOn) {
        qmlItem(qml_root,"btnABLine")->setProperty("isChecked",true);
    } else {
        qmlItem(qml_root,"btnABLine")->setProperty("isChecked",false);
    }
}

void FormGPS::onBtnABCurve_clicked(){
    if (closeAllMenus()) return;
    qDebug()<<"ABCurve button clicked." ;
    ABLine.isBtnABLineOn = !ABLine.isBtnABLineOn;
    curve.isBtnCurveOn = !curve.isBtnCurveOn;
    if (curve.isBtnCurveOn) {
        qmlItem(qml_root,"btnABCurve")->setProperty("isChecked",true);
    } else {
        qmlItem(qml_root,"btnABCurve")->setProperty("isChecked",false);
    }
}
void FormGPS::onBtnContour_clicked(){
    if (closeAllMenus()) return;
    qDebug()<<"contour button clicked." ;

    ct.isContourBtnOn = !ct.isContourBtnOn;
    if (ct.isContourBtnOn) {
        qmlItem(qml_root,"btnContour")->setProperty("isChecked",true);
        qmlItem(qml_root,"btnContourPriority")->setProperty("visible",true);
    } else {
        qmlItem(qml_root,"btnContour")->setProperty("isChecked",false);
        qmlItem(qml_root,"btnContourPriority")->setProperty("visible",false);
    }

}

void FormGPS::onBtnContourPriority_clicked(){
    if (closeAllMenus()) return;
    qDebug()<<"contour priority button clicked." ;

    ct.isRightPriority = !ct.isRightPriority;
    if (ct.isRightPriority)
        qmlItem(qml_root,"btnContourPriority")->setProperty("isChecked",true);
    else
        qmlItem(qml_root,"btnContourPriority")->setProperty("isChecked",false);
}

void FormGPS::onBtnTiltDown_clicked(){

    if (closeAllMenus()) return;
    qDebug()<<"TiltDown button clicked.";

    if (camera.camPitch > -59) camera.camPitch = -60;
    camera.camPitch += ((camera.camPitch * 0.012) - 1);
    if (camera.camPitch < -76) camera.camPitch = -76;

    lastHeight = -1; //redraw the sky
    property_setwin = camera.camPitch;
    openGLControl->update();
}

void FormGPS::onBtnTiltUp_clicked(){
    double camPitch = property_setwin;

    if (closeAllMenus()) return;
    qDebug()<<"TiltUp button clicked.";

    lastHeight = -1; //redraw the sky
    camera.camPitch -= ((camera.camPitch * 0.012) - 1);
    if (camera.camPitch > -58) camera.camPitch = 0;

    property_setwin = camera.camPitch;
    openGLControl->update();
}

void FormGPS::onBtnZoomIn_clicked(){
    if (closeAllMenus()) return;
    qDebug() <<"ZoomIn button clicked.";
    if (camera.zoomValue <= 20) {
        if ((camera.zoomValue -= camera.zoomValue * 0.1) < 3.0)
            camera.zoomValue = 3.0;
    } else {
        if ((camera.zoomValue -= camera.zoomValue * 0.05) < 3.0)
            camera.zoomValue = 3.0;
    }
    camera.camSetDistance = camera.zoomValue * camera.zoomValue * -1;
    SetZoom();
    //TODO save zoom to properties
    openGLControl->update();
}

void FormGPS::onBtnZoomOut_clicked(){
    if (closeAllMenus()) return;
    qDebug() <<"ZoomOut button clicked.";
    if (camera.zoomValue <= 20) camera.zoomValue += camera.zoomValue * 0.1;
    else camera.zoomValue += camera.zoomValue * 0.05;
    if (camera.zoomValue > 220) camera.zoomValue = 220;
    camera.camSetDistance = camera.zoomValue * camera.zoomValue * -1;
    SetZoom();

    //todo save to properties
    openGLControl->update();
}

void FormGPS::onBtnRedFlag_clicked()
{
    flagColor = 0;
    contextFlag->setProperty("visible",false);
    btnFlag->setProperty("icon","/images/FlagRed.png");
}

void FormGPS::onBtnGreenFlag_clicked()
{
    flagColor = 1;
    contextFlag->setProperty("visible",false);
    btnFlag->setProperty("icon","/images/FlagGrn.png");
}

void FormGPS::onBtnYellowFlag_clicked()
{
    flagColor = 2;
    contextFlag->setProperty("visible",false);
    btnFlag->setProperty("icon","/images/FlagYel.png");
}

void FormGPS::onBtnDeleteFlag_clicked()
{
    //delete selected flag and set selected to none
    flagPts.remove(flagNumberPicked - 1);
    flagsBufferCurrent = false;
    flagNumberPicked = 0;

    int flagCnt = flagPts.size();
    if (flagCnt > 0) {
        for (int i = 0; i < flagCnt; i++)
            flagPts[i].ID = i + 1;
    }
    contextFlag->setProperty("visible",false);
}

void FormGPS::onBtnDeleteAllFlags_clicked()
{
    contextFlag->setProperty("visible",false);
    flagPts.clear();
    flagsBufferCurrent = false;
    flagNumberPicked = 0;
    //TODO: FileSaveFlags
}

bool FormGPS::closeAllMenus()
{
    //If any of the popup menus are showing, close them,
    //and cancel the click.
    /*if (!btnMenuDrawer->property("hideMenu").toBool() ||
        contextArea->property("visible").toBool() ||
        contextFlag->property("visible").toBool()) {

        btnMenuDrawer->setProperty("hideMenu",true);
        contextArea->setProperty("visible",false);
        contextFlag->setProperty("visible",false);
        openGLControl->update();
        return true;
    }*/
    return false;
}

void FormGPS::onBtnManUTurnLeft_clicked()
{
    if (yt.isYouTurnTriggered) {
        yt.ResetYouTurn();
    }else {
        yt.isYouTurnTriggered = true;
        yt.BuildManualYouTurn(vehicle, ABLine, curve, false, true);
   }
}

void FormGPS::onBtnManUTurnRight_clicked()
{
    if (yt.isYouTurnTriggered) {
        yt.ResetYouTurn();
    }else {
        yt.isYouTurnTriggered = true;
        yt.BuildManualYouTurn(vehicle, ABLine, curve, true, true);
   }
}

void FormGPS::TimedMessageBox(int timeout, QString s1, QString s2)
{
    qDebug() << s1 << ", " << s2 << Qt::endl;
    //TODO ask QML to display a message
}

void FormGPS::turnOffBoundAlarm()
{
    qDebug() << "Bound alarm should be off" << Qt::endl;
    //TODO implement sounds
}

void FormGPS::FixPanelsAndMenus()
{
    //TODO QML, perhaps not much needed to do here

    if (tool.isSectionsNotZones)
        LineUpIndividualSectionBtns();
    else
        LineUpAllZoneButtons();

}

void FormGPS::FixTramModeButton()
{
    //TODO QML
    //unhide button if it should be seen
    if (tram.tramList.count() > 0 || tram.tramBndOuterArr.count() > 0)
    {
        //btnTramDisplayMode.Visible = true;
        tram.displayMode = 1;
    }

    //make sure tram has right icon.  DO this through javascript

}
