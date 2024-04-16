import QtQuick
import QtQuick.Controls

import ".."
import "../components"

Rectangle{
    id: configImpTimWin
    anchors.fill: parent
    color: aog.backgroundColor
    visible: false
    Text{
        font.pixelSize: 25
        font.bold: true
        text: qsTr("Look Ahead Time Settings")
        anchors.bottom: offPic.top
        anchors.bottomMargin: 70
        anchors.horizontalCenter: offPic.horizontalCenter
    }
        AnimatedImage{
            id: onPic
            width: parent.width /3 -10
            anchors.margins: 5
            anchors.left: parent.left
            height: parent.height/2 + 50
            anchors.verticalCenter: parent.verticalCenter
            cache: true
            source: "/images/Config/SectionOnLookAhead.gif"
        }
        AnimatedImage{
            id: offPic
            width: parent.width /3 -10
            anchors.margins: 5
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height/2 + 50
            anchors.verticalCenter: parent.verticalCenter
            cache: true
            source: "/images/Config/SectionLookAheadOff.gif"
        }
        AnimatedImage{
            id: offDelayPic
            width: parent.width /3 -10
            anchors.margins: 5
            anchors.right: parent.right
            height: parent.height/2 + 50
            anchors.verticalCenter: parent.verticalCenter
            cache: true
            source: "/images/Config/SectionLookAheadDelay.gif"
        }

        SpinBoxOneDecimal{
            anchors.horizontalCenter: onPic.horizontalCenter
            anchors.top: onPic.bottom
            anchors.topMargin: 10
            from: 0.2
            value: settings.setVehicle_toolLookAheadOn
            onValueChanged: settings.setVehicle_toolLookAheadOn = value
            to: 22
            text: qsTr("On (secs)")
        }
        SpinBoxOneDecimal{
            anchors.horizontalCenter: offPic.horizontalCenter
            anchors.top: offPic.bottom
            anchors.topMargin: 10
            from: 0
            value:settings.setVehicle_toolLookAheadOff
            onValueChanged: settings.setVehicle_toolLookAheadOff = value
            to: 20
            editable: true
            text: qsTr("Off (secs)")
        }
        SpinBoxOneDecimal{
            anchors.horizontalCenter: offDelayPic.horizontalCenter
            anchors.top: offDelayPic.bottom
            anchors.topMargin: 10
            from: 0
            value: settings.setVehicle_toolOffDelay
            onValueChanged: settings.setVehicle_toolOffDelay = value
            to: 10
            editable: true
            text: qsTr("Delay (secs)")
        }
    }
