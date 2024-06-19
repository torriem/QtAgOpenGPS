#include <QHostAddress>
#include <QNetworkDatagram>
#include <QString>
#include <QByteArray>

class UDP : public QObject{
	Q_OBJECT
};
class  CTraffic : public QObject
{
	public: 
        int cntrGPSIn = 0;
        int cntrGPSInBytes = 0;
        int cntrGPSOut = 0;

        unsigned int helloFromMachine = 99, helloFromAutoSteer = 99, helloFromIMU = 99;
		CTraffic();
};
//chatGPT

class CScanReply : public QObject
{
public:
    // Member variables
    QString steerIP;
    QString machineIP;
    QString GPS_IP;
    QString IMU_IP;
    QString subnetStr;

    QByteArray subnet;

    bool isNewSteer;
    bool isNewMachine;
    bool isNewGPS;
    bool isNewIMU;

    bool isNewData;

    // Constructor
    CScanReply();
};

/*class CScanReply : public QObject
{
	public:
        char steerIP =   "";
        char machineIP = "";
        char GPS_IP =    "";
        char IMU_IP =    "";
        char subnetStr = "";

        QByteArray subnet = { 0, 0, 0 };

        bool isNewSteer, isNewMachine, isNewGPS, isNewIMU;

        bool isNewData = false;
};*/

class FormLoop : public QObject
{
public: 
	bool isUDPNetworkConnected;

	//skiped the endpointj 
	//ditto the classes
	
	QByteArray ipAutoSet = "192.168.5";


    void LoadUDPNetwork();
	//void SendDataLoopAsync(IAsyncResult* asyncResult);
	//void SendUDPMessageAOG(QByteArray byteData, IPEndPoint endPoint);
	void SendUDPMessageAOG(QByteArray byteData);

private:

    //data stream
	QByteArray buffer(1024);

    QByteArray helloFromAgIO( 0x80, 0x81, 0x7F, 200, 3, 56, 0, 0, 0x47 );
	void LoadLoopback();
	//void SendToLoopbackMessageAOG(QByteArray byteData);
	void SendDataToLoopback(QByteArray byteData);
	void ReceiveFromLoopback(QByteArray data);
	//void ReceiveDataLoopAsync(IAsyncResult* asyncResult);
	//void SendDataUDPAsync(IAsyncResult* asyncResult);
	//void ReceiveDataUdpAsync(IAsyncResult* asyncResult);
	void ReceiveFromUDP(QByteArray data);
};

