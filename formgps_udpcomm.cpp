#include <QNetworkDatagram>
#include "formgps.h"
#include "aogsettings.h"
#include "cnmea.h"

#define UDP_NMEA_PORT 9999

void FormGPS::udpServerReadDatagrams()
{
    QNetworkDatagram datagram;

    /* this runs in the main GUI thread, it won't ever run at the
     * exact same time as the timer callback, so we can safely just
     * append to the rawBuffer, even if it's about to be processed
     * by the parser.
     */
    while (udpSocket->hasPendingDatagrams()) {
        datagram = udpSocket->receiveDatagram();

        pn->rawBuffer += datagram.data();
        //Will a ton of incoming data make this whole thing block
        //forever? maybe.
    }
    //qDebug() << pn->rawBuffer ;
}

void FormGPS::startUDPServer()
{
    AOGSettings s;
    int port = s.value("port/udp_port_num",9999).toInt();

    if(udpSocket) stopUDPServer();

    udpSocket = new QUdpSocket(this); //should auto delete with the form
    udpSocket->bind(port); //by default, bind to all interfaces.

    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(udpServerReadDatagrams()));
}

void FormGPS::stopUDPServer()
{
    if(udpSocket) {
        udpSocket->close();
        delete udpSocket;
        udpSocket = NULL;
    }
}

//TODO: connection to MKR1000 on fixed address.. probably in another module
