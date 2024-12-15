// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
//
import QtQuick
import QtQuick.Layouts
import Qt.labs.folderlistmodel
import QtQuick.Controls.Fusion
import QtQuick.Controls.Material

import ".."
import "components" as Comp


Item {

    id: mainWindowTram
    width: parent.width
    height: parent.height


    Popup {
        //anchors.left: parent.left
        //anchors.bottom: parent.bottom
        //anchors.right: parent.right
        //anchors.top: topLine.bottom
        height: 600  * theme.scaleHeight
        width: 300  * theme.scaleWidth
        modal: false
        visible: mainWindowTram.visible
        anchors.centerIn: parent
        Comp.TopLine{
            id: tramtopLine
            titleText: "Tram Lines"
        }
        Row{
            anchors.top: tramtopLine.bottom
            anchors.topMargin: 20  * theme.scaleHeight
            anchors.horizontalCenter: parent.horizontalCenter
            id: tramHalfNudge
            width: children.width
            spacing: 25  * theme.scaleWidth
            height: children.height
            Comp.IconButtonTransparent{
                icon.source: prefix + "/images/SnapLeftHalf.png"
            }
            Text{
                text: "2.00 m"
                font.bold: true
                font.pixelSize: 15
                anchors.verticalCenter: parent.verticalCenter
            }

            Comp.IconButtonTransparent{
                icon.source: prefix + "/images/SnapRightHalf.png"
            }
        }
        Row{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: tramHalfNudge.bottom
            anchors.topMargin: 30  * theme.scaleHeight
            id: tramSmallNudge
            width: children.width
            spacing: 25  * theme.scaleWidth
            height: children.height
            Comp.IconButtonTransparent{
                icon.source: prefix + "/images/ArrowLeft.png"
            }
            Text{
                text: "10 cm"
                font.bold: true
                font.pixelSize: 15
                anchors.verticalCenter: parent.verticalCenter
            }

            Comp.IconButtonTransparent{
                icon.source: prefix + "/images/ArrowRight.png"
            }
        }
        Row{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: tramSmallNudge.bottom
            anchors.topMargin: 30  * theme.scaleHeight
            id: tramSwapModeRow
            width: children.width
            spacing: 25  * theme.scaleWidth
            height: children.height
			Comp.IconButtonTransparent{
				id: tramSwapAB
				icon.source: prefix + "/images/ABSwapPoints.png"
			}
			Text {
				text: qsTr("Mode")
				font.pixelSize: 15
				anchors.verticalCenter: parent.verticalCenter
			}
			Comp.IconButtonTransparent{
				id: tramSwapMode
				icon.source: prefix + "/images/TramLines.png"
			}
        }
        Row{
            id: tramPasses
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: tramSwapModeRow.bottom
            anchors.topMargin: 30  * theme.scaleHeight
            height:tramPassesDown.height
            spacing: 25  * theme.scaleWidth
            Comp.IconButtonTransparent{
                id: tramPassesDown
                icon.source: prefix + "/images/DnArrow64.png"
            }
            Comp.SpinBoxCustomized{
                id: passesRow
                width: tramPassesDown.width
                height: tramPassesDown.height
                text: qsTr("Passes")
                from: 1
                value: 1
                to: 999
            }
            Comp.IconButtonTransparent{
                icon.source: prefix + "/images/UpArrow64.png"
            }
        }

        Comp.IconButtonTransparent{
            anchors.left: parent.left
            anchors.leftMargin: 10  * theme.scaleWidth
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10  * theme.scaleHeight
            icon.source: prefix + "/images/SwitchOff.png"
            onClicked: tramLinesEditor.visible = false
        }
        Comp.IconButtonTransparent{
            anchors.right: parent.right
            anchors.rightMargin: 10  * theme.scaleWidth
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10  * theme.scaleHeight
            icon.source: prefix + "/images/VehFileSave.png"
        }
        Column{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 30  * theme.scaleHeight
            anchors.top: tramPasses.bottom
            spacing: 10  * theme.scaleHeight
            Text{
                text: "Seed"
            }
            Text{
                text: "Spray"
            }
            Text{
                text: "Track"
            }
        }
    }
}
