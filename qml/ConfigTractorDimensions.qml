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
        source: "/images/RadiusWheelBase.png"
        width: 350
        height: 350
        anchors.centerIn: parent
    }
    SpinBoxCM{
        id: wheelbase
        anchors.verticalCenter: dimImage.verticalCenter
        anchors.right: dimImage.left
        from: 20
        value: settings.setVehicle_wheelbase
        Connections {
            target: settings
            function onSetVehicle_wheelbaseChanged(){
                wheelbase.value = settings.setVehicle_wheelbase
            }
        }
        onValueChanged: settings.setVehicle_wheelbase = value
        to: 787
            text: qsTr("Wheelbase")
    }
    SpinBoxCM{
        id: trackWidth
        anchors.top: dimImage.top
        anchors.left: dimImage.right
        from: 50
        value: settings.setVehicle_trackWidth
        onValueChanged: settings.setVehicle_trackWidth = value
        Connections {
            target: settings
            function onSetVehicle_trackWidthChanged(){
                trackWidth.value = settings.setVehicle_trackWidth
            }
        }
        to: 9999
        text: qsTr("Track")
    }
    SpinBoxCM{
        id: minTurningRadius
        anchors.bottom: dimImage.bottom
        anchors.left: dimImage.right
        from: 50
        value: settings.setVehicle_minTurningRadius
        onValueChanged: settings.setVehicle_minTurningRadius = value
        Connections {
            target: settings
            function onSetVehicle_minTurningRadiusChanged(){
                minTurningRadius.value = settings.setVehicle_minTurningRadius
            }
        }
        to: 9999
        text: qsTr("Turn Radius")
    }
}
