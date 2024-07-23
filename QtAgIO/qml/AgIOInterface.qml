import QtQuick
import QtQuick.Controls.Fusion

/* This type contains properties, signals, and functions to interface
   the C++ backend with the QML gui, while abstracting and limiting
   how much the C++ needs to know about the QML structure of the GUI,
   and how much the QML needs to know about the backends.

   This type also exposes the QSettings object for QML to use. However
   there are not change signals in the settings object, so we'll provide
   a signal here to let the backend know QML touched a setting.  Also
   a javascript function here that C++ can call to let QML objects know
   something in settings changed.

*/




Item {
    id: agioInterfaceType

	//These are the status booleans for the various connections.
	property bool ethernetConnected: false
	property bool ntripConnected: false
	property bool aogConnected: false
	property bool steerConnected: false
	property bool gpsConnected: false
	property bool imuConnected: false
	property bool machineConnected: false

}

