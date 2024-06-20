#include "CTraffic.h"

/* This is a storage class
 * It is used by UDPComm.cpp*/


CTraffic::CTraffic(QObject *parent)
	: QObject(parent), 
	  cntrGPSIn(0),
	  cntrGPSInBytes(0),
	  cntrGPSOut(0),
	  helloFromMachine(99),
	  helloFromAutoSteer(99),
	  helloFromIMU(99)
{
	//optional: other initialization code
}
