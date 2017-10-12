import QtQuick 2.0
import QtQuick.Controls 2.1


Item {

    Column {
        id: flow1
        x: 120
        y: 72
        width: 400
        height: 400
        spacing: 10

        Button {
            id: button2
            text: qsTr("Resume last ")
        }

        Button {
            id: button
            text: qsTr("Create New")
        }

        Button {
            id: button1
            text: qsTr("Open Existing")
        }
    }

}
