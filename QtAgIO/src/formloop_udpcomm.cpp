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

    // Initialise the socket
    udpSocket = new QUdpSocket(this);

    //set up the connection
    //this is the part that listens
    if(!udpSocket->bind(QHostAddress("10.0.0.255"), 9999))
    {
        qDebug() << "Failed to bind udpSocket" << udpSocket->errorString();
        qDebug() << "Exiting program due to fatal error";
        std::exit;
    }else {
        qDebug() << "udpSocket bound";
    }

    //trigger ReceiveFromUDP() when a packet is present
    connect(udpSocket, &QUdpSocket::readyRead, this, &FormLoop::ReceiveFromUDP);

    isUDPNetworkConnected = true;

}

void FormLoop::LoadLoopback() //this should be done. David 6/18/24
{

    /*try //loopback
	  {
	  loopBackSocket = new QUdpSocket(this);
	  loopBackSocket->bind(new IPEndPoint(IPAddress.Loopback, 17777));
      loopBackSocket.BeginReceiveFrom(buffer, 0, buffer.Length, SocketFlags.None, ref endPointLoopBack,
	  new AsyncCallback(ReceiveDataLoopAsync), null);

	//connect(udpSocket,SIGNAL(readyRead()),this,SLOT(ReceiveFromAgIO()));
	}
	catch (Exception ex)
	{
	qDebug() << "Serious Loopback Connection Error";
	}*/
    //Loopback David 6/18/24
    loopBackSocket = new QUdpSocket(this);
    if(!loopBackSocket->bind(QHostAddress::LocalHost, 17770))
    {
        qDebug() <<"Failed to bind loopBackSocket" << loopBackSocket->errorString();
        qDebug() << "Exiting program due to fatal error";
        std::exit;
    } else {
        qDebug() << "loopBackSocket bound";
    }

    connect(loopBackSocket, &QUdpSocket::readyRead, this, &FormLoop::ReceiveFromLoopBack);

}

void FormLoop::SendDataToLoopBack(QByteArray byteData)//this also should work David 6/18/24
{
    loopBackSocket->writeDatagram(byteData, QHostAddress::LocalHost, 15550);
    //qDebug() << "Sent size: " << byteData.size();

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
        //SendUDPMessage(QByteArray byteData);

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
/*void ReceiveDataLoopAsync(IAsyncResult asyncResult)
  {
  try
  {
/* Receive all data

 * How much of this do we need? How much is for cs stuff, and how much is required for networking?
 * David 6/18/24
 *
 int msgLen = loopBackSocket.EndReceiveFrom(asyncResult, ref endPointLoopBack);

 byte[] localMsg = new byte[msgLen];
 Array.Copy(buffer, localMsg, msgLen);

// Listen for more connections again...
loopBackSocket.BeginReceiveFrom(buffer, 0, buffer.Length, SocketFlags.None, ref endPointLoopBack,
new AsyncCallback(ReceiveDataLoopAsync), null);

BeginInvoke((MethodInvoker)(() => ReceiveFromLoopBack(localMsg)));
}
catch (Exception)
{
}
}*/

void FormLoop::SendUDPMessage(QByteArray byteData)
{

    if (isUDPNetworkConnected)
    {

        // Send packet to the zero
        if (byteData.size() != 0)
            udpSocket->writeDatagram(byteData, QHostAddress("10.0.0.255"), 8888);
    }
}
/*void UDP::SendDataUDPAsync(IAsyncResult asyncResult) //not necessary with qt
  {
  try
  {
  UDPSocket.EndSend(asyncResult);
  }
  catch (Exception)
  {
  }
  }

  void UDP::ReceiveDataUDPAsync(IAsyncResult asyncResult)
  {

  try
  {
// Receive all data
int msgLen = UDPSocket.EndReceiveFrom(asyncResult, ref endPointUDP);

byte[] localMsg = new byte[msgLen];
Array.Copy(buffer, localMsg, msgLen);

// Listen for more connections again...
udpSocket.BeginReceiveFrom(buffer, 0, buffer.Length, SocketFlags.None, ref endPointUDP,
new AsyncCallback(ReceiveDataUDPAsync), null);

BeginInvoke((MethodInvoker)(() => ReceiveFromUDP(localMsg)));

}
catch (Exception)
{
}
}*/
void FormLoop::ReceiveFromUDP() //this should work. David 6/18/24
{

    //read the data
    while (udpSocket->hasPendingDatagrams()){
        QByteArray data;
        data.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(data.data(), data.size());
        //qDebug() << data[0] << data[1] << data[2], data[3];

            //SendDataToLoopBack(data);
        //if (data[0] == 0x80 && data[1] == 0x81)
            if (data.length() > 4 && data[0] == (char)0x80 && data[1] == (char)0x81)
        {
            //module return via udp sent to AOG
        //qDebug() << "RECD size: " << data.size();
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
            //traffic.cntrGPSOut += data.Length;
              //rawBuffer += Encoding.ASCII.GetString(data);
            rawBuffer += QString::fromLatin1(data); //is this right? David
              ParseNMEA(rawBuffer);
            if(!haveWeSentToParser) {
                  qDebug() << "sent to parser";
                    haveWeSentToParser = true;
            }
             /*forget about gpsOut for the moment*
			 */

        }
    }
}
