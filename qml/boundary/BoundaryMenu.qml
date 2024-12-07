// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Shows all boundaries + add + delete
import QtQuick
import QtQuick.Layouts
import Qt.labs.folderlistmodel
import QtQuick.Controls.Fusion
import QtQuick.Controls.Material

import ".."
import "../components"

Item {
    id: boundaryMenu

    width: parent.width
    height: parent.height
    function show() {
        boundaryInterface.update_list()
        boundaryMenuPopup.visible = true
    }

    Dialog {
        id: boundaryMenuPopup
        height: 500  * theme.scaleHeight
        width:700  * theme.scaleWidth
        modal: true
        visible: boundaryMenu.visible
        anchors.centerIn: parent
        TopLine{
            id: topLine
            titleText: "Start or Delete A Boundary"
        }
        Rectangle{
            id: boundaryMain
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: topLine.bottom
            border.width: 2
            border.color: "aqua"
            color: "lightgray"
            BoundaryTable {
                id: boundaryList
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: buttonsRow.bottom
                anchors.bottom: parent.bottom
                anchors.margins: 20
                anchors.topMargin: 50
                ScrollBar.vertical: ScrollBar {
                    id: scrollbar
                    anchors.right: parent.right
                    active: true
                    contentItem.opacity: 1
                    policy: ScrollBar.AlwaysOn
                    width: 10
                }

                onChangeDriveThrough: {
                    boundaryInterface.set_drive_through(index, drive_through)
                }

            }

            RowLayout{
                id: buttonsRow
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                height: children.height
                width: parent.width - 20
                IconButtonTransparent{
                    objectName: "btnBoundaryDeleteCurrent"
                    enabled: (boundaryList.currentIndex === 0 &&
                              boundaryInterface.boundary_list.length === 1) ||
                             (boundaryList.currentIndex > 0)
                    icon.source: prefix + "/images/BoundaryDelete.png"
                    onClicked: boundaryInterface.delete_boundary(boundaryList.currentIndex)
                }
                IconButtonTransparent{
                    objectName: "btnBoundaryDeleteAll"
                    icon.source: prefix + "/images/BoundaryDeleteAll.png"
                    onClicked: {
                        deleteAllSure.visible = true
                    }

                    //TODO: popup are you sure dialog
                }
                IconButtonTransparent{
                    objectName: "btnBoundaryOpenGE"
                    icon.source: prefix + "/images/GoogleEarth.png"
                }
                IconButtonTransparent{
                    objectName: "btnBoundaryFancyDrawer"
                    icon.source: prefix + "/images/bing.png"
                }
                IconButtonTransparent{
                    icon.source: prefix + "/images/AddNew.png"
                    onClicked: {
                        boundaryMenuPopup.visible = false
                        boundaryType.show()
                    }

                }
                IconButtonTransparent{
                    icon.source: prefix + "/images/OK64.png"
                    onClicked: boundaryMenuPopup.visible = false
                }
            }
        }
    }
    Dialog {
        id: deleteAllSure
        visible: false
        title: qsTr("Delete All Boundaries?")
        standardButtons: Dialog.No | Dialog.Yes
        height: 300  * theme.scaleHeight
        width: 500  * theme.scaleWidth
        modal: true

        onAccepted: {
            boundaryInterface.delete_all()
        }

        Text {
            anchors.fill: parent
            text: qsTr("Are you sure you wish to delete all boundaries for this field? This action cannot be undone.")
            wrapMode: Text.WordWrap
        }

    }
    BoundaryType {
        id: boundaryType
        visible: false
    }
}
