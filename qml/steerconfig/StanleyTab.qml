import QtQuick 2.0
import QtQuick.Controls 2.0

import ".."
import "../components"
Item{
    id: stanleyTab
    Column{
        id: stanleyColumn
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        height: childrenRect.height
        width: childrenRect.width
        anchors.margins: 5
        spacing: 45
        anchors.leftMargin: 20
        SliderCustomized {
            id: stanleyAggressivenessSlider
            objectName: "stanleyAgressivenessSlider"
            width: 200
            from: .1
            to: 4
            value: Math.round(settings.stanleyDistanceErrorGain, 0)
            onValueChanged: settings.stanleyDistanceErrorGain = value
            leftText: value
            centerTopText: "Agressiveness"
            stepSize: .1
        }
        SliderCustomized {
            id: overShootReductionSlider
            objectName: "overShootReductionSlider"
            width: 200
            from: .1
            to: 1.5
            value: Math.round(settings.stanleyHeadingErrorGain, 0)
            onValueChanged: settings.stanleyHeadingErrorGain = value
            leftText: value
            centerTopText: "OverShoot Reduction"
            stepSize: .1
        }
        SliderCustomized {
            id: integralStanleySlider
            objectName: "integralStanleySlider"
            width: 200
            from: 0
            to: 100
            value: Math.round(settings.stanleyIntegralGainAB * 100, 0)
            onValueChanged: settings.stanleyIntegralGainAB = value /100
            leftText: value
            centerTopText: "Integral"
            stepSize: 1
        }
    }
    Image {
        width: parent.width
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        height: parent.height
        source: "/images/Steer/Sf_Stanley.png"
    }

}
