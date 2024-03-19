import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import ".."

Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent
        color: "ghostwhite"
        TitleFrame{
            id: fieldMenuText
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 30
            title: "Field Menu"
            font.bold: true
            width: childrenRect.width
            anchors.bottom: parent.bottom
            ColumnLayout{
                id: fieldMenuColumn
                width: children.width
                height: children.height
                IconButtonColor{
                    icon.source: "/images/TramAll.png"
                    text: "Tram Lines"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    isChecked: settings.setFeature_isTramOn
                    onCheckedChanged: settings.setFeature_isTramOn = checked
                }
                IconButtonColor{
                    icon.source: "/images/HeadlandOn.png"
                    text: "Headland"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    isChecked: settings.setFeature_isHeadlandOn
                    onCheckedChanged: settings.setFeature_isHeadlandOn = checked
                }
                IconButtonColor{
                    icon.source: "/images/BoundaryOuter.png"
                    text: "Boundary"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    isChecked: settings.setFeature_isBoundaryOn
                    onCheckedChanged: settings.setFeature_isBoundaryOn = checked
                }
                IconButtonColor{
                    icon.source: "/images/RecPath.png"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    isChecked: settings.setFeature_isRecPathOn
                    onCheckedChanged: settings.setFeature_isRecPathOn = checked
                }
            }
        }
        TitleFrame{
            id: toolsMenuText
            width: childrenRect.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: fieldMenuText.right
            anchors.margins: 30
            text: "Tools Menu"
            font.bold: true
            Column{
                id: toolsMenuColumn
                IconButtonColor{
                    icon.source: "/images/ABSmooth.png"
                    text: "AB Smooth"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    isChecked: settings.setFeature_isABSmoothOn
                    onCheckedChanged: settings.setFeature_isABSmoothOn = checked
                }
                IconButtonColor{
                    icon.source: "/images/HideContour.png"
                    text: "Hide Contour"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    isChecked: settings.setFeature_isHideContourOn
                    onCheckedChanged: settings.setFeature_isHideContourOn = checked
                }
                IconButtonColor{
                    icon.source: "/images/Webcam.png"
                    text: "WebCam"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    isChecked: settings.setFeature_isWebCamOn
                    onCheckedChanged: settings.setFeature_isWebCamOn = checked
                }
                IconButtonColor{
                    icon.source: "/images/YouTurnReverse.png"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    isChecked: settings.setFeature_isOffsetFixOn
                    onCheckedChanged: settings.setFeature_isOffsetFixOn = checked
                }
            }
        }
        TitleFrame{
            id: screenButtons
            anchors.fill: screenButtonsRow
            width: screenButtonsRow.width
            height: screenButtonsRow.height
            Row{
                id: screenButtonsRow
                spacing: 20
                width: children.width
                height: children.height
                anchors.top: screenButtons.top
                anchors.left: screenButtons.left
                anchors.leftMargin: 30
                IconButtonColor{
                    id: uturn
                    icon.source: "/images/Images/z_TurnManual.png"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    isChecked: settings.setFeature_isUTurnOn
                    onCheckedChanged: settings.setFeature_isUTurnOn = checked
                }
                IconButtonColor{
                    id: lateral
                    icon.source: "/images/Images/z_LateralManual.png"
                    Layout.alignment: Qt.AlignCenter
                    isChecked: settings.setFeature_isLateralOn
                    onCheckedChanged: settings.setFeature_isLateralOn = checked
                }
            }
        }
    }
}
