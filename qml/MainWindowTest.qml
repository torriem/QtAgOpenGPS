import QtQuick 2.7
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
                    text: qsTr("File") // icon-folder-open-empty

                    Shortcut {
                        sequence: "Alt+F"
                        onActivated: filemenu.open()
                    }
                    onClicked: filemenu.open()

                    Menu {
                        y: parent.height
                        id: filemenu


                        MenuItem {
                            text: qsTr("Load Vehicle")
                            //checkable: true
                        }
                        MenuItem {
                            text: "Save Vehicle"
                        }
                        MenuSeparator {

                        }
                        MenuItem {
                            text: "Start Field"
                        }
                    }
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

    ToolBar {
        id: toolBar
        x: 152
        y: 203
        width: 360

        ToolButton {
            id: toolButton
            x: 0
            y: 0
            text: qsTr("Blah")
        }
    }
}
