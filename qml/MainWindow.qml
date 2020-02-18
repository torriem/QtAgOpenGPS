import QtQuick 2.8
//import QtQuick.Controls 2.1
import AgOpenGPS 1.0

Item {
    //We draw native opengl to this root object
    id: item1
    width: 1024
    height: 600
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
            text: "Top line stuff goes here"
            color: systemPalette.windowText
            anchors.horizontalCenter: parent.horizontalCenter

        }
        color: systemPalette.window
        anchors.top: parent.top
    }

    Rectangle {
        id :background
        objectName: "background"
        anchors.top: topLine.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom:  statusBar.top

        color: "black"
        Text {
            id: text2
            text: qsTr("No GPS")
            color: "white"
            font.pointSize: 24
            anchors.centerIn: parent
        }
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
                cSimForm.visible = false
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

                IconButtonText
                {
                    id: btnMenuDrawer
                    objectName: "btnMenuDrawer"
                    buttonText: qsTr("Menu")
                    icon: "qrc:/images/ArrowRight.png"

                    //pseudo state
                    property bool hideMenu: true

                    onHideMenuChanged:
                    {
                        if (hideMenu == true)
                        {
                            icon="qrc:/images/ArrowRight.png"
                            iconPalette.visible = false
                        } else
                        {
                            icon="qrc:/images/ArrowLeft.png"
                            iconPalette.visible = true
                        }
                    }

                    function toggle_menu()
                    {
                        if (hideMenu)
                        {
                            hideMenu = false
                        } else {
                            hideMenu = true
                        }
                    }

                    onClicked: {
                        toggle_menu();
                        btnMinMaxZoom.hideMenuDrag = true
                    }
                }

                IconButton {
                    id: btnDisplayDrag
                    objectName: "btnDisplayDrag"
                    icon: "qrc:/images/DisplayDrag.png"
                    property bool hideMenuDrag: true

                    onHideMenuDragChanged: {
                        if (hideMenuDrag){
                            iconDragMenu.visible = false
                        }else{
                            iconDragMenu.visible = true
                        }
                    }

                    function toggle_menu() {
                        if (hideMenuDrag == true) {
                            hideMenuDrag = false

                        }else {
                            hideMenuDrag = true
                        }
                    }
                    onClicked: {
                        toggle_menu()
                    }
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
            }


            IconPaletteSlideOutMenu {
                id: iconPalette
                objectName: "slideoutMenu"
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.left: leftColumn.right
                anchors.leftMargin: 15
                visible: false
                onBtnGPSDataBtnClicked: {
                    if (!cSimForm.visible){
                        cSimForm.visible = true
                    }else{
                        cSimForm.visible = false
                    }
                }
            }
            IconPaletteDragMenu {
                id: iconDragMenu
                objectName: "dragMenu"
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.left: leftColumn.right
                anchors.leftMargin: 15
                visible: false
            }

            CSimForm {
                id: cSimForm
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.left: leftColumn.right
                anchors.leftMargin: iconPalette.width + 20
                MouseArea {
                    id: cSimMouseArea
                    anchors.fill: parent
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
        height: screenPixelDensity * 0.3
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
