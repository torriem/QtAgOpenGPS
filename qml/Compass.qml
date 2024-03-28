import QtQuick 2.0
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
        source: "/images/textures/Compass.png"

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
        source: "/images/UpArrow64.png"
        width: 35
        height: 35
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.verticalCenter
        visible: false
    }
}
