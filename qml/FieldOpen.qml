import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.15

Popup {
    id:fieldOpen
    //color: "ghostwhite"
    visible: false
    width: 700
    height: 500
    modal: true

    property int sortBy: 1

    Rectangle {
        id: topLine
        objectName: "topLine"
        width: parent.width
        height: screenPixelDensity * 0.5 //.3" tall
        color: "light grey"
        z: 4
        border.color: "black"
        border.width: 1
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: "Open Field"
            font.pointSize: 20
        }
    }
    FieldTable {
        id: fieldTable
        anchors.top: topLine.bottom
        anchors.bottom: grid3.top
        width:parent.width - 10
        anchors.left: parent.left
        anchors.topMargin: 10
        anchors.leftMargin: 5
        anchors.rightMargin: 15
        anchors.bottomMargin: 10

        sortBy: fieldOpen.sortBy

        anchors.right: topLine.right
        ScrollBar.vertical: ScrollBar {
            id: scrollbar
            anchors.left: fieldOpen.right
            anchors.rightMargin: 10
            width: 10
            policy: ScrollBar.AlwaysOn
            active: true
            contentItem.opacity: 1
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
        //color: "white"
        color: Material.backgroundColor
        Row {
            anchors.fill: parent
            spacing: 4
            //flow: Grid.TopToBottom
            //rows: 1
            IconButtonText {
                id: deleteField
                objectName: "btnDeleteField"
                icon.source: "/images/skull.png"
                text: "Delete Field"
                radius: 0
                //color3: "white"
                border: 1
                height: 75
                enabled: fieldTable.currentIndex > -1
                onClicked: {
                    aog.field_delete(fieldTable.currentFieldName)
                    //backend should update the list
                }
            }
        }
        Row {
            spacing: 5
            anchors.right: parent.right
            IconButtonText {
                id: sort
                //color3: "white"
                height: 75
                text: "Toggle Sort"
                radius: 0

                border: 1
                onClicked: {
                    fieldTable.sortBy = (fieldTable.sortBy % 3) + 1
                }
            }

            IconButtonText {
                id: cancel
                objectName: "btnCancel"
                icon.source: "/images/Cancel64.png"
                text: "Cancel"
                //color3: "white"
                radius: 0
                border: 1
                height: 75
                onClicked: {
                    fieldTable.clear_selection()
                    fieldOpen.close()
                }
            }
            IconButtonText {
                id: useSelected
                objectName: "btnUseSelected"
                icon.source: "/images/FileOpen.png"
                text: "Use Selected"
                radius: 0
                //color3: "white"
                border: 1
                height: 75
                enabled: fieldTable.currentIndex > -1
                onClicked: {
                    aog.field_open(fieldTable.currentFieldName)
                    fieldTable.clear_selection()
                    fieldOpen.close()
                }
            }
        }
    }
}
