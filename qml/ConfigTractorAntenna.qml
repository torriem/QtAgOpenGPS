import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle{
    id: configTractorAntenna
    anchors.fill: parent
    color: "ghostwhite"
    visible: false
    Image {
        id: antImage
        source: "/images/AntennaTractor.png"
        width: 350
        height: 350
        anchors.centerIn: parent
    }
    SpinBoxCM{
        id: antennaPivot
        anchors.bottom: antImage.top
        anchors.left: antImage.left
        anchors.leftMargin: 30
        from: -999
        value: settings.setVehicle_antennaPivot
        onValueChanged: settings.setVehicle_antennaPivot = value
        Connections {
            target: settings
            function onSetVehicle_antennaPivotChanged(){
                antennaPivot.value = settings.setVehicle_antennaPivot
            }
        }

        to: 999
    }
    SpinBoxCM{
        id: antennaHeight
        anchors.top: antImage.top
        anchors.topMargin: 100
        anchors.left: antImage.right
        anchors.leftMargin: -50
        from: 0
        editable: true
        value: settings.setVehicle_antennaHeight
        onValueChanged: settings.setVehicle_antennaHeight = value
        Connections {
            target: settings
            function onSetVehicle_antennaHeightChanged(){
                antennaHeight.value = settings.setVehicle_antennaHeight
            }
        }
        to: 1000
    }
    SpinBoxCM{
        id: antennaOffset
        anchors.bottom: antImage.bottom
        anchors.bottomMargin: 70
        anchors.left: antImage.right
        anchors.leftMargin: -25
        from: -500
        value: settings.setVehicle_antennaOffset
        to: 500
        onValueChanged: settings.setVehicle_antennaOffset = value
        Connections {
            target: settings
            function onSetVehicle_antennaOffsetChanged(){
                antennaOffset.value = settings.setVehicle_antennaOffset
            }
        }
    }
}
