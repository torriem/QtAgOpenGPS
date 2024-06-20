#include <QApplication>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
#include <QStandardPaths>

#include "IORouter.h"

// Helper to generate default settings
void generateDefaultSettings(QSettings& settings)
{
    qWarning() << "Generating default settings";
    
    settings.clear();

    settings.setValue("AogUdpPort", 17777);
    settings.setValue("NumConnections", 2);
	settings.setValue("IP1", 192);
	settings.setValue("IP2", 168);
	settings.setValue("IP3", 5);

    settings.beginGroup("Connection0");
    settings.setValue("type", "UDP");
    settings.setValue("name", "SteerBoard");
    settings.setValue("host", "192.168.5.12");
    settings.setValue("port", 8888);
    settings.endGroup();

    settings.beginGroup("Connection1");
    settings.setValue("type", "Serial");
    settings.setValue("name", "GPSReceiver");
    settings.setValue("port", "/dev/ttyACM0");
    settings.setValue("baud", 9600);
    settings.endGroup();

	settings.beginGroup("Connection2");
	settings.setValue("type", "RTK");
	settings.setValue("name", "NTRIP");
	settings.setValue("url", "rtk2go.com");
	settings.setValue("mount", "OverlandRTK");
	settings.setValue("username", "johndoe@gmail.com");
	settings.setValue("password", "password");
	settings.setValue("port", 2101);
	settings.endGroup();


    settings.sync();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("QtAgOpenGPS");
    QCoreApplication::setOrganizationDomain("qtagopengps");
    QCoreApplication::setApplicationName("QtAgIO");
    QCoreApplication::setApplicationVersion("0.0.1");

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat,
                       QSettings::UserScope,
                       QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    QCommandLineParser parser;
    parser.setApplicationDescription("IO handler for AgOpenGPS and QtAgOpenGPS");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption verbose("verbose", "Set verbosity level, 0-7. 0=silent, 7=noisy, 2=default", "verbose", "2");
    parser.addOption(verbose);

    parser.process(app);

    // I don't think this does what I think it does.  Can I set verbosity of the default object?
    // Does that do anything or is verbosity only useful for diy streams?
    qDebug().setVerbosity(parser.value(verbose).toInt());

    QSettings settings;

    // Check for settings and generate defaults if none present
    if(!settings.contains("AogUdpPort"))
    {
        generateDefaultSettings(settings);
    }

	qDebug() << "Starting IORouter";
    IORouter router(settings);

	
	return app.exec();
}

