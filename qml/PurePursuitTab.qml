import QtQuick 2.0
import QtQuick.Controls 2.5

Item{
    id: ppTab
    Column{
        id: ppColumn
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        height: childrenRect.height
        width: childrenRect.width
        anchors.margins: 5
        spacing: 45
        anchors.leftMargin: 20
        SliderCustomized {
            id: acqLookAheadSlider
            objectName: "acqLookAheadSlider"
            width: 200
            from: 1
            to: 5
            value: Math.round(settings.setVehicle_goalPointLookAhead, 1)
            onValueChanged: settings.setVehicle_goalPointLookAhead = value
            leftText: value
            centerTopText: "Acquire Look Ahead"
            stepSize: .1
        }
        SliderCustomized {
            id: holdLookAheadSlider
            objectName: "holdLookAheadSlider"
            width: 200
            from: 1
            to: 5
            value: Math.round(settings.setVehicle_goalPointLookAheadHold, 1)
            onValueChanged: settings.setVehicle_goalPointLookAheadHold = value
            leftText: value
            centerTopText: "Hold Look Ahead"
            stepSize: .1
        }
        SliderCustomized {
            id: lookAheadSpeedGainSlider
            objectName: "lookAheadSpeedGainSlider"
            width: 200
            from: .5
            to: 3
            value: Math.round(settings.setVehicle_goalPointLookAheadMult, 1)
            onValueChanged: settings.setVehicle_goalPointLookAheadMult = value
            leftText: value
            centerTopText: "Look Ahead Speed Gain"
            stepSize: .1
        }
        SliderCustomized {
            id: ppIntegralSlider
            objectName: "ppIntegralSlider"
            width: 200
            from: 0
            to: 100
            value: Math.round(settings.purePursuitIntegralGainAB *100, 0)
            onValueChanged: settings.purePursuitIntegralGainAB = value /100
            leftText: value
            centerTopText: "Integral"
            stepSize: 1
        }
    }
    Image {
        width: parent.width
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        height: ppColumn.height
        source: "/images/Steer/Sf_PP.png"
    }
}
