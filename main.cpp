#include "formgps.h"
#include <QApplication>
#include <QCoreApplication>

#include "aogrenderer.h"

#ifndef TESTING
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont f = a.font();
    f.setPointSize(16);
    a.setFont(f);
    //glutInit(&argc,argv);
    QCoreApplication::setOrganizationName("AgOpenGPS");
    QCoreApplication::setOrganizationDomain("agopengps");
    QCoreApplication::setApplicationName("QtAgOpenGPS");
    qmlRegisterType<AOGRendererInSG>("AgOpenGPS",1,0,"AOGRenderer");
    FormGPS w;
    w.show();

    return a.exec();
}
#endif
