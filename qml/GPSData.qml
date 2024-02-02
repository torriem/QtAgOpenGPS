import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle{
    id: gpsData
    width: childrenRect.width
    height: childrenRect.height + 30
    color: "#4d4d4d"
    Column{
        id: column
        anchors.margins: 15
        TextLine{ color: "white"; text: qsTr("Lat "+(Number(aog.latitude).toLocaleString(Qt.locale(), 'f', 9)))}
        TextLine{ color: "white"; text: qsTr("Lon "+(Number(aog.longitude).toLocaleString(Qt.locale(), 'f', 9)))}
        TextLine{ color: "white"; text: qsTr("N " + (Number(aog.northing).toLocaleString(Qt.locale(), 'f', 3)))}
        TextLine{ color: "white"; text: qsTr("E "+ (Number(aog.easting).toLocaleString(Qt.locale(), 'f', 3)))}
        TextLine{ color: "white"; text: qsTr("Elev "+ (Number(aog.altitude).toLocaleString(Qt.locale(), 'f', 2)))}
        TextLine{ color: "white"; text: qsTr("# Sats "+ aog.satellitesTracked)}
        TextLine{ color: "white"; text: qsTr("HDOP " + (Number(aog.hdop).toLocaleString(Qt.locale(), 'f', 3)))}
        TextLine{ color: "white"; text: qsTr("Frame "+ (Number(aog.frameTime).toLocaleString(Qt.locale(), 'f', 1)))}
        TextLine{ color: "white"; text: qsTr("Raw Hz "+ aog.rawHZ)}
        TextLine{ color: "white"; text: qsTr("Hz "+ aog.hz)}
        TextLine{ color: "white"; text: qsTr("Missed "+ aog.missedSentences)}
        TextLine{ color: "white"; text: qsTr("Fix2Fix "+ (Number(aog.heading * 180 / Math.PI).toLocaleString(Qt.locale(), 'f', 1)))}
        TextLine{ color: "white"; text: qsTr("IMU "+ (Number(aog.imuHeading * 180 / Math.PI).toLocaleString(Qt.locale(), 'f', 1)))}
        TextLine{ color: "white"; text: qsTr("Heading "+ (Number(aog.fusedHeading * 180 / Math.PI).toLocaleString(Qt.locale(), 'f', 1)))}
        Item{
            visible: false //should reference a setting if rtk is turned on
            TextLine{ color: "white"; text: "<br>"}
            TextLine{ color: "white"; text: qsTr("RTK Age"+ aog.rtkAge)}
            TextLine{ color: "white"; text: qsTr("Fix: "+aog.gpsFixQuality)}
        }
    }
}
