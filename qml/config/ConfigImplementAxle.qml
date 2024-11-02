// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Set where main axle is on implement
import QtQuick
import QtQuick.Controls.Fusion

import ".."
import "../components"

Rectangle{
    anchors.fill: parent
    visible: true
    color: aog.backgroundColor

    IconButtonTransparent{
        icon.source: "/images/SteerZeroSmall.png"
        onClicked: {offsetSpin.value = 0; offsetSpin.boundValue = 0}
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: forwardBackSpin.bottom
        anchors.topMargin:30 * theme.scaleHeight
    }
    SpinBoxCM{
        id: forwardBackSpin
        from: 0
        to: 2000
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        boundValue: Math.abs(settings.setTool_trailingToolToPivotLength)
        onValueChanged: {
            if(backBtn.checked){
                settings.setTool_trailingToolToPivotLength = -value
            } else {
                settings.setTool_trailingToolToPivotLength = value
            }
        }
    }
    TextLine{
        anchors.bottom: forwardBackSpin.top
        text: utils.cm_unit_abbrev()
    }
        ButtonGroup {
            buttons: [ forwardBtn, backBtn ]
        }

    IconButtonColor{
        id: forwardBtn
        implicitWidth: 250 * theme.scaleWidth
        implicitHeight: 400 * theme.scaleHeight
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 50 * theme.scaleWidth
        checkable: true
        isChecked: (settings.setTool_trailingToolToPivotLength > 0)
        icon.source: "/images/Config/ToolHitchPivotOffsetNeg.png"
        onClicked: settings.setTool_trailingToolToPivotLength = Math.abs(settings.setTool_trailingToolToPivotLength)
    }

    IconButtonColor{
        id: backBtn
        implicitWidth: 250 * theme.scaleWidth
        implicitHeight: 400 * theme.scaleHeight
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 50 * theme.scaleWidth
        isChecked: (settings.setTool_trailingToolToPivotLength <= 0)
        checkable: true
        onClicked: settings.setTool_trailingToolToPivotLength = -Math.abs(settings.setTool_trailingToolToPivotLength)
        icon.source: "/images/Config/ToolHitchPivotOffsetPos.png"
    }
}
