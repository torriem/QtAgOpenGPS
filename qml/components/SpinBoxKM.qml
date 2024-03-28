import QtQuick

//This is a spinbox for displaying dimensions that are either
//cm or inches

//The from and to values are *cm*, but the final value output is in metres always

Item {
    id: spinBoxCM
    property int from //these are in cm
    property double value //meters
    property int to //centimeters
    property int stepSize: 1
    property bool editable: true
    property string text: ""

    property double boundValue

    signal valueModified()

    width: spinner.width
    height: spinner.height

    //set the spinner value without triggering valueChanged
    function setValue(value) {
        spinner.setValue(utils.km_to_mi(value))
    }

    onBoundValueChanged: {
        value = boundValue
    }

    Connections {
        target: settings
        function onSetMenu_isMetricChanged() {
            spinner.value = utils.km_to_mi(value)
        }
    }

    SpinBoxCustomized {
        id: spinner
        from: utils.km_to_mi(spinBoxCM.from)
        to: utils.km_to_mi(spinBoxCM.to)
        editable: spinBoxCM.editable
        text: spinBoxCM.text
        value: utils.km_to_mi(spinBoxCM.value) // should be in metres!
        stepSize: spinBoxCM.stepSize
        anchors.fill: parent

        onValueModified: {
            spinBoxCM.value = utils.mi_to_km(value)
            spinBoxCM.valueModified()
        }
    }
}
