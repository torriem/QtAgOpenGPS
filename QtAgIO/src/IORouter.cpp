#include <QCoreApplication>
#include <QDebug>

#include "IORouter.h"
#include "UdpConnection.h"

//IORouter::IORouter()
//{
//    qDebug() << "IORouter Default CTOR";
//}

IORouter::IORouter(QSettings& settings) : 
    QObject(nullptr),
    serverConnection(QString("AOG"), QHostAddress::LocalHost, settings.value("AogUdpPort", 17777).toInt())
{
    qDebug() << "IORouter CTOR";
    loadSettings(settings);
}

void IORouter::handleMessage(QByteArray& msg, Connection& source)
{
    qDebug() << "Got message " << msg << " from " << source.getName();

    // TODO: Handle an incoming message.
    // This is where routing is done
}
 
void IORouter::loadSettings(QSettings& settings)
{
    qDebug() << "Loading settings from " << settings.fileName();

    // Close and remove exisitng connections
    for (auto* connection: ioConnections)
    {
        delete connection;
    }
    ioConnections.clear();

    // Iterate over connections given in settings
    int numConnections = settings.value("NumConnections", 0).toInt();

    if (numConnections < 1)
    {
        qFatal() << "No connections provided.  Nothing to do. Exiting.";
        QCoreApplication::exit(1);
    }

//    for (int i = 0; i < numConnections; i++)
//    {
//    }

    // TODO:Handle changed settings.  Probably something like:
    // 1. Close all existing connections
    // 2. Deallocate all existing connections
    // 3. Allocate new connections from settings info
    // 4. Open all connections?
}

void IORouter::openAllConnections()
{
    qDebug() << "Opening all connections";
    emit connectionsOpened();
}

void IORouter::closeAllConnections()
{
    qDebug() << "Closing all connections";
    emit connectionsClosed();
}

