// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// IMU angle display, bottom right. Called "SteerCircle:" in AOG
import QtQuick 2.15
import QtQuick.Effects

Rectangle {
    id: steerCircle
    color: "transparent"
    width: 120
    height: 120
    property color steerColor: "#F218ED"
    property double rollAngle: 0
    property font font: Qt.application.font

    Image {
        id: steerCircleImage
        anchors.fill: parent
        source: "/images/textures/z_SteerPointer.png"
        fillMode: Image.PreserveAspectCrop
        visible: false
    }

    MultiEffect {
        id: colorize1
        anchors.fill: steerCircleImage
        source: steerCircleImage
        colorization: 1.0
        colorizationColor: steerColor
        saturation: 0.0
        //brightness: -0.5

        transform: Rotation {
            origin.x: width / 2
            origin.y: height / 2
            angle: steerCircle.rollAngle
        }

    }

    Image {
        id: steerDotImage
        anchors.fill: parent
        source: "/images/textures/z_SteerDot.png"
        fillMode: Image.PreserveAspectCrop
        visible:false

    }

    MultiEffect {
        id: colorize2
        anchors.fill: steerDotImage
        source: steerDotImage
        colorization: 1.0
        colorizationColor: steerColor
        saturation: 0.0
        //brightness: -0.5
    }

    Rectangle {
        width: t_metrics.width
        height: t_metrics.height
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: (parent.width - width) / 2
        anchors.topMargin: (parent.height - height) / 2 - t_metrics.height

        //border.width: 1
        //border.color: steerColor

        color: "transparent"

        Text {
            anchors.fill:parent
            color: steerColor
            font: steerCircle.font

            id: rollAngleDisp
            text: Number(rollAngle).toLocaleString(Qt.locale(),'f',1)
        }

        TextMetrics {
            id: t_metrics
            font: rollAngleDisp.font
            text: rollAngleDisp.text
        }
    }
}
