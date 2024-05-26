import QtQuick
import QtQuick.Controls.Fusion

Item {
    id: spinBox_Customized
    property double from
    property double value
    property double to
    property string text: ""
	property int stepSize: 1
    property bool editable
    property double boundValue
    property int fontPixelSize: 15
	property int decimals: 0
	property int decimalFactor: 1

	onDecimalsChanged: {
		if (decimals > 0)
			decimalFactor = Math.pow(10, decimals)
		else
			decimalFactor = 1
	}
    signal valueModified()

    width: spinner.width
    height: spinner.height + (spin_message.length > 0 ? spin_message.height : 0) + (spin_text.length > 0 ? spin_text.height : 0)

    //this method sets the spinner value without firing the
    //valueChanged signal

    function setValue(value) {
        spinner.value = value
    }

    onBoundValueChanged: {
        value = boundValue
        spinner.value = boundValue
    }

	SpinBoxFusion{
		id: spinner
		from: spinBox_Customized.from * decimalFactor
		to: spinBox_Customized.to * decimalFactor
		editable: spinBox_Customized.editable
		value: spinBox_Customized.value * decimalFactor
		stepSize: spinBox_Customized.stepSize
		property int decimals: spinBox_Customized.decimals
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.verticalCenter: parent.verticalCenter
		height: 30 * theme.scaleHeight
		width: 100 * theme.scaleWidth

		Keys.onReturnPressed: {
			//console.debug("enter was pressed.  ignore it.")
		}
		contentItem: TextInput {
			id: text_input
			text: parent.value
		}

		onValueModified: {
			//this only fires when the user interactively changes the spinbox.

			if (value / decimalFactor == spinBox_Customized.from) {
				spin_message.visible = true
				spin_message.text = "Min:"+from / decimalFactor
			} else if(value / decimalFactor == spinBox_Customized.to){
				spin_message.visible = true
				spin_message.text = "Max: " + to / decimalFactor
			}else {
				spin_message.visible = false
			}
			spinBox_Customized.value = spinner.value/ decimalFactor
			text_input.text = spinBox_Customized.value
			spinBox_Customized.valueModified()

		}
		textFromValue: function(value, locale) {
			return Number(value / decimalFactor).toLocaleString(locale, 'f', spinner.decimals)
		}

		valueFromText: function(text, locale) {
			return Number.fromLocaleString(locale, text) * decimalFactor
		}
	}

	Text {
		visible: false
		onTextChanged: {
			visible = true
		}
		id: spin_text
		text: spinBox_Customized.text
		anchors.bottom: spinner.top
		anchors.left: spinner.left
		font.pixelSize: spinBox_Customized.fontPixelSize
		onVisibleChanged: {
			if (visible)
			height = text.height 
			else
			height = 0
		}
	}

	Text {
		id: spin_message
		visible: false
		text: ""
		color: "red"
		anchors.top: spinner.bottom
		anchors.left: spinner.left
		onVisibleChanged: {
			if (visible)
			height = text.height 
			else
			height = 0
		}
	}
}
