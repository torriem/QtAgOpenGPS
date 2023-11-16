#include <QQuickView>
#include <QQuickWidget>
#include <QQmlContext>
#include <QScreen>
#include "formgps.h"
//#include "ui_formgps.h" //moc-generated from ui file
#include "qmlutil.h"
#include <QTimer>
#include "cnmea.h"
#include "cvehicle.h"
#include "csection.h"
#include "ccontour.h"
#include "cabline.h"

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
    setTitle(tr("QtAgOpenGPS"));

    //Load the QML into a view
    engine()->rootContext()->setContextProperty("screenPixelDensity",QGuiApplication::primaryScreen()->physicalDotsPerInch() * QGuiApplication::primaryScreen()->devicePixelRatio());
    setSource(QUrl("qrc:/qml/MainWindow.qml"));
    setColor(Qt::transparent);

    //qmlview->setClearBeforeRendering(false);
    //connect(qmlview,SIGNAL(beforeRendering()), this, SLOT(openGLControl_Draw()),Qt::DirectConnection);
    //connect(qmlview,SIGNAL(sceneGraphInitialized()), this, SLOT(openGLControl_Initialized()),Qt::DirectConnection);
    //connect(qmlview,SIGNAL(sceneGraphInvalidated()), this, SLOT(openGLControl_Shutdown()),Qt::DirectConnection);

    /*
    qmlcontainer = QWidget::createWindowContainer(qmlview);


    //place the widget in our window's layout
    ui->verticalLayout->addWidget(qmlcontainer);

    //hide the section control lookahead widget; it should still work
    ui->grnPixels->hide(); //debugging widget, shows lookahead
    */
    //get pointer to root QML object, which is the OpenGLControl,
    //store in a member variable for future use.
    qml_root = this->rootObject();


    //connect qml button signals to callbacks (it's not automatic with qml)
    //btnMenuDrawer = qmlItem(qml_root, "btnMenuDrawer");


    /*btnPerimeter = qmlItem(qml_root,"btnPerimeter");
    connect(btnPerimeter,SIGNAL(clicked()),this,
            SLOT(onBtnPerimeter_clicked()));
*/
    contextArea = qmlItem(qml_root, "contextArea");

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
            SLOT(onBtnManualOffOn_clicked()));

    btnSectionOffAutoOn = qmlItem(qml_root,"btnSectionOffAutoOn");
    connect(btnSectionOffAutoOn,SIGNAL(clicked()),this,
            SLOT(onBtnSectionOffAutoOn_clicked()));


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
    for(int i=0; i < MAXSECTIONS-1; i++){
        sectionButton[i] = qmlItem(qml_root,QString("section")+QString::number(i));
        sectionButton[i]->setProperty("state","off");
        connect(sectionButton[i],SIGNAL(clicked()),
                sectionButtonsSignalMapper,
                SLOT(map()));
        sectionButtonsSignalMapper->setMapping(sectionButton[i], i);
    }
    connect(sectionButtonsSignalMapper,SIGNAL(mapped(int)),this,
            SLOT(onBtnSectionMan_clicked(int)));

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
    openGLControl->setProperty("mainform",qVariantFromValue((void *) this));

    openGLControl->setProperty("samples",settings.value("display/antiAliasSamples", 0));
    openGLControl->setMirrorVertically(true);
    connect(openGLControl,SIGNAL(clicked(QVariant)),this,SLOT(onGLControl_clicked(QVariant)));

    //TODO: save and restore these numbers from settings
    setWidth(1920);
    setHeight(1080);

    tmrWatchdog = new QTimer(this);
    connect (tmrWatchdog, SIGNAL(timeout()),this,SLOT(tmrWatchdog_timeout()));
    tmrWatchdog->start(50); //fire every 50ms.

    //SIM on
    connect(&simTimer, SIGNAL(timeout()),this,SLOT(onSimTimerTimeout()));
    if (SETTINGS_SIM_ON) {
        simTimer.start(100); //fire simulator every 200 ms.
        fixUpdateHz = 10;
    } else {
        fixUpdateHz = 5;
        //TODO! NMEA update settings
    }

    fixUpdateTime = 1.0 / fixUpdateHz;



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
    mouseY = height() - m->property("y").toInt();
    leftMouseDownOnOpenGL = true;
    openGLControl->update();
}

void FormGPS::onBtnMinMaxZoom_clicked(){
    if (closeAllMenus()) return;
    qDebug()<<"Min Max button clicked." ;
}

void FormGPS::onBtnPerimeter_clicked(){
    if (closeAllMenus()) return;
    qDebug()<<"Perimeter button clicked." ;
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
    !ABLine.isBtnABLineOn;
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

void FormGPS::onBtnManualOffOn_clicked(){
    USE_SETTINGS;

    int tool_numOfSections = SETTINGS_TOOL_NUMSECTIONS;

    qDebug()<<"Manual off on button clicked." ;
    switch (manualBtnState)
    {
    case btnStates::Off:
        //roll to "on" state
        manualBtnState = btnStates::On;
        qmlItem(qml_root,"btnManualOffOn")->setProperty("isChecked",true);

        autoBtnState = btnStates::Off;
        qmlItem(qml_root,"btnSectionOffAutoOn")->setProperty("isChecked",false);

        //turn all the sections allowed and update to ON!! Auto changes to ON
        for (int j = 0; j < tool_numOfSections; j++)
        {
            tool.section[j].isAllowedOn = true;
            tool.section[j].manBtnState = btnStates::Auto; //auto rolls over to on
        }

        manualAllBtnsUpdate();
        break;

    case btnStates::On:
        manualBtnState = btnStates::Off;
        qmlItem(qml_root,"btnManualOffOn")->setProperty("isChecked",false);

        //turn section buttons all OFF or Auto if SectionAuto was on or off
        for (int j = 0; j < tool_numOfSections; j++)
        {
            tool.section[j].isAllowedOn = false;
            tool.section[j].manBtnState = btnStates::On;
        }

        //Update the button colors and text
        manualAllBtnsUpdate();
        break;
    case btnStates::Auto:
        //shouldn't ever happen!
        assert(1 == 0);
        break;
    }
    closeAllMenus();
}

void FormGPS::onBtnSectionOffAutoOn_clicked(){
    USE_SETTINGS;

    int tool_numOfSections = SETTINGS_TOOL_NUMSECTIONS;

    qDebug()<<"Section off auto on button clicked." ;
    switch (autoBtnState)
    {
        case btnStates::Off:
            autoBtnState = btnStates::Auto;
            qmlItem(qml_root,"btnSectionOffAutoOn")->setProperty("isChecked",true);

            //turn off manual if on
            manualBtnState = btnStates::Off;
            qmlItem(qml_root,"btnManualOffOn")->setProperty("isChecked",false);

            //turn all the sections allowed and update to ON!! Auto changes to ON
            for (int j = 0; j < tool_numOfSections; j++)
            {
                tool.section[j].isAllowedOn = true;
                tool.section[j].manBtnState = btnStates::Off;
            }

            manualAllBtnsUpdate();
            break;

        case btnStates::Auto:
            autoBtnState = btnStates::Off;
            qmlItem(qml_root,"btnSectionOffAutoOn")->setProperty("isChecked",false);

            //turn section buttons all OFF or Auto if SectionAuto was on or off
            for (int j = 0; j < tool_numOfSections; j++)
            {
                tool.section[j].isAllowedOn = false;
                tool.section[j].manBtnState = btnStates::On;
            }

            //Update the button colors and text
            manualAllBtnsUpdate();
            break;

        case btnStates::On:
            //shouldn't ever happen
            assert(1 == 0);

    }
    closeAllMenus();
}

//individual buttons for section (called by actual
//qt callback onSectionButton_clicked() SLOT
void FormGPS::onBtnSectionMan_clicked(int sectNumber) {
    if (autoBtnState != btnStates::Auto) {
        //if auto is off just have on-off for choices of section buttons
        if (tool.section[sectNumber].manBtnState == btnStates::Off) {
            ///set to auto so that manuBtnUpdate will roll it over to On
            tool.section[sectNumber].manBtnState = btnStates::Auto;
        }
    }
    //Roll over button to next state
    manualBtnUpdate(sectNumber);
    if (closeAllMenus()) return;
}

void FormGPS::onBtnTiltDown_clicked(){
    USE_SETTINGS;

    double camPitch = SETTINGS_DISPLAY_CAMPITCH;

    if (closeAllMenus()) return;
    qDebug()<<"TiltDown button clicked.";
    camPitch -= (camPitch*0.03-1);
    if (camPitch > 0) camPitch = 0;
    lastHeight = -1; //redraw the sky
    SETTINGS_SET_DISPLAY_CAMPITCH(camPitch);
    openGLControl->update();
}

void FormGPS::onBtnTiltUp_clicked(){
    USE_SETTINGS;

    double camPitch = SETTINGS_DISPLAY_CAMPITCH;

    if (closeAllMenus()) return;
    qDebug()<<"TiltUp button clicked.";
    camPitch += (camPitch*0.03-1);
    if (camPitch < -80) camPitch = -80;
    lastHeight = -1; //redraw the sky
    SETTINGS_SET_DISPLAY_CAMPITCH(camPitch);
    openGLControl->update();
}

void FormGPS::onBtnZoomIn_clicked(){
    if (closeAllMenus()) return;
    qDebug() <<"ZoomIn button clicked.";
    if (zoomValue <= 20) {
        if ((zoomValue -= zoomValue * 0.1) < 6.0) zoomValue = 6.0;
    } else {
        if ((zoomValue -= zoomValue*0.05) < 6.0) zoomValue = 6.0;
    }

    camera.camSetDistance = zoomValue * zoomValue * -1;
    setZoom();
    openGLControl->update();
}

void FormGPS::onBtnZoomOut_clicked(){
    if (closeAllMenus()) return;
    qDebug() <<"ZoomOut button clicked.";
    if (zoomValue <= 20)
        zoomValue += zoomValue*0.1;
    else
        zoomValue += zoomValue*0.05;
    camera.camSetDistance = zoomValue * zoomValue * -1;
    setZoom();
    openGLControl->update();
}

void FormGPS::onBtnSnap_clicked(){
    qDebug()<<"Snap button clicked." ;
}

void FormGPS::onBtnTripOdometer_clicked(){
    qDebug()<<"TripOdometer button clicked." ;
}

void FormGPS::onBtnGPSData_clicked(){
    qDebug()<<"GPSData button clicked." ;
}

void FormGPS::onBtnSettings_clicked(){
    qDebug()<<"Settings button clicked." ;
}

void FormGPS::onBtnJob_clicked(){
    qDebug()<<"Job button clicked." ;
}

void FormGPS::onBtnBoundaryMenu_clicked(){
    qDebug()<<"BoundaryMenu button clicked." ;
}

void FormGPS::onBtnComm_clicked(){
    qDebug()<<"Comm button clicked." ;
}

void FormGPS::onBtnUnits_clicked(){
    qDebug()<<"Units button clicked." ;
}

void FormGPS::onBtnFileExplorer_clicked(){
    qDebug()<<"FileExplorer button clicked." ;
}

void FormGPS::onBtnAutoSteerConfig_clicked(){
    qDebug()<<"AutoSteerConfig button clicked." ;
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
        yt.resetYouTurn();
    }else {
        yt.isYouTurnTriggered = true;
        yt.buildManualYouTurn(ABLine, curve, false, true);
   }
}

void FormGPS::onBtnManUTurnRight_clicked()
{
    if (yt.isYouTurnTriggered) {
        yt.resetYouTurn();
    }else {
        yt.isYouTurnTriggered = true;
        yt.buildManualYouTurn(ABLine, curve, true, true);
   }
}
