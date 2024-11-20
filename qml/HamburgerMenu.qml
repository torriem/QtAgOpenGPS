// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Top left button menu
import QtQuick 2.0
import QtQuick.Controls.Fusion

import "components" as Comp

Drawer{
    id: hamburgerMenuRoot
    width: 300
    height: mainWindow.height
    visible: false
    modal: true
    contentItem: Rectangle{
        id: hamburgerMenuContent
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        height: fieldMenu.height

        color: "black"
        Comp.ScrollViewExpandableColumn{
            id: hamburgerMenuColumn
            anchors.fill: parent
            Comp.ButtonColor{
                text: "Languages"
                onClicked: console.log("")
                width: 250
                height: 50
                visible: false//todo
            }
            Comp.ButtonColor{
                text: "Directories"
                onClicked: console.log("")
                width: 250
                height: 50
                visible: false//todo
            }
            Comp.ButtonColor{
                text: "Colors"
                onClicked: console.log("")
                width: 250
                height: 50
            }
            Comp.ButtonColor{
                text: "Section Colors"
                onClicked: console.log("")
                width: 250
                height: 50
            }
            Comp.ButtonColor{
                text: "Enter Sim Coords"
                onClicked: {
                    console.log("showing")
                    hamburgerMenuRoot.visible = false
                    setSimCoords.show()
                }
                width: 250
                height: 50
            }
            Comp.ButtonColor{
                text: "Simulator On"
                property bool isChecked: settings.setMenu_isSimulatorOn
                onIsCheckedChanged: {
                    checked = isChecked
                }

                checkable: true
                checked: isChecked
                onCheckedChanged: {
                    settings.setMenu_isSimulatorOn = checked
                }
                width: 250
                height: 50
            }
            Comp.ButtonColor{
                text: "Reset All"
                onClicked: console.log("")
                width: 250
                height: 50
                visible: false//todo
            }
            Comp.ButtonColor{
                text: "HotKeys"
                onClicked: console.log("")
                width: 250
                height: 50
                visible: false//todo
            }
            Comp.ButtonColor{
                text: "About..."
                onClicked: console.log("")
                width: 250
                height: 50
                visible: false//todo
            }
            Comp.ButtonColor{
                text: "Help"
                onClicked: console.log("")
                width: 250
                height: 50
                visible: false//todo
            }
        }
    }
}
