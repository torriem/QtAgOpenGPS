import QtQuick 2.0

//This is a spinbox for displaying dimensions that are either
//cm or inches

//The from and to values are *cm*, but the final value output is in metres always

Item {
   id: spinBoxCM
    property int from: 0 //these are in cm
    property double value: 1 //meters
    property int to: 30 //centimeters
    property int stepSize: 1
    property bool editable: true

    property bool suppress_onchange_in: false
    property bool suppress_onchange_out: false

    width: spinner.width
    height: spinner.height

    onValueChanged: {
        //value changed from the outside; we need
        //the spin box to update itself but not set
        //off the spinner's onValueChanged routine
        //and try to set this value again, causing a loop
        if (suppress_onchange_out)
            return; //do nothing since it was the spinner that originated the change

        //otherwise it came from outside

        //value is always in metres.  but the from and to are in cm.

        if ((value*100) < from) value = from/ 100.0
        if ((value*100) > to) value = to / 100.0

        suppress_onchange_in = true
        spinner.value = utils.cm_to_unit(value)
        suppress_onchange_in = false
    }

    SpinBoxCustomized {
        id: spinner
        from: utils.cm_to_unit(spinBoxCM.from / 100.0)
        to: utils.cm_to_unit(spinBoxCM.to / 100.0)
        editable: spinBoxCM.editable
        value: utils.cm_to_unit(SpinBoxCM.value) // should be in metres!
        stepSize: spinBoxCM.stepSize
        anchors.fill: parent

        onValueChanged: {
            //Not sure if this is needed! test it.
            if (spinBoxCM.suppress_onchange_in)
                return;

            spinBoxCM.suppress_onchange_out = true
            spinBoxCM.value = utils.cm_from_unit(spinner.value)
            spinBoxCM.suppress_onchange_out = false
        }
    }
}
