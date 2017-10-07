#include "formgps.h"
#include <QApplication>
#include "openglcontrolitem.h"

#ifndef TESTING
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qmlRegisterType<OpenGLControlItem>("AgOpenGPS",1,0,"OpenGLControl");
    FormGPS w;
    w.show();

    return a.exec();
}
#endif
