import QtQuick
import QtQuick.Controls

SpinBoxCustomized{
	id: spinBoxDecimal
	decimals: 1
	onValueChanged: {
		valueModified()
	}
}
