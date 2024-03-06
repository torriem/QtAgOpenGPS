import QtQuick 2.0
import QtQuick.Controls 2.5

TimedRectangle{
    id: displayButtons
    color: "white"
    Grid {
        id: tiltButtons
        anchors.leftMargin: 5
        anchors.topMargin: 5
        spacing: 6
        flow: Grid.TopToBottom
        rows:6
        columns:2

        IconButtonTransparent {
            id: btnTiltDown
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/TiltDown.png"
            onClicked: aog.tiltDown()
        }
        IconButtonTransparent {
            id: btnCamera2d
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/Camera2D64.png"
            onClicked: aog.btn2D()
        }
        IconButtonTransparent {
            id: btnCameraNorth2d
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/CameraNorth2D.png"
            onClicked: aog.n2D()
        }
        IconButtonTransparent {
            id: btnZoomOut
            onClicked: aog.zoomOut()
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/ZoomOut48.png"
        }
        IconButtonTransparent {
            id: btnWindowDayNight
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/WindowDayMode.png"
        }
        IconButtonTransparent {
            id: btnBrightnessDown
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/BrightnessDn.png"
        }
        IconButtonTransparent {
            id: btnTiltUp
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/TiltUp.png"
            onClicked: aog.tiltUp()
        }
        IconButtonTransparent {
            id: btnCamera3d
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/Camera3D64.png"
            onClicked: aog.btn3D()
        }
        IconButtonTransparent {
            id: btnCameraNorth3d
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/CameraNorth64.png"
            onClicked: aog.n3D()
        }
        IconButtonTransparent {
            id: btnZoomIn
            onClicked: aog.zoomIn()
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/ZoomIn48.png"
        }
        Rectangle {
            id: btnHZ
            width: 70
            height: 70
            radius: 10
        }
        IconButtonTransparent {
            id: btnBrightnessUp
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/BrightnessUp.png"
        }
    }
}
