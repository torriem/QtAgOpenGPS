#include <QQuickView>
#include <QQuickWidget>
#include <QQmlContext>
#include <QScreen>
#include "formgps.h"
#include "qmlutil.h"
#include <QTimer>
#include "cvehicle.h"
#include "ccontour.h"
#include "cabline.h"
#include "aogproperty.h"
#include <QGuiApplication>
#include <QQmlEngine>
#include <functional>
#include <assert.h>
#include "aogrenderer.h"
#include "qmlsettings.h"
#include "qmlsectionbuttons.h"
#include "interfaceproperty.h"
#include "cboundarylist.h"

QString caseInsensitiveFilename(QString directory, QString filename);

extern QMLSettings qml_settings;

void FormGPS::setupGui()
{
    /* Load the QML UI and display it in the main area of the GUI */
    setProperty("title","QtAgOpenGPS");

    //Load the QML into a view
    rootContext()->setContextProperty("screenPixelDensity",QGuiApplication::primaryScreen()->physicalDotsPerInch() * QGuiApplication::primaryScreen()->devicePixelRatio());
    rootContext()->setContextProperty("mainForm", this);
    rootContext()->setContextProperty("settings", &qml_settings);

    load(QUrl("qrc:/qml/MainWindow.qml"));
    //setColor(Qt::transparent);

    //get pointer to root QML object, which is the OpenGLControl,
    //store in a member variable for future use.
    qml_root = rootObjects().first();

    qml_root->setProperty("visible",true);

    //have to do this for each Interface and supported data type.
    InterfaceProperty<AOGInterface, int>::set_qml_root(qmlItem(qml_root, "aog"));
    InterfaceProperty<AOGInterface, bool>::set_qml_root(qmlItem(qml_root, "aog"));
    InterfaceProperty<AOGInterface, double>::set_qml_root(qmlItem(qml_root, "aog"));
    InterfaceProperty<AOGInterface, btnStates>::set_qml_root(qmlItem(qml_root, "aog"));

    InterfaceProperty<LinesInterface, int>::set_qml_root(qmlItem(qml_root, "linesInterface"));
    InterfaceProperty<LinesInterface, bool>::set_qml_root(qmlItem(qml_root, "linesInterface"));
    InterfaceProperty<LinesInterface, double>::set_qml_root(qmlItem(qml_root, "linesInterface"));
    InterfaceProperty<LinesInterface, btnStates>::set_qml_root(qmlItem(qml_root, "linesInterface"));

    InterfaceProperty<FieldInterface, int>::set_qml_root(qmlItem(qml_root, "fieldInterface"));
    InterfaceProperty<FieldInterface, bool>::set_qml_root(qmlItem(qml_root, "fieldInterface"));
    InterfaceProperty<FieldInterface, double>::set_qml_root(qmlItem(qml_root, "fieldInterface"));
    InterfaceProperty<FieldInterface, btnStates>::set_qml_root(qmlItem(qml_root, "fieldInterface"));

    InterfaceProperty<VehicleInterface, int>::set_qml_root(qmlItem(qml_root, "vehicleInterface"));
    InterfaceProperty<VehicleInterface, bool>::set_qml_root(qmlItem(qml_root, "vehicleInterface"));
    InterfaceProperty<VehicleInterface, double>::set_qml_root(qmlItem(qml_root, "vehicleInterface"));
    InterfaceProperty<VehicleInterface, btnStates>::set_qml_root(qmlItem(qml_root, "vehicleInterface"));

    InterfaceProperty<BoundaryInterface, int>::set_qml_root(qmlItem(qml_root, "boundaryInterface"));
    InterfaceProperty<BoundaryInterface, bool>::set_qml_root(qmlItem(qml_root, "boundaryInterface"));
    InterfaceProperty<BoundaryInterface, double>::set_qml_root(qmlItem(qml_root, "boundaryInterface"));
    InterfaceProperty<BoundaryInterface, btnStates>::set_qml_root(qmlItem(qml_root, "boundaryInterface"));

    QMLSectionButtons::set_aog_root(qmlItem(qml_root, "aog"));

    //hook up our AOGInterface properties
    QObject *aog = qmlItem(qml_root, "aog");
    QObject *linesInterface = qmlItem(qml_root, "linesInterface");
    QObject *vehicleInterface = qmlItem(qml_root, "vehicleInterface");
    QObject *fieldInterface = qmlItem(qml_root, "fieldInterface");
    QObject *boundaryInterface = qmlItem(qml_root, "boundaryInterface");

    //react to UI changing this property
    connect(aog,SIGNAL(sectionButtonStateChanged()), &tool.sectionButtonState, SLOT(onStatesUpdated()));

    openGLControl = qml_root->findChild<AOGRendererInSG *>("openglcontrol");
    //This is a bit hackish, but all rendering is done in this item, so
    //we have to give it a way of calling our initialize and draw functions
    openGLControl->setProperty("callbackObject",QVariant::fromValue((void *) this));
    openGLControl->setProperty("initCallback",QVariant::fromValue<std::function<void (void)>>(std::bind(&FormGPS::openGLControl_Initialized, this)));
    openGLControl->setProperty("paintCallback",QVariant::fromValue<std::function<void (void)>>(std::bind(&FormGPS::oglMain_Paint,this)));

    openGLControl->setProperty("samples",settings->value("display/antiAliasSamples", 0));
    openGLControl->setMirrorVertically(true);
    connect(openGLControl,SIGNAL(clicked(QVariant)),this,SLOT(onGLControl_clicked(QVariant)));
    connect(openGLControl,SIGNAL(dragged(int,int,int,int)),this,SLOT(onGLControl_dragged(int,int,int,int)));
    connect(openGLControl,SIGNAL(zoomIn()),this,SLOT(onBtnZoomIn_clicked()));
    connect(openGLControl,SIGNAL(zoomOut()),this,SLOT(onBtnZoomOut_clicked()));

    //TODO: save and restore these numbers from settings
    qml_root->setProperty("width",1024);
    qml_root->setProperty("height",768);

    //AB Line Picker
    //react to UI changing these properties
    connect(aog,SIGNAL(currentABLineChanged()), this, SLOT(update_current_ABline_from_qml()));
    connect(aog,SIGNAL(currentABCurveChanged()), this, SLOT(update_current_ABline_from_qml()));
    //linesInterface signals
    connect(linesInterface,SIGNAL(abLine_updateLines()),this,SLOT(update_ABlines_in_qml()));
    connect(linesInterface,SIGNAL(abLine_addLine(QString, double, double, double)), this, SLOT(add_new_ABline(QString,double,double,double)));
    connect(linesInterface,SIGNAL(abLine_setA(bool,double,double,double)), this, SLOT(start_newABLine(bool,double,double,double)));
    connect(linesInterface,SIGNAL(abLine_deleteLine(int)), this, SLOT( delete_ABLine(int)));
    connect(linesInterface,SIGNAL(abLine_swapHeading(int)), this, SLOT(swap_heading_ABLine(int)));
    connect(linesInterface,SIGNAL(abLine_changeName(int, QString)), this, SLOT(change_name_ABLine(int,QString)));


    //manual youturn buttons
    connect(aog,SIGNAL(uturn(bool)), this, SLOT(onBtnManUTurn_clicked(bool)));
    connect(aog,SIGNAL(lateral(bool)), this, SLOT(onBtnLateral_clicked(bool)));

    //TODO interface with UI's are you sure close dialog
    connect(qml_root,SIGNAL(closing(QQuickCloseEvent *)), this, SLOT(fileSaveEverythingBeforeClosingField(QQuickCloseEvent *)));


    //connect settings dialog box
    connect(aog,SIGNAL(settings_reload()), this, SLOT(on_settings_reload()));
    connect(aog,SIGNAL(settings_save()), this, SLOT(on_settings_save()));

    //vehicle saving and loading
    connect(vehicleInterface,SIGNAL(vehicle_update_list()), this, SLOT(vehicle_update_list()));
    connect(vehicleInterface,SIGNAL(vehicle_load(QString)), this, SLOT(vehicle_load(QString)));
    connect(vehicleInterface,SIGNAL(vehicle_delete(QString)), this, SLOT(vehicle_delete(QString)));
    connect(vehicleInterface,SIGNAL(vehicle_saveas(QString)), this, SLOT(vehicle_saveas(QString)));

    //field saving and loading
    connect(fieldInterface,SIGNAL(field_update_list()), this, SLOT(field_update_list()));
    connect(fieldInterface,SIGNAL(field_close()), this, SLOT(field_close()));
    connect(fieldInterface,SIGNAL(field_open(QString)), this, SLOT(field_open(QString)));
    connect(fieldInterface,SIGNAL(field_new(QString)), this, SLOT(field_new(QString)));
    connect(fieldInterface,SIGNAL(field_new_from(QString,QString,int)), this, SLOT(field_new_from(QString,QString,int)));
    connect(fieldInterface,SIGNAL(field_delete(QString)), this, SLOT(field_delete(QString)));

    //React to UI changing imuHeading, in order to reset the IMU heading
    connect(aog, SIGNAL(changeImuHeading(double)), &ahrs, SLOT(changeImuHeading(double)));
    connect(aog, SIGNAL(changeImuRoll(double)), &ahrs, SLOT(changeImuRoll(double)));

    //React to UI setting hyd life settings
    connect(aog, SIGNAL(modules_send_238()), this, SLOT(modules_send_238()));

    connect(aog, SIGNAL(sim_bump_speed(bool)), &sim, SLOT(speed_bump(bool)));
    connect(aog, SIGNAL(sim_zero_speed()), &sim, SLOT(speed_zero()));

    //boundary signals and slots
    connect(&yt, SIGNAL(outOfBounds()),boundaryInterface,SLOT(setIsOutOfBoundsTrue()));
    connect(boundaryInterface, SIGNAL(calculate_area()), this, SLOT(boundary_calculate_area()));
    connect(boundaryInterface, SIGNAL(update_list()), this, SLOT(boundary_update_list()));
    connect(boundaryInterface, SIGNAL(start()), this, SLOT(boundary_start()));
    connect(boundaryInterface, SIGNAL(stop()), this, SLOT(boundary_stop()));
    connect(boundaryInterface, SIGNAL(add_point()), this, SLOT(boundary_add_point()));
    connect(boundaryInterface, SIGNAL(delete_last_point()), this, SLOT(boundary_delete_last_point()));
    connect(boundaryInterface, SIGNAL(pause()), this, SLOT(boundary_pause()));
    connect(boundaryInterface, SIGNAL(record()), this, SLOT(boundary_record()));
    connect(boundaryInterface, SIGNAL(reset()), this, SLOT(boundary_restart()));
    connect(boundaryInterface, SIGNAL(delete_boundary(int)), this, SLOT(boundary_delete(int)));
    connect(boundaryInterface, SIGNAL(set_drive_through(int, bool)), this, SLOT(boundary_set_drivethru(int,bool)));
    connect(boundaryInterface, SIGNAL(delete_all()), this, SLOT(boundary_delete_all()));


    headland_form.bnd = &bnd;
    headland_form.vehicle = &vehicle;
    headland_form.hdl = &hdl;
    headland_form.tool = &tool;

    headland_form.connect_ui(qmlItem(qml_root, "headlandDesigner"));

    //connect qml button signals to callbacks (it's not automatic with qml)

    /*btnPerimeter = qmlItem(qml_root,"btnPerimeter");
    connect(btnPerimeter,SIGNAL(clicked()),this,
            SLOT(onBtnPerimeter_clicked()));
    */

    btnFlag = qmlItem(qml_root,"btnFlag");
    connect(btnFlag,SIGNAL(clicked()),this,
            SLOT(onBtnFlag_clicked()));

    btnContour = qmlItem(qml_root,"btnContour");
    connect(btnContour,SIGNAL(clicked()),this,
            SLOT(onBtnContour_clicked()));

    btnContourPriority = qmlItem(qml_root,"btnContourPriority");
    connect(btnContourPriority,SIGNAL(clicked()),this,
            SLOT(onBtnContourPriority_clicked()));

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

    //txtDistanceOffABLine = qmlItem(qml_root,"txtDistanceOffABLine");

    tmrWatchdog = new QTimer(this);
    connect (tmrWatchdog, SIGNAL(timeout()),this,SLOT(tmrWatchdog_timeout()));
    tmrWatchdog->start(250); //fire every 50ms.

    //SIM on
    connect_classes();


    swFrame.start();

    stopwatch.start();

    vehicle_update_list();

}

void FormGPS::onGLControl_dragged(int pressX, int pressY, int mouseX, int mouseY)
{
    QVector3D from,to,offset;

    from = mouseClickToPan(pressX, pressY);
    to = mouseClickToPan(mouseX, mouseY);
    offset = to - from;

    camera.panX += offset.x();
    camera.panY += offset.y();
    openGLControl->update();
}

void FormGPS::onGLControl_clicked(const QVariant &event)
{
    QObject *m = event.value<QObject *>();

    //Pass the click on to the rendering routine.
    //make the bottom left be 0,0
    mouseX = m->property("x").toInt();
    mouseY = m->property("y").toInt();

    QVector3D field = mouseClickToField(mouseX, mouseY);
    mouseEasting = field.x();
    mouseNorthing = field.y();

    leftMouseDownOnOpenGL = true;
    openGLControl->update();
}

void FormGPS::onBtnAcres_clicked(){
    qDebug()<<"AcresButton";
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

void FormGPS::onBtnFlag_clicked() {

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

void FormGPS::onBtnContour_clicked(){
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
    qDebug()<<"contour priority button clicked." ;

    ct.isRightPriority = !ct.isRightPriority;
    if (ct.isRightPriority)
        qmlItem(qml_root,"btnContourPriority")->setProperty("isChecked",true);
    else
        qmlItem(qml_root,"btnContourPriority")->setProperty("isChecked",false);
}

void FormGPS::onBtnTiltDown_clicked(){

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

    qDebug()<<"TiltUp button clicked.";

    lastHeight = -1; //redraw the sky
    camera.camPitch -= ((camera.camPitch * 0.012) - 1);
    if (camera.camPitch > -58) camera.camPitch = 0;

    property_setwin = camera.camPitch;
    openGLControl->update();
}

void FormGPS::onBtnZoomIn_clicked(){
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

void FormGPS::onBtnManUTurn_clicked(bool right)
{
    if (yt.isYouTurnTriggered) {
        yt.ResetYouTurn();
    }else {
        yt.isYouTurnTriggered = true;
        yt.BuildManualYouTurn(vehicle, ABLine, curve, right, true);
   }
}

void FormGPS::onBtnLateral_clicked(bool right)
{
   yt.BuildManualYouLateral(vehicle, ABLine, curve, right);
}

void FormGPS::TimedMessageBox(int timeout, QString s1, QString s2)
{
    qDebug() << "Timed message " << timeout << s1 << ", " << s2 << Qt::endl;
    //TODO ask QML to display a message
    QObject *temp = qmlItem(qml_root, "timedMessage");
    QMetaObject::invokeMethod(temp, "addMessage", Q_ARG(int, timeout), Q_ARG(QString, s1), Q_ARG(QString, s2));
}

void FormGPS::turnOffBoundAlarm()
{
    qDebug() << "Bound alarm should be off" << Qt::endl;
    //TODO implement sounds
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
void FormGPS::on_settings_reload() {
    loadSettings();
    //TODO: if vehicle name is set, write settings out to that
    //vehicle json file
}

void FormGPS::on_settings_save() {
    settings->sync();

    loadSettings();
}

void FormGPS::modules_send_238() {
    qDebug() << "Sending 238 message to AgIO";
    p_238.pgn[p_238.set0] = (int)property_setArdMac_setting0;
    p_238.pgn[p_238.raiseTime] = (int)property_setArdMac_hydRaiseTime;
    p_238.pgn[p_238.lowerTime] = (int)property_setArdMac_hydLowerTime;

    p_238.pgn[p_238.user1] = (int)property_setArdMac_user1;
    p_238.pgn[p_238.user2] = (int)property_setArdMac_user2;
    p_238.pgn[p_238.user3] = (int)property_setArdMac_user3;
    p_238.pgn[p_238.user4] = (int)property_setArdMac_user4;

    qDebug() << p_238.pgn;
    SendPgnToLoop(p_238.pgn);
}
