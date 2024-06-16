#include "UdpComm.h"
#include <QHostAddress>
#include <QNetworkDatagram>


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

