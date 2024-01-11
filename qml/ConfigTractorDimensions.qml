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
    SpinBoxCustomized{
        id: wheelBase
        anchors.verticalCenter: dimImage.verticalCenter
        anchors.right: dimImage.left
        from: 20
        value: 20
        to: 787
            text: "Wheelbase"
    }
    SpinBoxCustomized{
        id: track
        anchors.top: dimImage.top
        anchors.left: dimImage.right
        from: 20
        value: 20
        to: 3937
        text: qsTr("Track")
    }
    SpinBoxCustomized{
        id: turnRadius
        anchors.bottom: dimImage.bottom
        anchors.left: dimImage.right
        from: 20
        value: 20
        to: 787
        text: qsTr("Turn Radius")
    }
}
