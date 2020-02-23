#ifndef CMAZEGRID_H
#define CMAZEGRID_H

#include <QVector>
#include "vec3.h"

class QOpenGLFunctions;
class QMatrix4x4;
class CGeoFence;
class CBoundary;

class CMazeGrid
{
public:
    int *mazeArr;
    int mazeScale = 1;
    int mazeRowYDim;
    int mazeColXDim;

    CMazeGrid();
    ~CMazeGrid();

    void buildMazeGridArray(const CBoundary &bnd, CGeoFence &gf,
                            double minFieldX, double maxFieldX,
                            double minFieldY, double maxFieldY);

    QVector<Vec3> searchForPath(double minFieldX, double minFieldY,
                                const Vec3 start, const Vec3 stop);

    void drawArr(QOpenGLFunctions *gl, const QMatrix4x4 &mvp,
                 double minFieldX, double minFieldY);


};

#endif // CMAZEGRID_H
