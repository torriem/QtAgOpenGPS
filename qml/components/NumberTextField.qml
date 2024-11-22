import QtQuick
import QtQuick.Controls

TextField {
	id: root
	property double bottomVal: 1
	property double topVal: 100
	property int decimals: 1
	enabled: true
	width: 200
	height: 50
	text: "0.0"
	color: enabled ? "black" : "gray"

	font.pointSize: 20
	background: Rectangle{
		property bool isFocus: parent.focus
		id: background
		color: "white"
		border.color: "black"
		border.width: 1
		onIsFocusChanged:	border.color = isFocus ? "blue" : "black"
	}
	validator: DoubleValidator {
		id: validator
		bottom: root.bottomVal
		top: root.topVal
		decimals: root.decimals
	}
}
