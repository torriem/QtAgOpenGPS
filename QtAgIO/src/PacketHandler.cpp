#include "PacketHandler.h"
#include "IORouter.h"
#include "UdpSender.h"

PacketHandler::PacketHandler(QObject *parent) : QObject(parent) {}

void PacketHandler::processPacket(const QByteArray &data, const QHostAddress &sender, quint16 senderPort) {
    //qDebug() << "Received data from" << sender.toString() << ":" << senderPort << "data:" << data;
	if(debugCounter > 10)
	{
		debugCounter = 0;
		qDebug() << sender.toString() << ":" << senderPort << "data:" << data;
	}
	debugCounter++;
	
	try
	{
			emit sendToAOG(data);
	}
	catch(...)
	{
	}
}
