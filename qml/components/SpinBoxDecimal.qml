import QtQuick
import QtQuick.Controls

SpinBoxCustomized{
	//set how many decimals you want
	id: spinBoxDecimal
	decimals: 1
	onValueChanged: {
		valueModified()
	}
}
