import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import ".."

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
            width: height*1.5
            icon.source: "/images/SteerCenter.png"
            implicitHeight: parent.height /5 -10
            Layout.alignment: Qt.AlignCenter
        }

        SliderCustomized {
            id: wasZeroSlider
            objectName: "wasZeroSlider"
            width: steerColumn.width
            leftText: value
            from: -30//idk
            to: 30
            value: 5
            centerTopText: "WAS Zero"
            height: parent.height /5 -10
            Layout.alignment: Qt.AlignCenter
        }
        SliderCustomized {
            id: cpDegSlider
            objectName: "cpDegSlider"
            width: steerColumn.width
            centerTopText: "Counts per Degree"
            from: 1
            to: 255
            value: Math.round(settings.setAS_countsPerDegree, 0)
            onValueChanged: settings.setAS_countsPerDegree = value
            leftText: value
            stepSize: 1
            height: parent.height /5 -10
            Layout.alignment: Qt.AlignCenter
        }
        SliderCustomized {
            id: ackermannSlider
            objectName: "ackermannSlider"
            width: steerColumn.width
            centerTopText: "AckerMann"
            from: 1
            to: 200
            leftText: value
            value: Math.round(settings.setAS_ackerman, 0)
            onValueChanged: settings.setAS_ackerman = value
            stepSize: 1
            height: parent.height /5 -10
            Layout.alignment: Qt.AlignCenter
        }
        SliderCustomized {
            id: maxSteerSlider
            objectName: "maxSteerSlider"
            width: steerColumn.width
            leftText: value
            centerTopText:"Max Steer Angle"
            from: 10
            to: 80
            value: Math.round(settings.setVehicle_maxSteerAngle)
            onValueChanged: settings.setVehicle_maxSteerAngle= value
            stepSize: 1
            height: parent.height /5 -10
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
