// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Menu when we load a field from KML
import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Controls.Material

import ".."
import "../components"

Dialog{
    id: fieldFromKML
    height: 400  * theme.scaleHeight
    width:700  * theme.scaleWidth
    visible: false
    function show(){
        parent.visible = true
    }
    TopLine{
        id: topLine
        titleText: qsTr("Load From KML")
    }

    Rectangle{
        id: textEntry
        width:parent.width*0.75
        height: 50  * theme.scaleHeight
        anchors.top:parent.top
        anchors.topMargin: 75
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
            icon.source: prefix + "/images/JobNameCalendar.png"
            Text{
                anchors.right: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: "+"
            }
        }
        IconButtonTransparent{
            objectName: "btnAddTime"
            icon.source: prefix + "/images/JobNameTime.png"
            Text{
                anchors.right: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: "+"
            }
        }
    }
    IconButtonTransparent{
        objectName: "btnGetKML"
        icon.source: prefix + "/images/BoundaryLoadFromGE.png"
        anchors.bottom: parent.bottom
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
            icon.source: prefix + "/images/Cancel64.png"
        }
        IconButtonTransparent{
            objectName: "btnSave"
            icon.source: prefix + "/images/OK64.png"
        }
    }
}
