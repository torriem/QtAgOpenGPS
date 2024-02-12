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

class HeadlandDesigner;


class FormHeadland : public QObject
{
    Q_OBJECT
protected:
    double maxFieldDistance;
    double fieldCenterX;
    double fieldCenterY;

    //Point fixPt;
    InterfaceProperty<HeadlandDesigner,bool> isA = InterfaceProperty<HeadlandDesigner,bool>("isA");
    int start = 99999, end = 99999;
    int bndSelect = 0, mode;
    QVector<Vec3> sliceArr;
    QVector<Vec3> backupList;

    InterfaceProperty<HeadlandDesigner,int> sliceCount = InterfaceProperty<HeadlandDesigner,int>("sliceCount");
    InterfaceProperty<HeadlandDesigner,int> backupCount = InterfaceProperty<HeadlandDesigner,int>("backupCount");
    InterfaceProperty<HeadlandDesigner,int> hdlIndex = InterfaceProperty<HeadlandDesigner,int>("hdlIndex");
    InterfaceProperty<HeadlandDesigner,bool> curveLine = InterfaceProperty<HeadlandDesigner,bool>("curveLine");
    InterfaceProperty<HeadlandDesigner,double> lineDistance = InterfaceProperty<HeadlandDesigner,double>("lineDistance");

    bool zoomToggle;
    double zoom = 1, sX = 0, sY = 0;

    Vec3 pint = Vec3(0,1,0);

public:
    CBoundary *bnd;
    CHeadLine *hdl;
    CTool *tool;
    CVehicle *vehicle;
    QObject *qml_object;

    explicit FormHeadland(QObject *parent = nullptr);

    //this class is pretty closely coupled to the QML file
    //of necessity
    void connect_ui(QObject *headland_designer_instance);

public slots:
    void setFieldInfo(double maxFieldDistance,
                      double fieldCenterX,
                      double fieldCenterY);

    void load_headline();
    void clicked(int mouseX, int mouseY);

    void ogl_resize();
    void ogl_paint();
    void ogl_initialize();


signals:
    void turnOffAutoSteerBtn();
    void turnOffYouTubeBtn();
    void timedMessageBox(int, QString, QString);
    void saveHeadland();
};

#endif // FORMHEADLAND_H
