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
        TopLine{
            id: topLine
            titleText: "AB Curve"
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
                icon.source: "/images/FileCopy.png"
                onClicked: copyCurveName.visible = true
            }
            IconButtonTransparent{
                objectName: "btnLineEdit"
                 onClicked: editCurveName.visible = true
               icon.source: "/images/FileEditName.png"
            }
            IconButtonTransparent{
                objectName: "btnLineSwapPoints"
                icon.source: "/images/ABSwapPoints.png"
            }
            IconButtonTransparent{
                objectName: "btnLineExit"
                icon.source: "/images/OK64.png"
                onClicked: abCurvePicker.visible = false
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
                    icon.source: "/images/ABLineDelete.png"
                }
                IconButtonTransparent{
                    objectName: "btnLineExit"
                    icon.source: "/images/SwitchOff.png"
                    onClicked: abCurvePicker.visible = false
                }
                IconButtonTransparent{
                    objectName: "btnLineAdd"
                    icon.source: "/images/AddNew.png"
                    onClicked: abSetter.visible = true
                }
                IconButtonTransparent{
                    objectName: "btnLineLoadFromKML"
                    icon.source: "/images/BoundaryLoadFromGE.png"
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
            TopLine{
                id: settertopLine
                titleText: "AB Curve"
            }
            IconButtonTransparent{
                objectName: "a"
                id: a
                anchors.top: settertopLine.bottom
                anchors.left: parent.left
                anchors.margins: 5
                isChecked: false
                icon.source: "/images/LetterABlue.png"
            }
            IconButtonTransparent{
                objectName: "b"
                anchors.top: settertopLine.bottom
                anchors.right: parent.right
                anchors.margins: 5
                isChecked: true
                onClicked: newLineName.visible = true
                icon.source: "/images/LetterBBlue.png"
            }
            IconButtonTransparent{
                objectName: "btnRecord"
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.margins: 20
                icon.source: "/images/boundaryPause.png"
            }
            IconButtonTransparent{
                objectName: "btnCancel"
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 20
                icon.source: "/images/Cancel64.png"
                onClicked:{
                    parent.visible = false
                }
            }
            LineName{
                id: newLineName
                objectName: "newLineName"
                anchors.top: parent.top
                anchors.left: parent.left
                title: "AB Curve"
                visible: false
            }
        }

        LineName{
            id: copyCurveName
            objectName: "copyCurveName"
            anchors.top:parent.top
            anchors.left: parent.left
            title: "AB Curve"
            visible: false
            z: 2
        }
        LineName{
            id: editCurveName
            objectName: "editCurveName"
            anchors.top:parent.top
            anchors.left: parent.left
            title: "AB Curve"
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
