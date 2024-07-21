#include "formloop.h"

FormLoop::FormLoop(QObject *parent)
    : QObject(parent),
    wwwNtrip("192.168.1.100", 2101, 2102), // Example initial values
    ethUDP("192.168.1.101", 2201, 2202)    // Example
{
    //loadSettings();

    LoadLoopback();
	LoadUDPNetwork();
   //buffer.resize(1024);

    ConfigureNTRIP();

    oneSecondTimer = new QTimer(this);
    oneSecondTimer->setInterval(1000);
    connect(oneSecondTimer, &QTimer::timeout, this, &FormLoop::oneSecondLoopTimer_Tick);
    oneSecondTimer->start();

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
