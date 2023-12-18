import QtQuick 2.7
import QtGraphicalEffects 1.0

Rectangle {
    id: iconButton
    x: 0
    y: 0
    width: 100
    height: 100
    radius: 10

    property color color1: "#ffffff"
    property color color2: "#ffffff"
    property color color3: "#888888"
    property url icon: "/images/AutoSteerOn.png"
    property color colorChecked1: "#c8e8ff"
    property color colorChecked2: "#7cc8ff"
    property color colorChecked3: "#467191"
    property url iconChecked: "/images/AutoSteerOn.png"

    property string buttonText: "Hello"
    property string leftText: ""
    property string rightText: ""
    property int bordersize: 0
    property double iconHeightScale: 0.75

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

    Text {
        id: text1
        text: parent.buttonText
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height*0.05
        anchors.horizontalCenter: parent.horizontalCenter
        font.bold: true
        font.pixelSize: parent.height * 0.15
        //z: 1
    }

    Image {
        id: image
        width: parent.width
        height: parent.height * parent.iconHeightScale
        anchors.left:parent.left
        anchors.leftMargin: parent.width * 0.05
        anchors.right: parent.right
        anchors.rightMargin: parent.width * 0.05
        anchors.top: parent.top
        anchors.topMargin: parent.height * 0.05
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height - parent.height * parent.iconHeightScale + parent.height * 0.05
        fillMode: Image.PreserveAspectFit
        source: parent.icon
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

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onClicked: {
            parent.clicked(mouse)
        }
        onPressAndHold: {
            parent.pressAndHold();
        }
    }
    states: [
        State {
            when: mouseArea.pressed && !isChecked
            name: "pressedUnchecked"
            PropertyChanges {
                target: gradientStop1
                color: color3
            }
            PropertyChanges {
                target: gradientStop2
                color: color3
            }
            PropertyChanges {
                target: gradientStop3
                color: color1
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
        }
    ]
}
