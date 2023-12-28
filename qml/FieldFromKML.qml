import QtQuick 2.0

Rectangle{
    id: fieldFromKML
    width: 500
    height: 450
    color: "lightgray"
    Rectangle{
        id: textEntry
        width: 450
        height: 30
        anchors.top:parent.top
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        color: "ghostwhite"
        border.color: "darkgray"
        border.width: 1
        Text {
            anchors.left: parent.left
            anchors.bottom: parent.top
            font.bold: true
            font.pixelSize: 15
            text: qsTr("Enter Field Name")
        }
        TextInput{
            objectName: "fieldNew"
            anchors.fill: parent
        }
    }
    Row{
        id: additives
        anchors.left: parent.left
        anchors.top: textEntry.bottom
        anchors.margins: 30
        spacing: 30
        IconButtonTransparent{
            objectName: "btnAddDate"
            id: marker
            icon.source: "qrc:/images/JobNameCalendar.png"
            Text{
                anchors.right: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: "+"
            }
        }
        IconButtonTransparent{
            objectName: "btnAddTime"
            icon.source: "qrc:/images/JobNameTime.png"
            Text{
                anchors.right: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: "+"
            }
        }
    }
    IconButtonTransparent{
        objectName: "btnGetKML"
        icon.source: "qrc:/images/BoundaryLoadFromGE.png"
        anchors.top: additives.bottom
        anchors.left: parent.left
        anchors.margins: 20
    }

    Row{
        id: saveClose
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 10
        width: children.width
        height: children.height
        spacing: 10
        IconButtonTransparent{
            onClicked: fieldFromKML.visible = false
            icon.source: "qrc:/images/Cancel64.png"
        }
        IconButtonTransparent{
            objectName: "btnSave"
            icon.source: "qrc:/images/OK64.png"
        }
    }
}
