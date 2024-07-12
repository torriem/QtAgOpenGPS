#include "formloop.h"

//set up connection to caster
// main routine
void FormLoop::DoNTRIPSecondRoutine()
{
	//count up the ntrip clock only if everything is alive
	if (isNTRIP_RequiredOn || isRadio_RequiredOn || isSerialPass_RequiredOn)
	{
		IncrementNTRIPWatchDog();
	}

	//Have we NTRIP connection?
    //start if it is required
	if (isNTRIP_RequiredOn && !isNTRIP_Connected && !isNTRIP_Connecting)
	{
		if (!isNTRIP_Starting && ntripCounter > 20)
		{
			StartNTRIP();
		}
	}

    /*radio and serial
	//check if we need to send...?
	if ((isRadio_RequiredOn || isSerialPass_RequiredOn) && !isNTRIP_Connected && !isNTRIP_Connecting)
	{
		if (!isNTRIP_Starting)
		{
			StartNTRIP();
		}
    }*/

	if (isNTRIP_Connecting)
	{
		if (ntripCounter > 29)
		{
			qDebug() << "Connection Problem. Not Connecting To Caster";
			ReconnectRequest();
		}
        if (clientSocket != NULL && clientSocket->state() == QAbstractSocket::ConnectedState)
		{
			SendAuthorization();
		}
	}

	if (isNTRIP_RequiredOn || isRadio_RequiredOn)
	{

		//Bypass if sleeping
		if (focusSkipCounter != 0)
		{

			//watchdog for Ntrip
			if (isNTRIP_Connecting)
				qDebug() << "Authourizing";
			else
			{
				if (isNTRIP_RequiredOn && NTRIP_Watchdog > 10)
					qDebug() << "Waiting";
			}

			if (sendGGAInterval > 0 && isNTRIP_Sending)
			{
				qDebug() << "Sending";
				isNTRIP_Sending = false;
			}
		}
	}
}

void FormLoop::ConfigureNTRIP() //set the variables to the settings
{

	aList.clear();
	rList.clear();

	//start NTRIP if required
	isNTRIP_RequiredOn = settings.value("RTK/setNTRIP_isOn").toBool();
	isRadio_RequiredOn = settings.value("RTK/setRadio_isOn").toBool();
	isSerialPass_RequiredOn = settings.value("RTK/setPass_isOn").toBool();

	/*I'm not worrying about either one for now
	 * if (isRadio_RequiredOn || isSerialPass_RequiredOn)
	{
		// Immediatly connect radio
		ntripCounter = 20;
	}*/
}

void FormLoop::StartNTRIP()
{
	if (isNTRIP_RequiredOn)
	{
		//load the settings
		broadCasterPort = settings.value("RTK/setNTRIP_casterPort").toInt(); //Select correct port (usually 80 or 2101)
		mount = settings.value("RTK/setNTRIP_mount").toString(); //Insert the correct mount
		username = settings.value("RTK/setNTRIP_userName").toString(); //Insert your username!
		password = settings.value("RTK/setNTRIP_userPassword").toString(); //Insert your password!
		toUDP_Port = settings.value("RTK/setNTRIP_sendToUDPPort").toInt(); //send rtcm to which udp port
		sendGGAInterval = settings.value("RTK/setNTRIP_sendGGAInterval").toInt(); //how often to send fixes

		//if we had a timer already, kill it
        if (tmr != nullptr)
		{
			//tmr.Dispose();
			delete tmr;
		}

		//create new timer at fast rate to start
		if (sendGGAInterval > 0)
		{
			/*this.tmr = new System.Windows.Forms.Timer();
			this.tmr.Interval = 5000;
            this.tmr.Tick += new EventHandler(NTRIPtick);*/
			//is this right? David
            tmr = new QTimer(this);
            tmr->setSingleShot(false);
            tmr->setInterval(5000);
            connect(tmr, &QTimer::timeout, this, &FormLoop::SendGGA);
			tmr->start();

		}

		try
		{
			// Close the socket if it is still open
            if (clientSocket != NULL && clientSocket->state() == QAbstractSocket::ConnectedState)
            {
                //clientSocket.Shutdown(SocketShutdown.Both);
                //clientSocket.Close();
                //correct? David
                clientSocket->disconnectFromHost();
                if (clientSocket->state() != QAbstractSocket::UnconnectedState) {
                    clientSocket->waitForDisconnected(3000); // Optional: Wait for up to 3 seconds to ensure the socket is disconnected
                }
                clientSocket->close();
            }

			//NTRIP endpoint
			uint ip1 = settings.value("UDPComm/IP1").toUInt();
			uint ip2 = settings.value("UDPComm/IP2").toUInt();
			uint ip3 = settings.value("UDPComm/IP3").toUInt();
			uint ip4 = 255; //broadcast

			quint32 ipAddress = (ip1 << 24) | (ip2 << 16) | (ip3 << 8) | ip4;


            //this is the socket that sends to the receiver
			// Create the socket object
			clientSocket = new QTcpSocket(this);

			//set socket to non-blocking mode
            clientSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

			//clientSocket.BeginConnect(new IPEndPoint(IPAddress.Parse(broadCasterIP), broadCasterPort), new AsyncCallback(OnConnect), null);
			// Connect to server
			clientSocket->connectToHost(QHostAddress(ipAddress), broadCasterPort);

		}
                    //clientSocket.BeginConnect(new IPEndPoint(IPAddress.Parse(broadCasterIP), broadCasterPort), new AsyncCallback(OnConnect), null);
					//todo: we need a line like this 
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
	}
    /*radio
	 * If someone wants to attempt this, go ahead. However, I don't think many
	 * if anyone uses it. 
	 * I think it is for a radio connected to AgIO over usb, not a radio plugged
	 * into the receiver. David
	 * Later: Maybe a plugin or extension for this
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
	qDebug() << "Reconnect Request";
	ntripCounter = 15;
	isNTRIP_Connected = false;
	isNTRIP_Starting = false;
	isNTRIP_Connecting = false;

	//if we had a timer already, kill it
    if (tmr != nullptr)
	{
		delete tmr;
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
    if (sendGGAInterval > 0 && ntripCounter == 40) tmr->setInterval(sendGGAInterval * 1000);
}

void FormLoop::SendAuthorization()
{
	// Check we are connected
    if (clientSocket == NULL || clientSocket->state() != QAbstractSocket::ConnectedState)
	{
		qDebug() << "At the StartNTRIP()";
		ReconnectRequest();
		return;
	}

	// Read the message from settings and send it
	try
	{
		if (!settings.value("RTK/setNTRIP_isTCP").toBool())//if we are not using TCP. Should that go in an extension also?
														   //a sort of "ntrip weirdos" for all the unusual stuff to go.
		{
			//encode user and password
			QString auth = ToBase64(username + ":" + password);

			//grab location sentence
			BuildGGA();
            GGASentence = sbGGA;

			QString htt;
            if (settings.value("RTK/setNTRIP_isHTTP10").toBool()) htt = "1.0";
			else htt = "1.1";

			//Build authorization string
			QString str = "GET /" + mount + " HTTP/" + htt + "\r\n";
			str += "User-Agent: NTRIP AgOpenGPSClient/20221020\r\n";
			str += "Authorization: Basic " + auth + "\r\n"; //This line can be removed if no authorization is needed
															//str += GGASentence; //this line can be removed if no position feedback is needed
			str += "Accept: */*\r\nConnection: close\r\n";
			str += "\r\n";

            // Convert to byte array and send.
            // is this right? david
            //Byte[] byteDateLine = Encoding.ASCII.GetBytes(str.ToCharArray());
            QByteArray byteDateLine = str.toLatin1();

            clientSocket.Send(byteDateLine, byteDateLine.length(), 0);

			//enable to periodically send GGA sentence to server.
            if (sendGGAInterval > 0) tmr->start();

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

void FormLoop::OnAddMessage(qint8 data)
{

	//reset watchdog since we have updated data
	NTRIP_Watchdog = 0;

	if (isNTRIP_RequiredOn)
	{
		//move the ntrip stream to queue
        for (int i = 0; i < data.length(); i++)
		{
            rawTrip.enqueue(data[i]);
		}

		ntripMeterTimer.Enabled = true;
	}
	else
	{
		//send it
		SendNTRIP(data);
	}


}

void FormLoop::ntripMeterTimer_Tick()
{
	//we really should get here, but have to check
    if (rawTrip.size() == 0) return;

	//how many bytes in the Queue
    int cnt = rawTrip.size();

	//how many sends have occured
	traffic.cntrGPSIn++;

	//128 bytes chunks max
	if (cnt > packetSizeNTRIP) cnt = packetSizeNTRIP;

	//new data array to send
    qint8 trip = cnt;

	traffic.cntrGPSInBytes += cnt;

	//dequeue into the array
    for (int i = 0; i < cnt; i++) trip[i] = rawTrip.dequeue();

	//send it
	SendNTRIP(trip);

	//Are we done?
    if (rawTrip.size() == 0)
	{
		ntripMeterTimer.Enabled = false;

		if (focusSkipCounter != 0)
			traffic.cntrGPSInBytes = 0;
	}

	//Can't keep up as internet dumped a shit load so clear
    if (rawTrip.size() > 10000) rawTrip.clear();
}

void FormLoop::SendNTRIP(qint8 data)
{
	//serial send out GPS port
	/* don't worry about serial
	 *
	if (isSendToSerial)
	{
		SendGPSPort(data);
	}*/

	//send out UDP Port
	if (isSendToUDP)
	{
        SendUDPMessage(data, epNtrip);//I guess ip is set in the function. change before work
	}
}

void FormLoop::SendGGA()
{
	//timer may have brought us here so return if not connected
	if (!isNTRIP_Connected)
		return;
	// Check we are connected
    if (clientSocket == NULL || clientSocket->state() != QAbstractSocket::ConnectedState)
	{
		ReconnectRequest();
		return;
	}

	// Read the message from the text box and send it
	try
	{
		isNTRIP_Sending = true;
		BuildGGA();
        //I'm not sure about this. sbGGA is a stringbuilder in cs, however, I think it's a
        //regular qstring in qt.
        //QString str = sbGGA.toString();
        QString str = sbGGA;

        QByteArray byteDateLine = str.toLatin1();
        clientSocket.Send(byteDateLine, byteDateLine.length(), 0);
	}
	catch (...)
	{
		ReconnectRequest();
	}
}

void FormLoop::OnConnect()//where we begin to listen--if we are connected
{
	// Check if we were sucessfull
	try
	{
        if (clientSocket->state() == QAbstractSocket::ConnectedState)
			clientSocket.BeginReceive(casterRecBuffer, 0, casterRecBuffer.Length, SocketFlags.None, new AsyncCallback(OnRecievedData), null);
	}
	catch (...)
	{
		//MessageBox.Show(ex.Message, "Unusual error during Connect!");
	}
}

 void FormLoop::FormLoop::OnRecievedData() //where we listen
{
	// Check if we got any data
	try
	{
		int nBytesRec = clientSocket.EndReceive(ar);
		if (nBytesRec > 0)
		{
            QByteArray localMsg(nBytesRec, 0);
			Array.Copy(casterRecBuffer, localMsg, nBytesRec);

			BeginInvoke((MethodInvoker)(() => OnAddMessage(localMsg)));
			clientSocket.BeginReceive(casterRecBuffer, 0, casterRecBuffer.Length, SocketFlags.None, new AsyncCallback(OnRecievedData), null);
		}
		else
		{
			// If no data was recieved then the connection is probably dead
            qDebug() << "Shutting down clientSocket as we got no data";
			clientSocket.Shutdown(SocketShutdown.Both);
			clientSocket.Close();
		}
	}
	catch (...)
	{
		qDebug() << "Unusual error druing Recieve!";
	}
}

void FormLoop::NtripPort_DataReceived()//this is the serial port, right?
{
	// Check if we got any data
	/*try
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
		qDebug() << "Unusual error druing Recieve!";
	}*/
	//do we need to return??
}

 QString FormLoop::ToBase64(QString str)
{
    /*Encoding asciiEncoding = Encoding.ASCII;
	byte[] byteArray = new byte[asciiEncoding.GetByteCount(str)];
	byteArray = asciiEncoding.GetBytes(str);
	return Convert.ToBase64String(byteArray, 0, byteArray.Length);
    is the following code correct??
    */

    // Get the ASCII-encoded QByteArray
    QByteArray byteArray = str.toLatin1(); // Latin-1 is equivalent to ASCII for plain English characters

    // Convert to Base64
    QString base64String = byteArray.toBase64();

    return base64String;
}

void FormLoop::ShutDownNTRIP()
{
    if (clientSocket != NULL && clientSocket->state() == QAbstractSocket::ConnectedState)
	{
		//shut it down
                clientSocket->disconnectFromHost();
                if (clientSocket->state() != QAbstractSocket::UnconnectedState) {
                    clientSocket->waitForDisconnected(3000); // Optional: Wait for up to 3 seconds to ensure the socket is disconnected
                }
                clientSocket->close();
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
    if (clientSocket != NULL && clientSocket->state() == QAbstractSocket::ConnectedState)
    {
        clientSocket->disconnectFromHost();
        if (clientSocket->state() != QAbstractSocket::UnconnectedState) {
            clientSocket->waitForDisconnected(3000); // Optional: Wait for up to 3 seconds to ensure the socket is disconnected
        }
        clientSocket->close();
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
QString FormLoop::CalculateChecksum(QString Sentence)
{
	int sum = 0, inx;
    QChar sentence_chars = Sentence.toWCharArray();
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
    //return String.Format("{0:X2}", sum);
    return QString::asprintf("%02X", sum); //correct??

}

//private readonly StringBuilder sbGGA = new StringBuilder();
//see formloop.h line 195

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
        //latitude = this.latitude;
        latitude = this->latitude; //correct??
        longitude = this->longitude;
	}

	//convert to DMS from Degrees
	double latMinu = latitude;
	double longMinu = longitude;

	double latDeg = (int)latitude;
	double longDeg = (int)longitude;

	latMinu -= latDeg;
	longMinu -= longDeg;

	//is this section right?? ChatGPT says it is. David
	//latMinu = Math.Round(latMinu * 60.0, 7);
	latMinu = qRound(latMinu * 60 * 1000000) / 1000000.0;
	//longMinu = Math.Round(longMinu * 60.0, 7);
	longMinu = qRound(longMinu * 60 * 1000000) / 1000000.0;

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
    //sbGGA.append("$GPGGA,");
    //sbGGA.append(DateTime.Now.toString("HHmmss.00,", CultureInfo.InvariantCulture));
    //sbGGA.append(Math.Abs(latNMEA).toString("0000.000", CultureInfo.InvariantCulture)).append(',').append(NS).append(',');
    //sbGGA.append(Math.Abs(longNMEA).toString("00000.000", CultureInfo.InvariantCulture)).append(',').append(EW);
    //sbGGA.append(",1,10,1,43.4,M,46.4,M,5,0*");

    //sbGGA.append(CalculateChecksum(sbGGA.toString()));
    //sbGGA.append("\r\n");
	sbGGA.clear();
    sbGGA.append("$GPGGA,");
    //sbGGA.append(DateTime.Now.toString("HHmmss.00,", CultureInfo.InvariantCulture));
	sbGGA.append(QDateTime::currentDateTime().toString("HHmmss.00,"));
    //sbGGA.append(abs(latNMEA).toString("0000.000", CultureInfo.InvariantCulture)).append(',').append(NS).append(',');
    sbGGA.append(QString::number(abs(latNMEA), 'f', 3)).append(',').append(NS).append(',');
    //sbGGA.append(abs(longNMEA).toString("00000.000", CultureInfo.InvariantCulture)).append(',').append(EW);
	sbGGA.append(QString::number(abs(longNMEA), 'f', 3)).append(',').append(EW);
    //sbGGA.append(',').append(fixQualityData.toString()).append(',');
	sbGGA.append(",").append(QString::number(fixQualityData)).append(',');
    //sbGGA.append(satellitesData.toString()).append(',');
	sbGGA.append(QString::number(satellitesData)).append(',');

    //if (hdopData > 0) sbGGA.append(hdopData.toString("0.##", CultureInfo.InvariantCulture)).append(',');
	if (hdopData > 0) sbGGA.append(QString::number(hdopData, 'f', 2)).append(',');

    else sbGGA.append("1,");

    //sbGGA.append(altitudeData.toString("#.###", CultureInfo.InvariantCulture)).append(',');
	sbGGA.append(QString::number(altitudeData, 'f', 3)).append(',');
    sbGGA.append("M,");
    sbGGA.append("46.4,M,");  //udulation
    //sbGGA.append(ageData.toString("0.#", CultureInfo.InvariantCulture)).append(','); //age
    sbGGA.append(QString::number(ageData, 'f', 1)).append(','); //age
    sbGGA.append("0*");

    sbGGA.append(CalculateChecksum(sbGGA));
    sbGGA.append("\r\n");
	/*
	   $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,5,0*47
	   0     1      2      3    4      5 6  7  8   9    10 11  12 13  14
	   Time      Lat       Lon     FixSatsOP Alt */
}

