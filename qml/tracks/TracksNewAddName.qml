import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ".."
import "../components"

MoveablePopup {
	id: trackAddName
	property string defaultText: "blah"
	width: 200 * theme.scaleWidth
	height: 300 * theme.scaleHeight
	function show(textString) {
		trackAddName.visible = true
		trackAddName.defaultText = textString
	}

	GridLayout {
		id: trackAddNameLayout
		anchors.centerIn: parent
		width: parent.width *.9
		height: parent.height *.9
		flow: Grid.leftToRight
		rows: 4
		columns: 2
		Text {
			text: qsTr("Enter Track Name:")
			Layout.columnSpan: 2
		}
		TextField {
			id: trackName
			Layout.columnSpan: 2
			text: trackAddName.defaultText
		}
		IconButtonTransparent{
			Layout.columnSpan: 2
			Layout.alignment: Qt.AlignCenter
			icon.source: "/images/JobNameTime.png"
			Text{
				anchors.right: parent.left
				anchors.verticalCenter: parent.verticalCenter
				text: "+"
			}
			onClicked: {
				var time = new Date().toLocaleTimeString(Qt.locale())
				trackName.text += " " + time
			}
		}

		IconButtonTransparent {
			icon.source: "/images/Cancel64.png"
			onClicked: {
				trackAddName.visible = false
			}
		}
		IconButtonTransparent {
			icon.source: "/images/OK64.png"
			onClicked: {
				trackAddName.visible = false
			}
		}
	}
}

