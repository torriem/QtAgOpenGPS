// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
import QtQuick
import QtQuick.Layouts
import Qt.labs.folderlistmodel
import QtQuick.Controls.Fusion
import QtQuick.Controls.Material

import ".."
import "../components"

Item {
    id: boundaryType
    width: parent.width
    height: parent.height
    function show() {
    boundaryTypePopup.visible = true
    }

    //----------pick whether to import kml or record------------------------------------
    Dialog{
        id: boundaryTypePopup
        //anchors.top: parent.top
        //anchors.left: parent.left
        height: 350  * theme.scaleHeight
        width:200  * theme.scaleWidth
        visible: boundaryType.visible
        anchors.centerIn: parent

        modal: true

        TopLine{
            id: typeTopLine
            titleText: qsTr("Choose a Type")
        }

        Rectangle{
            id: typeMain
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: typeTopLine.bottom
            color: "lightgray"
            border.color: "aqua"
            border.width: 2
            Column{
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 10
                width: children.width
                height: children.height
                spacing: 60
                IconButtonTransparent{
                    objectName: "btnBoundaryFromKML"
                    icon.source: prefix + "/images/BoundaryLoadFromGE.png"
                    onClicked: {
                        boundaryKMLType.visible = true
                        boundaryType.visible = false
                    }
                }
                IconButtonTransparent{
                    icon.source: prefix + "/images/SteerRight.png"
                    onClicked: {
                        boundaryTypePopup.visible = false
                        boundaryRecord.visible = true
                    }
                    Text{
                        anchors.bottom: parent.top
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottomMargin: 20
                        text: "?"
                        font.bold: true
                    }
                }
            }
            IconButtonTransparent{
                anchors.margins: 10
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                icon.source: prefix + "/images/Cancel64.png"
                onClicked: boundaryTypePopup.visible = false
            }
        }
    }
        //----------------Window for picking whether to import 1 or several kml files
    Dialog{
        id: boundaryKMLType
        //anchors.top: parent.top
        //anchors.left: parent.left
        height: 350  * theme.scaleHeight
        width:200  * theme.scaleWidth
        anchors.centerIn: parent
        visible: false


        TopLine{
            id: kmlTypeTopLine
            titleText: qsTr("Choose a Type")
        }

        Rectangle{
            id: kmlTypeMain
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: kmlTypeTopLine.bottom
            color: "lightgray"
            border.color: "aqua"
            border.width: 2
            Column{
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 10
                width: children.width
                height: children.height
                spacing: 60
                IconButtonTransparent{
                    objectName: "btnOneKML"
                    icon.source: prefix + "/images/BoundaryLoadFromGE.png"
                    Text{
                        anchors.left: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 20
                        text: "+"
                    }
                }
                IconButtonTransparent{
                    objectName: "btnMultiKML"
                    icon.source: prefix + "/images/BoundaryLoadMultiFromGE.png"
                    Text{
                        anchors.left: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 20
                        text: "+++"
                    }
                }
            }
            IconButtonTransparent{
                anchors.margins: 10
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                icon.source: prefix + "/images/Cancel64.png"
                onClicked: boundaryKMLType.visible = false
            }
        }
    }

    BoundaryRecord {
        id: boundaryRecord
        Material.theme: Material.Dark
        //anchors.top: parent.top
        //anchors.left: parent.left
        height: 350  * theme.scaleHeight
        width:200  * theme.scaleWidth
        //anchors.centerIn: parent
        x: 850  * theme.scaleHeight
        y:100  * theme.scaleWidth
        visible: false
        modal: false
        closePolicy: Popup.NoAutoClose
    }
}
