// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// The panel with display controls (+, -, 2d/3d etc.
import QtQuick
import QtQuick.Controls.Fusion
import "components" as Comp

Comp.TimedRectangle{
    id: displayButtons
    color: aog.backgroundColor
    Grid {
        id: tiltButtons
        anchors.leftMargin: 5
        anchors.topMargin: 5
        spacing: 6
        flow: Grid.TopToBottom
        //rows:6
		rows: 5
        columns:2
        onChildrenChanged: console.log("childrenChanged")
        Comp.IconButtonTransparent {
            id: btnTiltDown
            width: 70
            height: 70
            radius: 10
            icon.source: prefix + "/images/TiltDown.png"
            onClicked: {
                aog.tiltDown()
                displayButtons.resetTimer() 
            }
        }
        Comp.IconButtonTransparent {
            id: btnCamera2d
            width: 70
            height: 70
            radius: 10
            icon.source: prefix + "/images/Camera2D64.png"
            onClicked: {
                aog.btn2D()
                displayButtons.resetTimer() 
            }
        }
        Comp.IconButtonTransparent {
            id: btnCameraNorth2d
            width: 70
            height: 70
            radius: 10
            icon.source: prefix + "/images/CameraNorth2D.png"
            onClicked: {
                aog.n2D()
                displayButtons.resetTimer() 
            }
        }
        Comp.IconButtonTransparent {
            id: btnZoomOut
            width: 70
            height: 70
            radius: 10
            icon.source: prefix + "/images/ZoomOut48.png"
            onClicked: {
                aog.zoomOut()
                displayButtons.resetTimer() 
            }
        }
        Comp.IconButtonTransparent {
            id: btnWindowDayNight
            width: 70
            height: 70
            radius: 10
            icon.source: prefix + "/images/WindowNightMode.png"
            iconChecked: prefix + "/images/WindowDayMode.png"
            checkable: true
            isChecked: settings.setDisplay_isDayMode
            onCheckedChanged: settings.setDisplay_isDayMode = checked
            onClicked: displayButtons.resetTimer()
        }
        Comp.IconButtonTransparent {
            id: btnBrightnessDown
            width: 70
            height: 70
            radius: 10
            icon.source: prefix + "/images/BrightnessDn.png"
            onClicked: displayButtons.resetTimer()
			visible: false //todo
        }
        Comp.IconButtonTransparent {
            id: btnTiltUp
            width: 70
            height: 70
            radius: 10
            icon.source: prefix + "/images/TiltUp.png"
            onClicked: {
                aog.tiltUp()
                displayButtons.resetTimer() 
            }
        }
        Comp.IconButtonTransparent {
            id: btnCamera3d
            width: 70
            height: 70
            radius: 10
            icon.source: prefix + "/images/Camera3D64.png"
            onClicked: {
                aog.btn3D()
                displayButtons.resetTimer() 
            }
        }
        Comp.IconButtonTransparent {
            id: btnCameraNorth3d
            width: 70
            height: 70
            radius: 10
            icon.source: prefix + "/images/CameraNorth64.png"
            onClicked: {
                aog.n3D()
                displayButtons.resetTimer() 
            }
        }
        Comp.IconButtonTransparent {
            id: btnZoomIn
            width: 70
            height: 70
            radius: 10
            icon.source: prefix + "/images/ZoomIn48.png"
            onClicked: {
                aog.zoomIn()
                displayButtons.resetTimer() 
            }
        }
        Rectangle {
            id: btnHZ
            width: 70
            height: 70
            radius: 10
        }
        Comp.IconButtonTransparent {
            id: btnBrightnessUp
			visible: false //todo
            width: 70
            height: 70
            radius: 10
            icon.source: prefix + "/images/BrightnessUp.png"
            onClicked: displayButtons.resetTimer()
        }
    }
}
