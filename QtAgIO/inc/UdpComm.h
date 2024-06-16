#include "UdpComm.h"
#include <QHostAddress>
#include <QNetworkDatagram>

class  CTraffic : public QObject
{
	public: 
        int cntrGPSIn = 0;
        int cntrGPSInBytes = 0;
        int cntrGPSOut = 0;

        unsigned int helloFromMachine = 99, helloFromAutoSteer = 99, helloFromIMU = 99;
};

class CScanReply : public QObject
{
	public:
        char steerIP =   "";
        char machineIP = "";
        char GPS_IP =    "";
        char IMU_IP =    "";
        char subnetStr = "";

        QByteArray subnet = { 0, 0, 0 };

        bool isNewSteer, isNewMachine, isNewGPS, isNewIMU;

        bool isNewData = false;
};

class UDP : public QObject
{
public: 
	bool isUDPNetworkConnected;

	//skiped the endpointj 
	//ditto the classes
	
	QByteArray ipAutoSet = "192.168.5";


    void LoadUDPNetwork();
	void SendDataLoopAsync(IAsyncResult* asyncResult);
	void SendUDPMessageAOG(QByteArray byteData, IPEndPoint endPoint);

private:

    //data stream
	QByteArray buffer = new QByteArray(1024);

    QByteArray helloFromAgIO = { 0x80, 0x81, 0x7F, 200, 3, 56, 0, 0, 0x47 };
	void LoadLoopback();
	void SendToLoopbackMessageAOG(QByteArray byteData);
	void SendDataToLoopback(QByteArray byteData, epAgOpen);
	void ReceiveFromLoopback(QByteArray data);
	void ReceiveDataLoopAsync(IAsyncResult* asyncResult);
	void SendDataUDPAsync(IAsyncResult* asyncResult);
	void ReceiveDataUdpAsync(IAsyncResult* asyncResult);
	void ReceiveFromUDP(QByteArray data);
};

