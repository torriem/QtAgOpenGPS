#include "UdpComm.h"
#include <QHostInfo>
#include <QNetworkInterface>


UDP::UDP()
{

}

UDP::CTraffic()
	: cntrGPSIn(0),
	  cntrGPSInBytes(0),
	  cntrGPSOut(0),
	  helloFromMachine(99),
	  helloFromAutoSteer(99),
	  helloFromIMU(99)
{
	//these are simply storage classes, right?
}
//chatgpt
CScanReply::CScanReply()
    : steerIP(""),
      machineIP(""),
      GPS_IP(""),
      IMU_IP(""),
      subnetStr(""),
      subnet(3, 0), // Initialize with three bytes set to 0
      isNewSteer(false),
      isNewMachine(false),
      isNewGPS(false),
      isNewIMU(false),
      isNewData(false)
{
    // Optional: other initialization code
}

/*UDP::CScanReply(){

}*/

UDP::FormLoop(){

	void UDP::LoadUDPNetwork()
	{

		helloFromAgIO[5] = 56;

		try //udp network
		{

			//get the hostname	
			QString hostName = QHostInfo::localHostName();

			// Get all IP addresses associated with the host name
			QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
			//Is the above code all right? I don't know what I"m doing. David
			foreach (const QHostAddress &address, addresses) {
				{
					qDebug() << "Found IP Address:" << address.toString();
					if (IPA.AddressFamily == AddressFamily.InterNetwork)
					{
						char  data = IPA.ToString();
					}
				}

			// Initialise the socket
			udpSocket = new QUdpSocket(this);
			udpSocket->bind(QHostAddress::Any, 9999));

			/*udpSocket.BeginReceiveFrom(buffer, 0, buffer.Length, SocketFlags.None, ref endPointUDP,
					new AsyncCallback(ReceiveDataUDPAsync), null);
					*AOG stuff
					*/
			connect(udpSocket, SIGNAL(readyRead()), this, SLOT(ReceiveFromUDP()));

			//I need somethign like this, right?
			//connect(->udpSocket,SIGNAL(readyRead()),this,SLOT(ReceiveFromAgIO()));
			//that is 308 in udpcomm.cpp

			isUDPNetworkConnected = true;
			qDebug() << "UDP Network connected";

		}
		catch (Exception e)
		{
			//WriteErrorLog("UDP Server" + e);
			qDebug() << "Serious Network Connection Error";
		}
	}

	void UDP::LoadLoopback() //this should be done. David 6/18/24
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
		loopBackSocket->bind(QHostAddress::LocalHost, 17777);
		
		connect(loopBackSocket, SIGNAL(readyRead()), this, SLOT(ReceiveFromLoopBack()));

	}

	/*void UDP::SendToLoopBackMessageAOG(QByteArray byteData)
	{
		SendDataToLoopBack(byteData, epAgOpen);
	}
	*I don't think this is necessary
	*/

	void UDP::SendDataToLoopBack(QByteArray byteData)//this also should work David 6/18/24
	{
        loopBackSocket->writeDatagram(byteData, 17777);

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

	void UDP::ReceiveFromLoopBack(QByteArray data)
	{

		//SendUDPMessage(data);
		connect(loopBackSocket, SIGNAL(readyRead()), this, SLOT(SendUDPMessage()));
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

	void UDP::SendUDPMessage( QByteArray byteData)
	{

		if (isUDPNetworkConnected)
		{

			try
			{
				// Send packet to the zero
				if (byteData.size() != 0)
				{
					udpSocket->writeDatagram(byteData, 8888);
				}
			}
			catch (Exception)
			{
			}
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
	void UDP::ReceiveFromUDP(QByteArray data) //this should work. David 6/18/24
	{

		try
		{
			if (data[0] == 0x80 && data[1] == 0x81)
			{
				//module return via udp sent to AOG
                connect(udpSocket, SIGNAL(readyRead()), this, SLOT(SendDataToLoopback()));
				//check for Scan and Hello
				if (data[3] == 126 && data.Length == 11)
				{

					traffic.helloFromAutoSteer = 0;
				}

				else if (data[3] == 123 && data.Length == 11)
				{

					traffic.helloFromMachine = 0;

				}

				else if (data[3] == 121 && data.Length == 11)
					traffic.helloFromIMU = 0;

				//scan Reply
				else if (data[3] == 203 && data.Length == 13) //
				{
					if (data[2] == 126)  //steer module
					{
						scanReply.steerIP = data[5].ToString() + "." + data[6].ToString() + "." + data[7].ToString() + "." + data[8].ToString();

						scanReply.subnet[0] = data[9];
						scanReply.subnet[1] = data[10];
						scanReply.subnet[2] = data[11];

						scanReply.subnetStr = data[9].ToString() + "." + data[10].ToString() + "." + data[11].ToString();

						scanReply.isNewData = true;
						scanReply.isNewSteer = true;
					}
					//
					else if (data[2] == 123)   //machine module
					{
						scanReply.machineIP = data[5].ToString() + "." + data[6].ToString() + "." + data[7].ToString() + "." + data[8].ToString();

						scanReply.subnet[0] = data[9];
						scanReply.subnet[1] = data[10];
						scanReply.subnet[2] = data[11];

						scanReply.subnetStr = data[9].ToString() + "." + data[10].ToString() + "." + data[11].ToString();

						scanReply.isNewData = true;
						scanReply.isNewMachine = true;

					}
					else if (data[2] == 121)   //IMU Module
					{
						scanReply.IMU_IP = data[5].ToString() + "." + data[6].ToString() + "." + data[7].ToString() + "." + data[8].ToString();

						scanReply.subnet[0] = data[9];
						scanReply.subnet[1] = data[10];
						scanReply.subnet[2] = data[11];

						scanReply.subnetStr = data[9].ToString() + "." + data[10].ToString() + "." + data[11].ToString();

						scanReply.isNewData = true;
						scanReply.isNewIMU = true;
					}

					else if (data[2] == 120)    //GPS module
					{
						scanReply.GPS_IP = data[5].ToString() + "." + data[6].ToString() + "." + data[7].ToString() + "." + data[8].ToString();

						scanReply.subnet[0] = data[9];
						scanReply.subnet[1] = data[10];
						scanReply.subnet[2] = data[11];

						scanReply.subnetStr = data[9].ToString() + "." + data[10].ToString() + "." + data[11].ToString();

						scanReply.isNewData = true;
						scanReply.isNewGPS = true;
					}
				}


			} // end of pgns

			else if (data[0] == 36 && (data[1] == 71 || data[1] == 80 || data[1] == 75))
			{
				/*traffic.cntrGPSOut += data.Length;
				rawBuffer += Encoding.ASCII.GetString(data);
				ParseNMEA(ref rawBuffer);
				*forget about gpsOut for the moment*
				*/

			}
		}
		catch
		{

		}
	}
}

