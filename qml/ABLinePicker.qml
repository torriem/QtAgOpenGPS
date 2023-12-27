import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import Qt.labs.folderlistmodel 2.2
import QtQml.Models 2.3

Item {
    width: 600
    height: 400
    Rectangle{
        anchors.fill: parent
        border.width: 1
        border.color: "black"
        color: "lightgray"
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
                text: qsTr("AB Line")
                font.pixelSize: 20
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 5
            }
            Button{
                objectName: "lineHelp"
                height: parent.height
                width: parent.height * 2
                text: "?"
                font.pixelSize: 15
                anchors.right: close.left
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
            Button{
                id: close
                height: parent.height
                width: parent.height * 2
                text: "×"
                font.pixelSize: 15
                anchors.right: parent.right
                anchors.rightMargin: 2
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        ColumnLayout{
            id: rightColumn
            anchors.top: topLine.bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: 1
            anchors.bottomMargin: 1
            width: childrenRect.width
            IconButtonTransparent{
                objectName: "btnLineCopy"
                icon.source: "qrc:/images/FileCopy.png"
                onClicked: copyLineName.visible = true
            }
            IconButtonTransparent{
                objectName: "btnLineEdit"
                icon.source: "qrc:/images/FileEditName.png"
            }
            IconButtonTransparent{
                objectName: "btnLineSwapPoints"
                icon.source: "qrc:/images/ABSwapPoints.png"
            }
            IconButtonTransparent{
                objectName: "btnLineExit"
                icon.source: "qrc:/images/OK64.png"
            }
        }

        Rectangle{
            id: bottomRow
            anchors.bottom: parent.bottom
            anchors.right: rightColumn.left
            anchors.left: parent.left
            anchors.bottomMargin: 1
            anchors.leftMargin: 1
            height: btnLineDelete.height + 10
            color: parent.color
            z: 1


            RowLayout{
                anchors.fill: parent

                IconButtonTransparent{
                    id: btnLineDelete
                    objectName: "btnLineDelete"
                    icon.source: "qrc:/images/ABLineDelete.png"
                }
                IconButtonTransparent{
                    objectName: "btnLineExit"
                    icon.source: "qrc:/images/SwitchOff.png"
                    onClicked: abLinePicker.visible = false
                }
                IconButtonTransparent{
                    objectName: "btnLineAdd"
                    icon.source: "qrc:/images/AddNew.png"
                    onClicked: abSetter.visible = true
                }
                IconButtonTransparent{
                    objectName: "btnLineLoadFromKML"
                    icon.source: "qrc:/images/BoundaryLoadFromGE.png"
                }
            }
        }
        Rectangle{ //don't ask any questions
            anchors.top: topLine.bottom
            height: 10
            color: parent.color
            width: picker.width
            anchors.left: picker.left
            z: 1
        }
        Rectangle{
            id: abSetter
            anchors.left: parent.left
            anchors.top: parent.top
            width: 300
            height: 400
            color: "lightgray"
            border.width: 1
            border.color: "black"
            z: 1
            visible: false
            Rectangle{
                id: settertopLine
                color: "white"
                height: 30
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                Text{
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    text: line_Name.title
                    /*font.pixelsize: 15
            anchors.leftMargin: 5*/
                }

                Button{
                    id: lineHelp
                    objectName: "lineHelp"
                    height: parent.height
                    width: parent.height * 2
                    text: "?"
                    //font.pixelsize: 15
                    anchors.right: setterclose.left
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    background: Rectangle{color:"white"}
                }
                Button{
                    id: setterclose
                    height: parent.height
                    width: parent.height * 2
                    text: "×"
                    //font.pixelSize: 15
                    anchors.right: parent.right
                    anchors.rightMargin: 2
                    anchors.verticalCenter: parent.verticalCenter
                    background: Rectangle{color: "white"}
                }
            }
            IconButtonTransparent{
                objectName: "a"
                id: a
                anchors.top: settertopLine.bottom
                anchors.left: parent.left
                anchors.margins: 5
                isChecked: false
                icon.source: "qrc:/images/LetterABlue.png"
            }
            IconButtonTransparent{
                objectName: "b"
                anchors.top: settertopLine.bottom
                anchors.right: parent.right
                anchors.margins: 5
                isChecked: true
                icon.source: "qrc:/images/LetterBBlue.png"
            }
            Rectangle{
                    id: headingTextInput
                    anchors.topMargin: 20
                anchors.top: a.bottom
                height: 50
                width: parent.width -50
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                border.color: "gray"
                border.width: 1
                TextInput {
                    objectName: "heading"
                    anchors.fill: parent
                    validator: RegExpValidator {
                        regExp: /(\d{1,3})([.,]\d{1,5})?$/
                    }
                }
            }
           IconButtonTransparent{
               id: fancyEditor
               anchors.top: headingTextInput.bottom
               anchors.topMargin: 20
               anchors.horizontalCenter: parent.horizontalCenter
               icon.source: "qrc:/images/FileEditName.png"
           }

           IconButtonTransparent{
               objectName: "btnCancel"
               anchors.bottom: parent.bottom
               anchors.left: parent.left
               anchors.margins: 20
               icon.source: "qrc:/images/Cancel64.png"
               onClicked:{
                   parent.visible = false
               }
           }
           IconButtonTransparent{
               objectName: "btnOk"
               anchors.bottom: parent.bottom
               anchors.right: parent.right
               anchors.margins: 20
               icon.source: "qrc:/images/OK64.png"
               onClicked: newLineName.visible = true
           }
           LineName{
               id: newLineName
               objectName: "newLineName"
               anchors.top: parent.top
               anchors.left: parent.left
               title: "AB Line"
               visible: false
           }
        }

        LineName{
            id: copyLineName
            objectName: "copyLineName"
            anchors.top:parent.top
            anchors.left: parent.left
            title: "AB Line"
            visible: false
            z: 2
        }
        LineName{
            id: editLineName
            objectName: "editLineName"
            anchors.top:parent.top
            anchors.left: parent.left
            title: "AB Line"
            visible: false
            z: 1
        }

        Rectangle{
            id: picker
            anchors.left: parent.left
            anchors.top:topLine.bottom
            anchors.right: rightColumn.left
            anchors.bottom: bottomRow.top
            anchors.bottomMargin: 0
            anchors.margins: 10
            color: "white"
            ListView {
                anchors.fill: parent
                property Component mycomponent: fileName
                model: FolderListModel{
                    id: fieldList
                    folder: "file:/home/davidwedel/Documents/QtAgOpenGPS/Fields/ex"

                }

                delegate: RadioButton{
                    id: control
                    indicator: Rectangle{
                        anchors.fill: parent
                        anchors.margins: 2
                        color: control.down ? "white" : "blue"
                        visible: control.checked
                    }

                    width:parent.width
                    height:50
                    //anchors.fill: parent
                    //color: "light gray"
                    Text{
                        anchors.left: parent.left
                        anchors.leftMargin: 5
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
    }
}
