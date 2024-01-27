import QtQuick 2.15
import QtQuick.Controls 2.15

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

