import QtQuick 2.8
import QtQuick.Controls 2.1
import AgOpenGPS 1.0

Item {
    //We draw native opengl to this root object
    id: item1
    width: 900
    height: 600
    //objectName: "openGLControl"
    //width:800
    //height:600
    anchors.fill: parent

    SystemPalette {
        id: systemPalette
        colorGroup: SystemPalette.Active
    }

    Rectangle {
        id: topLine
        objectName: "topLine"
        width: parent.width
        height: screenPixelDensity * 0.3 //.3" tall
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

        Item {
            id: buttonsArea
            anchors.top: parent.top
            anchors.topMargin: 42 //TODO: convert to scalable
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Column {
                id: leftColumn
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 6
                spacing: 3

                IconButtonText{
                    id: btnMenuDrawer
                    objectName: "btnMenuDrawer"
                    buttonText: qsTr("Menu")
                    icon: "qrc:/images/ArrowRight.png"

                    //pseudo state
                    property bool hideMenu: true

                    onHideMenuChanged: {
                        if (hideMenu == true) {
                            icon="qrc:/images/ArrowRight.png"
                            iconPalette.visible = false
                        } else {
                            icon="qrc:/images/ArrowLeft.png"
                            iconPalette.visible = true
                        }
                    }

                    function toggle_menu() {
                        if (hideMenu == true) {
                            hideMenu = false
                        } else {
                            hideMenu = true
                        }
                    }

                    onClicked: {
                        toggle_menu();
                    }
                }

                IconButton{
                    id: btnMinMaxZoom
                    objectName: "btnMinMaxZoom"

                    icon: "qrc:/images/Display.png"
                }

                IconButtonText {
                    id: btnPerimeter
                    objectName: "btnPerimeter"
                    buttonText: "000.00"
                    isChecked: false
                    iconHeightScale: 1.0
                    icon: "qrc:/images/PeriArea.png"
                    iconChecked: "qrc:/images/PeriDraw.png"
                    onPressAndHold: {
                        if (contextArea.visible) {
                            contextArea.visible = false;
                        } else {
                            contextArea.visible = true;
                        }
                    }
                }

                IconButtonText {
                    id: btnAutoSteer
                    objectName: "btnAutoSteer"
                    icon: "qrc:/images/AutoSteerOff.png"
                    buttonText: "X"
                }

                IconButton {
                    id: btnFlag
                    objectName: "btnFlag"
                    icon: "qrc:/images/FlagRed.png"
                    onPressAndHold: {
                        if (contextFlag.visible) {
                            contextFlag.visible = false;
                        } else {
                            contextFlag.visible = true;
                        }
                    }
                }
            }

            Column {
                id: rightColumn
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.rightMargin: 6
                spacing: 3

                IconButtonText{
                    id: btnABLine
                    objectName: "btnABLine"
                    buttonText: "X"
                    icon: "qrc:/images/ABLineOff.png"
                }

                IconButton {
                    id: btnContour
                    objectName: "btnContour"
                    checkable: true
                    isChecked: false
                    icon: "qrc:/images/ContourOff.png"
                    iconChecked: "qrc:/images/ContourOn.png"
                }

                IconButton {
                    id: btnManualOffOn
                    objectName: "btnManualOffOn"
                    checkable: true
                    isChecked: false
                    icon: "qrc:/images/ManualOff.png"
                    iconChecked: "qrc:/images/ManualOn.png"
                }

                IconButtonText {
                    id: btnSectionOffAutoOn
                    objectName: "btnSectionOffAutoOn"
                    isChecked: false
                    icon: "qrc:/images/SectionMasterOff.png"
                    iconChecked: "qrc:/images/SectionMasterOn.png"
                    iconHeightScale: 1.0
                }

            }

            Row {
                id: sectionButtons
                anchors.horizontalCenter: parent.horizontalCenter

                anchors.bottom: parent.bottom
                anchors.bottomMargin:15

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

            Column {
                id: tiltButtons
                spacing: 6
                anchors.left: leftColumn.right
                anchors.leftMargin: 20
                anchors.top: parent.top
                anchors.topMargin: 20

                IconButton {
                    id: btnTiltDown
                    objectName: "btnTiltDown"
                    width: 70
                    height: 70
                    radius: 10
                    icon: "qrc:/images/TiltDown.png"
                }

                IconButton {
                    id: btnTiltUp
                    objectName: "btnTiltUp"
                    width: 70
                    height: 70
                    radius: 10
                    icon: "qrc:/images/TiltUp.png"
                }
                IconButton {
                    id: btnZoomIn
                    objectName: "btnZoomIn"
                    width: 70
                    height: 70
                    radius: 10
                    icon: "qrc:/images/ZoomIn48.png"
                }

                IconButton {
                    id: btnZoomOut
                    objectName: "btnZoomOut"
                    width: 70
                    height: 70
                    radius: 10
                    icon: "qrc:/images/ZoomOut48.png"
                }
            }

            Row {
                id: manTurnButtons
                visible: true
                objectName: "manUTurnButtons"
                IconButtonTransparent {
                    id: manUturnBtnLeft
                    objectName: "btnManUturnLeft"
                    width: 58
                    height: 58
                    icon: "qrc:/images/TurnManualLeft.png"
                }
                IconButtonTransparent {
                    id: manUturnBtnRight
                    objectName: "btnManUturnRight"
                    width: 58
                    height: 58
                    icon: "qrc:/images/TurnManualRight.png"
                }
                anchors.left: tiltButtons.right;
                anchors.top: tiltButtons.top;
                spacing: 6
                anchors.leftMargin: 10
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

            Slider {
                id: speedSlider
                objectName: "simSpeed"
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: 200
                from: 0
                to: 100
                value: 5
            }
            Slider {
                id: steerSlider
                objectName: "simSteer"
                anchors.bottom: speedSlider.top
                anchors.left: parent.left
                width: 200
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
                anchors.bottom: steerSlider.bottom
                onClicked: {
                    steerSlider.value = 300;
                }

            }



            IconPalette {
                id: iconPalette
                objectName: "slideoutMenu"
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.left: leftColumn.right
                anchors.leftMargin: 15
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
                anchors.leftMargin: btnPerimeter.width + 10
                anchors.topMargin: btnPerimeter.y
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
                        icon: "qrc:/images/AreaSide.png";
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
                        icon: "qrc:/images/FlagRed.png";
                    }
                    IconButton {
                        id: greenFlag
                        objectName: "btnGreenFlag"
                        icon: "qrc:/images/FlagGrn.png";
                    }
                    IconButton {
                        id: yellowFlag
                        objectName: "btnYellowFlag"
                        icon: "qrc:/images/FlagYel.png";
                    }
                    IconButton {
                        id: deleteFlag
                        objectName: "btnDeleteFlag"
                        icon: "qrc:/images/FlagDelete.png"
                        enabled: false
                    }
                    IconButton {
                        id: deleteAllFlags
                        objectName: "btnDeleteAllFlags"
                        icon: "qrc:/images/FlagDeleteAll.png"
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
