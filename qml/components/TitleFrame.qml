import QtQuick 2.15
import QtQuick.Controls 2.15


Rectangle {
    id: titleFrame
    default property alias content: contentRect.children
    property string title
    property alias font: titleLabel.font

    property bool enabled: true
    property color titleColorEnabled: aog.backgroundColor
    property color titleColorDisabled: "grey"

    property int titleHeight: titleLabel.height

    onEnabledChanged: setEnabled()
    Component.onCompleted: setEnabled()

    color: "transparent"

    function setEnabled() {
        for( var i = 0; i < contentRect.children.length ; i ++) {
            if (typeof(contentRect.children[i].enabled) !== "undefined") {
                contentRect.children[i].enabled = titleFrame.enabled
            }
        }
    }

    Label {
        id: titleLabel
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        text: title
        color: titleFrame.enabled ? titleColorEnabled : titleColorDisabled
        visible: title != "" ? true : false
    }

    Rectangle {
        id: contentRect

        anchors.top: titleLabel.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        color: "transparent"
    }
}
