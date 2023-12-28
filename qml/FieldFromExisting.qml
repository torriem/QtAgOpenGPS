import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import Qt.labs.folderlistmodel 2.2

Rectangle {
    id: fieldFromExisting
    height: 700
    width:1024
    color: "lightgray"
    border.color: "black"
    border.width: 1
    Item{
        id: fieldPicker
        anchors.top:parent.top
        anchors.bottom: bottomRect.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 1
        anchors.bottomMargin: 20
        Rectangle{
            id: topLine
            anchors.top:parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 40
            z: 1
            color: "ghostwhite"
            Text{
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: "Field"
                font.pixelSize: 30
            }
            Text{
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: rightText.left
                anchors.rightMargin: 50
                text: "Distance"
                font.pixelSize: 30
            }
            Text{
                id: rightText
                anchors.right: parent.right
                anchors.rightMargin: 50
                anchors.verticalCenter: parent.verticalCenter
                text: "Area"
                font.pixelSize: 30
            }
        }

        ListView {
            id: listView
            anchors.top: topLine.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            property Component mycomponent: fileName
            model :FolderListModel{
                id: fieldList
                showDirs: true
                showFiles: false
                folder: "file:/home/davidwedel/Documents/QtAgOpenGPS/Fields/"
            }

            delegate: RadioButton{
                id: control
                indicator: Rectangle{
                    border.width: 1
                    border.color: "black"
                    color: "lightgray"
                    anchors.fill: parent
                    Rectangle{
                        anchors.fill: parent
                        anchors.margins: 1
                        color: control.down ? "gray" : "blue"
                        visible: control.checked
                    }
                }

                width:listView.width
                height:50
                Text{
                    anchors.left: parent.left
                    anchors.leftMargin: 2
                    anchors.verticalCenter: parent.verticalCenter
                    text: fileName
                    font.pixelSize: 25
                    font.bold: true
                    color: control.checked ? "white" : "black"
                    z: 2
                }
            }
        }
    }
Rectangle{
    id: bottomRect
    height: 200
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    color: "ghostwhite"
    Rectangle{
        id: editFieldName
        height: 30
        width: 550
        anchors.left: parent.left
        anchors.bottom: bottomButtons.top
        anchors.bottomMargin: 50
        anchors.leftMargin: 10
        color: "lightgray"
        border.color: "darkgray"
        border.width: 1
        Text {
            anchors.left: parent.left
            anchors.bottom: parent.top
            font.bold: true
            font.pixelSize: 15
            text: qsTr("---")
        }
        TextInput{
            objectName: "lineNudgeDistance"
            anchors.fill: parent
        }
        Text {
            anchors.left: parent.left
            anchors.top: parent.bottom
            font.bold: true
            font.pixelSize: 15
            text: qsTr("Edit Field Name")
        }
    }
    IconButtonTransparent{
        anchors.verticalCenter: editFieldName.verticalCenter
        anchors.left: editFieldName.right
        anchors.leftMargin: 5
        objectName: "btnBackSpace"
        icon.source: "qrc:/images/BackSpace.png"
    }

    IconButtonTextBeside{
        anchors.verticalCenter: editFieldName.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 5
        objectName: "btnSort"
        icon.source: "qrc:/images/Sort.png"
        buttonText: "Sort"
    }

    RowLayout{
        id: bottomButtons
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: saveClose.left
        anchors.rightMargin: 10
        anchors.bottomMargin: 20
        anchors.leftMargin: 10
        width: children.width
        height: children.height
        IconButtonTransparent{
            objectName: "btnAddVehicleName"
            icon.source: "qrc:/images/Config/Con_VehicleMenu.png"
            Text{
                anchors.right: parent.left
                text: "+"
            }
        }
        IconButtonTransparent{
            objectName: "btnAddDate"
            id: marker
            icon.source: "qrc:/images/JobNameCalendar.png"
            Text{
                anchors.right: parent.left
                text: "+"
            }
        }
        IconButtonTransparent{
            objectName: "btnAddTime"
            icon.source: "qrc:/images/JobNameTime.png"
            Text{
                anchors.right: parent.left
                text: "+"
            }
        }
        IconButtonColor{
            objectName: "btnKeepFlags"
            isChecked: false
            width: marker.width
            height: marker.height
            icon: "qrc:/images/FlagRed.png"
            buttonText: "Flags"
        }
        IconButtonColor{
            objectName: "btnKeepMapping"
            isChecked: false
            width: marker.width
            height: marker.height
            icon: "qrc:/images/ManualOff.png"
            buttonText: "Mapping"
        }
        IconButtonColor{
            objectName: "btnKeepHeadland"
            isChecked: false
            width: marker.width
            height: marker.height
            icon: "qrc:/images/HeadlandMenu.png"
            buttonText: "Headland"
        }
        IconButtonColor{
            objectName: "btnKeepLines"
            isChecked: false
            width: marker.width
            height: marker.height
            icon: "qrc:/images/ABLineEdit.png"
            buttonText: "Lines"
        }
    }
    Row{
        id: saveClose
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: 5
        width: children.width
        height: children.height
        spacing: 10
        IconButtonTransparent{
            onClicked: fieldFromExisting.visible = false
            icon.source: "qrc:/images/Cancel64.png"
        }
        IconButtonTransparent{
            objectName: "btnSave"
            icon.source: "qrc:/images/OK64.png"
        }
    }
}
}
