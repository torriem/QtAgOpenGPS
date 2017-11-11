#include <QQuickView>
#include <QQuickWidget>
#include "formgps.h"
#include "ui_formgps.h" //moc-generated from ui file
#include "openglcontrolitem.h"
#include "openglcontrol.h"
#include "qmlutil.h"
#include <QTimer>
#include "cnmea.h"
#include "cvehicle.h"
#include "csection.h"
#include "toplinedisplay.h"
#include "ccontour.h"

//#include <QGuiApplication>
//#include <QtGui/private/qguiapplication_p.h>
//#include <QtGui/qpa/qplatformintegration.h>
#include <QtOpenGL>
#include <QOpenGLFunctions_2_1>
#include <functional>
#include <assert.h>

void FormGPS::setupGui()
{
    /* Load the QML UI and display it in the main area of the GUI */
    ui->setupUi(this);

    //set up units group
    QActionGroup *unitsGroup = new QActionGroup(this);
    unitsGroup->addAction(ui->actionMetric);
    unitsGroup->addAction(ui->actionImperial);

    ui->statusBar->hide();

    tlDisp = new TopLineDisplay(this);
    tlDisp->txtDistanceOffABLine->setFixedWidth(300);
    ui->menuBar->setCornerWidget(tlDisp);
    //tlDisp->setFixedWidth(400);
    ui->menuBar->adjustSize();

    setWindowTitle(tr("QtAgOpenGPS"));

    //Load the QML into a view
    qmlview = new QQuickView();
    qmlview->setSource(QUrl("qrc:/qml/MainWindow.qml"));
    qmlview->setColor(Qt::transparent);
    //qmlview->setGeometry(0,0,800,600);
    //qmlview->show();

    qmlview->setClearBeforeRendering(false);
    connect(qmlview,SIGNAL(beforeRendering()), this, SLOT(openGLControl_Draw()),Qt::DirectConnection);
    connect(qmlview,SIGNAL(sceneGraphInitialized()), this, SLOT(openGLControl_Initialized()),Qt::DirectConnection);
    connect(qmlview,SIGNAL(sceneGraphInvalidated()), this, SLOT(openGLControl_Shutdown()),Qt::DirectConnection);

    qmlcontainer = QWidget::createWindowContainer(qmlview);


    //place the widget in our window's layout
    ui->verticalLayout->addWidget(qmlcontainer);

    //hide the section control lookahead widget; it should still work
    ui->grnPixels->hide(); //debugging widget, shows lookahead

    //get pointer to root QML object, which is the OpenGLControl,
    //store in a member variable for future use.
    qml_root = qmlview->rootObject();

    connect(qml_root,SIGNAL(clicked(QVariant)),this,SLOT(onGLControl_clicked(QVariant)));
    //attach callback to signal so we can interact with the control
    //once it is realized.
    //connect(qml_root,SIGNAL(controlSet(OpenGLControl *)),this,
    //        SLOT(openGLControl_set(OpenGLControl*)));

    //save a copy of this item because we'll need it to request updates
    //when GPS data comes in (or on a timer for FPS).
    //openGLControl_item = qobject_cast<OpenGLControlItem *>(qml_root);

    //connect qml button signals to callbacks (it's not automatic with qml)
    btnMenuDrawer = qmlItem(qml_root, "btnMenuDrawer");

    btnMinMaxZoom = qmlItem(qml_root,"btnMinMaxZoom");
    connect(btnMinMaxZoom,SIGNAL(clicked()),this,

            SLOT(onBtnMinMaxZoom_clicked()));

    btnPerimeter = qmlItem(qml_root,"btnPerimeter");
    connect(btnPerimeter,SIGNAL(clicked()),this,
            SLOT(onBtnPerimeter_clicked()));

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

    btnContour = qmlItem(qml_root,"btnContour");
    connect(btnContour,SIGNAL(clicked()),this,
            SLOT(onBtnContour_clicked()));

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
    btnSnap = qmlItem(qml_root,"btnSnap");
    connect(btnSnap,SIGNAL(clicked()),this,
            SLOT(onBtnSnap_clicked()));

    btnTripOdometer = qmlItem(qml_root,"btnTripOdometer");
    connect(btnTripOdometer,SIGNAL(clicked()),this,
            SLOT(onBtnTripOdometer_clicked()));

    btnGPSData = qmlItem(qml_root,"btnGPSData");
    connect(btnGPSData,SIGNAL(clicked()),this,
            SLOT(onBtnGPSData_clicked()));

    btnSettings = qmlItem(qml_root,"btnSettings");
    connect(btnSettings,SIGNAL(clicked()),this,
            SLOT(onBtnSettings_clicked()));

    btnJob = qmlItem(qml_root,"btnJob");
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
        qDebug() << QString("section") <<i;
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

    //this is for rendering to the hidden gl widget. Once the offscreen
    //context stuff is working and we don't need to debug the lookahead
    //graphics, we can remove the widget and comment out this stuff.
    /*
    ui->openGLControlBack->setPaintGLCallback(std::bind(&FormGPS::openGLControlBack_Draw,this));
    connect(ui->openGLControlBack,SIGNAL(afterRender()),this,SLOT(processSectionLookahead()));
    */

    //set up off-screen openGL context for section lookahead
    /*
    if (QGuiApplicationPrivate::platform_integration->hasCapability(QPlatformIntegration::ThreadedOpenGL)) {
        qDebug() << "We in a multithread environment?";
        backSurfaceFormat.setDepthBufferSize(24);
        backSurfaceFormat.setMajorVersion(4);
        backSurfaceFormat.setMinorVersion(3);

        backOpenGLContext.setFormat(backSurfaceFormat);
        backOpenGLContext.create();
        //if we're on a single-threaded opengl platform, this will fail
        //we'll have to check for it when drawing to the back opengl
        //surface and just reuse the QML context.

        backSurface.setFormat(backSurfaceFormat);
        backSurface.create();
        assert(backSurface.isValid());
    }
    */
    tmrWatchdog = new QTimer(this);
    connect (tmrWatchdog, SIGNAL(timeout()),this,SLOT(tmrWatchdog_timeout()));
    tmrWatchdog->start(50); //fire every 50ms.

    swFrame.start();

    stopwatch.start();

}

//not currently using this. TODO remove perhaps
void FormGPS::openGLControl_set(OpenGLControl *c){
    //openGLControl = c;
    //qDebug() << "Apparently the renderer is activated now." ;

    //tell the control to call our main form function for drawing.
    //c->registerInitCallback(std::bind(&FormGPS::openGLControl_Initialized, this, std::placeholders::_1));
    //c->registerPaintCallback(std::bind(&FormGPS::openGLControl_Draw, this, std::placeholders::_1));
    //c->registerPaintCallback(std::bind(&gltest_draw,openGLControl,std::placeholders::_1));
}

void FormGPS::renderGL()
{
}

void FormGPS::onGLControl_clicked(const QVariant &event)
{
    QObject *m = event.value<QObject *>();

    //cancel click if there are menus to close
    if (closeAllMenus()) return;

    //Pass the click on to the rendering routine.
    //make the bottom left be 0,0
    mouseX = m->property("x").toInt();
    mouseY = qmlview->height() - m->property("y").toInt();
    leftMouseDownOnOpenGL = true;
}

void FormGPS::onBtnMinMaxZoom_clicked(){
    if (closeAllMenus()) return;
    qDebug()<<"Min Max button clicked." ;
}

void FormGPS::onBtnPerimeter_clicked(){
    if (closeAllMenus()) return;
    qDebug()<<"Perimeter button clicked." ;

    //If we were in the "drawing" state, go to the "done" state
    if (periArea.isBtnPerimeterOn && periArea.periPtList.size() > 0) {
        qDebug() << "draw -> done";
        periArea.isBtnPerimeterOn = false;
        qmlItem(qml_root,"btnPerimeter")->setProperty("icon","qrc:/images/PeriDone.png");
        qmlItem(qml_root,"btnPerimeter")->setProperty("isChecked",false);
        return;
    }

    if (periArea.isBtnPerimeterOn) {
        //go to area; haven't recorded anything
        qmlItem(qml_root,"btnPerimeter")->setProperty("icon","qrc:/images/PeriArea.png");
        qmlItem(qml_root,"btnPerimeter")->setProperty("isChecked",true);
        qmlItem(qml_root,"btnPerimeter")->setProperty("isChecked",false);
        periArea.isBtnPerimeterOn = false;
        return;
    }


    //if we are in the "done" state, go to the "area" state
    if (!periArea.isBtnPerimeterOn && periArea.periPtList.size() > 0) {
        qDebug() << "done -> area";

        periArea.periPtList.clear();
        //periArea.calcList.clear();
        qmlItem(qml_root,"btnPerimeter")->setProperty("icon","qrc:/images/PeriArea.png");
        qmlItem(qml_root,"btnPerimeter")->setProperty("isChecked",true);
        qmlItem(qml_root,"btnPerimeter")->setProperty("isChecked",false);
        return;
    }

    //if we are in the "area state" state, go to the "draw" state
    if (!periArea.isBtnPerimeterOn && periArea.periPtList.size() == 0) {
        periArea.isBtnPerimeterOn = true;
        qDebug() << "area -> draw";
        qmlItem(qml_root,"btnPerimeter")->setProperty("isChecked",true);

    }
}

/* moved this into qml javascript
void FormGPS::onBtnPerimeter_pressAndHeld() {
    qDebug() << "pressed and held!";
    QObject *contextArea = qmlItem(qml_root, "contextArea");

    if (contextArea->property("visible").toBool())
        contextArea->setProperty("visible",false);
    else
        contextArea->setProperty("visible",true);
}
*/

void FormGPS::onBtnAreaSide_clicked() {
    isAreaOnRight = !isAreaOnRight;
    settings.setValue("vehicle/isAreaOnRight", isAreaOnRight);
    contextArea->setProperty("visible",false);
    vehicle->settingsChanged();
}

void FormGPS::onBtnAutoSteer_clicked(){
    if (closeAllMenus()) return;
    qDebug()<<"Autosteer button clicked." ;
}

void FormGPS::onBtnFlag_clicked() {
    if (closeAllMenus()) return;

    //TODO if this button is disabled until field is started, we won't
    //need this check.

    if(isGPSPositionInitialized) {
        int nextflag = flagPts.size() + 1;
        CFlag flagPt(pn->latitude, pn->longitude, pn->easting, pn->northing, flagColor, nextflag);
        flagPts.append(flagPt);
        flagsBufferCurrent = false;
        //TODO: FileSaveFlags();
    }
}

void FormGPS::onBtnABLine_clicked(){
    if (closeAllMenus()) return;
    qDebug()<<"abline button clicked." ;
}

void FormGPS::onBtnContour_clicked(){
    if (closeAllMenus()) return;
    qDebug()<<"contour button clicked." ;

    ct->isContourBtnOn = !ct->isContourBtnOn;
    if (ct->isContourBtnOn)
        qmlItem(qml_root,"btnContour")->setProperty("isChecked",true);
    else
        qmlItem(qml_root,"btnContour")->setProperty("isChecked",false);

}

void FormGPS::onBtnManualOffOn_clicked(){
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
        for (int j = 0; j < vehicle->numOfSections; j++)
        {
            vehicle->section[j].isAllowedOn = true;
            vehicle->section[j].manBtnState = btnStates::Auto; //auto rolls over to on
        }

        manualAllBtnsUpdate();
        break;

    case btnStates::On:
        manualBtnState = btnStates::Off;
        qmlItem(qml_root,"btnManualOffOn")->setProperty("isChecked",false);

        //turn section buttons all OFF or Auto if SectionAuto was on or off
        for (int j = 0; j < vehicle->numOfSections; j++)
        {
            vehicle->section[j].isAllowedOn = false;
            vehicle->section[j].manBtnState = btnStates::On;
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
            for (int j = 0; j < vehicle->numOfSections; j++)
            {
                vehicle->section[j].isAllowedOn = true;
                vehicle->section[j].manBtnState = btnStates::Off;
            }

            manualAllBtnsUpdate();
            break;

        case btnStates::Auto:
            autoBtnState = btnStates::Off;
            qmlItem(qml_root,"btnSectionOffAutoOn")->setProperty("isChecked",false);

            //turn section buttons all OFF or Auto if SectionAuto was on or off
            for (int j = 0; j < vehicle->numOfSections; j++)
            {
                vehicle->section[j].isAllowedOn = false;
                vehicle->section[j].manBtnState = btnStates::On;
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
        if (vehicle->section[sectNumber].manBtnState == btnStates::Off) {
            ///set to auto so that manuBtnUpdate will roll it over to On
            vehicle->section[sectNumber].manBtnState = btnStates::Auto;
        }
    }
    //Roll over button to next state
    manualBtnUpdate(sectNumber);
    if (closeAllMenus()) return;
}

void FormGPS::onBtnTiltDown_clicked(){
    if (closeAllMenus()) return;
    qDebug()<<"TiltDown button clicked.";
    camera.camPitch -= (camera.camPitch*0.03-1);
    if (camera.camPitch > 0) camera.camPitch = 0;
    lastHeight = -1; //redraw the sky

    qmlview->update();
}

void FormGPS::onBtnTiltUp_clicked(){
    if (closeAllMenus()) return;
    qDebug()<<"TiltUp button clicked.";
    camera.camPitch += (camera.camPitch*0.03-1);
    if (camera.camPitch < -80) camera.camPitch = -80;
    lastHeight = -1; //redraw the sky

    qmlview->update();
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
    qmlview->update();
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
    qmlview->update();
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
    flagNumberPicked = 0;
    //TODO: FileSaveFlags
}

bool FormGPS::closeAllMenus()
{
    //If any of the popup menus are showing, close them,
    //and cancel the click.
    if (!btnMenuDrawer->property("hideMenu").toBool() ||
        contextArea->property("visible").toBool() ||
        contextFlag->property("visible").toBool()) {

        btnMenuDrawer->setProperty("hideMenu",true);
        contextArea->setProperty("visible",false);
        contextFlag->setProperty("visible",false);
        return true;
    }
    return false;
}
