#include "formloop.h"


void FormLoop::DoNTRIPSecondRoutine()
{
	//count up the ntrip clock only if everything is alive
	if (isNTRIP_RequiredOn || isRadio_RequiredOn || isSerialPass_RequiredOn)
	{
		IncrementNTRIPWatchDog();
	}

	//Have we NTRIP connection
	if (isNTRIP_RequiredOn && !isNTRIP_Connected && !isNTRIP_Connecting)
	{
		if (!isNTRIP_Starting && ntripCounter > 20)
		{
			StartNTRIP();
		}
	}

	//check if we need to send...?
	if ((isRadio_RequiredOn || isSerialPass_RequiredOn) && !isNTRIP_Connected && !isNTRIP_Connecting)
	{
		if (!isNTRIP_Starting)
		{
			StartNTRIP();
		}
	}

	if (isNTRIP_Connecting)
	{
		if (ntripCounter > 29)
		{
			//TimedMessageBox(1500, "Connection Problem", "Not Connecting To Caster");
			qDebug() << "Connection Problem. Not Connecting To Caster";
			ReconnectRequest();
		}
		if (clientSocket != NULL && clientSocket.Connected)
		{
			SendAuthorization();
		}
	}

	if (isNTRIP_RequiredOn || isRadio_RequiredOn)
	{
		//pbarNtripMenu.Value = unchecked((byte)(tripBytes * 0.02)); //commented in cs
		//lblNTRIPBytes.Text = ((tripBytes >> 10)).toString("###,###,### kb");

		//Bypass if sleeping
		if (focusSkipCounter != 0)
		{
			//update byte counter and up counter
			/*if (ntripCounter > 59) btnStartStopNtrip.Text = (ntripCounter >> 6) + " Min";
			else if (ntripCounter < 60 && ntripCounter > 25) btnStartStopNtrip.Text = ntripCounter + " Secs";
			else btnStartStopNtrip.Text = "In " + (Math.Abs(ntripCounter - 25)) + " secs";*/

			//watchdog for Ntrip
			if (isNTRIP_Connecting)
			{
				//lblWatch.Text = "Authourizing";
				qDebug() << "Authourizing";
			}
			else
			{
				if (isNTRIP_RequiredOn && NTRIP_Watchdog > 10)
				{
					//lblWatch.Text = "Waiting";
					qDebug() << "Waiting";
				}
				else
				{
					//lblWatch.Text = "Listening";

					if (isNTRIP_RequiredOn)
					{
						//lblWatch.Text += " NTRIP";
					}
					else if (isRadio_RequiredOn)
					{
						//lblWatch.Text += " Radio";
					}
				}
			}

			if (sendGGAInterval > 0 && isNTRIP_Sending)
			{
				//lblWatch.Text = "Send GGA";
				isNTRIP_Sending = false;
			}
		}
	}
	else if (isSerialPass_RequiredOn)
	{
		//pbarNtripMenu.Value = unchecked((byte)(tripBytes * 0.02)); //commented in cs
		//lblNTRIPBytes.Text = ((tripBytes >> 10)).toString("###,###,### kb");

		//update byte counter and up counter
		/*if (ntripCounter > 59) btnStartStopNtrip.Text = (ntripCounter >> 6) + " Min";
		else if (ntripCounter < 60 && ntripCounter > 22) btnStartStopNtrip.Text = ntripCounter + " Secs";
		else btnStartStopNtrip.Text = "In " + (Math.Abs(ntripCounter - 22)) + " secs";*/

	}
}

void FormLoop::ConfigureNTRIP()
{
	/*lblWatch.Text = "Wait GPS";
	lblMessages.Text = "Reading...";
	lblNTRIP_IP.Text = "";
	lblMount.Text = "";*/

	aList.clear();
	rList.clear();
	//lblMessages.Text = "Reading....";

	//start NTRIP if required
	isNTRIP_RequiredOn = settings.value("RTK/setNTRIP_isOn").toBool();
	isRadio_RequiredOn = settings.value("RTK/setRadio_isOn").toBool();
	isSerialPass_RequiredOn = settings.value("RTK/setPass_isOn").toBool();

	if (isRadio_RequiredOn || isSerialPass_RequiredOn)
	{
		// Immediatly connect radio
		ntripCounter = 20;
	}

	if (isNTRIP_RequiredOn || isRadio_RequiredOn || isSerialPass_RequiredOn)
	{
		/*btnStartStopNtrip.Visible = true;
		btnStartStopNtrip.Visible = true;
		lblWatch.Visible = true;
		lblNTRIPBytes.Visible = true;
		lblToGPS.Visible = true;
		lblMount.Visible = true;
		lblNTRIP_IP.Visible = true;*/
	}
	else
	{
		/*btnStartStopNtrip.Visible = false;
		btnStartStopNtrip.Visible = false;
		lblWatch.Visible = false;
		lblNTRIPBytes.Visible = false;
		lblToGPS.Visible = false;
		lblMount.Visible = false;
		lblNTRIP_IP.Visible = false;*/
	}

	//btnStartStopNtrip.Text = "Off";
}

void FormLoop::StartNTRIP()
{
	if (isNTRIP_RequiredOn)
	{
		broadCasterPort = settings.value("RTK/setNTRIP_casterPort").toInt(); //Select correct port (usually 80 or 2101)
		mount = settings.value("RTK/setNTRIP_mount").toString(); //Insert the correct mount
		username = settings.value("RTK/setNTRIP_userName").toString(); //Insert your username!
		password = settings.value("RTK/setNTRIP_userPassword").toString(); //Insert your password!
		toUDP_Port = settings.value("RTK/setNTRIP_sendToUDPPort").toInt(); //send rtcm to which udp port
		sendGGAInterval = settings.value("RTK/setNTRIP_sendGGAInterval").toInt(); //how often to send fixes

		//if we had a timer already, kill it
		if (tmr != NULL)
		{
			tmr.Dispose();
		}

		//create new timer at fast rate to start
		if (sendGGAInterval > 0)
		{
			this.tmr = new System.Windows.Forms.Timer();
			this.tmr.Interval = 5000;
			this.tmr.Tick += new EventHandler(NTRIPtick);
		}

		try
		{
			// Close the socket if it is still open
			if (clientSocket != NULL && clientSocket.Connected)
			{
				clientSocket.Shutdown(SocketShutdown.Both);
				System.Threading.Thread.Sleep(100);
				clientSocket.Close();
			}

			//NTRIP endpoint
    uint ip1 = settings.value("UDPComm/IP1").toUInt();
    uint ip2 = settings.value("UDPComm/IP2").toUInt();
    uint ip3 = settings.value("UDPComm/IP3").toUInt();
    uint ip4 = 255; //broadcast

    quint32 ipAddress = (ip1 << 24) | (ip2 << 16) | (ip3 << 8) | ip4;


			/*epNtrip = new IPEndPoint(IPAddress.Parse(
						perties.Settings.Default.etIP_SubnetOne.toString() + "." +
						Properties.Settings.Default.etIP_SubnetTwo.toString() + "." +
						Properties.Settings.Default.etIP_SubnetThree.toString() + ".255"), toUDP_Port);*/

			// Create the socket object
			//clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

			//set socket to non-blocking mode
			clientSocket->setSocketOption(QAbstractSocket::LopDelayOption, 1);

			//clientSocket.BeginConnect(new IPEndPoint(IPAddress.Parse(broadCasterIP), broadCasterPort), new AsyncCallback(OnConnect), null);
// Connect to server
    clientSocket->connectToHost(QHostAddress(ipAddress), broadCasterPort);
		}
		// I don't think this is all good. The catch is supposed 
		// to catch failed connections. RN, I don't think it will
		// catch anything, as I don't think tcpsocket will throw
		// any errors.
		catch (...)
		{
			ReconnectRequest();
			return;
		}

		isNTRIP_Connecting = true;
		//lblNTRIP_IP.Text = broadCasterIP;
		//lblMount.Text = mount;
	}
	/*
	 * If someone wants to attempt this, go ahead. However, I don't think many
	 * if anyone uses it. 
	 * I think it is for a radio connected to AgIO over usb, not a radio plugged
	 * into the receiver. David
	else if (isRadio_RequiredOn)
	{
		if (!string.IsNullOrEmpty(Properties.Settings.Default.setPort_portNameRadio))
		{
			// Disconnect when already connected
			if (spRadio != NULL)
			{
				spRadio.Close();
				spRadio.Dispose();
			}

			// Setup and open serial port
			spRadio = new SerialPort(Properties.Settings.Default.setPort_portNameRadio);
			spRadio.BaudRate = int.Parse(Properties.Settings.Default.setPort_baudRateRadio);
			spRadio.DataReceived += NtripPort_DataReceived;
			isNTRIP_Connecting = false;
			isNTRIP_Connected = true;

			try
			{
				spRadio.Open();
			}
			catch (...)
			{
				isNTRIP_Connecting = false;
				isNTRIP_Connected = false;
				isRadio_RequiredOn = false;

				TimedMessageBox(2000, "Error connecting to radio", $"{ex.Message}");
			}
		}
	}*/

	/*
	 * forget about serial for right now. David
	else if (isSerialPass_RequiredOn)
	{
		toUDP_Port = Properties.Settings.Default.setNTRIP_sendToUDPPort; //send rtcm to which udp port
		epNtrip = new IPEndPoint(IPAddress.Parse(
					Properties.Settings.Default.etIP_SubnetOne.toString() + "." +
					Properties.Settings.Default.etIP_SubnetTwo.toString() + "." +
					Properties.Settings.Default.etIP_SubnetThree.toString() + ".255"), toUDP_Port);

		if (!string.IsNullOrEmpty(Properties.Settings.Default.setPort_portNameRadio))
		{
			// Disconnect when already connected
			if (spRadio != NULL)
			{
				spRadio.Close();
				spRadio.Dispose();
			}

			// Setup and open serial port
			spRadio = new SerialPort(Properties.Settings.Default.setPort_portNameRadio);
			spRadio.BaudRate = int.Parse(Properties.Settings.Default.setPort_baudRateRadio);
			spRadio.DataReceived += NtripPort_DataReceived;
			isNTRIP_Connecting = false;
			isNTRIP_Connected = true;
			//lblWatch.Text = "RTCM Serial";


			try
			{
				spRadio.Open();
			}
			catch (...)
			{
				isNTRIP_Connecting = false;
				isNTRIP_Connected = false;
				isSerialPass_RequiredOn = false;

				TimedMessageBox(2000, "Error connecting to Serial Pass", $"{ex.Message}");
			}
		}
	}*/
}

void FormLoop::ReconnectRequest()
{
	//TimedMessageBox(2000, "NTRIP Not Connected", " Reconnect Request");
	ntripCounter = 15;
	isNTRIP_Connected = false;
	isNTRIP_Starting = false;
	isNTRIP_Connecting = false;

	//if we had a timer already, kill it
	if (tmr != NULL)
	{
		tmr.Dispose();
	}
}

void FormLoop::IncrementNTRIPWatchDog()
{
	//increment once every second
	ntripCounter++;

	//Thinks is connected but not receiving anything
	if (NTRIP_Watchdog++ > 30 && isNTRIP_Connected) 
		ReconnectRequest();

	//Once all connected set the timer GGA to NTRIP Settings
	if (sendGGAInterval > 0 && ntripCounter == 40) tmr.Interval = sendGGAInterval * 1000;
}

void FormLoop::SendAuthorization()
{
	// Check we are connected
	if (clientSocket == NULL || !clientSocket.connected)
	{
		//TimedMessageBox(2000, gStr.gsNTRIPNotConnected, " At the StartNTRIP() ");
		ReconnectRequest();
		return;
	}

	// Read the message from settings and send it
	try
	{
		if (!settings.value("RTK/setNTRIP_isTCP").toBool())
		{
			//encode user and password
			QString auth = ToBase64(username + ":" + password);

			//grab location sentence
			BuildGGA();
			GGASentence = sbGGA.toString();

			QString htt;
			if (settings.value("RTK/setNTRIP_isHTTP10").toString()) htt = "1.0";
			else htt = "1.1";

			//Build authorization string
			QString str = "GET /" + mount + " HTTP/" + htt + "\r\n";
			str += "User-Agent: NTRIP AgOpenGPSClient/20221020\r\n";
			str += "Authorization: Basic " + auth + "\r\n"; //This line can be removed if no authorization is needed
															//str += GGASentence; //this line can be removed if no position feedback is needed
			str += "Accept: */*\r\nConnection: close\r\n";
			str += "\r\n";

			// Convert to byte array and send.
			Byte[] byteDateLine = Encoding.ASCII.GetBytes(str.ToCharArray());
			clientSocket.Send(byteDateLine, byteDateLine.Length, 0);

			//enable to periodically send GGA sentence to server.
			if (sendGGAInterval > 0) tmr.Enabled = true;
		}
		//say its connected
		isNTRIP_Connected = true;
		isNTRIP_Starting = false;
		isNTRIP_Connecting = false;
	}
	catch (...)
	{
		ReconnectRequest();
	}
}

void FormLoop::OnAddMessage(QByte data)
{
	//update gui with stats
	tripBytes += (unsigned int)data.Length;

	if (isViewAdvanced && isNTRIP_RequiredOn )
	{
		int mess = 0;
		//lblPacketSize.Text = data.Length.toString(); //commented in cs

		try
		{
			//lblStationID.Text = (((data[4] & 15) << 8) + (data[5])).toString();

			for (int i = 0; i < data.Length - 5; i++)
			{

				if (data[i] == 211 && (data[i + 1] >> 2) == 0)
				{
					mess = ((data[i + 3] << 4) + (data[i + 4] >> 4));
					if (mess > 1000 && mess < 1231)
					{
						rList.Add(mess);
						i += (data[i + 1] << 6) + (data[i + 2])+5;
						if (data[i + 1] != 211)
						{
							//rList.clear();
							//break;
						}
					}
					else
					{
						rList.clear();
						break;
					}
				}
			}
		}
		catch
		{
			//MessageBox.Show("Error");
		}
	}

	//reset watchdog since we have updated data
	NTRIP_Watchdog = 0;

	if (isNTRIP_RequiredOn)
	{
		//move the ntrip stream to queue
		for (int i = 0; i < data.Length; i++)
		{
			rawTrip.Enqueue(data[i]);
		}

		ntripMeterTimer.Enabled = true;
	}
	else
	{
		//lblToGPS.Text = data.Length.toString();
		//send it
		SendNTRIP(data);
	}


}

void FormLoop::ntripMeterTimer_Tick()
{
	//we really should get here, but have to check
	if (rawTrip.Count == 0) return;

	//how many bytes in the Queue
	int cnt = rawTrip.Count;

	//how many sends have occured
	traffic.cntrGPSIn++;

	//128 bytes chunks max
	if (cnt > packetSizeNTRIP) cnt = packetSizeNTRIP;

	//new data array to send
	byte[] trip = new byte[cnt];

	traffic.cntrGPSInBytes += cnt;

	//dequeue into the array
	for (int i = 0; i < cnt; i++) trip[i] = rawTrip.Dequeue();

	//send it
	SendNTRIP(trip);

	//Are we done?
	if (rawTrip.Count == 0)
	{
		ntripMeterTimer.Enabled = false;

		if (focusSkipCounter != 0)
		{
			//lblToGPS.Text = traffic.cntrGPSInBytes == 0 ? "---" : (traffic.cntrGPSInBytes).toString();
			traffic.cntrGPSInBytes = 0;
		}
	}

	//Can't keep up as internet dumped a shit load so clear
	if (rawTrip.Count > 10000) rawTrip.clear();

	////show how many bytes left in the queue
	if (isViewAdvanced)
		//lblCount.Text = rawTrip.Count.toString();
}

void FormLoop::SendNTRIP(byte[] data)
{
	//serial send out GPS port
	if (isSendToSerial)
	{
		SendGPSPort(data);
	}

	//send out UDP Port
	if (isSendToUDP)
	{
		SendUDPMessage(data, epNtrip);
	}
}

void FormLoop::SendGGA()
{
	//timer may have brought us here so return if not connected
	if (!isNTRIP_Connected)
		return;
	// Check we are connected
	if (clientSocket == NULL || !clientSocket.Connected)
	{
		ReconnectRequest();
		return;
	}

	// Read the message from the text box and send it
	try
	{
		isNTRIP_Sending = true;
		BuildGGA();
		QString str = sbGGA.toString();

		Byte[] byteDateLine = Encoding.ASCII.GetBytes(str.ToCharArray());
		clientSocket.Send(byteDateLine, byteDateLine.Length, 0);
	}
	catch (...)
	{
		ReconnectRequest();
	}
}
void FormLoop::NTRIPtick()
{
	SendGGA();
}

void FormLoop::OnConnect(IAsyncResult ar)
{
	// Check if we were sucessfull
	try
	{
		if (clientSocket.Connected)
			clientSocket.BeginReceive(casterRecBuffer, 0, casterRecBuffer.Length, SocketFlags.None, new AsyncCallback(OnRecievedData), null);
	}
	catch (...)
	{
		//MessageBox.Show(ex.Message, "Unusual error during Connect!");
	}
}

 void FormLoop::FormLoop::OnRecievedData(IAsyncResult ar)
{
	// Check if we got any data
	try
	{
		int nBytesRec = clientSocket.EndReceive(ar);
		if (nBytesRec > 0)
		{
			byte[] localMsg = new byte[nBytesRec];
			Array.Copy(casterRecBuffer, localMsg, nBytesRec);

			BeginInvoke((MethodInvoker)(() => OnAddMessage(localMsg)));
			clientSocket.BeginReceive(casterRecBuffer, 0, casterRecBuffer.Length, SocketFlags.None, new AsyncCallback(OnRecievedData), null);
		}
		else
		{
			// If no data was recieved then the connection is probably dead
			Console.WriteLine("Client {0}, disconnected", clientSocket.RemoteEndPoint);
			clientSocket.Shutdown(SocketShutdown.Both);
			clientSocket.Close();
		}
	}
	catch (...)
	{
		//MessageBox.Show( this, ex.Message, "Unusual error druing Recieve!" );
		qDebug() << "Unusual error druing Recieve!";
	}
}

void FormLoop::NtripPort_DataReceived()
{
	// Check if we got any data
	try
	{
		SerialPort comport = (SerialPort)sender;
		if (comport.BytesToRead < 32) 
			return;

		int nBytesRec = comport.BytesToRead;

		if (nBytesRec > 0)
		{
			byte[] localMsg = new byte[nBytesRec];
			comport.Read(localMsg, 0, nBytesRec);

			BeginInvoke((MethodInvoker)(() => OnAddMessage(localMsg)));
		}
		else
		{
			// If no data was recieved then the connection is probably dead
			// TODO: What can we do?
		}
	}
	catch (...)
	{
		//MessageBox.Show( this, ex.Message, "Unusual error druing Recieve!" );
		qDebug() << "Unusual error druing Recieve!";
	}
}

 QString FormLoop::ToBase64(QString str)
{
	Encoding asciiEncoding = Encoding.ASCII;
	byte[] byteArray = new byte[asciiEncoding.GetByteCount(str)];
	byteArray = asciiEncoding.GetBytes(str);
	return Convert.ToBase64String(byteArray, 0, byteArray.Length);
}

void FormLoop::ShutDownNTRIP()
{
	if (clientSocket != NULL && clientSocket.Connected)
	{
		//shut it down
		clientSocket.Shutdown(SocketShutdown.Both);
		clientSocket.Close();
		System.Threading.Thread.Sleep(500);

		//start it up again
		ReconnectRequest();

		//Also stop the requests now
		isNTRIP_RequiredOn = false;
	}
	/*
	else if(spRadio != NULL)
	{
		spRadio.Close();
		spRadio.Dispose();
		spRadio = null;

		ReconnectRequest();

		//Also stop the requests now
		isRadio_RequiredOn = false;
	}*/
}

void FormLoop::SettingsShutDownNTRIP()
{
	if (clientSocket != NULL && clientSocket.Connected)
	{
		clientSocket.Shutdown(SocketShutdown.Both);
		clientSocket.Close();
		System.Threading.Thread.Sleep(500);
		ReconnectRequest();
	}

	/*
	if (spRadio != NULL && spRadio.IsOpen)
	{
		spRadio.Close();
		spRadio.Dispose();
		spRadio = NULL;
		ReconnectRequest();
	}*/
}

//calculate the NMEA checksum to stuff at the end
QString FormLoop::CalculateChecksum(string Sentence)
{
	int sum = 0, inx;
	QChar sentence_chars = Sentence.ToCharArray();
	QChar tmp;

	// All character xor:ed results in the trailing hex checksum
	// The checksum calc starts after '$' and ends before '*'
	for (inx = 1; ; inx++)
	{
		tmp = sentence_chars[inx];

		// Indicates end of data and start of checksum
		if (tmp == '*')
			break;
		sum ^= tmp;    // Build checksum
	}

	// Calculated checksum converted to a 2 digit hex string
	return String.Format("{0:X2}", sum);
}

private readonly StringBuilder sbGGA = new StringBuilder();

void FormLoop::BuildGGA()
{
	double latitude = 0;
	double longitude = 0;

	if (settings.value("RTK/setNTRIP_isGGAManual").toBool())
	{
		latitude = settings.value("RTK/setNTRIP_manualLat").toDouble();
		longitude = settings.value("RTK/setNTRIP_manualLon").toDouble();
	}
	else
	{
		latitude = this.latitude;
		longitude = this.longitude;
	}

	//convert to DMS from Degrees
	double latMinu = latitude;
	double longMinu = longitude;

	double latDeg = (int)latitude;
	double longDeg = (int)longitude;

	latMinu -= latDeg;
	longMinu -= longDeg;

	latMinu = Math.Round(latMinu * 60.0, 7);
	longMinu = Math.Round(longMinu * 60.0, 7);

	latDeg *= 100.0;
	longDeg *= 100.0;

	double latNMEA = latMinu + latDeg;
	double longNMEA = longMinu + longDeg;

	char NS = 'W';
	char EW = 'N';
	if (latitude >= 0) NS = 'N';
	else NS = 'S';
	if (longitude >= 0) EW = 'E';
	else EW = 'W';

	//sbGGA.clear();
	//sbGGA.Append("$GPGGA,");
	//sbGGA.Append(DateTime.Now.toString("HHmmss.00,", CultureInfo.InvariantCulture));
	//sbGGA.Append(Math.Abs(latNMEA).toString("0000.000", CultureInfo.InvariantCulture)).Append(',').Append(NS).Append(',');
	//sbGGA.Append(Math.Abs(longNMEA).toString("00000.000", CultureInfo.InvariantCulture)).Append(',').Append(EW);
	//sbGGA.Append(",1,10,1,43.4,M,46.4,M,5,0*");

	//sbGGA.Append(CalculateChecksum(sbGGA.toString()));
	//sbGGA.Append("\r\n");
	sbGGA.clear();
	sbGGA.Append("$GPGGA,");
	sbGGA.Append(DateTime.Now.toString("HHmmss.00,", CultureInfo.InvariantCulture));
	sbGGA.Append(Math.Abs(latNMEA).toString("0000.000", CultureInfo.InvariantCulture)).Append(',').Append(NS).Append(',');
	sbGGA.Append(Math.Abs(longNMEA).toString("00000.000", CultureInfo.InvariantCulture)).Append(',').Append(EW);
	sbGGA.Append(',').Append(fixQualityData.toString()).Append(',');
	sbGGA.Append(satellitesData.toString()).Append(',');

	if (hdopData > 0) sbGGA.Append(hdopData.toString("0.##", CultureInfo.InvariantCulture)).Append(',');

	else sbGGA.Append("1,");

	sbGGA.Append(altitudeData.toString("#.###", CultureInfo.InvariantCulture)).Append(',');
	sbGGA.Append("M,");
	sbGGA.Append("46.4,M,");  //udulation
	sbGGA.Append(ageData.toString("0.#", CultureInfo.InvariantCulture)).Append(','); //age
	sbGGA.Append("0*");

	sbGGA.Append(CalculateChecksum(sbGGA.toString()));
	sbGGA.Append("\r\n");
	/*
	   $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,5,0*47
	   0     1      2      3    4      5 6  7  8   9    10 11  12 13  14
	   Time      Lat       Lon     FixSatsOP Alt */
}
}
}
