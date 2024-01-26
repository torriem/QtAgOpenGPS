import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.13

Button {
    implicitWidth: 70
    implicitHeight: 70
    id: icon_button
    text: ""
    hoverEnabled: true
    //checkable: true
    icon.source: ""
    icon.color: "transparent"

    property double iconHeightScaleText: 0.75
    property int border: 0

    property color color1: "#ffffff"
    property color color2: "#ffffff"
    property color color3: "#888888"

    property color colorHover1: "#ffffff"
    property color colorHover2: "#ffffff"
    property color colorHover3: "#888888"

    property color colorChecked1: "#c8e8ff"
    property color colorChecked2: "#7cc8ff"
    property color colorChecked3: "#467191"


    //For compatibility with the old IconButton and friends
    property bool isChecked: icon_button.checked

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

    property int radius: 10
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

        Text {
            id: text1
            text: icon_button.text
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height*0.02
            anchors.horizontalCenter: parent.horizontalCenter
            font.bold: true
            font.pixelSize: parent.height * 0.15
            z: 1
            visible: icon_button ? true : false
        }

        Image {
            id: content_image
            width: parent.width
            anchors.fill: parent
            anchors.topMargin: parent.height * 0.00
            anchors.bottomMargin: icon_button.text  ?
                                      parent.height - parent.height * icon_button.iconHeightScaleText + parent.height * 0.00
                                    : parent.height * 0.02
            anchors.leftMargin: parent.width * 0.00
            anchors.rightMargin: parent.width * 0.00
            fillMode: Image.PreserveAspectFit
            source: icon_button.icon.source
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


    }

    background: Rectangle {
        border.width: icon_button.border
        //border.width: icon_button.border
        radius: 10
        id: icon_button_background
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
               /* PropertyChanges {
                    target: icon_button_background
                    border.width: 5
                }*/
                /*
                PropertyChanges {
                    target: content_image
                    source: icon_button.icon.source
                }
                */

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
                    source: icon_button.icon.source
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
                /*
                PropertyChanges {
                    target: content_image
                    source: (icon_button.iconChecked ? icon_button.iconChecked : icon_button.icon.source)
                }
                */
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
                /*
                PropertyChanges {
                    target: content_image
                    source: icon_button.icon.source
                }
                */
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
                /*
                PropertyChanges {
                    target: content_image
                    source: icon_button.icon.source
                }
                */
            }
        ]


    }
}
