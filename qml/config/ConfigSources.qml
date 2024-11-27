// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// GPS heading source (dual/single)
import QtQuick
import QtQuick.Controls.Fusion

import ".."
import "../components"

Rectangle{
    id: configSources
    anchors.fill: parent
    visible: true
    color: aog.backgroundColor
    TitleFrame {
        id:antennaType
        width: 360 * theme.scaleWidth
        anchors.top: parent.top
		anchors.left: parent.left
		anchors.bottom: parent.verticalCenter
		anchors.topMargin: 5 * theme.scaleHeight
		anchors.leftMargin: 5 * theme.scaleWidth
		anchors.rightMargin: 5 * theme.scaleWidth
		anchors.bottomMargin: 5 * theme.scaleHeight
        color: aog.backgroundColor
        border.color: aog.blackDayWhiteNight
        visible: true
        title: qsTr("Antenna Type", "GPS Antenna type, fixed or dual")
        font.pointSize: 16 

        Row{
            id: sourceRow
            anchors.top: parent.top
            anchors.topMargin: 10 * theme.scaleHeight
			anchors.leftMargin: 10 * theme.scaleWidth
			anchors.rightMargin: 10 * theme.scaleWidth
			anchors.bottomMargin: 10 * theme.scaleHeight
            anchors.horizontalCenter: parent.horizontalCenter
            height: childrenRect.height  + 25 * (theme.scaleHeight)
            spacing: 10 * theme.scaleWidth

            ButtonGroup {
                buttons: [ dualBtn, fixBtn ]
            }

            IconButtonColor{
                id: dualBtn
                width:150 * theme.scaleWidth
                height:100 * theme.scaleHeight
                text: "Dual"
                checkable: true
                icon.source: prefix + "/images/Config/Con_SourcesGPSDual.png"

                property string headingSource: settings.setGPS_headingFromWhichSource

                checked: (settings.setGPS_headingFromWhichSource === "Dual" ? true: false)

                onCheckedChanged: {
					if (checked){
						settings.setGPS_headingFromWhichSource = "Dual"
						if(settings.setVehicle_antennaOffset < 0)
						    timedMessage.addMessage(7000, qsTr("Antenna Offset error!"), qsTr('You have antenna offset set to "left". Dual requires it set to "right". Change it or you will have offset errors')) 
					}
				}

                onHeadingSourceChanged: {
                    if(headingSource === "Dual"){
                        dualBtn.checked = true
                    }else{
                        dualBtn.checked = false
                    }
                }

            }
            IconButtonColor{
                id: fixBtn
                width:150 * theme.scaleWidth
                height:100 * theme.scaleHeight
                text: "Fix"
                checkable: true
                icon.source: prefix + "/images/Config/Con_SourcesGPSSingle.png"

                property string headingSource: settings.setGPS_headingFromWhichSource

                checked: (settings.setGPS_headingFromWhichSource === "Fix" ? true : false)

                onCheckedChanged: {
                    if(checked)
                        settings.setGPS_headingFromWhichSource = "Fix"
                }

                onHeadingSourceChanged: {
                    if(headingSource === "Fix"){
                        fixBtn.checked = true
                    }else{
                        fixBtn.checked = false
                    }
                }
            }
        }
    }

    TitleFrame {
        id:rtkAlarm
        width: parent.width /2
        height: rtkAlarmRow.height + titleHeight + (10 * theme.scaleHeight)
        anchors.bottom: parent.bottom
        anchors.left: parent.left
		anchors.topMargin: 5 * theme.scaleHeight
		anchors.leftMargin: 5 * theme.scaleWidth
		anchors.rightMargin: 5 * theme.scaleWidth
		anchors.bottomMargin: 5 * theme.scaleHeight
        color: aog.backgroundColor
        visible: true
        title: qsTr("RTK Alarm")
        font.pointSize: 16

        Row{
            id: rtkAlarmRow
            anchors.top: parent.top
            anchors.topMargin: 10 * theme.scaleHeight
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10 * theme.scaleWidth
            height: childrenRect.height + (10 * theme.scaleHeight)

            Component.onCompleted: console.debug(height)

            IconButtonColor{
                width:150 * theme.scaleWidth
                height:100 * theme.scaleHeight
                id: alarm
                icon.source: prefix + "/images/Config/Con_SourcesRTKAlarm.png"
                isChecked: settings.setGPS_isRTK
                onClicked: settings.setGPS_isRTK = true
            }
            Rectangle {
                height: 100 * theme.scaleHeight
                width: childrenRect.width

                Text{
                    anchors.verticalCenter: parent.verticalCenter
                    text: "  ->  "
                }
            }
            IconButtonColor{
                width:150 * theme.scaleWidth
                height:100 * theme.scaleHeight
                checkable: true
                id: killAutoSteer
                icon.source: prefix + "/images/AutoSteerOff.png"
                isChecked: settings.setGPS_isRTK_KillAutoSteer
                onClicked: settings.setGPS_isRTK_KillAutoSteer = true
            }
        }
    }
    TitleFrame{
        id:singleAntennaSettings
        enabled: fixBtn.checked
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
        anchors.top: parent.top
		anchors.topMargin: 5 * theme.scaleHeight
		anchors.leftMargin: 5 * theme.scaleWidth
		anchors.rightMargin: 5 * theme.scaleWidth
		anchors.bottomMargin: 5 * theme.scaleHeight
		onEnabledChanged: visible = enabled
        visible: fixBtn.checked

        border.color: enabled ? aog.blackDayWhiteNight : "grey"

        color: aog.backgroundColor

        title: qsTr("Single Antenna Settings")
        font.pointSize: 16

        Row {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 10 * theme.scaleHeight
            anchors.leftMargin: 10 * theme.scaleWidth

            id: minGPSStep
            Label {
                color: fixBtn.checked ? aog.blackDayWhiteNight : "grey"
                text: qsTr("Minimum GPS Step")
                width: singleAntennaSettings.width * 0.5
            }
            ButtonColor{
                id: minGPSStepBtn
                width: 180 * theme.scaleWidth
                height: 50 * theme.scaleHeight
                checkable: true

                color: "light blue"

                onCheckedChanged: {
                    if(checked){
                        settings.setGPS_minimumStepLimit = 0.1
                        settings.setF_minHeadingStepDistance = 1
                    }else{
                        settings.setGPS_minimumStepLimit = 0.05
                        settings.setF_minHeadingStepDistance = 0.5
                    }
                }
                text: settings.setGPS_minimumStepLimit * 100 + " " + qsTr("cm", "centimeter abbreviation")
            }
        }
        Row {
            id: headingDistance
            anchors.top: minGPSStep.bottom
            anchors.left: parent.left
            anchors.topMargin: 10 * theme.scaleHeight
            anchors.leftMargin: 10 * theme.scaleWidth

            Label {
                color: fixBtn.checked ? aog.blackDayWhiteNight : "grey"
                text: qsTr("Heading Distance")
                width: singleAntennaSettings.width * 0.5
            }
            Label {
                id: headingDistanceText
                width: minGPSStepBtn.width
                color: fixBtn.checked ? aog.blackDayWhiteNight : "grey"
                text: settings.setF_minHeadingStepDistance * 100 + " " + qsTr("cm", "centimeter abbreviation")
            }
        }

        Rectangle{//fusion adjuster
            id: fusionRow
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: 360 * theme.scaleWidth
            height: 100 * theme.scaleHeight
            border.color: fixBtn.checked ? aog.blackDayWhiteNight : "grey"
            color: aog.backgroundColor
            SliderCustomized{
                leftText: 100 - value
                rightText: value
                leftTopText: "IMU <"
                colorLeftTopText: "green"
                anchors.centerIn: parent
                rightTopText: "> GPS"
                colorRightTopText: "red"
                centerTopText: "Fusion"
                from: 20
                to: 40

                property int fusionWeight: settings.setIMU_fusionWeight2

                onFusionWeightChanged: {
                    value = fusionWeight * 100
                }

                value: settings.setIMU_fusionWeight2 * 100
                onValueChanged: {
                    settings.setIMU_fusionWeight2 = value / 100
                }
                stepSize: 1
            }
        }

        Text{
            anchors.left: fusionRow.left
            anchors.top: fusionRow.bottom
            anchors.topMargin: 15 * theme.scaleHeight
            color: fixBtn.checked ? aog.blackDayWhiteNight : "grey"
            text: qsTr("Default: 70%")
        }


        ButtonColor{
            text: qsTr("Reverse Detection")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30 * theme.scaleHeight
            anchors.horizontalCenter: parent.horizontalCenter
            color: aog.backgroundColor
            colorChecked: "green"
            isChecked: settings.setIMU_isReverseOn
            onClicked: settings.setIMU_isReverseOn = checked
            width: 250*theme.scaleWidth
        }
    }
    TitleFrame {
        id: dualAntennaSettings
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
        anchors.top: parent.top
		anchors.topMargin: 5 * theme.scaleHeight
		anchors.leftMargin: 5 * theme.scaleWidth
		anchors.rightMargin: 5 * theme.scaleWidth
		anchors.bottomMargin: 5 * theme.scaleHeight
        border.color: aog.blackDayWhiteNight
		visible: dualBtn.checked
		onEnabledChanged: visible = enabled
        color: aog.backgroundColor
        title: qsTr("Dual Antenna Settings")
        font.pointSize: 16
        enabled: dualBtn.checked

        Image {
            id: head
            source: prefix + "/images/Config/Con_SourcesHead.png"
            width: 100 * theme.scaleWidth
			height: 100 * theme.scaleHeight
            anchors.top: parent.top
            anchors.topMargin: 40 * theme.scaleHeight
            anchors.left: parent.left
            anchors.leftMargin: 30 * theme.scaleWidth
        }
        SpinBoxCustomized{
            id: headingOffSet
            anchors.left: head.right
			anchors.verticalCenter: head.verticalCenter
            anchors.leftMargin: 10 * theme.scaleWidth
            decimals: 1
            from: -100
            boundValue: settings.setGPS_dualHeadingOffset
            onValueChanged: settings.setGPS_dualHeadingOffset = value
            to: 100
            editable: true
            text: qsTr("Heading Offset (Degrees)")
        }
        ButtonColor{
            id: dualAsIMU
            objectName: "dualAsIMU"
            text: qsTr("Dual As IMU")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30 * theme.scaleHeight
            anchors.horizontalCenter: parent.horizontalCenter
            checkable: true
            color: aog.backgroundColor
            colorChecked: "green"
            isChecked: settings.setIMU_isDualAsIMU
            onClicked: settings.setIMU_isDualAsIMU = checked
            width: 250*theme.scaleWidth
        }
    }
}
