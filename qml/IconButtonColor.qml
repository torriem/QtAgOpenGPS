import QtQuick 2.7
import QtGraphicalEffects 1.0

Rectangle {
    id: iconButtonColor
    x: 0
    y: 0
    width: 120
    height: 65

    property color color1: "#ffffff"
    property color color2: "#ffffff"
    property color color3: "#ffffff"
    property url icon: "/images/AutoSteerOn.png"
    property int bordersize: 3
    property bool checkable: true
    property color colorChecked1: "green"
    property color colorChecked2: "green"
    property color colorChecked3: "green"
    property string buttonText: ""
    property string leftText: ""
    property string rightText: ""

    property bool isChecked: false

    onIsCheckedChanged: {
        if (isChecked) {
            gradientStop1.color = colorChecked1
            gradientStop2.color = colorChecked2
            gradientStop3.color = colorChecked3
            //image.source = iconChecked
        } else {
            gradientStop1.color = color1
            gradientStop2.color = color2
            gradientStop3.color = color3
            //image.source = icon
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
                target: iconButtonColor
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
                target: iconButtonColor
                bordersize: 1
            }
        },

        State {
            when: mouseArea.containsMouse
            name: "hover"
            PropertyChanges { target: iconButtonColor; bordersize: 1}
        }
    ]

    signal clicked
    signal pressAndHold

    gradient: Gradient {
        GradientStop {
            id: gradientStop1
            position: 0
            color: iconButtonColor.color1
        }

        GradientStop {
            id: gradientStop2
            position: 0.5
            color: iconButtonColor.color2
        }

        GradientStop {
            id: gradientStop3
            position: 1
            color: iconButtonColor.color3
        }
    }
    border.width: bordersize
    Text {
        id: text1
        text: parent.buttonText
        anchors.bottom: parent.top
        anchors.bottomMargin: 5
        anchors.left: parent.left
        font.bold: true
        font.pixelSize: parent.height * 0.15
        //z: 1
    }

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
