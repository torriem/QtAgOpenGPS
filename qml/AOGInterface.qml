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


    //AB Lines properties, signals, and methods
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

    signal abLine_updateLines()

    signal abLine_addLine(string name, double easting, double northing, double heading)
    signal abLine_deleteLine(int index)
    //signal changeABLineName(int lineno, string new_name)
    signal abLine_changeName(int which_one, string new_name)
    signal abLine_setA(bool start_or_cancel, double easting, double northing, double heading) //true to mark point, false to cancel new point
    signal abLine_swapHeading(int index)

    //on-screen buttons

    signal uturn(bool turn_right)
    signal lateral(bool go_right)


    //misc math functions
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

    function speed_unit() {
        if (settings.setMenu_isMetric)
            return qsTr("km/h","abbreviation for kilometers per hour")
        else
            return qsTr("MPH", "abbreviation for miles per hour")
    }

    function convert_speed(speed) {
        if (settings.setMenu_isMetric)
            return speed
        else
            return speed * 0.62137 //convert to mph
    }

    function convert_speed_text(speed, decimal_places) {
        if (settings.setMenu_isMetric)
            return Number(speed).toLocaleString(Qt.locale(), 'f', decimal_places)
        else
            return Number(speed * 0.62137).toLocaleString(Qt.locale(), 'f', decimal_places) //convert to mph
    }

    function cm_unit() {
        if(settings.setMenu_isMetric)
            return qsTr("cm", "centimeter abbreviation")
        else
            return qsTr("in", "inches abbreviation")
    }

    function cm_convert(distance) {
        if(settings.setMenu_isMetric)
            return distance
        else
            return distance / 25.4 //convert to inches
    }

    function cm_convert_text(distance, decimal_places) {
        if(settings.setMenu_isMetric)
            return Number(distance).toLocaleString(Qt.locale(),'f', decimal_places)
        else
            return Number(distance / 25.4).toLocaleString(Qt.locale(), 'f', decimal_places) //convert to inches
    }

    function m_unit() {
        if(settings.setMenu_isMetric)
            return qsTr("m", "metre abbreviation")
        else
            return qsTr("ft", "imperial feet abbreviation")
    }

    function m_convert(distance) {
        if(settings.setMenu_isMetric)
            return distance
        else
            return distance / 3.2808 //convert to ft
    }

    function m_convert_text(distance, decimal_places) {
        if(settings.setMenu_isMetric)
            return Number(distance).toLocaleString(Qt.locale(),'f', decimal_places)
        else
            return Number(distance / 3.2808).toLocaleString(Qt.locale(), 'f', decimal_places) //convert to ft
    }

    function km_unit() {
        if(settings.setMenu_isMetric)
            return qsTr("km", "kilometre abbreviation")
        else
            return qsTr("mi", "imperial miles abbreviation")
    }

    function km_convert(distance) {
        if(settings.setMenu_isMetric)
            return distance
        else
            return distance / 0.62137 //convert to miles
    }

    function km_convert_text(distance, decimal_places) {
        if(settings.setMenu_isMetric)
            return Number(distance).toLocaleString(Qt.locale(),'f', decimal_places)
        else
            return Number(distance / 0.62137).toLocaleString(Qt.locale(), 'f', decimal_places) //convert to miles
    }

}
