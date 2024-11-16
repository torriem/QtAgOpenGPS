// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later

// Displays the main QML page. All other QML windows are children of this one.
//Loaded by formgps_ui.cpp.
import QtQuick
import QtQuick.Window
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Effects
import QtQuick.Dialogs
import QtMultimedia
import AgOpenGPS 1.0

import "interfaces" as Interfaces
import "boundary" as Boundary
import "steerconfig" as SteerConfig
import "config" as ConfigSettings //"Config" causes errors
import "field" as Field
import "tracks" as Tracks
import "components" as Comp

Window {


    AOGTheme{
        id: theme
        objectName: "theme"
    }
    //We draw native opengl to this root object
    id: mainWindow
	height: theme.defaultHeight
	width: theme.defaultWidth

    onVisibleChanged: if(settings.setDisplay_isStartFullScreen){
                          mainWindow.showMaximized()
                      }

    signal save_everything()

    function close() {
		if (areWindowsOpen()) {
			timedMessage.addMessage(2000,qsTr("Some windows are open. Close them first."))
			console.log("some windows are open. close them first")
			return
		}
        if (aog.autoBtnState + aog.manualBtnState  > 0) {
            timedMessage.addMessage(2000,qsTr("Section Control on. Shut off Section Control."))
            close.accepted = false
			console.log("Section Control on. Shut off Section Control.")
            return
        }
        if (mainWindow.visibility !== (Window.FullScreen) && mainWindow.visibility !== (Window.Maximized)){
            settings.setWindow_Size = ((mainWindow.width).toString() + ", "+  (mainWindow.height).toString())
        }

        if (aog.isJobStarted) {
            closeDialog.visible = true
            close.accepted = false
			console.log("job is running. close it first")
			return
        }
		Qt.quit()
    }
	function areWindowsOpen() {
        if (config.visible === true) {
			console.log("config visible") 
			return true
		}
        else if (headlandDesigner.visible === true) {
			console.log("headlandDesigner visible") 
			return true
		}
        else if (headacheDesigner.visible === true) {
			console.log("headacheDesigner visible") 
			return true
		}
        else if (steerConfigWindow.visible === true) {
			console.log("steerConfigWindow visible") 
			return true
		}
        else if (abCurvePicker.visible === true) {
			console.log("abCurvePicker visible") 
			return true
		}
        else if (abLinePicker.visible === true) {
			console.log("abLinePicker visible") 
			return true
		}
        else if (tramLinesEditor.visible === true) {
			console.log("tramLinesEditor visible") 
			return true
		}
        else if (lineEditor.visible === true) {
			console.log("lineEditor visible") 
			return true
		}
		//if (boundaryMenu.visible == true) return false
		//if (lineDrawer.visible) return false
        //if (lineNudge.acive) return false
		//if (refNudge.acive) return false
        else if (setSimCoords.visible === true) {
			console.log("setSimCoords visible") 
			return true
		}
        else if (trackNew.visible === true) {
			console.log("trackNew visible") 
			return true
		}
        else if (fieldNew.visible === true) {
			console.log("FieldNew visible") 
			return true
		}
		//if (fieldFromKML.visible) return false
        else if (fieldOpen.visible === true) return true
		//if (contextFlag.visible == true) return false
		else return false
	}

    //there's a global "settings" property now.  In qmlscene we'll have to fake it somehow.

    //MockSettings {
    //    id: settings
    //}

    AOGInterface {
        id: aog
        objectName: "aog"
    }
    Interfaces.LinesInterface {
        objectName: "linesInterface"
        id: linesInterface
    }

    Interfaces.FieldInterface {
        id: fieldInterface
        objectName: "fieldInterface"
    }

    Interfaces.VehicleInterface {
        id: vehicleInterface
        objectName: "vehicleInterface"
    }

    Interfaces.BoundaryInterface {
        id: boundaryInterface
        objectName: "boundaryInterface"
    }

    Interfaces.RecordedPathInterface {
        id: recordedPathInterface
        objectName: "recordedPathInterface"
    }

    UnitConversion {
        id: utils
    }

    Comp.TimedMessage {
        //This is a popup message that dismisses itself after a timeout
        id: timedMessage
        objectName: "timedMessage"
    }

    SystemPalette {
        id: systemPalette
        colorGroup: SystemPalette.Active
    }


    Rectangle { //do we need this
        id: background
        objectName: "background"
        anchors.top: topLine.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom:  parent.bottom

        color: "black"
        /*
              Text {
                  id: text2
                  text: qsTr("No GPS")
                  color: "white"
                  font.pointSize: 24
                  anchors.centerIn: parent
              }
              */
    }

    Rectangle{
        id: topLine
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        color: aog.backgroundColor
        height: 50 *theme.scaleHeight
        visible: true

        Comp.IconButtonTransparent {
            id: btnfileMenu
            height: parent.height
            width: 75 * theme.scaleWidth
            icon.source: prefix + "/images/fileMenu.png"
            onClicked: hamburgerMenu.visible = true
        }
//        Menu{
//            id: fileMenu
//            //anchors.left: buttonsArea.left
//            //anchors.top: buttonsArea.top
//            //anchors. margins: 200
//            MenuItem{ text: "Languages"}
//            MenuItem{ text: "Directories"}
//            MenuItem{ text: "Colors"}
//            MenuItem{ text: "Section Colors"}
//            MenuItem{ text: "Top Field View"}
//            MenuItem{ text: "Enter Sim Coords"}
//            MenuItem{
//                property bool isChecked: settings.setMenu_isSimulatorOn
//                onIsCheckedChanged: {
//                    checked = isChecked
//                }

//                text: "Simulator On"
//                checkable: true
//                checked: isChecked
//                onCheckedChanged: {
//                    settings.setMenu_isSimulatorOn = checked
//                    console.log("Sim = "+settings.setMenu_isSimulatorOn)
//                }
//            }
//            MenuItem{ text: "Reset All"}
//            MenuItem{ text: "HotKeys"}
//            MenuItem{ text: "About..."}
//            MenuItem{ text: "Help"}
//            closePolicy: Popup.CloseOnPressOutsideParent
//        }

        Text{
            anchors.top:parent.top
            anchors.left: parent.left
            anchors.leftMargin: leftColumn.width+20
            text: (aog.fixQuality === 0 ? "Invalid":
                  aog.fixQuality ===1 ? "GPS Single":
                  aog.fixQuality ===2 ? "DGPS":
                  aog.fixQuality ===3 ? "PPS":
                  aog.fixQuality ===4 ? "RTK Fix":
                  aog.fixQuality ===5 ? "RTK Float":
                  aog.fixQuality ===6 ? "Estimate":
                  aog.fixQuality ===7 ? "Man IP":
                  aog.fixQuality ===8 ? "Sim":
                  "Invalid") + ": Age: "+ Math.round(aog.age * 10)/ 10
            font.pixelSize: 20
            anchors.bottom: parent.verticalCenter
        }

//        Text {
//            anchors.top: parent.top
//            anchors.left: parent.left
//            anchors.leftMargin: 120
//            text: qsTr("Field: "+ (aog.isJobStarted ? settings.setF_CurrentDir: "None"))
//            anchors.bottom: parent.verticalCenter
//            font.bold: true
//            font.pixelSize: 15
//        }
        Text {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 150
            text: qsTr("ac")
            anchors.top: parent.verticalCenter
            font.bold: true
            font.pixelSize: 15
        }
        Text {
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("ab")
            font.bold: true
            font.pixelSize: 15
        }
        Row{
            id: topRowWindow
            width: childrenRect.width
            height: parent.height
            anchors.top: parent.top
            anchors.right: parent.right
			spacing: 5 * theme.scaleWidth
            Comp.IconButton {
                id: btnFieldInfo
                icon.source: prefix + "/images/FieldStats.png"
                Layout.alignment: Qt.AlignCenter
                implicitWidth: theme.buttonSize
                height:parent.height
				visible: aog.isJobStarted
                onClicked: {
                    fieldData.visible = !fieldData.visible
                    gpsData.visible = false
                }
            }
            Comp.IconButtonColor{
                id: rtkStatus
                icon.source: prefix + "/images/GPSQuality.png"
                implicitWidth: 75 * theme.scaleWidth
                implicitHeight: parent.height
                color: "yellow"
                onClicked: {
                    gpsData.visible = !gpsData.visible
                    fieldData.visible = false
                }
                Connections{
                    target: aog
                    function onFixQualityChanged() {
                        if(aog.fixQuality == 4) rtkStatus.color = "green"
                        else if(aog.fixQuality == 5) rtkStatus.color = "orange"
                        else if(aog.fixQuality == 2) rtkStatus.color = "yellow"
                        else rtkStatus.color = "red"
                    }
                }

            }

            Text{
                id: speed
                anchors.verticalCenter: parent.verticalCenter
                width: 75 * theme.scaleWidth
                height:parent.height
                text: utils.speed_to_unit_string(aog.speedKph, 1)
                font.bold: true
                font.pixelSize: 35
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Comp.IconButtonTransparent{
                height: parent.height
                icon.source: prefix + "/images/WindowMinimize.png"
                width: 75 * theme.scaleWidth
                onClicked: mainWindow.showMinimized()
            }
            Comp.IconButtonTransparent{
				id: btnMaximize
                height: parent.height
                icon.source: prefix + "/images/WindowMaximize.png"
                width: 75 * theme.scaleWidth
                onClicked: {
                    console.debug("Visibility is " + mainWindow.visibility)
                    if (mainWindow.visibility == Window.FullScreen){
                        mainWindow.showNormal()
                }else{
                        settings.setWindow_Size = ((mainWindow.width).toString() + ", "+  (mainWindow.height).toString())
                        mainWindow.showFullScreen()
                    }
                }
            }
            Comp.IconButtonTransparent{
                height: parent.height
                width: 75 * theme.scaleWidth
                icon.source: prefix + "/images/WindowClose.png"
                onClicked: {
                    mainWindow.save_everything()
                    mainWindow.close()
                }
            }
        }
    }

    AOGRenderer {
        id: glcontrolrect
        objectName: "openglcontrol"

        anchors.top: topLine.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        //for moving the center of the view around
        property double shiftX: 0 //-1 left to 1 right
        property double shiftY: 0 //-1 down to 1 up

        signal clicked(var mouse)
        signal dragged(int fromX, int fromY, int toX, int toY)
        signal zoomOut()
        signal zoomIn()

        MouseArea {
            id: mainMouseArea
            anchors.fill: parent

            property int fromX: 0
            property int fromY: 0
            property Matrix4x4 clickModelView
            property Matrix4x4 clickProjection
            property Matrix4x4 panModelView
            property Matrix4x4 panProjection

            onClicked: {
                parent.clicked(mouse)
            }

            onPressed: if(aog.panMode){
                           //save a copy of the coordinates
                           fromX = mouseX
                           fromY = mouseY
                       }

            onPositionChanged: if(aog.panMode){
                                   parent.dragged(fromX, fromY, mouseX, mouseY)
                                   fromX = mouseX
                                   fromY = mouseY
                               }

            onWheel:(wheel)=>{
                if (wheel.angleDelta.y > 0) {
                    aog.zoomIn()
                } else if (wheel.angleDelta.y <0 ) {
                    aog.zoomOut()
                }
            }

            Image {
                id: reverseArrow
                x: aog.vehicle_xy.x - 150
                y: aog.vehicle_xy.y - height
                width: 70 * theme.scaleWidth
                height: 70 * theme.scaleHeight
                source: prefix + "/images/Images/z_ReverseArrow.png"
                visible: aog.isReverse
            }
            MouseArea{
                //button that catches any clicks on the vehicle in the GL Display
                id: resetDirection
                onClicked: {
                    aog.reset_direction()
                    console.log("reset direction")
                }
                propagateComposedEvents: true
                x: aog.vehicle_bounding_box.x
                y: aog.vehicle_bounding_box.y
                width: aog.vehicle_bounding_box.width
                height: aog.vehicle_bounding_box.height
                onPressed: (mouse)=>{
                               aog.reset_direction()
                               console.log("pressed")
                               mouse.accepted = false

                           }
            }
//            Rectangle{
//              // to show the reset vehicle direction button for testing purposes
//                color: "blue"
//                anchors.fill: resetDirection
//            }
        }

    }

    Rectangle{
        id: noGPS
        anchors.fill: glcontrolrect
        color: "#0d0d0d"
        visible: aog.sentenceCounter> 29
        onVisibleChanged: if(visible){
                              console.log("no gps now visible")
                          }

        Image {
            id: noGPSImage
            source: prefix + "/images/Images/z_NoGPS.png"
            anchors.centerIn: parent
            anchors.margins: 200
            visible: noGPS.visible
            height: parent.height /2
            width: height
        }
    }

    //----------------------------------------------------------------------------------------left column
    Item {//item to hold all the main window buttons. Fills all of main screen

        id: buttonsArea
        anchors.top: parent.top
        anchors.topMargin: 2 //TODO: convert to scalable //do we need?
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        ColumnLayout {
            id: leftColumn
            anchors.top: parent.top
            anchors.topMargin: topLine.height
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 6
			onHeightChanged: {
				theme.btnSizes[2] = height / (children.length) 
				theme.buttonSizesChanged()
			}
            onVisibleChanged: if(visible === false)
                                  width = 0
                              else
                                  width = children.width
            Button {
                id: btnAcres
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                Layout.alignment: Qt.AlignCenter
                onClicked: {
                    aog.distanceUser = "0"
                    aog.workedAreaTotalUser = "0"
                }

                background: Rectangle{
                    anchors.fill: parent
                    color: aog.backgroundColor
                    radius: 10
                    Text{
                        anchors.top: parent.top
                        anchors.bottom: parent.verticalCenter
                        anchors.margins: 5
                        width: parent.width
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: utils.m_to_unit_string(aog.distanceUser, 2)
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: parent.height * .33
                    }
                    Text{
                        anchors.top: parent.verticalCenter
                        anchors.bottom: parent.bottom
                        anchors.margins: 5
                        width: parent.width
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: utils.area_to_unit_string(aog.workedAreaTotalUser, 2)
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: parent.height * .33
                    }
                }
            }
            Comp.IconButtonText {
                id: btnnavigationSettings
                buttonText: qsTr("Display")
                icon.source: prefix + "/images/NavigationSettings.png"
                onClicked: displayButtons.visible = !displayButtons.visible
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                Layout.alignment: Qt.AlignCenter
            }
            Comp.IconButtonText {
                id: btnSettings
                buttonText: qsTr("Settings")
                icon.source: prefix + "/images/Settings48.png"
                onClicked: config.open()
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                Layout.alignment: Qt.AlignCenter
            }
            Comp.IconButtonText {
                id: btnTools
                buttonText: qsTr("Tools")
                icon.source: prefix + "/images/SpecialFunctions.png"
                onClicked: toolsMenu.visible = true
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                Layout.alignment: Qt.AlignCenter
            }
            Comp.IconButtonText{
                id: btnFieldMenu
                buttonText: qsTr("Field")
                icon.source: prefix + "/images/JobActive.png"
                onClicked: fieldMenu.visible = true
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                Layout.alignment: Qt.AlignCenter
            }
            Comp.IconButtonText{
                id: btnFieldTools
                buttonText: qsTr("Field Tools")
                icon.source: prefix + "/images/FieldTools.png"
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                onClicked: fieldTools.visible = true
                enabled: aog.isJobStarted ? true : false
                Layout.alignment: Qt.AlignCenter
            }

            Comp.IconButtonText {
                id: btnAgIO
                buttonText: qsTr("AgIO")
                icon.source: prefix + "/images/AgIO.png"
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                Layout.alignment: Qt.AlignCenter
            }
            Comp.IconButtonText {
                id: btnautoSteerConf
                buttonText: qsTr("Steer config")
                icon.source: prefix + "/images/AutoSteerConf.png"
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                onClicked: {
                    steerConfigWindow.visible = true
                    steerConfigWindow.show()
                }
            }
        }
        //------------------------------------------------------------------------------------------right

        Speedometer {
            anchors.top: parent.top
            anchors.right: rightColumn.left
            anchors.topMargin: topLine.height + 10
            anchors.margins: 10
            visible: settings.setMenu_isSpeedoOn

            speed: utils.speed_to_unit(aog.speedKph)
        }

        SteerCircle { //the IMU indicator on the bottom right -- Called the "SteerCircle" in AOG
            anchors.bottom: bottomButtons.top
            anchors.right: rightColumn.left
            anchors.margins: 10
            visible: true
            rollAngle: aog.imuRollDegrees
            steerColor: (aog.steerModuleConnectedCounter > 30 ?
                             "#f0f218f0" :
                             (aog.steerSwitchHigh === true ?
                                  "#faf80007" :
                                  (aog.isAutoSteerBtnOn === true ?
                                       "#f80df807" : "#f0f2c007")))

        }


        ColumnLayout {
            id: rightColumn //buttons
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.topMargin: topLine.height + 6
            anchors.rightMargin: 6

            visible: aog.isJobStarted

			onHeightChanged: {
				theme.btnSizes[0] = height / (children.length) 
				theme.buttonSizesChanged()
			}
            onVisibleChanged: if(visible === false)
                                  width = 0
                              else
                                  width = children.width

            Comp.IconButtonText {
                property bool isContourLockedByUser //store if user locked
                id: btnContourLock
                isChecked: aog.btnIsContourLocked
                visible: btnContour.checked
                checkable: true
                icon.source: prefix + "/images/ColorUnlocked.png"
                iconChecked: prefix + "/images/ColorLocked.png"
                implicitWidth: theme.buttonSize
                implicitHeight: theme.buttonSize
                buttonText: "Lock"
                Layout.alignment: Qt.AlignCenter
                onClicked: {
                    aog.btnContourLock()
                    if (aog.btnIsContourLocked)
                        isContourLockedByUser = true
                }
                Connections{
                    target: aog
                    function onBtnIsContourLockedChanged() {
                        btnContourLock.checked = aog.btnIsContourLocked
                        if(btnContourLock.isContourLockedByUser)
                            btnContourLock.isContourLockedByUser = false
                    }
                    function onIsAutoSteerBtnOnChanged() {
                        if (!btnContourLock.isContourLockedByUser && btnContour.checked === true){
                            if(btnContourLock.checked !== aog.isAutoSteerBtnOn){
                                aog.btnContourLock()
                            }
                        }
                    }
                }
            }
            Comp.IconButtonText {
                id: btnContour
                isChecked: aog.isContourBtnOn //set value from backend
                checkable: true
                icon.source: prefix + "/images/ContourOff.png"
                iconChecked: prefix + "/images/ContourOn.png"
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                buttonText: "Contour"
                //color: "white"
                Layout.alignment: Qt.AlignCenter
                onClicked: {
                    aog.btnContour()
                }
                onCheckedChanged: { //gui logic
                    btnABLineCycle.visible = !checked
                    btnABLineCycleBk.visible = !checked
                    btnContourPriority.visible = checked
                }
            }
            Comp.IconButtonText{
                id: btnABCurve
                isChecked: false
                checkable: true
                icon.source: prefix + "/images/CurveOff.png"
                iconChecked: prefix + "/images/CurveOn.png"
                buttonText: "ABCurve"
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                visible: !btnContour.checked
                onClicked: {
                    abCurvePicker.visible = true
                    if (aog.currentABCurve > -1) {
                        btnABCurve.checked = true
                    }
                }
                Connections {
                    target: abCurvePicker
                    function onAccepted(){
                        if(aog.currentABCurve > -1) {
                            //mutually exclusive with the other
                            //buttons
                            btnABCurve.checked = true
                            btnABLine.checked = false
                            aog.currentABLine = -1;
                        }
                    }
                    function onRejected(){
                        btnABCurve.checked = false
                        //leave any other buttons alone
                    }

                }
            }
            Comp.IconButtonText{
                id: btnABLine
                checkable: true
                //TODO: this should be set programmatically
                //Also the types of lines are all mutually exclusive
                icon.source: prefix + "/images/ABLineOff.png"
                iconChecked: prefix + "/images/ABLineOn.png"
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                Layout.alignment: Qt.AlignCenter
                visible: !btnContour.checked
                onClicked: {
                    abLinePicker.visible = true
                    if (aog.currentABLine > -1)
                        btnABLine.checked = true
                }
                Connections {
                    target: abLinePicker
                    function onAccepted(){
                        if(aog.currentABLine > -1) {
                            btnABLine.checked = true
                            btnABCurve.checked = false
                            aog.currentABCurve = -1
                        }
                        console.debug( ((aog.currentABLine > -1) || (aog.currentABCurve > -1)))
                    }
                    function onRejected(){
                        btnABLine.checked = false
                    }
                }
                checked: aog.currentABLine > -1 ? true : false
                buttonText: "ABLine"
            }

            Comp.IconButton{
                id: btnABLineCycle
                icon.source: prefix + "/images/ABLineCycle.png"
                width: btnABLine.width
                height: btnABLine.height
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
            }
            Comp.IconButton{
                id: btnABLineCycleBk
                icon.source: prefix + "/images/ABLineCycleBk.png"
                width: btnABLine.width
                height: btnABLine.height
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
            }

            Comp.IconButtonText {
                id: btnSectionManual
                isChecked: aog.manualBtnState == 2
                checkable: true
                icon.source: prefix + "/images/ManualOff.png"
                iconChecked: prefix + "/images/ManualOn.png"
                buttonText: "Manual"
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                onCheckedChanged: {
                    if (checked) {
                        btnSectionAuto.checked = false;
                        sectionButtons.setAllSectionsToState(2 /*auto*/);
                        aog.manualBtnState = 2 //btnStates::on

                    } else {
                        sectionButtons.setAllSectionsToState(0 /*off*/);
                        aog.manualBtnState = 0
                    }
                }
            }

            Comp.IconButtonText {
                id: btnSectionAuto
                isChecked: aog.autoBtnState == 1
                checkable: true
                icon.source: prefix + "/images/SectionMasterOff.png"
                iconChecked: prefix + "/images/SectionMasterOn.png"
                buttonText: "Auto"
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                onCheckedChanged: {
                    if (checked) {
                        btnSectionManual.checked = false;
                        sectionButtons.setAllSectionsToState(1 /*auto*/);
                        aog.autoBtnState = 1 //btnStates::auto
                    } else {
                        sectionButtons.setAllSectionsToState(0 /*off*/);
                        aog.autoBtnState = 0
                    }
                }
            }
            Comp.IconButtonText {
                id: btnAutoYouTurn
                isChecked: aog.isYouTurnBtnOn
                checkable: true
                icon.source: prefix + "/images/YouTurnNo.png"
                iconChecked: prefix + "/images/YouTurn80.png"
                buttonText: "AutoUturn"
                visible: aog.isTrackOn
                enabled: aog.isAutoSteerBtnOn
                onClicked: aog.autoYouTurn()
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
            }
            Comp.IconButtonText {
                id: btnAutoSteer
                icon.source: prefix + "/images/AutoSteerOff.png"
                iconChecked: prefix + "/images/AutoSteerOn.png"
                checkable: true
                checked: aog.isAutoSteerBtnOn
                enabled: aog.isTrackOn || aog.isContourBtnOn
                //Is remote activation of autosteer enabled? //todo. Eliminated in 6.3.3
                buttonText: (settings.setAS_isAutoSteerAutoOn === true ? "R" : "M")
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                onClicked: {
                    if (checked && ((aog.currentABCurve > -1) || (aog.currentABLine > -1) || btnContour.isChecked)) {
                        console.debug("okay to turn on autosteer button.")
                        aog.isAutoSteerBtnOn = true;
                    } else {
                        console.debug("keep autosteer button off.")
                        checked = false;
                        aog.isAutoSteerBtnOn = false;
                    }
                }
                Connections {
                    target: aog
                    function onIsAutoSteerBtnOnChanged() {
                        if (aog.isAutoSteerBtnOn && ((aog.currentABCurve > -1) || (aog.currentABLine > -1))) {
                            btnAutoSteer.checked = true
                        } else {
                            //default to turning everything off
                            btnAutoSteer.checked = false
                            aog.isAutoSteerBtnOn = false
                        }
                    }
                    function onSpeedKphChanged() {
                        if (btnAutoSteer.checked) {
                            if (aog.speedKph < settings.setAS_minSteerSpeed) {
                                aog.isAutoSteerBtnOn = false
                            } else if (aog.speedKph > settings.setAS_maxSteerSpeed) {
                                //timedMessage
                                aog.isAutoSteerBtnOn = false
                            }
                        }
                    }
                }
            }
        }

        Comp.IconButton {
            id: btnContourPriority
            anchors.top: parent.top
            anchors.topMargin: btnContour.height + 3
            anchors.right: rightColumn.left
            anchors.rightMargin: 3
            checkable: true
            isChecked: true
            visible: false
            icon.source: prefix + "/images/ContourPriorityLeft.png"
            iconChecked: prefix + "/images/ContourPriorityRight.png"
            onClicked: aog.btnContourPriority(checked)
        }

        RowLayout{
            id:bottomButtons
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: theme.buttonSize + 3
            anchors.right: parent.right
            anchors.rightMargin: theme.buttonSize + 3
            visible: aog.isJobStarted && leftColumn.visible

           /* onVisibleChanged: if(visible == false)
                                  height = 0
                              else
                                  height = children.height*/
            //spacing: parent.rowSpacing
			onWidthChanged: {
				theme.btnSizes[1] = width / (children.length) 
				theme.buttonSizesChanged()
			}
			onVisibleChanged: {
                if (visible === false)
					height = 0
				else
					height = children.height				

			}
            ComboBox {
                id: cbYouSkipNumber
                editable: true
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                model: ListModel {
                    id: model
                    ListElement {text: "1"}
                    ListElement {text: "2"}
                    ListElement {text: "3"}
                    ListElement {text: "4"}
                    ListElement {text: "5"}
                    ListElement {text: "6"}
                    ListElement {text: "7"}
                    ListElement {text: "8"}
                    ListElement {text: "9"}
                    ListElement {text: "10"}
                }
                onAccepted: {
                    if (cbYouSkipNumber.find(currentText) === -1){
                        model.append({text: editText})
                        curentIndex = cbYouSkipNumber.find(editText)
                    }
                }
            }
            Comp.IconButtonText {
                id: btnYouSkip // the "Fancy Skip" button
                isChecked: false
                checkable: true
                icon.source: prefix + "/images/YouSkipOff.png"
                iconChecked: prefix + "/images/YouSkipOn.png"
                buttonText: "YouSkips"
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
            }
            Comp.IconButtonText { //reset trailing tool to straight back
                id: btnResetTool
                icon.source: prefix + "/images/ResetTool.png"
                buttonText: "Reset Tool"
                Layout.alignment: Qt.AlignCenter
                onClicked: aog.btnResetTool()
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                visible: settings.setTool_isToolTrailing === true //hide if front or rear 3 pt
            }
            Comp.IconButtonText {
                id: btnSectionMapping
                icon.source: prefix + "/images/SectionMapping"
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                onClicked: cpSectionColor.open()
            }
            Comp.IconButtonText {
                id: btnTramLines
                icon.source: prefix + "/images/TramLines.png"
                buttonText: "Tram Lines"
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
            }
            Comp.IconButtonText {
                property bool isOn: false
                id: btnHydLift
                isChecked: isOn
                checkable: true
                disabled: btnHeadland.checked
                visible: utils.isTrue(settings.setArdMac_isHydEnabled) && btnHeadland.visible
                icon.source: prefix + "/images/HydraulicLiftOff.png"
                iconChecked: prefix + "/images/HydraulicLiftOn.png"
                buttonText: "HydLift"
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                onClicked: {
                    isOn = !isOn
                    aog.isHydLiftOn(isOn)
                }
            }
            Comp.IconButtonText {
                id: btnHeadland
                isChecked: aog.isHeadlandOn
                checkable: true
                icon.source: prefix + "/images/HeadlandOff.png"
                iconChecked: prefix + "/images/HeadlandOn.png"
                buttonText: "Headland"
                Layout.alignment: Qt.AlignCenter
                onClicked: aog.btnHeadland()
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
            }
            Comp.IconButtonText {
                id: btnFlag
                objectName: "btnFlag"
                isChecked: false
                icon.source: prefix + "/images/FlagRed.png"
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                onPressAndHold: {
                    if (contextFlag.visible) {
                        contextFlag.visible = false;
                    } else {
                        contextFlag.visible = true;
                    }
                }
                buttonText: "Flag"
            }

            Comp.IconButtonText {
                id: btnTrackOn
                icon.source: prefix + "/images/TrackOn.png"
                buttonText: "Track"
                onClicked: trackButtons.visible = !trackButtons.visible
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
            }

        }
        //----------------inside buttons-----------------------
        Item{
            //plan to move everything on top of the aogRenderer that isn't
            //in one of the buttons columns
            id: inner
            anchors.left: leftColumn.right
            anchors.top: parent.top
            anchors.topMargin: topLine.height
            anchors.right: rightColumn.left
            anchors.bottom: bottomButtons.top
            visible: !noGPS.visible
            Comp.IconButtonTransparent{ //button to pan around main GL
                implicitWidth: 50
                implicitHeight: 50 * theme.scaleHeight
                checkable: true
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 30
                icon.source: prefix + "/images/Pan.png"
                iconChecked: prefix + "/images/SwitchOff.png"
                onClicked: aog.panMode = !aog.panMode
            }
            Image{
                id: hydLiftIndicator
                property bool isDown: aog.hydLiftDown
                visible: false
                source: prefix + "/images/Images/z_Lift.png"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                width: 80 * theme.scaleWidth
                height: 130 * theme.scaleHeight
                onIsDownChanged: {
                    if(!isDown){
                        hydLiftIndicatorColor.color = "#00F200"
                        hydLiftIndicatorColor.rotation = 0
                    }else{
                        hydLiftIndicatorColor.rotation = 180
                        hydLiftIndicatorColor.color = "#F26600"
                    }
                }
            }
            MultiEffect{
                id: hydLiftIndicatorColor
                anchors.fill: hydLiftIndicator
                visible: btnHydLift.isOn
                colorizationColor:"#F26600"
                colorization: 1.0
                source: hydLiftIndicator
            }

            Comp.OutlineText{
                id: simulatorOnText
                visible: settings.setMenu_isSimulatorOn
                anchors.top: parent.top
                anchors.topMargin: lightbar.height+ 10
                anchors.horizontalCenter: lightbar.horizontalCenter
                font.pixelSize: 30
                color: "#cc5200"
                text: qsTr("Simulator On")
            }

            Comp.OutlineText{
                id: ageAlarm //Lost RTK count up display
                property int age: aog.age
                visible: settings.setGPS_isRTK
                anchors.top: simulatorOnText.bottom
                anchors.topMargin: 30
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Lost RTK"
                font.pixelSize: 65
                color: "#cc5200"
                onAgeChanged: {
                    if (age < 20)
                        text = ""
                    else if (age> 20 && age < 60)
                        text = qsTr("Age: ")+age
                    else
                        text = "Lost RTK"
                }
                onTextChanged: if (text.length > 0)
                                   console.log("rtk alarm sound")

            }

            Item{
                id: autoTurn // where the auto turn button and distance to turn are held
                anchors.top:gridTurnBtns.top
                anchors.right: parent.right
                anchors.rightMargin: 200
                width: 100 * theme.scaleWidth
                height: 100 * theme.scaleHeight

                 Image{
                    id: autoTurnImage
                    source: if(!aog.isYouTurnRight)
                                prefix + "/images/Images/z_TurnRight.png"
                            else
                                prefix + "/images/Images/z_TurnLeft.png"
                    visible: false
                    anchors.fill: parent
                }
                MultiEffect{
                    id: colorAutoUTurn
                    anchors.fill: parent
                    source: autoTurnImage
                    visible: btnAutoYouTurn.isChecked
                    //color: "#E5E54B"
                    colorizationColor: if(aog.distancePivotToTurnLine > 0)
                               "#4CF24C"
                            else
                                "#F7A266"
                    colorization: 1.0
                    MouseArea{
                        anchors.fill: parent
                        onClicked: aog.swapAutoYouTurnDirection()
                    }
                    Text{
                        id: distance
                        anchors.bottom: colorAutoUTurn.bottom
                        color: colorAutoUTurn.colorizationColor
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: if(aog.distancePivotToTurnLine > 0)
                                  utils.m_to_unit_string(aog.distancePivotToTurnLine, 0) + " "+utils.m_unit_abbrev()
                               else
                                  "--"
                    }
                }
            }
			Grid{
                id: gridTurnBtns //lateral turn and manual Uturn
				spacing: 10
				rows: 2
				columns: 2
				flow: Grid.LeftToRight
				anchors.top: lightbar.bottom
				anchors.left: parent.left
				anchors.topMargin: 30
				anchors.leftMargin: 150
				visible: aog.isAutoSteerBtnOn
                Comp.IconButtonTransparent{
					implicitHeight: 65 * theme.scaleHeight
					implicitWidth: 85 * theme.scaleWidth
					imageFillMode: Image.Stretch
                    icon.source: prefix + "/images/qtSpecific/z_TurnManualL.png"
					onClicked: {
						if (settings.setAS_functionSpeedLimit > aog.speedKph)
						aog.uturn(false)
						else
						timedMessage.addMessage(2000,qsTr("Too Fast"), qsTr("Slow down below") + " " +
						utils.speed_to_unit_string(settings.setAS_functionSpeedLimit,1) + " " + utils.speed_unit())
					}

				}

                Comp.IconButtonTransparent{
					implicitHeight: 65 * theme.scaleHeight
					implicitWidth: 85 * theme.scaleWidth
					imageFillMode: Image.Stretch
                    icon.source: prefix + "/images/qtSpecific/z_TurnManualR.png"
					onClicked: {
						if (settings.setAS_functionSpeedLimit > aog.speedKph)
						aog.uturn(true)
						else
						timedMessage.addMessage(2000,qsTr("Too Fast"), qsTr("Slow down below") + " " +
						utils.speed_to_unit_string(settings.setAS_functionSpeedLimit,1) + " " + utils.speed_unit())
					}
				}
                Comp.IconButtonTransparent{
					implicitHeight: 65 * theme.scaleHeight
					implicitWidth: 85 * theme.scaleWidth
					imageFillMode: Image.Stretch
                    icon.source: prefix + "/images/qtSpecific/z_LateralManualL.png"
					onClicked: {
						if (settings.setAS_functionSpeedLimit > aog.speedKph)
						aog.lateral(false)
						else
						timedMessage.addMessage(2000,qsTr("Too Fast"), qsTr("Slow down below") + " " +
						aog.convert_speed_text(settings.setAS_functionSpeedLimit,1) + " " + aog.speed_unit())
					}
				}
                Comp.IconButtonTransparent{
					implicitHeight: 65 * theme.scaleHeight
					implicitWidth: 85 * theme.scaleWidth
					imageFillMode: Image.Stretch
                    icon.source: prefix + "/images/qtSpecific/z_LateralManualR.png"
					onClicked: {
						if (settings.setAS_functionSpeedLimit > aog.speedKph)
						aog.lateral(true)
						else
						timedMessage.addMessage(2000,qsTr("Too Fast"), qsTr("Slow down below") + " " +
						aog.convert_speed_text(settings.setAS_functionSpeedLimit,1) + " " + aog.speed_unit())
					}
				}
			}
        LightBar {
            id: lightbar
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 5
            dotDistance: aog.avgPivDistance / 10 //avgPivotDistance is averaged
            visible: (aog.offlineDistance != 32000 &&
                      (settings.setMenu_isLightbarOn === true ||
                       settings.setMenu_isLightbarOn === "true")) ?
                         true : false
        }

        TrackNum {
            id: tracknum
            anchors.top: lightbar.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 5

            font.pixelSize: 24

            //only use dir names for AB Lines with heading
            useDirNames: (aog.currentABLine > -1)
            currentTrack: aog.current_trackNum

            trackHeading: aog.currentABLine > -1 ?
                              aog.currentABLine_heading :
                              0

            visible: (utils.isTrue(settings.setDisplay_topTrackNum) &&
                      ((aog.currentABLine > -1) ||
                       (aog.currentABCurve > -1)))
            //TODO add contour
        }

        //Components- this is where the windows that get displayed over the
        //ogl get instantiated.
        Field.FieldData{ //window that displays field acreage and such
            id: fieldData
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            visible: false
        }
        GPSData{ //window that displays GPS data
            id: gpsData
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            visible: false
        }

        SimController{
            id: simBarRect
            anchors.bottom: timeText.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 8
            visible: utils.isTrue(settings.setMenu_isSimulatorOn)
			height: 60 * theme.scaleHeight
			onHeightChanged: anchors.bottomMargin = (8 * theme.scaleHeight)
        }
        RecPath{// recorded path menu
            id: recPath
            visible: false
        }

        Comp.OutlineText{ //displays time on bottom right of GL
            id: timeText
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: (50 * theme.scaleWidth)
            font.pixelSize: 20
            color: "#cc5200"
            text: new Date().toLocaleTimeString(Qt.locale())
            Timer{
                interval: 100
                repeat: true
                running: true
                onTriggered: timeText.text = new Date().toLocaleTimeString(Qt.locale())
            }
        }
        Comp.SectionButtons {
            id: sectionButtons
            visible: aog.isJobStarted ? true : false
            anchors.horizontalCenter: parent.horizontalCenter
			anchors.bottom: simBarRect.top
			anchors.bottomMargin: 8
			height: 40 * theme.scaleHeight
            width: 500  * theme.scaleWidth
			//onHeightChanged: anchors.bottomMargin = (bottomButtons.height + simBarRect.height + (24 * theme.scaleHeight))
        }
        DisplayButtons{ // window that shows the buttons to change display. Rotate up/down, day/night, zoom in/out etc. See DisplayButtons.qml
            id: displayButtons
            width: childrenRect.width + 10
            height: childrenRect.height + 10
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 20
            visible: false
            z:1
        }

        Tracks.TrackButtons{
            id: trackButtons
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 20
            visible: false
            z:1
        }
        Comp.IconButtonTransparent{ //button on bottom left to show/hide the bottom and right buttons
            id: toggleButtons
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.margins: 25
            visible: aog.isJobStarted
            width: 45 * theme.scaleWidth
            height: 25 * theme.scaleHeight
            icon.source: prefix + "/images/MenuHideShow.png"
            onClicked: if(leftColumn.visible){
                           leftColumn.visible = false
                       }else{
                           leftColumn.visible = true
                       }
        }
        Compass{
            id: compass
            anchors.top: parent.top
            anchors.right: zoomBtns.left
            heading: -utils.radians_to_deg(aog.heading)
        }
        Column{
            id: zoomBtns
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 10
            spacing: 100
            width: children.width
            Comp.IconButton{
                implicitWidth: 30 * theme.scaleWidth
                implicitHeight: 30 * theme.scaleHeight
                radius: 0
                icon.source: prefix + "/images/ZoomIn48.png"
                onClicked: aog.zoomIn()
            }
            Comp.IconButton{
                implicitWidth: 30 * theme.scaleWidth
                implicitHeight: 30 * theme.scaleHeight
                radius: 0
                icon.source: prefix + "/images/ZoomOut48.png"
                onClicked: aog.zoomOut()
            }
        }
    }



        Comp.SliderCustomized { //quick dirty hack--the up and down buttons change this value, so the speed changes
            id: speedSlider
            //anchors.bottom: bottomButtons.top
//            anchors.bottomMargin: 3
//            anchors.left:bottomButtons.left
//            anchors.leftMargin: 3
            from: -80
            to: 300
            value: 0
            visible: false
        }

        StartUp{ //splash we show on startup
            id: startUp
            z:10
            //visible: true
			visible: false  //no reason to look at this until release
        }


        Field.FieldToolsMenu {
            id: fieldTools
            visible: false
        }
        Field.FieldMenu {
            id: fieldMenu
            objectName: "slideoutMenu"
            visible: false
        }
        ToolsWindow {
            id: toolsMenu
            visible: false
        }
        HamburgerMenu{ // window behind top left on main GL
            id: hamburgerMenu
            visible: false
        }

        ConfigSettings.Config {
            id:config
			x: 0
			y: 0
			width: parent.width
			height: parent.height
            visible:false

            onAccepted: {
                console.debug("accepting settings and closing window.")
                aog.settings_save()
                aog.settings_reload()
            }
            onRejected: {
                console.debug("rejecing all settings changes.")
                aog.settings_revert()
                aog.settings_reload()
            }

        }
        HeadlandDesigner{
            id: headlandDesigner
            objectName: "headlandDesigner"
            //anchors.horizontalCenter: parent.horizontalCenter
            //anchors.verticalCenter: parent.verticalCenter
            visible: false
        }
        HeadAcheDesigner{
            id: headacheDesigner
            objectName: "headacheDesigner"
            //anchors.horizontalCenter: parent.horizontalCenter
            //anchors.verticalCenter: parent.verticalCenter
            visible: false
        }
        SteerConfig.SteerConfigWindow {
            id:steerConfigWindow
            visible: false
        }
        SteerConfig.SteerConfigSettings{
			id: steerConfigSettings
			anchors.fill: parent
			visible: false
		}
        ABCurvePicker{
            id: abCurvePicker
            objectName: "abCurvePicker"
            visible: false
        }
        ABLinePicker{
            id: abLinePicker
            objectName: "abLinePicker"
            visible: false
        }
        TramLinesEditor{
            id: tramLinesEditor
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 150
            anchors.topMargin: 50
            visible: false
        }
        LineEditor{
            id: lineEditor
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 150
            anchors.topMargin: 50
            visible: false
        }
        Boundary.BoundaryMenu{
            id: boundaryMenu
            visible: false
        }

        Tracks.LineDrawer {//window where lines are created off field boundary/edited
            id:lineDrawer
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            height: 768
            width:1024
            visible:false
        }
        Tracks.LineNudge{
            id: lineNudge
            visible: false
        }
        Tracks.RefNudge{
            id: refNudge
            visible: false
        }
        SetSimCoords{
            id: setSimCoords
            anchors.fill: parent
        }

        Tracks.TrackNew{
            id: trackNew
            visible: false
        }

        Rectangle{//show "Are you sure?" when close button clicked
            id: closeDialog
            width: 500 * theme.scaleWidth
            height: 100 * theme.scaleHeight
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: aog.backgroundColor
            border.color: aog.blackDayWhiteNight
            border.width: 2
            visible: false
            Comp.IconButtonText{
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                color1: "transparent"
                color2: "transparent"
                color3: "transparent"
                icon.source: prefix + "/images/back-button.png"
                onClicked: parent.visible = false
            }
            Comp.IconButtonText{
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                color1: "transparent"
                color2: "transparent"
                color3: "transparent"
                icon.source: prefix + "/images/ExitAOG.png"
                onClicked: {
                    mainWindow.save_everything()
                    Qt.quit()
                }
            }
        }
        Item{
            id: windowsArea      //container for declaring all the windows
            anchors.fill: parent //that can be displayed on the main screen
            Field.FieldFromExisting{
                id: fieldFromExisting
                x: 0
                y: 0
            }
            Field.FieldNew{
                id: fieldNew
            }
            Field.FieldFromKML{
                id: fieldFromKML
                x: 100
                y: 75
            }
            Field.FieldOpen{
                id: fieldOpen
                x: 100    }

                y: 75
            }
        }


        Rectangle {
            id: contextFlag
            objectName: "contextFlag"
            width: childrenRect.width+10
            height: childrenRect.height + 10
            color: "#bf163814"
            visible: false
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: btnFlag.width + 10
            anchors.topMargin: btnFlag.y
            border.color: "#c3ecc0"

            Grid {
                id: contextFlagGrid
                spacing: 5
                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.left: parent.left
                anchors.leftMargin: 5

                width: childrenRect.width
                height: childrenRect.height

                columns: 5
                flow: Grid.LeftToRight

                Comp.IconButton {
                    id: redFlag
                    objectName: "btnRedFlag"
                    icon.source: prefix + "/images/FlagRed.png";
                }
                Comp.IconButton {
                    id: greenFlag
                    objectName: "btnGreenFlag"
                    icon.source: prefix + "/images/FlagGrn.png";
                }
                Comp.IconButton {
                    id: yellowFlag
                    objectName: "btnYellowFlag"
                    icon.source: prefix + "/images/FlagYel.png";
                }
                Comp.IconButton {
                    id: deleteFlag
                    objectName: "btnDeleteFlag"
                    icon.source: prefix + "/images/FlagDelete.png"
                    enabled: false
                }
                Comp.IconButton {
                    id: deleteAllFlags
                    objectName: "btnDeleteAllFlags"
                    icon.source: prefix + "/images/FlagDeleteAll.png"
                    enabled: false
                }
            }
            /********************************dialogs***********************/
            ColorDialog{//color picker
                id: cpSectionColor
                onSelectedColorChanged: {

                    //just use the Day setting. AOG has them locked to the same color anyways
                    settings.setDisplay_colorSectionsDay = cpSectionColor.selectedColor;

                    //change the color on the fly. In AOG, we had to cycle the sections off
                    //and back on. This does for us.
                    if(btnSectionManual){
                        btnSectionManual.clicked()
                        btnSectionManual.clicked()
                    }else if(btnSectionAuto){
                        btnSectionAuto.clicked()
                        btnSectionAuto.clicked()
                    }
                }
            }
            CloseAOG{
                id: closeAOG
            }

        }
}


//stowed here so it isn't lost. No uncommented code there, though
    /*Shortcut{ //vim navigation rules !!!!
		sequence: "j"
		onActivated: aog.sim_bump_speed(true)
	}
	Shortcut{
		sequence: "k"
		onActivated: aog.sim_bump_speed(false)
	}
	Shortcut{
		sequence: "l"
		onActivated: simBarRect.changedSteerDir(true)
	}
	Shortcut{
		sequence: "h"
		onActivated: simBarRect.changedSteerDir(false)
	}
	Shortcut{
		sequence: "s"
		onActivated: aog.sim_zero_speed()
	}
	ShortcutCustomized{
		hotkeys: 0 //autosteer button on off
		onActivated: btnAutoSteer.clicked();
	}
	ShortcutCustomized{
		hotkeys: 1 //open the steer chart
		onActivated: btnABLineCycle.clicked();
	}
	ShortcutCustomized{
		hotkeys: 2 //FileSaveEverythingBeforeClosingField();
		onActivated: if(aog.isJobStarted){
			fieldInterface.field_close();
		}else{
			fieldInterface.field_open(settings.setF_CurrentDir)
		}
	}
	ShortcutCustomized{
		hotkeys: 3 // Flag click
		onActivated: btnFlag.clicked();
	}
	ShortcutCustomized{
		hotkeys: 4 //auto section on off
		onActivated: btnSectionManual.clicked();
	}
	ShortcutCustomized{
		hotkeys: 5 //auto section on off
		onActivated: btnSectionAuto.clicked();
	}
	ShortcutCustomized{
		hotkeys: 6 // Snap/Prioritu click
		onActivated: lineNudge.SnapToPivot();
	}
	ShortcutCustomized{
		hotkeys: 7 //snap left
		onActivated: {
			/*if (trk.idx > -1) //
			 {
				 trk.NudgeTrack((double)Properties.Settings.Default.setAS_snapDistance * -0.01);
             }
			 lineNudge.snapLeft()
		 }
	 }
	 ShortcutCustomized{
		 hotkeys: 8 //snap right
		 onActivated: lineNudge.snapRight()
	 }
	 ShortcutCustomized{
		 hotkeys: 9 //open the vehicle Settings
		 onActivated: btnautoSteerConf.clicked();
	 }
	 ShortcutCustomized{
		 hotkeys: 10 // Wizard
		 onActivated: console.log("not implemented. go to line 102 in mainwindow.qml to implement")
		 /*
		  Form fcs = Application.OpenForms["FormSteer"];

		  if (fcs != null)
		  {
			  fcs.Focus();
			  fcs.Close();
		  }

		  //check if window already exists
		  Form fc = Application.OpenForms["FormSteerWiz"];

		  if (fc != null)
		  {
			  fc.Focus();
			  //fc.Close();
			  return true;
		  }

		  //
		  Form form = new FormSteerWiz(this);
          form.Show(this);

	  }
	  /*if (event.key == (hotkeys[11])) //section or zone button
	   {
		   if (isSectionsNotZones) btnSection1Man.clicked();
		   else btnZone1.clicked();
	   }

	   if (event.key == (hotkeys[12])) //section or zone button
	   {
		   if (isSectionsNotZones) btnSection2Man.clicked();
		   else btnZone2.clicked();
	   }

	   if (event.key == (hotkeys[13])) //section or zone button
	   {
		   if (isSectionsNotZones) btnSection3Man.clicked();
		   else btnZone3.clicked();
	   }

	   if (event.key == (hotkeys[14])) //section or zone button
	   {
		   if (isSectionsNotZones) btnSection4Man.clicked();
		   else btnZone4.clicked();
	   }

	   if (event.key == (hotkeys[15])) //section or zone button
	   {
		   if (isSectionsNotZones) btnSection5Man.clicked();
		   else btnZone5.clicked();
	   }

	   if (event.key == (hotkeys[16])) //section or zone button
	   {
		   if (isSectionsNotZones) btnSection6Man.clicked();
		   else btnZone6.clicked();
	   }

	   if (event.key == (hotkeys[17])) //section or zone button
	   {
		   if (isSectionsNotZones) btnSection7Man.clicked();
		   else btnZone7.clicked();
	   }

	   if (event.key == (hotkeys[18])) //section or zone button
	   {
		   if (isSectionsNotZones) btnSection8Man.clicked();
		   else btnZone8.clicked();
	   }

	   //////////////////////////////////////////////

	   if (event.key == (Keys.NumPad1)) //auto section on off
	   btnSectionAuto.clicked();

	   if (event.key == (Keys.NumPad0)) //auto section on off
	   btnSectionManual.clicked();

	   if (event.key == (Keys.F11)) // Full Screen click
	   btnMaximize.clicked();


	   //reset Sim
	   if (event.key == Keys.R)
	   {
		   aog.sim.reset()
	   }

	   //UTurn
	   if (event.key == Keys.U)
	   {
		   /*sim.headingTrue += Math.PI;
			ABLine.isABValid = false;
			curve.isCurveValid = false;
			if (isBtnAutoSteerOn) btnAutoSteer.clicked();

			console.log("not implemented. go to line 205 in mainwindow.qml to implement")
		}

		//speed up
		if (event.key == Keys.Up)
		{
		}

		//slow down
		if (event.key == Keys.Down)
		{
		}

		//Stop
		if (event.key == Keys.OemPeriod)
		{
		}

		//turn right
		if (event.key == Keys.Right)
		{
		}

		//turn left
		if (event.key == Keys.Left)
		{
		}

		//zero steering
		if (event.key == Keys.OemQuestion)
		{
			simBarRect.zeroSteerAngle()			
		}

*
          For	/*if (event.key == Keys.OemOpenBrackets)
		 {
			 sim.stepDistance = 0;
			 sim.isAccelBack = true;
		 }

		 if (event.key == Keys.OemCloseBrackets)
		 {
			 sim.stepDistance = 0;
			 sim.isAccelForward = true;
		 }

		 if (event.key == Keys.OemQuotes)
		 {
			 sim.stepDistance = 0;
			 return true;
		 }

		 if (event.key == (Keys.F6)) // Fast/Normal Sim
		 {
			 /*if (timerSim.Enabled)
			  {
				  if (timerSim.Interval < 20) timerSim.Interval = 93;
				  else timerSim.Interval = 15;
			  }
			  console.log("not implemented. go to line 260 in mainwindow.qml to implement")
		  }
	  }
  }
}*/
