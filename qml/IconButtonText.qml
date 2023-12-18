import QtQuick 2.15

IconButton {
    id: icon_button_text

    width: 100
    height: 100

    property double iconHeightScale: 0.75

    property string buttonText: ""

    onButtonTextChanged: {
        text = buttonText
    }


    contentItem: Rectangle {
        id: icon_button_background
        anchors.fill: parent
        color: "transparent"

        Text {
            id: text1
            text: icon_button_text.text
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height*0.05
            anchors.horizontalCenter: parent.horizontalCenter
            font.bold: true
            font.pixelSize: parent.height * 0.15
            z: 1
        }

        Image {
            width: parent.width
            anchors.fill: parent
            anchors.topMargin: parent.height * 0.05
            anchors.leftMargin: parent.width * 0.05
            anchors.rightMargin: parent.width * 0.05
            anchors.bottomMargin: parent.height - parent.height * icon_button_text.iconHeightScale + parent.height * 0.05
            fillMode: Image.PreserveAspectFit
            source: icon_button_text.icon.source
        }

    }
}
