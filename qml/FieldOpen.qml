import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

Dialog {
    id:fieldOpen
    //color: "ghostwhite"
    visible: false
    width: 1024
    height: 700
    z:3
    Rectangle {
        id: topLine
        objectName: "topLine"
        width: parent.width
        height: screenPixelDensity * 0.3 //.3" tall
        color: "ghostwhite"
        z: 4
    }
    FieldTable {
        id: fieldTable
        anchors.top: topLine.bottom
        anchors.bottom: grid3.top
        width:parent.width - 10
        anchors.left: parent.left
        anchors.topMargin: 10
        anchors.leftMargin: 5
        onCurrentRowChanged: {
            console.debug("clicked on", fieldTable.currentRow)
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
                icon.source: "/images/skull.png"
                text: "Delete Field"
                color3: "white"
                border: 1
                height: 75
                enabled: fieldTable.currentRow > -1
                onClicked: {
                    aog.field_delete(aog.field_list[fieldTable.currentRow].name)
                    //backend should update the list
                }
            }
            IconButtonText {
                id: cancel
                objectName: "btnCancel"
                icon.source: "/images/Cancel64.png"
                text: "Cancel"
                color3: "white"
                border: 1
                height: 75
                onClicked: {
                    fieldOpen.rejected()
                }
            }
            IconButtonText {
                id: useSelected
                objectName: "btnUseSelected"
                icon.source: "/images/FileOpen.png"
                text: "Use Selected"
                color3: "white"
                border: 1
                height: 75
                enabled: fieldTable.currentRow > -1
                onClicked: {
                    aog.field_open(aog.field_list[fieldTable.currentRow].name)
                    fieldOpen.accepted()
                }
            }
        }
    }
}
