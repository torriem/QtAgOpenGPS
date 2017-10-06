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
    property url icon: "images/AutoSteerOn.png"
    property string buttonText: "Hello"
    property string leftText: ""
    property string rightText: ""
    property int bordersize: 0
    property double iconHeightScale: 0.75

    signal clicked(variant mouse)

    gradient: Gradient {
        GradientStop {
            position: 0
            color: iconButton.color1
        }

        GradientStop {
            position: 0.5
            color: iconButton.color2
        }

        GradientStop {
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
    }

    Image {
        id: image
        width: parent.width
        height: parent.height * parent.iconHeightScale
        anchors.top: parent.top
        anchors.topMargin: parent.height * 0.05
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
    }
    states: [
        State {
            when: mouseArea.pressed
            name: "pressed"
            PropertyChanges { target: iconButton; color1: "#888888"; color2: "#888888"; color3: "#ffffff"; bordersize: 1 }
        },
        State {
            when: mouseArea.containsMouse
            name: "hover"
            PropertyChanges { target: iconButton; bordersize: 1}
        }
    ]
}
