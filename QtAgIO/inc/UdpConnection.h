#pragma once

#include <QHostAddress>
#include <QUdpSocket>

#include "Connection.h"

// Implements a UDP connection to the outside world
class UdpConnection : public Connection
{
public:
    UdpConnection(QString n, QHostAddress a, int p);
    ~UdpConnection();

private slots:
    void handleReadyRead();

private:
    QUdpSocket udpSocket;
};

