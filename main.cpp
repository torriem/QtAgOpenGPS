#include "formgps.h"
#include <QApplication>
#include <QCoreApplication>
#include <QLabel>

#include "aogrenderer.h"

QLabel *grnPixels;

#ifndef TESTING
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont f = a.font();
    f.setPointSize(16);
    a.setFont(f);
    QCoreApplication::setOrganizationName("AgOpenGPS");
    QCoreApplication::setOrganizationDomain("qtagopengps");
    QCoreApplication::setApplicationName("QtAgOpenGPS");
    qmlRegisterType<AOGRendererInSG>("AgOpenGPS",1,0,"AOGRenderer");
    FormGPS w;
    w.show();
    //grnPixels = new QLabel("Hello");
    //grnPixels->setFixedWidth(500);
    //grnPixels->setFixedHeight(500);
    //grnPixels->show();
    return a.exec();
}
#endif
