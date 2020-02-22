#ifndef CMAZEPATH_H
#define CMAZEPATH_H

#include <QVector>
#include "vec3.h"

class CMazePath
{
private:
    int *mazeGrid;
    int numCols;
    int numMax;

    enum mazePathStatus {
        Ready, Waiting, Processed
    };

public:
    CMazePath(int iRows, int iCols, int *_iMaze);
    QVector<Vec3> search (int iFromY, int iFromX, int iToY, int iToX);

};

#endif // CMAZEPATH_H
