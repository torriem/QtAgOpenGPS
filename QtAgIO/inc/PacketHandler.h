#include <QObject>
#include <QDebug>
#include <QUdpSocket>

class PacketHandler : public QObject {
    Q_OBJECT

public:
    explicit PacketHandler(QObject *parent = nullptr);

public slots:
    void processPacket(const QByteArray &data, const QHostAddress &sender, quint16 senderPort);
};


