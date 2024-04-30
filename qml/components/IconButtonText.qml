import QtQuick


IconButton {
    id: icon_button_text

    width: 100  * theme.scaleWidth
    height: 100 * theme.scaleHeight

    property double iconHeightScale: 0.75

    property string buttonText: ""

    onButtonTextChanged: {
        text = buttonText
    }
}
