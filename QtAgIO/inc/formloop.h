#ifndef FormLoop_H
#define FormLoop_H

#include <QHostAddress>
#include <QNetworkDatagram>
#include <QString>
#include <QByteArray>
#include "CTraffic.h"
#include "CScanReply.h"
#include <QUdpSocket>
#include "glm.h"
#include <cmath>
#include <QSettings>
#include "agiosettings.h"
#include <QTcpSocket>
#include <QTimer>
#include <QQueue>
#include <QDateTime>


class FormLoop : public QObject
{

	Q_OBJECT

	public:
		explicit FormLoop(QObject *parent = nullptr);
		~FormLoop();
		QSettings settings;


		QHostAddress udpIpAddress;
		//debug vars
		bool haveWeSentToParser = false;
		bool haveWeRecGGA = false;
		bool haveWeRecNDA = false;
		bool haveWeRecVTG = false;

		/*formloop.cpp
		 * formerly Formloop.cs */

		bool isGPSSentencesOn = false, isSendNMEAToUDP;
        int focusSkipCounter = 310;

		/* formloop_udpcomm.cpp
		 * formerly UDP.designer.cs */
	public:
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
		QString rawBuffer = 0;

		QStringList words;
		//QString words;
		QString nextNMEASentence = 0;

		bool isNMEAToSend = false;

	public:
		QString ggaSentence, vtgSentence, hdtSentence, avrSentence, paogiSentence,
				hpdSentence, rmcSentence, pandaSentence, ksxtSentence;

		float hdopData, altitude = glm::FLOAT_MAX, headingTrue = glm::FLOAT_MAX,
			  headingTrueDual = glm::FLOAT_MAX, speed = glm::FLOAT_MAX, roll = glm::FLOAT_MAX;
		float altitudeData, speedData, rollData, headingTrueData, headingTrueDualData, ageData;

		double latitudeSend = glm::DOUBLE_MAX, longitudeSend = glm::DOUBLE_MAX, latitude, longitude;


		unsigned short satellitesData, satellitesTracked = glm::USHORT_MAX, hdopX100 = glm::USHORT_MAX, ageX100 = glm::USHORT_MAX;


		//imu data
		unsigned short imuHeadingData, imuHeading = glm::USHORT_MAX;
		short imuRollData, imuRoll = glm::SHORT_MAX, imuPitchData, imuPitch = glm::SHORT_MAX, 
			  imuYawRateData, imuYawRate = glm::SHORT_MAX;

		quint8 fixQuality, fixQualityData = 255;


		float rollK, Pc, G, Xp, Zp, XeRoll, P = 1.0f;
		const float varRoll = 0.1f, varProcess = 0.0003f;

		double LastUpdateUTC = 0;

		QString FixQuality();
		QString Parse(QString &buffer);

		void ParseNMEA(QString rawBuffer);
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
		void ParseTRA();

		bool ValidateChecksum(QString Sentence);

		/* end of formloop_parseNMEA.cpp */

		/* formloop_ntripcomm.cpp
		 * formerly NTRIPComm.designer.cs */
	public:

		QString brandCasterIP;
		bool isNTRIP_RequiredOn = false;
		bool isNTRIP_Connected = false;
		bool isNTRIP_Starting = false;
		bool isNTRIP_Connecting = false;
		bool isNTRIP_Sending = false;
		bool isRunGGAInterval = false;

		bool isRadio_RequiredOn = false;
		bool isSerialPass_RequiredOn = false;
		unsigned int tripBytes = 0;

		void ConfigureNTRIP(); //I think this is all UI related??
		void StartNTRIP();
        void OnAddMessage(qint8 data);
        void SendNTRIP(qint8 data);
		void SendGGA();
		void OnConnect();
		void OnReceivedData();
		QString CalculateChecksum(QString Sentence);
	private:
		//for the NTRIP Client counting
		int ntripCounter = 10;

        QTcpSocket *clientSocket; //server connection   line 21
        qint8 casterRecBuffer = 2800; //received data buffer

        QTimer *tmr; //send gga back timer line 25

		QString mount;
		QString username;
		QString password;


		int broadCasterPort;

		int sendGGAInterval = 0;
		QString GGASentence;

		int toUDP_Port = 0;
		int NTRIP_Watchdog = 100;

		/*
		   internal SerialPort spRadio = new SerialPort("Radio", 9600, Parity.None, 8, StopBits.One);
		   */
		
		   QList<int> rList;
		   QList<int> aList;

		//NTRIP metering
		//Queue<byte> rawTrip = new Queue<byte>();
		QQueue<quint8> rawTrip;

		QString sbGGA; //string builder

		//forward declarations
		void DoNTRIPSecondRoutine();
		void ReconnectRequest();
		void IncrementNTRIPWatchDog();
		void SendAuthorization();
        void ntripMeterTimer_Tick();
		void NTRIPtick();
		void NtripPort_DataReceived();
		void ShutDownNTRIP();
		void SettingsShutDownNTRIP();
		void BuildGGA();
		QString ToBase64(QString str);
};


#endif
