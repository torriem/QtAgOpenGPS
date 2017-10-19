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
    property int bordersize: 0
    property bool checkable: false
    property color colorChecked1: "#ff4444"
    property color colorChecked2: "#ff0000"
    property color colorChecked3: "#888888"
    property url iconChecked: "/images/AutoSteerOn.png"

    state: "unchecked"

    states: [
        State {
            when: mouseArea.pressed
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
            when: mouseArea.containsMouse
            name: "hoverUnchecked"
            PropertyChanges { target: iconButton; bordersize: 1}
        }
        /*
        ,
        State {
            name: "unchecked"
            PropertyChanges {
                target: image
                source: icon
            }
            PropertyChanges {
                target: gradientStop1
                color: color1
            }
            PropertyChanges {
                target: gradientStop2
                color: color2
            }
            PropertyChanges {
                target: gradientStop2
                color: color2
            }
        },

        State {
            name: "checked"
            PropertyChanges {
                target: image
                source: iconChecked
            }
            PropertyChanges {
                target: gradientStop1
                color: colorChecked1
            }
            PropertyChanges {
                target: gradientStop2
                color: colorChecked2
            }
            PropertyChanges {
                target: gradientStop3
                color: colorChecked2
            }
        }*/
    ]

    signal clicked

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
            /*
            console.debug( "old state is " + iconButton.state);
            if (iconButton.state == "checked" || iconButton.state == "hoverUnchecked" || iconButton.state == "hoverPressed")
                iconButton.state = "unchecked";
            else
                iconButton.state = "checked";

            console.debug ("new state is " + iconButton.state);
            */
            parent.clicked(mouse);
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
