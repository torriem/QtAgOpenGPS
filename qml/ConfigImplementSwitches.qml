import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3


Rectangle{
	id: configImplementSwitch
	anchors.fill: parent
	color: "ghostwhite"
	visible: false
	Rectangle{
		anchors.left: parent.left
		anchors.bottom: parent.bottom
		anchors.top: parent.top
		width: workswitchcolumn.width + 40
		anchors.margins: 40
		border.color: "black"
		color: "ghostwhite"
		Text{
			id: workTitle
			anchors.top: parent.top
			anchors.left: parent.left
			text: qsTr("Work Switch")
		}
		ColumnLayout{
			id: workswitchcolumn
			anchors.top: workTitle.bottom
			anchors.bottom: parent.bottom
			anchors.left: parent.left
			width: childrenRect.width
			anchors.margins: 15
			IconButtonTextBeside{
				icon.source: "/images/HydraulicLiftOff.png"
				objectName: "btnWorkSwitch"
				isChecked: false
				text: qsTr("Work Switch")
			}
			IconButtonTextBeside{
				icon.source: "/images/ManualOff.png"
				objectName: "btnWorkSwitchManual"
				isChecked: false
				text: qsTr("Manual Sections")
			}

			IconButtonTextBeside{
				objectName: "btnWorkSwitchAuto"
				icon.source: "/images/SectionMasterOff.png"
				isChecked: false
				text: qsTr("Auto Sections")
			}
			IconButton{
				implicitWidth: 270
				implicitHeight: 80
				border: 1
				id: switchStateButton
				objectName: "workSwitchstatebutton"
				isChecked: false
				icon.source: "/images/SwitchActiveClosed.png"
				color3: "white"
				colorChecked1: "green"
				colorChecked2: "green"
				colorChecked3: "green"
			}
		}
	}
	Rectangle{
		anchors.right:  parent.right
		anchors.bottom: parent.bottom
		anchors.top: parent.top
		width: steerswitchcolumn.width + 40
		anchors.margins: 40
		border.color: "black"
		color: "ghostwhite"
		Text{
			id: steerTitle
			anchors.top: parent.top
			anchors.left: parent.left
			text: qsTr("Steer Switch")
		}
		ColumnLayout{
			id: steerswitchcolumn
			anchors.top: steerTitle.bottom
			anchors.bottom: parent.bottom
			width: childrenRect.width
			anchors.right: parent.right
			anchors.margins: 15
			IconButtonTextBeside{
				objectName: "btnSteerSwitch"
				isChecked: false
				text: qsTr("Steer Switch")
				icon.source: "/images/AutoSteerOff.png"
			}
			IconButtonTextBeside{
				objectName: "btnSteerSwitchManual"
				isChecked: false
				text: qsTr("Manual Sections")
				icon.source: "/images/ManualOff.png"
			}
			IconButtonTextBeside{
				objectName: "btnSteerSwitchAuto"
				isChecked: false
				text: qsTr("Auto Sections")
				icon.source: "/images/SectionMasterOff.png"
			}
		}
	}
}
