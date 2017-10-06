#include "formgps.h"
#include <QApplication>

#ifndef TESTING
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FormGPS w;
    w.show();

    return a.exec();
}
#endif
