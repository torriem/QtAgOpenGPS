import QtQuick 2.8

Item {
    id: item1
    width:800
    height:600
    anchors.fill: parent

    Rectangle {
        id: tempBG
        anchors.fill:parent
        color: "black"
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
    }

    Column {
        id: leftColumn
        anchors.top: parent.top
        anchors.topMargin: 6
        anchors.left: parent.left
        anchors.leftMargin: 6
        spacing: 3

        IconButtonText{
            id: btnMenuDrawer
            buttonText: qsTr("Menu")
            icon: "images/ArrowRight.png"

            //pseudo state
            property bool menuin: false

            function toggle_menu() {
                if (menuin == false) {
                    menuin = true
                    icon="images/ArrowLeft.png"
                } else {
                    menuin = false
                    icon="images/ArrowRight.png"
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
            icon: "images/Display.png"
        }

        IconButtonText {
            id: btnPerimeter
            buttonText: "000.00"
            iconHeightScale: 1.0
            icon: "images/PeriArea.png"
        }

        IconButtonText {
            id: btnAutoSteer
            buttonText: "                X"
            enabled: false
        }

        IconButton {
            id: btnFlag
            icon: "images/FlagRed.png"
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
            buttonText: "X"
            icon: "images/ABLineOff.png"
        }

        IconButton {
            id: btnContour
            icon: "images/ContourOff.png"
        }

        IconButton {
            id: btnManualOffOn
            icon: "images/ManualOff.png"
        }

        IconButtonText {
            id: btnSectionOffAutoOn
            icon: "images/SectionMasterOff.png"
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
            state: "on"
        }

        SectionButton {
            id: btnSection2Man
            state: "auto"
        }

        SectionButton {
            id: btnSection3Man
        }

        SectionButton {
            id: btnSection4Man
        }

        SectionButton {
            id: btnSection5Man
            visible: false
        }

        SectionButton {
            id: btnSection6Man
            visible: false
        }

        SectionButton {
            id: btnSection7Man
            visible: false
        }

        SectionButton {
            id: btnSection8Man
            visible: false
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
            width: 70
            height: 70
            radius: 10
            icon: "images/TiltDown.png"
        }

        IconButton {
            id: btnTiltUp
            width: 70
            height: 70
            radius: 10
            icon: "images/TiltUp.png"
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
            id: btnZoomOut
            width: 70
            height: 70
            radius: 10
            icon: "images/ZoomIn48.png"
        }

        IconButton {
            id: btnZoomIn
            width: 70
            height: 70
            radius: 10
            icon: "images/ZoomOut48.png"
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
