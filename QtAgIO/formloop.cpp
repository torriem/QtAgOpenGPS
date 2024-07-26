#include "formloop.h"
#include <QQuickWindow>
#include <QQmlApplicationEngine>

FormLoop::FormLoop(QObject *parent) : QObject(parent),
    wwwNtrip("192.168.1.100", 2101, 2102), // Example initial values
    ethUDP("192.168.1.101", 2201, 2202)    // Example
{
    //loadSettings();

    LoadLoopback();
	LoadUDPNetwork();
   //buffer.resize(1024);
    setupGUI();
    ConfigureNTRIP();

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
}

FormLoop::~FormLoop()
{
    /* clean up our dynamically-allocated
     * objects.
     */
}
void FormLoop::oneSecondLoopTimer_Tick(){
    DoNTRIPSecondRoutine();
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

    if (isConnectedMachine)
    {
        currentHello = traffic.helloFromMachine < 3;

        //check if anything changed, IE if currentHello is greater than
        //3 or not
        if (currentHello != lastHelloMachine)
        {
            //if (currentHello) btnMachine.BackColor = Color.LimeGreen;
            //else btnMachine.BackColor = Color.Red;
            if (currentHello) qDebug() << "Connected to machine";
            else qDebug() << "Not connected to machine";
            lastHelloMachine = currentHello;
                //ShowAgIO();
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
            //if (currentHello) btnSteer.BackColor = Color.LimeGreen;
            //else btnSteer.BackColor = Color.Red;
            if (currentHello) qDebug() << "Connected to steer";
            else qDebug() << "Not connected to steer";
            lastHelloAutoSteer = currentHello;
                //ShowAgIO();
        }
    }
    else if (traffic.helloFromAutoSteer < 90)
        isConnectedSteer = true;

    if (isConnectedIMU)
    {
        currentHello = traffic.helloFromIMU < 3;

        if (currentHello != lastHelloIMU)
        {
            //if (currentHello) btnIMU.BackColor = Color.LimeGreen;
            //else btnIMU.BackColor = Color.Red;
            if (currentHello) qDebug() << "Connected to IMU";
            else qDebug() << "Not connected to IMU";
            lastHelloIMU = currentHello;
                //ShowAgIO();
        }
    }
    else if (traffic.helloFromIMU < 90)
        isConnectedSteer = true;

    currentHello = traffic.cntrGPSOut != 0;

    //I suppose we could do the logic to not go red if
    //no GPS  was ever connected. But what's the point of AgIO if
    //no GPS is connected?
    if (currentHello != lastHelloGPS)
    {
        //if (currentHello) btnGPS.BackColor = Color.LimeGreen;
        //else btnGPS.BackColor = Color.Red;
        if (currentHello) qDebug() << "Connected to GPS";
        else qDebug() << "Not connected to GPS";
        lastHelloGPS = currentHello;
            //ShowAgIO();
    }
}

