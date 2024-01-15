#include "formgps.h"
#include <QApplication>
#include <QCoreApplication>
#include <QLabel>
#include "aogrenderer.h"
#include "aogproperty.h"

QLabel *grnPixelsWindow;
AOGSettings *settings;

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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    qRegisterMetaTypeStreamOperators<QList<int> >("QList<int>");
    qRegisterMetaTypeStreamOperators<QVector<int> >("QVector<int>");
#endif
    settings = new AOGSettings();
    AOGProperty::init_defaults();
    settings->sync();
    FormGPS w;
    //w.show();

    if (property_displayShowBack) {
        grnPixelsWindow = new QLabel("Back Buffer");
        grnPixelsWindow->setFixedWidth(500);
        grnPixelsWindow->setFixedHeight(500);
        grnPixelsWindow->show();
    }

    /*
    CDubinsTurningRadius = 5.25;

    CDubins c;
    Vec3 start(0,0,0);
    Vec3 goal (8,0,0);
    QVector<Vec3> pathlist;
    pathlist = c.GenerateDubins(start, goal);

    foreach(Vec3 goal: pathlist) {
        qDebug() << goal.easting<< ", "<<goal.northing;
    }
    return 0;
    */

    //Test file I/O
    //w.fileSaveCurveLines();
    //w.fileSaveBoundary();
    //w.fileSaveABLines();
    //w.fileSaveContour();
    //w.fileSaveVehicle("/tmp/TestVehicle.txt");
    //w.fileOpenField("49111 1 1 2020.Mar.21 09_58");
    //w.ABLine.isBtnABLineOn = true;
    //w.hd.isOn = true;
    //w.yt.isYouTurnBtnOn = true;

    //w.ABLine.isBtnABLineOn = true;
    //w.fileOpenTool("/tmp/TestTool1.txt");
    //w.fileOpenVehicle("/tmp/TestVehicle2.txt");
    //w.fileSaveTool("/tmp/TestTool.TXT");
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
