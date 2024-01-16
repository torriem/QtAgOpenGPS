import QtQuick 2.0
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.13

Button {
    implicitWidth: 120
    implicitHeight: 65
    id: icon_button
    text: ""
    hoverEnabled: true
    //checkable: true
    icon.source: ""
    icon.color: "transparent"

    property double iconHeightScaleText: 0.75
    property int border: 1

    property color color: "white"

    property color colorHover: "white"

    property color colorChecked: "green"


    //For compatibility with the old IconButton and friends
    property bool isChecked: icon_button.checked
    property string buttonText: ""

    onButtonTextChanged: {
        text = buttonText
    }
    onIsCheckedChanged: {
        checked = isChecked;
    }

    //This is specific to this base type... must be re-implemented in subtypes
    onCheckedChanged: {
        isChecked = checked

        if (checked && useIconChecked) {
            content_image.source = iconChecked
            //console.warn("icon should be ", content_image.source)
        } else {
            content_image.source = icon.source
            //console.warn("icon should be ", content_image.source)
        }

    }


    property int radius: 10

    onWidthChanged: {
        //console.warn(text, "Width is now ", width)
    }

    onHeightChanged: {
        //console.warn(height)
    }

    background: Rectangle {
        border.width: icon_button.border
        //border.width: icon_button.border
        radius: 10
        id: icon_button_background
//        color: icon_button.color

//        states: [
//            State {
//                when: icon_button.down
//                name: "pressedUnchecked"
//                color: icon_button.color
//            },
//            State {
//                when: icon_button.down && icon_button.checked
//                name: "pressedChecked"
//                color: icon_button.color
//            },
//            State {
//                when: ! icon_button.down && icon_button.checked
//                name: "checked"
//                color: icon_button.colorChecked
//            },
//            State {
//                when: ! icon_button.down && ! icon_button.checked && ! icon_button.hovered
//                name: "up"
//                color: icon_button.color
//            },
//            State {
//                when: icon_button.hovered
//                name: "hovered"
//                color: icon_button.colorHover
//            }
//        ]
//        Text {
//            id: text1
//            text: icon_button.text
//            anchors.bottom: parent.top
//            anchors.left: parent.left
//            font.bold: true
//            font.pixelSize: parent.height * 0.2
//            z: 1
//            visible: true
//        }
    }
}
