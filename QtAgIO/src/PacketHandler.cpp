#include "PacketHandler.h"

PacketHandler::PacketHandler(QObject *parent) : QObject(parent) {}

void PacketHandler::processPacket(const QByteArray &data, const QHostAddress &sender, quint16 senderPort) {
    qDebug() << "Received data from" << sender.toString() << ":" << senderPort << "data:" << data;
    // Add your packet processing logic here
}

