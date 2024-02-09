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
        BoundaryTable {
            id: boundaryList
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: buttonsRow.bottom
            anchors.bottom: parent.bottom
            anchors.margins: 20
            anchors.topMargin: 50
            ScrollBar.vertical: ScrollBar {
                id: scrollbar
                anchors.left: boundaryMain.right
                anchors.rightMargin: 10
                width: 10
                policy: ScrollBar.AlwaysOn
                active: true
                contentItem.opacity: 1
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
                enabled: (boundaryList.currentIndex === 0 &&
                          boundaryInterface.boundary_list.length === 1) ||
                         (boundaryList.currentIndex > 0)
                onClicked: boundaryInterface.delete_boundary(boundaryList.currentIndex)
            }
            IconButtonTransparent{
                objectName: "btnBoundaryDeleteAll"
                icon.source: "/images/BoundaryDeleteAll.png"

                //TODO: popup are you sure dialog
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
                onClicked: {
                    boundaryType.visible = true
                    boundaryMain.visible = false
                }

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
                    onClicked: {
                        boundaryKMLType.visible = true
                        boundaryType.visible = false
                    }
                }
                IconButtonTransparent{
                    icon.source: "/images/SteerRight.png"
                    onClicked: {
                        boundaryRecord.visible = true
                        boundaryType.visible = false
                    }
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

        onVisibleChanged: {
            if (visible) {
                boundaryRecordBtn.checked = false
                boundaryInterface.start()
            }
        }

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
            SpinBoxCM{
                height: 50
                anchors.left: parent.left
                anchors.right: side2Record.left
                anchors.top: recordBoundaryWindow.top
                anchors.margins: 5
                objectName: "boundaryRecordOffset"
                text: "Centimeter"
                from: 0
                to: 1968
                boundValue: boundaryInterface.createBndOffset
                onValueChanged: boundaryInterface.createBndOffset = value
            }
            IconButtonTransparent{
                id: side2Record
                objectName: "btnBoundarySide2Record"
                icon.source: "/images/BoundaryRight.png"
                iconChecked: "/images/BoundaryLeft.png"
                checkable: true
                anchors.top: recordBoundaryWindow.top
                anchors.right: parent.right
                anchors.margins: 5
                border: 1
                width: 80
                height: 80
                isChecked: ! boundaryInterface.isDrawRightSide
                onClicked: {
                    if (checked)
                        boundaryInterface.isDrawRightSide = false
                    else
                        boundaryInterface.isDrawRightSide = true
                }
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
                    onClicked: boundaryInterface.reset()
                }
                Column{
                    height: boundaryDelete.height
                    width: boundaryDelete.width
                    spacing: 20
                    Text{
                        text: qsTr("Area:") + " " + utils.area_to_unit_string(boundaryInterface.area,1) + " " + utils.area_unit()
                    }
                    Text{
                        text: qsTr("Points:") + " " + boundaryInterface.pts
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
                    onClicked: boundaryInterface.add_point()
                }
                IconButtonTransparent{
                    border: 1
                    objectName: "btnBoundaryRecordDeleteLastPoint"
                    icon.source: "/images/PointDelete.png"
                    width: 80
                    height: 80
                    onClicked: boundaryInterface.delete_last_point()
                }
                IconButtonTransparent{
                    id: boundaryRecordBtn
                    objectName: "btnBoundaryRecordRecord"
                    icon.source: "/images/BoundaryRecord.png"
                    checkable: true
                    width: 80
                    height: 80
                    border: 1

                    onCheckedChanged: {
                        if (checked)
                            boundaryInterface.record()
                        else
                            boundaryInterface.pause()
                    }
                }

                IconButtonTransparent{
                    objectName: "btnBoundaryRecordSave"
                    icon.source: "/images/OK64.png"
                    onClicked: {
                        boundaryRecord.visible = false
                        boundaryInterface.stop()
                    }
                    width: 80
                    height: 80
                }
            }
        }
    }
}
