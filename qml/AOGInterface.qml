import QtQuick 2.15
import QtQuick.Controls 2.15

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
    id: aogInterfaceType

    property bool isJobStarted: false

    property int manualBtnState: 0
    property int autoBtnState: 0

    //sections 0-15 are used for on-screen buttons if
    //not using zones.  If using zones the rest are used
    //for mapping zones to sections

    property variant sectionButtonState: [ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ]

    property bool isAutoSteerBtnOn: false

    onIsAutoSteerBtnOnChanged: {
        console.debug("isAutoSteerBtnOn is now " + isAutoSteerBtnOn)
    }

    property double latStart: 0
    property double lonStart: 0

    property double easting: 0
    property double northing: 0
    property double latitude: 0
    property double longitude: 0
    property double imuRollDegrees: 0
    property double speedKph: 0
    property double offlineDistance: 0


    property int currentABLine: -1 //use this instead of signals?
    property int currentABCurve: -1

    //onLatitudeChanged: {
    //    console.debug(latitude + ", " + longitude + ", " + easting + ", " + northing)
    //    console.debug(offlineDistance + ", " + imuRollDegrees + ", " + speedKph)
    //}

    //ABLine dialogs

    property var abLinesList: [
        {index: 0, name: "one", easting: 3, northing: 4, heading: 75, visible: true },
        {index: 1, name: "two", easting: 3, northing: 4, heading: 75, visible: true },
        {index: 2, name: "three", easting: 3, northing: 4, heading: 75, visible: true },
        {index: 3, name: "four", easting: 3, northing: 4, heading: 75, visible: true }
    ]

    property var abCurvesList: [
        {index: 0, name: "one", visible: true },
        {index: 1, name: "two", visible: true },
        {index: 2, name: "three", visible: true }
    ]

    signal settingChangedCPP(string key) //QML to let c++ know
    signal settingChangedQML(string key) //javascript call this to let qml types know



    //C++ can call this to let QML know about a change. Types will
    //connect themselves to the settingChangedQML() signal
    function settingsUpdated(key: string) {
        settingChangedQML(key)
    }


}
