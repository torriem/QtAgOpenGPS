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
    property double fromVal: 0
    property double valueVal: 1
    property double toVal: 10 * 10
    property string title: ""
	property int stepSize: 1
	property int decimals: 1
    width: spinner.width
    height: 100
	property real realValue: valueVal / 10

    SpinBox {
        id: spinner
        from: spinBox_singledigit.fromVal * 10
        to: spinBox_singledigit.toVal *10
        editable: true
        value: valueVal
		stepSize: spinBox_singledigit.stepSize
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
		property int decimals: spinBox_singledigit.decimals

        onValueChanged: {
            if (value == spinner.from) {
                spin_message.visible = true
                spin_message.text = "Must be "+spinner.from+" or greater"
            } else if(value == spinner.to){
                spin_message.visible = true
                spin_message.text = "Can't be larger than " + spinner.to
            }else {
                spin_message.visible = false
            }

            //some validation here
            //emit signal.  We know our section number because it's in the model
		}
		textFromValue: function(value, locale) {
			return Number(value / 10).toLocaleString(locale, 'f', spinner.decimals)
		}

		valueFromText: function(text, locale) {
			return Number.fromLocaleString(locale, text) * 10
		}
	}

	Text {
        text: spinBox_singledigit.title
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
