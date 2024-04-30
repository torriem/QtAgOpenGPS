import QtQuick
import QtQuick.Window
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Effects
import QtMultimedia
import AgOpenGPS 1.0

import "interfaces"
import "boundary"
import "steerconfig"
import "config"
import "field"
import "tracks"
import "components"

Window {


    AOGTheme{
        id: theme
        objectName: "theme"
    }
    //We draw native opengl to this root object
    id: mainWindow
    //height: utils.string_after_comma(settings.setWindow_Size)
    //width: utils.string_before_comma(settings.setWindow_Size)
	height: theme.defaultHeight
	width: theme.defaultWidth
    onVisibleChanged: if(settings.setDisplay_isStartFullScreen){
                          mainWindow.showMaximized()
                      }


    signal save_everything()

    function close() {
		console.log("close called")
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
		if (config.visible == true) {
			console.log("config visible") 
			return true
		}
		else if (headlandDesigner.visible == true) {
			console.log("headlandDesigner visible") 
			return true
		}
		else if (headacheDesigner.visible == true) {
			console.log("headacheDesigner visible") 
			return true
		}
		else if (steerConfigWindow.visible == true) {
			console.log("steerConfigWindow visible") 
			return true
		}
		else if (abCurvePicker.visible == true) {
			console.log("abCurvePicker visible") 
			return true
		}
		else if (abLinePicker.visible == true) {
			console.log("abLinePicker visible") 
			return true
		}
		else if (tramLinesEditor.visible == true) {
			console.log("tramLinesEditor visible") 
			return true
		}
		else if (lineEditor.visible == true) {
			console.log("lineEditor visible") 
			return true
		}
		//if (boundaryMenu.visible == true) return false
		//if (lineDrawer.visible) return false
        //if (lineNudge.acive) return false
		//if (refNudge.acive) return false
		else if (setSimCoords.visible == true) {
			console.log("setSimCoords visible") 
			return true
		}
		else if (trackNew.visible == true) {
			console.log("trackNew visible") 
			return true
		}
		else if (fieldNew.visible == true) {
			console.log("FieldNew visible") 
			return true
		}
		//if (fieldFromKML.visible) return false
		else if (fieldOpen.visible == true) return true
		//if (contextFlag.visible == true) return false
		else return false
	}

    //objectName: "openGLControl"
    //width:800
    //height:600
    //anchors.fill: parent

    //there's a global "settings" property now.  In qmlscene we'll have to fake it somehow.

    //MockSettings {
    //    id: settings
    //}

    AOGInterface {
        id: aog
        objectName: "aog"
    }
    LinesInterface {
        objectName: "linesInterface"
        id: linesInterface
    }

    FieldInterface {
        id: fieldInterface
        objectName: "fieldInterface"
    }

    VehicleInterface {
        id: vehicleInterface
        objectName: "vehicleInterface"
    }

    BoundaryInterface {
        id: boundaryInterface
        objectName: "boundaryInterface"
    }

    RecordedPathInterface {
        id: recordedPathInterface
        objectName: "recordedPathInterface"
    }

    UnitConversion {
        id: utils
    }

    TimedMessage {
        //This is a popup message that dismisses itself after a timeout
        id: timedMessage
        objectName: "timedMessage"
    }

    SystemPalette {
        id: systemPalette
        colorGroup: SystemPalette.Active
    }


    Rectangle {
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

        IconButtonTransparent {
            id: btnfileMenu
            height: parent.height
            width: 75 * theme.scaleWidth
            icon.source: "/images/fileMenu.png"
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
            text: qsTr(aog.fixQuality ===1 ? "GPS Single":
                        aog.fixQuality ===2 ? "DGPS":
                        aog.fixQuality ===3 ? "RTK Float":
                        aog.fixQuality ===4 ? "RTK Fix":
                         "Invalid" + ": Age: "+ Math.round(aog.age, 1))
            font.bold: true
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
            IconButtonColor{
                id: rtkStatus
                icon.source: "/images/GPSQuality.png"
                implicitWidth: 75 * theme.scaleWidth
                implicitHeight: parent.height
                onClicked: {
                    gpsData.visible = !gpsData.visible
                    fieldData.visible = false
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
            IconButtonTransparent{
                height: parent.height
                width: 75 * theme.scaleWidth
                icon.source: "/images/Help.png"
            }
            IconButtonTransparent{
                height: parent.height
                icon.source: "/images/WindowMinimize.png"
                width: 75 * theme.scaleWidth
                onClicked: mainWindow.showMinimized()
            }
            IconButtonTransparent{
				id: btnMaximize
                height: parent.height
                icon.source: "/images/WindowMaximize.png"
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
            IconButtonTransparent{
                height: parent.height
                width: 75 * theme.scaleWidth
                icon.source: "/images/WindowClose.png"
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
                source: "/images/Images/z_ReverseArrow.png"
                visible: aog.isReverse
            }
            MouseArea{
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
//              // to test the reset vehicle direction button
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
            source: "/images/Images/z_NoGPS.png"
            anchors.centerIn: parent
            anchors.margins: 200
            visible: noGPS.visible
            height: parent.height /2
            width: height
        }
    }

    //----------------------------------------------------------------------------------------left column
    Item {

        id: buttonsArea
        anchors.top: parent.top
        anchors.topMargin: 2 //TODO: convert to scalable
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
			//Layout.maximumHeight: theme.buttonSize
            onVisibleChanged: if(visible == false)
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
            IconButtonText {
                id: btnnavigationSettings
                buttonText: qsTr("Display")
                icon.source: "/images/NavigationSettings.png"
                onClicked: displayButtons.visible = !displayButtons.visible
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                Layout.alignment: Qt.AlignCenter
            }
            IconButtonText {
                id: btnSettings
                buttonText: qsTr("Settings")
                icon.source: "/images/Settings48.png"
                onClicked: config.open()
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                Layout.alignment: Qt.AlignCenter
            }
            IconButtonText {
                id: btnTools
                buttonText: qsTr("Tools")
                icon.source: "/images/SpecialFunctions.png"
                onClicked: toolsMenu.visible = true
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                Layout.alignment: Qt.AlignCenter
            }
            IconButtonText{
                id: btnFieldMenu
                buttonText: qsTr("Field")
                icon.source: "/images/JobActive.png"
                onClicked: fieldMenu.visible = true
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                Layout.alignment: Qt.AlignCenter
            }
            IconButtonText{
                id: btnFieldTools
                buttonText: qsTr("Field Tools")
                icon.source: "/images/FieldTools.png"
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                onClicked: fieldTools.visible = true
                enabled: aog.isJobStarted ? true : false
                Layout.alignment: Qt.AlignCenter
            }

            IconButtonText {
                id: btnAgIO
                buttonText: qsTr("AgIO")
                icon.source: "/images/AgIO.png"
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                Layout.alignment: Qt.AlignCenter
            }
            IconButtonText {
                id: btnautoSteerConf
                buttonText: qsTr("Steer config")
                icon.source: "/images/AutoSteerConf.png"
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
            //anchors.rightMargin: 10
            anchors.margins: 10
            visible: settings.setMenu_isSpeedoOn

            speed: utils.speed_to_unit(aog.speedKph)
        }

        SteerCircle {
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
            id: rightColumn
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
			//Layout.maximumWidth: theme.buttonSize
            onVisibleChanged: if(visible == false)
                                  width = 0
                              else
                                  width = children.width

            IconButtonText {
                id: btnContour
                objectName: "btnContour"
                isChecked: false
                checkable: true
                icon.source: "/images/ContourOff.png"
                iconChecked: "/images/ContourOn.png"
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                buttonText: "Contour"
                //color: "white"
                Layout.alignment: Qt.AlignCenter
            }
            IconButtonText{
                id: btnABCurve
                isChecked: false
                checkable: true
                icon.source: "/images/CurveOff.png"
                iconChecked: "/images/CurveOn.png"
                buttonText: "ABCurve"
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
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
            IconButtonText{
                id: btnABLine
                checkable: true
                //TODO: this should be set programmatically
                //Also the types of lines are all mutually exclusive
                icon.source: "/images/ABLineOff.png"
                iconChecked: "/images/ABLineOn.png"
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                Layout.alignment: Qt.AlignCenter
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

            IconButton{
                id: btnABLineCycle
                icon.source: "/images/ABLineCycle.png"
                width: btnABLine.width
                height: btnABLine.height
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
            }
            IconButton{
                id: btnABLineCycleBk
                icon.source: "/images/ABLineCycleBk.png"
                width: btnABLine.width
                height: btnABLine.height
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
            }

            IconButtonText {
                id: btnSectionManual
                isChecked: aog.manualBtnState == 2
                checkable: true
                icon.source: "/images/ManualOff.png"
                iconChecked: "/images/ManualOn.png"
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

            IconButtonText {
                id: btnSectionAuto
                isChecked: aog.autoBtnState == 1
                checkable: true
                icon.source: "/images/SectionMasterOff.png"
                iconChecked: "/images/SectionMasterOn.png"
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
            IconButtonText {
                id: btnAutoYouTurn
                isChecked: aog.isYouTurnBtnOn
                checkable: true
                icon.source: "/images/YouTurnNo.png"
                iconChecked: "/images/YouTurn80.png"
                buttonText: "AutoUturn"
                visible: aog.isTrackOn
                enabled: aog.isAutoSteerBtnOn
                onClicked: aog.autoYouTurn()
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
            }
            IconButtonText {
                id: btnAutoSteer
                icon.source: "/images/AutoSteerOff.png"
                iconChecked: "/images/AutoSteerOn.png"
                checkable: true
                checked: aog.isAutoSteerBtnOn
                enabled: aog.isTrackOn
                //Is remote activation of autosteer enabled?
                buttonText: (settings.setAS_isAutoSteerAutoOn === true ? "R" : "M")
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                onClicked: {
                    if (checked && ((aog.currentABCurve > -1) || (aog.currentABLine > -1))) {
                        console.debug("okay to turn on autosteer button.")
                        aog.isAutoSteerBtnOn = true;
                    } else {
                        console.debug("keep autoster button off.")
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

        Column {
            id: rightSubColumn
            anchors.top: parent.top
            anchors.topMargin: btnContour.height + 3
            anchors.right: rightColumn.left
            anchors.rightMargin: 3
            spacing: 3

            IconButton {
                id: btnContourPriority
                objectName: "btnContourPriority"
                checkable: true
                isChecked: true
                visible: false
                icon.source: "/images/ContourPriorityLeft.png"
                iconChecked: "/images/ContourPriorityRight.png"
            }
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
				if (visible == false)
					height = 0
				else
					height = children.height				

			}
            ComboBox {
                id: skips
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
                    if (skips.find(currentText) === -1){
                        model.append({text: editText})
                        curentIndex = skips.find(editText)
                    }
                }
            }
            IconButtonText {
                id: btnYouSkip
                isChecked: false
                checkable: true
                icon.source: "/images/YouSkipOff.png"
                iconChecked: "/images/YouSkipOn.png"
                buttonText: "YouSkips"
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
            }
            IconButtonText {
                id: btnResetTool
                icon.source: "/images/ResetTool.png"
                buttonText: "Reset Tool"
                Layout.alignment: Qt.AlignCenter
                onClicked: aog.btnResetTool()
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
            }
            IconButtonText {
                id: btnSectionMapping
                icon.source: "/images/SectionMapping"
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
            }
            IconButtonText {
                id: btnFieldInfo
                icon.source: "/images/FieldStats.png"
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                onClicked: {
                    fieldData.visible = !fieldData.visible
                    gpsData.visible = false
                }
            }
            IconButtonText {
                id: btnTramLines
                icon.source: "/images/TramLines.png"
                buttonText: "Tram Lines"
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
            }
            IconButtonText {
                property bool isOn: false
                id: btnHydLift
                isChecked: isOn
                checkable: true
                disabled: btnHeadland.checked
                visible: utils.isTrue(settings.setArdMac_isHydEnabled) && btnHeadland.visible
                icon.source: "/images/HydraulicLiftOff.png"
                iconChecked: "/images/HydraulicLiftOn.png"
                buttonText: "HydLift"
                Layout.alignment: Qt.AlignCenter
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
                onClicked: {
                    isOn = !isOn
                    aog.isHydLiftOn(isOn)
                }
            }
            IconButtonText {
                id: btnHeadland
                isChecked: aog.isHeadlandOn
                checkable: true
                icon.source: "/images/HeadlandOff.png"
                iconChecked: "/images/HeadlandOn.png"
                buttonText: "Headland"
                Layout.alignment: Qt.AlignCenter
                onClicked: aog.btnHeadland()
				implicitWidth: theme.buttonSize
				implicitHeight: theme.buttonSize
            }
            IconButtonText {
                id: btnFlag
                objectName: "btnFlag"
                isChecked: false
                icon.source: "/images/FlagRed.png"
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

            IconButtonText {
                id: btnTrackOn
                icon.source: "/images/TrackOn.png"
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
            IconButtonTransparent{
                id: pan
                implicitWidth: 50
                implicitHeight: 50 * theme.scaleHeight
                checkable: true
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 30
                icon.source: "/images/Pan.png"
                iconChecked: "/images/SwitchOff.png"
                onClicked: aog.panMode = !aog.panMode
            }
            Image{
                id: hydLiftIndicator
                property bool isDown: aog.hydLiftDown
                visible: false
                source: "/images/Images/z_Lift.png"
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

            OutlineText{
                id: simulatorOnText
                visible: settings.setMenu_isSimulatorOn
                anchors.top: parent.top
                anchors.topMargin: lightbar.height+ 10
                anchors.horizontalCenter: lightbar.horizontalCenter
                font.pixelSize: 30
                color: "#cc5200"
                text: qsTr("Simulator On")
            }

            OutlineText{
                property int age: aog.age
                id: ageAlarm
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
                id: autoTurn
                anchors.top:manualUturnLateral.top
                anchors.right: parent.right
                anchors.rightMargin: 200
                width: 100 * theme.scaleWidth
                height: 100 * theme.scaleHeight

                 Image{
                    id: autoTurnImage
                    source: if(!aog.isYouTurnRight)
                                "/images/Images/z_TurnRight.png"
                            else
                                "/images/Images/z_TurnLeft.png"
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
                        color: colorAutoUTurn.color
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: if(aog.distancePivotToTurnLine > 0)
                                  utils.m_to_unit_string(aog.distancePivotToTurnLine, 0) + " "+utils.m_unit_abbrev()
                               else
                                  "--"
                    }
                }
            }
            Item{
                //this whole item is for the manual uturn buttons, and lateral buttons
                id: manualUturnLateral
                anchors.top: lightbar.bottom
                anchors.left: parent.left
                anchors.topMargin: 30
                anchors.leftMargin: 150
                width: childrenRect.width
                height: childrenRect.height
                visible: aog.isAutoSteerBtnOn
                MultiEffect{
                    colorizationColor: "#e6e600"
                    colorization: 1.0
                    anchors.fill: uturn
                    source: uturn
                    visible: settings.setFeature_isYouTurnOn
                    Button{
                        id: manualUturnLeft
                        background: Rectangle{color: "transparent"}
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.horizontalCenter
                        onClicked: {
                            if (settings.setAS_functionSpeedLimit > aog.speedKph)
                                aog.uturn(false)
                            else
                                timedMessage.addMessage(2000,qsTr("Too Fast"), qsTr("Slow down below") + " " +
                                                        utils.speed_to_unit_string(settings.setAS_functionSpeedLimit,1) + " " + utils.speed_unit())
                        }
                    }
                    Button{
                        id: manualUturnRight
                        background: Rectangle{color: "transparent"}
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        anchors.left: parent.horizontalCenter
                        onClicked: {
                            if (settings.setAS_functionSpeedLimit > aog.speedKph)
                                aog.uturn(true)
                            else
                                timedMessage.addMessage(2000,qsTr("Too Fast"), qsTr("Slow down below") + " " +
                                                        utils.speed_to_unit_string(settings.setAS_functionSpeedLimit,1) + " " + utils.speed_unit())
                        }
                    }
                }
                Image{
                    id: uturn
                    anchors.top: parent.top
                    height: 60 * theme.scaleHeight
                    anchors.left: parent.left
                    width: 150 * theme.scaleWidth
                    source: '/images/Images/z_TurnManual.png'
                    visible: false
                }
                MultiEffect{
                    colorizationColor: "#80aaff"
                    colorization: 1.0
                    anchors.fill: lateral
                    source: lateral
                    visible: settings.setFeature_isLateralOn
                    Button{
                        background: Rectangle{color: "transparent"}
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.horizontalCenter
                        onClicked: {
                            if (settings.setAS_functionSpeedLimit > aog.speedKph)
                                aog.lateral(false)
                            else
                                timedMessage.addMessage(2000,qsTr("Too Fast"), qsTr("Slow down below") + " " +
                                                        aog.convert_speed_text(settings.setAS_functionSpeedLimit,1) + " " + aog.speed_unit())
                        }
                    }
                    Button{
                        background: Rectangle{color: "transparent"}
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        anchors.left: parent.horizontalCenter
                        onClicked: {
                            if (settings.setAS_functionSpeedLimit > aog.speedKph)
                                aog.lateral(true)
                            else
                                timedMessage.addMessage(2000,qsTr("Too Fast"), qsTr("Slow down below") + " " +
                                                        aog.convert_speed_text(settings.setAS_functionSpeedLimit,1) + " " + aog.speed_unit())
                        }
                    }
                }

                Image{
                    visible: false
                    id: lateral
                    anchors.top: uturn.bottom
                    height: 60 * theme.scaleHeight
                    anchors.left: parent.left
                    width: 150 * theme.scaleWidth
                    source: '/images/Images/z_LateralManual.png'
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
        FieldData{
            id: fieldData
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            visible: false
        }
        GPSData{
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

        OutlineText{
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
        SectionButtons {
            id: sectionButtons
            visible: aog.isJobStarted ? true : false
            anchors.horizontalCenter: parent.horizontalCenter
			anchors.bottom: simBarRect.top
			anchors.bottomMargin: 8
			height: 40 * theme.scaleHeight
            width: 500  * theme.scaleWidth
			//onHeightChanged: anchors.bottomMargin = (bottomButtons.height + simBarRect.height + (24 * theme.scaleHeight))
        }
        DisplayButtons{
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

        TrackButtons{
            id: trackButtons
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 20
            visible: false
            z:1
        }
        IconButtonTransparent{
            id: toggleButtons
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.margins: 25
            visible: aog.isJobStarted
            width: 45 * theme.scaleWidth
            height: 25 * theme.scaleHeight
            icon.source: "/images/MenuHideShow.png"
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
            IconButton{
                implicitWidth: 30 * theme.scaleWidth
                implicitHeight: 30 * theme.scaleHeight
                radius: 0
                icon.source: "/images/ZoomIn48.png"
                onClicked: aog.zoomIn()
            }
            IconButton{
                implicitWidth: 30 * theme.scaleWidth
                implicitHeight: 30 * theme.scaleHeight
                radius: 0
                icon.source: "/images/ZoomOut48.png"
                onClicked: aog.zoomOut()
            }
        }
    }



        SliderCustomized { //quick dirty hack--the up and down buttons change this value, so the speed changes
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

        StartUp{
            id: startUp
            z:10
            //visible: true
			visible: false  //no reason to look at this until release
        }


        FieldToolsMenu {
            id: fieldTools
            visible: false
        }
        FieldMenu {
            id: fieldMenu
            objectName: "slideoutMenu"
            visible: false
        }
        ToolsWindow {
            id: toolsMenu
            visible: false
        }
        HamburgerMenu{
            id: hamburgerMenu
            visible: false
        }

        Config {
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
        SteerConfigWindow {
            id:steerConfigWindow
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
        BoundaryMenu{
            id: boundaryMenu
            visible: false
        }

        LineDrawer {
            id:lineDrawer
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            height: 768
            width:1024
            visible:false
        }
        LineNudge{
            id: lineNudge
            visible: false
        }
        RefNudge{
            id: refNudge
            visible: false
        }
        SetSimCoords{
            id: setSimCoords
            anchors.fill: parent
        }

        TrackNew{
            id: trackNew
            visible: false
        }

        Rectangle{
            id: closeDialog
            width: 500 * theme.scaleWidth
            height: 100 * theme.scaleHeight
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: aog.backgroundColor
            border.color: aog.blackDayWhiteNight
            border.width: 2
            visible: false
            IconButtonText{
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                color1: "transparent"
                color2: "transparent"
                color3: "transparent"
                icon.source: "/images/back-button.png"
                onClicked: parent.visible = false
            }
            IconButtonText{
                objectName: "btnExitAOG"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                color1: "transparent"
                color2: "transparent"
                color3: "transparent"
                icon.source: "/images/ExitAOG.png"
                onClicked: {
                    mainWindow.save_everything()
                    Qt.quit()
                }
            }
        }
        Item{
            id: windowsArea      //container for declaring all the windows
            anchors.fill: parent //that can be displayed on the main screen
            FieldFromExisting{
                id: fieldFromExisting
                x: 0
                y: 0
            }
            FieldNew{
                id: fieldNew
            }
            FieldFromKML{
                id: fieldFromKML
                x: 100
                y: 75
            }
            FieldOpen{
                id: fieldOpen
                x: 100
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

                IconButton {
                    id: redFlag
                    objectName: "btnRedFlag"
                    icon.source: "/images/FlagRed.png";
                }
                IconButton {
                    id: greenFlag
                    objectName: "btnGreenFlag"
                    icon.source: "/images/FlagGrn.png";
                }
                IconButton {
                    id: yellowFlag
                    objectName: "btnYellowFlag"
                    icon.source: "/images/FlagYel.png";
                }
                IconButton {
                    id: deleteFlag
                    objectName: "btnDeleteFlag"
                    icon.source: "/images/FlagDelete.png"
                    enabled: false
                }
                IconButton {
                    id: deleteAllFlags
                    objectName: "btnDeleteAllFlags"
                    icon.source: "/images/FlagDeleteAll.png"
                    enabled: false
                }
            }
        }
    }
			/*Item{ IMPORTANT: This does not yet work. It is the hotKey code. I'm going to commit it tho, or it will get lost.
			id: keys
			anchors.fill: parent
		focus: true
		onFocusChanged:{
			console.log("focus changed to "+focus)
			if(!focus)
			    focus = true
		}
		z: 10
		Keys.onPressed: (event)=> {

			console.log("key pressed = "+event.key)
			var hotkeys = settings.setKeys_hotkeys
			var isSectionsNotZones = settings.setTools_isSectionsNotZones

			if (event.key == hotkeys[0]) //autosteer button on off
			btnAutoSteer.clicked();

			if (event.key == hotkeys[1]) //open the steer chart
			btnABLineCycle.clicked();

			if (event.key == hotkeys[2])
			//FileSaveEverythingBeforeClosingField();
			console.log("not implemented. go to line 61 in mainwindow.qml to implement")

			if (event.key == hotkeys[3]) // Flag click
			btnFlag.clicked();

			if (event.key == hotkeys[4]) //auto section on off
			btnSectionManual.clicked();

			if (event.key == hotkeys[5]) //auto section on off
			btnSectionAuto.clicked();

			if (event.key == hotkeys[6]) // Snap/Prioritu click
			lineNudge.SnapToPivot();

			if (event.key == hotkeys[7])//snap left
			{
				/*if (trk.idx > -1) //
				 {
					 trk.NudgeTrack((double)Properties.Settings.Default.setAS_snapDistance * -0.01);
				 }
				 lineNudge.snapLeft()
			 }

			 if (event.key == hotkeys[8]) //snap rightj
			 {
				 lineNudge.snapRight()
				 console.log("not implemented. go to line 85 in mainwindow.qml to implement")
				 /* if (trk.idx > -1)
				  {
					  trk.NudgeTrack((double)Properties.Settings.Default.setAS_snapDistance * 0.01);
				  }
			  }

			  if (event.key == (hotkeys[9])) //open the vehicle Settings
			  btnautoSteerConf.clicked();

			  if (event.key == (hotkeys[10])) // Wizard
			  {
				  console.log("not implemented. go to line 102 in mainwindow.qml to implement")
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

			   /*if (event.key == Keys.OemOpenBrackets)
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
		 }*/
}
