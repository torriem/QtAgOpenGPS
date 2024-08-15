#include "formloop.h"
#include "agioproperty.h"
#include "qmlutil.h"
#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include "qmlsettings.h"

extern QMLSettings qml_settings;

FormLoop::FormLoop(QObject *parent) : QObject(parent),
    qml_root(parent),
    wwwNtrip("192.168.1.100", 2101, 2102), // Example initial values
    ethUDP("192.168.1.101", 2201, 2202),
    ethModulesSet("255.255.255.255", 2301, 2302)
{

	qml_settings.setupKeys();
	qml_settings.loadSettings();

	setupGUI();
	loadSettings();
    //loadSettings();

    LoadLoopback();
	LoadUDPNetwork();
   //buffer.resize(1024);
    ConfigureNTRIP();

    halfSecondTimer = new QTimer(this);
    halfSecondTimer->setInterval(500);
    connect(halfSecondTimer, &QTimer::timeout, this, &FormLoop::timer1_Tick);
    //halfSecondTimer->start();

    oneSecondTimer = new QTimer(this);
    oneSecondTimer->setInterval(1000);
    connect(oneSecondTimer, &QTimer::timeout, this, &FormLoop::oneSecondLoopTimer_Tick);
    oneSecondTimer->start();


	twoSecondTimer = new QTimer(this);
	twoSecondTimer->setInterval(2000);
	connect(twoSecondTimer, &QTimer::timeout, this, &FormLoop::TwoSecondLoop);
	twoSecondTimer->start();

    tmr = new QTimer(this);// the timer used in formloop_ntripcomm
    tmr->setSingleShot(false);
    tmr->setInterval(5000);
    connect(tmr, &QTimer::timeout, this, &FormLoop::SendGGA);
    tmr->start();

    ntripMeterTimer = new QTimer(this);
    ntripMeterTimer->setSingleShot(false);
    ntripMeterTimer->setInterval(50);
    connect(ntripMeterTimer, &QTimer::timeout, this, &FormLoop::ntripMeterTimer_Tick);


    clientSocket = new QTcpSocket(this);

    FormUDp_Load();
    swFrame.start();

    if (settings.value("UDPComm/ListenOnly").toBool()) qDebug() << "Listen Only mode activated! Will not send to modules!";
}

FormLoop::~FormLoop()
{
    /* clean up our dynamically-allocated
     * objects.
     */
}
void FormLoop::oneSecondLoopTimer_Tick(){
    DoNTRIPSecondRoutine();
	UpdateUIVars();
}

void FormLoop::TwoSecondLoop()
{
	//Hello Alarm logic
	DoHelloAlarmLogic();
	DoTraffic();

	//send a hello to modules
    SendUDPMessage(helloFromAgIO, ethUDP.address, ethUDP.portToSend);
}

void FormLoop::DoTraffic()
{
	traffic.helloFromMachine++;
	traffic.helloFromAutoSteer++;
	traffic.helloFromIMU++;

    traffic.cntrGPSOut = 0;
}

void FormLoop::DoHelloAlarmLogic()
{
    bool currentHello;
    agio = qmlItem(qml_root, "agio");

    if (isConnectedMachine)
    {
        currentHello = traffic.helloFromMachine < 3;

        //check if anything changed, IE if currentHello is greater than
        //3 or not
        if (currentHello != lastHelloMachine)
        {
            agio->setProperty("machineConnected", currentHello); //set qml

            if (currentHello)
                qDebug() << "Connected to machine";
            else qDebug() << "Not connected to machine";

            lastHelloMachine = currentHello;
            ShowAgIO();
        }
    }
    // traffic.helloFromMachine starts at 99
    //so this is triggered when a machine board is connected
    else if (traffic.helloFromMachine < 90)
        isConnectedMachine = true;

    if (isConnectedSteer)
    {
        currentHello = traffic.helloFromAutoSteer < 3;

        if (currentHello != lastHelloAutoSteer)
        {
            agio->setProperty("steerConnected", currentHello);

            if (currentHello) qDebug() << "Connected to steer";
            else qDebug() << "Not connected to steer";

            lastHelloAutoSteer = currentHello;

            ShowAgIO();
        }
    }
    else if (traffic.helloFromAutoSteer < 90)
        isConnectedSteer = true;

    if (isConnectedIMU)
    {
        currentHello = traffic.helloFromIMU < 3;

        if (currentHello != lastHelloIMU)
        {
            agio->setProperty("imuConnected", currentHello);

            if (currentHello) qDebug() << "Connected to IMU";
            else qDebug() << "Not connected to IMU";

            lastHelloIMU = currentHello;
            ShowAgIO();
        }
    }
    else if (traffic.helloFromIMU < 90)
        isConnectedIMU = true;

    currentHello = traffic.cntrGPSOut != 0;

    //I suppose we could do the logic to not go red if
    //no GPS  was ever connected. But what's the point of AgIO if
    //no GPS is connected?
    if (currentHello != lastHelloGPS)
    {
        agio->setProperty("gpsConnected", currentHello);

        if (currentHello) qDebug() << "Connected to GPS";
        else qDebug() << "Not connected to GPS";

        lastHelloGPS = currentHello;

        ShowAgIO();
    }
}

