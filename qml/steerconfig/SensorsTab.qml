import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

import ".."
import "../components"
Item {
    id: sensorWindowItem
    anchors.fill: parent
    ButtonGroup{
        buttons: sensorsWindow.children
    }

    RowLayout{
        id: sensorsWindow
        width: parent.width
        IconButtonColor{
            icon.source: "/images/Config/ConSt_TurnSensor.png"
            checkable: true
            isChecked: (settings.setArdSteer_setting0 & 128) != 0
            onCheckedChanged: checked ? (settings.setArdSteer_setting0 | (1 << 128)) : (settings.setArdSteer_setting0 & ~(1 << 128));
            buttonText: "Turn Sensor"
            Layout.alignment: Qt.AlignCenter
            colorChecked: steerConfig.color
        }
        IconButtonColor{
            icon.source: "/images/Config/ConSt_TurnSensorPressure.png"
            checkable: true
            isChecked: (settings.setArdSteer_setting1 & 2) != 0
            // onCheckedChanged: {
            //     console.log(toString(settings.setArdSteer_setting1))
            //     settings.setArdSteer_setting1 = utils.setBit(settings.setArdSteer_setting1, 1, checked)
            //     console.log(toString(settings.setArdSteer_setting1))
            // }
            buttonText: "Pressure Turn Sensor"
            Layout.alignment: Qt.AlignCenter
            colorChecked: steerConfig.color
        }
        IconButtonColor{
            icon.source: "/images/Config/ConSt_TurnSensorCurrent.png"
            checkable: true
            isChecked: (settings.setArdSteer_setting1 & 4) != 0
            /*onCheckedChanged: utils.setBit(settings.setArdSteer_setting1, 2, checked)*/
            buttonText: "Current Turn Sensor"
            Layout.alignment: Qt.AlignCenter
            colorChecked: steerConfig.color
        }
    }
}
