// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Menu when we want to open a field
import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Controls.Material

import ".."
import "../components"

Dialog {
    id:fieldOpen
    //color: "ghostwhite"
    visible: false
    height: 500  * theme.scaleHeight
    width:700  * theme.scaleWidth
    anchors.centerIn: parent
    modal: falce
    function show(){
        parent.visible = true
    }

    property int sortBy: 1

    TopLine{
        id: topLine
        titleText: qsTr("Open Field")
    }

    FieldTable {
        id: fieldTable
        anchors.top: topLine.bottom
        anchors.bottom: grid3.top
        width:parent.width - 10
        anchors.left: parent.left
        anchors.topMargin: 10
        anchors.leftMargin: 5
        anchors.rightMargin: 15
        anchors.bottomMargin: 10

        sortBy: fieldOpen.sortBy

        anchors.right: topLine.right
        ScrollBar.vertical: ScrollBar {
            id: scrollbar
            anchors.left: fieldOpen.right
            anchors.rightMargin: 10
            width: 10
            policy: ScrollBar.AlwaysOn
            active: true
            contentItem.opacity: 1
        }


    }

    Rectangle{
        id: grid3
        z: 4
        width: parent.width - 10
        height: deleteField.height + 10
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.bottom: parent.bottom
        //color: "white"
        color: Material.backgroundColor
        Row {
            anchors.fill: parent
            spacing: 4
            //flow: Grid.TopToBottom
            //rows: 1
            IconButtonTransparent {
                id: deleteField
                objectName: "btnDeleteField"
                icon.source: prefix + "/images/skull.png"
                text: "Delete Field"
                //radius: 0
                //color3: "white"
                //border: 1
                //height: 75
                enabled: fieldTable.currentIndex > -1
                onClicked: {
                    fieldInterface.field_delete(fieldTable.currentFieldName)
                    //backend should update the list
                }
            }
        }
        Row {
            spacing: 5
            anchors.right: parent.right
            IconButtonTransparent {
                id: sort
                icon.source: prefix + "/images/Sort.png"
                //color3: "white"
                //height: 75
                text: "Toggle Sort"
                //radius: 0

                //border: 1
                onClicked: {
                    fieldTable.sortBy = (fieldTable.sortBy % 3) + 1
                }
            }

            IconButtonTransparent {
                id: cancel
                objectName: "btnCancel"
                icon.source: prefix + "/images/Cancel64.png"
                text: "Cancel"
                //color3: "white"
                //radius: 0
                //border: 1
                //height: 75
                onClicked: {
                    fieldTable.clear_selection()
                    fieldOpen.close()
                    closeDialog()
                }
            }
            IconButtonTransparent {
                id: useSelected
                objectName: "btnUseSelected"
                icon.source: prefix + "/images/FileOpen.png"
                text: "Use Selected"
                //radius: 0
                //color3: "white"
                //border: 1
                //height: 75
                enabled: fieldTable.currentIndex > -1
                onClicked: {
                    fieldInterface.field_open(fieldTable.currentFieldName)
                    fieldTable.clear_selection()
                    fieldOpen.close()
                }
            }
        }
    }
}
