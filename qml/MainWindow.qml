import QtQuick 2.8
import AgOpenGPS 1.0

Item {
    //We draw native opengl to this root object
    id: item1
    width: 900
    height: 600
    objectName: "openGLControl"
    //width:800
    //height:600
    anchors.fill: parent

    Column {
        id: leftColumn
        anchors.top: parent.top
        anchors.topMargin: 6
        anchors.left: parent.left
        anchors.leftMargin: 6
        spacing: 3

        IconButtonText{
            id: btnMenuDrawer
            objectName: "btnMenuDrawer"
            buttonText: qsTr("Menu")
            icon: "qrc:/images/ArrowRight.png"

            //pseudo state
            property bool menuin: false

            function toggle_menu() {
                if (menuin == false) {
                    menuin = true
                    icon="qrc:/images/ArrowLeft.png"
                } else {
                    menuin = false
                    icon="qrc:/images/ArrowRight.png"
                }
            }

            onClicked: {
                toggle_menu();
                if (menuin) {
                    iconPalette.visible = true
                } else {
                    iconPalette.visible = false
                }
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
            iconHeightScale: 1.0
            icon: "qrc:/images/PeriArea.png"
        }

        IconButtonText {
            id: btnAutoSteer
            objectName: "btnAutoSteer"
            buttonText: "X"
            enabled: false
        }

        IconButton {
            id: btnFlag
            objectName: "btnFlag"
            icon: "qrc:/images/FlagRed.png"
        }
    }

    Column {
        id: rightColumn
        anchors.top: parent.top
        anchors.topMargin: 6
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
            icon: "qrc:/images/ContourOff.png"
        }

        IconButton {
            id: btnManualOffOn
            objectName: "btnManualOffOn"
            checkable: true
            state: "unchecked"
            icon: "qrc:/images/ManualOff.png"
        }

        IconButtonText {
            id: btnSectionOffAutoOn
            objectName: "btnSectionOffAutoOn"
            icon: "qrc:/images/SectionMasterOff.png"
            iconHeightScale: 1.0
        }

    }

    Row {
        id: sectionButtons
        anchors.horizontalCenter: parent.horizontalCenter

        anchors.bottom: statusBar.top
        anchors.bottomMargin:15

        spacing: 3

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
    }

    Column {
        id: zoomButtons
        spacing: 6
        anchors.right: rightColumn.left
        anchors.rightMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20

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


    IconPalette {
        id: iconPalette
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: leftColumn.right
        anchors.leftMargin: 15
        visible: false

    }

    Text {
        id: txtDistanceOffABLine
        objectName: "txtDistanceOffABLine"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Hi there"
        color: "white"
    }

    Rectangle {
        id: statusBar
        y: 566
        height: 25
        color: "#ffffff"
        opacity: 0.8
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        Text {
            id: stripHz
            objectName: "stripHz"
            text: qsTr("Hz")
            font.pixelSize: parent.height - 10;
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
        }

        Text {
            id: stripDistance
            objectName: "stripDistance"
            text: "555555 ft"
            font.pixelSize: parent.height - 10;
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: stripHz.right
            anchors.leftMargin: parent.height * 1.5
        }

        Item {
            id: toolStripBtnResetDistance
            objectName: "toolStripBtnResetDistance"
            width: 150
            height: parent.height - 10
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: stripDistance.right
            anchors.leftMargin: parent.height * 1.5
        }

        Text {
            id: stripAreaUser
            objectName: "stripAreaUser"
            text: qsTr("188.88")
            font.pixelSize: parent.height - 10;
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: toolStripBtnResetDistance.right
            anchors.leftMargin: parent.height * 1.5
        }

        Text {
            id: stripEqWidth
            objectName: "stripEqWidth"
            text: "Width"
            font.pixelSize: parent.height - 10;
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: stripAreaUser.right
            anchors.leftMargin: parent.height * 1.5
        }

        Text {
            id: stripBoundaryArea
            objectName: "stripBoundaryArea"
            text: "Area"
            font.pixelSize: parent.height - 10;
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: stripEqWidth.right
            anchors.leftMargin: parent.height * 1.5
        }
        Text {
            id: stripAreaRate
            objectName: "stripAreaRate"
            text: "Ac/hr"
            font.pixelSize: parent.height - 10;
            verticalAlignment: Text.AlignVCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: stripBoundaryArea.right
            anchors.leftMargin: parent.height * 1.5
        }
         Text {
            id: stripPortGPS
            objectName: "stripPortGPS"
            text: "* *"
            font.pixelSize: parent.height - 10;
            verticalAlignment: Text.AlignVCenter
            color: "red"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: stripOnlineGPS.left
            anchors.rightMargin:  parent.height / 10
        }
        PortLight {
            id: stripOnlineGPS
            objectName: "stripOnlineGPS"
            height: parent.height - 10
            width: (parent.height - 10) / 2
            color: "grey"
            radius: (parent.height-10) / 10
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: stripPortArduino.left
            anchors.rightMargin: parent.height / 10
        }
       Text {
            id: stripPortArduino
            objectName: "stripPortArduino"
            text: "* *"
            font.pixelSize: parent.height - 10;
            verticalAlignment: Text.AlignVCenter
            color: "red"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: stripOnlineArduino.left
            anchors.rightMargin:  parent.height / 10
        }
        PortLight {
            id: stripOnlineArduino
            objectName: "stripOnlineArduino"
            height: parent.height - 10
            width: (parent.height - 10) / 2
            color: "grey"
            radius: (parent.height-10) / 10
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: stripPortAutoSteer.left
            anchors.rightMargin: parent.height / 10
        }
        Text {
            id: stripPortAutoSteer
            objectName: "stripPortAutoSteer"
            text: "* *"
            font.pixelSize: parent.height - 10;
            verticalAlignment: Text.AlignVCenter
            color: "red"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: stripOnlineAutoSteer.left
            anchors.rightMargin: parent.height / 10
        }
        PortLight {
            id: stripOnlineAutoSteer
            objectName: "stripOnlineAutoSteer"
            height: parent.height - 10
            width: (parent.height - 10) / 2
            color: "grey"
            radius: (parent.height-10) / 10
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin:15
        }
    }

}
