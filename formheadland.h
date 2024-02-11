#ifndef FORMHEADLAND_H
#define FORMHEADLAND_H

#include <QObject>
#include <QVector>
#include "vec3.h"

class CBoundary;
class CHeadLine;
class CTool;
class CVehicle;


class FormHeadland : public QObject
{
    Q_OBJECT
protected:
    double maxFieldDistance;
    double fieldCenterX;
    double fieldCenterY;

    //Point fixPt;
    bool isA = true;
    int start = 99999, end = 99999;
    int bndSelect = 0, mode;
    QVector<Vec3> sliceArr;
    QVector<Vec3> backupList;

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

public slots:
    void setFieldInfo(double maxFieldDistance,
                      double fieldCenterX,
                      double fieldCenterY);

    void load_headline();

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
