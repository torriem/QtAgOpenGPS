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

    property double frameTime: 0

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

    property double latStart: 53
    property double lonStart: -111

    property double easting: 0
    property double northing: 0
    property double latitude: 0
    property double longitude: 0
    property double heading: 0
    property double toolEasting: 0
    property double toolNorthing: 0
    property double toolLatitude: 0
    property double toolLongitude: 0
    property double toolHeading: 0
    property double imuRollDegrees: 0
    property double speedKph: 0
    property double offlineDistance: 0


    property int currentABLine: -1 //use this instead of signals?
    property int currentABCurve: -1

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

    signal addABLine(string name, double easting, double northing, double heading)
    signal updateABLines()
    signal deleteABLine(int lineno)
    signal changeABLineName(int lineno, string new_name)
    signal setNewABLineAPoint(bool start_or_cancel, double easting, double northing, double heading) //true to mark point, false to cancel new point

    //this is a bit clumsy but it's the only way I can get
    //the signals defined above to be accessible to the other
    //qml files.  Some kind of scope limitation I guess
    function abLine_addLine(name, easting, northing, heading_radians) {
        console.debug("add new line: " + name + ", " + easting + ", " + northing + ", " + heading_radians)
        addABLine(name,easting, northing, heading_radians);
    }

    function abLine_updateLines() {
        updateABLines()
    }

    function abLine_deleteLine(index) {
        deleteABLine(index)
    }

    function abLine_changeName(index, new_name) {
        updateABLines(index, new_name)

    }

    function abLine_setA(start_or_cancel, easting, northing, heading) {
        setNewABLineAPoint(start_or_cancel, easting, northing, heading)
    }


    property double mPerDegreeLat
    property double mPerDegreeLon

    function setLocalMetersPerDegree() {
        mPerDegreeLat = 111132.92 - 559.82 * Math.cos(2.0 * latStart * 0.01745329251994329576923690766743) + 1.175
                              * Math.cos(4.0 * latStart * 0.01745329251994329576923690766743) - 0.0023
                              * Math.cos(6.0 * latStart * 0.01745329251994329576923690766743)

        mPerDegreeLon = 111412.84 * Math.cos(latStart * 0.01745329251994329576923690766743) - 93.5
                              * Math.cos(3.0 * latStart * 0.01745329251994329576923690766743) + 0.118
                              * Math.cos(5.0 * latStart * 0.01745329251994329576923690766743)
    }

    function convertLocalToWGS84(northing, easting) { //note northing first here
        var mPerDegreeLat = 111132.92 - 559.82 * Math.cos(2.0 * latStart * 0.01745329251994329576923690766743) + 1.175
                              * Math.cos(4.0 * latStart * 0.01745329251994329576923690766743) - 0.0023
                              * Math.cos(6.0 * latStart * 0.01745329251994329576923690766743)
        var mPerDegreeLon = 111412.84 * Math.cos(latStart * 0.01745329251994329576923690766743) - 93.5
                              * Math.cos(3.0 * latStart * 0.01745329251994329576923690766743) + 0.118
                              * Math.cos(5.0 * latStart * 0.01745329251994329576923690766743)

        var outLat = (northing / mPerDegreeLat) + latStart;
        var outLon = (easting  / mPerDegreeLon) + lonStart;

        return [ outLat, outLon ]
    }

    function convertWGS84ToLocal(latitude, longitude) {
        var mPerDegreeLat = 111132.92 - 559.82 * Math.cos(2.0 * latStart * 0.01745329251994329576923690766743) + 1.175
                              * Math.cos(4.0 * latStart * 0.01745329251994329576923690766743) - 0.0023
                              * Math.cos(6.0 * latStart * 0.01745329251994329576923690766743)
        var mPerDegreeLon = 111412.84 * Math.cos(latStart * 0.01745329251994329576923690766743) - 93.5
                              * Math.cos(3.0 * latStart * 0.01745329251994329576923690766743) + 0.118
                              * Math.cos(5.0 * latStart * 0.01745329251994329576923690766743)

        var outNorthing = (latitude - latStart) * mPerDegreeLat;
        var outEasting = (longitude - lonStart) * mPerDegreeLon;

        return [outNorthing, outEasting]
    }


}
