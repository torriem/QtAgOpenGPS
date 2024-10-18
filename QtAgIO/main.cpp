#include <QApplication>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDebug>
#include <QStandardPaths>
#include "formloop.h"
#include "agioproperty.h"

AgIOSettings *settings;


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


    settings = new AgIOSettings();
    AgIOProperty::init_defaults();
    settings->sync();

    FormLoop formLoop;
	

	
	return app.exec();
}

