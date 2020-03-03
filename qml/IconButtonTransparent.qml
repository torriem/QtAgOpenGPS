import QtQuick 2.7
import QtGraphicalEffects 1.0

Rectangle {
    id: iconButton
    x: 0
    y: 0
    width: 100
    height: 100
    radius: 10

    property color color1: "#00000000"
    property color color2: "#00000000"
    property color color3: "#00000000"

    property color colorPressed1: "#888888"
    property color colorPressed2: "#ffffff"
    property color colorPressed3: "#ffffff"

    property color colorHover1: "#ffffff"
    property color colorHover2: "#ffffff"
    property color colorHover3: "#888888"

    property url icon: "/images/AutoSteerOn.png"
    property int bordersize: 0
    property bool checkable: false
    property color colorChecked1: "#c8e8ff"
    property color colorChecked2: "#7cc8ff"
    property color colorChecked3: "#467191"
    property url iconChecked: "/images/AutoSteerOn.png"

    property bool isChecked: false

    onIsCheckedChanged: {
        if (isChecked) {
            gradientStop1.color = colorChecked1
            gradientStop2.color = colorChecked2
            gradientStop3.color = colorChecked3
            image.source = iconChecked
        } else {
            gradientStop1.color = color1
            gradientStop2.color = color2
            gradientStop3.color = color3
            image.source = icon
        }
    }

    states: [
        State {
            when: mouseArea.pressed && !isChecked
            name: "pressedUnchecked"
            PropertyChanges {
                target: gradientStop1
                color: colorPressed3
            }
            PropertyChanges {
                target: gradientStop2
                color: colorPressed3
            }
            PropertyChanges {
                target: gradientStop3
                color: colorPressed1
            }
            PropertyChanges {
                target: iconButton
                bordersize: 1
            }
        },
        State {
            when: mouseArea.pressed && isChecked
            name: "pressedChecked"
            PropertyChanges {
                target: gradientStop1
                color: colorChecked3
            }
            PropertyChanges {
                target: gradientStop2
                color: colorChecked3
            }
            PropertyChanges {
                target: gradientStop3
                color: colorChecked1
            }
            PropertyChanges {
                target: iconButton
                bordersize: 1
            }
        },

        State {
            when: mouseArea.containsMouse
            name: "hover"
            PropertyChanges { target: iconButton; bordersize: 1}
            PropertyChanges {
                target: gradientStop1
                color: colorHover1
            }
            PropertyChanges {
                target: gradientStop2
                color: colorHover2
            }
            PropertyChanges {
                target: gradientStop3
                color: colorHover3
            }
        }
    ]

    signal clicked
    signal pressAndHold

    gradient: Gradient {
        GradientStop {
            id: gradientStop1
            position: 0
            color: iconButton.color1
        }

        GradientStop {
            id: gradientStop2
            position: 0.5
            color: iconButton.color2
        }

        GradientStop {
            id: gradientStop3
            position: 1
            color: iconButton.color3
        }
    }
    border.width: bordersize

    Image {
        id: image
        //x: 50
        //y: 13
        width: parent.width
        //height: parent.height
        anchors.fill: parent
        anchors.topMargin: parent.height * 0.05
        anchors.bottomMargin: parent.height * 0.05
        anchors.leftMargin: parent.width * 0.05
        anchors.rightMargin: parent.width * 0.05
        fillMode: Image.PreserveAspectFit
        source: parent.icon
    }

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent

        onClicked:{
            parent.clicked(mouse);
        }
        onPressAndHold: {
            parent.pressAndHold();
        }
    }

    Colorize {
        id: disableFilter
        anchors.fill:image
        source: image
        hue: 0.0
        saturation: 0.0
        lightness: 0.2
        visible: !parent.enabled
    }

}
