import QtQuick 2.0
import QtQuick.Controls.Fusion

import "components"

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
        ScrollViewExpandableColumn{
            id: hamburgerMenuColumn
            anchors.fill: parent
            ButtonColor{
                text: "Languages"
                onClicked: console.log("")
                width: 250
                height: 50
                visible: false//todo
            }
            ButtonColor{
                text: "Directories"
                onClicked: console.log("")
                width: 250
                height: 50
                visible: false//todo
            }
            ButtonColor{
                text: "Colors"
                onClicked: console.log("")
                width: 250
                height: 50
            }
            ButtonColor{
                text: "Section Colors"
                onClicked: console.log("")
                width: 250
                height: 50
            }
            ButtonColor{
                text: "Enter Sim Coords"
                onClicked: {
                    console.log("showing")
                    hamburgerMenuRoot.visible = false
                    setSimCoords.show()
                }
                width: 250
                height: 50
            }
            ButtonColor{
                text: "Simulator On"
                property bool isChecked: settings.setMenu_isSimulatorOn
                onIsCheckedChanged: {
                    checked = isChecked
                }

                checkable: true
                checked: isChecked
                onCheckedChanged: {
                    settings.setMenu_isSimulatorOn = checked
                    console.log("Sim = "+settings.setMenu_isSimulatorOn)
                }
                width: 250
                height: 50
            }
            ButtonColor{
                text: "Reset All"
                onClicked: console.log("")
                width: 250
                height: 50
                visible: false//todo
            }
            ButtonColor{
                text: "HotKeys"
                onClicked: console.log("")
                width: 250
                height: 50
                visible: false//todo
            }
            ButtonColor{
                text: "About..."
                onClicked: console.log("")
                width: 250
                height: 50
                visible: false//todo
            }
            ButtonColor{
                text: "Help"
                onClicked: console.log("")
                width: 250
                height: 50
                visible: false//todo
            }
        }
    }
}
