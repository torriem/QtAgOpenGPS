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
				id: tabArea
				anchors.top: buttonsTop.bottom
				anchors.right: parent.right
				anchors.left: parent.left
				anchors.bottom: angleInfo.top

				//region WAStab
				Item{
                    id: steerTab
                    anchors.fill: parent
                    visible: steerBtn.checked
                    ColumnLayout{
						id: steerColumn
						anchors.bottom: parent.bottom
                        anchors.bottomMargin: 5 * theme.scaleHeight
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.leftMargin: 20 * theme.scaleWidth
                        anchors.rightMargin: 20 * theme.scaleWidth
                        anchors.top: parent.top
                        anchors.topMargin: 5 * theme.scaleHeight
                        width: parent.width *.5
                        IconButtonTransparent{ //was zero button
							width: height*2
                            Layout.alignment: Qt.AlignCenter
                            icon.source: prefix + "/images/SteerCenter.png"
                            implicitHeight: parent.height /5 -20
                            visible: false

						}

						SliderCustomized {
                            property int wasOffset: settings.setAS_wasOffset
                            id: wasZeroSlider
                            Layout.alignment: Qt.AlignCenter
                            centerTopText: "WAS Zero"
                            implicitHeight: 50 * theme.scaleHeight
                            width: 200 * theme.scaleWidth
                            from: -4000
                            leftText: utils.decimalRound(value / cpDegSlider.value, 2)
                            onValueChanged: settings.setAS_wasOffset = value * cpDegSlider.value
                            to: 4000
                            value: settings.setAS_wasOffset / cpDegSlider.value
                        }
						SliderCustomized {
							id: cpDegSlider
                            Layout.alignment: Qt.AlignCenter
                            centerTopText: "Counts per Degree"
                            from: 1
                            implicitHeight: 50 * theme.scaleHeight
                            leftText: value
                            onValueChanged: settings.setAS_countsPerDegree = value
                            stepSize: 1
                            to: 255
                            value: Math.round(settings.setAS_countsPerDegree, 0)
                            width: 200 * theme.scaleWidth
                        }
						SliderCustomized {
							id: ackermannSlider
                            Layout.alignment: Qt.AlignCenter
                            centerTopText: "AckerMann"
                            from: 1
                            implicitHeight: 50 * theme.scaleHeight
                            leftText: value
                            onValueChanged: settings.setAS_ackerman = value
                            stepSize: 1
                            to: 200
                            value: Math.round(settings.setAS_ackerman, 0)
                            width: 200 * theme.scaleWidth
                        }
						SliderCustomized {
							id: maxSteerSlider
                            Layout.alignment: Qt.AlignCenter
                            centerTopText:"Max Steer Angle"
                            from: 10
                            implicitHeight: 50 * theme.scaleHeight
                            implicitWidth: 200 * theme.scaleWidth
                            leftText: value
                            onValueChanged: settings.setVehicle_maxSteerAngle= value
                            stepSize: 1
                            to: 80
                            value: Math.round(settings.setVehicle_maxSteerAngle)
                        }
					}
					Image {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        height: parent.height
                        source: prefix + "/images/Steer/Sf_SteerTab.png"
                        width: parent.width
                    }

				}
				//endregion WAStab

				//region PWMtab
				Item{
					id: gainTab
					anchors.fill: parent
                    visible: gainBtn.checked
                    Column{
						id: gainColumn
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 5 * theme.scaleHeight
                        anchors.left: parent.left
                        anchors.rightMargin: 5 * theme.scaleWidth
                        anchors.topMargin: 5 * theme.scaleHeight
                        height: childrenRect.height
                        spacing: 45 * theme.scaleHeight
                        width: childrenRect.width
                        anchors.leftMargin: 20 * theme.scaleWidth
                        SliderCustomized {
							id: propGainlider
                            centerTopText: "Proportional Gain"
                            from: 0
                            leftText: value
                            onValueChanged: settings.setAS_Kp = value
                            stepSize: 1
                            to: 200
                            value: Math.round(settings.setAS_Kp, 0)
                            implicitHeight: 50 * theme.scaleHeight
                            width: 200 * theme.scaleWidth
                        }
						SliderCustomized {
							id: maxLimitSlider
                            centerTopText: "Maximum Limit"
                            from: 0
                            leftText: value
                            onValueChanged: settings.setAS_highSteerPWM = value
                            stepSize: 1
                            to: 254
                            value: Math.round(settings.setAS_highSteerPWM, 0)
                            implicitHeight: 50 * theme.scaleHeight
                            width: 200 * theme.scaleWidth
                        }
						SliderCustomized {
							id: min2moveSlider
                            centerTopText: "Minimum to Move"
                            from: 0
                            leftText: value
                            onValueChanged: settings.setAS_minSteerPWM = value
                            stepSize: 1
                            to: 100
                            value: Math.round(settings.setAS_minSteerPWM, 0)
                            implicitHeight: 50 * theme.scaleHeight
                            width: 200 * theme.scaleWidth
                        }
					}
					Image {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        source: prefix + "/images/Steer/Sf_GainTab.png"
                        width: parent.width
                        height: gainColumn.height
                    }

				}
				//endregion PWMtab

				//region StanleyTab
				Item{
					id: stanleyTab
					anchors.fill: parent
					visible: stanleyBtn.checked
					Column{
						id: stanleyColumn
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 5 * theme.scaleHeight
                        anchors.left: parent.left
                        anchors.leftMargin: 20 * theme.scaleWidth
                        anchors.rightMargin: 5 * theme.scaleWidth
                        anchors.topMargin: 5 * theme.scaleHeight
                        height: childrenRect.height
                        spacing: 45 * theme.scaleHeight
                        width: childrenRect.width
                        SliderCustomized {
							id: stanleyAggressivenessSlider
                            centerTopText: "Agressiveness"
                            from: .1
                            onValueChanged: settings.stanleyDistanceErrorGain = value
                            stepSize: .1
                            to: 4
                            width: 200 * theme.scaleWidth
                            leftText: Math.round(value * 100)/100
                            value: settings.stanleyDistanceErrorGain
                        }
						SliderCustomized {
							id: overShootReductionSlider
                            centerTopText: "OverShoot Reduction"
                            from: .1
                            onValueChanged: settings.stanleyHeadingErrorGain = value
                            stepSize: .1
                            to: 1.5
                            width: 200 * theme.scaleWidth
                            leftText: Math.round(value * 100) / 100
                            value: settings.stanleyHeadingErrorGain
                        }
						SliderCustomized {
							id: integralStanleySlider
                            centerTopText: "Integral"
                            from: 0
                            leftText: value
                            onValueChanged: settings.stanleyIntegralGainAB = value /100
                            stepSize: 1
                            to: 100
                            value: Math.round(settings.stanleyIntegralGainAB * 100, 0)
                            width: 200 * theme.scaleWidth
                        }
					}
					Image {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        height: parent.height
                        source: prefix + "/images/Steer/Sf_Stanley.png"
                        width: parent.width
                    }

				}
				//endregion StanleyTab
				//
				//region PurePursuitTab
				Item{
					id: ppTab
                    anchors.fill: parent
                    visible: ppBtn.checked
                    Column{
						id: ppColumn
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 5 * theme.scaleHeight
                        anchors.left: parent.left
                        anchors.leftMargin: 20 * theme.scaleWidth
                        anchors.rightMargin: 5 * theme.scaleWidth
                        anchors.topMargin: 5 * theme.scaleHeight
                        height: childrenRect.height
                        spacing: 45 * theme.scaleHeight
                        width: childrenRect.width
                        SliderCustomized {
							id: acqLookAheadSlider
                            centerTopText: "Acquire Look Ahead"
                            from: 1
                            implicitHeight: 50 * theme.scaleHeight
                            onValueChanged: settings.setVehicle_goalPointLookAhead = value
                            stepSize: .1
                            leftText: Math.round(value * 100) / 100
                            to: 7
                            value: settings.setVehicle_goalPointLookAhead
                        }
						SliderCustomized {
							id: holdLookAheadSlider
                            centerTopText: "Hold Look Ahead"
                            from: 1
                            implicitHeight: 50 * theme.scaleHeight
                            stepSize: .1
                            width: 200
                            leftText: Math.round(value * 100) / 100
                            onValueChanged: settings.setVehicle_goalPointLookAheadHold = utils.decimalRound(value, 1)
                            to: 7
                            value: settings.setVehicle_goalPointLookAheadHold
                        }
						SliderCustomized {
							id: lookAheadSpeedGainSlider
                            centerTopText: "Look Ahead Speed Gain"
                            from: .5
                            implicitHeight: 50 * theme.scaleHeight
                            onValueChanged: settings.setVehicle_goalPointLookAheadMult = value
                            stepSize: .1
                            to: 3
                            width: 200 * theme.scaleWidth
                            leftText: Math.round(value * 100) / 100
                            value: settings.setVehicle_goalPointLookAheadMult
                        }
						SliderCustomized {
							id: ppIntegralSlider
                            centerTopText: "Integral"
                            from: 0
                            implicitHeight: 50 * theme.scaleHeight
                            onValueChanged: settings.purePursuitIntegralGainAB = value /100
                            stepSize: 1
                            to: 100
                            width: 200 * theme.scaleWidth
                            leftText: Math.round(value *100) / 100
                            value: settings.purePursuitIntegralGainAB *100
                        }
					}
					Image {
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        height: ppColumn.height
                        source: prefix + "/images/Steer/Sf_PP.png"
                        width: parent.width
                    }
				}
				//endregion PurePursuitTab
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
					onClicked: {
						steerConfigSettings.show()
						steerConfigWindow.visible = false
					}

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
