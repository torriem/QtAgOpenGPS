#include "CScanReply.h"

/* This is a storage class
 * It is used by UDPComm.cpp*/

CScanReply::CScanReply(QObject *parent)
    : QObject(parent), 
	  steerIP(""),
      machineIP(""),
      GPS_IP(""),
      IMU_IP(""),
      subnetStr(""),
      subnet(3, 0), // Initialize with three bytes set to 0
      isNewSteer(false),
      isNewMachine(false),
      isNewGPS(false),
      isNewIMU(false),
      isNewData(false)
{
    // Optional: other initialization code
}
