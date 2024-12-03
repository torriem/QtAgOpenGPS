// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Record Boudnary window
import QtQuick
import QtQuick.Layouts
import Qt.labs.folderlistmodel
import QtQuick.Controls.Fusion

import ".."
import "../components"

MoveablePopup {
    id: boundaryRecord
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
    TopLine{
        id: recordTopLine
        titleText: qsTr("Record Boundary")
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
            icon.source: prefix + "/images/BoundaryRight.png"
            iconChecked: prefix + "/images/BoundaryLeft.png"
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
                icon.source: prefix + "/images/BoundaryDelete.png"
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
                icon.source: prefix + "/images/PointAdd.png"
                width: 80
                height: 80
                onClicked: boundaryInterface.add_point()
            }
            IconButtonTransparent{
                border: 1
                objectName: "btnBoundaryRecordDeleteLastPoint"
                icon.source: prefix + "/images/PointDelete.png"
                width: 80
                height: 80
                onClicked: boundaryInterface.delete_last_point()
            }
            IconButtonTransparent{
                id: boundaryRecordBtn
                objectName: "btnBoundaryRecordRecord"
                icon.source: prefix + "/images/BoundaryRecord.png"
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
                icon.source: prefix + "/images/OK64.png"
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
