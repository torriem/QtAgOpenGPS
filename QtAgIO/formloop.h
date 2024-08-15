#ifndef FormLoop_H
#define FormLoop_H

#include <QHostAddress>
#include <QNetworkDatagram>
#include <QString>
#include <QByteArray>
#include "CTraffic.h"
#include "CScanReply.h"
#include <QUdpSocket>
#include <QQmlApplicationEngine>
#include "glm.h"
#include <cmath>
#include <QSettings>
#include "agiosettings.h"
#include <QTcpSocket>
#include <QTimer>
#include <QQueue>
#include <QDateTime>
#include <QElapsedTimer>


class FormUDP;

//in cs, the ipEndPoint is both the ip address and port.
//cpp doesn't have that luxury, so we create a struct.
//we might as well put all 3 in here.
struct IPAndPort {
    QHostAddress address;
    quint16 portToSend;
    quint16 portToListen;

    IPAndPort(const QString &ipAddress, quint16 sendPort, quint16 listenPort)
        : address(ipAddress), portToSend(sendPort), portToListen(listenPort) {}

};

class FormLoop : public QObject
{

	Q_OBJECT

	public:
    explicit FormLoop(QObject *parent = nullptr);
		~FormLoop();
		QSettings settings;

        QObject *qml_root;
        QObject *agio;
        QWidget *qmlcontainer;

        IPAndPort wwwNtrip; //send ntrip to module
        IPAndPort ethUDP;//the main ethernet network
        IPAndPort ethModulesSet;

        quint16 sendNtripToModulePort = 2233; // sends to .255, so ip is the same as ethUDP

		//debug vars
        bool haveWeRecUDP = false;
        bool haveWeSentToParser = false;
		bool haveWeRecGGA = false;
		bool haveWeRecNDA = false;
		bool haveWeRecVTG = false;

        /*formloop_formUDP.cpp
         * formerly FormUDP.cs*/
    public slots:
        void btnSendSubnet_Click();
        void btnAutoSet_Click();


    public:
        //used to send communication check pgn= C8 or 200
        QByteArray sendIPToModules = QByteArray::fromRawData("\x80\x81\x7F\xC9\x05\xC9\xC9\xC0\xA8\x05\x47", 11);

        //QByteArray ipCurrent = { 192, 168, 5 };
        //QByteArray ipNew = { 192, 168, 5 };
        QByteArray ipCurrent;
        QByteArray ipNew;

        int tickCounter = 0;

        void SetFormLoop(FormLoop *formLoop);
        void FormUDp_Load();

        void ScanNetwork();

        void nudFirstIP_Click();

        /*formloop_ui.cpp*/

        void setupGUI();
        void ShowAgIO();
		void UpdateUIVars();

		/*formloop.cpp
		 * formerly Formloop.cs */
        int nmeaErrCounter = 0;

		bool isGPSSentencesOn = false, isSendNMEAToUDP;
        int focusSkipCounter = 310;
        bool isSendToSerial = false, isSendToUDP = true;

        bool lastHelloGPS, lastHelloAutoSteer, lastHelloMachine, lastHelloIMU;
        bool isConnectedIMU, isConnectedSteer, isConnectedMachine = true;

        int packetSizeNTRIP = 256;

        QTimer *ntripMeterTimer;

        QTimer *oneSecondTimer;

		QTimer *twoSecondTimer;

        QTimer *halfSecondTimer;

		void DoHelloAlarmLogic();
		void DoTraffic();

    public slots:
        void timer1_Tick();
        void oneSecondLoopTimer_Tick();
	    void TwoSecondLoop();


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
        void SendUDPMessage(QByteArray byteData, QHostAddress address, uint portNumber);
		void SendDataToLoopBack(QByteArray byteData);

		QUdpSocket *udpSocket;
		QUdpSocket *loopBackSocket;
		CTraffic traffic;
		CScanReply scanReply;

	private:

        QQmlApplicationEngine engine; //grasping at straws, here

		//data stream
		//QByteArray buffer(1024);
		QByteArray buffer;

		//QByteArray helloFromAgIO( 0x80, 0x81, 0x7F, 200, 3, 56, 0, 0, 0x47 );
        QByteArray helloFromAgIO = QByteArray::fromRawData("\x80\x81\x7F\xC8\x03\x38\x00\x00\x47", 9);


		public slots:

			void ReceiveFromLoopBack();
		void ReceiveFromUDP();
		//end of formloop_udpcomm.cpp


		/* formloop_parseNMEA.cpp
		 * formerly NMEA.designer.cs */
	private:

        double nowHz;
        double gpsHz;
		QString rawBuffer = 0;

		QStringList words;
		//QString words;
		QString nextNMEASentence = 0;

		bool isNMEAToSend = false;

	public:
        QElapsedTimer swFrame;
		QString ggaSentence, vtgSentence, hdtSentence, avrSentence, paogiSentence,
				hpdSentence, rmcSentence, pandaSentence, ksxtSentence;

		float hdopData, altitude = glm::FLOAT_MAX, headingTrue = glm::FLOAT_MAX,
			  headingTrueDual = glm::FLOAT_MAX, speed = glm::FLOAT_MAX, roll = glm::FLOAT_MAX;
		float altitudeData, speedData, rollData, headingTrueData, headingTrueDualData, ageData;

		double latitudeSend = glm::DOUBLE_MAX, longitudeSend = glm::DOUBLE_MAX, latitude, longitude;

        float previousAltitude = 0;
        bool nmeaError = false;

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
        QString Parse(QString& buffer);

        void ParseNMEA(QString& buffer);
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
        bool isNTRIP_RequiredOn = false; //these are all settings-derived vars. Set in ntripcomm
		bool isNTRIP_Connected = false;
		bool isNTRIP_Starting = false;
		bool isNTRIP_Connecting = false;
		bool isNTRIP_Sending = false;
		bool isRunGGAInterval = false;
		bool isRadio_RequiredOn = false;
		bool isSerialPass_RequiredOn = false;

		unsigned int tripBytes = 0;

        void ConfigureNTRIP();
		void StartNTRIP();
        void OnAddMessage(QByteArray data);
        void SendNTRIP(QByteArray data);
		void SendGGA();
		void OnConnect();
		void OnReceivedData();
		QString CalculateChecksum(QString Sentence);
	private:
		//for the NTRIP Client counting
		int ntripCounter = 10;
        bool debugNTRIP = false;

        QTcpSocket *clientSocket; //server connection   line 21
        QByteArray casterRecBuffer; //received data buffer
        //casterRecBuffer(2800); don't know if this is right yet

        QByteArray trip;

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
		void NtripPort_DataReceived();
		void ShutDownNTRIP();
		void SettingsShutDownNTRIP();
		void BuildGGA();
		QString ToBase64(QString str);
    private slots:
        void NTRIPDebugMode(bool doWeDebug);
};


#endif
