#ifndef UDP_H
#define UDP_H

#include <QHostAddress>
#include <QNetworkDatagram>
#include <QString>
#include <QByteArray>
#include "CTraffic.h"
#include "CScanReply.h"
#include <QUdpSocket>


class UDP : public QObject
{

	Q_OBJECT

public: 
	explicit UDP(QObject *parent = nullptr);

	bool isUDPNetworkConnected;
	
	//QByteArray ipAutoSet = "192.168.5";
	QByteArray ipAutoSet;


    void LoadUDPNetwork();
	//void SendDataLoopAsync(IAsyncResult* asyncResult);
	//void SendUDPMessageAOG(QByteArray byteData, IPEndPoint endPoint);
	void SendUDPMessage(QByteArray byteData);

	void LoadLoopback();
	/*void ReceiveFromLoopBack(QByteArray data);
	void SendDataToLoopback(QByteArray byteData);
	void ReceiveFromUDP(QByteArray data);*/
	QUdpSocket *udpSocket;
	QUdpSocket *loopBackSocket;
	CTraffic traffic;
	CScanReply scanReply;

private:

    //data stream
	//QByteArray buffer(1024);
	QByteArray buffer;

    //QByteArray helloFromAgIO( 0x80, 0x81, 0x7F, 200, 3, 56, 0, 0, 0x47 );
	QByteArray helloFromAgIO;

	//void SendToLoopbackMessageAOG(QByteArray byteData);
	//void ReceiveDataLoopAsync(IAsyncResult* asyncResult);
	//void SendDataUDPAsync(IAsyncResult* asyncResult);
	//void ReceiveDataUdpAsync(IAsyncResult* asyncResult);

public slots:
    /*void ReceiveFromLoopback();
	void SendDataToLoopback();
	void ReceiveFromUDP();*/
	void ReceiveFromLoopBack(QByteArray data);
	void SendDataToLoopBack(QByteArray byteData);
	void ReceiveFromUDP(QByteArray data);

};


#endif
