#include <QCoreApplication>
#include "formloop.h"
#include <QHostInfo>
#include <QNetworkInterface>

void FormLoop::LoadUDPNetwork()
{
    qDebug() << "Loading UDP Network";
    //helloFromAgIO[5] = 56;


    //get the hostname
    QString hostName = QHostInfo::localHostName();

    // Get all IP addresses associated with the host name
    QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
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
    uint ip1 = settings.value("UDPComm/IP1").toUInt();
    uint ip2 = settings.value("UDPComm/IP2").toUInt();
    uint ip3 = settings.value("UDPComm/IP3").toUInt();
    uint ip4 = 255; //broadcast

    quint32 ipAddress = (ip1 << 24) | (ip2 << 16) | (ip3 << 8) | ip4;

    ethUDP.address.setAddress(ipAddress);
    qDebug() << "The UDPComm ip address is:" << ethUDP.address.toString();

    //set the port where we listen
    ethUDP.portToListen = settings.value("UDPComm/UdpListenPort").toInt();

    // Initialise the socket
    udpSocket = new QUdpSocket(this);


    //set up the connection
    //this is the part that listens
    if(!udpSocket->bind(ethUDP.address, settings.value("UDPComm/UdpListenPort").toInt())) //TODO settings
    {
        qDebug() << "Failed to bind udpSocket: " << udpSocket->errorString();
        //qDebug() << "Exiting program due to fatal error";
        //QCoreApplication::exit(0);
    }else {
        qDebug() << "udpSocket bound";
    }

    //trigger ReceiveFromUDP() when a packet is present
    connect(udpSocket, &QUdpSocket::readyRead, this, &FormLoop::ReceiveFromUDP);

    isUDPNetworkConnected = true;

}

void FormLoop::LoadLoopback() //set up the connection that listens to loopback
{
    loopBackSocket = new QUdpSocket(this);
    if(!loopBackSocket->bind(QHostAddress::LocalHost, settings.value("LoopbackComm/ListenToAOGPort").toInt()))
    {
        qDebug() <<"Failed to bind loopBackSocket: : " << loopBackSocket->errorString();
        qDebug() << "Exiting program due to fatal error";
        QCoreApplication::exit(0);
    } else {
        qDebug() << "loopBackSocket bound";
    }

    connect(loopBackSocket, &QUdpSocket::readyRead, this, &FormLoop::ReceiveFromLoopBack);

}

void FormLoop::SendDataToLoopBack(QByteArray byteData)
{
    loopBackSocket->writeDatagram(byteData, QHostAddress::LocalHost, settings.value("LoopbackComm/SendToAOGPort").toInt());
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

    //if (isUDPNetworkConnected)
    if(!settings.value("UDPComm/ListenOnly").toBool())
    {

        // Send packet to the zero
        if (byteData.size() != 0)

            udpSocket->writeDatagram(byteData, address, portNumber);

    }else{
        qDebug() << "Listen to modules only";
    }
}
void FormLoop::ReceiveFromUDP()
{

    //read the data
    while (udpSocket->hasPendingDatagrams()){
        QByteArray data;
        data.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(data.data(), data.size());

        buffer = data;
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
            //traffic.cntrGPSOut += data.Length; // don't worry about gpsOut right now
            rawBuffer += QString::fromLatin1(data); //is this right? David
              ParseNMEA(rawBuffer);
            if(!haveWeSentToParser) {
                  qDebug() << "sent to parser";
                    haveWeSentToParser = true;
            }
        }
    }
}
