import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    width: 800
    height: 600
    color: "grey"

    AOGInterface {
        id: aog
    }

    MockSettings {
        id: settings
    }
    
    UnitConversion {
        id: utils
    }

    Component.onCompleted: {
        aog.avgPivDistance = 14 //mm
    }

    LightBar {
        id: test

        border.color: "white"
        border.width: 1
        dotDistance: aog.avgPivDistance / 10

        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        
    }
}
