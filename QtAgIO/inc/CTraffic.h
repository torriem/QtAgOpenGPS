#ifndef CTRAFFIC_H
#define CTRAFFIC_H

#include <QObject>

class  CTraffic : public QObject
{
	Q_OBJECT
	public: 
		explicit CTraffic(QObject *parent = nullptr);
        int cntrGPSIn = 0;
        int cntrGPSInBytes = 0;
        int cntrGPSOut = 0;

        unsigned int helloFromMachine = 99, helloFromAutoSteer = 99, helloFromIMU = 99;
};

#endif
