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
        TextLine{ text: qsTr("Lat "+(Number(aog.latitude).toLocaleString(Qt.locale(), 'f', 9)))}
        TextLine{ text: qsTr("Lon "+(Number(aog.longitude).toLocaleString(Qt.locale(), 'f', 9)))}
        TextLine{ text: qsTr("N " + (Number(aog.northing).toLocaleString(Qt.locale(), 'f', 3)))}
        TextLine{ text: qsTr("E "+ (Number(aog.easting).toLocaleString(Qt.locale(), 'f', 3)))}
        TextLine{ text: qsTr("Elev "+ (Number(aog.altitude).toLocaleString(Qt.locale(), 'f', 2)))}
        TextLine{ text: qsTr("# Sats "+ aog.satellitesTracked)}
        TextLine{ text: qsTr("HDOP " + (Number(aog.hdop).toLocaleString(Qt.locale(), 'f', 3)))}
        TextLine{ text: qsTr("Frame "+ (Number(aog.frameTime).toLocaleString(Qt.locale(), 'f', 1)))}
        TextLine{ text: qsTr("Raw Hz "+ aog.rawHZ)}
        TextLine{ text: qsTr("Hz "+ aog.hz)}
        TextLine{ text: qsTr("Missed "+ aog.missedSentences)}
        TextLine{ text: qsTr("Fix2Fix "+ (Number(aog.heading * 180 / Math.PI).toLocaleString(Qt.locale(), 'f', 1)))}
        TextLine{ text: qsTr("IMU "+ (Number(aog.imuHeading * 180 / Math.PI).toLocaleString(Qt.locale(), 'f', 1)))}
        TextLine{ text: qsTr("Heading "+ (Number(aog.fusedHeading * 180 / Math.PI).toLocaleString(Qt.locale(), 'f', 1)))}
        Item{
            visible: false //should reference a setting if rtk is turned on
            TextLine{ text: "<br>"}
            TextLine{ text: qsTr("RTK Age"+ aog.rtkAge)}
            TextLine{ text: qsTr("Fix: "+aog.gpsFixQuality)}
        }
    }
}
