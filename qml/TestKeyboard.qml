import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.VirtualKeyboard 2.15
import QtQuick.Window 2.15

Window {
    height: 600
    width: 800

    ScrollView {
        id: sv 
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: inputPanel.top

        property double scrollPosition: 0

        contentWidth: 800
        contentHeight: 600
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        ScrollBar.vertical.position: scrollPosition

        Rectangle {
            id: r
            width: 800
            height: 600

            anchors.margins: 10

            color: "red"
            SpinBox {
                id: spin
                x: 50
                y: 300
                to: 100
                from: 0
                editable: true

                onActiveFocusChanged: function(focused) {
                    if  (focused) {
                        console.debug("y position is " + spin.y)
                        sv.scrollPosition = (spin.y / sv.height)

                    }
                }
            }
        }
    }

    InputPanel {
        id: inputPanel
        y: Qt.inputMethod.visible ? parent.height - inputPanel.height : parent.height
        anchors.left: parent.left
        anchors.right: parent.right
    }
}
