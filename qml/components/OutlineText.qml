// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Text with an outline of a different color
import QtQuick
import QtQuick.Controls.Fusion

Label {
    id: outlineText
    text: "123"
    //font.pixelSize: 28
    property color outlineColor: "black"

    Repeater {
        model: 9 //??
        Label {
            x: index % 3 - 1
            y: Math.floor(index / 3) - 1
            text: parent.text
            font.pixelSize: parent.font.pixelSize
            font.family: parent.font.family
            color: outlineColor
            z: -2
        }
    }
}

