#include "formloop.h"

FormLoop::FormLoop(QObject *parent) 
	: QObject(parent)
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
