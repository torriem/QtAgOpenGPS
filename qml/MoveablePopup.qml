import QtQuick 2.0
import QtQuick.Controls 2.5

Popup {
    id: rootPopup
    MouseArea {
            anchors.fill: topBar
            property point lastMousePos: Qt.point(0, 0)
            onPressed: { lastMousePos = Qt.point(mouseX, mouseY); }
            onMouseXChanged: rootPopup.x += (mouseX - lastMousePos.x)
            onMouseYChanged: rootPopup.y += (mouseY - lastMousePos.y)
        }
    Rectangle{
        id: topBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 50
    }

}
