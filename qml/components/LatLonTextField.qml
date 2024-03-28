import QtQuick
import QtQuick.Controls

TextField {
    id: latlon_textfield

    property bool suppress_onchange: false
    property color defaultTextColor

    signal manualTextChanged()

    Component.onCompleted: {
        //save default color. Seems like a hack to me
        defaultTextColor = color
    }

    function set_without_onchange(new_value) {
        suppress_onchange = true
        text = Number(new_value).toLocaleString(Qt.locale(),'f',9)
        suppress_onchange = false
    }

    onTextChanged: {
        //manual entry!
        if (! suppress_onchange) {
            manualTextChanged()
        }

        var value = Number(text)
        if ((value > 180) || (value < -180)) {
            color = "red"
        } else {
            color = defaultTextColor
        }
    }

    validator: DoubleValidator {
    }

    onEditingFinished: {
        var value = Number(text)
        if (value > 180) {
            text = "180.000000000"
        } else if (value < -180) {
            text = "-180.000000000"
        }
    }

    selectByMouse: true
}
