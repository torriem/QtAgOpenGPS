import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle{
    id: configImpTimWin
    anchors.fill: parent
    color: "ghostwhite"
    visible: false
    Image{
        id: configImpImage
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottomMargin: 70
        anchors.rightMargin: 30
        anchors.leftMargin: 200
        anchors.topMargin: 100
        source: "/images/ImplementSettings.png"

        SpinBoxOneDecimal{
            anchors.right: parent.left
            anchors.top: parent.top
            from: 0.2
            value: settings.setVehicle_toolLookAheadOn
            onValueChanged: settings.setVehicle_toolLookAheadOn = value
            to: 22
            text: qsTr("On (secs)")
        }
        SpinBoxOneDecimal{
            anchors.right: parent.left
            anchors.verticalCenter: parent.verticalCenter
            from: 0
            value:settings.setVehicle_toolLookAheadOff
            onValueChanged: settings.setVehicle_toolLookAheadOff = value
            to: 20
            editable: true
            text: qsTr("Off (secs)")
        }
        SpinBoxOneDecimal{
            anchors.right: parent.left
            anchors.bottom: parent.bottom
            from: 0
            value: settings.setVehicle_toolOffDelay
            onValueChanged: settings.setVehicle_toolOffDelay = value
            to: 10
            editable: true
            text: qsTr("Delay (secs)")
        }
        SpinBoxCM{
            id: toolOffset
            anchors.left: parent.left
            anchors.leftMargin: 500
            anchors.top: parent.bottom
            anchors.bottomMargin: -10
            from: -2500
            value: settings.setVehicle_toolOffset
            onValueChanged: settings.setVehicle_toolOffset = value
            to: 2500
            Connections {
                target: settings
                function onSetVehicle_toolOffsetChanged(){
                    toolOffset.value = settings.setVehicle_toolOffset
                }
            }
            editable: true
            text: qsTr("Offset")
        }
        SpinBoxCM{
            anchors.right: parent.right
            anchors.rightMargin: 120
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 100
            from: -3000
            value: settings.setVehicle_toolOverlap
            onValueChanged: settings.setVehicle_toolOverlap
            to: 3000
            editable: true
            text: qsTr("Overlap / Gap")
            Connections {
                target: settings
                function onSetVehicle_toolOverlapChanged(){
                    hitchLength.value = settings.setVehicle_toolOverlap
                }
            }
        }
    }
}
