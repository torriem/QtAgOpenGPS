import QtQuick 2.0
import QtQuick.Controls 2.1

Item{
    width: childrenRect.width + 10
    height: childrenRect.height + 10
    anchors.left: parent.left + 150
    anchors.top: parent.top + 150
    Rectangle{
        id:fieldWindow
        color: "white"
        visible: true
        width: 500
        height: 500
        z:9
        Grid {
            id: grid1
            width: childrenRect.width
            height: childrenRect.height
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5
            spacing: 10
            flow: Grid.TopToBottom
            rows: 1
            columns: 1


            Button {
                id: button14
                width: 250
                height: 50
                text: qsTr("Boundary")
            }
       }
    }
}
