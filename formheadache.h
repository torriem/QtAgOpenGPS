#ifndef FORMHEADLAND_H
#define FORMHEADLAND_H

#include <QObject>
#include <QVector>
#include "vec3.h"
#include "interfaceproperty.h"

class CBoundary;
class CHeadLine;
class CTool;
class CVehicle;
class QOpenGLFunctions;

class HeadacheDesigner;


class FormHeadache : public QObject
{
    Q_OBJECT
protected:
    InterfaceProperty<HeadacheDesigner,double> maxFieldDistance = InterfaceProperty<HeadacheDesigner,double>("maxFieldDistance");
    InterfaceProperty<HeadacheDesigner,double> fieldCenterX = InterfaceProperty<HeadacheDesigner,double>("fieldCenterX");
    InterfaceProperty<HeadacheDesigner,double> fieldCenterY = InterfaceProperty<HeadacheDesigner,double>("fieldCenterY");
    InterfaceProperty<HeadacheDesigner,bool> showa = InterfaceProperty<HeadacheDesigner,bool>("showa");
    InterfaceProperty<HeadacheDesigner,bool> showb = InterfaceProperty<HeadacheDesigner,bool>("showb");
    InterfaceProperty<HeadacheDesigner,QPoint> apoint = InterfaceProperty<HeadacheDesigner,QPoint>("apoint");
    InterfaceProperty<HeadacheDesigner,QPoint> bpoint = InterfaceProperty<HeadacheDesigner,QPoint>("bpoint");

    InterfaceProperty<AOGInterface,bool> isBtnAutoSteerOn = InterfaceProperty<AOGInterface,bool>("isBtnAutoSteerOn");
    InterfaceProperty<AOGInterface,bool> isYouTurnBtnOn = InterfaceProperty<AOGInterface,bool>("isYouTurnBtnOn");

    int fixX, fixY;
    //InterfaceProperty<HeadacheDesigner,bool> isA = InterfaceProperty<HeadacheDesigner,bool>("isA");
    bool isA = false;
    bool draw = false;
    int start = 99999, end = 99999;
    int bndSelect = 0, mode;
    QVector<Vec3> sliceArr;
    QVector<Vec3> backupList;

    InterfaceProperty<HeadacheDesigner,int> headacheCount = InterfaceProperty<HeadacheDesigner,int>("headacheCount");
    InterfaceProperty<HeadacheDesigner,bool> curveLine = InterfaceProperty<HeadacheDesigner,bool>("curveLine");
    InterfaceProperty<HeadacheDesigner,double> lineDistance = InterfaceProperty<HeadacheDesigner,double>("lineDistance");

    bool zoomToggle;
    InterfaceProperty<HeadacheDesigner,double> zoom = InterfaceProperty<HeadacheDesigner,double>("zoom");
    InterfaceProperty<HeadacheDesigner,double> sX = InterfaceProperty<HeadacheDesigner,double>("sX");
    InterfaceProperty<HeadacheDesigner,double> sY = InterfaceProperty<HeadacheDesigner,double>("sY");

    Vec3 pint = Vec3(0,1,0);
    bool isLinesVisible = true;

    QVector3D mouseClickToField(int mouseX, int mouseY);

    void setup_matrices(QMatrix4x4 &modelview, QMatrix4x4 &projection);

public:
    double iE = 0, iN = 0;
    QVector<int> crossings;

    CBoundary *bnd;
    CHeadLine *hdl;
    CTool *tool;
    CVehicle *vehicle;
    QObject *headache_designer_instance;

    explicit FormHeadache(QObject *parent = nullptr);

    //this class is pretty closely coupled to the QML file
    //of necessity
    void connect_ui(QObject *headache_designer_instance);

public slots:
    void setFieldInfo(double maxFieldDistance,
                      double fieldCenterX,
                      double fieldCenterY);

    void load_headline();
    void update_lines(); //update the boundary lines in GUI
    void update_headland(); //update headland line
    void update_ab(); //update slice and A B points in GUI

    void clicked(int mouseX, int mouseY);

    void btnCycleForward_Click();
    void btnCycleBackward_Click();
    void btnDeleteCurve_Click();

    void FormHeadLine_FormClosing();
    void btnExit_Click();
    void isSectionControlled(bool wellIsIt);
    void btnBndLoop_Click();
    void btnSlice_Click();
    //void btnDeletePoints_Click();
    void btnALength_Click();
    void btnBLength_Click();
    void btnBShrink_Click();
    void btnAShrink_Click();
    void btnHeadlandOff_Click();
    void btnCancelTouch_Click();
    void btnDeleteHeadland_Click();

signals:
    void timedMessageBox(int, QString, QString);
    void saveHeadlines();
    void loadHeadlines();
    void saveHeadland();
};

#endif // FORMHEADLAND_H
