import QtQuick 2.0

Rectangle {

    UnitConversion {
        id: utils
    }

    AOGInterface {
        id: aog
    }

    id: ligthbar
    width: 600
    height: 40

    property double dotDistance: 0

    Text {
        id: number
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        text: utils.cm_to_unit_string(aog.avgPivDistance / 1000,0)
        color: (aog.avgPivDistance > 0.01) ?
                   "#f9804c" :
                   "#80f34c"
    }




}
