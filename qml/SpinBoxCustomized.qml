import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    id: spinBox_Customized
    property int from: 0
    property int to: 10
    property int value: 0
    property string text: ""
	property int stepSize: 1
    property bool editable: true
    width: spinner.width
    property int spinValue: spinner.value

    SpinBox {
        id: spinner
        from: spinBox_Customized.from
        to: spinBox_Customized.to
        editable: spinBox_Customized.editable
        value: spinBox_Customized.value
		stepSize: spinBox_Customized.stepSize
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        onValueChanged: {
            if (value == spinBox_Customized.from) {
                spin_message.visible = true
                spin_message.text = "Must be "+from+" or greater"
            } else if(value == spinBox_Customized.to){
                spin_message.visible = true
                spin_message.text = "Can't be larger than " + to
            }else {
                spin_message.visible = false
            }

            //some validation here
            //emit signal.  We know our section number because it's in the model
        }
    }

    Text {
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
