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
}
