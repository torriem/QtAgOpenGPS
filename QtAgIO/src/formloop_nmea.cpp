#include "formloop.h"

QString FormLoop::fixQuality()const{
	switch (fixQualityData) {
		case 0:
			return "Invalid: ";
		case 1:
			return "GPS 1: ";
		case 2:
			return "DGPS: ";
		case 3:
			return "PPS: ";
		case 4:
			return "RTK fix: ";
		case 5:
			return "Float: ";
		case 6:
			return "Estimate: ";
		case 7:
			return "Man IP: ";
		case 8:
			return "Sim: ";
		default:
			return "Unknown: ";
	}
}

QString FormLoop::Parse(QString buffer)
{
	QString sentence;
	do
	{
		//double check for valid sentence
		// Find start of next sentence
		int start = buffer.indexOf('$');
		if (start == -1) return null;
		buffer = buffer.mid(start);

		// Find end of sentence
		int end = buffer.IndexOf("\r", StringComparison.Ordinal);
		if (end == -1) return null;

		//the NMEA sentence to be parsed
		//sentence = buffer.Substring(0, end + 1);
		sentence = buffer.left(end + 1);

		//remove the processed sentence from the rawBuffer
		//buffer = buffer.Substring(end + 1);
		buffer = buffer.mid(end + 1);
	}

	//if sentence has valid checksum, its all good
	while (!ValidateChecksum(sentence));

	// Remove trailing checksum and \r\n and return
	//sentence = sentence.Substring(0, sentence.IndexOf("*", StringComparison.Ordinal));
	sentence = sentence.left(sentence.indexOf('*'));

	return sentence;
}


void FormLoop::ParseNMEA(QString buffer)
{

	if (rawBuffer == null) return;

	//find end of a sentence
	int cr = rawBuffer.indexOf('\r');
	if (cr == -1) return; // No end found, wait for more data

	// Find start of next sentence
	int dollar = rawBuffer.indexOf('$');
	if (dollar == -1) return;

	//if the $ isn't first, get rid of the tail of corrupt sentence
	if (dollar >= cr) rawBuffer = rawBuffer.mid(dollar);

	cr = rawBuffer.indexOf('\r');
	if (cr == -1) return; // No end found, wait for more data
	dollar = rawBuffer.indexOf('$');
	if (dollar == -1) return;

	//if the $ isn't first, get rid of the tail of corrupt sentence
	if (dollar >= cr) rawBuffer = rawBuffer.mid(dollar);

	cr = rawBuffer.indexOf('\r');
	dollar = rawBuffer.indexOf('$');
	if (cr == -1 || dollar == -1) return;

	if (rawBuffer.length() > 301)
	{
		//if (isLogNMEA)
		//{
		//    logNMEASentence.Append("\r\n" +
		//        DateTime.UtcNow.ToString(" ->>  mm:ss.fff ", CultureInfo.InvariantCulture)
		//        + "\r\n" + rawBuffer + "\r\n");
		//}

		rawBuffer = "";
		return;
	}



	//now we have a complete sentence or more somewhere in the portData
	while (true)
	{
		//extract the next NMEA single sentence
		nextNMEASentence = Parse(ref buffer); //is this right? David 6/22/24
		if (nextNMEASentence == null) break;

		words = nextNMEASentence.split(',');

		//if (isLogNMEA)
		//{
		//    logNMEASentence.Append(DateTime.UtcNow.ToString("HHmmss.fff ", CultureInfo.InvariantCulture)
		//    + " " + nextNMEASentence + "\r\n");
		//}

		//parse them accordingly
		if (words.length() < 3) break;

		if ((words[0] == "$GPGGA" || words[0] == "$GNGGA") && words.length() > 13)
		{
			ParseGGA();
			if (isGPSSentencesOn) ggaSentence = nextNMEASentence;
		}

		else if ((words[0] == "$GPVTG" || words[0] == "$GNVTG") && words.length() > 7)
		{
			ParseVTG();
			if (isGPSSentencesOn) vtgSentence = nextNMEASentence;
		}

		//else if (words[0] == "$GPRMC" || words[0] == "$GNRMC")
		//{
		//    ParseRMC();
		//    if (isGPSSentencesOn) rmcSentence = nextNMEASentence;
		//}

		else if (words[0] == "$KSXT")
		{
			ParseKSXT();
			if (isGPSSentencesOn) ksxtSentence = nextNMEASentence;
		}

		else if (words[0] == "$GPHPD")
		{
			ParseHPD();
			if (isGPSSentencesOn) hpdSentence = nextNMEASentence;
		}

		else if (words[0] == "$PAOGI" && words.length() > 14)
		{
			ParseOGI();
			if (isGPSSentencesOn) paogiSentence = nextNMEASentence;
		}

		else if (words[0] == "$PANDA" && words.length() > 14)
		{
			ParsePANDA();
			if (isGPSSentencesOn) pandaSentence = nextNMEASentence;
		}

		else if (words[0] == "$GPHDT" || words[0] == "$GNHDT")
		{
			ParseHDT();
			if (isGPSSentencesOn) hdtSentence = nextNMEASentence;
		}

		else if (words[0] == "$PTNL" && words.length() > 8)
		{
			ParseAVR();
			if (isGPSSentencesOn) avrSentence = nextNMEASentence;
		}

		else if (words[0] == "$GNTRA")
		{
			ParseTRA();
		}

		else if (words[0] == "$PSTI" && (words[1] == "032" || words[1] == "035") )
		{
			ParseSTI032(); //there is also an $PSTI,030,... wich contains different data!
		}
	}// while still data

	if (isNMEAToSend)
	{
		isNMEAToSend = false;

		byte[] nmeaPGN = new byte[57]; //is this right? David 6/22/24

		nmeaPGN[0] = 0x80;
		nmeaPGN[1] = 0x81;
		nmeaPGN[2] = 0x7C;
		nmeaPGN[3] = 0xD6;
		nmeaPGN[4] = 0x33; // nmea total array count minus 6

		//longitude
		//Buffer.BlockCopy(BitConverter.GetBytes(longitudeSend), 0, nmeaPGN, 5, 8);
		memcpy(nmeaPGN + 5, &longitudeSend, 8);
		longitudeSend = DOUBLE_MAX;

		//latitude
		//Buffer.BlockCopy(BitConverter.GetBytes(latitudeSend), 0, nmeaPGN, 13, 8);
		memcpy(nmeaPGN + 13, &latitudeSend, 8);
		latitudeSend = DOUBLE_MAX;

		//the different dual antenna headings
		//Buffer.BlockCopy(BitConverter.GetBytes(headingTrueDual), 0, nmeaPGN, 21, 4);
		memcpy(nmeaPGN + 21, &headingTrueDual, 4);
		headingTrueDual = FLOAT_MAX;

		//single antenna heading in degrees
		//Buffer.BlockCopy(BitConverter.GetBytes(headingTrue), 0, nmeaPGN, 25, 4);
		memcpy(nmeaPGN + 25, &headingTrue, 4);
		headingTrue = FLOAT_MAX;

		//speed converted to kmh from knots
		//Buffer.BlockCopy(BitConverter.GetBytes(speed), 0, nmeaPGN, 29, 4);
		memcpy(nmeaPGN + 29, &speed, 4);
		speed = FLOAT_MAX;

		//roll value in degrees
		//Buffer.BlockCopy(BitConverter.GetBytes(roll), 0, nmeaPGN, 33, 4);
		memcpy(nmeaPGN + 33, &roll, 4);
		this->roll = FLOAT_MAX;

		//altitude in meters
		//Buffer.BlockCopy(BitConverter.GetBytes(altitude), 0, nmeaPGN, 37, 4);
		memcpy(nmeaPGN + 37, &altitude, 4);
		this->altitude = FLOAT_MAX;

		//Buffer.BlockCopy(BitConverter.GetBytes(satellitesTracked), 0, nmeaPGN, 41, 2);
		memcpy(nmeaPGN + 41, &satellitesTracked, 2);
		satellitesTracked = USHORT_MAX;

		nmeaPGN[43] = (byte)fixQuality;
		fixQuality = BYTE_MAX;

		//Buffer.BlockCopy(BitConverter.GetBytes(hdopX100), 0, nmeaPGN, 44, 2);
		memcpy(nmeaPGN + 44, &hdopX100, 2);
		hdopX100 = USHORT_MAX;

		//Buffer.BlockCopy(BitConverter.GetBytes(ageX100), 0, nmeaPGN, 46, 2);
		memcpy(nmeaPGN + 46, &ageX100, 2);
		ageX100 = USHORT_MAX;

		//Buffer.BlockCopy(BitConverter.GetBytes(imuHeading), 0, nmeaPGN, 48, 2);
		memcpy(nmeaPGN + 48, &imuHeading, 2);
		imuHeading = USHORT_MAX;

		//Buffer.BlockCopy(BitConverter.GetBytes(imuRoll), 0, nmeaPGN, 50, 2);
		memcpy(nmeaPGN + 50, &imuRoll, 2);
		imuRoll = SHORT_MAX;

		//Buffer.BlockCopy(BitConverter.GetBytes(imuPitch), 0, nmeaPGN, 52, 2);
		memcpy(nmeaPGN + 52, &imuPitch, 2);
		imuPitch = SHORT_MAX;

		//Buffer.BlockCopy(BitConverter.GetBytes(imuYawRate), 0, nmeaPGN, 54, 2);
		memcpy(nmeaPGN + 54, &imuYawRate, 2);
		imuYawRate = SHORT_MAX;


		int CK_A = 0;
		for (int j = 2; j < nmeaPGN.Length; j++)
		{
			CK_A += nmeaPGN[j];
		}

		//checksum
		nmeaPGN[56] = (byte)CK_A;

		//Send nmea to AgOpenGPS
		SendToLoopBackMessageAOG(nmeaPGN);

		//Send nmea to autosteer module 8888
		if (isSendNMEAToUDP) SendUDPMessage(nmeaPGN, epModule); //is this right? David 6/22/24
	}
}
void FormLoop::ParseKSXT()
{
	if (!words[1].isEmpty() && !words[2].isEmpty() && !words[3].isEmpty()
			&& !words[4].isEmpty() && !words[5].isEmpty())
	{

		bool ok; //did the parsing go ok? If so, use the val, if not, don't

		//double.TryParse(words[2], NumberStyles.Float, CultureInfo.InvariantCulture, out longitude);
		double longitude = words[2].toDouble(&ok); //try to parse
		if (ok){ //did it work? Use if it did, skip if it didn't
			longitudeSend = longitude;
		}

		//double.TryParse(words[3], NumberStyles.Float, CultureInfo.InvariantCulture, out latitude);
		double latitude = words[3].toDouble(&ok);
		if (ok){
			latitudeSend = latitude;
		}

		//float.TryParse(words[4], NumberStyles.Float, CultureInfo.InvariantCulture, out altitude);
		float altitude = words[4].toFloat(&ok);
		if (ok){
			altitudeSend = altitude;
		}

		//float.TryParse(words[5], NumberStyles.Float, CultureInfo.InvariantCulture, out headingTrueDual);
		float headingTrueDual = words[5].toFloat(&ok);
		if (ok){
			headingTrueDualData = headingTrueDual;
		}

		//float.TryParse(words[6], NumberStyles.Float, CultureInfo.InvariantCulture, out rollK);
		float rollK = words[6].toFloat(&ok); //why do we need to parse??

		//float.TryParse(words[8], NumberStyles.Float, CultureInfo.InvariantCulture, out speed);
		float speed = words[8].toFloat(&ok);
		if (ok){
			speedData = speed;
		}

		/*	byte.TryParse(words[10], NumberStyles.Float, CultureInfo.InvariantCulture, out fixQuality);
			if (fixQuality == 0) fixQualityData = 0;
			else if (fixQuality == 1) fixQualityData = 1;
			else if (fixQuality == 2) fixQualityData = 5;
			else if (fixQuality == 3) fixQualityData = 4;*/
		int fixQuality = words[10].toInt(&ok);
		if (ok) {
			switch (fixQuality) {
				case 0:
					fixQualityData = 0;
					break;
				case 1:
					fixQualityData = 1;
					break;
				case 2:
					fixQualityData = 5;
					break;
				case 3:
					fixQualityData = 4;
					break;
				default:
					break;
			}
		}

		fixQuality = fixQualityData;

		/*int headingQuality;

		  int.TryParse(words[11], NumberStyles.Float, CultureInfo.InvariantCulture, out headingQuality);

		  if (headingQuality == 3)   // roll only when rtk 
		  {
		  roll = (float)(rollK);
		  rollData = rollK;
		  }
		  else
		  {
		  roll = FLOAT_MIN;
		  rollData = 0;
		  }*/
		int headingQuality = words[11].toInt(&ok);

		if (headingQuality == 3) {
			roll = rollK;
			rollData = rollK;
		} else {
			roll = FLOAT_MIN; // Assuming FLOAT_MIN is defined appropriately
			rollData = 0;
		}

		//unsigned short.TryParse(words[13], NumberStyles.Float, CultureInfo.InvariantCulture, out satellitesTracked);
		unsigned short satellitesTracked = words[13].toUShort(&ok);
		if (ok){
			satellitesData = satellitesTracked;
		}


		//float.TryParse(words[20], NumberStyles.Float, CultureInfo.InvariantCulture, out ageData);
		float ageData = words[20].toFloat(&ok);
		if (ok){
			ageX100 = (unsigned short)(ageData * 100.0);
		}

		isNMEAToSend = true;
	}
}
void FormLoop::ParseGGA()
{

	{
		//$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M ,  ,*47

		//GGA          Global Positioning System Fix Data
		//123519       Fix taken at 12:35:19 UTC
		//4807.038,N   Latitude 48 deg 07.038' N
		//01131.000,E  Longitude 11 deg 31.000' E
		//1            Fix quality: 0 = invalid
		//                          1 = GPS fix (SPS)
		//                          2 = DGPS fix
		//                          3 = PPS fix
		//                          4 = Real Time Kinematic
		//                          5 = Float RTK
		//                          6 = estimated (dead reckoning) (2.3 feature)
		//                          7 = Manual input mode
		//                          8 = Simulation mode
		//08           Number of satellites being tracked
		//0.9          Horizontal dilution of position
		//545.4,M      Altitude, Meters, above mean sea level
		//46.9,M       Height of geoid (mean sea level) above WGS84 ellipsoid
		//(empty field) time in seconds since last DGPS update
		//(empty field) DGPS station ID number
		//*47          the checksum data, always begins with *

		if (!words[1].isEmpty() && !words[2].isEmpty() && !words[3].isEmpty()
				&& !words[4].isEmpty() && !words[5].isEmpty())
		{
			bool ok;

			//double.TryParse(words[1], NumberStyles.Float, CultureInfo.InvariantCulture, out double UTC);
			//if ((UTC < LastUpdateUTC ? 240000 + UTC : UTC) - LastUpdateUTC > 0.045)
			//{

			//FixQuality
			byte.TryParse(words[6], NumberStyles.Float, CultureInfo.InvariantCulture, out fixQuality);
			//I don't know about this

			fixQualityData = fixQuality;

			//satellites tracked
			//unsigned short.TryParse(words[7], NumberStyles.Float, CultureInfo.InvariantCulture, out satellitesTracked);
			unsigned short satellitesTracked = words[7].toUShort(&ok);
			if (ok){
				satellitesData = satellitesTracked;
			}

			//hdop
			//float.TryParse(words[8], NumberStyles.Float, CultureInfo.InvariantCulture, out hdopData);
			float hdopData = words[8].toFloat(&ok);
			if (ok){
				hdopX100 = (unsigned short)(hdopData * 100.0);
			}

			//altitude
			//float.TryParse(words[9], NumberStyles.Float, CultureInfo.InvariantCulture, out altitude);
			float altitude = words[9].toFloat(&ok);
			if (ok){
				altitudeData = altitude;
			}

			//age
			//float.TryParse(words[13], NumberStyles.Float, CultureInfo.InvariantCulture, out ageData);
			float ageData = words[13].toFloat(&ok);
			if (ok){
				ageX100 = (unsigned short)(ageData * 100.0);
			}

			//LastUpdateUTC = UTC;

			//get latitude and convert to decimal degrees
			int decim = words[2].indexOf(".");
			if (decim == -1)
			{
				words[2] += ".00";
				decim = words[2].indexOf(".");
			}

			decim -= 2;
			//double.TryParse(words[2].Substring(0, decim), NumberStyles.Float, CultureInfo.InvariantCulture, out latitude);
			double latitude = words[2].mid(0, decim).toDouble();

			//double.TryParse(words[2].Substring(decim), NumberStyles.Float, CultureInfo.InvariantCulture, out double temp);
			double temp = words[2].mid(decim).toDouble();
			temp *= 0.01666666666667;
			latitude += temp;
			if (words[3] == "S")
				latitude *= -1;
			latitudeSend = latitude;

			//get longitude and convert to decimal degrees
			decim = words[4].indexOf(".");
			if (decim == -1)
			{
				words[4] += ".00";
				decim = words[4].indexOf(".");
			}

			decim -= 2;
			//double.TryParse(words[4].Substring(0, decim), NumberStyles.Float, CultureInfo.InvariantCulture, out longitude);
			double longitude = words[4].mid(0, decim).toDouble();

			//double.TryParse(words[4].Substring(decim), NumberStyles.Float, CultureInfo.InvariantCulture, out temp);
			temp = words[4].mid(decim).toDouble();

			longitude += temp * 0.0166666666667;

			{ if (words[5] == "W") longitude *= -1; }
			longitudeSend = longitude;

			isNMEAToSend = true;                
		}
		}
		void FormLoop::ParseVTG()
		{
			//$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48

			//VTG          Track made good and ground speed
			//054.7,T      True track made good (degrees)
			//034.4,M      Magnetic track made good
			//005.5,N      Ground speed, knots
			//010.2,K      Ground speed, Kilometers per hour
			//*48          Checksum


			if (!words[7].isEmpty())
			{
				//kph for speed
				bool ok;

				//float.TryParse(words[7], NumberStyles.Float, CultureInfo.InvariantCulture, out speed);
				float speed = words[7].toFloat(&ok);
				if (ok){
					speedData = speed;
				}
			}
			else
			{
				speed = speedData = 0;
			}

			if (!words[1].isEmpty())
			{ 
				//True heading
				//float.TryParse(words[1], NumberStyles.Float, CultureInfo.InvariantCulture, out headingTrue);
				float headingTrue = words[1].toFloat(&ok);
				if (ok){
					headingTrueData = headingTrue;
				}
			}
		}
		void FormLoop::ParseAVR()
		{ 
			// $PTNL,AVR,145331.50,+35.9990,Yaw,-7.8209,Tilt,-0.4305,Roll,444.232,3,1.2,17 * 03

			//0 Message ID $PTNL,AVR
			//1 UTC of vector fix
			//2 Yaw angle, in degrees
			//3 Yaw
			//4 Tilt angle, in degrees
			//5 Tilt
			//6 Roll angle, in degrees
			//7 Roll
			//8 Range, in meters
			//9 GPS quality indicator:
			// 0: Fix not available or invalid
			// 1: Autonomous GPS fix
			// 2: Differential carrier phase solution RTK(Float)
			// 3: Differential carrier phase solution RTK(Fix)
			// 4: Differential code-based solution, DGPS
			//10 PDOP
			//11 Number of satellites used in solution
			//12 The checksum data, always begins with *

			bool ok;

			if (!words[1].isEmpty())
			{
				float.TryParse(words[8] == "Roll" ? words[7] : words[5], NumberStyles.Float, CultureInfo.InvariantCulture, out rollK);
				/*float rollK;
				  QString wordToParse = (words[8] == "Roll") ? words[7] : words[5];
				  rollK = wordToParse.toFloat(&ok);
				  if (!ok) {
				// Handle parsing failure, if needed
				}*/ //chatgpt says this shoud work


				//Kalman filter
				Pc = P + varProcess;
				G = Pc / (Pc + varRoll);
				P = (1 - G) * Pc;
				Xp = XeRoll;
				Zp = Xp;
				XeRoll = (G * (rollK - Zp)) + Xp;
				rollData = XeRoll;

				roll = (float)(XeRoll);
			}
		}

		void FormLoop::ParseHPD()
		{
			if (!words[1].isEmpty())
			{
				//Dual heading
				//float.TryParse(words[3], NumberStyles.Float, CultureInfo.InvariantCulture, out headingTrueDual);
				float headingTrueDual = words[3].toFloat(&ok);
				if (ok){
					headingTrueDualData = headingTrueDual;
				}

				//float.TryParse(words[4], NumberStyles.Float, CultureInfo.InvariantCulture, out rollK);
				float rollK = words[4].toFloat();

				//double.TryParse(words[18], NumberStyles.Float, CultureInfo.InvariantCulture, out double baseline);
				double baseline = words[18].toDouble();

				//int.TryParse(words[21], NumberStyles.Float, CultureInfo.InvariantCulture, out int solheading);
				//hmmm this was commented out in cs

				if (baseline > 0)   // roll only when rtk and baseline
				{
					roll = (float)(rollK);
					rollData = rollK;
				}
				else
				{
					roll = FLOAT_MIN;
					rollData = 0;
				}
			}
		}
		void FormLoop::ParseOGI()
		{

			/*
			   $PAOGI
			   (1) 123519 Fix taken at 1219 UTC

			   Roll corrected position
			   (2,3) 4807.038,N Latitude 48 deg 07.038' N
			   (4,5) 01131.000,E Longitude 11 deg 31.000' E

			   (6) 1 Fix quality: 
			   0 = invalid
			   1 = GPS fix(SPS)
			   2 = DGPS fix
			   3 = PPS fix
			   4 = Real Time Kinematic
			   5 = Float RTK
			   6 = estimated(dead reckoning)(2.3 feature)
			   7 = Manual input mode
			   8 = Simulation mode
			   (7) Number of satellites being tracked
			   (8) 0.9 Horizontal dilution of position
			   (9) 545.4 Altitude (ALWAYS in Meters, above mean sea level)
			   (10) 1.2 time in seconds since last DGPS update

			   (11) 022.4 Speed over the ground in knots - can be positive or negative

			   FROM AHRS:
			   (12) Heading in degrees
			   (13) Roll angle in degrees(positive roll = right leaning - right down, left up)
			   (14) Pitch angle in degrees(Positive pitch = nose up)
			   (15) Yaw Rate in Degrees / second

			 * CHKSUM
			 */
			bool ok;

			if (!words[1].isEmpty() && !words[2].isEmpty() && !words[3].isEmpty()
					&& !words[4].isEmpty() && !words[5].isEmpty())
			{
				//double.TryParse(words[1], NumberStyles.Float, CultureInfo.InvariantCulture, out double UTC);
				//if ((UTC < LastUpdateUTC ? 240000 + UTC : UTC) - LastUpdateUTC > 0.045)
				//{

				bool ok;
				//FixQuality
				byte.TryParse(words[6], NumberStyles.Float, CultureInfo.InvariantCulture, out fixQuality);
				//not sure
				fixQualityData = fixQuality;

				//satellites tracked
				//unsigned short.TryParse(words[7], NumberStyles.Float, CultureInfo.InvariantCulture, out satellitesTracked);
				unsigned short satellitesTracked = words[7].toUShort(&ok);
				if (ok){
					satellitesData = satellitesTracked;
				}

				//hdop
				//float.TryParse(words[8], NumberStyles.Float, CultureInfo.InvariantCulture, out hdopData);
				float hdopData = words[8].toFloat(&ok);
				if (ok){
					hdopX100 = (unsigned short)(hdopData * 100.0);
				}

				//altitude
				//float.TryParse(words[9], NumberStyles.Float, CultureInfo.InvariantCulture, out altitude);
				float altitude = words[9].toFloat(&ok);
				if (ok){
					altitudeData = altitude;
				}

				//kph for speed - knots read
				//ffloat.TryParse(words[11], NumberStyles.Float, CultureInfo.InvariantCulture, out speed);
				float speed = words[11].toFloat(&ok);
				speed *= 1.852f;
				speedData = speed;

				//Dual antenna derived heading
				//float.TryParse(words[12], NumberStyles.Float, CultureInfo.InvariantCulture, out headingTrueDual);
				float headingTrueDual = words[12].toFloat(&ok);
				if (ok){
					headingTrueDualData = headingTrueDual;
				}

				//roll
				//float.TryParse(words[13], NumberStyles.Float, CultureInfo.InvariantCulture, out roll);
				float roll = words[13].toFloat(&ok);
				if (ok){
					rollData = roll;
				}

				//get latitude and convert to decimal degrees
				//int decim = words[2].IndexOf(".", StringComparison.Ordinal);
				int decim = words[2].indexOf(".");
				if (decim == -1)
				{
					words[2] += ".00";
					decim = words[2].indexOf(".");
				}

				//age
				//float.TryParse(words[10], NumberStyles.Float, CultureInfo.InvariantCulture, out ageData);
				float ageData = words[10].toFloat(&ok);
				if (ok){
					ageX100 = (unsigned short)(ageData * 100.0);
				}

				decim -= 2;
				//double.TryParse(words[2].Substring(0, decim), NumberStyles.Float, CultureInfo.InvariantCulture, out latitude);
				double latitude = words[2].mid(0, decim).toDouble();
				//double.TryParse(words[2].Substring(decim), NumberStyles.Float, CultureInfo.InvariantCulture, out double temp);
				double temp = words[2].mid(decim).toDouble();

				temp *= 0.01666666666666666666666666666667;
				latitude += temp;
				if (words[3] == "S")
					latitude *= -1;

				latitudeSend = latitude;

				//get longitude and convert to decimal degrees
				decim = words[4].indexOf(".");
				if (decim == -1)
				{
					words[4] += ".00";
					decim = words[4].indexOf(".");
				}

				decim -= 2;
				//double.TryParse(words[4].Substring(0, decim), NumberStyles.Float, CultureInfo.InvariantCulture, out longitude);
				double longitude = words[4].mid(0, decim).toDouble();

				//double.TryParse(words[4].Substring(decim), NumberStyles.Float, CultureInfo.InvariantCulture, out temp);
				temp = words[4].mid(decim).toDouble();

				longitude += temp * 0.01666666666666666666666666666667;

				{ if (words[5] == "W") longitude *= -1; }
				longitudeSend = longitude;

				//always send because its probably the only one.
				isNMEAToSend = true;
			}
			}
			void FormLoop::ParsePanda()
			{
				/*
				   $PANDA
				   (1) Time of fix

				   position
				   (2,3) 4807.038,N Latitude 48 deg 07.038' N
				   (4,5) 01131.000,E Longitude 11 deg 31.000' E

				   (6) 1 Fix quality: 
				   0 = invalid
				   1 = GPS fix(SPS)
				   2 = DGPS fix
				   3 = PPS fix
				   4 = Real Time Kinematic
				   5 = Float RTK
				   6 = estimated(dead reckoning)(2.3 feature)
				   7 = Manual input mode
				   8 = Simulation mode
				   (7) Number of satellites being tracked
				   (8) 0.9 Horizontal dilution of position
				   (9) 545.4 Altitude (ALWAYS in Meters, above mean sea level)
				   (10) 1.2 time in seconds since last DGPS update
				   (11) Speed in knots

				   FROM IMU:
				   (12) Heading in degrees
				   (13) Roll angle in degrees(positive roll = right leaning - right down, left up)

				   (14) Pitch angle in degrees(Positive pitch = nose up)
				   (15) Yaw Rate in Degrees / second

				 * CHKSUM
				 */

				bool ok;
				if (!words[1].isEmpty() && !words[2].isEmpty() && !words[3].isEmpty()
						&& !words[3].isEmpty()) && !words[0].isEmpty())
						{

							//get latitude and convert to decimal degrees
							int decim = words[2].indexOf(".");
							if (decim == -1)
							{
								words[2] += ".00";
								decim = words[2].indexOf(".");
							}

							decim -= 2;
							//double.TryParse(words[2].Substring(0, decim), 
							//			NumberStyles.Float, CultureInfo.InvariantCulture, out latitude);

							double latitude = words[2].mid(0, decim).toDouble();
							/*double.TryParse(words[2].Substring(decim), 
							  NumberStyles.Float, CultureInfo.InvariantCulture, out double temp);*/
							double temp = words[2].mid(decim).toDouble();

							temp *= 0.01666666666666666666666666666667;
							latitude += temp;
							if (words[3] == "S")
								latitude *= -1;

							latitudeSend = latitude;

							//get longitude and convert to decimal degrees
							decim = words[4].indexOf(".");
							if (decim == -1)
							{
								words[4] += ".00";
								decim = words[4].indexOf(".");
							}

							decim -= 2;
							/*double.TryParse(words[4].Substring(0, decim), 
							  NumberStyles.Float, CultureInfo.InvariantCulture, out longitude);*/
							double longitude = words[4].mid(0, decim).toDouble();

							/*double.TryParse(words[4].Substring(decim), 
							  NumberStyles.Float, CultureInfo.InvariantCulture, out temp);*/
							temp = words[4].mid(decim).toDouble();

							longitude += temp * 0.01666666666666666666666666666667;

							{ if (words[5] == "W") longitude *= -1; }
							longitudeSend = longitude;

							//FixQuality
							byte.TryParse(words[6], NumberStyles.Float, CultureInfo.InvariantCulture, out fixQuality);
							fixQualityData = fixQuality;

							//satellites tracked
							//unsigned short.TryParse(words[7], NumberStyles.Float, CultureInfo.InvariantCulture, out satellitesTracked);
							unsigned short satellitesTracked = words[7].toUShort(&ok);
							if (ok) {
								satellitesData = satellitesTracked;
							}

							//hdop
							//float.TryParse(words[8], NumberStyles.Float, CultureInfo.InvariantCulture, out hdopData);
							float hdopData = words[8].toFloat(&ok);
							if (ok) {
								hdopX100 = (unsigned short)(hdopData * 100.0);
							}

							//altitude
							//float.TryParse(words[9], NumberStyles.Float, CultureInfo.InvariantCulture, out altitude);
							float altitude = words[9].toFloat(&ok);
							if (ok) {
								altitudeData = altitude;
							}

							//age
							//float.TryParse(words[10], NumberStyles.Float, CultureInfo.InvariantCulture, out ageData);
							float ageData = words[10].toFloat(&ok);
							if (ok) {
								ageX100 = (unsigned short)(ageData * 100.0);
							}

							//kph for speed - knots read
							//float.TryParse(words[11], NumberStyles.Float, CultureInfo.InvariantCulture, out speed);
							float speed = words[11].toFloat();
							speed *= 1.852f;
							speedData = speed;

							//imu heading
							//unsigned short.TryParse(words[12], NumberStyles.Float, CultureInfo.InvariantCulture, out imuHeading);
							unsigned short imuHeading = words[12].toUShort(&ok);
							if (ok) {
								imuHeadingData = imuHeading;
							}

							//roll
							//short.TryParse(words[13], NumberStyles.Float, CultureInfo.InvariantCulture, out imuRoll);
							short imuRoll = words[13].toShort(&ok);
							if (ok) {
								imuRollData = imuRoll;
							}

							//Pitch
							//short.TryParse(words[14], NumberStyles.Float, CultureInfo.InvariantCulture, out imuPitch);
							short imuPitch = words[14].toShort(&ok);
							if (ok) {
								imuPitchData = imuPitch;
							}

							//YawRate
							//short.TryParse(words[15], NumberStyles.Float, CultureInfo.InvariantCulture, out imuYawRate);
							short imuYawRate = words[15].toShort(&ok);
							if (ok) {
								imuYawRateData = imuYawRate;
							}

							//always send because its probably the only one.
							isNMEAToSend = true;
							//}
			}
		}
		void FormLoop::ParseHDT()
		{
			//$GNHDT,123.456,T * 00

			//(0)   Message ID $GNHDT
			//(1)   Heading in degrees
			//(2)   T: Indicates heading relative to True North
			//(3)   The checksum data, always begins with *


			bool ok;
			if (!words[1].isEmpty())
			{
				//True heading
				//float.TryParse(words[1], NumberStyles.Float, CultureInfo.InvariantCulture, out headingTrueDual);
				float headingTrueDual = words[1].toFloat(&ok);
				if (ok){
					headingTrueDualData = headingTrueDual;
				}
			}
		}

		void FormLoop::ParseSTI032()
		{
			//$PSTI,032,033010.000,111219,A,R,‐4.968,‐10.817,‐1.849,12.046,204.67,,,,,*39

			//(1) 032 Baseline Data indicator
			//(2) UTC time hhmmss.sss
			//(3) UTC date ddmmyy
			//(4) Status:
			//    V = Void
			//    A = Active
			//(5) Mode Indicator:
			//    F = Float RTK
			//    R = fixed RTK
			//(6) East-projection of baseline, meters
			//(7) North-projection of baseline, meters
			//(8) Up-projection of baseline, meters
			//(9) Baseline length, meters
			//(10) Baseline course: angle between baseline vector and north direction, degrees
			//(11) - (15) Reserved
			//(16) * Checksum

			bool ok;

			if (!words[10].isEmpty())
			{
				//baselineCourse: angle between baseline vector (from kinematic base to rover) and north direction, degrees
				//float.TryParse(words[10], NumberStyles.Float, CultureInfo.InvariantCulture, out float baselineCourse);
				float baselineCourse = words[10].toFloat(&ok);
				if (ok){
					headingTrueDual = ((baselineCourse < 270.0f) ? (baselineCourse + 90.0f) : (baselineCourse - 270.0f)); //Rover Antenna on the left, kinematic base on the right!!!
				}
			}

			if (!words[8].isEmpty() && !words[9].isEmpty())
			{
				//double.TryParse(words[8], NumberStyles.Float, CultureInfo.InvariantCulture, out double upProjection); //difference in hight of both antennas (rover - kinematic base)
				double upProjection = words[8].toDouble(&ok); //difference in hight of both antennas (rover - kinematic base)
															  //double.TryParse(words[9], NumberStyles.Float, CultureInfo.InvariantCulture, out double baselineLength); //distance between kinematic base and rover
				double baselineLength = words[9].toDouble(&ok); //distance between kinematic base and rover
				rollK = (float)glm.toDegrees(Math.Atan(upProjection / baselineLength)); //roll to the right is positiv (rover left, kinematic base right!)
																						//that won't work.
																						//Kalman filter
				Pc = P + varProcess;
				G = Pc / (Pc + varRoll);
				P = (1 - G) * Pc;
				Xp = XeRoll;
				Zp = Xp;
				XeRoll = (G * (rollK - Zp)) + Xp;
				rollData = XeRoll;

				roll = (float)(XeRoll);
			}
		}

		void FormLoop::ParseTRA()
		{
			bool ok;
			if (!words[1]isEmpty()
					{
					//float.TryParse(words[2], NumberStyles.Float, CultureInfo.InvariantCulture, out headingTrueDual);
					float headingTrueDual = words[2].toFloat(&ok);
					if (ok){
					headingTrueDualData = headingTrueDual;
					}

					//  Console.WriteLine(HeadingForced);
					//float.TryParse(words[3], NumberStyles.Float, CultureInfo.InvariantCulture, out rollK);
					float rollK = words[3].toFloat();
					// Console.WriteLine(nRoll);

					//int.TryParse(words[5], NumberStyles.Float, CultureInfo.InvariantCulture, out int trasolution);
					int trasolution = words[5].toInt(&ok);
					if (trasolution != 4) rollK = 0;
					rollData = rollK;
					roll = (float)(rollK);
					}
					}

		void FormLoop::ParseRMC()
		{
			//$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A

			//RMC          Recommended Minimum sentence C
			//123519       Fix taken at 12:35:19 UTC
			//A            Status A=active or V=Void.
			//4807.038,N   Latitude 48 deg 07.038' N
			//01131.000,E  Longitude 11 deg 31.000' E
			//022.4        Speed over the ground in knots
			//084.4        Track angle in degrees True
			//230394       Date - 23rd of March 1994
			//003.1,W      Magnetic Variation
			//*6A          * Checksum

			bool ok;

			if (!words[1].isEmpty() && !words[3].isEmpty()) && !words[4].isEmpty()
				&& !words[5].isEmpty() && !words[6].isEmpty())
				{
					//Convert from knots to kph for speed
					//float.TryParse(words[7], NumberStyles.Float, CultureInfo.InvariantCulture, out speed);
					float speed = words[7].toFloat();
					speed *= 1.852f;
					speedData = speed;

					//True heading
					//float.TryParse(words[8], NumberStyles.Float, CultureInfo.InvariantCulture, out headingTrueDual);
					float headingTrueDual = words[8].toFloat(&ok);
					if (ok){
						headingTrueDualData = headingTrueDual;
					}

					//double.TryParse(words[1], NumberStyles.Float, CultureInfo.InvariantCulture, out double UTC);
					double UTC = words[1].toDouble();
					if ((UTC < LastUpdateUTC ? 240000 + UTC : UTC) - LastUpdateUTC > 0.045)
					{
						LastUpdateUTC = UTC;

						//get latitude and convert to decimal degrees
						int decim = words[3].IndexOf(".", StringComparison.Ordinal);
						if (decim == -1)
						{
							words[3] += ".00";
							decim = words[3].IndexOf(".", StringComparison.Ordinal);
						}

						decim -= 2;


						//double.TryParse(words[3].Substring(0, decim), NumberStyles.Float, CultureInfo.InvariantCulture, out latitude);
						double latitude = words[3].mid(0, decim).toDouble();
						//double.TryParse(words[3].Substring(decim), NumberStyles.Float, CultureInfo.InvariantCulture, out double temp);
						double temp = words[3].mid(decim).toDouble();

						latitude += temp * 0.01666666666666666666666666666667;

						if (words[4] == "S")
							latitude *= -1;
						latitudeSend = latitude;

						//get longitude and convert to decimal degrees
						decim = words[5].indexOf(".");
						if (decim == -1)
						{
							words[5] += ".00";
							decim = words[5].indexOf(".");
						}

						decim -= 2;
						//double.TryParse(words[5].Substring(0, decim), NumberStyles.Float, CultureInfo.InvariantCulture, out longitude);
						double longitude = words[5].mid(0, decim).toDouble();
						//double.TryParse(words[5].Substring(decim), NumberStyles.Float, CultureInfo.InvariantCulture, out temp);
						temp = words[5].mid(decim).toDouble();
						longitude += temp * 0.01666666666666666666666666666667;

						if (words[6] == "W") longitude *= -1;
						longitudeSend = longitude;

						isNMEAToSend = true;
					}
				}
		}

		bool ValidateChecksum(QString Sentence)
		{
			int sum = 0;
			try
			{
				char[] sentenceChars = Sentence.ToCharArray();
				// All character xor:ed results in the trailing hex checksum
				// The checksum calc starts after '$' and ends before '*'

				int inx = Sentence.IndexOf("*", StringComparison.Ordinal);

				if (sentenceChars.Length - inx == 4)
				{
					for (inx = 1; ; inx++)
					{
						if (inx >= sentenceChars.length()) // No checksum found
							return false;
						var tmp = sentenceChars[inx];
						// Indicates end of data and start of checksum
						if (tmp == '*') break;
						sum ^= tmp;    // Build checksum
					}

					// Calculated checksum converted to a 2 digit hex string
					//string sumStr = string.Format("{0:X2}", sum);
					QString sumStr = QString::asprintf("%02X", sum);

					// Compare to checksum in sentence
					//return sumStr.Equals(Sentence.Substring(inx + 1, 2));
					return sumStr == Sentence.mid(inx + 1, 2));
				}
				else
				{
					//CRC code goes here - return true for now if $KS
					if(sentenceChars[0] == 36 && sentenceChars[1] == 75 && sentenceChars[2] == 83) return true;
					else return false;  
				}
			}
			catch (...)
			{
				qDebug() << "Error Validating Checksum";
				//mf.WriteErrorLog("Validate Checksum" + e);
				return false;
			}
		}
