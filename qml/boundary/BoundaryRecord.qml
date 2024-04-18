import QtQuick
import QtQuick.Layouts
import Qt.labs.folderlistmodel
import QtQuick.Controls.Fusion

import ".."
import "../components"

MoveablePopup {
    id: boundaryRecord
    //anchors.top: parent.top
    //anchors.left: parent.left
    width: 250
    visible: false
    height: 415
    modal: false

    onVisibleChanged: {
        if (visible) {
            boundaryRecordBtn.checked = false
            boundaryInterface.start()
        }
    }

    Rectangle{
        id: recordTopLine
        anchors.top:parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 1
        height: 30
        color: "white"
        z: 1
        Text {
            text: qsTr("Record Boundary")
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
            anchors.right: boundaryClose.left
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
        }
        Button{
            id: boundaryClose
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
        id: recordBoundaryWindow
        anchors.top: recordTopLine.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: "lightgray"
        border.color: "aqua"
        border.width: 2
        SpinBoxCM{
            height: 50
            anchors.left: parent.left
            anchors.right: side2Record.left
            anchors.top: recordBoundaryWindow.top
            anchors.margins: 5
            objectName: "boundaryRecordOffset"
            text: "Centimeter"
            from: 0
            to: 1968
            boundValue: boundaryInterface.createBndOffset
            onValueChanged: boundaryInterface.createBndOffset = value
        }
        IconButtonTransparent{
            id: side2Record
            objectName: "btnBoundarySide2Record"
            icon.source: "/images/BoundaryRight.png"
            iconChecked: "/images/BoundaryLeft.png"
            checkable: true
            anchors.top: recordBoundaryWindow.top
            anchors.right: parent.right
            anchors.margins: 5
            border: 1
            width: 80
            height: 80
            isChecked: ! boundaryInterface.isDrawRightSide
            onClicked: {
                if (checked)
                    boundaryInterface.isDrawRightSide = false
                else
                    boundaryInterface.isDrawRightSide = true
            }
        }

        Row{
            anchors.top: side2Record.bottom
            anchors.margins: 5
            width: parent.width -10
            anchors.horizontalCenter: parent.horizontalCenter
            height: childrenRect.height
            spacing: 30
            IconButtonTransparent{
                border: 1
                id: boundaryDelete
                objectName: "recordBoundaryDelete"
                icon.source: "/images/BoundaryDelete.png"
                width: 80
                height: 80
                onClicked: boundaryInterface.reset()
            }
            Column{
                height: boundaryDelete.height
                width: boundaryDelete.width
                spacing: 20
                Text{
                    text: qsTr("Area:") + " " + utils.area_to_unit_string(boundaryInterface.area,1) + " " + utils.area_unit()
                }
                Text{
                    text: qsTr("Points:") + " " + boundaryInterface.pts
                }
            }
        }

        Grid{
            id: bottom4buttons
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 5
            height: children.height
            columns: 2
            rows: 2
            flow: Grid.LeftToRight
            spacing:	30
            IconButtonTransparent{
                border: 1
                objectName: "btnBoundaryRecordAddPoint"
                icon.source: "/images/PointAdd.png"
                width: 80
                height: 80
                onClicked: boundaryInterface.add_point()
            }
            IconButtonTransparent{
                border: 1
                objectName: "btnBoundaryRecordDeleteLastPoint"
                icon.source: "/images/PointDelete.png"
                width: 80
                height: 80
                onClicked: boundaryInterface.delete_last_point()
            }
            IconButtonTransparent{
                id: boundaryRecordBtn
                objectName: "btnBoundaryRecordRecord"
                icon.source: "/images/BoundaryRecord.png"
                checkable: true
                width: 80
                height: 80
                border: 1

                onCheckedChanged: {
                    if (checked)
                        boundaryInterface.record()
                    else
                        boundaryInterface.pause()
                }
            }

            IconButtonTransparent{
                objectName: "btnBoundaryRecordSave"
                icon.source: "/images/OK64.png"
                onClicked: {
                    boundaryRecord.visible = false
                    boundaryInterface.stop()
                }
                width: 80
                height: 80
            }
        }
    }
}
