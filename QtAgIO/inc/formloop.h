#ifndef FormLoop_H
#define FormLoop_H

#include <QHostAddress>
#include <QNetworkDatagram>
#include <QString>
#include <QByteArray>
#include "CTraffic.h"
#include "CScanReply.h"
#include <QUdpSocket>
#include <limits>


class FormLoop : public QObject
{

	Q_OBJECT

	public: 
		explicit FormLoop(QObject *parent = nullptr);
		~FormLoop();

		static const double DOUBLE_MAX = std::numeric_limits<double>::max();
		static const double DOUBLE_MIN = std::numeric_limits<double>::min();
		static const double FLOAT_MAX = std::numeric_limits<float>::max();
		static const double FLOAT_MIN = std::numeric_limits<float>::min();

		static const double BYTE_MAX = std::numeric_limits<uchar>::max();
		static const double SHORT_MAX = std::numeric_limits<short>::max();
		static const double USHORT_MAX = std::numeric_limits<unsigned short>::max();

		/* formloop_udpcomm.cpp
		 * formerly UDP.designer.cs */
		bool isUDPNetworkConnected;

		//QByteArray ipAutoSet = "192.168.5";
		QByteArray ipAutoSet;


		//set up the networks
		void LoadUDPNetwork();
		void LoadLoopback();

		//send
		void SendUDPMessage(QByteArray byteData);
		void SendDataToLoopBack(QByteArray byteData);

		QUdpSocket *udpSocket;
		QUdpSocket *loopBackSocket;
		CTraffic traffic;
		CScanReply scanReply;

	private:

		//data stream
		//QByteArray buffer(1024);
		QByteArray buffer;

		//QByteArray helloFromAgIO( 0x80, 0x81, 0x7F, 200, 3, 56, 0, 0, 0x47 );
		QByteArray helloFromAgIO;

	public slots:

		void ReceiveFromLoopBack();
		void ReceiveFromUDP();
		//end of formloop_udpcomm.cpp


		/* formloop_parseNMEA.cpp
		 * formerly NMEA.designer.cs */
	private:
		QString rawBuffer = "";

		QString words;
		QString nextNMEASentence = "";

		bool isNMEAToSend = false;

	public:
		QString ggaSentence, vtgSentence, hdtSentence, avrSentence, paogiSentence, 
				hpdSentence, rmcSentence, pandaSentence, ksxtSentence;

		float hdopData, altitude = FLOAT_MAX, headingTrue = FLOAT_MAX,
			  headingTrueDual = FLOAT_MAX, speed = FLOAT_MAX, roll = FLOAT_MAX;
		float altitudeData, speedData, rollData, headingTrueData, headingTrueDualData, ageData;

		double latitudeSend = DOUBLE_MAX, longitudeSend = DOUBLE_MAX, latitude, longitude;


		unsigned short satellitesData, satellitesTracked = USHORT_MAX, hdopX100 = USHORT_MAX, 
					   ageX100 = USHORT_MAX;


		//imu data
		unsigned short imuHeadingData, imuHeading = USHORT_MAX;
		short imuRollData, imuRoll = SHORT_MAX, imuPitchData, imuPitch = SHORT_MAX, 
			  imuYawRateData, imuYawRate = SHORT_MAX;

		QByteArray fixQualityData, fixQuality; // = byte.MaxValue;HOY! byte max value needs to be set in the ctor


		float rollK, Pc, G, Xp, Zp, XeRoll, P = 1.0f;
		const float varRoll = 0.1f, varProcess = 0.0003f;

		double LastUpdateUTC = 0;

		QString fixQuality() const;
		QString Parse(QString buffer);

		void ParseNMEA(QString buffer);
		void ParseKSXT();
		void ParseGGA();
		void ParseVTG();
		void ParseHDT();
		void ParseAVR();
		void ParseOGI();
		void ParseHPD();
		void ParseRMC();
		void ParsePANDA();
		void ParseSTI032();
		void ParseTRANS();

		bool ValidateChecksum(QString Sentence);

		/* end of formloop_parseNMEA.cpp */
};


#endif
