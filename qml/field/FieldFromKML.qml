import QtQuick
import QtQuick.Controls

import ".."
import "../components"

Popup{
    id: fieldFromKML
    width: 500
    height: 450
    visible: false
    function show(){
        parent.visible = true
    }
    TopLine{
        id: topLine
        width: parent.width
        height: screenPixelDensity *.5
        titleText: "Load From KML"
    }

    Rectangle{
        id: textEntry
        width: 450
        height: 30
        anchors.top:parent.top
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        color: aog.backgroundColor
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
            icon.source: "/images/JobNameCalendar.png"
            Text{
                anchors.right: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: "+"
            }
        }
        IconButtonTransparent{
            objectName: "btnAddTime"
            icon.source: "/images/JobNameTime.png"
            Text{
                anchors.right: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: "+"
            }
        }
    }
    IconButtonTransparent{
        objectName: "btnGetKML"
        icon.source: "/images/BoundaryLoadFromGE.png"
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
            icon.source: "/images/Cancel64.png"
        }
        IconButtonTransparent{
            objectName: "btnSave"
            icon.source: "/images/OK64.png"
        }
    }
}
