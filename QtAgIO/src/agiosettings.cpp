// settings.cpp

#include "agiosettings.h"

AgIOSettings::AgIOSettings(QObject *parent)
    : QObject(parent)
{
// Define the global QSettings object

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat,
                       QSettings::UserScope,
                       QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
QSettings settings;


    if(!settings.contains("LoopbackComm/ListenToAOGPort"))
    {
        generateDefaultSettings(settings);
    } else {
        qDebug() << "Using pre-generated settings file";
    qDebug() << "QSettings file location:" << settings.fileName();
    }
}
// Helper to generate default settings
    void AgIOSettings::generateDefaultSettings(QSettings& settings)
{
    qWarning() << "Generating default settings";

    settings.clear();

    settings.beginGroup("UDPComm");
    settings.setValue("UdpListenPort", 9999);
    settings.setValue("UdpSendPort", 8888);
    settings.setValue("ListenOnly", false); //don't send to modules. for testing while using aog in the field
    settings.setValue("NumConnections", 2);
    settings.setValue("IP1", 192);
    settings.setValue("IP2", 168);
    settings.setValue("IP3", 5);
    settings.endGroup();

    settings.beginGroup("LoopbackComm");// setting the ip address to QHostAddress::LocalHost seems to work, on Linux at least
    settings.setValue("ListenToAOGPort", 17770);//this way we can use on the same device as aog
    settings.setValue("SendToAOGPort", 15550);
    settings.endGroup();

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
