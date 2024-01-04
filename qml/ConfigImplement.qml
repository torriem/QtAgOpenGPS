import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4 as OldCtrl

Rectangle{
	anchors.fill: parent
	visible: true
	color: "ghostwhite"
	Text{
		id: text
		anchors.top: parent.top
		anchors.horizontalCenter: parent.horizontalCenter
		text: qsTr("Attachment Style")
	}
	GridLayout{
		anchors.top: text.bottom
		anchors.horizontalCenter: parent.horizontalCenter
		width: 500
		height: 400
		rows:2
		columns: 2
		flow:Grid.TopToBottom
		IconButtonColor{
			width:200
			height:150
			id: i3pt
			icon: "/images/ToolChkRear.png"
			isChecked: false
		}

		IconButtonColor{
			width:200
			height:150
			id: i3ptfront
			icon: "/images/ToolChkFront.png"
			isChecked: false
		}
		IconButtonColor{
			width:200
			height:150
			id: itrailed
			icon: "/images/ToolChkTrailing.png"
			isChecked: false
		}
		IconButtonColor{
			width:200
			height:150
			id: iTBT
			icon: "/images/ToolChkTBT.png"
			isChecked: false
		}
	}
}
