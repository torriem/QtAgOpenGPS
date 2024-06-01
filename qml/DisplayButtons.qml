import QtQuick
import QtQuick.Controls.Fusion
import "components"

TimedRectangle{
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
        IconButtonTransparent {
            id: btnTiltDown
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/TiltDown.png"
            onClicked: {
                aog.tiltDown()
                displayButtons.resetTimer() 
            }
        }
        IconButtonTransparent {
            id: btnCamera2d
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/Camera2D64.png"
            onClicked: {
                aog.btn2D()
                displayButtons.resetTimer() 
            }
        }
        IconButtonTransparent {
            id: btnCameraNorth2d
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/CameraNorth2D.png"
            onClicked: {
                aog.n2D()
                displayButtons.resetTimer() 
            }
        }
        IconButtonTransparent {
            id: btnZoomOut
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/ZoomOut48.png"
            onClicked: {
                aog.zoomOut()
                displayButtons.resetTimer() 
            }
        }
        IconButtonTransparent {
            id: btnWindowDayNight
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/WindowNightMode.png"
            iconChecked: "/images/WindowDayMode.png"
            checkable: true
            isChecked: settings.setDisplay_isDayMode
            onCheckedChanged: settings.setDisplay_isDayMode = checked
            onClicked: displayButtons.resetTimer()
        }
        IconButtonTransparent {
            id: btnBrightnessDown
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/BrightnessDn.png"
            onClicked: displayButtons.resetTimer()
			visible: false //todo
        }
        IconButtonTransparent {
            id: btnTiltUp
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/TiltUp.png"
            onClicked: {
                aog.tiltUp()
                displayButtons.resetTimer() 
            }
        }
        IconButtonTransparent {
            id: btnCamera3d
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/Camera3D64.png"
            onClicked: {
                aog.btn3D()
                displayButtons.resetTimer() 
            }
        }
        IconButtonTransparent {
            id: btnCameraNorth3d
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/CameraNorth64.png"
            onClicked: {
                aog.n3D()
                displayButtons.resetTimer() 
            }
        }
        IconButtonTransparent {
            id: btnZoomIn
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/ZoomIn48.png"
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
        IconButtonTransparent {
            id: btnBrightnessUp
			visible: false //todo
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/BrightnessUp.png"
            onClicked: displayButtons.resetTimer()
        }
    }
}
