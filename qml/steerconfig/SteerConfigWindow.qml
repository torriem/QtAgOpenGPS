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
	height: 475
	width:400
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
			height: 25
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
				anchors.rightMargin: 20
			}
		}
		Item{
			id: steerSlidersConfig
			height: 475
			width:400
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
					icon.source: "/images/Steer/ST_SteerTab.png"
					implicitWidth: parent.width /4 - 4
					implicitHeight: 50
					checkable: true
					checked: true
					colorChecked: "lightgray"
				}
				IconButtonColor{
					id: gainBtn
					icon.source: "/images/Steer/ST_GainTab.png"
					implicitWidth: parent.width /4 - 4
					implicitHeight: 50
					checkable: true
					colorChecked: "lightgray"
				}
				IconButtonColor{
					id: stanleyBtn
					icon.source: "/images/Steer/ST_StanleyTab.png"
					implicitWidth: parent.width /4 - 4
					implicitHeight: 50
					checkable: true
					colorChecked: "lightgray"
				}
				IconButtonColor{
					id: ppBtn
					icon.source: "/images/Steer/Sf_PPTab.png"
					implicitWidth: parent.width /4 - 4
					implicitHeight: 50
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
						anchors.margins: 5
						anchors.leftMargin: 20
						IconButtonTransparent{
							width: height*2
							icon.source: "/images/SteerCenter.png"
							implicitHeight: parent.height /5 -20
							Layout.alignment: Qt.AlignCenter

						}

						SliderCustomized {
							id: wasZeroSlider
							width: 200
							leftText: value
							from: -30//idk
							to: 30
							value: 5
							centerTopText: "WAS Zero"
							Layout.alignment: Qt.AlignCenter
							implicitHeight: 50
						}
						SliderCustomized {
							id: cpDegSlider
							width: 200
							centerTopText: "Counts per Degree"
							implicitHeight: 50
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
							width: 200
							centerTopText: "AckerMann"
							implicitHeight: 50
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
							implicitWidth: 200
							leftText: value
							implicitHeight: 50
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
						source: "/images/Steer/Sf_SteerTab.png"
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
						anchors.margins: 5
						spacing: 45
						anchors.leftMargin: 20
						SliderCustomized {
							id: propGainlider
							leftText: value
							width: 200
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
							width: 200
							from: 0
							to: 254
							value: Math.round(settings.setAS_highSteerPWM, 0)
							onValueChanged: settings.setAS_highSteerPWM = value
							stepSize: 1
						}
						SliderCustomized {
							id: min2moveSlider
							width: 200
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
						source: "/images/Steer/Sf_GainTab.png"
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
						anchors.margins: 5
						spacing: 45
						anchors.leftMargin: 20
						SliderCustomized {
							id: stanleyAggressivenessSlider
							width: 200
							from: .1
							to: 4
							value: Math.round(settings.stanleyDistanceErrorGain, 0)
							onValueChanged: settings.stanleyDistanceErrorGain = value
							leftText: value
							centerTopText: "Agressiveness"
							stepSize: .1
						}
						SliderCustomized {
							id: overShootReductionSlider
							width: 200
							from: .1
							to: 1.5
							value: Math.round(settings.stanleyHeadingErrorGain, 0)
							onValueChanged: settings.stanleyHeadingErrorGain = value
							leftText: value
							centerTopText: "OverShoot Reduction"
							stepSize: .1
						}
						SliderCustomized {
							id: integralStanleySlider
							width: 200
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
						source: "/images/Steer/Sf_Stanley.png"
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
						anchors.margins: 5
						spacing: 45
						anchors.leftMargin: 20
						SliderCustomized {
							id: acqLookAheadSlider
							implicitHeight: 50
							from: 1
							to: 5
							value: Math.round(settings.setVehicle_goalPointLookAhead, 1)
							onValueChanged: settings.setVehicle_goalPointLookAhead = value
							leftText: value
							centerTopText: "Acquire Look Ahead"
							stepSize: .1
						}
						SliderCustomized {
							id: holdLookAheadSlider
							width: 200
							implicitHeight: 50
							from: 1
							to: 5
							value: Math.round(settings.setVehicle_goalPointLookAheadHold, 1)
							onValueChanged: settings.setVehicle_goalPointLookAheadHold = value
							leftText: value
							centerTopText: "Hold Look Ahead"
							stepSize: .1
						}
						SliderCustomized {
							id: lookAheadSpeedGainSlider
							width: 200
							implicitHeight: 50
							from: .5
							to: 3
							value: Math.round(settings.setVehicle_goalPointLookAheadMult, 1)
							onValueChanged: settings.setVehicle_goalPointLookAheadMult = value
							leftText: value
							centerTopText: "Look Ahead Speed Gain"
							stepSize: .1
						}
						SliderCustomized {
							id: ppIntegralSlider
							width: 200
							from: 0
							implicitHeight: 50
							to: 100
							value: Math.round(settings.purePursuitIntegralGainAB *100, 0)
							onValueChanged: settings.purePursuitIntegralGainAB = value /100
							leftText: value
							centerTopText: "Integral"
							stepSize: 1
						}
					}
					Image {
						width: parent.width
						anchors.left: parent.left
						anchors.bottom: parent.bottom
						height: ppColumn.height
						source: "/images/Steer/Sf_PP.png"
					}
				}
				//endregion PurePursuitTab
			}

			Rectangle{
				id: angleInfo
				anchors.bottom: parent.bottom
				anchors.left: parent.left
				anchors.right: parent.right
				height: 50
				MouseArea{
					anchors.fill: parent
					onClicked: {
						function showHide(){
							if(pwmWindow.visible == false){//show full menu
							steerConfigWindow.height = 768
							steerConfigWindow.width=1024
							pwmWindow.visible = true
							steerConfig.visible = true
							settings.setWindow_steerSettingsLocation.x = steerConfigWindow.x
							settings.setWindow_steerSettingsLocation.y = steerConfigWindow.y
							steerConfigWindow.x = 0
							steerConfigWindow.y = 0
						}else{//hide
						steerConfigWindow.height = 465
						steerConfigWindow.width=400
						pwmWindow.visible = false
						steerConfig.visible = false
						steerConfigWindow.x = settings.setWindow_steerSettingsLocation.x
						steerConfigWindow.y = settings.setWindow_steerSettingsLocation.y
					}
				}
				showHide()
			}
		}

		Text {
			anchors.left: parent.left
			anchors.verticalCenter: parent.verticalCenter
			text: qsTr("Set: " + aog.steerAngleSetRounded)
		}
		Text {
			text: qsTr("Act: " + aog.steerAngleActualRounded)
			anchors.horizontalCenter: parent.horizontalCenter
			anchors.verticalCenter: parent.verticalCenter
		}
		Text {
			id: errorlbl
			property double err: aog.steerAngleActualRounded - aog.steerAngleSetRounded
			text: qsTr("Err: " + err)
			anchors.right: parent.right
			anchors.verticalCenter: parent.verticalCenter
			onErrChanged: err > 0 ? errorlbl.color = "red" : errorlbl.color = "darkgreen"
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
	anchors.margins: 8
	RowLayout{
		id: pwmRow
		anchors.top: parent.top
		anchors.margins: 10
		anchors.left: parent.left
		width: parent.width
		height: 50
		IconButton{
			id: pwmSteer
			isChecked: false
			icon.source: "/images/SteerDriveOff.png"
			iconChecked: "/images/SteerDriveOn.png"
			implicitWidth: parent.width/4.5
			implicitHeight: parent.height
			color3: "white"
			border: 2
		}
		IconButton{
			icon.source: "/images/SnapLeft.png"
			implicitWidth: parent.width/4.5
			implicitHeight: parent.height
			color3: "white"
			border: 2
		}
		IconButton{
			icon.source: "/images/SnapRight.png"
			implicitWidth: parent.width/4.5
			implicitHeight: parent.height
			color3: "white"
			border: 2
		}
		IconButton{
			id: pwmZero
			icon.source: "/images/SteerZeroSmall.png"
			implicitWidth: parent.width/4.5
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
		anchors.rightMargin: 50
		anchors.top: pwmRow.bottom
		text: qsTr("0r +5")
		font.pixelSize: 15
	}
	IconButton{
		id: steerRecord
		anchors.left: parent.left
		anchors.bottom: parent.bottom
		isChecked: false
		width: 75
		height: 75
		color3: "white"
		border: 2
		icon.source: "/images/BoundaryRecord.png"
		iconChecked: "/images/Stop"
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
Rectangle{
	id: steerConfig
	anchors.left: steerSlidersConfig.right
	anchors.top: topLine.bottom
	anchors.right: parent.right
	anchors.bottom: parent.bottom
	anchors.margins: 10
	color: "lightgray"
	visible: false
	ButtonGroup{
		buttons: settingsBtns.children
	}

	RowLayout{
		id: settingsBtns
		spacing: 3
		width: parent.width
		anchors.top: parent.top
		anchors.horizontalCenter: parent.horizontalCenter
		IconButtonTextBeside{
			id: sensorsBtn
			icon.source: "/images/Config/ConD_Speedometer.png"
			buttonText: "Sensors"
			Layout.alignment: Qt.AlignCenter
			checkable: true
			checked: true
			colorChecked: "lightgray"
			implicitWidth: parent.width /3 -4
		}
		IconButtonTextBeside{
			id: configBtn
			icon.source: "/images/Config/ConS_Pins.png"
			buttonText: "Config"
			Layout.alignment: Qt.AlignCenter
			checkable: true
			colorChecked: "lightgray"
			implicitWidth: parent.width /3 -4
		}
		IconButtonTextBeside{
			id: settingsBtn
			icon.source: "/images/Config/ConS_ImplementConfig.png"
			buttonText: "Settings"
			Layout.alignment: Qt.AlignCenter
			checkable: true
			colorChecked: "lightgray"
			implicitWidth: parent.width /3 -4
		}
	}
	Item{
		id: settingsArea
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.top: settingsBtns.bottom
		anchors.bottom: bottomRightButtons.top
		anchors.margins: 10

		onVisibleChanged:{
			console.log("loading settings")
			load_settings()
		}
		function load_settings(){ 
			if (visible) {
				var sett = settings.setArdSteer_setting1

				if ((sett & 1) == 0) chkInvertWAS.checked = false;
				else chkInvertWAS.checked = true;

				if ((sett & 2) == 0) chkSteerInvertRelays.checked = false;
				else chkSteerInvertRelays.checked = true;

				if ((sett & 4) == 0) chkInvertSteer.checked = false;
				else chkInvertSteer.checked = true;

				//if ((sett & 8) == 0) cboxConv.currentText = "Differential";
				//else cboxConv.currentText = "Single";
				if ((sett & 8) == 0) cboxConv.currentIndex = 1;
				else cboxConv.currentIndex = 0;

				//if ((sett & 16) == 0) cboxMotorDrive.currentText = "IBT2";
				//else cboxMotorDrive.currentText = "Cytron";
				if ((sett & 16) == 0) cboxMotorDrive.currentIndex = 1;
				else cboxMotorDrive.currentIndex = 0;

				if ((sett & 32) == 32) cboxSteerEnable.currentIndex = 1;
				else if ((sett & 64) == 64) cboxSteerEnable.currentIndex = 2;
				else cboxSteerEnable.currentIndex = 0;

				if ((sett & 128) == 0){
					console.log("encoder not checked")
					cboxEncoder.checked = false;
				}
				else {
					console.log("encoder set to checked") 
					cboxEncoder.checked = true;
				}

				/*nudMaxCounts.Value = (decimal)Properties.Settings.Default.setArdSteer_maxPulseCounts;
				 hsbarSensor.Value = (int)Properties.Settings.Default.setArdSteer_maxPulseCounts;
				 lblhsbarSensor.Text = ((int)((double)hsbarSensor.Value * 0.3921568627)).ToString() + "%";*/

				 sett = settings.setArdSteer_setting1;

				 if ((sett & 1) == 0) cboxDanfoss.checked = false;
				 else cboxDanfoss.checked = true;

				 if ((sett & 8) == 0) cboxXY.currentIndex = 0;
				 else cboxXY.currentIndex = 1;

				 if ((sett & 2) == 0) cboxPressureSensor.checked = false;
				 else cboxPressureSensor.checked = true;

				 if ((sett & 4) == 0) cboxCurrentSensor.checked = false;
				 else cboxCurrentSensor.checked = true;


				 if (cboxEncoder.checked)
				 {
					 /* label61.Visible = true;
					  lblPercentFS.Visible = true;
					  nudMaxCounts.Visible = true;
					  pbarSensor.Visible = false;
					  hsbarSensor.Visible = false;
					  lblhsbarSensor.Visible = false;
					  label61.Text = gStr.gsEncoderCounts;*/
				  }
				  else if (cboxPressureSensor.checked)
				  {
					  //label61.Visible = true;
					  //lblPercentFS.Visible = true;
					  //nudMaxCounts.visible = false;
					  /*pbarSensor.Visible = true;
					   hsbarSensor.Visible = true;
					   lblhsbarSensor.Visible = true;

					   label61.Text = "Off at %";*/
				   }
				   else if (cboxCurrentSensor.checked)
				   {
					   //label61.Visible = true;
					   //lblPercentFS.Visible = true;
					   //nudMaxCounts.visible = false;
					   /* pbarSensor.Visible = true;
						hsbarSensor.Visible = true;
						lblhsbarSensor.Visible = true;

						label61.Text = "Off at %";*/
					}
					else
					{
						//console.log("nothing checked--what's going on here?")
						/*label61.Visible = false;
						 lblPercentFS.Visible = false;*/
						 //nudMaxCounts.Visible = false;
						 /*pbarSensor.Visible = false;
						  hsbarSensor.Visible = false;
						  lblhsbarSensor.Visible = false;*/
					  }

				  }
			  }

			  function save_settings() {
				  var set = 1;
				  var reset = 2046;
				  var sett = 0;

				  if (chkInvertWAS.checked) sett |= set;
				  else sett &= reset;

				  set <<= 1;
				  reset <<= 1;
				  reset += 1;
				  if (chkSteerInvertRelays.checked) sett |= set;
				  else sett &= reset;

				  set <<= 1;
				  reset <<= 1;
				  reset += 1;
				  if (chkInvertSteer.checked) sett |= set;
				  else sett &= reset;

				  set <<= 1;
				  reset <<= 1;
				  reset += 1;
				  if (cboxConv.currentText == "Single") sett |= set;
				  else sett &= reset;

				  set <<= 1;
				  reset <<= 1;
				  reset += 1;
				  if (cboxMotorDrive.currentText == "Cytron") sett |= set;
				  else sett &= reset;

				  set <<= 1;
				  reset <<= 1;
				  reset += 1;
				  if (cboxSteerEnable.currentText == "Switch") sett |= set;
				  else sett &= reset;

				  set <<= 1;
				  reset <<= 1;
				  reset += 1;
				  if (cboxSteerEnable.currentIndex == 2) sett |= set;
				  else sett &= reset;

				  set <<= 1;
				  reset <<= 1;
				  reset += 1;
				  if (cboxEncoder.checked){
					  console.log("encoder checked")
					  sett |= set;
				  }
				  else {
					  console.log("encoder not checked")
					  sett &= reset;
				  }

				  //set = (set << 1);
				  //reset = (reset << 1);
				  //reset = (reset + 1);
				  //if ( ) sett |= set;
				  //else sett &= reset;

				  settings.setArdSteer_setting0 = sett;
				  settings.setArdMac_isDanfoss = cboxDanfoss.checked;

				  if (cboxCurrentSensor.checked || cboxPressureSensor.checked)
				  {
					  //settings.setArdSteer_maxPulseCounts = hsbarSensor.Value; //todo add
				  }
				  else
				  {
					  //settings.setArdSteer_maxPulseCounts = nudMaxCounts.Value; //todo add
				  }

				  // Settings1
				  set = 1;
				  reset = 2046;
				  sett = 0;

				  if (cboxDanfoss.checked) sett |= set;
				  else sett &= reset;

				  set <<= 1;
				  reset <<= 1;
				  reset += 1;
				  if (cboxPressureSensor.checked) sett |= set;
				  else sett &= reset;

				  //bit 2
				  set <<= 1;
				  reset <<= 1;
				  reset += 1;
				  if (cboxCurrentSensor.checked) sett |= set;
				  else sett &= reset;

				  //bit 3
				  set <<= 1;
				  reset <<= 1;
				  reset += 1;
				  if (cboxXY.currentIndex == 1) sett |= set;
				  else sett &= reset;

				  settings.setArdSteer_setting1 = sett;

				  //Properties.Settings.Default.Save(); not sure what happens here?? David

				  //set pgns
				  //signal handler will do the following:
				  /*
				   mf.p_251.pgn[mf.p_251.set0] = Properties.Settings.Default.setArdSteer_setting0;
				   mf.p_251.pgn[mf.p_251.set1] = Properties.Settings.Default.setArdSteer_setting1;
				   mf.p_251.pgn[mf.p_251.maxPulse] = Properties.Settings.Default.setArdSteer_maxPulseCounts;
				   mf.p_251.pgn[mf.p_251.minSpeed] = 5; //0.5 kmh THIS IS CHANGED IN AOG FIXES

				   if (settings.setAS_isConstantContourOn)
				   mf.p_251.pgn[mf.p_251.angVel] = 1;
				   else mf.p_251.pgn[mf.p_251.angVel] = 0;
				   */
				  aog.modules_send_251()

				  unsaved.visible = false; 
			  }
			  //region sensorsTab
			  Item {
				  visible: sensorsBtn.checked
				  id: sensorWindowItem
				  anchors.fill: parent
				  ButtonGroup{
					  buttons: sensorsWindow.children
				  }

				  RowLayout{
					  id: sensorsWindow
					  width: parent.width
					  IconButtonColor{
						  id: cboxEncoder
						  icon.source: "/images/Config/ConSt_TurnSensor.png"
						  checkable: true
						  buttonText: "Turn Sensor"
						  Layout.alignment: Qt.AlignCenter
						  colorChecked: steerConfig.color
						  onClicked: unsaved.visible = true
					  }
					  IconButtonColor{
						  id: cboxPressureSensor
						  icon.source: "/images/Config/ConSt_TurnSensorPressure.png"
						  checkable: true
						  buttonText: "Pressure Turn Sensor"
						  Layout.alignment: Qt.AlignCenter
						  colorChecked: steerConfig.color
						  onClicked: unsaved.visible = true
					  }
					  IconButtonColor{
						  id: cboxCurrentSensor
						  icon.source: "/images/Config/ConSt_TurnSensorCurrent.png"
						  checkable: true
						  buttonText: "Current Turn Sensor"
						  Layout.alignment: Qt.AlignCenter
						  colorChecked: steerConfig.color
						  onClicked: unsaved.visible = true
					  }
				  }
			  }
			  //endregion sensorsTab
			  //
			  //region configTab
			  Item{
				  anchors.fill: parent
				  visible: configBtn.checked
				  id: configWindow
				  GridLayout{
					  anchors.fill: parent
					  rows: 4
					  columns: 2
					  flow: Grid.TopToBottom
					  IconButtonColor{
						  id: cboxDanfoss
						  icon.source: "/images/Config/ConST_Danfoss.png"
						  checkable: true
						  buttonText: "Danfoss"
						  Layout.alignment: Qt.AlignCenter
						  onClicked: unsaved.visible = true
					  }
					  IconButtonColor{
						  id: chkInvertWAS
						  icon.source: "/images/Config/ConSt_InvertWAS.png"
						  checkable: true
						  Layout.alignment: Qt.AlignCenter
						  buttonText: "Invert WAS"
						  onClicked: unsaved.visible = true
					  }
					  IconButtonColor{
						  id: chkInvertSteer
						  icon.source: "/images/Config/ConSt_InvertDirection.png"
						  checkable: true
						  buttonText: "Invert Motor Dir"
						  Layout.alignment: Qt.AlignCenter
						  onClicked: unsaved.visible = true
					  }
					  IconButtonColor{
						  id: chkSteerInvertRelays
						  icon.source: "/images/Config/ConSt_InvertRelay.png"
						  checkable: true
						  buttonText: "Invert Relays"
						  Layout.alignment: Qt.AlignCenter
						  onClicked: unsaved.visible = true
					  }
				  }
				  ColumnLayout{
					  id: columnOfDropDown
					  anchors.top: parent.top
					  anchors.margins: 10
					  anchors.bottomMargin: 50
					  anchors.verticalCenter: parent.verticalCenter
					  anchors.right: parent.right
					  height: parent.height /2
					  ComboBoxCustomized {
						  id: cboxMotorDrive
						  editable: true
						  // Component.onCompleted: currentIndex = ((settings.setArdSteer_setting0 & 16) == 0) ? 1 : 0
						  model: ListModel {
							  id: modelmotorDriver
							  ListElement {text: "Cytron"}
							  ListElement {text: "IBT2"}
						  }
						  text: ("Motor Driver")
						  onActivated: unsaved.visible = true
					  }
					  ComboBoxCustomized {
						  id: cboxConv
						  editable: true
						  // Component.onCompleted: currentIndex = ((settings.setArdSteer_setting0 & 8) == 0) ? 1 : 0
						  model: ListModel {
							  id: a2Dmodel
							  ListElement {text: "Single"}
							  ListElement {text: "Differential"}
						  }
						  text: qsTr("A2D Converter")
						  onActivated: unsaved.visible = true
					  }
					  ComboBoxCustomized {
						  id: cboxXY
						  editable: true
						  // Component.onCompleted: currentIndex = ((settings.setArdSteer_setting1 & 8) == 0) ? 0 : 1
						  model: ListModel {
							  id: imuAxismodel
							  ListElement {text: "X"}
							  ListElement {text: "Y"}
						  }
						  text: qsTr("IMU X or Y Axis")
						  onActivated: unsaved.visible = true
					  }

					  ComboBoxCustomized {
						  id: cboxSteerEnable
						  /* Component.onCompleted: if((settings.setArdSteer_setting0 & 32) == 32)
						   currentIndex = 1
						   else if((settings.setArdSteer_setting0 & 64) == 64)
						   currentIndex = 2
						   else
						   currentIndex = 0*/
						   model: ListModel {
							   //   id: steerEnablemodel
							   ListElement {text: "None"}
							   ListElement {text: "Switch"}
							   ListElement {text: "Button"}
						   }
						   text: qsTr("Steer Enable")
						   onActivated: unsaved.visible = true
						   Text{
							   anchors.top: cboxSteerEnable.bottom
							   anchors.left: cboxSteerEnable.left
							   text: qsTr("Button- Push On, Push Off
							   Switch- Pushed On, Release Off")
							   font.pixelSize: 10
						   }
					   }
				   }
			   }
			   //endregion configTab
			   //
			   //region settingsTab
			   Item{
				   id: settingsWindow
				   visible: settingsBtn.checked
				   anchors.centerIn: parent
				   SpinBoxDecimal{
					   text: "Panic Stop"
					   implicitWidth: 100
					   implicitHeight: 65
					   from: 0
					   value: 0
					   to: 10
					   editable: true
					   onValueChanged: unsaved.visible = true
				   }
			   }
			   //endregion settingsTab
		   }
		   RowLayout{
			   id: bottomRightButtons
			   anchors.right: parent.right
			   anchors.rightMargin: unsaved.width + 20
			   anchors.left: parent.left
			   anchors.bottom: parent.bottom
			   height: 70
			   IconButtonText{
				   id: wizard
				   text: qsTr("Wizard")
				   icon.source: "/images/WizardWand.png"
				   Layout.alignment: Qt.AlignCenter
			   }
			   IconButtonText{
				   id: reset
				   text: qsTr("Reset to Defaults")
				   icon.source: "/images/Reset_Default.png"
				   Layout.alignment: Qt.AlignCenter
			   }
			   Text {
				   text: qsTr("Send + Save")
				   Layout.alignment: Qt.AlignRight
			   }
			   IconButton{
				   id: send
				   Layout.alignment: Qt.AlignLeft
				   icon.source: "/images/ToolAcceptChange.png"
				   implicitWidth: 130
				   onClicked: { settingsArea.save_settings() ; unsaved.visible = false }
			   }
		   }
		   Image {
			   id: unsaved
			   width: 100
			   anchors.right: parent.right
			   anchors.rightMargin: 10
			   anchors.verticalCenter: bottomRightButtons.verticalCenter
			   visible: false
			   source: "/images/Config/ConSt_Mandatory.png"
		   }
	   }
   }
}
