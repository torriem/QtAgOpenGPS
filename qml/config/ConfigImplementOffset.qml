import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
//import QtQuick.Extras 1.4
import QtQuick.Dialogs
import Qt.labs.folderlistmodel

import ".."
import "../components"

Rectangle{
    anchors.fill: parent
    visible: true
    color: "ghostwhite"
    TitleFrame{
        id: offset
        title: qsTr("Tool Offset Direction")
        font.bold: true
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 30
        anchors.bottomMargin: 100
        height: leftBtn.height + 70
        width: leftBtn.width*2+ 45
        border.width: 1
        Row{
            id: offsetBns
            anchors.bottom: parent.top
            anchors.bottomMargin: 50
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10
            IconButtonTransparent{
                icon.source: "/images/SteerZeroSmall.png"
                onClicked: {offsetSpin.value = 0; offsetSpin.boundValue = 0}
                anchors.verticalCenter: parent.verticalCenter
            }
            SpinBoxCM{
                id: offsetSpin
                from: 0
                to: 2500
                anchors.verticalCenter: parent.verticalCenter
                boundValue: Math.abs(settings.setVehicle_toolOffset)
                onValueChanged: {
                    if(leftBtn.checked){
                        settings.setVehicle_toolOffset = -value
                    } else {
                        settings.setVehicle_toolOffset = value
                    }
                }
            }
            TextLine{
                anchors.horizontalCenter: undefined;
                anchors.verticalCenter: parent.verticalCenter
                text: utils.cm_unit_abbrev()
            }
        }
        Item{
            ButtonGroup {
                buttons: [ leftBtn, rightBtn ]
            }
        }

        IconButtonColor{
            id: leftBtn
            implicitWidth: 130
            implicitHeight: 170
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 15
            checkable: true
            isChecked: (settings.setVehicle_toolOffset < 0)
            property string offsetNum: settings.setVehicle_toolOffset
            icon.source: "/images/Config/ToolOffsetNegativeLeft.png"
            onClicked: settings.setVehicle_toolOffset = -Math.abs(settings.setVehicle_toolOffset)
        }

        IconButtonColor{
            id: rightBtn
            implicitWidth: 130
            implicitHeight: 170
            anchors.left: leftBtn.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 15
            isChecked: (settings.setVehicle_toolOffset >= 0)
            checkable: true
            onClicked: settings.setVehicle_toolOffset = Math.abs(settings.setVehicle_toolOffset)
            icon.source: "/images/Config/ToolOffsetPositiveRight.png"
            TextLine{ text: qsTr("Tool Right"); anchors.top: right.bottom}
        }
    }
    TitleFrame{
        id: gapOverlap
        title: qsTr("Overlap / Gap")
        font.bold: true
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: 30
        anchors.bottomMargin: 100
        height: leftBtn.height + 70
        width: leftBtn.width*2+ 45
        border.width: 1
        Row{
            id: overlapGapBns
            anchors.bottom: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10
            anchors.bottomMargin: 50
            IconButtonTransparent{
                icon.source: "/images/SteerZeroSmall.png"
                onClicked: overlapGapSpin.value = 0
                anchors.verticalCenter: parent.verticalCenter
            }
            SpinBoxCM{
                anchors.verticalCenter: parent.verticalCenter
                id: overlapGapSpin
                from: 0
                to: 2500
                boundValue: Math.abs(settings.setVehicle_toolOverlap)
                onValueChanged: {
                    if(overlap.checked){
                        settings.setVehicle_toolOverlap = -value
                    } else {
                        settings.setVehicle_toolOverlap = value
                    }
                }
            }
            TextLine{
                anchors.horizontalCenter: undefined;
                anchors.verticalCenter: parent.verticalCenter
                text: utils.cm_unit_abbrev()
            }

        }
        Item{
            ButtonGroup {
                buttons: [ overlap, gap ]
            }
        }
        IconButtonColor{
            id: overlap
            implicitWidth: 130
            implicitHeight: 170
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 15
            checkable: true
            isChecked: (settings.setVehicle_toolOverlap >= 0)
            onClicked: settings.setVehicle_toolOverlap = Math.abs(settings.setVehicle_toolOverlap)
            icon.source: "/images/Config/ToolOverlap.png"
            TextLine{ text: qsTr("Overlap"); anchors.top: left.bottom}
        }

        IconButtonColor{
            id: gap
            implicitWidth: 130
            implicitHeight: 170
            anchors.left: left.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 15
            isChecked: (settings.setVehicle_toolOverlap < 0)
            onClicked: settings.setVehicle_toolOverlap = -Math.abs(settings.setVehicle_toolOverlap)
            checkable: true
            icon.source: "/images/Config/ToolGap.png"
            TextLine{
                anchors.horizontalCenter: undefined;
                anchors.verticalCenter: parent.verticalCenter
                text: utils.cm_unit_abbrev()
            }
        }
    }
}
