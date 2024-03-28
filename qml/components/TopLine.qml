import QtQuick
import QtQuick.Controls
//the top line on most of the little windows. Basically
//a title, help button, and close button

Rectangle{
    property string titleText: ""
    id: topLine
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    height: 30
    color: "white"
    Text {
        text: topLine.titleText
        font.pixelSize: 20
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 5
    }
    Button{
        objectName: "lineHelp"
        height: parent.height
        width: parent.height * 2
        text: "?"
        font.pixelSize: 15
        anchors.right: close.left
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
    }
    Button{
        id: close
        height: parent.height
        width: parent.height * 2
        text: "×"
        font.pixelSize: 15
        anchors.right: parent.right
        anchors.rightMargin: 2
        anchors.verticalCenter: parent.verticalCenter
    }
}
