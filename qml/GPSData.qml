import QtQuick
import QtQuick.Controls.Fusion
import "components"

Rectangle{
    id: gpsData
    width: childrenRect.width
    height: childrenRect.height + 30
    color: "#4d4d4d"
    Column{
        id: column
        anchors.margins: 15
        TextLine{ color: "white"; text: qsTr("Lat ", "abbreviation for latitude")+(Number(aog.latitude).toLocaleString(Qt.locale(), 'f', 9))}
        TextLine{ color: "white"; text: qsTr("Lon ", "abbreviation for longitude")+(Number(aog.longitude).toLocaleString(Qt.locale(), 'f', 9))}
        TextLine{ color: "white"; text: qsTr("N ", "abbreviation for northing") + (Number(aog.northing).toLocaleString(Qt.locale(), 'f', 3))}
        TextLine{ color: "white"; text: qsTr("E ", "abbreviation for easting")+ (Number(aog.easting).toLocaleString(Qt.locale(), 'f', 3))}
        TextLine{ color: "white"; text: qsTr("Elev ", "abbreviation for elevation")+ (Number(aog.altitude).toLocaleString(Qt.locale(), 'f', 2))}
        TextLine{ color: "white"; text: qsTr("# Sats ", "abbreviation for # Satellites")+ aog.satellitesTracked}
        TextLine{ color: "white"; text: qsTr("HDOP ", "acronym for Horizontal Degree Of Position") + (Number(aog.hdop).toLocaleString(Qt.locale(), 'f', 3))}
        TextLine{ color: "white"; text: qsTr("Frame ")+ (Number(aog.frameTime).toLocaleString(Qt.locale(), 'f', 1))}
        TextLine{ color: "white"; text: qsTr("Raw Hz ", "abbreviation for Raw Hertz")+ (Number(aog.rawHz).toLocaleString(Qt.locale(), 'f', 1))}
        TextLine{ color: "white"; text: qsTr("Hz ", "abbreviation for Hertz")+ (Number(aog.hz).toLocaleString(Qt.locale(), 'f', 1))}
        TextLine{ color: "white"; text: qsTr("Dropped ")+ aog.droppedSentences}
        TextLine{ color: "white"; text: qsTr("Fix2Fix ")+ (Number(aog.heading * 180 / Math.PI).toLocaleString(Qt.locale(), 'f', 1))}//convert from radians
        TextLine{ color: "white"; text: qsTr("IMU ")+ (Number(aog.imuHeading * 180 / Math.PI).toLocaleString(Qt.locale(), 'f', 1))}
        TextLine{ color: "white"; text: qsTr("Heading ")+ (Number(aog.fusedHeading * 180 / Math.PI).toLocaleString(Qt.locale(), 'f', 1))}
        Item{
            visible: false //should reference a setting if rtk is turned on
            TextLine{ color: "white"; text: "<br>"}
            TextLine{ color: "white"; text: qsTr("RTK Age", "abbreviation for Real Time Kinematic Age")+ aog.rtkAge}
            TextLine{ color: "white"; text: qsTr("Fix: ")+aog.gpsFixQuality}
        }
    }
}
