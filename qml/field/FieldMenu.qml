// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Main menu when we click the "Field" button on main screen. "New, Drive In, Open, Close, etc"
import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

import ".."
import "../components"

Drawer {
    id: fieldMenu
    //modal: false
    width: 270 * theme.scaleWidth
    height: mainWindow.height
    visible: false
    modal: true

    onVisibleChanged: {
        fieldInterface.field_update_list()
    }

    contentItem: Rectangle{
        id: mainFieldMenu
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        height: fieldMenu.height

        //border.color: "lightblue"
        //border.width: 2
        color: "black"

        ScrollViewExpandableColumn{
            id: fieldMenuColumn
            anchors.fill: parent
            IconButtonTextBeside{
                objectName: "btnFieldDriveIn"
                Layout.fillWidth: true
                isChecked: false
                text: "Drive In"
                icon.source: prefix + "/images/AutoManualIsAuto.png"
                onClicked: { fieldMenu.visible = false ; fieldOpen.sortBy = 2 ; fieldOpen.visible = true; }
                enabled: !aog.isJobStarted
            }
            IconButtonTextBeside{
                objectName: "btnFieldISOXML"
                isChecked: false
                text: "ISO-XML"
                icon.source: prefix + "/images/ISOXML.png"
                enabled: !aog.isJobStarted
            }
            IconButtonTextBeside{
                objectName: "btnFieldFromKML"
                isChecked: false
                text: "From KML"
                icon.source: prefix + "/images/BoundaryLoadFromGE.png"
                onClicked: fieldFromKML.visible = true
                enabled: !aog.isJobStarted
            }
            IconButtonTextBeside{
                objectName: "btnFieldFromExisting"
                isChecked: false
                text: "From Existing"
                icon.source: prefix + "/images/FileExisting.png"
                enabled: !aog.isJobStarted
                onClicked: {
                    fieldMenu.visible = false
                    fieldFromExisting.visible = true
                }
            }
            IconButtonTextBeside{
                objectName: "New"
                isChecked: false
                text: "New"
                icon.source: prefix + "/images/FileNew.png"
                enabled: !aog.isJobStarted
                onClicked: {
                    fieldMenu.visible = false
                    fieldNew.visible = true
                }
            }
            IconButtonTextBeside{
                objectName: "btnFieldResume"
                isChecked: false
                text: "Resume"
                icon.source: prefix + "/images/FilePrevious.png"
                enabled: settings.setF_CurrentDir !== "Default" && !aog.isJobStarted
                onEnabledChanged: fieldToResumeText.visible = enabled

                onClicked: {
                    fieldMenu.visible = false
                    fieldInterface.field_open(settings.setF_CurrentDir)
                }
                Text{ //show which field will be enabled
                    id: fieldToResumeText
                    visible: false
                    onVisibleChanged: console.log("rtxt: " + fieldToResumeText.visible)
                    anchors.left: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 5
                    font.pixelSize: 20
                    text: settings.setF_CurrentDir
                }
            }
            IconButtonTextBeside{
                objectName: "btnFieldClose"
                isChecked: false
                text: "Close"
                icon.source: prefix + "/images/FileClose.png"
                enabled: aog.isJobStarted
                onClicked: {
                    fieldInterface.field_close()
                    fieldMenu.visible = false
                }

            }
            IconButtonTextBeside{
                objectName: "btnFieldOpen"
                isChecked: false
                text: "Open"
                icon.source: prefix + "/images/FileOpen.png"
                enabled: !aog.isJobStarted
                onClicked: {
                    fieldMenu.visible = false
                    fieldOpen.visible = true;
                }
            }
        }
    }


    /*
        IconButtonTransparent{
            anchors.top: buttons1.bottom
            anchors.left: parent.left
            anchors.margins: 10
            anchors.leftMargin: 100
            id: fieldCancel
            icon.source: prefix + "/images/Cancel64.png"
            //buttonText: "Cancel"
            onClicked: fieldMenu.visible = false
            height: 75
        }
        */

}
