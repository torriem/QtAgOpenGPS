#ifndef FormLoop_H
#define FormLoop_H

#include <QHostAddress>
#include <QNetworkDatagram>
#include <QString>
#include <QByteArray>
#include "CTraffic.h"
#include "CScanReply.h"
#include <QUdpSocket>


class FormLoop : public QObject
{

	Q_OBJECT

public: 
	explicit FormLoop(QObject *parent = nullptr);

	bool isUDPNetworkConnected;
	
	//QByteArray ipAutoSet = "192.168.5";
	QByteArray ipAutoSet;


	//set up the networks
    void LoadUDPNetwork();
	void LoadLoopback();

	//send
	void SendUDPMessage(QByteArray byteData);
    void SendDataToLoopBack(QByteArray byteData);

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

public slots:

    void ReceiveFromLoopBack();
    void ReceiveFromUDP();

};


#endif
