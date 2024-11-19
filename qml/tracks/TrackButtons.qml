// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// The panel of buttons where track editing(new, go to select which track, line and ref nudge, etc)
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import "../components" as Comp

import ".."

Comp.TimedRectangle {
    id: trackButtons
    color: "white"
    width: 110*7
    height: 110
    RowLayout{
        anchors.fill: parent
        anchors.margins: 5
        Comp.IconButtonTransparent{
            id: marker
            icon.source: prefix + "/images/ABSnapNudgeMenuRef.png"
            Layout.alignment: Qt.AlignCenter
            onClicked: refNudge.show()
        }
        Comp.IconButtonColor{
            icon.source: prefix + "/images/AutoSteerSnapToPivot.png"
            implicitWidth: marker.width
            implicitHeight: marker.width
            Layout.alignment: Qt.AlignCenter
        }
        Comp.IconButtonTransparent{
            icon.source: prefix + "/images/SwitchOff.png"
            Layout.alignment: Qt.AlignCenter
        }
        Comp.IconButtonTransparent{
            icon.source: prefix + "/images/ABTracks.png"
            Layout.alignment: Qt.AlignCenter
			onClicked: trackList.show()
        }
        Comp.IconButtonTransparent{
            icon.source: prefix + "/images/AddNew.png"
            Layout.alignment: Qt.AlignCenter
            onClicked: trackNew.show()
        }
        Comp.IconButtonTransparent{
            icon.source: prefix + "/images/ABDraw.png"
            Layout.alignment: Qt.AlignCenter
            onClicked: lineDrawer.show()
        }
        Comp.IconButtonTransparent{
            icon.source: prefix + "/images/ABSnapNudgeMenu.png"
            Layout.alignment: Qt.AlignCenter
            onClicked: lineNudge.show()
        }
    }
}
