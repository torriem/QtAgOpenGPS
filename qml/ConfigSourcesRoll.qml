import QtQuick 2.0
import QtQuick.Controls 2.5

/*todo:
  not sure how to handle "remove offset, zero roll, and reset IMU". Will leave for now
  */
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
        text: qsTr("Remove Offset")
        icon.source: "/images/Config/ConDa_RemoveOffset.png"

    }
    IconButtonColor{
        objectName: "zeroRoll"
        text: qsTr("Zero Roll")
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
        icon.source: "/images/Config/ConDa_RollSetZero.png"
        isChecked: false
    }
    IconButtonColor{
        objectName: "btnResetIMU"
        anchors.left: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
        icon.source: "/images/Config/ConDa_ResetIMU.png"
        isChecked: false
    }

    IconButtonColor{
        objectName: "btnInvertRoll"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.horizontalCenter
        anchors.topMargin: 20
        text: qsTr("Invert Roll")
        icon.source: "/images/Config/ConDa_InvertRoll.png"
        checked: settings.setIMU_invertRoll
        onCheckedChanged: settings.setIMU_invertRoll = checked
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
            value: settings.setIMU_rollFilter
            onValueChanged: settings.setIMU_rollFilter = value
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
