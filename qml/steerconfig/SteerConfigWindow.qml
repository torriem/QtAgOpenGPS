// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// The window where we set WAS, Stanley, PP, PWM
import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

import ".."
import "../components"

MoveablePopup {
	id: steerConfigWindow
    closePolicy: Popup.NoAutoClose
    height: 475 * theme.scaleHeight
    modal: false
    visible: false
    width:400 * theme.scaleWidth
    x: settings.setWindow_steerSettingsLocation.x
    y: settings.setWindow_steerSettingsLocation.y
    function show (){
		steerConfigWindow.visible = true
	}

	Rectangle{
		id: steerConfigFirst
        anchors.fill: parent
        border.color: aog.blackDayWhiteNight
        border.width: 1
        color: aog.backgroundColor
        visible: true
        TopLine{
			id:topLine
            onBtnCloseClicked:  steerConfigWindow.close()
            titleText: qsTr("Auto Steer Config")
        }
		Item{
			id: steerSlidersConfig
            anchors.left: parent.left
            anchors.top: topLine.bottom
            height: 475 * theme.scaleHeight
            width:400 * theme.scaleWidth
            ButtonGroup {
				buttons: buttonsTop.children
			}

			RowLayout{
                id: buttonsTop
                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - 10
				IconButtonColor{
					id: steerBtn
                    checkable: true
                    checked: true
                    colorChecked: "lightgray"
                    icon.source: prefix + "/images/Steer/ST_SteerTab.png"
                    implicitHeight: 50 * theme.scaleHeight
                    implicitWidth: parent.width /4 - 4
                }
				IconButtonColor{
					id: gainBtn
                    checkable: true
                    colorChecked: "lightgray"
                    icon.source: prefix + "/images/Steer/ST_GainTab.png"
                    implicitHeight: 50 * theme.scaleHeight
                    implicitWidth: parent.width /4 - 4
                }
				IconButtonColor{
					id: stanleyBtn
                    checkable: true
                    colorChecked: "lightgray"
                    icon.source: prefix + "/images/Steer/ST_StanleyTab.png"
                    implicitHeight: 50 * theme.scaleHeight
                    implicitWidth: parent.width /4 - 4
                }
				IconButtonColor{
					id: ppBtn
                    checkable: true
                    colorChecked: "lightgray"
                    icon.source: prefix + "/images/Steer/Sf_PPTab.png"
                    implicitHeight: 50 * theme.scaleHeight
                    implicitWidth: parent.width /4 - 4
                }
			}

            Item{
                id: slidersArea
                anchors.top: buttonsTop.bottom
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.bottom: angleInfo.top

                ColumnLayout{
                    id: slidersColumn
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5 * theme.scaleHeight
                    anchors.left: parent.left
                    anchors.leftMargin: 20 * theme.scaleWidth
                    anchors.top: parent.top
                    anchors.topMargin: 5 * theme.scaleHeight
                    width: parent.width *.5

                    /* Here, we just set which Sliders we want to see, and the
                      ColumnLayout takes care of the rest. No need for
                      4 ColumnLayouts*/

                    //region WAStab
                    IconButtonTransparent { //was zero button
                        width: height*2
                        Layout.alignment: Qt.AlignCenter
                        icon.source: prefix + "/images/SteerCenter.png"
                        implicitHeight: parent.height /5 -20
                        visible: false
                        //visible: steerBtn.checked
                    }

                    SteerConfigSliderCustomized {
                        property int wasOffset: settings.setAS_wasOffset
                        id: wasZeroSlider
                        centerTopText: "WAS Zero"
                        width: 200 * theme.scaleWidth
                        from: -4000
                        leftText: utils.decimalRound(value / cpDegSlider.value, 2)
                        onValueChanged: settings.setAS_wasOffset = value * cpDegSlider.value
                        to: 4000
                        value: settings.setAS_wasOffset / cpDegSlider.value
                        visible: steerBtn.checked
                    }
                    SteerConfigSliderCustomized {
                        id: cpDegSlider
                        centerTopText: "Counts per Degree"
                        from: 1
                        leftText: value
                        onValueChanged: settings.setAS_countsPerDegree = value
                        stepSize: 1
                        to: 255
                        value: Math.round(settings.setAS_countsPerDegree, 0)
                        width: 200 * theme.scaleWidth
                        visible: steerBtn.checked
                    }
                    SteerConfigSliderCustomized {
                        id: ackermannSlider
                        centerTopText: "AckerMann"
                        from: 1
                        leftText: value
                        onValueChanged: settings.setAS_ackerman = value
                        stepSize: 1
                        to: 200
                        value: Math.round(settings.setAS_ackerman, 0)
                        visible: steerBtn.checked
                    }
                    SteerConfigSliderCustomized {
                        id: maxSteerSlider
                        centerTopText:"Max Steer Angle"
                        from: 10
                        leftText: value
                        onValueChanged: settings.setVehicle_maxSteerAngle= value
                        stepSize: 1
                        to: 80
                        value: Math.round(settings.setVehicle_maxSteerAngle)
                        visible: steerBtn.checked
                    }

                    //endregion WAStab

                    //region PWMtab
                    SteerConfigSliderCustomized {
                        id: propGainlider
                        centerTopText: "Proportional Gain"
                        from: 0
                        leftText: value
                        onValueChanged: settings.setAS_Kp = value
                        stepSize: 1
                        to: 200
                        value: Math.round(settings.setAS_Kp, 0)
                        visible: gainBtn.checked
                    }
                    SteerConfigSliderCustomized {
                        id: maxLimitSlider
                        centerTopText: "Maximum Limit"
                        from: 0
                        leftText: value
                        onValueChanged: settings.setAS_highSteerPWM = value
                        stepSize: 1
                        to: 254
                        value: Math.round(settings.setAS_highSteerPWM, 0)
                        visible: gainBtn.checked
                    }
                    SteerConfigSliderCustomized {
                        id: min2moveSlider
                        centerTopText: "Minimum to Move"
                        from: 0
                        leftText: value
                        onValueChanged: settings.setAS_minSteerPWM = value
                        stepSize: 1
                        to: 100
                        value: Math.round(settings.setAS_minSteerPWM, 0)
                        visible: gainBtn.checked
                    }

                    //endregion PWMtab

                    //region StanleyTab
                    SteerConfigSliderCustomized {
                        id: stanleyAggressivenessSlider
                        centerTopText: "Agressiveness"
                        from: .1
                        onValueChanged: settings.stanleyDistanceErrorGain = value
                        stepSize: .1
                        to: 4
                        leftText: Math.round(value * 100)/100
                        value: settings.stanleyDistanceErrorGain
                        visible: stanleyBtn.checked
                    }
                    SteerConfigSliderCustomized {
                        id: overShootReductionSlider
                        centerTopText: "OverShoot Reduction"
                        from: .1
                        onValueChanged: settings.stanleyHeadingErrorGain = value
                        stepSize: .1
                        to: 1.5
                        leftText: Math.round(value * 100) / 100
                        value: settings.stanleyHeadingErrorGain
                        visible: stanleyBtn.checked
                    }
                    SteerConfigSliderCustomized {
                        id: integralStanleySlider
                        centerTopText: "Integral"
                        from: 0
                        leftText: value
                        onValueChanged: settings.stanleyIntegralGainAB = value /100
                        stepSize: 1
                        to: 100
                        value: Math.round(settings.stanleyIntegralGainAB * 100, 0)
                        visible: stanleyBtn.checked
                    }

                    //endregion StanleyTab
                    //
                    //region PurePursuitTab
                    SteerConfigSliderCustomized {
                        id: acqLookAheadSlider
                        centerTopText: "Acquire Look Ahead"
                        from: 1
                        onValueChanged: settings.setVehicle_goalPointLookAhead = value
                        stepSize: .1
                        leftText: Math.round(value * 100) / 100
                        to: 7
                        value: settings.setVehicle_goalPointLookAhead
                        visible: ppBtn.checked
                    }
                    SteerConfigSliderCustomized {
                        id: holdLookAheadSlider
                        centerTopText: "Hold Look Ahead"
                        from: 1
                        stepSize: .1
                        leftText: Math.round(value * 100) / 100
                        onValueChanged: settings.setVehicle_goalPointLookAheadHold = utils.decimalRound(value, 1)
                        to: 7
                        value: settings.setVehicle_goalPointLookAheadHold
                        visible: ppBtn.checked
                    }
                    SteerConfigSliderCustomized {
                        id: lookAheadSpeedGainSlider
                        centerTopText: "Look Ahead Speed Gain"
                        from: .5
                        onValueChanged: settings.setVehicle_goalPointLookAheadMult = value
                        stepSize: .1
                        to: 3
                        leftText: Math.round(value * 100) / 100
                        value: settings.setVehicle_goalPointLookAheadMult
                        visible: ppBtn.checked
                    }
                    SteerConfigSliderCustomized {
                        id: ppIntegralSlider
                        centerTopText: "Integral"
                        from: 0
                        onValueChanged: settings.purePursuitIntegralGainAB = value /100
                        stepSize: 1
                        to: 100
                        leftText: Math.round(value *100) / 100
                        value: settings.purePursuitIntegralGainAB *100
                        visible: ppBtn.checked
                    }
                    //endregion PurePursuitTab
                }
                Image {
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    height: slidersColumn.height
                    source: prefix + (steerBtn.checked === true ? "/images/Steer/Sf_SteerTab.png" :
                                     gainBtn.checked === true ? "/images/Steer/Sf_GainTab.png" :
                                     stanleyBtn.checked === true ? "/images/Steer/Sf_Stanley.png" :
                                    "/images/Steer/Sf_PP.png")
                    width: parent.width
                }
            }

            Rectangle{
                id: angleInfo
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 50 * theme.scaleHeight
                MouseArea{
                    id: angleInfoMouse
                    anchors.fill: parent
                    onClicked: steerConfigSettings.show()

                }
                RowLayout{
                    id: angleInfoRow
                    anchors.fill: parent
                    spacing: 10 * theme.scaleWidth

                    Text {
                        text: qsTr("Set: " + aog.steerAngleSetRounded)
                        Layout.alignment: Qt.AlignCenter
                    }
                    Text {
                        text: qsTr("Act: " + aog.steerAngleActualRounded)
                        Layout.alignment: Qt.AlignCenter
                    }
                    Text {
                        property double err: aog.steerAngleActualRounded - aog.steerAngleSetRounded
                        id: errorlbl
                        Layout.alignment: Qt.AlignCenter
                        onErrChanged: err > 0 ? errorlbl.color = "red" : errorlbl.color = "darkgreen"
                        text: qsTr("Err: " + err)
                    }
                    IconButtonTransparent{
                        //show angle info window
                        Layout.alignment: Qt.AlignRight
                        icon.source: prefix + "/images/ArrowRight.png"
                        implicitHeight: parent.height
                        implicitWidth: parent.width/4
                        onClicked: angleInfoMouse.clicked(true)
                    }
                }
            }
        }
        Rectangle{
            id: pwmWindow
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8 * theme.scaleHeight
            anchors.left: steerSlidersConfig.left
            anchors.leftMargin: 8 * theme.scaleWidth
            anchors.rightMargin: 8 * theme.scaleWidth
            anchors.top: steerSlidersConfig.bottom
            anchors.topMargin: 8 * theme.scaleHeight
            visible: false
            width: steerSlidersConfig.width
            RowLayout{
                id: pwmRow
                anchors.bottomMargin: 10 * theme.scaleHeight
                anchors.left: parent.left
                anchors.leftMargin: 10 * theme.scaleWidth
                anchors.rightMargin: 10 * theme.scaleWidth
                anchors.top: parent.top
                anchors.topMargin: 10 * theme.scaleHeight
                height: 50 * theme.scaleHeight
                width: parent.width
                IconButton{
                    id: pwmSteer
                    border: 2
                    color3: "white"
                    icon.source: prefix + "/images/SteerDriveOff.png"
                    iconChecked: prefix + "/images/SteerDriveOn.png"
                    implicitHeight: parent.height
                    implicitWidth: parent.width/4
                    isChecked: false
                }
                IconButton{
                    border: 2
                    color3: "white"
                    icon.source: prefix + "/images/SnapLeft.png"
                    implicitHeight: parent.height
                    implicitWidth: parent.width/4
                }
                IconButton{
                    border: 2
                    color3: "white"
                    icon.source: prefix + "/images/SnapRight.png"
                    implicitHeight: parent.height
                    implicitWidth: parent.width/4
                }
                IconButton{
                    border: 2
                    color3: "white"
                    icon.source: prefix + "/images/SteerZeroSmall.png"
                    id: pwmZero
                    implicitHeight: parent.height
                    implicitWidth: parent.width/4
                }
            }
            Text{
                anchors.left: pwmRow.left
                anchors.top: pwmRow.bottom
                text: qsTr("PWM: ")
            }
            Text{
                anchors.right: pwmRow.right
                anchors.rightMargin: 50 * theme.scaleWidth
                anchors.top: pwmRow.bottom
                font.pixelSize: 15
                text: qsTr("0r +5")
            }
            IconButton{
                id: steerRecord
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                border: 2
                color3: "white"
                height: 75 * theme.scaleHeight
                icon.source: prefix + "/images/BoundaryRecord.png"
                iconChecked: prefix + "/images/Stop"
                isChecked: false
                width: 75 * theme.scaleWidth
            }
            Text{
                anchors.top: steerRecord.top
                anchors.left: steerRecord.right
                text: qsTr("Steer Angle: ")
            }
            Text{
                anchors.bottom: steerRecord.bottom
                anchors.left: steerRecord.right
                text: qsTr("Diameter: ")
            }
        }
    }
}
