#pragma once
#include <QUdpSocket>
#include <QByteArray>
#include <QSettings>

#include "Connection.h"
#include "UdpConnection.h"

// Main message handler and router.
// Responsible for creating and destroying connections,
// and routing messages between them.
class IORouter : public QObject
{
    Q_OBJECT

public:
    IORouter() = delete;
    IORouter(QSettings& settings);
    virtual ~IORouter() = default;

public slots:
    // Handle an incoming message from one of the connections
    void handleMessage(QByteArray& msg, Connection& source);

    // User has changed settings.  Reload them.
    void loadSettings(QSettings& settings);

    // Open connections
    void openAllConnections();

    // Close connections();
    void closeAllConnections();

signals:
    // Notify when all connections have been opened
    void connectionsOpened();

    // Notify when all connections have been closed
    void connectionsClosed();

private:

    // The server's connection to AOG
    UdpConnection serverConnection;

    // All other IO connections are dynamically allocated
    QVector<Connection*> ioConnections;
};
