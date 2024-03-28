import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ".."
import "../components"

RowLayout{
    id: sensorsWindow
    IconButtonColor{
        icon.source: "/images/Config/ConSt_TurnSensor.png"
        objectName: "btnTurnSensor"
        isChecked: false
        buttonText: "Turn Sensor"
        Layout.alignment: Qt.AlignCenter
    }
    IconButtonColor{
        icon.source: "/images/Config/ConSt_TurnSensorPressure.png"
        isChecked: false
        objectName: "btnPressureTurnSensor"
        buttonText: "Pressure Turn Sensor"
        Layout.alignment: Qt.AlignCenter
    }
    IconButtonColor{
        icon.source: "/images/Config/ConSt_TurnSensorCurrent.png"
        objectName: "btnCurrentTurnSensor"
        isChecked: false
        buttonText: "Current Turn Sensor"
        Layout.alignment: Qt.AlignCenter
    }
}
