import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    width: 800
    height: 600

    MockSettings {
        id: settings
    }
    
    UnitConversion {
        id: utils
    }

    Component.onCompleted: {
        test.value = 0.04 //4 cm
    }

    SpinBoxCM {
        id: test
        
        from: 4 //centimetres
        to: 100 //centimetres
        text: "section width"

        onValueChanged: {
            console.debug("value in metres is " + test.value + " which is local units " + utils.cm_to_unit(test.value))
        }
    }

    Button {
        id: test_button
        anchors.top: test.bottom
        text: "Click to change number"
        onClicked: test.value = 0.5 //metres
    }
}
