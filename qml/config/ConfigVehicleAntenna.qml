// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Antenna dimensions
import QtQuick
import QtQuick.Controls.Fusion

import ".."
import "../components"

Rectangle{
    id: configTractorAntenna
    anchors.fill: parent
    color: aog.backgroundColor
    visible: false
    Image {
        id: antImage
        //3 vehicle types  tractor=0 harvestor=1 4wd=2
        source: settings.setVehicle_vehicleType === 0 ? prefix + "/images/qtSpecific/AntennaTractor.png" :
                settings.setVehicle_vehicleType === 1 ? prefix + "/images/qtSpecific/AntennaHarvester.png" :
                settings.setVehicle_vehicleType === 2 ? prefix + "/images/qtSpecific/Antenna4WD.png" :
                prefix + "/images/Config/ConSt_Mandatory.png"
        width: 350 * theme.scaleWidth
        height: 175 * theme.scaleHeight
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.top: parent.top
		anchors.topMargin: (30+antennaPivot.height) * theme.scaleHeight
    }
    SpinBoxCM{
        id: antennaPivot
        anchors.bottom: antImage.top
        anchors.left: antImage.left
        anchors.leftMargin: 30
        from: -999
        to: 999
        editable: true
        boundValue: settings.setVehicle_antennaPivot
        onValueModified: settings.setVehicle_antennaPivot = value
    }
    SpinBoxCM{
        id: antennaHeight
        anchors.top: antImage.top
        anchors.topMargin: 100
        anchors.left: antImage.right
        anchors.leftMargin: -50
        from: 0
        to: 1000
        editable: true
        boundValue: settings.setVehicle_antennaHeight
        onValueModified: settings.setVehicle_antennaHeight = value
    }
	TitleFrame{
		anchors.top: antImage.bottom
		anchors.topMargin: 20 * theme.scaleHeight
		title: qsTr("Antenna Offset")
		anchors.horizontalCenter: parent.horizontalCenter
		width: parent.width * 0.65
		anchors.bottom: parent.bottom
		anchors.bottomMargin: 20 * theme.scaleHeight
		border.width: 2
        Item{
            ButtonGroup {
                buttons: [ offsetLeft, offsetRight ]
            }
        }

        IconButtonColor{
            id: offsetLeft
            implicitWidth: 170 * theme.scaleWidth
            implicitHeight: 250 * theme.scaleHeight
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
			anchors.leftMargin: 7 * theme.scaleWidth
			iconHeightScaleText: 1
            checkable: settings.setGPS_headingFromWhichSource === "Single"
            isChecked: (settings.setVehicle_antennaOffset < 0)
			icon.source: settings.setVehicle_vehicleType === 0 ? prefix + "/images/qtSpecific/LeftAntenna.png"
			: settings.setVehicle_vehicleType === 1 ? prefix + "/images/qtSpecific/LeftAntennaHarvester.png"
			: settings.setVehicle_vehicleType === 2 ? prefix + "/images/qtSpecific/LeftAntenna4WD.png"
			: prefix + "/images/Config/ConSt_Mandatory.png"
			onClicked: {
				if (settings.setGPS_headingFromWhichSource === "Dual") 
				    timedMessage.addMessage(5000, qsTr("Not Allowed"), qsTr("Dual heading MUST be right only"))
				else
					settings.setVehicle_antennaOffset = -Math.abs(settings.setVehicle_antennaOffset)
			}

        }

        IconButtonColor{
            id: offsetRight
            implicitWidth: 170 * theme.scaleWidth
            implicitHeight: 250 * theme.scaleHeight
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
			anchors.rightMargin: 7 * theme.scaleWidth
            isChecked: (settings.setVehicle_antennaOffset >= 0)
            checkable: true
            onClicked: settings.setVehicle_antennaOffset = Math.abs(settings.setVehicle_antennaOffset)
            icon.source: settings.setVehicle_vehicleType === 0 ? prefix + "/images/qtSpecific/RightAntenna.png"
					: settings.setVehicle_vehicleType === 1 ? prefix + "/images/qtSpecific/RightAntennaHarvester.png"
					: settings.setVehicle_vehicleType === 2 ? prefix + "/images/qtSpecific/RightAntenna4WD.png"
					: prefix + "/images/Config/ConSt_Mandatory.png"
        }
		SpinBoxCM{
			id: antennaOffset
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.bottom: parent.verticalCenter
			anchors.bottomMargin: 10 * theme.scaleHeight
			from: 0
			to: 500
			editable: true
			boundValue: Math.abs(settings.setVehicle_antennaOffset)
			onValueChanged:{
				if (offsetLeft.checked){
					settings.setVehicle_antennaOffset = -value
				} else {
				settings.setVehicle_antennaOffset = value
				}
				console.log(settings.setVehicle_antennaOffset)
			}
		}
	}
}
