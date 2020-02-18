import QtQuick 2.0

Rectangle {
    id: rectangle
    width: childrenRect.width+10
    height: childrenRect.height+10
    color: "#bf163814"
    visible: true
    border.color: "#c3ecc0"
    radius: 20

    Grid {
        id: grid
        width: childrenRect.width
        height: childrenRect.height
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
        spacing: 6
        flow: Grid.TopToBottom
        rows: 4
        columns: 3

        IconButton {
            id: btnTiltDown
            objectName: "btnTiltDown"
            width: 70
            height: 70
            radius: 10
            icon: "qrc:/images/TiltDown.png"
        }

        IconButton {
            id: btnTiltUp
            objectName: "btnTiltUp"
            width: 70
            height: 70
            radius: 10
            icon: "qrc:/images/TiltUp.png"
        }

        IconButton {
            id: btnZoomIn
            objectName: "btnZoomIn"
            width: 70
            height: 70
            radius: 10
            icon: "qrc:/images/ZoomIn48.png"
        }

        IconButton {
            id: btnZoomOut
            objectName: "btnZoomOut"
            width: 70
            height: 70
            radius: 10
            icon: "qrc:/images/ZoomOut48.png"
        }
        IconButton {
            id: btnTwoD
            objectName: "btnTwoD"
            width: 70
            height: 70
            radius: 10
            icon: "qrc:/images/Camera2D64.png"
        }

        IconButton {
            id: btnThreeD
            objectName: "btnThreeD"
            width: 70
            height: 70
            radius: 10
            icon: "qrc:/images/Camera3D64.png"
        }
    }
}
