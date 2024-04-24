import QtQuick
import QtQuick.Controls.Fusion

import ".."
import "../components"

Rectangle{
    id: configTractorAntenna
    anchors.fill: parent
    color: aog.backgroundColor
    visible: false
    Image {
        id: antImage
        //3 vehicle types  tractor=0 harvestor=1 4wd=2
        source: settings.setVehicle_vehicleType === 0 ? "/images/AntennaTractor.png" :
                settings.setVehicle_vehicleType === 1 ? "/images/AntennaHarvester.png" :
                settings.setVehicle_vehicleType === 2 ? "/images/Antenna4WD.png" :
                "/images/Config/ConSt_Mandatory.png"
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
        to: 999
        editable: true
        boundValue: settings.setVehicle_antennaPivot
        onValueModified: settings.setVehicle_antennaPivot = value
    }
    SpinBoxCM{
        id: antennaHeight
        anchors.top: antImage.top
        anchors.topMargin: 100
        anchors.left: antImage.right
        anchors.leftMargin: -50
        from: 0
        to: 1000
        editable: true
        boundValue: settings.setVehicle_antennaHeight
        onValueModified: settings.setVehicle_antennaHeight = value
    }
    SpinBoxCM{
        id: antennaOffset
        anchors.bottom: antImage.bottom
        anchors.bottomMargin: 70
        anchors.left: antImage.right
        anchors.leftMargin: -25
        from: -500
        to: 500
        editable: true
        boundValue: settings.setVehicle_antennaOffset
        onValueModified: settings.setVehicle_antennaOffset = value
    }
}
