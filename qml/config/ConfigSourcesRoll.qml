// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// IMU settings (reset, offset, zero, etc
import QtQuick
import QtQuick.Controls.Fusion

import ".."
import "../components"

/*todo:
  not sure how to handle "remove offset, zero roll, and reset IMU". Will leave for now
  */
Rectangle{
    id: configSourcesRoll
    anchors.fill: parent
    color: aog.backgroundColor
    visible: false
    IconButtonColor{
        objectName: "btnRemoveOffset"
        anchors.top: parent.top
        anchors.topMargin: 20 * theme.scaleHeight
        anchors.left: parent.left
        anchors.leftMargin: 20 * theme.scaleWidth
        text: qsTr("Remove Offset")
        icon.source: prefix + "/images/Config/ConDa_RemoveOffset.png"
        onClicked: {
            settings.setIMU_rollZero = 0
        }
    }
    IconButtonColor{
        id: zeroRollBtn
        text: qsTr("Zero Roll")
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20 * theme.scaleWidth
        icon.source: prefix + "/images/Config/ConDa_RollSetZero.png"
        isChecked: false
        onClicked: {
            if (aog.imuRollDegrees != 88888) {
                var roll = aog.imuRollDegrees + settings.setIMU_rollZero
                settings.setIMU_rollZero = roll;
                aog.changeImuRoll(roll)
            }
        }
    }

    Label {
        id: rollZeroDisplay
        anchors.left: zeroRollBtn.right
        anchors.verticalCenter: zeroRollBtn.verticalCenter
        anchors.leftMargin: 20 * theme.scaleWidth
        text: Number(settings.setIMU_rollZero).toLocaleString(Qt.locale(), 'f', 2);
    }

    IconButtonTransparent {
        id: rollOffsetUpBtn
        anchors.left: rollZeroDisplay.right
        anchors.verticalCenter: zeroRollBtn.verticalCenter
        anchors.leftMargin: 20 * theme.scaleWidth

        icon.source: prefix + "/images/UpArrow64.png"
        onClicked: settings.setIMU_rollZero += 0.1
    }

    IconButtonTransparent {
        id: rollOffsetDownBtn
        anchors.left: rollOffsetUpBtn.right
        anchors.verticalCenter: zeroRollBtn.verticalCenter
        anchors.leftMargin: 5 * theme.scaleWidth

        icon.source: prefix + "/images/DnArrow64.png"
        onClicked: settings.setIMU_rollZero -= 0.1
    }

    IconButtonColor{
        objectName: "btnResetIMU"
        anchors.left: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20 * theme.scaleHeight
        icon.source: prefix + "/images/Config/ConDa_ResetIMU.png"
        isChecked: false
        onClicked: {
            aog.changeImuHeading(88888);
            aog.changeImuRoll(99999)
        }
    }

    IconButtonColor{
        objectName: "btnInvertRoll"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.horizontalCenter
        anchors.topMargin: 20 * theme.scaleHeight
        text: qsTr("Invert Roll")
        icon.source: prefix + "/images/Config/ConDa_InvertRoll.png"
        checkable: true
        checked: settings.setIMU_invertRoll
        onCheckedChanged: settings.setIMU_invertRoll = checked
    }
    Rectangle{
        id: rollFilterSlider
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: 50 * theme.scaleHeight
        anchors.leftMargin: 50 * theme.scaleWidth
        width: 270 * theme.scaleWidth
        height: 50 * theme.scaleHeight
        //color: "lightgray"
        SliderCustomized{
            objectName: "rollFilterSlider"
            anchors.fill: parent
            from: 0
            to: 98
            property double boundValue: settings.setIMU_rollFilter
            value: settings.setIMU_rollFilter * 100
            onValueChanged: settings.setIMU_rollFilter = value / 100.0
            leftTopText: "Less"
            centerTopText: "Roll Filter"
            rightTopText: "More"
        }
    }
    Image {
        source: prefix + "/images/Config/ConD_RollHelper.png"
        anchors.right: parent.right
        anchors.rightMargin: 50 * theme.scaleWidth
        width: 150 * theme.scaleWidth
        height: 200 * theme.scaleHeight
        anchors.verticalCenter: parent.verticalCenter
    }
}
