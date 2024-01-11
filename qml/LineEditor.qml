import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle {
    id: lineEditor
    width: 300
    height: 375
    color: "ghostwhite"
    border.width: 1
    border.color: "black"
    TopLine{
        id: topLine
        titleText: "Edit AB"
    }
    Row{
        anchors.top: topLine. bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        id: lineNudge
        width: children.width
        spacing: 35
        height: children.height
        IconButtonTransparent{
            id: leftNudger
            objectName: "btnLineNudgeLeft"
            icon.source: "/images/ArrowLeft.png"
        }
            SpinBoxOneDecimal{
            width: leftNudger.width
            height: 50
            anchors.verticalCenter: parent.verticalCenter
                objectName: "lineNudgeDistance"
                text: qsTr("cm")
                from: 0
                value: 7.5
                to: 9999
            }

        IconButtonTransparent{
            objectName: "btnLineNudgeRight"
            icon.source: "/images/ArrowRight.png"
        }
    }
    Row{
        id: middleIcons
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: lineNudge.bottom
        anchors.topMargin: 35
        width: children.width
        spacing: 35
        height: children.height
        IconButtonTransparent{
            objectName: "btnLineSwapAB"
            icon.source: "/images/ABSwapPoints.png"
        }
        IconButtonTransparent{
            objectName: "btnLineSnap2Pivot"
            icon.source: "/images/SnapToPivot.png"
        }

    }
    Row{
        id: bottomIcons
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: middleIcons.bottom
        anchors.topMargin: 35
        width: children.width
        spacing: 15
        height: children.height
        IconButtonText{
            icon.source: "/images/Cancel64.png"
            onClicked: lineEditor.visible = false
            color1: "transparent"
            color2: "transparent"
            color3: "transparent"
            buttonText: "Cancel"
            width: 75
            height: 75
        }
        IconButtonText{
            objectName: "btnLineSaveforNow"
            icon.source: "/images/FileDontSave.png"
            color1: "transparent"
            color2: "transparent"
            color3: "transparent"
            buttonText: "For Now"
            width: 75
            height: 75
            onClicked: lineEditor.visible = false
        }
        IconButtonText{
            objectName: "btnLineSave"
            icon.source: "/images/FileSave.png"
            color1: "transparent"
            color2: "transparent"
            color3: "transparent"
            buttonText: "Save"
            width: 75
            height: 75
            onClicked: lineEditor.visible = false
        }
    }
}
