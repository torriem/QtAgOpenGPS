import QtQuick 2.0

//This is a spinbox for displaying dimensions that are either
//km or mi

//The from and to values are *km*, but the final value output is in metres always

Item {
    id: spinBoxKM
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
        spinner.setValue(utils.km_to_unit(value))
    }

    onBoundValueChanged: {
        value = boundValue
    }

    Connections {
        target: settings
        function onSetMenu_isMetricChanged() {
            spinner.value = utils.km_to_unit(value)
        }
    }

    SpinBoxCustomized {
        id: spinner
        from: utils.km_to_unit(spinBoxKM.from * 100.0)
        to: utils.km_to_unit(spinBoxKM.to * 100.0)
        editable: spinBoxKM.editable
        text: spinBoxKM.text
        value: utils.km_to_unit(spinBoxKM.value) // should be in metres!
        stepSize: spinBoxKM.stepSize
        anchors.fill: parent

        onValueModified: {
            spinBoxKM.value = utils.km_from_unit(value)
            spinBoxKM.valueModified()
        }
    }
}
