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
    //We're supposed to be compatible with the saved data
    //from this version of AOG:
    QCoreApplication::setApplicationVersion("4.1.0");
    qmlRegisterType<AOGRendererInSG>("AgOpenGPS",1,0,"AOGRenderer");
    FormGPS w;
    //w.show();

    USE_SETTINGS;
    if (SETTINGS_DISPLAY_SHOWBACK) {
        grnPixelsWindow = new QLabel("Back Buffer");
        grnPixelsWindow->setFixedWidth(500);
        grnPixelsWindow->setFixedHeight(500);
        grnPixelsWindow->show();
    }

    /*
    //testing to see how locale affects numbers in the stream writer
    QFileInfo testit("/tmp/noexistant/file.txt");
    qDebug() << testit.baseName();
    qDebug() << testit.suffix();
QFile testFile("/tmp/test.txt");
    testFile.open(QIODevice::WriteOnly);
    QTextStream writer(&testFile);
    writer << "Testing" << Qt::endl;
    writer << qSetFieldWidth(3) << (double)3.1415926535 << Qt::endl;
    testFile.close();
    */

    return a.exec();
}
#endif
