import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    id: spinBox_Customized
    property int from
    property int value
    property int to
    property string text: ""
	property int stepSize: 1
    property bool editable
    property int boundValue

    signal valueModified()

    width: spinner.width
    height: spinner.height + spin_text.height + spin_message.height + 20

    //this method sets the spinner value without firing the
    //valueChanged signal

    function setValue(value) {
        spinner.value = value
    }

    onBoundValueChanged: {
        value = boundValue
    }

    SpinBox {
        id: spinner
        from: spinBox_Customized.from
        to: spinBox_Customized.to
        editable: spinBox_Customized.editable
        value: spinBox_Customized.value
		stepSize: spinBox_Customized.stepSize
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Keys.onReturnPressed: {
            //console.debug("enter was pressed.  ignore it.")
        }

        onValueModified: {
            //this only fires when the user interactively changes the spinbox.

            spinBox_Customized.value = value
            if (value == spinBox_Customized.from) {
                spin_message.visible = true
                spin_message.text = "Must be "+from+" or greater"
            } else if(value == spinBox_Customized.to){
                spin_message.visible = true
                spin_message.text = "Can't be larger than " + to
            }else {
                spin_message.visible = false
            }

            spinBox_Customized.valueModified()

        }
    }

    Text {
        id: spin_text
        text: spinBox_Customized.text
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
