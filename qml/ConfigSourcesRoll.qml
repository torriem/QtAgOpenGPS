import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle{
    id: configSourcesRoll
    anchors.fill: parent
    color: "ghostwhite"
    visible: false
    IconButtonColor{
        objectName: "btnRemoveOffset"
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        buttonText: qsTr("Remove Offset")
        icon: "/images/Config/ConDa_RemoveOffset.png"
        isChecked: false
    }
    IconButtonColor{
        objectName: "zeroRoll"
        buttonText: "Zero Roll"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
        icon: "/images/Config/ConDa_RollSetZero.png"
        isChecked: false
    }
    IconButtonColor{
        objectName: "btnResetIMU"
        anchors.left: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
        icon: "/images/Config/ConDa_ResetIMU.png"
        isChecked: false
    }

    IconButtonColor{
        objectName: "btnInvertRoll"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.horizontalCenter
        anchors.topMargin: 20
        buttonText: qsTr("Invert Roll")
        icon: "/images/Config/ConDa_InvertRoll.png"
        isChecked: false
    }
    Rectangle{
        id: rollFilterSlider
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: 50
        anchors.leftMargin: 50
        width: 270
        height: 50
        //color: "lightgray"
        SliderCustomized{
            objectName: "rollFilterSlider"
            anchors.fill: parent
            from: 0
            to: 98
            value: 0
            leftTopText: "Less"
            centerTopText: "Roll Filter"
            rightTopText: "More"
        }
    }
    Image {
        source: "/images/Config/ConD_RollHelper"
        anchors.right: parent.right
        anchors.rightMargin: 50
        width: 150
        height: 200
        anchors.verticalCenter: parent.verticalCenter
    }
}
