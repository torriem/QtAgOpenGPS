#ifndef CHEAD_H
#define CHEAD_H
#include <QObject>
#include "vec2.h"
#include <QVector>

class CHeadLines;
class CTool;
class CVehicle;
class QOpenGLFunctions;
class QMatrix4x4;



class CHead: public QObject
{
    Q_OBJECT
private:
    int A, B, C, D, Q;
public:
    double singleSpaceHeadlandDistance;
    bool isOn;
    double leftToolDistance;
    double rightToolDistance;

    //generated box for finding closest point
    Vec2 downL = Vec2(9000, 9000), downR = Vec2(9000, 9002);
    bool isToolUp, isToolInWorkArea, isToolInBothPlaces, isToolInHeadland;
    bool isLookInWorkArea, isLookInBothPlaces, isLookInHealand;

    // array of turns
    QVector<CHeadLines> headArr;

    CHead();
    bool findHeadlandDistance(const CVehicle &vehicle, const CTool &tool);
    void drawHeadLines(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
    bool isPointInsideHeadLine(Vec2 pt);
signals:
    //indicate that tool is to be lowered or raised
    void moveHydraulics(int);
};

#endif // CHEAD_H
