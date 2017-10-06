import QtQuick 2.0
import QtQuick.Controls 2.1

ApplicationWindow {
    id: mainWindow
    width: 640
    height: 480
    visible: true

    header: ToolBar {
        id: toolbar1
        Flow {
            id: flow1
            width: parent.width

            Row {
                id: fileRow
                ToolButton {
                    id: fileButton
                    text: "File" // icon-folder-open-empty
                }
                ToolButton {
                    id: optionsButton
                    text: "Options"
                }
                ToolButton {
                    id: toolsButton
                    text: "Tools"
                }
            }
        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            Text {
                id: speedText
                text: "0 kph"
            }
            Text {
                id: txtDistanceFromABLine
                text: "000000"
            }

        }
    }
}
