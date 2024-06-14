#include <QObject>
#include <QUdpSocket>

class UdpListener : public QObject {
    Q_OBJECT

public:
    explicit UdpListener(QObject *parent = nullptr);
    ~UdpListener();


signals:
    void packetReceived(const QByteArray &data, const QHostAddress &sender, quint16 senderPort);

private slots:
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    QUdpSocket *socket;
};


