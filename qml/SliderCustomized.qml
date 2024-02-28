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
    /*topInset: topText.height
    topPadding: topInset
    leftInset: leftText.width
    leftPadding: leftInset
    rightInset: rightText.width
    rightPadding: rightInset
    */topInset: topText.textHeight
    topPadding: topInset
    leftInset: leftText.text.length * leftText.font.pixelSize
    leftPadding: leftInset
    rightInset: rightText.text.length
    rightPadding: rightInset
    height: 75
    width: 200

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
        color: "white"


    }

    handle: Rectangle{
        id: handleRect
        height: backgroundRect.height - 4
        radius: 2
        width: 20
        visible: true
        color: "lightgray"
        x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
        y: parent.topPadding + parent.availableHeight / 2 - height / 2
        anchors.verticalCenter: parent.verticalCenter
    }
    /*Button{
        id: rightSliderButton
        anchors.right: sliderCustomized.right
        anchors.left: handleRect.right
        anchors.top: sliderCustomized.top
        anchors.bottom: sliderCustomized.bottom
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
        anchors.left: sliderCustomized.left
        anchors.right: handleRect.left
        anchors.top: sliderCustomized.top
        onClicked: sliderCustomized.value = sliderCustomized.value - sliderCustomized.stepSize * multiplicationValue
        anchors.bottom: sliderCustomized.bottom
        width: 20
        background: Rectangle{
            Text{
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                text: "<"
                color: sliderCustomized.enabled ? "black" : "grey"
            }
        }
    }*/
    TextLine{
        id: leftText
        text: ""
        color: sliderCustomized.enabled ? "black" : "grey"
        anchors.left: parent.left
        anchors.verticalCenter: backgroundRect.verticalCenter
    }
    TextLine{
        id: rightText
        text: ""
        color: sliderCustomized.enabled ? "black" : "grey"
        anchors.right: parent.right
        anchors.verticalCenter: backgroundRect.verticalCenter
    }
    TextLine{
        id: leftTopText
        text: ""
        anchors.left:  parent.left
        anchors.bottom: parent.top
        color: "black"
    }
    TextLine{
        id: rightTopText
        text: ""
        anchors.right: parent.right
        anchors.bottom: parent.top
        color: "black"
    }
    TextLine{
        id: topText
        property int textHeight: text.length * font.pixelSize
        text: ""
        color: sliderCustomized.enabled ? "black" : "grey"
        anchors.top: parent.top
    }
}
