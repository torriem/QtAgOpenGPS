import QtQuick 2.0
import QtQuick.Controls 2.5
import "components"

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
            objectName: "btnTiltDown"
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/TiltDown.png"
        }
        IconButtonTransparent {
            id: btnCamera2d
            objectName: "btnCamera2d"
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/Camera2D64.png"
        }
        IconButtonTransparent {
            id: btnCameraNorth2d
            objectName: "btnCameraNorth2d"
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/CameraNorth2D.png"
        }
        IconButtonTransparent {
            id: btnZoomOut
            objectName: "btnZoomOut"
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/ZoomOut48.png"
        }
        IconButtonTransparent {
            id: btnWindowDayNight
            objectName: "btnWindowDayNight"
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/WindowDayMode.png"
        }
        IconButtonTransparent {
            id: btnBrightnessDown
            objectName: "btnBrightnessDown"
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/BrightnessDn.png"
        }
        IconButtonTransparent {
            id: btnTiltUp
            objectName: "btnTiltUp"
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/TiltUp.png"
        }
        IconButtonTransparent {
            id: btnCamera3d
            objectName: "btnCamera3d"
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/Camera3D64.png"
        }
        IconButtonTransparent {
            id: btnCameraNorth3d
            objectName: "btnCameraNorth3d"
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/CameraNorth64.png"
        }
        IconButtonTransparent {
            id: btnZoomIn
            objectName: "btnZoomIn"
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
            objectName: "btnBrightnessUp"
            width: 70
            height: 70
            radius: 10
            icon.source: "/images/BrightnessUp.png"
        }
    }
}
