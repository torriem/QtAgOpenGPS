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
        TextLine{ text: qsTr(""+aog.latitude)}
        TextLine{ text: qsTr(""+aog.longitude)}
        TextLine{ text: qsTr("N " + aog.northing)}
        TextLine{ text: qsTr("E "+ aog.easting)}
        TextLine{ text: qsTr("Elev "+ aog.elevation)}
        TextLine{ text: qsTr("# Sats "+ aog.numOfSats)}
        TextLine{ text: qsTr("HDOP" + aog.gpsHDOP)}
        TextLine{ text: qsTr("Frame "+ aog.frame)}
        TextLine{ text: qsTr("Raw Hz "+ aog.rawHZ)}
        TextLine{ text: qsTr("Hz "+ aog.hz)}
        TextLine{ text: qsTr("Missed "+ aog.missedSentences)}
        TextLine{ text: qsTr("Fix2Fix "+ aog.gpsHeading)}
        TextLine{ text: qsTr("IMU "+ aog.imuHeading)}
        TextLine{ text: qsTr("Heading "+ aog.fusedHeading)}
        Item{
            visible: false //should reference a setting if rtk is turned on
            TextLine{ text: "<br>"}
            TextLine{ text: qsTr("RTK Age"+ aog.rtkAge)}
            TextLine{ text: qsTr("Fix: "+aog.gpsFixQuality)}
        }
    }
}
