import QtQuick 2.8

Image {
    id: image
    state: "off"

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }
    states: [
        State {
            name: "on"
            PropertyChanges {
                target: image
                source: "images/SectionOn.png"
            }
        },
        State {
            name: "auto"

            PropertyChanges {
                target: image
                source: "images/SectionAuto.png"
            }
        },
        State {
            name: "off"
            PropertyChanges {
                target: image
                source: "images/SectionOff.png"
            }
        }
    ]
}
