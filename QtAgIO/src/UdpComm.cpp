#include "UDP.h"
#include <QHostInfo>
#include <QNetworkInterface>


UDP::UDP()
{

}

UDP::CTraffic()
{
	//these are simply storage classes, right?
}

UDP::CScanReply(){

}

UDP::FormLoop(){

	void UDP::LoadUDPNetwork
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
			udpSocket->bind(new IPEndPoint(IPAddress.Any, 9999));
			udpSocket.BeginReceiveFrom(buffer, 0, buffer.Length, SocketFlags.None, ref endPointUDP,
					new AsyncCallback(ReceiveDataUDPAsync), null);

			//I need somethign like this, right?
			//connect(udpSocket,SIGNAL(readyRead()),this,SLOT(ReceiveFromAgIO()));
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

	void UDP::LoadLoopback()
	{

		try //loopback
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
		}
	}

	void UDP::SendToLoopBackMessageAOG(QByteArray byteData)
	{
		SendDataToLoopBack(byteData, epAgOpen);
	}

	void UDP::SendDataToLoopBack(QByteArray byteData, IPEndPoint endPoint)
	{

		try
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
	}

	void UDP::ReceiveFromLoopBack(QByteArray data)
	{

		SendUDPMessage(data, epModule);
		/*
		 * This is a part of the whole serial nightmare...
		 * Do we actually want to deal with this?? UDP is soo easy. 
		 * Just forward everything on.
		 * And much more reliable.
		 * David 6/6/24

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
	void ReceiveDataLoopAsync(IAsyncResult asyncResult)
	{
		try
		{
			// Receive all data
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
	}

	void UDP::SendUDPMessage( QByteArray byteData, IPEndPoint endPoint)
	{

		if (isUDPNetworkConnected)
		{

			try
			{
				// Send packet to the zero
				if (byteData.Length != 0)
				{
					UDPSocket.BeginSendTo(byteData, 0, byteData.Length, SocketFlags.None,
							endPoint, new AsyncCallback(SendDataUDPAsync), null);
				}
			}
			catch (Exception)
			{
			}
		}
	}
	void UDP::SendDataUDPAsync(IAsyncResult asyncResult)
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
	}
	void UDP::ReceiveFromUDP(QByteArray data)
	{

		try
		{
			if (data[0] == 0x80 && data[1] == 0x81)
			{
				//module return via udp sent to AOG
				SendToLoopBackMessageAOG(data);

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

						scanReply.subnet[0] = data[09];
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

						scanReply.subnet[0] = data[09];
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

						scanReply.subnet[0] = data[09];
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
				traffic.cntrGPSOut += data.Length;
				rawBuffer += Encoding.ASCII.GetString(data);
				ParseNMEA(ref rawBuffer);

			}
		}
		catch
		{

		}
	}
}

