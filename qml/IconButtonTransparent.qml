import QtQuick 2.15

IconButton {
    id: icon_button_transparent
    color1: "#00000000"
    color2: "#00000000"
    color3: "#00000000"
    property double iconHeightScale: (text.length > 0) ? .75 : 1

    property string buttonText: ""

    onButtonTextChanged: {
         text = buttonText
     }
}
