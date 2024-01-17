import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle{
    id: configTractorDimensions
    anchors.fill: parent
    color: "ghostwhite"

    visible: false
    Image {
        id: dimImage
        source: "/images/RadiusWheelBase.png"
        width: 350
        height: 350
        anchors.centerIn: parent
    }
    SpinBoxOneDecimal{
        id: wheelBase
        anchors.verticalCenter: dimImage.verticalCenter
        anchors.right: dimImage.left
        from: 20
        value: settings.setVehicle_wheelbase
        //onValueChanged: settings.setVehicle_wheelbase = value
        to: 787
            text: qsTr("Wheelbase")
    }
    SpinBoxOneDecimal{
        id: track
        anchors.top: dimImage.top
        anchors.left: dimImage.right
        from: 50
        value: settings.setVehicle_trackWidth
        //onValueChanged: settings.setVehicle_trackWidth = value
        to: 9999
        text: qsTr("Track")
    }
    SpinBoxOneDecimal{
        id: turnRadius
        anchors.bottom: dimImage.bottom
        anchors.left: dimImage.right
        from: 50
        value: settings.setVehicle_minTurningRadius
        //onValueChanged: settings.setVehicle_minTurningRadius = value
        to: 9999
        text: qsTr("Turn Radius")
    }
}
