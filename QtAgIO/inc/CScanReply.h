#ifndef CScanReply_H
#define CScanReply_H

#include <QString>
#include <QByteArray>
#include <QObject>

class CScanReply : public QObject
{
	Q_OBJECT
	public:
		explicit CScanReply(QObject *parent = nullptr);

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
};

#endif
