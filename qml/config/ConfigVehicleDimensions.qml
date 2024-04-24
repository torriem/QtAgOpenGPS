import QtQuick
import QtQuick.Controls.Fusion

import ".."
import "../components"
/*todo:
  switch to something with configImplementDimensions
  */

Rectangle{
    id: configTractorDimensions
    anchors.fill: parent
    color: aog.backgroundColor

    visible: false
    Image {
        id: dimImage
        source: Number(settings.setVehicle_vehicleType) === 0 ? "/images/RadiusWheelBase.png":
                Number(settings.setVehicle_vehicleType) === 1 ? "/images/RadiusWheelBaseHarvester.png" :
                Number(settings.setVehicle_vehicleType) === 2 ? "/images/RadiusWheelBase4WD.png":
                "/images/Config/ConSt_Mandatory.png"
        anchors.fill: parent
        anchors.margins: 15
    }
    SpinBoxCM{
        id: wheelbase
        anchors.bottom: dimImage.bottom
        anchors.right: dimImage.right
        anchors.rightMargin: dimImage.width * .65
        anchors.bottomMargin: dimImage.height *.15
        from: 20
        to: 787
        boundValue: settings.setVehicle_wheelbase
        onValueModified: settings.setVehicle_wheelbase = value
        text: qsTr("Wheelbase")
    }
    SpinBoxCM{
        id: trackWidth
        anchors.top: dimImage.top
        anchors.topMargin: dimImage.height *.25
        anchors.right: dimImage.right
        from: 50
        to: 9999
        boundValue: settings.setVehicle_trackWidth
        onValueModified: settings.setVehicle_trackWidth = value
        text: qsTr("Track")
    }
    SpinBoxCM{
        id: minTurningRadius
        anchors.bottom: dimImage.bottom
        anchors.bottomMargin: dimImage.height *.18
        anchors.right: dimImage.right
        from: 50
        to: 9999
        boundValue: settings.setVehicle_minTurningRadius
        onValueModified: settings.setVehicle_minTurningRadius = value
        text: qsTr("Turn Radius")
    }
    SpinBoxCM{
        id: hitchLength
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: dimImage.left
        from: 10
        to:3000
        boundValue: settings.setVehicle_hitchLength < 0 ? -settings.setVehicle_hitchLength : settings.setVehicle_hitchLength
        onValueModified: settings.setVehicle_hitchLength = -value
        visible: settings.setTool_isToolTrailing
    }
}
