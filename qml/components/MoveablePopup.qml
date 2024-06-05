import QtQuick
import QtQuick.Controls.Fusion

Popup {
	id: rootPopup
	padding: 0
	closePolicy: Popup.NoAutoClose
	property alias topBar: topBar
	MouseArea {
		id: topBar
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		height: 50
		property point lastMousePos: Qt.point(0, 0)
		onPressed: { lastMousePos = Qt.point(mouseX, mouseY); }
		onMouseXChanged: rootPopup.x += (mouseX - lastMousePos.x)
		onMouseYChanged: rootPopup.y += (mouseY - lastMousePos.y)
	}
	background: Rectangle {
		id: background
		color: aog.backgroundColor
		border.width: 1
		border.color: aog.blackDayWhiteNight
	}
}
