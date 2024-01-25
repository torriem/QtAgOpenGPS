import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4

/* todo:
  don't know how to make one of 4 options here
  */
Rectangle{
	anchors.fill: parent
	visible: true
	color: "ghostwhite"
	Text{
		id: text
		anchors.top: parent.top
		anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("<h1>Attachment Style</h1>")
	}
	GridLayout{
		anchors.top: text.bottom
		anchors.horizontalCenter: parent.horizontalCenter
		width: 500
		height: 400
		rows:2
		columns: 2
        flow:Grid.TopToBottom

        ButtonGroup {
            buttons: [ i3pt, i3ptfront, itrailed, iTBT]
        }

		IconButtonColor{
			width:200
			height:150
			id: i3pt
            icon.source: "/images/ToolChkRear.png"
            checkable: true
            checked: utils.isTrue(settings.setTool_isToolRearFixed)
            onCheckedChanged: {
                settings.setTool_isToolRearFixed = checked
            }
            Connections {
                target: settings
                function onSetTool_isToolRearFixedChanged(){
                    i3pt.checked = utils.isTrue(settings.setTool_isToolRearFixed)
                }
            }
		}

		IconButtonColor{
			width:200
			height:150
			id: i3ptfront
            icon.source: "/images/ToolChkFront.png"
            checkable: true
            checked: utils.isTrue(settings.setTool_isToolFront)
            onCheckedChanged: {
                settings.setTool_isToolFront = checked
            }
            Connections {
                target: settings
                function onSetTool_isToolFrontChanged(){
                    i3ptfront.checked = utils.isTrue(settings.setTool_isToolFront)
                }
            }
        }

		IconButtonColor{
			width:200
			height:150
			id: itrailed
            icon.source: "/images/ToolChkTrailing.png"
            checkable: true
            checked: utils.isTrue(settings.setTool_isToolTrailing) && ! utils.isTrue(settings.setTool_isToolTBT)
            onCheckedChanged: {
                settings.setTool_isToolTrailing = checked
                settings.setTool_isToolTBT = false
            }
            Connections {
                target: settings
                function onSetTool_isToolTrailingChanged(){
                    if (!utils.isTrue(settings.setTool_isToolTBT))
                        itrailed.checked = utils.isTrue(settings.setTool_isToolTrailing)
                }
            }
        }
		IconButtonColor{
			width:200
			height:150
			id: iTBT
            icon.source: "/images/ToolChkTBT.png"
            checkable: true
            checked: utils.isTrue(settings.setTool_isToolTBT)
            onCheckedChanged: {
                settings.setTool_isToolTBT = true
                settings.setTool_isToolTrailing = true
            }
            Connections {
                target: settings
                function onSetTool_isToolTBTChanged(){
                    iTBT.checked = utils.isTrue(settings.setTool_isToolTBT)
                }
            }
        }
	}
}
