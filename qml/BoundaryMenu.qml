import QtQuick 2.0
import QtQuick.Layouts 1.3
import Qt.labs.folderlistmodel 2.2
import QtQuick.Controls 2.5

Item {
    id: boundaryMenu
    width: 500
    height: 400
    TopLine{
        id: topLine
        titleText: "Start or Delete A Boundary"
    }
    Rectangle{
        id: boundaryMain
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: topLine.bottom
        border.width: 2
        border.color: "aqua"
        color: "lightgray"
        Rectangle{
            id: boundaryList
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: buttonsRow.bottom
            anchors.bottom: parent.bottom
            anchors.margins: 20
            anchors.topMargin: 50
            Text{
                anchors.bottom: parent.top
                anchors.left: parent.left
                font.pixelSize: 20
                text: "Bounds"
            }
            Text{
                anchors.bottom: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 20
                text: "Area"
            }
            Text{
                anchors.bottom: parent.top
                anchors.right: parent.right
                font.pixelSize: 20
                text: "Drive Thru"
            }
            Grid{
                anchors.fill: parent
                anchors.margins: 5
                flow: Grid.LeftToRight
                columns: 3
                rows: 4
                rowSpacing: 15
                columnSpacing: 20
                Button{
                    text: "Outer"
                    width: 150
                }
                Button{
                    text: "ar"
                    width: 150
                }
                Button{
                    text: "dt"
                    width: 70
                }
                Button{
                    text: "Inner 2"
                    width: 150
                }
                Button{
                    text: "ar"
                    width: 150
                }
                Button{
                    text: "dt"
                    width: 70
                }
                Button{
                    text: "Inner 3"
                    width: 150
                }
                Button{
                    text: "ar"
                    width: 150
                }
                Button{
                    text: "dt"
                    width: 70
                }
                Button{
                    text: "Inner 4"
                    width: 150
                }
                Button{
                    text: "ar"
                    width: 150
                }
                Button{
                    text: "dt"
                    width: 70
                }
            }
        }


        RowLayout{
            id: buttonsRow
            width: parent.width - 20
            anchors.top: parent.top
            height: children.height
            anchors.horizontalCenter: parent.horizontalCenter
            IconButtonTransparent{
                objectName: "btnBoundaryDeleteCurrent"
                icon.source: "/images/BoundaryDelete.png"
            }
            IconButtonTransparent{
                objectName: "btnBoundaryDeleteAll"
                icon.source: "/images/BoundaryDeleteAll.png"
            }
            IconButtonTransparent{
                objectName: "btnBoundaryOpenGE"
                icon.source: "/images/GoogleEarth.png"
            }
            IconButtonTransparent{
                objectName: "btnBoundaryFancyDrawer"
                icon.source: "/images/bing.png"
            }
            IconButtonTransparent{
                icon.source: "/images/AddNew.png"
                onClicked: boundaryType.visible = true

            }
            IconButtonTransparent{
                icon.source: "/images/OK64.png"
                onClicked: boundaryMenu.visible = false
            }
        }
    }
    //----------pick whether to import kml or record------------------------------------
    Item{
        id: boundaryType
        anchors.top: parent.top
        anchors.left: parent.left
        width: 250
        height: 400
        visible: false
        Rectangle{
            id: typeTopLine
            anchors.top:parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 1
            height: 30
            color: "white"
            z: 1
            Text {
                text: qsTr("Choose a Type")
                font.pixelSize: 15
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
                anchors.right: typeClose.left
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
            Button{
                id: typeClose
                height: parent.height
                width: parent.height * 2
                text: "×"
                font.pixelSize: 15
                anchors.right: parent.right
                anchors.rightMargin: 2
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Rectangle{
            id: typeMain
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: typeTopLine.bottom
            color: "lightgray"
            border.color: "aqua"
            border.width: 2
            Column{
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 10
                width: children.width
                height: children.height
                spacing: 60
                IconButtonTransparent{
                    objectName: "btnBoundaryFromKML"
                    icon.source: "/images/BoundaryLoadFromGE.png"
                    onClicked: boundaryKMLType.visible = true
                }
                IconButtonTransparent{
                    icon.source: "/images/SteerRight.png"
                    onClicked: boundaryRecord.visible = true
                    Text{
                        anchors.bottom: parent.top
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottomMargin: 20
                        text: "?"
                        font.bold: true
                    }
                }
            }
            IconButtonTransparent{
                anchors.margins: 10
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                icon.source: "/images/Cancel64.png"
                onClicked: boundaryType.visible = false
            }
        }
    }
        //----------------Window for picking whether to import 1 or several kml files
    Item{
        id: boundaryKMLType
        anchors.top: parent.top
        anchors.left: parent.left
        width: 250
        visible: false
        height: 400
        Rectangle{
            id: kmlTypeTopLine
            anchors.top:parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 1
            height: 30
            color: "white"
            z: 1
            Text {
                text: qsTr("Choose a Type")
                font.pixelSize: 15
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
                anchors.right: kmlTypeClose.left
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
            Button{
                id: kmlTypeClose
                height: parent.height
                width: parent.height * 2
                text: "×"
                font.pixelSize: 15
                anchors.right: parent.right
                anchors.rightMargin: 2
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Rectangle{
            id: kmlTypeMain
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: kmlTypeTopLine.bottom
            color: "lightgray"
            border.color: "aqua"
            border.width: 2
            Column{
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 10
                width: children.width
                height: children.height
                spacing: 60
                IconButtonTransparent{
                    objectName: "btnOneKML"
                    icon.source: "/images/BoundaryLoadFromGE.png"
                    Text{
                        anchors.left: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 20
                        text: "+"
                    }
                }
                IconButtonTransparent{
                    objectName: "btnMultiKML"
                    icon.source: "/images/BoundaryLoadMultiFromGE.png"
                    Text{
                        anchors.left: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 20
                        text: "+++"
                    }
                }
            }
            IconButtonTransparent{
                anchors.margins: 10
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                icon.source: "/images/Cancel64.png"
                onClicked: boundaryKMLType.visible = false
            }
        }
    }
    Item{//--------window for recording boundary-------------------------------------------

        id: boundaryRecord
        anchors.top: parent.top
        anchors.left: parent.left
        width: 250
        visible: false
        height: 415
        Rectangle{
            id: recordTopLine
            anchors.top:parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 1
            height: 30
            color: "white"
            z: 1
            Text {
                text: qsTr("Choose a Type")
                font.pixelSize: 15
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
                anchors.right: boundaryClose.left
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
            Button{
                id: boundaryClose
                height: parent.height
                width: parent.height * 2
                text: "×"
                font.pixelSize: 15
                anchors.right: parent.right
                anchors.rightMargin: 2
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Rectangle{
            id: recordBoundaryWindow
            anchors.top: recordTopLine.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            color: "lightgray"
            border.color: "aqua"
            border.width: 2
            SpinBoxCustomized{
                height: 50
                anchors.left: parent.left
                anchors.right: side2Record.left
                anchors.top: recordBoundaryWindow.top
                anchors.margins: 5
                objectName: "boundaryRecordOffset"
                text: "Centimeter"
                from: 0
                value: 79
                to: 1968
            }
            IconButtonTransparent{
                id: side2Record
                objectName: "btnBoundarySide2Record"
                icon.source: "/images/BoundaryRight.png"
                iconChecked: "/images/BoundaryLeft.png"
                anchors.top: recordBoundaryWindow.top
                anchors.right: parent.right
                anchors.margins: 5
                border: 1
                width: 80
                height: 80
            }
            Row{
                anchors.top: side2Record.bottom
                anchors.margins: 5
                width: parent.width -10
                anchors.horizontalCenter: parent.horizontalCenter
                height: childrenRect.height
                spacing: 30
                IconButtonTransparent{
                    border: 1
                    id: boundaryDelete
                    objectName: "recordBoundaryDelete"
                    icon.source: "/images/BoundaryDelete.png"
                    width: 80
                    height: 80
                }
                Column{
                    height: boundaryDelete.height
                    width: boundaryDelete.width
                    spacing: 20
                    Text{
                        text: "Area: 0"
                    }
                    Text{
                        text: "Points: 1"
                    }
                }
            }

            Grid{
                id: bottom4buttons
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 5
                height: children.height
                columns: 2
                rows: 2
                flow: Grid.LeftToRight
                spacing:	30
                IconButtonTransparent{
                    border: 1
                    objectName: "btnBoundaryRecordAddPoint"
                    icon.source: "/images/PointAdd.png"
                    width: 80
                    height: 80
                }
                IconButtonTransparent{
                    border: 1
                    objectName: "btnBoundaryRecordDeleteLastPoint"
                    icon.source: "/images/PointDelete.png"
                    width: 80
                    height: 80
                }
                IconButtonTransparent{
                    objectName: "btnBoundaryRecordRecord"
                    icon.source: "/images/BoundaryRecord.png"
                    width: 80
                    height: 80
                    border: 1
                }
                IconButtonTransparent{
                    objectName: "btnBoundaryRecordSave"
                    icon.source: "/images/OK64.png"
                    onClicked: boundaryRecord.visible = false
                    width: 80
                    height: 80
                }
            }
        }
    }
}
