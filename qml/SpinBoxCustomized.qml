import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    id: spinBox_Customized
    property int fromVal: 0
    property int valueVal: 1
    property int toVal: 10
    property string title: "Hello"
    width: spinner.width
    height: 100

    SpinBox {
        id: spinner
        from: spinBox_Customized.fromVal
        to: spinBox_Customized.toVal
        editable: true
        value: valueVal
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        onValueChanged: {
            if (value == spinBox_Customized.fromVal) {
                spin_message.visible = true
                spin_message.text = "Must be "+spinBox_Customized.fromVal+" or greater"
            } else if(value == spinBox_Customized.toVal){
                spin_message.visible = true
                spin_message.text = "Can't be larger than " + toVal
            }else {
                spin_message.visible = false
            }

            //some validation here
            //emit signal.  We know our section number because it's in the model
        }
    }

    Text {
        text: spinBox_Customized.title
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
