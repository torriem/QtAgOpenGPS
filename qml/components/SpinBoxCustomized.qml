import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import QtQuick.Controls.impl
import QtQuick.Controls.Fusion
import QtQuick.Controls.Fusion.impl

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
    height: spinner.height + spin_text.height + spin_message.height + 20

    //this method sets the spinner value without firing the
    //valueChanged signal

    function setValue(value) {
        spinner.value = value
		console.log("set value to " + value)
    }

    onBoundValueChanged: {
        value = boundValue
        spinner.value = boundValue
    }

    T.SpinBox {
		id: spinner
		from: spinBox_Customized.from * decimalFactor
		to: spinBox_Customized.to * decimalFactor
		editable: spinBox_Customized.editable
		value: spinBox_Customized.value * decimalFactor
		stepSize: spinBox_Customized.stepSize
		property int decimals: spinBox_Customized.decimals
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.verticalCenter: parent.verticalCenter
		// Note: the width of the indicators are calculated into the padding
		implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
		contentItem.implicitWidth + leftPadding + rightPadding)
		implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
		implicitContentHeight + topPadding + bottomPadding,
		up.implicitIndicatorHeight + down.implicitIndicatorHeight)

		onValueChanged: {
			console.log("value changed to " + value / decimalFactor)
		}
		padding: 4
		leftPadding: padding + (spinner.mirrored ? (up.indicator ? up.indicator.width : 0) : 0)
		rightPadding: padding + (!spinner.mirrored ? (up.indicator ? up.indicator.width : 0) : 0)

		validator: IntValidator {
			locale: spinner.locale.name
			bottom: Math.min(spinner.from, spinner.to)
			top: Math.max(spinner.from, spinner.to)
		}

		contentItem: TextInput {
			z: 2
			//text: spinner.displayText //this line causes a loop
			//don't ask why, you would think qt's default would work.
			text:  spinBox_Customized.value//this seems to fix above error
			font: spinner.font
			color: aog.blackDayWhiteNight
			selectionColor: spinner.palette.highlight
			selectedTextColor: spinner.palette.highlightedText
			horizontalAlignment: Qt.AlignHCenter
			verticalAlignment: Qt.AlignVCenter

			readOnly: !spinner.editable
			validator: spinner.validator
			inputMethodHints: spinner.inputMethodHints
			clip: width < implicitWidth
		}

		up.indicator: PaddedRectangle {
			x: spinner.mirrored ? 1 : spinner.width - width - 1
			y: 1
			height: spinner.height / 2 - 1
			implicitWidth: 16
			implicitHeight: 10

			radius: 1.7
			clip: true
			topPadding: -2
			leftPadding: -2
			//color: spinner.up.pressed ? Fusion.buttonColor(spinner.palette, false, true, true) : "transparent"
			color: "transparent"

			ColorImage {
				scale: -1
				width: parent.width
				height: parent.height
				opacity: enabled ? 1.0 : 0.5
				color: aog.blackDayWhiteNight
				source: "qrc:/qt-project.org/imports/QtQuick/Controls/Fusion/images/arrow.png"
				fillMode: Image.Pad
			}
		}

		down.indicator: PaddedRectangle {
			x: spinner.mirrored ? 1 : spinner.width - width - 1
			y: spinner.height - height - 1
			height: spinner.height / 2 - 1
			implicitWidth: 16
			implicitHeight: 10

			radius: 1.7
			clip: true
			topPadding: -2
			leftPadding: -2
			//color: spinner.down.pressed ? Fusion.buttonColor(spinner.palette, false, true, true) : "transparent"
			color: "transparent"

			ColorImage {
				width: parent.width
				height: parent.height
				opacity: enabled ? 1.0 : 0.5
				color: aog.blackDayWhiteNight
				source: "qrc:/qt-project.org/imports/QtQuick/Controls/Fusion/images/arrow.png"
				fillMode: Image.Pad
			}
		}

		background: Rectangle {
			implicitWidth: 120
			implicitHeight: 24

			radius: 2
			color: aog.whiteDayBlackNight
			border.color: aog.blackDayWhiteNight

			Rectangle {
				x: 2
				y: 1
				width: parent.width - 4
				height: 1
				color: "transparent"
			}

			Rectangle {
				x: spinner.mirrored ? 1 : parent.width - width - 1
				y: 1
				width: Math.max(spinner.up.indicator ? spinner.up.indicator.width : 0,
				spinner.down.indicator ? spinner.down.indicator.width : 0) + 1
				height: parent.height - 2

				radius: 2
				gradient: Gradient { //this should all really be removed.
					GradientStop {
						position: 0
						color: aog.whiteDayBlackNight
					}
					GradientStop {
						position: 1
						color: aog.whiteDayBlackNight
					}
				}

				Rectangle {
					x: spinner.mirrored ? parent.width - 1 : 0
					height: parent.height
					width: 1
					color: Fusion.outline(spinner.palette)
				}
			}

			Rectangle {
				x: 1; y: 1
				width: parent.width - 2
				height: parent.height - 2
				color: "transparent"
				border.color: Color.transparent(Fusion.highlightedOutline(spinner.palette), 40 / 255)
				visible: spinner.activeFocus
				radius: 1.7
			}
		}

		Keys.onReturnPressed: {
			//console.debug("enter was pressed.  ignore it.")
		}

		onValueModified: {
			//this only fires when the user interactively changes the spinbox.

			if (value == spinBox_Customized.from) {
				spin_message.visible = true
				spin_message.text = "Must be "+from / decimalFactor+" or greater"
			} else if(value == spinBox_Customized.to){
				spin_message.visible = true
				spin_message.text = "Can't be larger than " + to / decimalFactor
			}else {
				spin_message.visible = false
			}
			spinBox_Customized.value = value / decimalFactor
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
		id: spin_text
		text: spinBox_Customized.text
		anchors.bottom: spinner.top
		anchors.left: spinner.left
		font.pixelSize: spinBox_Customized.fontPixelSize
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
