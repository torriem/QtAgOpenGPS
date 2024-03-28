import QtQuick

//This is a spinbox for displaying dimensions that are either
//cm or inches

//The from and to values are *cm*, but the final value output is in metres always

Item {
    id: spinBoxM
    property int from //these are in meters
    property double value //meters
    property int to //meters
    property int stepSize: 1
    property bool editable: true
    property string text: ""

    property double boundValue

    signal valueModified()

    width: spinner.width
    height: spinner.height

    //set the spinner value without triggering valueChanged
    function setValue(value) {
        spinner.setValue(utils.m_to_unit(value))
    }

    onBoundValueChanged: {
        spinner.value = utils.m_to_unit(boundValue)
        value = boundValue
    }

    Connections {
        target: settings
        function onSetMenu_isMetricChanged() {
            spinner.value = utils.m_to_unit(value)
        }
    }

    SpinBoxCustomized {
        id: spinner
        from: utils.m_to_unit(spinBoxM.from)
        to: utils.m_to_unit(spinBoxM.to)
        editable: spinBoxM.editable
        text: spinBoxM.text
        value: utils.m_to_unit(spinBoxM.value) // should be in metres!
        stepSize: spinBoxM.stepSize
        anchors.fill: parent

        onValueModified: {
            spinBoxM.value = utils.m_from_unit(value)
            spinBoxM.valueModified()
        }
    }
}
