import QtQuick
import QtQuick.Controls
import QtQuick.Templates as T
import QtQuick.Controls.impl
import QtQuick.Controls.Fusion
import QtQuick.Controls.Fusion.impl

/*So-this is a copy of the default spinbox value, but with a binding loop fixed
 * It has no validation, that is what SpinBoxCustomized is for.*/
T.SpinBox {
	id: spinner

	// Note: the width of the indicators are calculated into the padding
	implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
	contentItem.implicitWidth + leftPadding + rightPadding)
	implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
	implicitContentHeight + topPadding + bottomPadding,
	up.implicitIndicatorHeight + down.implicitIndicatorHeight)

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
		text:  value//this seems to fix above error
		font: spinner.font
        color: agio.blackDayWhiteNight
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
            color: agio.blackDayWhiteNight
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
            color: agio.blackDayWhiteNight
			source: "qrc:/qt-project.org/imports/QtQuick/Controls/Fusion/images/arrow.png"
			fillMode: Image.Pad
		}
	}

	background: Rectangle {
		implicitWidth: 120
		implicitHeight: 24

		radius: 2
        color: agio.whiteDayBlackNight
        border.color: agio.blackDayWhiteNight

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
                color: agio.whiteDayBlackNight
			}
			GradientStop {
				position: 1
                color: agio.whiteDayBlackNight
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
}
