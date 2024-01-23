import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    id: spinBox_Customized
    property int from: -1
    property int value: 1
    property int to: 10
    property string text: ""
	property int stepSize: 1
    property bool editable: true
    property bool suppress_onchange_in: false
    property bool suppress_onchange_out: false
    width: spinner.width
    height: spinner.height + spin_text.height + spin_message.height + 20

    onValueChanged: {
        //value changed from the outside; we need
        //the spin box to update itself but not set
        //off the spinner's onValueChanged routine
        //and try to set this value again, causing a loop
        if (suppress_onchange_out)
            return; //do nothing since it was the spinner that originated the change

        //otherwise it came from outside

        if (value < from) value = from
        if (value > to) value = to

        suppress_onchange_in = true
        spinner.value = value
        suppress_onchange_in = false
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

        onValueChanged: {
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

            if(spinBox_Customized.suppress_onchange_in) {
                //console.debug("suppressing inner spinbox onchange since change originated externally")
                return
            }

            //if this change was caused by the user manipulating the
            //spin box, we need to update the Item's value so that
            //users can react to it.
            spinBox_Customized.suppress_onchange_out = true
            spinBox_Customized.value = spinner.value
            spinBox_Customized.suppress_onchange_out = false

            //console.debug("SpinBox value changed; updating customized value")
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
