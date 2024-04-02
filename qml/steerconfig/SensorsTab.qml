import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ".."
import "../components"
Item {
    id: sensorWindowItem
    ButtonGroup{
        buttons: sensorsWindow.children
    }

    RowLayout{
        id: sensorsWindow
        IconButtonColor{
            icon.source: "/images/Config/ConSt_TurnSensor.png"
            checkable: true
            isChecked: utils.readBit(settings.setArdSteer_setting0, 128)
            onCheckedChanged: utils.setBit(settings.setArdSteer_setting0, 128, checked)
            buttonText: "Turn Sensor"
            Layout.alignment: Qt.AlignCenter
        }
        IconButtonColor{
            icon.source: "/images/Config/ConSt_TurnSensorPressure.png"
            checkable: true
            isChecked: utils.readBit(settings.setArdSteer_setting1, 1)
            onCheckedChanged: utils.setBit(settings.setArdSteer_setting1, 1, checked)
            buttonText: "Pressure Turn Sensor"
            Layout.alignment: Qt.AlignCenter
        }
        IconButtonColor{
            icon.source: "/images/Config/ConSt_TurnSensorCurrent.png"
            checkable: true
            isChecked: utils.readBit(settings.setArdSteer_setting1, 2)
            onCheckedChanged: utils.setBit(settings.setArdSteer_setting1, 2, checked)
            buttonText: "Current Turn Sensor"
            Layout.alignment: Qt.AlignCenter
        }
    }
}
