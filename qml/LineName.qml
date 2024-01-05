import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle{
    id: line_Name
    width: 300
    height: 400
    color: "ghostwhite"
            border.width: 1
            border.color: "black"
    property string title: ""
            TopLine{
                id: topLine
                titleText: line_Name.title
            }
    Rectangle{
        id: textInputRect
        width: parent.width -20
        anchors.left: parent.left
        anchors.top: topLine.bottom
        anchors.margins: 10
        height: 80
        color:"lightgray"
        border.color: "gray"
        TextInput{
            id: textInputBox
            anchors.fill: parent
            objectName: "textInputBox"
            text: ""
            cursorVisible: true
        }
    }

    IconButtonTransparent{
        objectName: "btnAddTime"
        anchors.top: textInputRect.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 20
        icon.source: "/images/Time.png"
        Text{
            anchors.right: parent.left
            anchors.rightMargin: 30
            anchors.verticalCenter: parent.verticalCenter
            text: "+"
            font.bold: true
            font.pixelSize: 35
        }
    }
    IconButtonTransparent{
        objectName: "btnCancel"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 20
        icon.source: "/images/Cancel64.png"
        onClicked:{
            parent.visible = false
        }
    }
    IconButtonTransparent{
        objectName: "btnOk"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 20
        icon.source: "/images/OK64.png"

    }
}
