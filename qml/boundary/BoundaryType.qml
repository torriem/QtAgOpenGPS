import QtQuick
import QtQuick.Layouts
import Qt.labs.folderlistmodel
import QtQuick.Controls

import ".."
import "../components"

Item {
    id: boundaryType

    function show() {
        boundaryTypePopup.visible = true
    }

    //----------pick whether to import kml or record------------------------------------
    Popup{
        id: boundaryTypePopup
        //anchors.top: parent.top
        //anchors.left: parent.left
        width: 250
        height: 400
        visible: boundaryType.visible
        modal: true

        Rectangle{
            id: typeTopLine
            anchors.top:parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 1
            height: 30
            color: "white"
            z: 1
            Text {
                text: qsTr("Choose a Type")
                font.pixelSize: 15
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
                anchors.right: typeClose.left
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
            Button{
                id: typeClose
                height: parent.height
                width: parent.height * 2
                text: "×"
                font.pixelSize: 15
                anchors.right: parent.right
                anchors.rightMargin: 2
                anchors.verticalCenter: parent.verticalCenter
            }
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
                    icon.source: "/images/BoundaryLoadFromGE.png"
                    onClicked: {
                        boundaryKMLType.visible = true
                        boundaryType.visible = false
                    }
                }
                IconButtonTransparent{
                    icon.source: "/images/SteerRight.png"
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
                icon.source: "/images/Cancel64.png"
                onClicked: boundaryTypePopup.visible = false
            }
        }
    }
        //----------------Window for picking whether to import 1 or several kml files
    Popup{
        id: boundaryKMLType
        //anchors.top: parent.top
        //anchors.left: parent.left
        width: 250
        visible: false
        height: 400
        Rectangle{
            id: kmlTypeTopLine
            anchors.top:parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 1
            height: 30
            color: "white"
            z: 1
            Text {
                text: qsTr("Choose a Type")
                font.pixelSize: 15
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
                anchors.right: kmlTypeClose.left
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
            Button{
                id: kmlTypeClose
                height: parent.height
                width: parent.height * 2
                text: "×"
                font.pixelSize: 15
                anchors.right: parent.right
                anchors.rightMargin: 2
                anchors.verticalCenter: parent.verticalCenter
            }
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
                    icon.source: "/images/BoundaryLoadFromGE.png"
                    Text{
                        anchors.left: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 20
                        text: "+"
                    }
                }
                IconButtonTransparent{
                    objectName: "btnMultiKML"
                    icon.source: "/images/BoundaryLoadMultiFromGE.png"
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
                icon.source: "/images/Cancel64.png"
                onClicked: boundaryKMLType.visible = false
            }
        }
    }

    BoundaryRecord {
        id: boundaryRecord
        //anchors.top: parent.top
        //anchors.left: parent.left
        width: 250
        visible: false
        height: 415
        modal: false
        closePolicy: Popup.NoAutoClose
    }
}
