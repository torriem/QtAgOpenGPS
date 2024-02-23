import QtQuick 2.0
import QtQuick.Controls 2.5

Item{
    id: steerTab
    anchors.fill: parent
    Column{
        id: steerColumn
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        height: childrenRect.height
        width: childrenRect.width
        anchors.margins: 5
        anchors.leftMargin: 20
        spacing: 45
        SliderCustomized {
            id: wasZeroSlider
            objectName: "wasZeroSlider"
            width: 200
            leftText: value
            from: -30//idk
            to: 30
            value: 5
            centerTopText: "WAS Zero"
        }
        SliderCustomized {
            id: cpDegSlider
            objectName: "cpDegSlider"
            width: 200
            centerTopText: "Counts per Degree"
            from: 1
            to: 255
            value: Math.round(settings.setAS_countsPerDegree, 0)
            onValueChanged: settings.setAS_countsPerDegree = value
            leftText: value
            stepSize: 1
        }
        SliderCustomized {
            id: ackermannSlider
            objectName: "ackermannSlider"
            width: 200
            centerTopText: "AckerMann"
            from: 1
            to: 200
            leftText: value
            value: Math.round(settings.setAS_ackerman, 0)
            onValueChanged: settings.setAS_ackerman = value
            stepSize: 1
        }
        SliderCustomized {
            id: maxSteerSlider
            objectName: "maxSteerSlider"
            width: 200
            leftText: value
            centerTopText:"Max Steer Angle"
            from: 10
            to: 80
            value: Math.round(settings.setVehicle_maxSteerAngle)
            onValueChanged: settings.setVehicle_maxSteerAngle= value
            stepSize: 1
        }
    }
    Image {
        width: parent.width
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        height: ppColumn.height
        source: "/images/Steer/Sf_SteerTab.png"
    }

}
