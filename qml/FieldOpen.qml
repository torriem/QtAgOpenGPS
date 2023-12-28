import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3
import Qt.labs.folderlistmodel 2.2

Rectangle{
    id:fieldOpen
    color: "white"
    visible: false
    width: 800
    height: 500
    z:3
    Rectangle {
        id: topLine
        objectName: "topLine"
        width: parent.width
        height: screenPixelDensity * 0.3 //.3" tall
        color: "black"
        z: 4
        Text {
            id: field
            text: "Field"
            color: "white"
            anchors.left: topLine.left
            anchors.leftMargin: 15
            anchors.verticalCenter: topLine.verticalCenter
        }
        Text {
            id: distance
            text: qsTr("Distance")
            color: "White"
            anchors.right: area.left
            anchors.verticalCenter: topLine.verticalCenter
            anchors.rightMargin: 100
        }
        Text {
            id: area
            text: qsTr("Area")
            color: "white"
            anchors.right: parent.right
            anchors.verticalCenter: topLine.verticalCenter
            anchors.rightMargin: 30
        }
    }
    ListView {
        anchors.top: topLine.bottom
        anchors.bottom: grid3.top
        width:parent.width - 10
        anchors.left: parent.left
        anchors.leftMargin: 5
        property Component mycomponent: fileName
        model :FolderListModel{
            id: fieldList
            showDirs: true
            showFiles: false
            folder: "file:/home/davidwedel/Documents/QtAgOpenGPS/Fields/"
        }

        delegate: RadioButton{
            id: control
            indicator: Rectangle{
                border.width: 1
                border.color: "black"
                anchors.fill: parent
                Rectangle{
                    anchors.fill: parent
                    anchors.margins: 1
                    color: control.down ? "white" : "blue"
                    visible: control.checked
                }
            }

            width:parent.width
            height:50
            Text{
                anchors.left: parent.left
                anchors.leftMargin: 2
                anchors.verticalCenter: parent.verticalCenter
                text: fileName
                font.pixelSize: 25
                font.bold: true
                color: control.checked ? "white" : "black"
                z: 2
            }
        }
    }

    Rectangle{
        id: grid3
        z: 4
        width: parent.width - 10
        height: deleteField.height + 10
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.bottom: parent.bottom
        color: "white"
        Grid {
            anchors.fill: parent
            spacing: 60
            flow: Grid.TopToBottom
            rows: 1
            columns: 4
            IconButtonText {
                id: deleteField
                objectName: "btnDeleteField"
                icon.source: "qrc:/images/skull.png"
                buttonText: "Delete Field"
                color3: "white"
                border: 1
                height: 75
            }
            IconButtonText {
                id: sort
                objectName: "btnSort"
                icon.source: "qrc:/images/Sort.png"
                buttonText: "Sort"
                color3: "white"
                border: 1
                height: 75
            }
            IconButtonText {
                id: cancel
                objectName: "btnCancel"
                icon.source: "qrc:/images/Cancel64.png"
                buttonText: "Cancel"
                onClicked: fieldOpen.visible = false
                color3: "white"
                border: 1
                height: 75
            }
            IconButtonText {
                id: useSelected
                objectName: "btnUseSelected"
                icon.source: "qrc:/images/FileOpen.png"
                buttonText: "Use Selected"
                color3: "white"
                border: 1
                height: 75
            }
        }
    }
}
