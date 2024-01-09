import QtQuick 2.0

Rectangle
{
    id: compass
    width: 104
    height: 104
    color: "transparent"
    property double heading: 0

    Image {
        anchors.fill: parent
        source: "/images/textures/Compass.png"

        fillMode: Image.PreserveAspectCrop


        transform: Rotation {
                origin.x: width/2
                origin.y: height/2
                angle: compass.heading
        }
    }

}
