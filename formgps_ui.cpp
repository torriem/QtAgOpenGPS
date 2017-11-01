#include <QQuickView>
#include <QQuickWidget>
#include "formgps.h"
#include "ui_formgps.h" //moc-generated from ui file
#include "openglcontrolitem.h"
#include "openglcontrol.h"
#include "qmlutil.h"
#include <QTimer>
#include "cvehicle.h"
#include "csection.h"

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

    ui->statusBar->hide();

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
    //attach callback to signal so we can interact with the control
    //once it is realized.
    //connect(qml_root,SIGNAL(controlSet(OpenGLControl *)),this,
    //        SLOT(openGLControl_set(OpenGLControl*)));

    //save a copy of this item because we'll need it to request updates
    //when GPS data comes in (or on a timer for FPS).
    //openGLControl_item = qobject_cast<OpenGLControlItem *>(qml_root);

    //connect qml button signals to callbacks (it's not automatic with qml)
    btnMinMaxZoom = qmlItem(qml_root,"btnMinMaxZoom");
    connect(btnMinMaxZoom,SIGNAL(clicked()),this,
            SLOT(onBtnMinMaxZoom_clicked()));

    btnPerimeter = qmlItem(qml_root,"btnPerimeter");
    connect(btnPerimeter,SIGNAL(clicked()),this,
            SLOT(onBtnPerimeter_clicked()));

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
    openGLControl = c;
    qDebug() << "Apparently the renderer is activated now." ;

    //tell the control to call our main form function for drawing.
    //c->registerInitCallback(std::bind(&FormGPS::openGLControl_Initialized, this, std::placeholders::_1));
    //c->registerPaintCallback(std::bind(&FormGPS::openGLControl_Draw, this, std::placeholders::_1));
    //c->registerPaintCallback(std::bind(&gltest_draw,openGLControl,std::placeholders::_1));
}

void FormGPS::renderGL()
{
    //QOpenGLContext *glContext = QOpenGLContext::currentContext();
    //QOpenGLFunctions_2_1 *gl = glContext->versionFunctions<QOpenGLFunctions_2_1>();

    //glViewport(0,0,400,400);

    /*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 2.0, -2.0, 2.0, -1.5, 1.5);
    //glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.22f, 0.2858f, 0.16f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //gluLookAt(0.0, 0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glColor3f(1.0f, 1.0f, 1.0f);

    double size = 2 * 0.5;

#   define V(a,b,c) glVertex3d( a size, b size, c size );
#   define N(a,b,c) glNormal3d( a, b, c );

    glLineWidth(3);
    // PWO: I dared to convert the code to use macros...
    glBegin( GL_LINE_LOOP ); N( 1.0, 0.0, 0.0); V(+,-,+); V(+,-,-); V(+,+,-); V(+,+,+); glEnd();
    glBegin( GL_LINE_LOOP ); N( 0.0, 1.0, 0.0); V(+,+,+); V(+,+,-); V(-,+,-); V(-,+,+); glEnd();
    glBegin( GL_LINE_LOOP ); N( 0.0, 0.0, 1.0); V(+,+,+); V(-,+,+); V(-,-,+); V(+,-,+); glEnd();
    glBegin( GL_LINE_LOOP ); N(-1.0, 0.0, 0.0); V(-,-,+); V(-,+,+); V(-,+,-); V(-,-,-); glEnd();
    glBegin( GL_LINE_LOOP ); N( 0.0,-1.0, 0.0); V(-,-,+); V(-,-,-); V(+,-,-); V(+,-,+); glEnd();
    glBegin( GL_LINE_LOOP ); N( 0.0, 0.0,-1.0); V(-,-,-); V(-,+,-); V(+,+,-); V(+,-,-); glEnd();

#   undef V
#   undef N

    glFlush();
    */
}

void FormGPS::onBtnMinMaxZoom_clicked(){
    qDebug()<<"Min Max button clicked." ;
}

void FormGPS::onBtnPerimeter_clicked(){
    qDebug()<<"Perimeter button clicked." ;
}

void FormGPS::onBtnAutoSteer_clicked(){
    qDebug()<<"Autosteer button clicked." ;
}

void FormGPS::onBtnFlag_clicked(){
    qDebug()<<"flag button clicked." ;
}

void FormGPS::onBtnABLine_clicked(){
    qDebug()<<"abline button clicked." ;
}

void FormGPS::onBtnContour_clicked(){
    qDebug()<<"contour button clicked." ;
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
            section[j].isAllowedOn = true;
            section[j].manBtnState = btnStates::Auto; //auto rolls over to on
        }

        manualAllBtnsUpdate();
        break;

    case btnStates::On:
        manualBtnState = btnStates::Off;
        qmlItem(qml_root,"btnManualOffOn")->setProperty("isChecked",false);

        //turn section buttons all OFF or Auto if SectionAuto was on or off
        for (int j = 0; j < vehicle->numOfSections; j++)
        {
            section[j].isAllowedOn = false;
            section[j].manBtnState = btnStates::On;
        }

        //Update the button colors and text
        manualAllBtnsUpdate();
        break;
    case btnStates::Auto:
        //shouldn't ever happen!
        assert(1 == 0);
        break;
    }
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
                section[j].isAllowedOn = true;
                section[j].manBtnState = btnStates::Off;
            }

            manualAllBtnsUpdate();
            break;

        case btnStates::Auto:
            autoBtnState = btnStates::Off;
            qmlItem(qml_root,"btnSectionOffAutoOn")->setProperty("isChecked",false);

            //turn section buttons all OFF or Auto if SectionAuto was on or off
            for (int j = 0; j < vehicle->numOfSections; j++)
            {
                section[j].isAllowedOn = false;
                section[j].manBtnState = btnStates::On;
            }

            //Update the button colors and text
            manualAllBtnsUpdate();
            break;

        case btnStates::On:
            //shouldn't ever happen
            assert(1 == 0);

    }
}

//individual buttons for section (called by actual
//qt callback onSectionButton_clicked() SLOT
void FormGPS::onBtnSectionMan_clicked(int sectNumber) {
    if (autoBtnState != btnStates::Auto) {
        //if auto is off just have on-off for choices of section buttons
        if (section[sectNumber].manBtnState == btnStates::Off) {
            ///set to auto so that manuBtnUpdate will roll it over to On
            section[sectNumber].manBtnState = btnStates::Auto;
        }
    }
    //Roll over button to next state
    manualBtnUpdate(sectNumber);
}

void FormGPS::onBtnTiltDown_clicked(){
    qDebug()<<"TiltDown button clicked.";
    camera.camPitch -= (camera.camPitch*0.03-1);
    if (camera.camPitch > 0) camera.camPitch = 0;

    qmlview->update();
}

void FormGPS::onBtnTiltUp_clicked(){
    qDebug()<<"TiltUp button clicked.";
    camera.camPitch += (camera.camPitch*0.03-1);
    if (camera.camPitch < -80) camera.camPitch = -80;

    qmlview->update();
}

void FormGPS::onBtnZoomIn_clicked(){
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

