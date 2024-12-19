// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
import QtQuick
import QtQuick.Effects

Rectangle
{
    id: compass
    width: 104
    height: 104
    color: "transparent"
    property double heading: 0

    Image {
        id: rootImage
        anchors.fill: parent
        source: prefix + "/images/textures/Compass.png"

        fillMode: Image.PreserveAspectCrop


        transform: Rotation {
                origin.x: width/2
                origin.y: height/2
                angle: compass.heading
        }
    }
    MultiEffect {
        colorization: 1.0
        colorizationColor: "red"
        brightness: 0.5
        anchors.fill: needle
        source: needle
    }
    Image {
        id: needle
        source: prefix + "/images/UpArrow64.png"
        width: 35
        height: 35
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.verticalCenter
        visible: false
    }
}
