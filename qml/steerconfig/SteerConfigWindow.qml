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
	modal: false
	visible: false
	height: 475 * theme.scaleHeight
	width:400 * theme.scaleWidth
	x: settings.setWindow_steerSettingsLocation.x
	y: settings.setWindow_steerSettingsLocation.y
	function show (){
		steerConfigWindow.visible = true
	}

	Rectangle{
		id: steerConfigFirst
		color: aog.backgroundColor
		border.color: aog.blackDayWhiteNight
		border.width: 1
		visible: true
		anchors.fill: parent
		Rectangle{
			id:topLine
			color:"white"
			visible: true
			width: parent.width
			height: 25 * theme.scaleHeight
			anchors.top: parent.top
			anchors.horizontalCenter: parent.horizontalCenter
			Text{
				anchors.left: parent.left
				anchors.verticalCenter: parent.verticalCenter
				text: qsTr("Auto Steer Config")
			}

			Button {
				id: close
				text: qsTr("×")
				font.pixelSize: parent.height
				width: parent.height+3
				height:parent.height
				anchors.right: parent.right
				onClicked: {
					if(!pwmWindow.visible){
						settings.setWindow_steerSettingsLocation.x = steerConfigWindow.x
						settings.setWindow_steerSettingsLocation.y = steerConfigWindow.y
					}
					steerConfigWindow.visible = false
				}
			}
			Button {
				id: help
				text: qsTr("?")
				width: parent.height+3
				height:parent.height
				anchors.right: close.left
				anchors.rightMargin: 20 * theme.scaleWidth
			}
		}
		Item{
			id: steerSlidersConfig
			height: 475 * theme.scaleHeight
			width:400 * theme.scaleWidth
			anchors.left: parent.left
			anchors.top: topLine.bottom
			ButtonGroup {
				buttons: buttonsTop.children
			}

			RowLayout{
				id: buttonsTop
				width: parent.width
				IconButtonColor{
					id: steerBtn
					icon.source: prefix + "/images/Steer/ST_SteerTab.png"
					implicitWidth: parent.width /4 - 4
					implicitHeight: 50 * theme.scaleHeight
					checkable: true
					checked: true
					colorChecked: "lightgray"
				}
				IconButtonColor{
					id: gainBtn
					icon.source: prefix + "/images/Steer/ST_GainTab.png"
					implicitWidth: parent.width /4 - 4
					implicitHeight: 50 * theme.scaleHeight
					checkable: true
					colorChecked: "lightgray"
				}
				IconButtonColor{
					id: stanleyBtn
					icon.source: prefix + "/images/Steer/ST_StanleyTab.png"
					implicitWidth: parent.width /4 - 4
					implicitHeight: 50 * theme.scaleHeight
					checkable: true
					colorChecked: "lightgray"
				}
				IconButtonColor{
					id: ppBtn
					icon.source: prefix + "/images/Steer/Sf_PPTab.png"
					implicitWidth: parent.width /4 - 4
					implicitHeight: 50 * theme.scaleHeight
					checkable: true
					colorChecked: "lightgray"
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
					visible: steerBtn.checked
					id: steerTab
					anchors.fill: parent
					ColumnLayout{
						id: steerColumn
						anchors.bottom: parent.bottom
						anchors.horizontalCenter: parent.horizontalCenter
						anchors.top: parent.top
						width: parent.width *.5
						anchors.topMargin: 5 * theme.scaleHeight
						anchors.bottomMargin: 5 * theme.scaleHeight
						anchors.rightMargin: 20 * theme.scaleWidth
						anchors.leftMargin: 20 * theme.scaleWidth
                        IconButtonTransparent{ //was zero button
							width: height*2
							icon.source: prefix + "/images/SteerCenter.png"
							implicitHeight: parent.height /5 -20
							Layout.alignment: Qt.AlignCenter
                            visible: false

						}

						SliderCustomized {
							id: wasZeroSlider
							width: 200 * theme.scaleWidth
                            leftText: utils.decimalRound(value / cpDegSlider.value, 2)
                            from: -4000
                            to: 4000

                            property int wasOffset: settings.setAS_wasOffset

                            //onWasOffsetChanged: value = wasOffset / cpDegSlider.value

                            value: settings.setAS_wasOffset / cpDegSlider.value

                            onValueChanged: settings.setAS_wasOffset = value * cpDegSlider.value
							centerTopText: "WAS Zero"
							Layout.alignment: Qt.AlignCenter
							implicitHeight: 50 * theme.scaleHeight
						}
						SliderCustomized {
							id: cpDegSlider
							width: 200 * theme.scaleWidth
							centerTopText: "Counts per Degree"
							implicitHeight: 50 * theme.scaleHeight
							from: 1
							to: 255
							value: Math.round(settings.setAS_countsPerDegree, 0)
							onValueChanged: settings.setAS_countsPerDegree = value
							leftText: value
							stepSize: 1
							Layout.alignment: Qt.AlignCenter
						}
						SliderCustomized {
							id: ackermannSlider
							width: 200 * theme.scaleWidth
							centerTopText: "AckerMann"
							implicitHeight: 50 * theme.scaleHeight
							from: 1
							to: 200
							leftText: value
							value: Math.round(settings.setAS_ackerman, 0)
							onValueChanged: settings.setAS_ackerman = value
							stepSize: 1
							Layout.alignment: Qt.AlignCenter
						}
						SliderCustomized {
							id: maxSteerSlider
							implicitWidth: 200 * theme.scaleWidth
							leftText: value
							implicitHeight: 50 * theme.scaleHeight
							centerTopText:"Max Steer Angle"
							from: 10
							to: 80
							value: Math.round(settings.setVehicle_maxSteerAngle)
							onValueChanged: settings.setVehicle_maxSteerAngle= value
							stepSize: 1
							Layout.alignment: Qt.AlignCenter
						}
					}
					Image {
						width: parent.width
						anchors.left: parent.left
						anchors.bottom: parent.bottom
						height: parent.height
						source: prefix + "/images/Steer/Sf_SteerTab.png"
					}

				}
				//endregion WAStab

				//region PWMtab
				Item{
					id: gainTab
					visible: gainBtn.checked
					anchors.fill: parent
					Column{
						id: gainColumn
						anchors.bottom: parent.bottom
						anchors.left: parent.left
						height: childrenRect.height
						width: childrenRect.width
						anchors.topMargin: 5 * theme.scaleHeight
						anchors.bottomMargin: 5 * theme.scaleHeight
						anchors.rightMargin: 5 * theme.scaleWidth
						spacing: 45 * theme.scaleHeight
						anchors.leftMargin: 20
						SliderCustomized {
							id: propGainlider
							leftText: value
							width: 200 * theme.scaleWidth
							from: 0
							to: 200
							value: Math.round(settings.setAS_Kp, 0)
							onValueChanged: settings.setAS_Kp = value
							centerTopText: "Proportional Gain"
							stepSize: 1
						}
						SliderCustomized {
							id: maxLimitSlider
							centerTopText: "Maximum Limit"
							leftText: value
							width: 200 * theme.scaleWidth
							from: 0
							to: 254
							value: Math.round(settings.setAS_highSteerPWM, 0)
							onValueChanged: settings.setAS_highSteerPWM = value
							stepSize: 1
						}
						SliderCustomized {
							id: min2moveSlider
							width: 200 * theme.scaleWidth
							from: 0
							to: 100
							value: Math.round(settings.setAS_minSteerPWM, 0)
							onValueChanged: settings.setAS_minSteerPWM = value
							leftText: value
							centerTopText: "Minimum to Move"
							stepSize: 1
						}
					}
					Image {
						width: parent.width
						anchors.left: parent.left
						anchors.bottom: parent.bottom
						height: parent.height
						source: prefix + "/images/Steer/Sf_GainTab.png"
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
						anchors.left: parent.left
						height: childrenRect.height
						width: childrenRect.width
						anchors.topMargin: 5 * theme.scaleHeight
						anchors.bottomMargin: 5 * theme.scaleHeight
						anchors.rightMargin: 5 * theme.scaleWidth
						spacing: 45 * theme.scaleHeight
						anchors.leftMargin: 20 * theme.scaleWidth
						SliderCustomized {
							id: stanleyAggressivenessSlider
							width: 200 * theme.scaleWidth
							from: .1
							to: 4
                            value: settings.stanleyDistanceErrorGain
							onValueChanged: settings.stanleyDistanceErrorGain = value
                            leftText: Math.round(value * 100)/100
							centerTopText: "Agressiveness"
							stepSize: .1
						}
						SliderCustomized {
							id: overShootReductionSlider
							width: 200 * theme.scaleWidth
							from: .1
							to: 1.5
                            value: settings.stanleyHeadingErrorGain
							onValueChanged: settings.stanleyHeadingErrorGain = value
                            leftText: Math.round(value * 100) / 100
							centerTopText: "OverShoot Reduction"
							stepSize: .1
						}
						SliderCustomized {
							id: integralStanleySlider
							width: 200 * theme.scaleWidth
							from: 0
							to: 100
							value: Math.round(settings.stanleyIntegralGainAB * 100, 0)
							onValueChanged: settings.stanleyIntegralGainAB = value /100
							leftText: value
							centerTopText: "Integral"
							stepSize: 1
						}
					}
					Image {
						width: parent.width
						anchors.left: parent.left
						anchors.bottom: parent.bottom
						height: parent.height
						source: prefix + "/images/Steer/Sf_Stanley.png"
					}

				}
				//endregion StanleyTab
				//
				//region PurePursuitTab
				Item{
					anchors.fill: parent
					visible: ppBtn.checked
					id: ppTab
					Column{
						id: ppColumn
						anchors.bottom: parent.bottom
						anchors.left: parent.left
						height: childrenRect.height
						width: childrenRect.width
						anchors.topMargin: 5 * theme.scaleHeight
						anchors.bottomMargin: 5 * theme.scaleHeight
						anchors.rightMargin: 5 * theme.scaleWidth
						spacing: 45 * theme.scaleHeight
						anchors.leftMargin: 20 * theme.scaleWidth
						SliderCustomized {
							id: acqLookAheadSlider
							implicitHeight: 50 * theme.scaleHeight
							from: 1
                            to: 7
                            value: settings.setVehicle_goalPointLookAhead
							onValueChanged: settings.setVehicle_goalPointLookAhead = value
                            leftText: Math.round(value * 100) / 100
							centerTopText: "Acquire Look Ahead"
							stepSize: .1
						}
						SliderCustomized {
							id: holdLookAheadSlider
							width: 200
							implicitHeight: 50 * theme.scaleHeight
							from: 1
                            to: 7
                            value: settings.setVehicle_goalPointLookAheadHold
                            onValueChanged: settings.setVehicle_goalPointLookAheadHold = utils.decimalRound(value, 1)
                            leftText: Math.round(value * 100) / 100
							centerTopText: "Hold Look Ahead"
							stepSize: .1
						}
						SliderCustomized {
							id: lookAheadSpeedGainSlider
							width: 200 * theme.scaleWidth
							implicitHeight: 50 * theme.scaleHeight
							from: .5
							to: 3
                            value: settings.setVehicle_goalPointLookAheadMult
							onValueChanged: settings.setVehicle_goalPointLookAheadMult = value
                            leftText: Math.round(value * 100) / 100
							centerTopText: "Look Ahead Speed Gain"
							stepSize: .1
						}
						SliderCustomized {
							id: ppIntegralSlider
							width: 200 * theme.scaleWidth
							from: 0
							implicitHeight: 50 * theme.scaleHeight
							to: 100
                            value: settings.purePursuitIntegralGainAB *100
							onValueChanged: settings.purePursuitIntegralGainAB = value /100
                            leftText: Math.round(value *100) / 100
							centerTopText: "Integral"
							stepSize: 1
						}
					}
					Image {
						width: parent.width
						anchors.left: parent.left
						anchors.bottom: parent.bottom
						height: ppColumn.height
						source: prefix + "/images/Steer/Sf_PP.png"
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
						id: errorlbl
						property double err: aog.steerAngleActualRounded - aog.steerAngleSetRounded
						text: qsTr("Err: " + err)
						onErrChanged: err > 0 ? errorlbl.color = "red" : errorlbl.color = "darkgreen"
						Layout.alignment: Qt.AlignCenter
					}
					IconButtonTransparent{
						icon.source: prefix + "/images/ArrowRight.png"
						implicitWidth: parent.width/4
						implicitHeight: parent.height
						onClicked: angleInfoMouse.clicked(true)
						Layout.alignment: Qt.AlignRight
					}
				}
			}
		}
		Rectangle{
			id: pwmWindow
			visible: false
			anchors.top: steerSlidersConfig.bottom
			anchors.left: steerSlidersConfig.left
			width: steerSlidersConfig.width
			anchors.bottom: parent.bottom
			anchors.topMargin: 8 * theme.scaleHeight
			anchors.bottomMargin: 8 * theme.scaleHeight
			anchors.leftMargin: 8 * theme.scaleWidth
			anchors.rightMargin: 8 * theme.scaleWidth
			RowLayout{
				id: pwmRow
				anchors.top: parent.top
				anchors.topMargin: 10 * theme.scaleHeight
				anchors.bottomMargin: 10 * theme.scaleHeight
				anchors.leftMargin: 10 * theme.scaleWidth
				anchors.rightMargin: 10 * theme.scaleWidth
				anchors.left: parent.left
				width: parent.width
				height: 50 * theme.scaleHeight
				IconButton{
					id: pwmSteer
					isChecked: false
					icon.source: prefix + "/images/SteerDriveOff.png"
					iconChecked: prefix + "/images/SteerDriveOn.png"
					implicitWidth: parent.width/4
					implicitHeight: parent.height
					color3: "white"
					border: 2
				}
				IconButton{
					icon.source: prefix + "/images/SnapLeft.png"
					implicitWidth: parent.width/4
					implicitHeight: parent.height
					color3: "white"
					border: 2
				}
				IconButton{
					icon.source: prefix + "/images/SnapRight.png"
					implicitWidth: parent.width/4
					implicitHeight: parent.height
					color3: "white"
					border: 2
				}
				IconButton{
					id: pwmZero
					icon.source: prefix + "/images/SteerZeroSmall.png"
					implicitWidth: parent.width/4
					implicitHeight: parent.height
					color3: "white"
					border: 2
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
				text: qsTr("0r +5")
				font.pixelSize: 15
			}
			IconButton{
				id: steerRecord
				anchors.left: parent.left
				anchors.bottom: parent.bottom
				isChecked: false
				width: 75 * theme.scaleWidth
				height: 75 * theme.scaleHeight
				color3: "white"
				border: 2
				icon.source: prefix + "/images/BoundaryRecord.png"
				iconChecked: prefix + "/images/Stop"
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
