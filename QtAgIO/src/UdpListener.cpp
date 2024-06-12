#include "UdpListener.h"

UdpListener::UdpListener(QObject *parent) : QObject(parent) {
    socket = new QUdpSocket(this);

    connect(socket, &QUdpSocket::readyRead, this, &UdpListener::onReadyRead);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &UdpListener::onErrorOccurred);

    // Bind to the specified address and port
    QHostAddress address("10.0.0.255");  // Change to specific address if needed
    quint16 port = 9999;

    if (!socket->bind(address, port)) {
        qDebug() << "Failed to bind socket:" << socket->errorString();
    } else {
        qDebug() << "Listening on" << address.toString() << ":" << port;
    }
}

UdpListener::~UdpListener() {
    delete socket;
}

void UdpListener::onReadyRead() {
    while (socket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

        emit packetReceived(buffer, sender, senderPort);
    }
}

void UdpListener::onErrorOccurred(QAbstractSocket::SocketError socketError) {
    qDebug() << "Socket error:" << socketError << socket->errorString();
}

