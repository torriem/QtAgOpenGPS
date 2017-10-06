import QtQuick 2.0

Rectangle {
    id: rectangle
    width: childrenRect.width+10
    height: childrenRect.height+10
    color: "#bf163814"
    visible: true
    border.color: "#c3ecc0"

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
