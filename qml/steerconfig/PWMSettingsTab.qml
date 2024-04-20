import QtQuick
import QtQuick.Controls.Fusion

import ".."
import "../components"

Item{
    id: gainTab
    Column{
        id: gainColumn
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        height: childrenRect.height
        width: childrenRect.width
        anchors.margins: 5
        spacing: 45
        anchors.leftMargin: 20
        SliderCustomized {
            id: propGainlider
            leftText: value
            width: 200
            from: 0
            to: 200
            value: Math.round(settings.setAS_Kp, 0)
            onValueChanged: settings.setAS_Kp = value
            centerTopText: "Proportional Gain"
            stepSize: 1
        }
        SliderCustomized {
            id: maxLimitSlider
            centerTopText: "Maximum Limit"
            leftText: value
            width: 200
            from: 0
            to: 254
            value: Math.round(settings.setAS_highSteerPWM, 0)
            onValueChanged: settings.setAS_highSteerPWM = value
            stepSize: 1
        }
        SliderCustomized {
            id: min2moveSlider
            width: 200
            from: 0
            to: 100
            value: Math.round(settings.setAS_minSteerPWM, 0)
            onValueChanged: settings.setAS_minSteerPWM = value
            leftText: value
            centerTopText: "Minimum to Move"
            stepSize: 1
        }
    }
    Image {
        width: parent.width
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        height: parent.height
        source: "/images/Steer/Sf_GainTab.png"
    }

}
