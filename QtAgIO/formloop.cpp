#include "formloop.h"
#include <QQuickWindow>

FormLoop::FormLoop(QWidget *parent) : QQmlApplicationEngine(parent),
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
