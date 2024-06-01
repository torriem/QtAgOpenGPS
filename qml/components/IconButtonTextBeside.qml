import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Effects

Button {
    implicitWidth: 270 * theme.scaleWidth
    implicitHeight: 80 * theme.scaleHeight
    id: icon_button_text_beside
    text: ""
    hoverEnabled: true
    //checkable: true
    icon.source: ""
    icon.color: "transparent"

    property double iconHeightScaleText: 0.75
    property int border: 1

    property color color1: color //redneck code
    property color color2: color
    property color color3: color

    property color colorHover1: "#ffffff"
    property color colorHover2: "#ffffff"
    property color colorHover3: "#888888"

    property color colorChecked1: colorChecked
    property color colorChecked2: colorChecked
    property color colorChecked3: colorChecked
    property color color: "white"
    property color colorChecked: enabled ? "green" : "pale green"

    property color textColor: enabled ? "black" : "grey"
    property color borderColor: enabled ? "black" : "grey"
    property int radius: 10

    //For compatibility with the old IconButton and friends
    property bool isChecked: icon_button_text_beside.checked

    onIsCheckedChanged: {
        checked = isChecked;
    }

    property string buttonText: ""

    onButtonTextChanged: {
        text = buttonText
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

        Text {
            id: text1
            text: icon_button_text_beside.text
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            color: textColor
            font.bold: false
            font.pixelSize: parent.height * 0.3
            z: 1
            visible: icon_button_text_beside ? true : false
        }

        Image {
            id: content_image
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 2
            width: height
            fillMode: Image.PreserveAspectFit
            source: icon_button_text_beside.icon.source
        }

        MultiEffect {
            id: disableFilter
            anchors.fill: content_image
            source: content_image
            colorization: 0.0
            saturation: 0.0
            brightness: 0.2
            visible: ! icon_button_text_beside.enabled
        }
    }

    background: Rectangle {
        border.width: icon_button_text_beside.border
        //border.width: icon_button.border
        radius: icon_button_text_beside.radius
        border.color: icon_button_text_beside.borderColor

        id: icon_button_background
        gradient: Gradient {
            GradientStop {
                id: gradientStop1
                position: 0
                color: icon_button_text_beside.color1
            }

            GradientStop {
                id: gradientStop2
                position: 0.5
                color: icon_button_text_beside.color2
            }

            GradientStop {
                id: gradientStop3
                position: 1
                color: icon_button_text_beside.color3
            }
        }

        states: [
            State {
                when: icon_button_text_beside.down
                name: "pressedUnchecked"
                PropertyChanges {
                    target: gradientStop1
                    color: icon_button_text_beside.color3
                }
                PropertyChanges {
                    target: gradientStop2
                    color: icon_button_text_beside.color3
                }
                PropertyChanges {
                    target: gradientStop3
                    color: icon_button_text_beside.color1
                }
               /* PropertyChanges {
                    target: icon_button_text_beside_background
                    border.width: 5
                }*/
                /*
                PropertyChanges {
                    target: content_image
                    source: icon_button_text_beside.icon.source
                }
                */

            },
            State {
                when: icon_button_text_beside.down && icon_button_text_beside.checked
                name: "pressedChecked"
                PropertyChanges {
                    target: gradientStop1
                    color: icon_button_text_beside.color3
                }
                PropertyChanges {
                    target: gradientStop2
                    color: icon_button_text_beside.color3
                }
                PropertyChanges {
                    target: gradientStop3
                    color: icon_button_text_beside.color1
                }
             /*   PropertyChanges {
                    target: icon_button_text_beside_background
                    border.width: 1
                }*/
                PropertyChanges {
                    target: content_image
                    source: icon_button_text_beside.icon.source
                }
            },
            State {
                when: ! icon_button_text_beside.down && icon_button_text_beside.checked
                name: "checked"
                PropertyChanges {
                    target: gradientStop1
                    color: icon_button_text_beside.colorChecked1
                }
                PropertyChanges {
                    target: gradientStop2
                    color: icon_button_text_beside.colorChecked2
                }
                PropertyChanges {
                    target: gradientStop3
                    color: icon_button_text_beside.colorChecked3
                }
               /* PropertyChanges {
                    target: icon_button_text_beside_background
                    border.width: 0
                }*/
                /*
                PropertyChanges {
                    target: content_image
                    source: (icon_button_text_beside.iconChecked ? icon_button_text_beside.iconChecked : icon_button_text_beside.icon.source)
                }
                */
            },
            State {
                when: ! icon_button_text_beside.down && ! icon_button_text_beside.checked && ! icon_button_text_beside.hovered
                name: "up"
                PropertyChanges {
                    target: gradientStop1
                    color: icon_button_text_beside.color1
                }
                PropertyChanges {
                    target: gradientStop2
                    color: icon_button_text_beside.color2
                }
                PropertyChanges {
                    target: gradientStop3
                    color: icon_button_text_beside.color3
                }
                /*PropertyChanges {
                    target: icon_button_text_beside_background
                    border.width: 0
                }*/
                /*
                PropertyChanges {
                    target: content_image
                    source: icon_button_text_beside.icon.source
                }
                */
           },
            State {
                when: icon_button_text_beside.hovered
                name: "hovered"
               /* PropertyChanges {
                    target: icon_button_background
                    border.width: 1
                }*/
                PropertyChanges {
                    target: gradientStop1
                    color: icon_button_text_beside.colorHover1
                }
                PropertyChanges {
                    target: gradientStop2
                    color: icon_button_text_beside.colorHover2
                }
                PropertyChanges {
                    target: gradientStop3
                    color: icon_button_text_beside.colorHover3
                }
                /*
                PropertyChanges {
                    target: content_image
                    source: icon_button_text_beside.icon.source
                }
                */
            }
        ]


    }
}
