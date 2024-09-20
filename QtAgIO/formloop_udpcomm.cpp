#include <QCoreApplication>
#include "formloop.h"
#include <QHostInfo>
#include <QNetworkInterface>
#include "qmlutil.h"
#include "interfaceproperty.h"
#include "agioproperty.h"

void FormLoop::LoadUDPNetwork()
{
    helloFromAgIO[5] = 56;

    //get the hostname
    //QString hostName = QHostInfo::localHostName();

    // Get all IP addresses associated with the host name
    //QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
    //Is the above code all right? I don't know what I"m doing. David
    /* Oh forget this for now. I'll work on it later
		 * foreach (const QHostAddress &address, addresses) {
		 {
		 qDebug() << "Found IP Address:" << address.toString();
		 if (IPA.AddressFamily == AddressFamily.InterNetwork)
		 {
		 char  data = IPA.ToString();
		 }
		 }*/

    //set the ip
    int ip1 = property_setUDP_IP1;
    int ip2 = property_setUDP_IP2;
    int ip3 = property_setUDP_IP3;
    int ip4 = 255; //broadcast

    quint32 ipAddress = (ip1 << 24) | (ip2 << 16) | (ip3 << 8) | ip4;

    ethUDP.portToSend = property_setUDP_sendPort;

    ethUDP.address.setAddress(ipAddress);

    //set the port where we listen
    ethUDP.portToListen = property_setUDP_listenPort;


    // Initialise the socket
    udpSocket = new QUdpSocket(this);

    agio = qmlItem(qml_root, "agio"); //UI connection

    //set up the connection
    //this is the part that listens
    if(!udpSocket->bind(QHostAddress::Any, ethUDP.portToListen)) //TODO settings
    {
        qDebug() << "Failed to bind udpSocket: " << udpSocket->errorString();
        agio->setProperty("ethernetConnected", false);
        isUDPNetworkConnected = true;
    }else {
        udpSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 1);
        qDebug() << "udpSocket bound to " << ethUDP.address << ":" << ethUDP.portToListen;
        agio->setProperty("ethernetConnected", true);
    }

    //trigger ReceiveFromUDP() when a packet is present
    connect(udpSocket, &QUdpSocket::readyRead, this, &FormLoop::ReceiveFromUDP);


}

void FormLoop::LoadLoopback() //set up the connection that listens to loopback
{

    loopListenPort = property_setLoop_listenPort;
    loopSendPort = property_setLoop_sendPort;
    agio = qmlItem(qml_root, "agio");
    loopBackSocket = new QUdpSocket(this);
    if(!loopBackSocket->bind(QHostAddress::LocalHost, loopListenPort))
    {
        qDebug() <<"Failed to bind loopBackSocket: : " << loopBackSocket->errorString();
        qDebug() << "Exiting program due to fatal error";
        agio->setProperty("aogConnected", false);

        QCoreApplication::exit(0);
    } else {
        qDebug() << "loopBackSocket bound";
        agio->setProperty("aogConnected", true);
    }

    connect(loopBackSocket, &QUdpSocket::readyRead, this, &FormLoop::ReceiveFromLoopBack);

}

void FormLoop::SendDataToLoopBack(QByteArray byteData)
{
    loopBackSocket->writeDatagram(byteData, QHostAddress::LocalHost, loopSendPort);
    /*try
	  {
	  if (byteData.Length != 0)
	  {
	// Send packet to AgVR
	loopBackSocket.BeginSendTo(byteData, 0, byteData.Length, SocketFlags.None, endPoint,
	new AsyncCallback(SendDataLoopAsync), null);
	}
	}
	catch (Exception ex)
	{
	qDebug() << "Failed to send to AgVr.";
	}
	 *Right now we don't send anything to agvr
     */
}

void FormLoop::ReceiveFromLoopBack()
{

    while (loopBackSocket->hasPendingDatagrams()){
        QByteArray byteData;
        byteData.resize(loopBackSocket->pendingDatagramSize());
        loopBackSocket->readDatagram(byteData.data(), byteData.size());
        SendUDPMessage(byteData, ethUDP.address, ethUDP.portToSend);

        /*
	 * This is a part of the whole serial nightmare...
     * Do we actually want to deal with this?? UDP is soo easy.
	 * Just forward everything on.
	 * And much more reliable.
	 * David 6/6/24
	 *
	 * Upon calmer reflection... Maybe best is serial for GPS only. So the lightbar guys
	 * can test and don't need a switch for ligtbar only.
	 * However, UDP takes priority for right now.


	 if (data[0] == 0x80 && data[1] == 0x81)
	 {
	 switch (data[3])
	 {
	 case 0xFE: //254 AutoSteer Data
	 {
	//serList.AddRange(data);
	SendSteerModulePort(data, data.Length);
	SendMachineModulePort(data, data.Length);
	break;
	}
	case 0xEF: //239 machine pgn
	{
	SendMachineModulePort(data, data.Length);
	SendSteerModulePort(data, data.Length);
	break;
	}
	case 0xE5: //229 Symmetric Sections - Zones
	{
	SendMachineModulePort(data, data.Length);
	//SendSteerModulePort(data, data.Length);
	break;
	}
	case 0xFC: //252 steer settings
	{
	SendSteerModulePort(data, data.Length);
	break;
	}
	case 0xFB: //251 steer config
	{
	SendSteerModulePort(data, data.Length);
	break;                        }

	case 0xEE: //238 machine config
	{
	SendMachineModulePort(data, data.Length);
	SendSteerModulePort(data, data.Length);
	break;                        }

	case 0xEC: //236 machine config
	{
	SendMachineModulePort(data, data.Length);
	SendSteerModulePort(data, data.Length);
	break;
	}
	}
    }

*/
    }
}

void FormLoop::SendUDPMessage(QByteArray byteData, QHostAddress address, uint portNumber)
{
    if(!udpListenOnly) //listen only
    {

        // Send packet to the zero
        if (byteData.size() != 0){
            qint64 bytesSent = udpSocket->writeDatagram(byteData, address, portNumber);
            if(bytesSent == -1)
                qDebug() << "SendUDP failed to send UDP message!";
        }else
            qDebug() << "SendUDP was requested to send empty data message. Did not send";


    }
}
void FormLoop::ReceiveFromUDP()
{

    if (!haveWeRecUDP) {
        haveWeRecUDP = true;
        qDebug() << "Recieved UDP!";
    }
    //read the data
    while (udpSocket->hasPendingDatagrams()){
        QByteArray data;
        data.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(data.data(), data.size());

        buffer = data;

        //verify the datagram isn't an NMEA string, and is from the module
        if (data.length() > 4 && data[0] == (char)0x80 && data[1] == (char)0x81)
        {
            //module return via udp sent to AOG
            SendDataToLoopBack(data);
            //check for Scan and Hello
            if (data[3] == 126 && data.size() == 11)
                traffic.helloFromAutoSteer = 0;

            else if (data[3] == 123 && data.size() == 11)
                traffic.helloFromMachine = 0;

            else if (data[3] == 121 && data.size() == 11)
                traffic.helloFromIMU = 0;

            //scan Reply
            else if (data[3] == 203 && data.size() == 13) //
            {
                if (data[2] == 126)  //steer module
                {
                    //scanReply.steerIP = data[5].ToString() + "." + data[6].ToString() + "." + data[7].ToString() + "." + data[8].ToString();
                    //this should do the same as the above line did in cs
                    scanReply.steerIP = QString::fromLatin1(data.mid(5, 4)).replace(" ", ".");

                    scanReply.subnet[0] = data[9];
                    scanReply.subnet[1] = data[10];
                    scanReply.subnet[2] = data[11];

                    //scanReply.subnetStr = data[9].ToString() + "." + data[10].ToString() + "." + data[11].ToString();
                    scanReply.subnetStr = QString::fromLatin1(data.mid(9, 3)).replace(" ", ".");

                    scanReply.isNewData = true;
                    scanReply.isNewSteer = true;
                }
                //
                else if (data[2] == 123)   //machine module
                {
                    //scanReply.machineIP = data[5].ToString() + "." + data[6].ToString() + "." + data[7].ToString() + "." + data[8].ToString();

                    scanReply.machineIP = QString::fromLatin1(data.mid(5, 4)).replace(" ", ".");

                    scanReply.subnet[0] = data[9];
                    scanReply.subnet[1] = data[10];
                    scanReply.subnet[2] = data[11];

                    //scanReply.subnetStr = data[9].ToString() + "." + data[10].ToString() + "." + data[11].ToString();

                    scanReply.subnetStr = QString::fromLatin1(data.mid(9, 3)).replace(" ", ".");

                    scanReply.isNewData = true;
                    scanReply.isNewMachine = true;

                }
                else if (data[2] == 121)   //IMU Module
                {
                    //scanReply.IMU_IP = data[5].ToString() + "." + data[6].ToString() + "." + data[7].ToString() + "." + data[8].ToString();

                    scanReply.IMU_IP = QString::fromLatin1(data.mid(5, 4)).replace(" ", ".");

                    scanReply.subnet[0] = data[9];
                    scanReply.subnet[1] = data[10];
                    scanReply.subnet[2] = data[11];

                    //scanReply.subnetStr = data[9].ToString() + "." + data[10].ToString() + "." + data[11].ToString();
                    scanReply.subnetStr = QString::fromLatin1(data.mid(9, 3)).replace(" ", ".");

                    scanReply.isNewData = true;
                    scanReply.isNewIMU = true;
                }

                else if (data[2] == 120)    //GPS module
                {
                    //scanReply.GPS_IP = data[5].ToString() + "." + data[6].ToString() + "." + data[7].ToString() + "." + data[8].ToString();

                    scanReply.GPS_IP = QString::fromLatin1(data.mid(5, 4)).replace(" ", ".");

                    scanReply.subnet[0] = data[9];
                    scanReply.subnet[1] = data[10];
                    scanReply.subnet[2] = data[11];

                    //scanReply.subnetStr = data[9].ToString() + "." + data[10].ToString() + "." + data[11].ToString();

                    scanReply.subnetStr = QString::fromLatin1(data.mid(9, 3)).replace(" ", ".");

                    scanReply.isNewData = true;
                    scanReply.isNewGPS = true;
                }
            }
        } // end of pgns

        else if (data[0] == 36 && (data[1] == 71 || data[1] == 80 || data[1] == 75))
        {
            traffic.cntrGPSOut += data.length();
            rawBuffer += QString::fromLatin1(data); //is this right? David
              ParseNMEA(rawBuffer);
            if(!haveWeSentToParser) {
                  qDebug() << "sent to parser";
                    haveWeSentToParser = true;
            }
        }
    }
}
