// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// ABLine drawer off boundary
import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

import ".."
import "../components" as Comp
//import AgOpenGPS 1.0

Item {
	id: lineDrawer
	function show(){
		lineDrawer.visible = true
	}

    Comp.TopLine{
		id: topLine
		titleText: "Click 2 points on the Boundary to Begin"
	}
	Rectangle{
		anchors.top: topLine.bottom
		anchors.left: parent.left
		anchors.right: buttons.left
		anchors.bottom:  parent.bottom
		color: "black"

		Rectangle { //temporary for qmlscene testing
		//AOGRenderer{
			id: lineDrawerField
			objectName: "lineDrawerField"
			anchors.fill: parent
            Comp.TextLine{ text: "nothing to see here yet"}

			signal clicked(var mouse)

			MouseArea {
				id: mainMouseArea
				anchors.fill: parent

				onClicked: {
					parent.clicked(mouse);
				}
			}
		}
	}
	Rectangle{
		id: buttons
		color: "lightgray"
		width: 250
		anchors.bottom: parent.bottom
		anchors.top: topLine.bottom
		anchors.right: parent.right

		//  I'll add this later, not sure how it works now.
		GridLayout{
			id: top6Buttons
			anchors.top: parent.top
			anchors.bottom: parent.bottom
			anchors.right: parent.right
			anchors.left: parent.left
			anchors.margins: 5
			columns: 2
			rows: 9
			flow: Grid.LeftToRight
            Comp.IconButtonTransparent{
				icon.source: prefix + "/images/APlusPlusB.png"
				Layout.alignment: Qt.AlignCenter
			}
            Comp.IconButtonTransparent{
				icon.source: prefix + "/images/MappingOff.png"
				Layout.alignment: Qt.AlignCenter
			}
            Comp.IconButtonTransparent{
				icon.source: prefix + "/images/APlusPlusA.png"
				Layout.alignment: Qt.AlignCenter
			}
            Comp.IconButtonTransparent{
				icon.source: prefix + "/images/HeadlandDeletePoints.png"
				Layout.alignment: Qt.AlignCenter
			}
            Comp.IconButtonTransparent{
				icon.source: prefix + "/images/TrackVisible.png"
				Layout.alignment: Qt.AlignCenter
			}
            Comp.IconButtonTransparent{
				id: boundaryCurve
				icon.source: prefix + "/images/BoundaryCurveLine.png"
				Layout.alignment: Qt.AlignCenter
				text: "Boundary Curve"
			}
            Comp.IconButtonTransparent{
				icon.source: prefix + "/images/ZoomOGL.png"
				Layout.alignment: Qt.AlignCenter
			}
            Comp.IconButtonTransparent{
				icon.source: prefix + "/images/Trash.png"
				Layout.alignment: Qt.AlignCenter
			}
            Comp.IconButtonTransparent{
				icon.source: prefix + "/images/ABTrackCurve.png"
				Layout.alignment: Qt.AlignCenter
			}
            Comp.IconButtonTransparent{
				icon.source: prefix + "/images/ABTrackAB.png"
				Layout.alignment: Qt.AlignCenter
			}
            Comp.IconButtonTransparent{
				icon.source: prefix + "/images/ABLineCycleBk.png"
				Layout.alignment: Qt.AlignCenter
			}
            Comp.IconButtonTransparent{
				icon.source: prefix + "/images/ABLineCycle.png"
				Layout.alignment: Qt.AlignCenter
			}
			Rectangle{
				id: curveNameInput
				width: parent.width - 10
				Layout.columnSpan: 2
				Layout.alignment: Qt.AlignCenter
				color: "ghostwhite"
				border.color: "darkgray"
				border.width: 1
				height: 40
				TextInput{
					id: newLine
					objectName: "drawerCurveName"
					anchors.fill: parent
				}
			}
			Text{
				text: "1/16"
				Layout.alignment: Qt.AlignCenter
			}
            Comp.IconButtonTransparent{
				icon.source: prefix + "/images/Time.png"
				Layout.alignment: Qt.AlignCenter
				onClicked: {
					var time = new Date().toLocaleTimeString(Qt.locale())
					newLine.text += " " + time
				}
			}

            Comp.IconButtonTransparent{
				icon.source: prefix + "/images/Cancel64.png"
				Layout.alignment: Qt.AlignCenter
				onClicked: lineDrawer.visible = false
			}

            Comp.IconButtonTransparent{
				objectName: "btnDrawerSave"
				icon.source: prefix + "/images/OK64.png"
				onClicked: lineDrawer.visible = false
				Layout.alignment: Qt.AlignCenter
			}
		}
	}
}
