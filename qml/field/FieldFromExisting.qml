// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Menu when we create a field from existing
import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtQuick.Controls.Material

import ".."
import "../components"

Dialog {
    id: fieldFromExisting
    height: 700  * theme.scaleHeight
    width:1000  * theme.scaleWidth
    anchors.centerIn: parent
    visible: false
    function show(){
        parent.visible = true
    }
    TopLine{
        id: topLine
        titleText: qsTr("Create New Field from Existing Field")
    }

    FieldTable {
        id: fieldView
        anchors.top: topLine.bottom
        anchors.bottom: bottomRect.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: 20

        ScrollBar.vertical: ScrollBar {
            id: scrollbar
            anchors.left: fieldFromExisting.right
            anchors.rightMargin: 10
            width: 10
            policy: ScrollBar.AlwaysOn
            active: true
            contentItem.opacity: 1
        }

        onCurrentFieldNameChanged: {
            existingField.text = currentFieldName
            newField.text = currentFieldName
        }
    }

    Rectangle{
        id: bottomRect
        height: parent.height*0.4
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        color: aog.backgroundColor
        Rectangle{
            id: editFieldName
            height: parent.height*0.15
            width: parent.width*0.5
            anchors.left: parent.left
            anchors.bottom: bottomButtons.top
            anchors.bottomMargin: 50
            anchors.topMargin: 20
            anchors.leftMargin: 10
            color: "lightgray"
            border.color: "darkgray"
            border.width: 1
            Text {
                id: existingField
                anchors.left: parent.left
                anchors.bottom: parent.top
                font.bold: true
                font.pixelSize: 15
                text: qsTr("---")
            }
            TextField{
                id: newField
                objectName: "fieldFromExisting"
                placeholderText: qsTr("New field name")
                anchors.fill: parent
                selectByMouse: true

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
                id: newFieldLabel
                anchors.left: parent.left
                anchors.top: parent.bottom
                font.bold: true
                font.pixelSize: 15
                text: qsTr("Edit Field Name")
            }
            Text {
                id: errorMessage
                anchors.top: newFieldLabel.bottom
                anchors.left: newFieldLabel.left
                color: "red"
                visible: false
                text: qsTr("This field exists already; please change the new name.")
            }

        }
        IconButtonTransparent{
            anchors.verticalCenter: editFieldName.verticalCenter
            anchors.left: editFieldName.right
            anchors.leftMargin: 5
            objectName: "btnBackSpace"
            icon.source: prefix + "/images/BackSpace.png"
            onClicked: newField.text = ""
            height: 50  * theme.scaleHeight
            width: 100  * theme.scaleWidth

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
                id: btnAddVehicleName
                icon.source: prefix + "/images/Config/Con_VehicleMenu.png"
                Text{
                    anchors.right: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    text: "+"
                }
                onClicked: {
                    newField.text += " " + settings.setVehicle_vehicleName
                }
            }
            IconButtonTransparent{
                id: marker
                icon.source: prefix + "/images/JobNameCalendar.png"
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
                id: btnAddTime
                icon.source: prefix + "/images/JobNameTime.png"
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
            IconButtonColor{
                id: btnKeepFlags
                checkable: true
                checked: false
                width: marker.width
                height: marker.height
                icon.source: prefix + "/images/FlagRed.png"
                text: "Flags"
            }
            IconButtonColor{
                id: btnKeepMapping
                checkable: true
                checked: false
                width: marker.width
                height: marker.height
                icon.source: prefix + "/images/ManualOff.png"
                text: "Mapping"
            }
            IconButtonColor{
                id: btnKeepHeadland
                checkable: true
                checked: true
                width: marker.width
                height: marker.height
                icon.source: prefix + "/images/HeadlandMenu.png"
                text: "Headland"
            }
            IconButtonColor{
                id: btnKeepLines
                checkable: true
                checked: true
                width: marker.width
                height: marker.height
                icon.source: prefix + "/images/ABLineEdit.png"
                text: "Lines"
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
                onClicked: {
                    fieldFromExisting.visible = false
                    newField.text = ""
                    existingField.text = ""
                    fieldView.clear_selection()
                }
                icon.source: prefix + "/images/Cancel64.png"
            }
            IconButtonTransparent{
                objectName: "btnSave"
                icon.source: prefix + "/images/OK64.png"
                enabled: (newField.text !== "" && existingField.text !== "" &&
                          newField.text != existingField.text &&
                          errorMessage.visible === false)
                onClicked: {
                    if(newField.text !== "" && existingField.text !== "" &&
                            newField.text != existingField.text) {
                        var flag = 0;
                        if (btnKeepFlags.checked)
                            flag |= fieldInterface.loadFlags
                        if (btnKeepHeadland.checked)
                            flag |= fieldInterface.loadHeadland
                        if (btnKeepLines.checked)
                            flag |= fieldInterface.loadLines
                        if (btnKeepMapping.checked)
                            flag |= fieldInterface.loadMapping

                        fieldFromExisting.visible = false
                        fieldInterface.field_new_from(fieldView.currentFieldName, newField.text, flag)
                        newField.text = ""
                        existingField.text = ""
                        fieldView.clear_selection()
                    }
                }
            }
        }
    }
}
