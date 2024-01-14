import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle{
    id: line_Name
    width: 300
    height: 400

    property string title: ""

    signal accepted()
    signal rejected()
    property string abLineName //TODO: use this as input for editing the name

    function findDirection(heading) {
        if (heading > 337.5 || heading < 22.5)
        {
            return qsTr(" N ")
        }
        if (heading > 22.5 && heading < 67.5)
        {
            return qsTr(" NE ");
        }
        if (heading > 67.5 && heading < 111.5)
        {
            return qsTr(" E ");
        }
        if (heading > 111.5 && heading < 157.5)
        {
            return qsTr(" SE ");
        }
        if (heading > 157.5 && heading < 202.5)
        {
            return qsTr(" S ");
        }
        if (heading > 202.5 && heading < 247.5)
        {
            return qsTr(" SW ");
        }
        if (heading > 247.5 && heading < 292.5)
        {
            return qsTr(" W ");
        }
        if (heading > 292.5 && heading < 337.5)
        {
            return qsTr(" NW ");
        }
        return qsTr(" ?? ");
    }

    function generate_ab_name(heading_degrees) {
        var name

        name = qsTr("AB ") + (heading_degrees).toLocaleString(Qt.locale(), 'f', 1) + "\u00B0" + line_Name.findDirection(heading_degrees)

        textInputBox.text = name
    }

    color: "ghostwhite"
            border.width: 1
            border.color: "black"

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
        onClicked: {
            var time = new Date().toLocaleTimeString(Qt.locale())
            textInputBox.text += time
        }
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
            rejected()
        }
    }
    IconButtonTransparent{
        objectName: "btnOk"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 20
        icon.source: "/images/OK64.png"
        onClicked:{
            line_Name.visible = false
            abLineName = textInputBox.text
            line_Name.accepted()
        }
    }
}
