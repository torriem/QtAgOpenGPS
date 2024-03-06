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

    /*
    Connections {
        target: settings
        function onSetMenu_isMetricChanged() {
            console.debug("isMetric is now",settings.setMenu_isMetric)
        }
    }
    */
    //Primarily these properties are updated by the backend c++ code
    //signals and data structures for specific UI functions are now
    //in the interfaces qml directory

    property double frameTime: 0

    property bool isJobStarted: true

    property int manualBtnState: 0
    property int autoBtnState: 0

    //sections 0-15 are used for on-screen buttons if
    //not using zones.  If using zones the rest are used
    //for mapping zones to sections

    property variant sectionButtonState: [ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ]

    property bool isAutoSteerBtnOn: false
    property bool isYouTurnBtnOn: false

    //onIsAutoSteerBtnOnChanged: {
    //    console.debug("isAutoSteerBtnOn is now " + isAutoSteerBtnOn)
    //}

    //General FormGPS information updated at GPS rate.
    property double latStart: 53
    property double lonStart: -111

    property double easting: 0
    property double northing: 0
    property double latitude: 53.2
    property double longitude: -111.2
    property double heading: 0//this is fix2fix heading
    property double toolEasting: 0
    property double toolNorthing: 0
    property double toolLatitude: 0
    property double toolLongitude: 0
    property double toolHeading: 0
    property double imuRollDegrees: 0
    signal changeImuRoll(double new_roll) //usually just set to 88888;

    property double speedKph: 0
    property double offlineDistance: 32000
    property double avgPivDistance: 32000

    property int steerModuleConnectedCounter: 0
    property bool steerSwitchHigh: false

    //david added these
    //formgps_position.cpp line 1144
    property double workedAreaTotal: 0
    property double workedAreaTotalUser: 0 //total acres covered
    property double actualAreaCovered: 0 //actual acres covered. Doesn't count overlap.
    property double distanceUser: 0
    property double areaOuterBoundary: 0
    property double areaBoundaryOuterLessInner: 0//outer minus the inner
    property double altitude: 0
    property double hdop: 0
    property double	age: 0
    property int fixQuality: 0
    property int satellitesTracked: 0

    property double imuHeading: 0
    signal changeImuHeading(double newImuHeading)

    property int angVel: 0//angular velocity I assume
    property string timeTilFinished: ""
    property string workRate: "value"
    property string percentOverlap: "value"
    property string percentLeft: "value"
    //these not added yet
    property double steerAngleActual: 0
    property double steerAngleSet: 0
    property double rawHZ:0
    property double hz:0
    property double missedSentences: 0
    property double gpsHeading: 0
    property double fusedHeading: 0

    property bool isTrackOn: false //checks if a guidance line is set.
    onCurrentABLineChanged: {
        if(currentABLine > -1 && isJobStarted === true){
            isTrackOn = true
        }
        else{
            isTrackOn = false
        }
    }

    //can we use these line properties for the Display?
    //AB Lines properties, signals, and methods
    property int currentABLine: -1
    property int currentABCurve: -1

    property double currentABLine_heading: 0 //TODO delete or move to interfaces/LinesInterface.qml.  seems to be unused

    property int current_trackNum: 0

    //on-screen buttons

    //DisplayButtons.qml
    signal zoomIn()
    signal zoomOut()
    signal tiltDown()
    signal tiltUp()
    signal btn2D()
    signal btn3D()
    signal n2D()
    signal n3D()


    signal uturn(bool turn_right)
    signal lateral(bool go_right)
    signal autoYouTurn()
    signal swapAutoYouTurnDirection()

    //general settings
    signal settings_reload() //tell backend classes to reload settings from store
    signal settings_tempsave() //save a temporary copy of all the settings
    signal settings_revert() //revert to temporary copy of all settings
    signal settings_save() //sync to disk, and also copy to current vehicle file, if active

    signal modules_send_238()

    signal sim_bump_speed(bool up)
    signal sim_zero_speed()

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
