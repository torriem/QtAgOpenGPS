import QtQuick 2.8
import AgOpenGPS 1.0

OpenGLControl {
    id: item1
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
            icon: "/images/ArrowRight.png"

            //pseudo state
            property bool menuin: false

            function toggle_menu() {
                if (menuin == false) {
                    menuin = true
                    icon="/images/ArrowLeft.png"
                } else {
                    menuin = false
                    icon="/images/ArrowRight.png"
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
            icon: "/images/Display.png"
        }

        IconButtonText {
            id: btnPerimeter
            objectName: "btnPerimeter"
            buttonText: "000.00"
            iconHeightScale: 1.0
            icon: "/images/PeriArea.png"
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
            icon: "/images/FlagRed.png"
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
            icon: "/images/ABLineOff.png"
        }

        IconButton {
            id: btnContour
            objectName: "btnContour"
            icon: "/images/ContourOff.png"
        }

        IconButton {
            id: btnManualOffOn
            objectName: "btnManualOffOn"
            icon: "/images/ManualOff.png"
        }

        IconButtonText {
            id: btnSectionOffAutoOn
            objectName: "btnSectionOffAutoOn"
            icon: "/images/SectionMasterOff.png"
            iconHeightScale: 1.0
        }

    }

    Row {
        id: row
        anchors.horizontalCenter: parent.horizontalCenter

        anchors.bottom: parent.bottom
        anchors.bottomMargin:15

        spacing: 3

        SectionButton {
            id: btnSection1Man
            objectName: "section1"
            state: "on"
        }

        SectionButton {
            id: btnSection2Man
            objectName: "section2"
            state: "auto"
        }

        SectionButton {
            id: btnSection3Man
            objectName: "section3"
        }

        SectionButton {
            id: btnSection4Man
            objectName: "section4"
        }

        SectionButton {
            id: btnSection5Man
            visible: false
            objectName: "section5"
        }

        SectionButton {
            id: btnSection6Man
            visible: false
            objectName: "section6"
        }

        SectionButton {
            id: btnSection7Man
            visible: false
            objectName: "section7"
        }

        SectionButton {
            id: btnSection8Man
            visible: false
            objectName: "section8"
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
            icon: "/images/TiltDown.png"
        }

        IconButton {
            id: btnTiltUp
            objectName: "btnTiltUp"
            width: 70
            height: 70
            radius: 10
            icon: "/images/TiltUp.png"
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
            icon: "/images/ZoomIn48.png"
        }

        IconButton {
            id: btnZoomOut
            objectName: "btnZoomOut"
            width: 70
            height: 70
            radius: 10
            icon: "/images/ZoomOut48.png"
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

}
