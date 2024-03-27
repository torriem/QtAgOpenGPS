import QtQuick.Controls 2.15
import QtGraphicalEffects 1.13
import QtQuick 2.15

Slider{
    value: 0
    to: 0
    from: 0
    id: sliderCustomized
    property double multiplicationValue: 1
    property alias leftText: leftText.text
    property alias rightText: rightText.text
    property alias centerTopText: topText.text
    property alias leftTopText: leftTopText.text
    property alias rightTopText: rightTopText.text
    property alias colorLeftTopText: leftTopText.color
    property alias colorRightTopText: rightTopText.color
    stepSize: 1
    topInset: topText.textHeight
    topPadding: topInset
    leftInset: leftText.textWidth
    leftPadding: leftInset
    rightInset: rightText.textWidth
    rightPadding: rightInset
    implicitHeight: 75
    implicitWidth: 200

    /*As was noted on the Qt forum, the text is still part of
      the clickable area, the idea is catcher will catch those
      and keep them from actually doing anything.
      */
    MouseArea{
        id: catcher
        anchors.top: parent.top
        anchors.bottom: backgroundRect.top
        anchors.right: parent.right
        anchors.left: parent.left
        onClicked: ("")

    }

    background: Rectangle {
        id: backgroundRect
        x: parent.leftPadding
        y: parent.topPadding + parent.availableHeight / 2 - height / 2
    /*    anchors.left: parent.left
        anchors.leftMargin: leftText.width
        anchors.top: parent.top
        anchors.topMargin: topText.height
        anchors.right: parent.right
        anchors.rightMargin: rightText.width
        anchors.bottom: parent.bottom
     */   radius: 2
        //color: "white"
        color: "blue"


    }

    handle: Rectangle{
        id: handleRect
        height: backgroundRect.height - 4
        radius: 2
        width: 8
        visible: true
        color: "lightgray"
        x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
        y: parent.topPadding + parent.availableHeight / 2 - height / 2
        anchors.verticalCenter: backgroundRect.verticalCenter
    }
    Button{
        id: rightSliderButton
        anchors.right: backgroundRect.right
        anchors.left: handleRect.right
        anchors.top: backgroundRect.top
        anchors.bottom: backgroundRect.bottom
        width: 20
        onClicked: sliderCustomized.value = sliderCustomized.value + sliderCustomized.stepSize * multiplicationValue
        background: Rectangle{
            color: "transparent"
            Text{
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                text: ">"
                color: sliderCustomized.enabled ? "black" : "grey"

            }
        }

    }
    Button{
        id: leftSliderButton
        anchors.left: backgroundRect.left
        anchors.right: handleRect.left
        anchors.top: backgroundRect.top
        onClicked: sliderCustomized.value = sliderCustomized.value - sliderCustomized.stepSize * multiplicationValue
        anchors.bottom: backgroundRect.bottom
        width: 20
        background: Rectangle{
            Text{
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                text: "<"
                color: sliderCustomized.enabled ? "black" : "grey"
            }
        }
    }
    TextLine{
        id: leftText
        text: ""
        property int textWidth: (text.length > 0) ? 30 : 0
        color: sliderCustomized.enabled ? "black" : "grey"
        anchors.left: parent.left
        anchors.verticalCenter: backgroundRect.verticalCenter
    }
    TextLine{
        id: rightText
        text: ""
        property int textWidth: (text.length > 0) ? 30 : 0
        color: sliderCustomized.enabled ? "black" : "grey"
        anchors.right: parent.right
        anchors.verticalCenter: backgroundRect.verticalCenter
        anchors.horizontalCenter: undefined
    }
    TextLine{
        id: leftTopText
        text: ""
        anchors.top: parent.top
        anchors.horizontalCenter: undefined
        color: "black"
        anchors.left: backgroundRect.left
    }
    TextLine{
        id: rightTopText
        text: ""
        anchors.right: backgroundRect.right
        anchors.horizontalCenter: undefined
        anchors.top: parent.top
        color: "black"
    }
    TextLine{
        id: topText
        property int textHeight: (text.length > 0) ? 18 : 0
        text: ""
        color: sliderCustomized.enabled ? "black" : "grey"
        anchors.top: parent.top
        anchors.horizontalCenter: backgroundRect.horizontalCenter
    }
}
