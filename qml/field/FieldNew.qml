import QtQuick 2.15
import QtQuick.Controls 2.15

import ".."
import "../components"

Popup {
    id: fieldNew
    width: 500
    height: 300

    onVisibleChanged: {
        if (visible)
            newField.text = ""
    }


    //color: "lightgray"
    Rectangle{
        id: textEntry
        width: 450
        height: 40
        anchors.top:parent.top
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        color: "ghostwhite"
        border.color: "darkgray"
        border.width: 1
        Text {
            id: newFieldLabel
            anchors.left: parent.left
            anchors.bottom: parent.top
            font.bold: true
            font.pixelSize: 15
            text: qsTr("Enter Field Name")
        }
        TextField{
            id: newField
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: newFieldLabel.bottom
            selectByMouse: true
            placeholderText: "New Field Name"
            onTextChanged: {
                for (var i=0; i < fieldInterface.field_list.length ; i++) {
                    if (text === fieldInterface.field_list[i].name) {
                        errorMessage.visible = true
                        break
                    } else
                        errorMessage.visible = false
                }
            }
        }
        Text {
            id: errorMessage
            anchors.top: newField.bottom
            anchors.left: newField.left
            color: "red"
            visible: false
            text: qsTr("This field exists already; please choose another name.")
        }
    }
    Row{
        anchors.left: parent.left
        anchors.top: textEntry.bottom
        anchors.margins: 30
        spacing: 30
        IconButtonTransparent{
            objectName: "btnAddDate"
            id: marker
            icon.source: "/images/JobNameCalendar.png"
            Text{
                anchors.right: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: "+"
            }
            onClicked: {
                var time = new Date().toLocaleDateString(Qt.locale(), Locale.ShortFormat)
                newField.text += " " + time
            }

        }
        IconButtonTransparent{
            objectName: "btnAddTime"
            icon.source: "/images/JobNameTime.png"
            Text{
                anchors.right: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: "+"
            }
            onClicked: {
                var time = new Date().toLocaleTimeString(Qt.locale())
                newField.text += " " + time
            }
        }
    }

    Row{
        id: saveClose
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 10
        width: children.width
        height: children.height
        spacing: 10
        IconButtonTransparent{
            onClicked: {
                fieldNew.visible = false
                newField.text = ""
            }
            icon.source: "/images/Cancel64.png"
        }
        IconButtonTransparent{
            enabled: newField.text != "" && errorMessage.visible == false;
            objectName: "btnSave"
            icon.source: "/images/OK64.png"

            onClicked: {
                fieldNew.visible = false
                fieldInterface.field_new(newField.text)
            }
        }
    }
}
