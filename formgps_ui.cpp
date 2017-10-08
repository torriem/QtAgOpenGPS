#include <QQuickView>
#include <QQuickWidget>
#include "formgps.h"
#include "ui_formgps.h" //moc-generated from ui file
#include "openglcontrolitem.h"
#include "openglcontrol.h"
#include "qmlutil.h"

#include <functional>

#include <iostream>

void FormGPS::setupGui()
{
    ui->setupUi(this);

    /* Load the QML UI and display it in the main area of the GUI */

    //Load the QML into a view
    QQuickView *qmlview = new QQuickView();
    qmlview->setSource(QUrl("qrc:/qml/MainWindow.qml"));
    qmlview->setColor(Qt::transparent);
    qmlview->show();

    //embed the view in a normal widget
    QWidget *qmlcontainer = QWidget::createWindowContainer(qmlview);

    //place the widget in our window's layout
    ui->verticalLayout->addWidget(qmlcontainer);
    //hide the section control lookahead widget; it should still work
    ui->openGLControlBack->hide();

    //get pointer to root QML object, which is the OpenGLControl,
    //store in a member variable for future use.
    qml_root = qmlview->rootObject();
    //attach callback to signal so we can interact with the control
    //once it is realized.
    connect(qml_root,SIGNAL(controlSet(OpenGLControl *)),this,
            SLOT(openGLControl_set(OpenGLControl*)));

    //save a copy of this item because we'll need it to request updates
    //when GPS data comes in (or on a timer for FPS).
    openGLControl_item = qobject_cast<OpenGLControlItem *>(qml_root);

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
    for(int i=1; i < MAXSECTIONS; i++){
        std::cout << (QString("section") + QString::number(i)).toStdString() << std::endl;
        sectionButton[i] = qmlItem(qml_root,QString("section")+QString::number(i));
        sectionButton[i]->setProperty("state","off");
        connect(sectionButton[i],SIGNAL(clicked()),
                sectionButtonsSignalMapper,
                SLOT(map()));
        sectionButtonsSignalMapper->setMapping(sectionButton[i], i);
    }
    connect(sectionButtonsSignalMapper,SIGNAL(mapped(int)),this,
            SLOT(onSectionButton_clicked(int)));

    txtDistanceOffABLine = qmlItem(qml_root,"txtDistanceOffABLine");

}

void FormGPS::openGLControl_set(OpenGLControl *c){
    openGLControl = c;
    std::cout << "Apparently the renderer is activated now." << std::endl;

    //tell the control to call our main form function for drawing.
    c->registerInitCallback(std::bind(&FormGPS::openGLControl_Initialized, this, std::placeholders::_1));
    c->registerPaintCallback(std::bind(&FormGPS::openGLControl_Draw, this, std::placeholders::_1));
}

void FormGPS::onBtnMinMaxZoom_clicked(){
    std::cout<<"Min Max button clicked." <<std::endl;
}

void FormGPS::onBtnPerimeter_clicked(){
    std::cout<<"Perimeter button clicked." <<std::endl;
}

void FormGPS::onBtnAutoSteer_clicked(){
    std::cout<<"Autosteer button clicked." <<std::endl;
}

void FormGPS::onBtnFlag_clicked(){
    std::cout<<"flag button clicked." <<std::endl;
}

void FormGPS::onBtnABLine_clicked(){
    std::cout<<"abline button clicked." <<std::endl;
}

void FormGPS::onBtnContour_clicked(){
    std::cout<<"contour button clicked." <<std::endl;
}

void FormGPS::onBtnManualOffOn_clicked(){
    std::cout<<"Manual off on button clicked." <<std::endl;
}

void FormGPS::onBtnSectionOffAutoOn_clicked(){
    std::cout<<"Section off auto on button clicked." <<std::endl;
}

void FormGPS::onSectionButton_clicked(int section_num) {
    //just for kicks we will cycle the states.
    //demo code only.
    QObject* button = qmlItem(qml_root,QString("section")+QString::number(section_num));

    std::cout <<"section button " << section_num << " clicked." <<std::endl;

    QString state;
    state = button->property("state").toString();

    std::cout << state.toStdString() << std::endl;

    if(state == "on")
        button->setProperty("state",QVariant("auto"));
    else if (state == "auto")
        button->setProperty("state",QVariant("off"));
    else
        button->setProperty("state",QVariant("on"));
}

void FormGPS::onBtnTiltDown_clicked(){
    std::cout<<"TiltDown button clicked." <<std::endl;
}

void FormGPS::onBtnTiltUp_clicked(){
    std::cout<<"TiltUp button clicked." <<std::endl;
}

void FormGPS::onBtnZoomIn_clicked(){
    std::cout<<"ZoomIn button clicked." <<std::endl;
}

void FormGPS::onBtnZoomOut_clicked(){
    std::cout<<"ZoomOut button clicked." <<std::endl;
}

void FormGPS::onBtnSnap_clicked(){
    std::cout<<"Snap button clicked." <<std::endl;
}

void FormGPS::onBtnTripOdometer_clicked(){
    std::cout<<"TripOdometer button clicked." <<std::endl;
}

void FormGPS::onBtnGPSData_clicked(){
    std::cout<<"GPSData button clicked." <<std::endl;
}

void FormGPS::onBtnSettings_clicked(){
    std::cout<<"Settings button clicked." <<std::endl;
}

void FormGPS::onBtnJob_clicked(){
    std::cout<<"Job button clicked." <<std::endl;
}

void FormGPS::onBtnBoundaryMenu_clicked(){
    std::cout<<"BoundaryMenu button clicked." <<std::endl;
}

void FormGPS::onBtnComm_clicked(){
    std::cout<<"Comm button clicked." <<std::endl;
}

void FormGPS::onBtnUnits_clicked(){
    std::cout<<"Units button clicked." <<std::endl;
}

void FormGPS::onBtnFileExplorer_clicked(){
    std::cout<<"FileExplorer button clicked." <<std::endl;
}

void FormGPS::onBtnAutoSteerConfig_clicked(){
    std::cout<<"AutoSteerConfig button clicked." <<std::endl;
}

