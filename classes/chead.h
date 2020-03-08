#ifndef CHEAD_H
#define CHEAD_H
#include <QObject>
#include "vec2.h"
#include <QVector>
#include "cheadlines.h"

class CTool;
class CVehicle;
class QOpenGLFunctions;
class QMatrix4x4;



class CHead: public QObject
{
    Q_OBJECT
private:
public:
    double singleSpaceHeadlandDistance;
    bool isOn;
    double leftToolDistance;
    double rightToolDistance;

    //generated box for finding closest point
    Vec2 downL = Vec2(9000, 9000), downR = Vec2(9000, 9002);
    bool isToolInHeadland, isToolUp, isToolOuterPointsInHeadland;
    bool isToolLeftIn = false;
    bool isToolRightIn = false;
    bool isLookRightIn = false;
    bool isLookLeftIn = false;

    // array of turns
    QVector<CHeadLines> headArr;

    explicit CHead(QObject *parent = 0);
    void whereAreToolCorners(CTool &tool);
    void whereAreToolLookOnPoints(const CVehicle &vehicle, CTool &tool);
    void drawHeadLines(QOpenGLFunctions *gl, const QMatrix4x4 &mvp, int lineWidth);
    void drawHeadLinesBack(QOpenGLFunctions *gl, const QMatrix4x4 &mvp);
    bool isPointInsideHeadLine(Vec2 pt);
    void setHydPosition(const CVehicle &vehicle, double speed);
signals:
    //indicate that tool is to be lowered or raised
    void moveHydraulics(int);
};

#endif // CHEAD_H
