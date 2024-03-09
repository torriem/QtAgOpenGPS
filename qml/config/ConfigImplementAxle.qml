import QtQuick 2.0
import QtQuick.Controls 2.5

import ".."

Rectangle{
    anchors.fill: parent
    visible: true
    color: "ghostwhite"

    IconButtonTransparent{
        icon.source: "/images/SteerZeroSmall.png"
        onClicked: {offsetSpin.value = 0; offsetSpin.boundValue = 0}
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: forwardBackSpin.bottom
        anchors.topMargin:30
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
        implicitWidth: 250
        implicitHeight: 400
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 50
        checkable: true
        isChecked: (settings.setTool_trailingToolToPivotLength > 0)
        icon.source: "/images/Config/ToolHitchPivotOffsetNeg.png"
        onClicked: settings.setTool_trailingToolToPivotLength = Math.abs(settings.setTool_trailingToolToPivotLength)
    }

    IconButtonColor{
        id: backBtn
        implicitWidth: 250
        implicitHeight: 400
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 50
        isChecked: (settings.setTool_trailingToolToPivotLength <= 0)
        checkable: true
        onClicked: settings.setTool_trailingToolToPivotLength = -Math.abs(settings.setTool_trailingToolToPivotLength)
        icon.source: "/images/Config/ToolHitchPivotOffsetPos.png"
    }
}
