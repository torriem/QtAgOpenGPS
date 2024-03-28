import QtQuick
import QtQuick.Controls

Item {
    id: spinBox_singledigit
    property double from: 0
    property double value: 1
    property double to: 10 * 10
    property string text: ""
	property int stepSize: 1
	property int decimals: 1
    property bool editable: true
    property double boundValue
    width: spinner.width
    height: 100

    signal valueModified()

    function setValue(newvalue) {
        spinner.value = newvalue * 10
        spinBox_singledigit.value = newvalue
    }

    onBoundValueChanged: {
        value = boundValue
    }

    SpinBox {
        id: spinner
        from: spinBox_singledigit.from * 10
        to: spinBox_singledigit.to *10
        editable: spinBox_singledigit.editable
        value: spinBox_singledigit.value * 10
		stepSize: spinBox_singledigit.stepSize
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
		property int decimals: spinBox_singledigit.decimals

        onValueModified: {
            if (value == spinner.from) {
                spin_message.visible = true
                spin_message.text = "Must be "+spinner.from /10+" or greater"
            } else if(value == spinner.to){
                spin_message.visible = true
                spin_message.text = "Can't be larger than " + spinner.to /10
            }else {
                spin_message.visible = false
            }

            spinBox_singledigit.value = value / 10

            spinBox_singledigit.valueModified()
        }

        validator: DoubleValidator {}

		textFromValue: function(value, locale) {
			return Number(value / 10).toLocaleString(locale, 'f', spinner.decimals)
		}

		valueFromText: function(text, locale) {
			return Number.fromLocaleString(locale, text) * 10
		}
	}

	Text {
        text: spinBox_singledigit.text
        anchors.bottom: spinner.top
        anchors.left: spinner.left
        font.pixelSize: 15
    }
    Text {
        id: spin_message
        visible: false
        text: "message"
        color: "red"
        anchors.top: spinner.bottom
        anchors.left: spinner.left
    }
}
