#include "formgps.h"
#include <QApplication>

#include "vec2.h"
#include <QVector>
#include <QSharedPointer>
#include <iostream>

typedef QVector<Vec2> TriangleList;

int main(int argc, char *arv[]) {
    QSharedPointer<TriangleList> triangleList;

    QVector<QSharedPointer<TriangleList>> patchList;

    std::cout << "Starting object creation" << std::endl;
    triangleList = QSharedPointer<TriangleList>(new TriangleList);
    patchList.append(triangleList);
    triangleList->append(Vec2(0,0));
    triangleList->append(Vec2(1,0));
    triangleList->append(Vec2(2,0));
    triangleList->append(Vec2(3,0));

    triangleList = QSharedPointer<TriangleList>(new TriangleList);
    patchList.append(triangleList);
    triangleList->append(Vec2(0,1));
    triangleList->append(Vec2(1,1));
    triangleList->append(Vec2(2,1));
    triangleList->append(Vec2(3,1));

    for (int l=0;l<patchList.size();l++) {
        std::cout << patchList[l]->size() << std::endl;
        for (int t=0; t < patchList[l]->size(); t++) {
            std::cout << (*patchList[l])[t].easting << " ";
            std::cout << (*patchList[l])[t].northing << std::endl;
        }

    }


}
/*
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FormGPS w;
    w.show();

    return a.exec();
}
*/
