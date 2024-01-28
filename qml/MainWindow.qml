import QtQuick 2.8
import QtGraphicalEffects 1.15
import QtQuick.Window 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import AgOpenGPS 1.0

Window {

    //We draw native opengl to this root object
    id: mainWindow
    width: 1000
    height: 700
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

        onIsJobStartedChanged: {
            console.debug("isJobStarted changed!")
        }

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
        color: "ghostwhite"
        height: 50
        visible: true
        Text {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 120
            text: qsTr("Field: "+ (aog.isJobStarted ? settings.setF_CurrentDir: "None"))
            anchors.bottom: parent.verticalCenter
            font.bold: true
            font.pixelSize: 15
        }
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
        Text {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 300
            text: qsTr("Age: ")
            font.pixelSize: 15
            font.bold: true
        }
        Text {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 150
            text: qsTr("Fixtype")
            font.bold: true
            font.pixelSize: 15
        }
        Button{
            implicitHeight: 30
            anchors.bottom: parent.bottom
            anchors.right: topRowWindow.left
            implicitWidth: 75
            background: Rectangle{
                Text {
                    text: aog.speedKph
                    font.bold: true
                    anchors.centerIn: parent
                    font.pixelSize: 35
                }
                color: parent.down ? "gray" : "ghostwhite"
            }
        }
        Row{
            id: topRowWindow
            width: childrenRect.width
            height: parent.height
            anchors.top: parent.top
            anchors.right: parent.right

            IconButtonTransparent{
                objectName: "btnHelp"
                height: parent.height
                width: 75
                icon.source: "/images/Help.png"
            }
            IconButtonTransparent{
                objectName: "btnWindowMinimize"
                height: parent.height
                icon.source: "/images/WindowMinimize.png"
                width: 75
                onClicked: mainWindow.showMinimized()
            }
            IconButtonTransparent{
                objectName: "btnWindowMaximize"
                height: parent.height
                icon.source: "/images/WindowMaximize.png"
                width: 75
                onClicked: {
                    console.debug("Visibility is " + mainWindow.visibility)
                    if (mainWindow.visibility == Window.FullScreen)
                        mainWindow.showNormal()
                    else
                        mainWindow.showFullScreen()
                }
            }
            IconButtonTransparent{
                objectName: "btnWindowClose"
                height: parent.height
                width: 75
                icon.source: "/images/WindowClose.png"
                onClicked: aog.autoBtnState + aog.manualBtnState  > 0 ? timedMessage.addMessage(2000,qsTr("Section Control on. Shut off Section Control.")):
                aog.isJobStarted ? closeDialog.visible = true:
                Qt.quit()
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

            onClicked: {
                parent.clicked(mouse);
            }

            onPressed: {
                //save a copy of the coordinates
                fromX = mouseX
                fromY = mouseY
            }

            onPositionChanged: {
                parent.dragged(fromX, fromY, mouseX, mouseY)
                fromX = mouseX
                fromY = mouseY
            }

            onWheel: {
                if (wheel.angleDelta.y > 0) {
                    parent.zoomIn()
                } else if (wheel.angleDelta.y <0 ) {
                    parent.zoomOut()
                }
            }
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
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            width: children.width + 6


            IconButtonText {
                id: btnfileMenu
                width: 75
                objectName: "btnfileMenu"
                buttonText: qsTr("FileMenu")
                icon.source: "/images/fileMenu.png"
                onClicked: fileMenu.popup()
            }
            Menu{
                id: fileMenu
                //anchors.left: buttonsArea.left
                //anchors.top: buttonsArea.top
                //anchors. margins: 200
                MenuItem{ text: "Languages"}
                MenuItem{ text: "Directories"}
                MenuItem{ text: "Colors"}
                MenuItem{ text: "Section Colors"}
                MenuItem{ text: "Top Field View"}
                MenuItem{ text: "Enter Sim Coords"}
                MenuItem{
                    text: "Simulator On"
                    checked: settings.setMenu_isSimulatorOn
                    onCheckedChanged: {
                        settings.setMenu_isSimulatorOn = checked
                        console.log(settings.setMenu_isSimulatorOn)
                    }
                }
                MenuItem{ text: "Reset All"}
                MenuItem{ text: "HotKeys"}
                MenuItem{ text: "About..."}
                MenuItem{ text: "Help"}
                closePolicy: Popup.CloseOnPressOutsideParent
            }

            IconButtonText {
                id: btnAcres
                objectName: "btnAcres"
                buttonText: qsTr("0.00")
                icon.source: "/images/TripOdometer.png"
                visible: aog.isJobStarted ? true : false

            }
            IconButtonText {
                id: btnnavigationSettings
                objectName: "btnnavigationSettings"
                buttonText: qsTr("Display")
                icon.source: "/images/NavigationSettings.png"
                onClicked: displayButtons.visible = !displayButtons.visible
            }
            IconButtonText {
                id: btnSettings
                objectName: "btnSettings"
                buttonText: qsTr("Settings")
                icon.source: "/images/Settings48.png"
                onClicked: config.open()

            }
            IconButtonText {
                id: btnTools
                objectName: "btnTools"
                buttonText: qsTr("Tools")
                icon.source: "/images/SpecialFunctions.png"
                onClicked: toolsWindow.visible = !toolsWindow.visible
            }
            IconButtonText{
                id: btnFieldMenu
                objectName: "btnFieldMenu"
                buttonText: qsTr("Field")
                icon.source: "/images/JobActive.png"
                onClicked: fieldMenu.visible = !fieldMenu.visible
            }
            IconButtonText{
                id: btnFieldTools
                objectName: "btnFieldTools"
                buttonText: qsTr("Field Tools")
                icon.source: "/images/FieldTools.png"
                onClicked: fieldTools.popup()
                visible: aog.isJobStarted ? true : false
            }
            Menu{
                id: fieldTools
                width: 300
                IconButtonTextBeside{
                    text: "Boundary"
                    icon.source: "/images/MakeBoundary.png"
                    width: 300
                    onClicked: boundaryMenu.visible = true
                }
                IconButtonTextBeside{
                    text: "Headland"
                    icon.source: "/images/HeadlandMenu.png"
                    width: 300
                }
                IconButtonTextBeside{
                    text: "Headland (Build)"
                    icon.source: "/images/Headache.png"
                    width: 300
                }
                IconButtonTextBeside{
                    text: "Tram Lines"
                    icon.source: "/images/TramLines.png"
                    width: 300
                    onClicked: tramLinesEditor.visible = true
                }
                IconButtonTextBeside{
                    text: "Recorded Path"
                    icon.source: "/images/RecPath.png"
                    width: 300
                    onClicked: recordButtons.visible = !recordButtons.visible
                }
            }

            IconButtonText {
                id: btnAgIO
                objectName: "btnAgIO"
                buttonText: qsTr("AgIO")
                icon.source: "/images/AgIO.png"
            }
            IconButtonText {
                id: btnautoSteerConf
                objectName: "btnAutosteerConf"
                buttonText: qsTr("Steer config")
                icon.source: "/images/AutoSteerConf.png"
                onClicked: {
                    steerConfigWindow.visible = true
                }
            }
        }
        //------------------------------------------------------------------------------------------right

        Speedometer {
            anchors.top: parent.top
            anchors.right: rightColumn.left
            anchors.topMargin: topLine.height + 10
            anchors.rightMargin: 10
            visible: (settings.setMenu_isSpeedoOn === true ||
                      settings.setMenu_isSpeedoOn === "true")

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

            IconButtonText {
                id: btnContour
                objectName: "btnContour"
                isChecked: false
                checkable: true
                icon.source: "/images/ContourOff.png"
                iconChecked: "/images/ContourOn.png"
                buttonText: "Contour"
                //color: "white"
                visible: aog.isJobStarted ? true : false
            }
            IconButtonText{
                id: btnABCurve
                objectName: "btnABCurve"
                isChecked: false
                checkable: true
                icon.source: "/images/CurveOff.png"
                iconChecked: "/images/CurveOn.png"
                buttonText: "ABCurve"
                visible: aog.isJobStarted ? true : false
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
                objectName: "btnABLine"
                checkable: true
                //TODO: this should be set programmatically
                //Also the types of lines are all mutually exclusive
                icon.source: "/images/ABLineOff.png"
                iconChecked: "/images/ABLineOn.png"
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
                visible: aog.isJobStarted ? true : false
            }

            IconButton{
                id: btnABLineCycle
                objectName: "btnABLineCycle"
                icon.source: "/images/ABLineCycle.png"
                width: btnABLine.width
                height: btnABLine.height
                visible: aog.isJobStarted ? true : false
            }
            IconButton{
                id: btnABLineCycleBk
                objectName: "btnABLineCycleBk"
                icon.source: "/images/ABLineCycleBk.png"
                width: btnABLine.width
                height: btnABLine.height
                visible: aog.isJobStarted ? true : false
            }

            IconButtonText {
                id: btnSectionManual
                objectName: "btnSectionManual"
                isChecked: false
                checkable: true
                icon.source: "/images/ManualOff.png"
                iconChecked: "/images/ManualOn.png"
                buttonText: "Manual"
                visible: aog.isJobStarted ? true : false
                onCheckedChanged: {
                    if (checked) {
                        btnSectionAuto.checked = false;
                        sectionButtons.setAllSectionsToState(2 /*auto*/);
                    } else {
                        sectionButtons.setAllSectionsToState(0 /*off*/);
                    }
                }
            }

            IconButtonText {
                id: btnSectionAuto
                objectName: "btnSectionAuto"
                isChecked: false
                checkable: true
                icon.source: "/images/SectionMasterOff.png"
                iconChecked: "/images/SectionMasterOn.png"
                buttonText: "Auto"
                visible: aog.isJobStarted ? true : false
                onCheckedChanged: {
                    if (checked) {
                        btnSectionManual.checked = false;
                        sectionButtons.setAllSectionsToState(1 /*auto*/);
                    } else {
                        sectionButtons.setAllSectionsToState(0 /*off*/);
                    }
                }
            }
            IconButtonText {
                id: btnAutoYouTurn
                objectName: "btnAutoYouTurn"
                isChecked: false
                checkable: true
                icon.source: "/images/YouTurnNo.png"
                iconChecked: "/images/YouTurn80.png"
                buttonText: "AutoUturn"
            }
            IconButtonText {
                id: btnAutoSteer
                objectName: "btnAutoSteer"
                icon.source: "/images/AutoSteerOff.png"
                iconChecked: "/images/AutoSteerOn.png"
                checkable: true
                checked: aog.isAutoSteerBtnOn
                visible: aog.isJobStarted ? true : false
                //Is remote activation of autosteer enabled?
                buttonText: (settings.setAS_isAutoSteerAutoOn === true ? "R" : "M")
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
            anchors.left: leftColumn.right
            anchors.leftMargin: 3
            anchors.right: rightColumn.left
            anchors.rightMargin: 3
            Layout.fillWidth: true
            //spacing: parent.rowSpacing
            IconButtonText {
                id: btnResetTool
                objectName: "btnResetTool"
                icon.source: "/images/ResetTool.png"
                buttonText: "Reset Tool"
            }
            IconButtonText {
                id: btnHeadland
                objectName: "btnHeadland"
                isChecked: false
                checkable: true
                icon.source: "/images/HeadlandOff.png"
                iconChecked: "/images/HeadlandOn.png"
                buttonText: "Headland"
            }
            IconButtonText {
                id: btnHydLift
                objectName: "btnHydLift"
                isChecked: false
                checkable: true
                icon.source: "/images/HydraulicLiftOff.png"
                iconChecked: "/images/HydraulicLiftOn.png"
                buttonText: "HydLift"
                onClicked: warningWindow.visible = true
            }
            IconButtonText {
                id: btnFlag
                objectName: "btnFlag"
                isChecked: false
                icon.source: "/images/FlagRed.png"
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
                id: btnTramLines
                objectName: "btnTramLines"
                icon.source: "/images/TramLines.png"
                buttonText: "Tram Lines"
            }
            IconButtonText {
                id: btnSectionMapping
                objectName: "btnSectionMapping"
                icon.source: "/images/SectionMapping"
                visible: aog.isJobStarted ? true : false
            }

            IconButtonText {
                id: btnPointStart
                objectName: "btnPointStart"
                icon.source: "/images/PointStart.png"
                buttonText: "LinePicker"
                onClicked: lineDrawer.visible = true
            }
            IconButtonText {
                id: btnABLineEdit
                objectName: "btnABLineEdit"
                icon.source: "/images/ABLineEdit.png"
                buttonText: "ABLineEdit"
                onClicked: lineEditor.visible = true
            }
            IconButtonText {
                id: btnYouSkip
                objectName: "btnYouSkip"
                isChecked: false
                checkable: true
                icon.source: "/images/YouSkipOff.png"
                iconChecked: "/images/YouSkipOn.png"
                buttonText: "YouSkips"
            }
            ComboBox {
                id: skips
                editable: true
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
                implicitHeight:parent.height
                implicitWidth: btnYouSkip.width
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
            Item{
                id: autoTurn
                anchors.top:parent.top
                anchors.left: parent.left
                width: 100
                height: 100
                visible: false
                Image {
                    id: autoTurnImage
                    mirror: false
                    signal clicked(var mouse)
                    source: "/images/Images/z_Turn.png"
                    visible: true
                    anchors.fill: parent
                }
            }
            Item{
                objectName: "manUTurnButtons"
                id: manualUturnLateral
                anchors.top: lightbar.bottom
                anchors.left: parent.left
                anchors.topMargin: 30
                anchors.leftMargin: 150
                width: childrenRect.width
                height: childrenRect.height
                visible: (aog.currentABCurve > -1) || (aog.currentABLine > -1)
                ColorOverlay{
                    color: "#e6e600"
                    anchors.fill: uturn
                    source: uturn

                    Button{
                        background: Rectangle{color: "transparent"}
                        objectName: "btnManUturnLeft"
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.horizontalCenter
                        onClicked: {
                            if (settings.setAS_functionSpeedLimit > aog.speedKph)
                                aog.uturn(false)
                            else
                                timedMessage.addMessage(2000,qsTr("Too Fast"), qsTr("Slow down below") + " " +
                                                        utils.speed_to_unit_text(settings.setAS_functionSpeedLimit,1) + " " + utils.speed_unit())
                        }
                    }
                    Button{
                        background: Rectangle{color: "transparent"}
                        objectName: "btnManUturnRight"
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        anchors.left: parent.horizontalCenter
                        onClicked: {
                            if (settings.setAS_functionSpeedLimit > aog.speedKph)
                                aog.uturn(true)
                            else
                                timedMessage.addMessage(2000,qsTr("Too Fast"), qsTr("Slow down below") + " " +
                                                        utils.speed_to_unit_text(settings.setAS_functionSpeedLimit,1) + " " + utils.speed_unit())
                        }
                    }
                }
                Image{
                    id: uturn
                    anchors.top: parent.top
                    height: 60
                    anchors.left: parent.left
                    width: 150
                    source: '/images/Images/z_TurnManual.png'
                    visible: false
                }
                ColorOverlay{
                    color: "#80aaff"
                    anchors.fill: lateral
                    source: lateral
                }

                Image{
                    visible: false
                    id: lateral
                    anchors.top: uturn.bottom
                    height: 60
                    anchors.left: parent.left
                    width: 150
                    source: '/images/Images/z_LateralManual.png'
                    Button{
                        background: Rectangle{color: "transparent"}
                        objectName: "btnManLateralLeft"
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
                        objectName: "btnManLateralRight"
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        anchors.left: parent.horizontalCenter
                        onClicked: {
                            if (settings.setAS_functionSpeedLimit > aog.speedKph)
                                aog.lateral(false)
                            else
                                timedMessage.addMessage(2000,qsTr("Too Fast"), qsTr("Slow down below") + " " +
                                                        aog.convert_speed_text(settings.setAS_functionSpeedLimit,1) + " " + aog.speed_unit())
                        }
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

        FieldData{
            id: fieldData
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            visible: true
        }
        GPSData{
            id: gpsData
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            visible: false
        }

        SimController{
            id: simBarRect
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 10
            visible: utils.isTrue(settings.setMenu_isSimulatorOn)
        }
        SectionButtons {
            id: sectionButtons
            visible: aog.isJobStarted ? true : false
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: simBarRect.top
            anchors.bottomMargin: 15
            //TODO: add logic to interact with the Auto and manual buttons

            //only allow on and off when auto is on. TODO what about when Manual is on?
            //triState: btnSectionAuto.checked ? false : true
            width: 500
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
    }



        SliderCustomized { //quick dirty hack--the up and down buttons change this value, so the speed changes
            id: speedSlider
            objectName: "simSpeed"
            //anchors.bottom: bottomButtons.top
//            anchors.bottomMargin: 3
//            anchors.left:bottomButtons.left
//            anchors.leftMargin: 3
            from: -30
            to: 30
            value: 0
            visible: false
        }

        StartUp{
            id: startUp
            z:10
            visible: true
        }


        FieldMenu {
            id: fieldMenu
            objectName: "slideoutMenu"
            height: 700
            width:1024
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.leftMargin: 15
            visible: false

        }
        FieldOpen {
            id: fieldOpen
            objectName: "slideoutMenu"
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.left: leftColumn.right
            anchors.leftMargin: 15
            visible: false

        }
        ToolsWindow {
            id: toolsWindow
            objectName: "slideoutMenu"
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.left: leftColumn.right
            anchors.leftMargin: 15
            visible: false

        }
        Config {
            id:config
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
        SteerConfigWindow {
            id:steerConfigWindow
            objectName: "steerConfigWindow"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            visible:false
            height: 600
            width:450
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
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.rightMargin: 150
            anchors.topMargin: 50
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

        Rectangle{
            id: recordButtons
            anchors.bottom: bottomButtons.top
            anchors.bottomMargin: 50
            anchors.left: leftColumn.right
            anchors.leftMargin: 50
            width: childrenRect.width
            height: children.height
            visible: false
            Column{
                width: children.width
                height: children.height
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                IconButtonTransparent{
                    objectName: "recordPlay"
                    icon.source: "/images/Play.png"
                    iconChecked: "/images/Stop.png"
                }
                IconButtonTransparent{
                    objectName: "recordStartPoint"
                    icon.source: "/images/pathResumeClose.png"
                }
                IconButtonTransparent{
                    objectName: "recordRecord"
                    icon.source: "/images/BoundaryRecord.png"
                }
                IconButtonTransparent{
                    objectName: "recordOpenFile"
                    icon.source: "/images/FileOpen.png"
                }
            }
        }

        Rectangle{
            id: closeDialog
            width: 500
            height: 100
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            color: "gray"
            border.color: "aqua"
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
                onClicked: Qt.quit()
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
}
