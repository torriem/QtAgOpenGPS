import QtQuick 2.0
import QtQuick.Controls 2.1
import Qt.labs.folderlistmodel 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

Item{
        Rectangle{
            id: topLine
            anchors.top:parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 1
            height: 30
            color: "white"
            z: 1
            Text {
                text: qsTr("Start New Field")
                font.pixelSize: 20
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 5
            }
        }
   Rectangle{
       id: mainFieldMenu
       anchors.bottom: parent.bottom
       anchors.left: parent.left
       anchors.right: parent.right
       anchors.top: topLine.bottom
       border.color: "lightblue"
       border.width: 2
       color: "black"
       ColumnLayout{
           anchors.left: parent.left
           anchors.leftMargin: 40
           anchors.top: parent.top
           anchors.topMargin: 5
           anchors.bottom: parent.bottom
           anchors.bottomMargin: 5
           width: children.width
           IconButtonTextBeside{
               objectName: "btnFieldDriveIn"
               isChecked: false
               text: "Drive In"
               icon.source: "qrc:/images/AutoManualIsAuto.png"
           }
           IconButtonTextBeside{
               objectName: "btnFieldISOXML"
               isChecked: false
               text: "ISO-XML"
               icon.source: "qrc:/images/ISOXML.png"
           }
           IconButtonTextBeside{
               objectName: "btnFieldFromKML"
               isChecked: false
               text: "From KML"
               icon.source: "qrc:/images/BoundaryLoadFromGE.png"
           }
           IconButtonTextBeside{
               objectName: "btnFieldFromExisting"
               isChecked: false
               text: "From Existing"
               icon.source: "qrc:/images/FileExisting.png"
           }
           IconButtonTextBeside{
               objectName: "New"
               isChecked: false
               text: "New"
               icon.source: "qrc:/images/FileNew.png"
           }
       }
       ColumnLayout{
           anchors.right: parent.right
           anchors.rightMargin: 40
           anchors.top: parent.top
           anchors.topMargin: 5
           anchors.bottom: fieldCancel.top
           anchors.bottomMargin: 5
           width: children.width
           IconButtonTextBeside{
               objectName: "btnFieldResume"
               isChecked: false
               text: "Resume"
               icon.source: "qrc:/images/FilePrevious.png"
           }
           IconButtonTextBeside{
               objectName: "btnFieldClose"
               isChecked: false
               text: "Close"
               icon.source: "qrc:/images/FileClose.png"
           }
           IconButtonTextBeside{
               objectName: "btnFeldOpen"
               isChecked: false
               text: "Open"
               icon.source: "qrc:/images/FileOpen"
                onClicked: fieldWindow.visible = true;
           }
       }
            IconButtonTransparent{
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 10
                id: fieldCancel
                icon.source: "qrc:/images/Cancel64.png"
                //buttonText: "Cancel"
                onClicked: fieldMenu.visible = false
                height: 75
            }

       FieldWindow{
           anchors.top:parent.top
           anchors.horizontalCenter: parent.horizontalCenter
           visible: false

       }
   }
}
