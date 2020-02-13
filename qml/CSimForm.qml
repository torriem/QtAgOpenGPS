import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    id: cSim
    width: 380
    height: 460
    background: Rectangle {
        border.color: "transparent"
        color: "transparent"
    }
    ColumnLayout {
        x: 8
        y: 15
        Grid {

            Text {
                id: text1
                width: 91
                height: 28
                text: qsTr("Status")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
            }

            Text {
                id: simStatus
                width: 91
                height: 28
                font.pixelSize: 12
            }

            Text {
                id: text6
                width: 91
                height: 28
                text: qsTr("Zone")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
            }

            Text {
                id: simZone
                width: 91
                height: 28
                font.pixelSize: 12
            }

            Text {
                id: text2
                width: 91
                height: 28
                text: qsTr("Fix Quality")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
            }

            Text {
                id: simQuality
                width: 91
                height: 28
                font.pixelSize: 12
            }

            Text {
                id: text7
                width: 91
                height: 28
                text: qsTr("Northing")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
            }

            Text {
                id: simNorth
                width: 91
                height: 28
                font.pixelSize: 12
            }

            Text {
                id: text3
                width: 91
                height: 28
                text: qsTr("Number Of Sats")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
            }

            Text {
                id: simSats
                width: 91
                height: 28
                font.pixelSize: 12
            }

            Text {
                id: text8
                width: 91
                height: 28
                text: qsTr("Easting")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
            }

            Text {
                id: simEast
                width: 91
                height: 28
                font.pixelSize: 12
            }

            Text {
                id: text4
                width: 91
                height: 28
                text: qsTr("HDOP")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
            }

            Text {
                id: simHDOP
                width: 91
                height: 28
                font.pixelSize: 12
            }

            Text {
                id: text9
                width: 91
                height: 28
                text: qsTr("Latitude")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
            }

            Text {
                id: simLat
                width: 91
                height: 28
                font.pixelSize: 12
            }

            Text {
                id: text5
                width: 91
                height: 28
                text: qsTr("Altitude")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
            }

            Text {
                id: simAlt
                width: 91
                height: 28
                font.pixelSize: 12
            }

            Text {
                id: text10
                width: 91
                height: 28
                text: qsTr("Longitude")
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
            }

            Text {
                id: simLong
                width: 91
                height: 28
                font.pixelSize: 12
            }
        }

        ColumnLayout {
            id: columnLayout

            Label {
                id: label
                text: qsTr("Serial To Relay")
            }

            TextArea {
                id: tBoxSerialToRelay
                text: qsTr("No Data")
                font.pointSize: 10
                verticalAlignment: Text.AlignVCenter
                Layout.preferredHeight: 26
                Layout.preferredWidth: 364
            }

            Label {
                id: label1
                text: qsTr("RelayLo, Spd, Dist, SteerAngle, UTurn")
            }

            TextArea {
                id: tBoxSerialToAutoSteer
                text: qsTr("No Data")
                font.pointSize: 10
                textFormat: Text.AutoText
                verticalAlignment: Text.AlignVCenter
                Layout.preferredHeight: 26
                Layout.preferredWidth: 364
            }

            Label {
                id: label2
                text: qsTr("Serial From Relay")
            }

            TextArea {
                id: tBoxSerialFromRelay
                text: qsTr("No Data")
                font.pointSize: 10
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                Layout.preferredHeight: 24
                Layout.preferredWidth: 364
            }

            Label {
                id: label3
                text: qsTr("Steer Actual, Set Point, Heading, Roll, Sw")
            }

            TextArea {
                id: tBoxSerialFromAutoSteer
                text: "No Data"
                font.pointSize: 10
                verticalAlignment: Text.AlignVCenter
                Layout.preferredHeight: 24
                Layout.preferredWidth: 364
            }

            TextField {
                id: textField
                text: qsTr("No Data")
                Layout.preferredHeight: 79
                Layout.preferredWidth: 364
            }
        }
    }

}
