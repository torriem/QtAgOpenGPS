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


class FormLoop : public QObject
{

	Q_OBJECT

	public: 
		explicit FormLoop(QObject *parent = nullptr);
		~FormLoop();

		/*formloop.cpp
		 * formerly Formloop.cs */
		
        bool isGPSSentencesOn = false, isSendNMEAToUDP;

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
        void ParseTRA();

        bool ValidateChecksum(QString Sentence);

		/* end of formloop_parseNMEA.cpp */
};


#endif
