import QtQuick 2.7
import QtQuick.Controls 2.5
import Qt5Compat.GraphicalEffects

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

    property alias border: icon_button_background.border
    property alias radius: icon_button_background.radius

    property color borderColorEnabled: "black"
    property color borderColorDisabled: "grey"

    property color color1: color //redneck code
    property color color2: color
    property color color3: color

    property color colorHover1: "white"
    property color colorHover2: "white"
    property color colorHover3: "white"

    property color colorChecked1: colorChecked
    property color colorChecked2: colorChecked
    property color colorChecked3: colorChecked
    property color color: "white"
    property color colorChecked: "#99ff99"


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
        if (checked && useIconChecked) {
            content_image.source = iconChecked
            //console.warn("icon should be ", content_image.source)
        } else {
            content_image.source = icon.source
            //console.warn("icon should be ", content_image.source)
        }
    }

    property url iconChecked: ""
    property bool useIconChecked: false
    onIconCheckedChanged: {
        if (iconChecked != "") {
            useIconChecked = true
        } else {
            useIconChecked = false
        }
    }

    //property int radius: 10
    onRadiusChanged: {
        icon_button_background.radius = radius
    }

    onWidthChanged: {
        //console.warn(text, "Width is now ", width)
    }

    onHeightChanged: {
        //console.warn(height)
    }
    contentItem: Rectangle {
        id: icon_button_content
        anchors.fill: parent
        color: "transparent"
        z: 1

        Image {
            id: content_image
            width: parent.width
            anchors.fill: parent
            anchors.margins: 10
            fillMode: Image.PreserveAspectFit
            source: icon_button.icon.source
            z:1
        }

        Colorize {
            id: disableFilter
            anchors.fill: content_image
            source: content_image
            hue: 0.0
            saturation: 0.0
            lightness: 0.2
            visible: ! icon_button.enabled
        }

        Text {
            id: text1
            text: icon_button.text
            anchors.bottom: parent.top
            anchors.left: parent.left
            font.bold: true
            font.pixelSize: 15
            //font.pixelSize: parent.height * 0.2
            visible: true
        }
    }

    background: Rectangle {
        border.width: 1
        //border.width: icon_button.border
        border.color: icon_button.enabled ?
                          icon_button.borderColorEnabled :
                          icon_button.borderColorDisabled
        radius: 10
        id: icon_button_background
        z: 0
        gradient: Gradient {
            GradientStop {
                id: gradientStop1
                position: 0
                color: icon_button.color1
            }

            GradientStop {
                id: gradientStop2
                position: 0.5
                color: icon_button.color2
            }

            GradientStop {
                id: gradientStop3
                position: 1
                color: icon_button.color3
            }
        }

        states: [
            State {
                when: icon_button.down
                name: "pressedUnchecked"
                PropertyChanges {
                    target: gradientStop1
                    //color: icon_button.color3
                    color: icon_button.colorChecked1
                }
                PropertyChanges {
                    target: gradientStop2
                    //color: icon_button.color3
                    color: icon_button.colorChecked2
                }
                PropertyChanges {
                    target: gradientStop3
                    //color: icon_button.color1
                    color: icon_button.colorChecked3
                }
               /* PropertyChanges {
                    target: icon_button_background
                    border.width: 5
                }*/

                PropertyChanges {
                    target: content_image
                    source: icon_button.icon.source
                }

            },
            State {
                when: icon_button.down && icon_button.checked
                name: "pressedChecked"
                PropertyChanges {
                    target: gradientStop1
                    color: icon_button.color3
                }
                PropertyChanges {
                    target: gradientStop2
                    color: icon_button.color3
                }
                PropertyChanges {
                    target: gradientStop3
                    color: icon_button.color1
                }
             /*   PropertyChanges {
                    target: icon_button_background
                    border.width: 1
                }*/
                PropertyChanges {
                    target: content_image
                    source: icon_button.iconChecked != "" ? icon_button.iconChecked : icon_button.icon.source
                }
            },
            State {
                when: ! icon_button.down && icon_button.checked
                name: "checked"
                PropertyChanges {
                    target: gradientStop1
                    color: icon_button.colorChecked1
                }
                PropertyChanges {
                    target: gradientStop2
                    color: icon_button.colorChecked2
                }
                PropertyChanges {
                    target: gradientStop3
                    color: icon_button.colorChecked3
                }
               /* PropertyChanges {
                    target: icon_button_background
                    border.width: 0
                }*/

                PropertyChanges {
                    target: content_image
                    source: icon_button.iconChecked != "" ? icon_button.iconChecked : icon_button.icon.source
                }

            },
            State {
                when: ! icon_button.down && ! icon_button.checked && ! icon_button.hovered
                name: "up"
                PropertyChanges {
                    target: gradientStop1
                    color: icon_button.color1
                }
                PropertyChanges {
                    target: gradientStop2
                    color: icon_button.color2
                }
                PropertyChanges {
                    target: gradientStop3
                    color: icon_button.color3
                }
                /*PropertyChanges {
                    target: icon_button_background
                    border.width: 0
                }*/
                PropertyChanges {
                    target: content_image
                    source: icon_button.icon.source
                }
           },
            State {
                when: icon_button.hovered
                name: "hovered"
               /* PropertyChanges {
                    target: icon_button_background
                    border.width: 1
                }*/
                PropertyChanges {
                    target: gradientStop1
                    color: icon_button.colorHover1
                }
                PropertyChanges {
                    target: gradientStop2
                    color: icon_button.colorHover2
                }
                PropertyChanges {
                    target: gradientStop3
                    color: icon_button.colorHover3
                }
                PropertyChanges {
                    target: content_image
                    source: icon_button.icon.source
                }
            }
        ]


    }
}
