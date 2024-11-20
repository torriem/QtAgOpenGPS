// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
//
import QtQuick 2.0
import QtQuick.Controls.Fusion
import "components" as Comp

Item {
    width: 340
    height: 550
        id: mainWindowTram
        Comp.TopLine{
            id: topLine
            titleText: "Tram Lines"
        }
    Rectangle{
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.top: topLine.bottom
        Row{
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            id: tramHalfNudge
            width: children.width
            spacing: 35
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
            anchors.topMargin: 30
            id: tramSmallNudge
            width: children.width
            spacing: 25
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
        Comp.IconButtonTransparent{
            id: tramSwapAB
            anchors.top: tramSmallNudge.bottom
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 60
            icon.source: prefix + "/images/ABSwapPoints.png"
        }
        Comp.IconButtonTransparent{
            id: tramSwapMode
            anchors.top: tramSmallNudge.bottom
            anchors.topMargin: 10
            anchors.left: tramSwapAB.right
            anchors.leftMargin: 60
            icon.source: prefix + "/images/TramLines.png"
        }
        Text {
            text: qsTr("Mode")
            font.pixelSize: 15
            anchors.left: tramSwapMode.right
            anchors.leftMargin: 30
            anchors.verticalCenter: tramSwapMode.verticalCenter
        }
        Row{
            id: tramPasses
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: tramSwapMode.bottom
            anchors.topMargin: 30
            height:tramPassesDown.height
            spacing: 25
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
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            icon.source: prefix + "/images/SwitchOff.png"
            onClicked: tramLinesEditor.visible = false
        }
        Comp.IconButtonTransparent{
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            icon.source: prefix + "/images/VehFileSave.png"
        }
        Column{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 30
            anchors.top: tramPasses.bottom
            spacing: 10
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
