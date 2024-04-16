import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ".."
import "../components"

Drawer {
    id: fieldMenu
    //modal: false
    //height: mainWindow.height
    width: 300
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
                icon.source: "/images/AutoManualIsAuto.png"
                onClicked: { fieldMenu.visible = false ; fieldOpen.sortBy = 2 ; fieldOpen.visible = true; }
            }
            IconButtonTextBeside{
                objectName: "btnFieldISOXML"
                isChecked: false
                text: "ISO-XML"
                icon.source: "/images/ISOXML.png"
            }
            IconButtonTextBeside{
                objectName: "btnFieldFromKML"
                isChecked: false
                text: "From KML"
                icon.source: "/images/BoundaryLoadFromGE.png"
                onClicked: fieldFromKML.visible = true
            }
            IconButtonTextBeside{
                objectName: "btnFieldFromExisting"
                isChecked: false
                text: "From Existing"
                icon.source: "/images/FileExisting.png"
                onClicked: {
                    fieldMenu.visible = false
                    fieldFromExisting.visible = true
                }
            }
            IconButtonTextBeside{
                objectName: "New"
                isChecked: false
                text: "New"
                icon.source: "/images/FileNew.png"
                onClicked: {
                    fieldMenu.visible = false
                    fieldNew.visible = true
                }
            }
            IconButtonTextBeside{
                objectName: "btnFieldResume"
                isChecked: false
                text: "Resume"
                icon.source: "/images/FilePrevious.png"
                enabled: settings.setF_CurrentDir !== "Default"

                onClicked: {
                    fieldMenu.visible = false
                    fieldInterface.field_open(settings.setF_CurrentDir)
                }
            }
            IconButtonTextBeside{
                objectName: "btnFieldClose"
                isChecked: false
                text: "Close"
                icon.source: "/images/FileClose.png"
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
                icon.source: "/images/FileOpen"
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
            icon.source: "/images/Cancel64.png"
            //buttonText: "Cancel"
            onClicked: fieldMenu.visible = false
            height: 75
        }
        */

}
