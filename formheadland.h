#ifndef FORMHEADLAND_H
#define FORMHEADLAND_H

#include <QObject>
#include <QVector>
#include "vec3.h"
#include "interfaceproperty.h"
#include <QTimer>

class CBoundary;
class CHeadLine;
class CTool;
class CVehicle;
class QOpenGLFunctions;

class HeadlandDesigner;


class FormHeadland : public QObject
{
    Q_OBJECT
protected:
    //InterfaceProperty<HeadlandDesigner,double> maxFieldDistance = InterfaceProperty<HeadlandDesigner,double>("maxFieldDistance");
    //InterfaceProperty<HeadlandDesigner,double> fieldCenterX = InterfaceProperty<HeadlandDesigner,double>("fieldCenterX");
    //InterfaceProperty<HeadlandDesigner,double> fieldCenterY = InterfaceProperty<HeadlandDesigner,double>("fieldCenterY");

    double maxFieldDistance = 0;
    double fieldCenterX = 0;
    double fieldCenterY = 0;

    InterfaceProperty<HeadlandDesigner,bool> showa = InterfaceProperty<HeadlandDesigner,bool>("showa");
    InterfaceProperty<HeadlandDesigner,bool> showb = InterfaceProperty<HeadlandDesigner,bool>("showb");
    InterfaceProperty<HeadlandDesigner,QPoint> apoint = InterfaceProperty<HeadlandDesigner,QPoint>("apoint");
    InterfaceProperty<HeadlandDesigner,QPoint> bpoint = InterfaceProperty<HeadlandDesigner,QPoint>("bpoint");
    InterfaceProperty<HeadlandDesigner,QPoint> vehiclePoint = InterfaceProperty<HeadlandDesigner,QPoint>("vehiclePoint");

    InterfaceProperty<AOGInterface,bool> isBtnAutoSteerOn = InterfaceProperty<AOGInterface,bool>("isBtnAutoSteerOn");
    InterfaceProperty<AOGInterface,bool> isYouTurnBtnOn = InterfaceProperty<AOGInterface,bool>("isYouTurnBtnOn");

    int fixX, fixY;
    //InterfaceProperty<HeadlandDesigner,bool> isA = InterfaceProperty<HeadlandDesigner,bool>("isA");
    bool isA = false;
    bool draw = false;
    int start = 99999, end = 99999;
    int bndSelect = 0, mode;
    QVector<Vec3> sliceArr;
    QVector<Vec3> backupList;

    InterfaceProperty<HeadlandDesigner,int> sliceCount = InterfaceProperty<HeadlandDesigner,int>("sliceCount");
    InterfaceProperty<HeadlandDesigner,int> backupCount = InterfaceProperty<HeadlandDesigner,int>("backupCount");
    InterfaceProperty<HeadlandDesigner,bool> curveLine = InterfaceProperty<HeadlandDesigner,bool>("curveLine");
    InterfaceProperty<HeadlandDesigner,double> lineDistance = InterfaceProperty<HeadlandDesigner,double>("lineDistance");

    bool zoomToggle;
    InterfaceProperty<HeadlandDesigner,double> zoom = InterfaceProperty<HeadlandDesigner,double>("zoom");
    InterfaceProperty<HeadlandDesigner,double> sX = InterfaceProperty<HeadlandDesigner,double>("sX");
    InterfaceProperty<HeadlandDesigner,double> sY = InterfaceProperty<HeadlandDesigner,double>("sY");

    Vec3 pint = Vec3(0,1,0);

    QTimer updateVehiclePositionTimer;

    void SetLineDistance();
    QVector3D mouseClickToField(int mouseX, int mouseY);

    void setup_matrices(QMatrix4x4 &modelview, QMatrix4x4 &projection);

public:
    double iE = 0, iN = 0;
    QVector<int> crossings;

    CBoundary *bnd;
    CHeadLine *hdl;
    CTool *tool;
    CVehicle *vehicle = NULL;
    QObject *headland_designer_instance;

    explicit FormHeadland(QObject *parent = nullptr);

    //this class is pretty closely coupled to the QML file
    //of necessity
    void connect_ui(QObject *headland_designer_instance);

public slots:
    void setFieldInfo(double maxFieldDistance,
                      double fieldCenterX,
                      double fieldCenterY);

    void load_headline();
    void update_lines(); //update the boundary lines in GUI
    void update_headland(); //update headland line
    void update_slice(); //update slice and A B points in GUI
    void FormHeadLine_FormClosing();
    void clicked(int mouseX, int mouseY);

    void btn_Exit_Click();
    void isSectionControlled(bool wellIsIt);
    void btnBndLoop_Click();
    void btnSlice_Click();
    void btnDeletePoints_Click();
    void btnUndo_Click();
    void btnALength_Click();
    void btnBLength_Click();
    void btnBShrink_Click();
    void btnAShrink_Click();
    void btnHeadlandOff_Click();

    void updateVehiclePosition();

signals:
    void timedMessageBox(int, QString, QString);
    void saveHeadland();
};

#endif // FORMHEADLAND_H
