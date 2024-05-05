#include "UdpConnection.h"

UdpConnection::UdpConnection(QString n, QHostAddress a, int p) : 
    Connection(n),
    udpSocket(nullptr)
{
    if(!udpSocket.bind(a, p))
    {
        qCritical() << "Failed to bind to UDP Socket " << a.toString();
    }

    QObject::connect(&udpSocket, &QUdpSocket::readyRead,
            this, &UdpConnection::handleReadyRead);
}

UdpConnection::~UdpConnection()
{
    //udpSocket.close();
}

void UdpConnection::handleReadyRead()
{
    //emit readyRead(this);
}
