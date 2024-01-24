import QtQuick 2.0
import QtQuick.Controls 2.5

/*SpinBoxCustomized{
	id: spinBox_singledigit
	property int decimals: 1
	property double fromDec: 10
	property double toDec: 100
	property double valueDec: 50
	fromVal: fromDec*10 
	toVal: toDec*10
	valueVal: valueDec*10
	stepSize: .1
}*/

Item {
    id: spinBox_singledigit
    property double from: 0
    property double value: 1
    property double to: 10 * 10
    property string text: ""
	property int stepSize: 1
	property int decimals: 1
    property bool editable: true
    width: spinner.width
    height: 100
    property real realValue: value / 10

    signal valueModified()

    function setValue(value) {
        //TODO.  Is this right?
        spinner.value = value
    }

    SpinBox {
        id: spinner
        from: spinBox_singledigit.from * 10
        to: spinBox_singledigit.to *10
        editable: spinBox_singledigit.editable
        value: value
		stepSize: spinBox_singledigit.stepSize
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
		property int decimals: spinBox_singledigit.decimals

        onValueModified: {
            if (value == spinner.from) {
                spinBox_singledigit.value = value
                spin_message.visible = true
                spin_message.text = "Must be "+spinner.from /10+" or greater"
            } else if(value == spinner.to){
                spin_message.visible = true
                spin_message.text = "Can't be larger than " + spinner.to /10
            }else {
                spin_message.visible = false
            }

            spinBox_singledigit.valueModified()
        }

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
