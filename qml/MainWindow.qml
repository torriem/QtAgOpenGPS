import QtQuick 2.8
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

    SystemPalette {
        id: systemPalette
        colorGroup: SystemPalette.Active
    }

    Rectangle {
        id: topLine
        objectName: "topLine"
        width: parent.width
        height: 50 //.3" tall
        Text {
            id: text1
            text: ""
            color: systemPalette.windowText
            anchors.horizontalCenter: parent.horizontalCenter

        }
        color: systemPalette.window
        anchors.top: parent.top
    }

    Rectangle {
        id: background
        objectName: "background"
        anchors.top: topLine.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom:  statusBar.top

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


    AOGRenderer {
        id: glcontrolrect
        objectName: "openglcontrol"

        anchors.top: topLine.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom:  statusBar.top

        signal clicked(var mouse)

        MouseArea {
            id: mainMouseArea
            anchors.fill: parent

            onClicked: {
                parent.clicked(mouse);
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

            //top Row
            Rectangle{
                id: topRow
                anchors.bottom: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                color: "ghostwhite"
                height: 50
                visible: true
                Text {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: 120
                    text: qsTr("Field: ")
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
                            text: qsTr("0")
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
                        icon.source: "qrc:/images/Help.png"
                    }
                    IconButtonTransparent{
                        objectName: "btnWindowMinimize"
                        height: parent.height
                        icon.source: "qrc:/images/WindowMinimize.png"
                        width: 75
                    }
                    IconButtonTransparent{
                        objectName: "btnWindowMaximize"
                        height: parent.height
                        icon.source: "qrc:/images/WindowMaximize.png"
                        width: 75
                    }
                    IconButtonTransparent{
                        objectName: "btnWindowClose"
                        height: parent.height
                        width: 75
                        icon.source: "qrc:/images/WindowClose.png"
                    }
                }
            }
            ColumnLayout {
                id: leftColumn
                anchors.top: parent.top
                anchors.topMargin: -55
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: children.width + 6


                IconButtonText {
                    id: btnfileMenu
                    width: 75
                    objectName: "btnfileMenu"
                    buttonText: qsTr("FileMenu")
                    icon.source: "qrc:/images/fileMenu.png"
                    onClicked: fileMenu.popup()
                }
                        Menu{
                        id: fileMenu
                        MenuItem{ text: "Languages"}
                        MenuItem{ text: "Directories"}
                        MenuItem{ text: "Colors"}
                        MenuItem{ text: "Section Colors"}
                        MenuItem{ text: "Top Field View"}
                        MenuItem{ text: "Enter Sim Coords"}
                        MenuItem{ text: "Simulator On"}
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
                    icon.source: "qrc:/images/TripOdometer.png"

                }
                IconButtonText {
                    id: btnnavigationSettings
                    objectName: "btnnavigationSettings"
                    buttonText: qsTr("Display")
                    icon.source: "qrc:/images/NavigationSettings.png"
                    property bool hideButtons: true

                    onHideButtonsChanged: {
                        if (hideButtons == true) {
                            displayButtons.visible = false
                        }else{
                            displayButtons.visible = true
                        }
                    }

                    function toggle_displaybuttons(){
                        if (hideButtons == true) {
                            hideButtons = false
                        }else{
                            hideButtons = true
                        }
                    }
                    onClicked: {
                        toggle_displaybuttons()
                    }
                }
                IconButtonText {
                    id: btnSettings
                    objectName: "btnSettings"
                    buttonText: qsTr("Settings")
                    icon.source: "qrc:/images/Settings48.png"
                    onClicked: config.visible = true

                }
                IconButtonText {
                    id: btnTools
                    objectName: "btnTools"
                    buttonText: qsTr("Tools")
                    icon.source: "qrc:/images/SpecialFunctions.png"
                    //pseudo state
                    property bool hideTools: true

                    onHideToolsChanged: {
                        if (hideTools == true) {
                            toolsWindow.visible = false
                        } else {
                            toolsWindow.visible = true
                        }
                    }

                    function toggle_toolsmenu() {
                        if (hideTools == true) {
                            hideTools = false
                        } else {
                            hideTools = true
                        }
                    }

                    onClicked: {
                        toggle_toolsmenu();
                    }
                }
                IconButtonText{
                    id: btnFieldMenu
                    objectName: "btnFieldMenu"
                    buttonText: qsTr("Field")
                    icon.source: "qrc:/images/JobActive.png"
                    //pseudo state
                    property bool hideFieldMenu: true

                    onHideFieldMenuChanged: {
                        if (hideFieldMenu == true) {
                            fieldMenu.visible = false
                        } else {
                            fieldMenu.visible = true
                        }
                    }

                    function toggle_fieldmenu() {
                        if (hideFieldMenu == true) {
                            hideFieldMenu = false
                        } else {
                            hideFieldMenu = true
                        }
                    }

                    onClicked: {
                        toggle_fieldmenu();
                    }
                }
                IconButtonText{
                    id: btnFieldTools
                    objectName: "btnFieldTools"
                    buttonText: qsTr("Field Tools")
                    icon.source: "qrc:/images/FieldTools.png"
                    onClicked: fieldTools.popup()
                }
                Menu{
                    id: fieldTools
                    width: 300
                    IconButtonTextBeside{
                        text: "Boundary"
                        icon.source: "qrc:/images/MakeBoundary.png"
                        width: 300
                    }
                    IconButtonTextBeside{
                        text: "Headland"
                        icon.source: "qrc:/images/HeadlandMenu.png"
                        width: 300
                    }
                    IconButtonTextBeside{
                        text: "Headland (Build)"
                        icon.source: "qrc:/images/Headache.png"
                        width: 300
                    }
                    IconButtonTextBeside{
                        text: "Tram Lines"
                        icon.source: "qrc:/images/TramLines.png"
                        width: 300
                        onClicked: tramLinesEditor.visible = true
                    }
                    IconButtonTextBeside{
                        text: "Recorded Path"
                        icon.source: "qrc:/images/RecPath.png"
                        width: 300
                    }
                }

                IconButtonText {
                    id: btnAgIO
                    objectName: "btnAgIO"
                    buttonText: qsTr("AgIO")
                    icon.source: "qrc:/images/AgIO.png"
                }
                IconButtonText {
                    id: btnautoSteerConf
                    objectName: "btnAutosteerConf"
                    buttonText: qsTr("Steer config")
                    icon.source: "qrc:/images/AutoSteerConf.png"
                    /*property bool hideSteerMenu: true

                    onHideSteerMenuChanged: {
                        if (hideSteerMenu == true) {
                            steerConfigWindow.visible = false
                        } else {
                            steerConfigWindow.visible = true
                        }
                    }

                    function toggle_steermenu() {
                        if (hideSteerMenu == true) {
                            hideSteerMenu = false
                        } else {
                            hideSteerMenu = true
                        }
                    }*/

                    onClicked: {
                            steerConfigWindow.visible = true

                        //toggle_steermenu();
                    }
                }
            }
            //------------------------------------------------------------------------------------------right

            ColumnLayout {
                id: rightColumn
                anchors.top: topRow.bottom
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.rightMargin: 6

                IconButtonText {
                    id: btnContour
                    objectName: "btnContour"
                    isChecked: false
                    checkable: true
                    icon.source: "qrc:/images/ContourOff.png"
                    iconChecked: "qrc:/images/ContourOn.png"
                    buttonText: "Contour"
                    //color: "white"
                }
                IconButtonText{
                    id: btnABCurve
                    objectName: "btnABCurve"
                    isChecked: false
                    checkable: true
                    icon.source: "qrc:/images/CurveOff.png"
                    iconChecked: "qrc:/images/CurveOn.png"
                    buttonText: "ABCurve"
                    onClicked: abCurvePicker.visible = true
                }
                IconButtonText{
                    id: btnABLine
                    objectName: "btnABLine"
                    checkable: true
                    //TODO: this should be set programmatically
                    //Also the types of lines are all mutually exclusive
                    checked: true
                    icon.source: "qrc:/images/ABLineOff.png"
                    iconChecked: "qrc:/images/ABLineOn.png"
                    onClicked: abLinePicker.visible = true
                    buttonText: "ABLine"
                }

                IconButton{
                    id: btnABLineCycle
                    objectName: "btnABLineCycle"
                    icon.source: "qrc:/images/ABLineCycle.png"
                    width: btnABLine.width
                    height: btnABLine.height
                }
                IconButton{
                    id: btnABLineCycleBk
                    objectName: "btnABLineCycleBk"
                    icon.source: "qrc:/images/ABLineCycleBk.png"
                    width: btnABLine.width
                    height: btnABLine.height
                }

                IconButtonText {
                    id: btnManualOffOn
                    objectName: "btnManualOffOn"
                    isChecked: false
                    checkable: true
                    icon.source: "qrc:/images/ManualOff.png"
                    iconChecked: "qrc:/images/ManualOn.png"
                    buttonText: "Manual"
                }

                IconButtonText {
                    id: btnSectionOffAutoOn
                    objectName: "btnSectionOffAutoOn"
                    isChecked: false
                    checkable: true
                    icon.source: "qrc:/images/SectionMasterOff.png"
                    iconChecked: "qrc:/images/SectionMasterOn.png"
                    buttonText: "Auto"
                }
                IconButtonText {
                    id: btnAutoYouTurn
                    objectName: "btnAutoYouTurn"
                    isChecked: false
                    checkable: true
                    icon.source: "qrc:/images/YouTurnNo.png"
                    iconChecked: "qrc:/images/YouTurn80.png"
                    buttonText: "AutoUturn"
                }
                IconButtonText {
                    id: btnAutoSteer
                    objectName: "btnAutoSteer"
                    icon.source: "qrc:/images/AutoSteerOff.png"
                    buttonText: "X"
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
                    icon.source: "qrc:/images/ContourPriorityLeft.png"
                    iconChecked: "qrc:/images/ContourPriorityRight.png"
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
                    icon.source: "qrc:/images/ResetTool.png"
                    buttonText: "Reset Tool"
                }
                IconButtonText {
                    id: btnHeadland
                    objectName: "btnHeadland"
                    isChecked: false
                    checkable: true
                    icon.source: "qrc:/images/HeadlandOff.png"
                    iconChecked: "qrc:/images/HeadlandOn.png"
                    buttonText: "Headland"
                }
                IconButtonText {
                    id: btnHydLift
                    objectName: "btnHydLift"
                    isChecked: false
                    checkable: true
                    icon.source: "qrc:/images/HydraulicLiftOff.png"
                    iconChecked: "qrc:/images/HydraulicLiftOn.png"
                    buttonText: "HydLift"
                }
                IconButtonText {
                    id: btnFlag
                    objectName: "btnFlag"
                    isChecked: false
                    icon.source: "qrc:/images/FlagRed.png"
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
                    icon.source: "qrc:/images/TramLines.png"
                    buttonText: "Tram Lines"
                }
                IconButtonText {
                    id: btnSectionMapping
                    Layout.alignment: parent.Center
                    objectName: "btnSectionMapping"
                    icon.source: "qrc:/images/SectionMapping"
                }

                IconButtonText {
                    id: btnPointStart
                    objectName: "btnPointStart"
                    icon.source: "qrc:/images/PointStart.png"
                    buttonText: "LinePicker"
                }
                IconButtonText {
                    id: btnSnaptoPivot
                    objectName: "btnSnaptoPivot"
                    icon.source: "qrc:/images/SnapToPivot.png"
                    buttonText: ""
                }
                IconButtonText {
                    id: btnABLineEdit
                    objectName: "btnABLineEdit"
                    icon.source: "qrc:/images/ABLineEdit.png"
                    buttonText: "ABLineEdit"
                }
                IconButtonText {
                    id: btnYouSkip
                    objectName: "btnYouSkip"
                    isChecked: false
                    checkable: true
                    icon.source: "qrc:/images/YouSkipOff.png"
                    iconChecked: "qrc:/images/YouSkipOn.png"
                    buttonText: "YouSkips"
                }
                ComboBox { //if someone knows how to do this right--size and/or model, do a pull request or tell me plz
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

            Row {
                id: sectionButtons
                anchors.horizontalCenter: parent.horizontalCenter

                anchors.bottom: bottomButtons.top
                anchors.bottomMargin:10

                spacing: 15

                SectionButton {
                    id: btnSection1Man
                    objectName: "section0"
                    state: "on"
                }

                SectionButton {
                    id: btnSection2Man
                    objectName: "section1"
                    state: "auto"
                }

                SectionButton {
                    id: btnSection3Man
                    objectName: "section2"
                }

                SectionButton {
                    id: btnSection4Man
                    objectName: "section3"
                }

                SectionButton {
                    id: btnSection5Man
                    visible: false
                    objectName: "section4"
                }

                SectionButton {
                    id: btnSection6Man
                    visible: false
                    objectName: "section5"
                }

                SectionButton {
                    id: btnSection7Man
                    visible: false
                    objectName: "section6"
                }

                SectionButton {
                    id: btnSection8Man
                    visible: false
                    objectName: "section7"
                }

                SectionButton {
                    id: btnSection9Man
                    visible: false
                    objectName: "section8"
                }

                SectionButton {
                    id: btnSection10Man
                    visible: false
                    objectName: "section9"
                }

                SectionButton {
                    id: btnSection11Man
                    visible: false
                    objectName: "section10"
                }

                SectionButton {
                    id: btnSection12Man
                    visible: false
                    objectName: "section11"
                }

                SectionButton {
                    id: btnSection13Man
                    visible: false
                    objectName: "section12"
                }

                SectionButton {
                    id: btnSection14Man
                    visible: false
                    objectName: "section13"
                }

                SectionButton {
                    id: btnSection15Man
                    visible: false
                    objectName: "section14"
                }

                SectionButton {
                    id: btnSection16Man
                    visible: false
                    objectName: "section15"
                }

                SectionButton {
                    id: btnSection17Man
                    visible: false
                    objectName: "section16"
                }

            }
            Rectangle{
                id: displayButtons
                width: childrenRect.width + 10
                height: childrenRect.height + 10
                anchors.left: leftColumn.right
                anchors.leftMargin: 20
                anchors.top: parent.top
                anchors.topMargin: 20
                color: "white"
                visible: false
                z:1
                Grid {
                    id: tiltButtons
                    anchors.leftMargin: 5
                    anchors.topMargin: 5
                    spacing: 6
                    flow: Grid.TopToBottom
                    rows:6
                    columns:2

                    IconButton {
                        id: btnTiltDown
                        objectName: "btnTiltDown"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "qrc:/images/TiltDown.png"
                    }
                    IconButton {
                        id: btnCamera2d
                        objectName: "btnCamera2d"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "qrc:/images/Camera2D64.png"
                    }
                    IconButton {
                        id: btnCameraNorth2d
                        objectName: "btnCameraNorth2d"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "qrc:/images/CameraNorth2D.png"
                    }
                    IconButton {
                        id: btnZoomOut
                        objectName: "btnZoomOut"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "qrc:/images/ZoomOut48.png"
                    }
                    IconButton {
                        id: btnWindowDayNight
                        objectName: "btnWindowDayNight"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "qrc:/images/WindowDayMode.png"
                    }
                    IconButton {
                        id: btnBrightnessDown
                        objectName: "btnBrightnessDown"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "qrc:/images/BrightnessDn.png"
                    }
                    IconButton {
                        id: btnTiltUp
                        objectName: "btnTiltUp"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "qrc:/images/TiltUp.png"
                    }
                    IconButton {
                        id: btnCamera3d
                        objectName: "btnCamera3d"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "qrc:/images/Camera3D64.png"
                    }
                    IconButton {
                        id: btnCameraNorth3d
                        objectName: "btnCameraNorth3d"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "qrc:/images/CameraNorth64.png"
                    }
                    IconButton {
                        id: btnZoomIn
                        objectName: "btnZoomIn"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "qrc:/images/ZoomIn48.png"
                    }
                    IconButton {
                        id: btnempty
                        objectName: "btnempty"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "qrc:/images/.png"
                    }
                    IconButton {
                        id: btnBrightnessUp
                        objectName: "btnBrightnessUp"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "qrc:/images/BrightnessUp.png"
                    }/*
                    IconButton {
                        id: btn
                        objectName: "btn"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "qrc:/images/.png"
                    }*/


                }
            }

            Row {
                id: manTurnButtons
                visible: true
                objectName: "manUTurnButtons"
                anchors.left: leftColumn.right
                anchors.leftMargin: 15
                IconButtonTransparent {
                    id: manUturnBtnLeft
                    objectName: "btnManUturnLeft"
                    width: 58
                    height: 58
                    icon.source: "qrc:/images/TurnManualLeft.png"
                }
                IconButtonTransparent {
                    id: manUturnBtnRight
                    objectName: "btnManUturnRight"
                    width: 58
                    height: 58
                    icon.source: "qrc:/images/TurnManualRight.png"
                }
                anchors.top: parent.top;
                anchors.topMargin: 15
                spacing: 6
            }

            /*
            Column {
                id: zoomButtons
                spacing: 6
                anchors.right: rightColumn.left
                anchors.rightMargin: 20
                anchors.top: parent.top
                anchors.topMargin: 20

            }
            */
            /*
            Rectangle {
                width: 104
                height: 104
                color: "#00000000"
                Image {
                    id: compassPic
                    source: "qrc:/images/textures/Compass.png"
                    transform: Rotation {
                        origin.x: 52
                        origin.y: 52
                        angle: 45
                    }
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                }
                anchors.right: zoomButtons.left
                anchors.top: zoomButtons.top
            }
            */

            SliderCustomized {
                id: speedSlider
                objectName: "simSpeed"
                anchors.bottom: bottomButtons.top
                anchors.bottomMargin: 3
                anchors.left:bottomButtons.left
                anchors.leftMargin: 3
                from: -30
                to: 30
                value: 5
            }

            IconButton {
                id: simStopButton
                objectName: "simStopButton"
                icon.source: "qrc:/images/Stop.png"
                width: 50
                height: 50
                anchors.left: speedSlider.right
                anchors.bottom: speedSlider.bottom
                onClicked: {
                    speedSlider.value = 0;
                }

            }

            SliderCustomized {
                id: steerSlider
                objectName: "simSteer"
                anchors.bottom: simStopButton.top
                anchors.leftMargin: 3
                anchors.left: bottomButtons.left
                multiplicationValue: 10
                from: 0
                to: 600
                value: 300
            }

            IconButton {
                id: centerSteerbutton
                objectName: "simSteerCenter"
                width: 50
                height: 50
                anchors.left: steerSlider.right
                anchors.leftMargin: 50
                anchors.bottom: steerSlider.bottom
                icon.source: "qrc:/images/AutoSteerOn.png"
                onClicked: {
                    steerSlider.value = 300;
                }

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
            FieldWindow {
                id: fieldWindow
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
                objectName: "config"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                height: 768
                width:1024
                visible:false
            }
            SteerConfigWindow {
                id:steerConfigWindow
                objectName: "steerConfigWindow"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                height: 768
                width:1024
                visible:false
            }
            ABCurvePicker{
                id: abCurvePicker
                objectName: "abCurvePicker"
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                visible: false
            }
            ABLinePicker{
                id: abLinePicker
                objectName: "abLinePicker"
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
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


            Rectangle {
                id: contextArea
                objectName: "contextArea"
                width: childrenRect.width+10
                height: childrenRect.height + 10
                color: "#bf163814"
                visible: false
                anchors.top: parent.top
                anchors.left: parent.left
                //anchors.leftMargin: btnPerimeter.width + 10
                //anchors.topMargin: btnPerimeter.y
                border.color: "#c3ecc0"

                Grid {
                    id: contextAreaGrid
                    spacing: 5
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    anchors.leftMargin: 5

                    width: childrenRect.width
                    height: childrenRect.height

                    IconButton {
                        id: areaSideBtn
                        objectName: "btnAreaSide"
                        icon.source: "qrc:/images/AreaSide.png";
                    }
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
                        icon.source: "qrc:/images/FlagRed.png";
                    }
                    IconButton {
                        id: greenFlag
                        objectName: "btnGreenFlag"
                        icon.source: "qrc:/images/FlagGrn.png";
                    }
                    IconButton {
                        id: yellowFlag
                        objectName: "btnYellowFlag"
                        icon.source: "qrc:/images/FlagYel.png";
                    }
                    IconButton {
                        id: deleteFlag
                        objectName: "btnDeleteFlag"
                        icon.source: "qrc:/images/FlagDelete.png"
                        enabled: false
                    }
                    IconButton {
                        id: deleteAllFlags
                        objectName: "btnDeleteAllFlags"
                        icon.source: "qrc:/images/FlagDeleteAll.png"
                        enabled: false
                    }
                }
            }
        }

    }

    Rectangle {
        id: statusBar
        //height: screenPixelDensity * 0.3
        height: screenPixelDensity * 0
        visible: false
        color: systemPalette.window
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        Row {
            spacing: 15
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: parent.height / 4

            Text {
                id: stripHz
                objectName: "stripHz"
                text: qsTr("Hz")
                font.pixelSize: parent.height - 10;
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: stripDistance
                objectName: "stripDistance"
                text: "55 ft"
                font.pixelSize: parent.height - 10;
                anchors.verticalCenter: parent.verticalCenter
            }

            Item {
                id: toolStripBtnResetDistance
                objectName: "toolStripBtnResetDistance"

                width: 10
                height: parent.height - 10
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: stripAreaUser
                objectName: "stripAreaUser"
                text: qsTr("188.88")
                font.pixelSize: parent.height - 10;
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: stripEqWidth
                objectName: "stripEqWidth"
                text: "Width"
                font.pixelSize: parent.height - 10;
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                id: stripBoundaryArea
                objectName: "stripBoundaryArea"
                text: "Area"
                font.pixelSize: parent.height - 10;
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                id: stripAreaRate
                objectName: "stripAreaRate"
                text: "Ac/hr"
                font.pixelSize: parent.height - 10;
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Row {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.rightMargin: parent.height / 4

            Text {
                id: stripPortGPS
                objectName: "stripPortGPS"
                text: "* *"
                font.pixelSize: parent.height - 10;
                verticalAlignment: Text.AlignVCenter
                color: "red"
                anchors.verticalCenter: parent.verticalCenter
            }
            PortLight {
                id: stripOnlineGPS
                objectName: "stripOnlineGPS"
                height: parent.height - 10
                width: (parent.height - 10) / 2
                color: "grey"
                radius: (parent.height-10) / 10
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                id: stripPortArduino
                objectName: "stripPortArduino"
                text: "* *"
                font.pixelSize: parent.height - 10;
                verticalAlignment: Text.AlignVCenter
                color: "red"
                anchors.verticalCenter: parent.verticalCenter
            }
            PortLight {
                id: stripOnlineArduino
                objectName: "stripOnlineArduino"
                height: parent.height - 10
                width: (parent.height - 10) / 2
                color: "grey"
                radius: (parent.height-10) / 10
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                id: stripPortAutoSteer
                objectName: "stripPortAutoSteer"
                text: "* *"
                font.pixelSize: parent.height - 10;
                verticalAlignment: Text.AlignVCenter
                color: "red"
                anchors.verticalCenter: parent.verticalCenter
            }
            PortLight {
                id: stripOnlineAutoSteer
                objectName: "stripOnlineAutoSteer"
                height: parent.height - 10
                width: (parent.height - 10) / 2
                color: "grey"
                radius: (parent.height-10) / 10
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

}
