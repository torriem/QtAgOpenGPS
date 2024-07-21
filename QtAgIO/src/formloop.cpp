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
}

FormLoop::~FormLoop()
{
    /* clean up our dynamically-allocated
     * objects.
     */
}
