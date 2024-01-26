import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

/*todo
  this page definitely isn't done yet. Need to add logic for
  changing one button state when it's opposite is clicked
  */
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
                checkable: true
                isChecked: settings.setF_isRemoteWorkSystemOn
                onClicked: settings.setF_isRemoteWorkSystemOn = checked
				text: qsTr("Work Switch")
			}

            ButtonGroup {
                buttons: [workSwitchMan, workSwitchAuto ]
            }

            IconButtonTextBeside{
                id: workSwitchMan
				icon.source: "/images/ManualOff.png"
				objectName: "btnWorkSwitchManual"
                checkable: true
                isChecked: settings.setF_isWorkSwitchManualSections
                onClicked: settings.setF_isWorkSwitchManualSections = checked
				text: qsTr("Manual Sections")
			}

            IconButtonTextBeside{
                id: workSwitchAuto
                objectName: "btnWorkSwitchAuto"
				icon.source: "/images/SectionMasterOff.png"
                checkable: true
                isChecked: !settings.setF_isWorkSwitchManualSections
                onClicked: settings.setF_isWorkSwitchManualSections = !checked
				text: qsTr("Auto Sections")
			}
			IconButton{
				implicitWidth: 270
				implicitHeight: 80
				border: 1
				id: switchStateButton
				objectName: "workSwitchstatebutton"
				icon.source: "/images/SwitchActiveClosed.png"
				color3: "white"
				colorChecked1: "green"
				colorChecked2: "green"
				colorChecked3: "green"
                checkable: true
                isChecked: settings.setF_isWorkSwitchActiveLow
                onCheckedChanged: settings.setF_isWorkSwitchActiveLow = checked
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
                isChecked: settings.setF_isSteerWorkSwitchEnabled
                onClicked: settings.setF_isSteerWorkSwitchEnabled = checked
				text: qsTr("Steer Switch")
				icon.source: "/images/AutoSteerOff.png"
			}

            ButtonGroup {
                buttons: [steerSwitchMan, steerSwitchAuto ]
            }

            IconButtonTextBeside{
                id: steerSwitchMan
				objectName: "btnSteerSwitchManual"
                isChecked: settings.setF_isSteerWorkSwitchManualSections
                onClicked: settings.setF_isSteerWorkSwitchManualSections = checked
                checkable: true
				text: qsTr("Manual Sections")
				icon.source: "/images/ManualOff.png"
			}
			IconButtonTextBeside{
                id: steerSwitchAuto
				objectName: "btnSteerSwitchAuto"
                isChecked: !settings.setF_isSteerWorkSwitchManualSections
                onClicked: settings.setF_isSteerWorkSwitchManualSections = !checked
				text: qsTr("Auto Sections")
				icon.source: "/images/SectionMasterOff.png"
			}
		}
	}
}
