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
        height: left.height + 70
        width: left.width*2+ 45
        border.width: 1
        Row{
            id: offsetBns
            anchors.bottom: parent.top
            anchors.bottomMargin: 50
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10
            IconButtonTransparent{
                icon.source: "/images/SteerZeroSmall.png"
                onClicked: offsetSpin.boundValue = 0
                anchors.verticalCenter: parent.verticalCenter
            }
            SpinBoxCM{
                id: offsetSpin
                from: 0
                to: 2500
                anchors.verticalCenter: parent.verticalCenter
                boundValue: Math.abs(settings.setVehicle_toolOffset)
                onValueChanged: {
                    if(left.checked){
                        settings.setVehicle_toolOffset = -value
                    } else {
                        settings.setVehicle_toolOffset = value
                    }
                }
            }
            TextLine{ text: utils.m_unit_abbrev()}
        }
                ButtonGroup {
                    buttons: [ leftBtn, rightBtn ]
                }

        IconButtonColor{
            id: leftBtn
            implicitWidth: 130
            implicitHeight: 170
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 15
            checkable: true
            checked: (settings.setVehicle_tooloffset < 0)
            icon.source: "/images/Config/ToolOffsetNegativeLeft.png"
            property bool toggle: false
            onClicked: if (toggle) { checked = false ; toggle = false }
            onPressed: if (checked) toggle=true
        }

        IconButtonColor{
            id: rightBtn
            implicitWidth: 130
            implicitHeight: 170
            anchors.left: leftBtn.right
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 15
            checkable: true
            checked: (settings.setVehicle_tooloffset > 0)
            icon.source: "/images/Config/ToolOffsetPositiveRight.png"
            property bool toggle: false
            onClicked: if (toggle) { checked = false ; toggle = false }
            onPressed: if (checked) toggle=true
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
        height: left.height + 70
        width: left.width*2+ 45
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
                value: 0
            }
            TextLine{ text: utils.m_unit_abbrev()}
        }
        IconButtonColor{
            id: overlap
            implicitWidth: 130
            implicitHeight: 170
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 15
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
            icon.source: "/images/Config/ToolGap.png"
            TextLine{ text: qsTr("Gap"); anchors.top: right.bottom}
        }
    }
}
