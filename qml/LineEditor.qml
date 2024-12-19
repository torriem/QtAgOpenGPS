// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Window for "Editing" lines. Rename, change heading
import QtQuick
import QtQuick.Controls.Fusion
import "components" as Comp

Rectangle {
    id: lineEditor
    width: 300
    height: 375
    color: aog.backgroundColor
    border.width: 1
    border.color: aog.blackDayWhiteNight
    Comp.TopLine{
        id: topLine
        titleText: "Edit AB"
    }
    Row{
        anchors.top: topLine. bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        id: lineNudge
        width: children.width
        spacing: 35
        height: children.height
        Comp.IconButtonTransparent{
            id: leftNudger
            icon.source: prefix + "/images/ArrowLeft.png"
        }
        Comp.SpinBoxCustomized{
            decimals: 1
            width: leftNudger.width
            height: 50
            anchors.verticalCenter: parent.verticalCenter
            text: qsTr("cm")
            from: 0
            value: 7.5
            to: 9999
        }

        Comp.IconButtonTransparent{
            icon.source: prefix + "/images/ArrowRight.png"
        }
    }
    Row{
        id: middleIcons
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: lineNudge.bottom
        anchors.topMargin: 35
        width: children.width
        spacing: 35
        height: children.height
        Comp.IconButtonTransparent{
            icon.source: prefix + "/images/ABSwapPoints.png"
        }
        Comp.IconButtonTransparent{
            icon.source: prefix + "/images/SnapToPivot.png"
        }

    }
    Row{
        id: bottomIcons
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: middleIcons.bottom
        anchors.topMargin: 35
        width: children.width
        spacing: 15
        height: children.height
        Comp.IconButtonText{
            icon.source: prefix + "/images/Cancel64.png"
            onClicked: lineEditor.visible = false
            color1: "transparent"
            color2: "transparent"
            color3: "transparent"
            text: "Cancel"
            width: 75
            height: 75
        }
        Comp.IconButtonText{
            icon.source: prefix + "/images/FileDontSave.png"
            color1: "transparent"
            color2: "transparent"
            color3: "transparent"
            text: "For Now"
            width: 75
            height: 75
            onClicked: lineEditor.visible = false
        }
        Comp.IconButtonText{
            icon.source: prefix + "/images/FileSave.png"
            color1: "transparent"
            color2: "transparent"
            color3: "transparent"
            text: "Save"
            width: 75
            height: 75
            onClicked: lineEditor.visible = false
        }
    }
}
