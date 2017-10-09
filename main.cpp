#include "formgps.h"
#include <QApplication>
#include <QCoreApplication>
#include <GL/glut.h>

#include "openglcontrolitem.h"

#ifndef TESTING
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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
