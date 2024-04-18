import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

import ".."
import "../components"

Item{
    id: steerTab
    anchors.fill: parent
    ColumnLayout{
        id: steerColumn
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        width: parent.width *.5
        anchors.margins: 5
        anchors.leftMargin: 20
        IconButtonTransparent{
            width: height*2
            icon.source: "/images/SteerCenter.png"
            implicitHeight: parent.height /5 -20
            Layout.alignment: Qt.AlignCenter

        }

        SliderCustomized {
            id: wasZeroSlider
            width: 200
            leftText: value
            from: -30//idk
            to: 30
            value: 5
            centerTopText: "WAS Zero"
            Layout.alignment: Qt.AlignCenter
            implicitHeight: 50
        }
        SliderCustomized {
            id: cpDegSlider
            width: 200
            centerTopText: "Counts per Degree"
            implicitHeight: 50
            from: 1
            to: 255
            value: Math.round(settings.setAS_countsPerDegree, 0)
            onValueChanged: settings.setAS_countsPerDegree = value
            leftText: value
            stepSize: 1
            Layout.alignment: Qt.AlignCenter
        }
        SliderCustomized {
            id: ackermannSlider
            width: 200
            centerTopText: "AckerMann"
            implicitHeight: 50
            from: 1
            to: 200
            leftText: value
            value: Math.round(settings.setAS_ackerman, 0)
            onValueChanged: settings.setAS_ackerman = value
            stepSize: 1
            Layout.alignment: Qt.AlignCenter
        }
        SliderCustomized {
            id: maxSteerSlider
            implicitWidth: 200
            leftText: value
            implicitHeight: 50
            centerTopText:"Max Steer Angle"
            from: 10
            to: 80
            value: Math.round(settings.setVehicle_maxSteerAngle)
            onValueChanged: settings.setVehicle_maxSteerAngle= value
            stepSize: 1
            Layout.alignment: Qt.AlignCenter
        }
    }
    Image {
        width: parent.width
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        height: parent.height
        source: "/images/Steer/Sf_SteerTab.png"
    }

}
