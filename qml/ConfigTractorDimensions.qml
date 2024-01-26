import QtQuick 2.0
import QtQuick.Controls 2.5
/*todo:
  switch to something with configImplementDimensions
  */

Rectangle{
    id: configTractorDimensions
    anchors.fill: parent
    color: "ghostwhite"

    visible: false
    Image {
        id: dimImage
        source: Number(settings.setVehicle_vehicleType) === 0 ? "/images/RadiusWheelBase.png":
                Number(settings.setVehicle_vehicleType) === 1 ? "/images/RadiusWheelBaseHarvester.png" :
                Number(settings.setVehicle_vehicleType) === 2 ? "/images/RadiusWheelBase4WD.png":
                "/images/Config/ConSt_Mandatory.png"
        width: 350
        height: 350
        anchors.centerIn: parent
    }
    SpinBoxCM{
        id: wheelbase
        anchors.verticalCenter: dimImage.verticalCenter
        anchors.right: dimImage.left
        from: 20
        to: 787
        boundValue: settings.setVehicle_wheelbase
        onValueModified: settings.setVehicle_wheelbase = value
        text: qsTr("Wheelbase")
    }
    SpinBoxCM{
        id: trackWidth
        anchors.top: dimImage.top
        anchors.left: dimImage.right
        from: 50
        to: 9999
        boundValue: settings.setVehicle_trackWidth
        onValueModified: settings.setVehicle_trackWidth = value
        text: qsTr("Track")
    }
    SpinBoxCM{
        id: minTurningRadius
        anchors.bottom: dimImage.bottom
        anchors.left: dimImage.right
        from: 50
        to: 9999
        boundValue: settings.setVehicle_minTurningRadius
        onValueModified: settings.setVehicle_minTurningRadius = value
        text: qsTr("Turn Radius")
    }
}
