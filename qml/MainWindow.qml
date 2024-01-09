import QtQuick 2.8
import QtGraphicalEffects 1.15
import QtQuick.Window 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import AgOpenGPS 1.0

Window {
    function warningWindowJS(text){
        warningWindow.visible = true;
        warningWindow.text = text;
        console.log("Warning sent to screen ", text);
     /*   timer = new Timer();
        timer.interval = 1000;
        timer.repeat = false;
        timer.triggered.connect(function () {
            warningWindow.visible = false;
            console.log("triggered");
        })
        timer.start();*/
    }

    //We draw native opengl to this root object
    id: mainWindow
    width: 1000
    height: 700
    //objectName: "openGLControl"
    //width:800
    //height:600
    //anchors.fill: parent

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
                        icon.source: "/images/Help.png"
                        onClicked: warningWindowJS("yo")
                    }
                    IconButtonTransparent{
                        objectName: "btnWindowMinimize"
                        height: parent.height
                        icon.source: "/images/WindowMinimize.png"
                        width: 75
                    }
                    IconButtonTransparent{
                        objectName: "btnWindowMaximize"
                        height: parent.height
                        icon.source: "/images/WindowMaximize.png"
                        width: 75
                    }
                    IconButtonTransparent{
                        objectName: "btnWindowClose"
                        height: parent.height
                        width: 75
                        icon.source: "/images/WindowClose.png"
                        onClicked: closeDialog.visible = true
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
                    icon.source: "/images/fileMenu.png"
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
                    icon.source: "/images/TripOdometer.png"

                }
                IconButtonText {
                    id: btnnavigationSettings
                    objectName: "btnnavigationSettings"
                    buttonText: qsTr("Display")
                    icon.source: "/images/NavigationSettings.png"
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
                    icon.source: "/images/Settings48.png"
                    onClicked: config.visible = true

                }
                IconButtonText {
                    id: btnTools
                    objectName: "btnTools"
                    buttonText: qsTr("Tools")
                    icon.source: "/images/SpecialFunctions.png"
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
                    icon.source: "/images/JobActive.png"
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
                    icon.source: "/images/FieldTools.png"
                    onClicked: fieldTools.popup()
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
                    //pseudo state
                    property bool hideRecordMenu: true

                    onHideRecordMenuChanged: {
                        if (hideRecordMenu == true) {
                            recordButtons.visible = false
                        } else {
                            recordButtons.visible = true
                        }
                    }

                    function toggle_recordmenu() {
                        if (hideRecordMenu == true) {
                            hideRecordMenu = false
                        } else {
                            hideRecordMenu = true
                        }
                    }

                    onClicked: {
                        toggle_recordmenu();
                    }

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
                    icon.source: "/images/ContourOff.png"
                    iconChecked: "/images/ContourOn.png"
                    buttonText: "Contour"
                    //color: "white"
                }
                IconButtonText{
                    id: btnABCurve
                    objectName: "btnABCurve"
                    isChecked: false
                    checkable: true
                    icon.source: "/images/CurveOff.png"
                    iconChecked: "/images/CurveOn.png"
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
                    icon.source: "/images/ABLineOff.png"
                    iconChecked: "/images/ABLineOn.png"
                    onClicked: abLinePicker.visible = true
                    buttonText: "ABLine"
                }

                IconButton{
                    id: btnABLineCycle
                    objectName: "btnABLineCycle"
                    icon.source: "/images/ABLineCycle.png"
                    width: btnABLine.width
                    height: btnABLine.height
                }
                IconButton{
                    id: btnABLineCycleBk
                    objectName: "btnABLineCycleBk"
                    icon.source: "/images/ABLineCycleBk.png"
                    width: btnABLine.width
                    height: btnABLine.height
                }

                IconButtonText {
                    id: btnManualOffOn
                    objectName: "btnManualOffOn"
                    isChecked: false
                    checkable: true
                    icon.source: "/images/ManualOff.png"
                    iconChecked: "/images/ManualOn.png"
                    buttonText: "Manual"
                }

                IconButtonText {
                    id: btnSectionOffAutoOn
                    objectName: "btnSectionOffAutoOn"
                    isChecked: false
                    checkable: true
                    icon.source: "/images/SectionMasterOff.png"
                    iconChecked: "/images/SectionMasterOn.png"
                    buttonText: "Auto"
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
                    buttonText: "1"
                }

                SectionButton {
                    id: btnSection2Man
                    objectName: "section1"
                    state: "auto"
                    buttonText: "2"
                }

                SectionButton {
                    id: btnSection3Man
                    objectName: "section2"
                    buttonText: "3"
                }

                SectionButton {
                    id: btnSection4Man
                    objectName: "section3"
                    buttonText: "4"
                }

                SectionButton {
                    id: btnSection5Man
                    visible: false
                    objectName: "section4"
                    buttonText: "5"
                }

                SectionButton {
                    id: btnSection6Man
                    visible: false
                    objectName: "section5"
                    buttonText: "6"
                }

                SectionButton {
                    id: btnSection7Man
                    visible: false
                    objectName: "section6"
                    buttonText: "7"
                }

                SectionButton {
                    id: btnSection8Man
                    visible: false
                    objectName: "section7"
                    buttonText: "8"
                }

                SectionButton {
                    id: btnSection9Man
                    visible: false
                    objectName: "section8"
                    buttonText: "9"
                }

                SectionButton {
                    id: btnSection10Man
                    visible: false
                    objectName: "section9"
                    buttonText: "10"
                }

                SectionButton {
                    id: btnSection11Man
                    visible: false
                    objectName: "section10"
                    buttonText: "11"
                }

                SectionButton {
                    id: btnSection12Man
                    visible: false
                    objectName: "section11"
                    buttonText: "12"
                }

                SectionButton {
                    id: btnSection13Man
                    visible: false
                    objectName: "section12"
                    buttonText: "13"
                }

                SectionButton {
                    id: btnSection14Man
                    visible: false
                    objectName: "section13"
                    buttonText: "14"
                }

                SectionButton {
                    id: btnSection15Man
                    visible: false
                    objectName: "section14"
                    buttonText: "15"
                }

                SectionButton {
                    id: btnSection16Man
                    visible: false
                    objectName: "section15"
                    buttonText: "16"
                }

                SectionButton {
                    id: btnSection17Man
                    visible: false
                    objectName: "section16"
                    buttonText: "17"
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

                    IconButtonTransparent {
                        id: btnTiltDown
                        objectName: "btnTiltDown"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "/images/TiltDown.png"
                    }
                    IconButtonTransparent {
                        id: btnCamera2d
                        objectName: "btnCamera2d"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "/images/Camera2D64.png"
                    }
                    IconButtonTransparent {
                        id: btnCameraNorth2d
                        objectName: "btnCameraNorth2d"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "/images/CameraNorth2D.png"
                    }
                    IconButtonTransparent {
                        id: btnZoomOut
                        objectName: "btnZoomOut"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "/images/ZoomOut48.png"
                    }
                    IconButtonTransparent {
                        id: btnWindowDayNight
                        objectName: "btnWindowDayNight"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "/images/WindowDayMode.png"
                    }
                    IconButtonTransparent {
                        id: btnBrightnessDown
                        objectName: "btnBrightnessDown"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "/images/BrightnessDn.png"
                    }
                    IconButtonTransparent {
                        id: btnTiltUp
                        objectName: "btnTiltUp"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "/images/TiltUp.png"
                    }
                    IconButtonTransparent {
                        id: btnCamera3d
                        objectName: "btnCamera3d"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "/images/Camera3D64.png"
                    }
                    IconButtonTransparent {
                        id: btnCameraNorth3d
                        objectName: "btnCameraNorth3d"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "/images/CameraNorth64.png"
                    }
                    IconButtonTransparent {
                        id: btnZoomIn
                        objectName: "btnZoomIn"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "/images/ZoomIn48.png"
                    }
                    Rectangle {
                        id: btnHZ
                        width: 70
                        height: 70
                        radius: 10
                    }
                    IconButtonTransparent {
                        id: btnBrightnessUp
                        objectName: "btnBrightnessUp"
                        width: 70
                        height: 70
                        radius: 10
                        icon.source: "/images/BrightnessUp.png"
                    }
                }
            }

            Item{
                objectName: "autoUTurnButton"
                id: autoUturn
                anchors.top: parent.top
                anchors.right: rightColumn.left
                anchors.topMargin: 50
                anchors.rightMargin: 150
                visible: true
                width: childrenRect.width
                height: childrenRect.height
                ColorOverlay{
                    id: myOverlay
                    color: "#40ff00"
                    anchors.fill: autoTurn
                    source: autoTurn
                    visible: true
                    Text{
                        anchors.top: myOverlay.bottom
                        anchors.topMargin: -25
                        anchors.left: myOverlay.left
                        text: " however ft"
                        color: myOverlay.color
                        font.pixelSize: 20
                        visible: true
                    }
                    MouseArea{
                        anchors.fill: parent
                        id: mouseToggle
                        onClicked:{
                            function imageMirror(){
                                if(autoTurnImage.mirror == true){
                                    autoTurnImage.mirror = false
                                }else{
                                    autoTurnImage.mirror = true
                                }
                            }
                            imageMirror()
                        }
                    }
                }
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
            }
            Item{
                 objectName: "manUTurnButtons"
               id: manualUturnLateral
                anchors.top: parent.top
                anchors.left: leftColumn.right
                anchors.topMargin: 50
                anchors.leftMargin: 150
                visible: true
                width: childrenRect.width
                height: childrenRect.height
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
                    }
                    Button{
                        background: Rectangle{color: "transparent"}
                        objectName: "btnManUturnRight"
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        anchors.left: parent.horizontalCenter
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
                    }
                    Button{
                        background: Rectangle{color: "transparent"}
                        objectName: "btnManLateralRight"
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        anchors.left: parent.horizontalCenter
                    }
                }
            }


            /*Row {
                id: manTurnButtons
                visible: False
                objectName: "manUTurnButtons"
                anchors.left: leftColumn.right
                anchors.leftMargin: 15
                IconButtonTransparent {
                    id: manUturnBtnLeft
                    objectName: "btnManUturnLeft"
                    width: 58
                    height: 58
                    icon.source: "/images/TurnManualLeft.png"
                }
                IconButtonTransparent {
                    id: manUturnBtnRight
                    objectName: "btnManUturnRight"
                    width: 58
                    height: 58
                    icon.source: "/images/TurnManualRight.png"
                }
                anchors.top: parent.top;
                anchors.topMargin: 15
                spacing: 6
            }*/

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
                icon.source: "/images/Stop.png"
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
                icon.source: "/images/AutoSteerOn.png"
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
                objectName: "config"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                height: parent.height + 50
                width:parent.width
                visible:false
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
                }
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
                        icon.source: "/images/AreaSide.png";
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
