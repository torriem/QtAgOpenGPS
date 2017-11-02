#include "formgps.h"
#include <QApplication>
#include <QCoreApplication>

#include "openglcontrolitem.h"

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
    qmlRegisterType<OpenGLControlItem>("AgOpenGPS",1,0,"OpenGLControl");
    FormGPS w;
    w.show();

    return a.exec();
}
#endif
