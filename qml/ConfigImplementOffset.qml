import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4
import QtQuick.Dialogs 1.3
import Qt.labs.folderlistmodel 2.2

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
                    if(value===0){
                        leftBtn.checked = false
                        leftBtn.checkable = false
                        rightBtn.checked = false
                        rightBtn.checkable = false
                    }else{
                        rightBtn.checkable = true
                        leftBtn.checkable = true
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
            checkable: !checked
            checked: (settings.setVehicle_toolOffset < 0)
            property string offsetNum: settings.setVehicle_toolOffset
            onOffsetNumChanged: {
                console.log(offsetNum)
                if(offsetNum < 0)
                    leftBtn.checked = true
            }
            icon.source: "/images/Config/ToolOffsetNegativeLeft.png"
        }

        IconButtonColor{
            id: rightBtn
            implicitWidth: 130
            implicitHeight: 170
            anchors.left: leftBtn.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 15
            checked: (settings.setVehicle_toolOffset > 0)
            checkable: !checked
            property double offsetNum: settings.setVehicle_toolOffset
            onOffsetNumChanged: {
                console.log(offsetNum)
                if(offsetNum > 0)
                    console.log("flag")
                    rightBtn.checked = true
            }
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
                    if(value===0){
                        overlap.checked = false
                        overlap.checkable = false
                        gap.checked = false
                        gap.checkable = false
                    }else{
                        overlap.checkable = true
                        gap.checkable = true
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
            checkable: !checked
            checked: (settings.setVehicle_toolOverlap < 0)
            property string overlapGapNum: settings.setVehicle_toolOverlap
            onOverlapGapNumChanged: {
                console.log(overlapGapNum)
                if(overlapGapNum < 0)
                    overlap.checked = true
            }
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
            checked: (settings.setVehicle_toolOverlap > 0)
            checkable: !checked
            property double overlapGapNum: settings.setVehicle_toolOverlap
            onOverlapGapNumChanged: {
                console.log(overlapGapNum)
                if(overlapGapNum > 0)
                    console.log("flag")
                    gap.checked = true
            }
            icon.source: "/images/Config/ToolGap.png"
            TextLine{
                anchors.horizontalCenter: undefined;
                anchors.verticalCenter: parent.verticalCenter
                text: utils.cm_unit_abbrev()
            }
        }
    }
}
