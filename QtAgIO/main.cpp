#include <QApplication>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
#include <QStandardPaths>
#include "formloop.h"
#include "agiosettings.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("QtAgOpenGPS");
    QCoreApplication::setOrganizationDomain("qtagopengps");
    QCoreApplication::setApplicationName("QtAgIO");
    QCoreApplication::setApplicationVersion("0.0.1");


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


    // Check for settings and generate defaults if none present

    //IORouter router(settings);
    AgIOSettings settings;
    FormLoop formLoop;
	

	
	return app.exec();
}

