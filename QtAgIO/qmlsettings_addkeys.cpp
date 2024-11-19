#include "qmlsettings.h"

void QMLSettings::setupKeys() {
    addKey(QString("setUDP_listenPort"), QString("comm/udpListenPort"), "int");
    addKey(QString("setUDP_sendPort"), QString("comm/udpSendPort"), "int");
	addKey(QString("setUDP_numConnections"), QString("comm/udpNumConnections"), "int");
    addKey(QString("setUDP_IP1"), QString("comm/udpIP1"), "int");
    addKey(QString("setUDP_IP2"), QString("comm/udpIP2"), "int");
    addKey(QString("setUDP_IP3"), QString("comm/udpIP3"), "int");
    addKey(QString("setLoop_listenPort"), QString("comm/loopListenPort"), "int");
    addKey(QString("setLoop_sendPort"), QString("comm/loopSendPort"), "int");
	addKey(QString("setNTRIP_isOn"), QString("comm/ntripIsOn"), "bool");
	addKey(QString("setRadio_isOn"), QString("comm/radioIsOn"), "bool");
	addKey(QString("setSerialPass_isOn"), QString("comm/serialPassIsOn"), "bool");
    addKey(QString("setNTRIP_casterPort"), QString("comm/ntripCasterPort"), "int");
	addKey(QString("setNTRIP_mount"), QString("comm/ntripMount"), "QString");
	addKey(QString("setNTRIP_userName"), QString("comm/ntripUserName"), "QString");
	addKey(QString("setNTRIP_userPassword"), QString("comm/ntripUserPassword"), "QString");
	addKey(QString("setNTRIP_sendToUDPPort"), QString("comm/ntripSendToUdpPort"), "int");
	addKey(QString("setNTRIP_sendGGAInterval"), QString("comm/ntripSendGGAInterval"), "int");
	addKey(QString("setNTRIP_isGGAManual"), QString("comm/ntripIsGGAManual"), "bool");
	addKey(QString("setNTRIP_manualLat"), QString("comm/ntripManualLat"), "double");
	addKey(QString("setNTRIP_manualLon"), QString("comm/ntripManualLon"), "double");
	addKey(QString("setNTRIP_isTCP"), QString("comm/ntripIsTCP"), "bool");
	addKey(QString("setNTRIP_isHTTP10"), QString("comm/ntripIsHTTP10"), "bool");
    addKey(QString("setNTRIP_ipAddress"), QString("comm/ntripIpAddress"), "QString");
    addKey(QString("setNTRIP_url"), QString("comm/ntripURL"), "QString");
    addKey(QString("run_isFirstRun"), QString("run/isFirstRun"), "bool");
}
