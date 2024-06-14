#include "UdpSender.h"
#include <QDebug>

UDPSender::UDPSender(const QString& ipAddress, quint16 port, QObject* parent)
    : QObject(parent),
      udpSocket(new QUdpSocket(this)),
      serverAddress(QHostAddress(ipAddress)),
      serverPort(port)
{
}

UDPSender::~UDPSender()
{
    // QUdpSocket will be deleted automatically as it is a child of this QObject
}

void UDPSender::sendToAOG(const QByteArray& data)
{
    qint64 bytesSent = udpSocket->writeDatagram(data, serverAddress, serverPort);
    if (bytesSent == -1) {
        qDebug() << "Failed to send message:" << udpSocket->errorString();
        return;
    }
	qDebug() << "Sent " << bytesSent << " bytes to " << serverAddress.toString() << ":" << serverPort;
    //return (bytesSent == data.size());
	return;
}

