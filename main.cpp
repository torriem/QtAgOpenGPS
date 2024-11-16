// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// main
#include "formgps.h"
#include <QApplication>
#include <QCoreApplication>
#include <QLabel>
#include "aogrenderer.h"
#include "aogproperty.h"
#include <QProcess>
#include <QSysInfo>

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
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat,
                       QSettings::UserScope,
                       QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    //We're supposed to be compatible with the saved data
    //from this version of AOG:
    QCoreApplication::setApplicationVersion("4.1.0");
    qmlRegisterType<AOGRendererInSG>("AgOpenGPS",1,0,"AOGRenderer");
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    qRegisterMetaTypeStreamOperators<QList<int> >("QList<int>");
    qRegisterMetaTypeStreamOperators<QVector<int> >("QVector<int>");
#endif
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
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

    //Returns "android" on Android, "windows" on Windows. See docs https://doc.qt.io/qt-6/qsysinfo.html#productType
    QString osType = QSysInfo::productType();
    qInfo() << "Os Type: " << osType;
    if (osType == "android")
        w.isAndroid = true;
    else if (osType == "windows")
        w.isWindows = true;

    //else assume Linux. Can add more if needed

    QProcess process;
    //auto start AgIO
    if((!w.isAndroid) && property_setFeature_isAgIOOn){

        //QObject::connect(&process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                        // &w, &QCoreApplication::quit);

        QObject::connect(&process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                         [&a](int exitCode, QProcess::ExitStatus exitStatus) {
                             qDebug() << "AgIO process finished with exit code:" << exitCode
                                      << "and status:" << (exitStatus == QProcess::NormalExit ? "Normal" : "Crashed");
                             qDebug() << "Closing main application...";
                             qDebug() << "Note to Michael: This is happening in Main.cpp line 65. Remove this line if this is how you want it, or the whole 'feature' if it isn't";
                             QCoreApplication::quit(); // Quit the application
                         });


        QObject::connect(&process, &QProcess::errorOccurred, [&](QProcess::ProcessError error) {
            if (error == QProcess::Crashed) {
                qDebug() << "AgIO Crashed! Quitting QtAgOpenGPS";
                w.quit();  // Quit the main application if the process crashes
            }
        });

        process.start("./QtAgIO/QtAgIO");

        // Ensure process starts successfully
        if (!process.waitForStarted()) {
            qWarning() << "AgIO failed to start. Quitting QtAgOpenGPS";
            return -1;  // Exit if process fails to start
        }
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
