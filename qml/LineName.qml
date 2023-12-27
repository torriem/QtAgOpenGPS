import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle{
    id: line_Name
    width: 350
    height: 500
    color: "ghostwhite"
    property string title: ""
    Rectangle{
        id: topLine
        color: "white"
        height: 30
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        Text{
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            text: line_Name.title
            /*font.pixelsize: 15
            anchors.leftMargin: 5*/
        }

        Button{
            id: lineHelp
            objectName: "lineHelp"
            height: parent.height
            width: parent.height * 2
            text: "?"
            //font.pixelsize: 15
            anchors.right: close.left
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            background: Rectangle{
                color:"white"
            }
        }
        Button{
            id: close
            height: parent.height
            width: parent.height * 2
            text: "×"
            //font.pixelSize: 15
            anchors.right: parent.right
            anchors.rightMargin: 2
            anchors.verticalCenter: parent.verticalCenter
            background: Rectangle{color: "white"}
        }
    }
    TextInput{
        id: textInputBox
        objectName: "textInputBox"
        width: parent.width
        anchors.left: parent.left
        anchors.top: topLine.bottom
        anchors.margins: 10
        height: 80
    }

    IconButtonTransparent{
        objectName: "btnAddTime"
        anchors.top: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 20
        icon.source: "qrc:/images/Cancel64.png"
        Text{
            anchors.right: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: "+"
            /*font.bold: true
            font.pixelSize: 8*/
        }
    }
    IconButtonTransparent{
        objectName: "btnCancel"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 20
        icon.source: "qrc:/images/Cancel64.png"
        onClicked:{
            parent.visible = false
        }
    }
    IconButtonTransparent{
        objectName: "btnOk"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 20
        icon.source: "qrc:/images/OK64.png"

    }
}
