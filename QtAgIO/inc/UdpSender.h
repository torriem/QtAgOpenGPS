#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

class UDPSender : public QObject
{
    Q_OBJECT

public:
    explicit UDPSender(const QString& ipAddress, quint16 port, QObject* parent = nullptr);
    ~UDPSender();

public slots:
	void sendToAOG(const QByteArray& data);

private:
    QUdpSocket* udpSocket;
    QHostAddress serverAddress;
    quint16 serverPort;
};

#endif // UDPSENDER_H

