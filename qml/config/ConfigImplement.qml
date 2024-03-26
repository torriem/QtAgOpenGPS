import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4

import ".."
import "../components"

/* todo:
  don't know how to make one of 4 options here
  */
Rectangle{
	anchors.fill: parent
	visible: true
    color: aog.backgroundColor
    TextLine{
		id: text
		anchors.top: parent.top
		anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("<h1>Attachment Style</h1>")
	}
	GridLayout{
		anchors.top: text.bottom
		anchors.horizontalCenter: parent.horizontalCenter
        width: 650
        height: 450
		rows:2
		columns: 2
        flow:Grid.TopToBottom

        ButtonGroup {
            buttons: [ i3pt, i3ptfront, itrailed, iTBT]
        }

		IconButtonColor{
            implicitWidth:300
            implicitHeight:200
			id: i3pt
            icon.source: "/images/ToolChkRear.png"
            checkable: true
            isChecked: settings.setTool_isToolRearFixed
            onClicked: {
                settings.setTool_isToolRearFixed = true
                settings.setTool_isToolFront = false
                settings.setTool_isToolTrailing = false
                settings.setTool_isToolTBT = false
            }
		}

		IconButtonColor{
            implicitWidth:300
            implicitHeight:200
			id: i3ptfront
            icon.source: "/images/ToolChkFront.png"
            checkable: true
            isChecked: settings.setTool_isToolFront
            onClicked: {
                settings.setTool_isToolRearFixed = false
                settings.setTool_isToolFront = true
                settings.setTool_isToolTrailing = false
                settings.setTool_isToolTBT = false
            }
        }

		IconButtonColor{
            implicitWidth:300
            implicitHeight:200
			id: itrailed
            icon.source: "/images/ToolChkTrailing.png"
            checkable: true
            isChecked: settings.setTool_isToolTrailing && ! settings.setTool_isToolTBT
            onClicked: {
                settings.setTool_isToolRearFixed = false
                settings.setTool_isToolFront = false
                settings.setTool_isToolTrailing = true
                settings.setTool_isToolTBT = false
            }
        }
		IconButtonColor{
            implicitWidth:300
            implicitHeight:200
			id: iTBT
            icon.source: "/images/ToolChkTBT.png"
            checkable: true
            isChecked: settings.setTool_isToolTBT
            onClicked: {
                settings.setTool_isToolRearFixed = false
                settings.setTool_isToolFront = false
                settings.setTool_isToolTrailing = true
                settings.setTool_isToolTBT = true
            }
        }
	}
}
