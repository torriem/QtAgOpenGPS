#include <QQuickView>
#include <QQuickWidget>
#include "formgps.h"
#include "ui_formgps.h"
#include "cworldgrid.h"
#include "cnmea.h"
#include "csection.h"
#include "cabline.h"
#include "ccontour.h"
#include "cvehicle.h"
#include "mainoverlay.h"
#include <iostream>
#include "openglcontrolitem.h"
#include "openglcontrol.h"
#include "qmlutil.h"


FormGPS::FormGPS(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FormGPS)
{
    setupGui();

    /* initialize child objects */
    /* these objects need pointers to this form
     * to retrieve information. they are tightly
     * coupled!  So we have to create them here, since
     * the header file has only incomplete types.
     */
    worldGrid = new CWorldGrid(this);
    pn = new CNMEA(this);
    section = new CSection[MAXSECTIONS];
    for (int i = 0; i < MAXSECTIONS; i++)
        section->set_mainform(this);

    ABLine = new CABLine(this);
    ct = new CContour(this);
    vehicle = new CVehicle(this);
}

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

    //connect qml button signals to callbacks (it's not automatic with qml)
    btnMinMaxZoom = qmlItem(qml_root,"btnMinMaxZoom");
    //temp is NULL if child is not found but we'd get a warning to stdout
    connect(btnMinMaxZoom,SIGNAL(clicked()),this,
            SLOT(onBtnMinMaxZoom_clicked()));

    btnPerimeter = qmlItem(qml_root,"btnPerimeter");
    connect(btnPerimeter,SIGNAL(clicked()),this,
            SLOT(onBtnPerimeter_clicked()));


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


}
FormGPS::~FormGPS()
{
    /* clean up our dynamically-allocated
     * objects.
     */
    delete ui;
    delete worldGrid;
    delete pn;
    delete[] section;
    delete ABLine;
    delete ct;
    delete vehicle;

}

void FormGPS::openGLControl_set(OpenGLControl *c){
    openGLControl = c;
    std::cout << "Apparently the renderer is activated now." << std::endl;
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

void FormGPS::setZoom()
{
}

