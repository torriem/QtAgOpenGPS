#include "formgps.h"
#include <QApplication>
#include <QCoreApplication>
#include <QLabel>

#include "aogrenderer.h"

QLabel *grnPixelsWindow;

#ifndef TESTING
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont f = a.font();
    f.setPointSize(16);
    a.setFont(f);
    QCoreApplication::setOrganizationName("QtAgOpenGPS");
    QCoreApplication::setOrganizationDomain("qtagopengps");
    QCoreApplication::setApplicationName("QtAgOpenGPS");
    qmlRegisterType<AOGRendererInSG>("AgOpenGPS",1,0,"AOGRenderer");
    FormGPS w;
    w.show();
    grnPixelsWindow = new QLabel("Hello");
    grnPixelsWindow->setFixedWidth(500);
    grnPixelsWindow->setFixedHeight(500);
    grnPixelsWindow->show();
    return a.exec();
}
#endif
