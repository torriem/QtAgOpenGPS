import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle{
    id: configTractorDimensions
    anchors.fill: parent
    color: "ghostwhite"

    visible: false
    Image {
        id: dimImage
        source: "qrc:/images/RadiusWheelBase.png"
        width: 350
        height: 350
        anchors.centerIn: parent
    }
    SpinBox{
        id: wheelBase
        anchors.verticalCenter: dimImage.verticalCenter
        anchors.right: dimImage.left
        from: 20
        value: 20
        to: 787
        editable: true
        Text{
            anchors.bottom: parent.top
            anchors.left: parent.left
            text: qsTr("Wheelbase")
        }
    }
    SpinBox{
        id: track
        anchors.top: dimImage.top
        anchors.left: dimImage.right
        from: 20
        value: 20
        to: 3937
        editable: true
        Text{
            anchors.bottom: parent.top
            anchors.left: parent.left
            text: qsTr("Track")
        }
    }
    SpinBox{
        id: turnRadius
        anchors.bottom: dimImage.bottom
        anchors.left: dimImage.right
        from: 20
        value: 20
        to: 787
        editable: true
        Text{
            anchors.bottom: parent.top
            anchors.left: parent.left
            text: qsTr("Turn Radius")
        }
    }
}
