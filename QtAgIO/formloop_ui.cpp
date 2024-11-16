#include <QQuickView>
#include <QQuickWidget>
#include <QQmlContext>
#include <QScreen>
#include <QGuiApplication>
#include <QQmlEngine>
#include "qmlutil.h"
#include "formloop.h"
#include "interfaceproperty.h"
#include "qmlsettings.h"
#include <QHostInfo>
#include "agioproperty.h"

extern QMLSettings qml_settings;

void FormLoop::setupGUI()
{
    //Load the QML UI and display it in the main area of the GUI
    setProperty("title", "QtAgIO");

    //load the QML into a view
    engine.rootContext()->setContextProperty("screenPixelDensity",QGuiApplication::primaryScreen()->physicalDotsPerInch() * QGuiApplication::primaryScreen()->devicePixelRatio());
    engine.rootContext()->setContextProperty("mainForm", this);
	engine.rootContext()->setContextProperty("settings", &qml_settings);
    engine.rootContext()->setContextProperty("prefix", "");
    engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));



    if (engine.rootObjects().isEmpty())
    {
        qDebug() << "Failed to load QML file";
        return;
    }
    else
    {
        qDebug() << "Displaying QML File";
    }

    QList<QObject*> root_context = engine.rootObjects();

    qml_root = root_context.first();

    //have to do this for each Interface and supported data type.
    InterfaceProperty<AgIOInterface, int>::set_qml_root(qmlItem(qml_root, "agio"));
    InterfaceProperty<AgIOInterface, uint>::set_qml_root(qmlItem(qml_root, "agio"));
    InterfaceProperty<AgIOInterface, bool>::set_qml_root(qmlItem(qml_root, "agio"));
    InterfaceProperty<AgIOInterface, double>::set_qml_root(qmlItem(qml_root, "agio"));
    //InterfaceProperty<AgIOInterface, btnStates>::set_qml_root(qmlItem(qml_root, "aog"));

    agio = qmlItem(qml_root, "agio");

    //this is where the connects with the frontend happen

    //UDP
    connect(agio, SIGNAL(btnSendSubnet_clicked()), this, SLOT(btnSendSubnet_Click())); // btnSendSubnet_Click lives in formloop_formudp.cpp
    connect(agio, SIGNAL(btnUDPListenOnly_clicked(bool)), this, SLOT(btnUDPListenOnly_Click(bool)));

    //NTRIP
    connect(agio, SIGNAL(configureNTRIP()), this, SLOT(ConfigureNTRIP())); //ConfigureNTRIP lives in formloop_ntripcomm.cpp
    connect(agio, SIGNAL(ntripDebug(bool)), this, SLOT(NTRIPDebugMode(bool)));
    connect(agio, SIGNAL(setIPFromUrl(QString)), this, SLOT(LookupNTripIP(QString)));


}

void FormLoop::btnUDPListenOnly_Click(bool isIt){
    udpListenOnly = isIt;
    if(udpListenOnly){
        qDebug() << "UDP Listen Only Mode Activiate!! Will Not Send To Modules!!";
        TimedMessageBox(3000, tr("UDP Listen Only Mode Activated!!"), tr("Warning!! Will Not Send To Modules!!"));
    }else{
        qDebug() << "UDP Listen Only Mode Deactivated.";
        TimedMessageBox(1500, tr("UDP Listen Only Mode Deactivated"), "");
    }
}

void FormLoop::ShowAgIO(){
    if (!engine.rootObjects().isEmpty()) {
        QObject *rootObject = engine.rootObjects().first();
        QQuickWindow *window = qobject_cast<QQuickWindow *>(rootObject);

        if (window) {
            window->raise();
            window->requestActivate();
        } else {
            qDebug() << "Root object is not a QQuickWindow.";
        }
    } else {
        qDebug() << "No root objects found.";
    }
}

void FormLoop::LookupNTripIP(QString url){
    QHostInfo::lookupHost(url, this, SLOT(ResolveNTRIPURL(QHostInfo)));

}


void FormLoop::TimedMessageBox(int timeout, QString s1, QString s2)
{
    qDebug() << "Timed message " << timeout << s1 << ", " << s2 << Qt::endl;
    //TODO ask QML to display a message
    QObject *temp = qmlItem(qml_root, "timedMessage");
    QMetaObject::invokeMethod(temp, "addMessage", Q_ARG(int, timeout), Q_ARG(QString, s1), Q_ARG(QString, s2));
}
void FormLoop::UpdateUIVars(){
	agio->setProperty("latitude", latitudeSend);
	agio->setProperty("longitude", longitudeSend);
	agio->setProperty("altitude", altitudeData);//float
	agio->setProperty("speed", speedData); //
	agio->setProperty("heading", headingTrueData);//
	agio->setProperty("imuroll", rollData);//
    agio->setProperty("imupitch", imuPitchData);//
	agio->setProperty("age", ageData);//ushort
	agio->setProperty("quality", fixQuality);//quint8
	agio->setProperty("sats", satellitesTracked);//ushort
    agio->setProperty("yawrate", imuYawRate);//short
	agio->setProperty("gga", ggaSentence);
	agio->setProperty("vtg", vtgSentence);
	agio->setProperty("panda", pandaSentence);
	agio->setProperty("paogi", paogiSentence);
	agio->setProperty("hdt", hdtSentence);
	agio->setProperty("avr", avrSentence);
	agio->setProperty("hpd", hpdSentence);
	agio->setProperty("sxt", ksxtSentence);

    //error checking altitude. If we change wildly, something wrong.
    //most likely two gps sources
    if(nmeaError){

        if (nmeaErrCounter>10) nmeaErrCounter = 0;

        nmeaErrCounter++ ;
        agio->setProperty("nmeaError", nmeaErrCounter);
        nmeaError = false;
    }
}

