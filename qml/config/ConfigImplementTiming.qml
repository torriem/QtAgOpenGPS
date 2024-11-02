import QtQuick
import QtQuick.Controls.Fusion

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
        anchors.bottomMargin: 70 * theme.scaleHeight
        anchors.horizontalCenter: offPic.horizontalCenter
    }
        AnimatedImage{
            id: onPic
            width: parent.width /3 - (10 * theme.scaleWidth)
			anchors.topMargin: 3 * theme.scaleHeight
			anchors.bottomMargin: 3 * theme.scaleHeight
			anchors.leftMargin: 3 * theme.scaleWidth
			anchors.rightMargin: 3 * theme.scaleWidth
            height: parent.height/2 + (50 * theme.scaleHeight)
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            cache: true
            source: prefix + "/images/Config/SectionOnLookAhead.gif"
        }
        AnimatedImage{
            id: offPic
            width: parent.width /3 - (10 * theme.scaleWidth)
			anchors.topMargin: 3 * theme.scaleHeight
			anchors.bottomMargin: 3 * theme.scaleHeight
			anchors.leftMargin: 3 * theme.scaleWidth
			anchors.rightMargin: 3 * theme.scaleWidth
            height: parent.height/2 + (50 * theme.scaleHeight)
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            cache: true
            source: prefix + "/images/Config/SectionLookAheadOff.gif"
        }
        AnimatedImage{
            id: offDelayPic
            width: parent.width /3 - (10 * theme.scaleWidth)
			anchors.topMargin: 3 * theme.scaleHeight
			anchors.bottomMargin: 3 * theme.scaleHeight
			anchors.leftMargin: 3 * theme.scaleWidth
			anchors.rightMargin: 3 * theme.scaleWidth
            height: parent.height/2 + (50 * theme.scaleHeight)
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            cache: true
            source: prefix + "/images/Config/SectionLookAheadDelay.gif"
        }

        SpinBoxCustomized{
            decimals: 1
            anchors.horizontalCenter: onPic.horizontalCenter
            anchors.top: onPic.bottom
            anchors.topMargin: 10
            from: 0.2
            value: settings.setVehicle_toolLookAheadOn
            onValueChanged: settings.setVehicle_toolLookAheadOn = value
            to: 22
            text: qsTr("On (secs)")
        }
        SpinBoxCustomized{
            decimals: 1
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
        SpinBoxCustomized{
            decimals: 1
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
