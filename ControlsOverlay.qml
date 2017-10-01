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
            buttonText: "Menu"
            icon: "images/ArrowRight.png"
        }

        IconButton{
            id: btnMinMaxZoom
            icon: "images/Display.png"
        }

        IconButtonText {
            id: btnPerimeter
            buttonText: "000.00"
            icon: "images/PeriArea.png"
        }

        IconButtonText {
            id: btnAutoSteer
            buttonText: "                X"
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

    Rectangle {
        id: rectangle
        width: childrenRect.width+10
        height: childrenRect.height+10
        color: "#bf163814"
        visible: true
        border.color: "#c3ecc0"
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: leftColumn.right
        anchors.leftMargin: 15

        Grid {
            id: grid
            width: childrenRect.width
            height: childrenRect.height
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5
            spacing: 6
            flow: Grid.TopToBottom
            rows: 4
            columns: 3

            IconButton {
                id: iconButton
            }

            IconButtonText {
                id: iconButtonText
            }

            IconButtonText {
                id: iconButtonText1
            }

            IconButtonText {
                id: iconButtonText2
            }

            IconButton {
                id: btnJob
                icon: "images/JobActive.png"
            }

            IconButtonText {
                id: iconButtonText3
            }

            IconButton {
                id: iconButton2
            }

            IconButtonText {
                id: iconButtonText4
            }

            IconButtonText {
                id: btnUnits
                buttonText: qsTr("Units")
                icon: "images/AcHa.png"
            }

            IconButtonText {
                id: btnFileExploer
                icon: "images/FileExplorerWindows.png"
                buttonText: qsTr("Explorer")
            }

        }
    }
}
