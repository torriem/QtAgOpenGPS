import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ".."
import "../components"

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
            width: tramAll.width
            anchors.bottom: parent.bottom
            ColumnLayout{
                id: fieldMenuColumn
                anchors.fill: parent
                IconButtonColor{
                    id: tramAll
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
                    text: "Rec Path"
                    checkable: true
                    isChecked: settings.setFeature_isRecPathOn
                    onCheckedChanged: settings.setFeature_isRecPathOn = checked
                }
            }
        }
        TitleFrame{
            id: toolsMenuText
            width: tramAll.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: fieldMenuText.right
            anchors.margins: 30
            title: "Tools Menu"
            font.bold: true
            ColumnLayout{
                id: toolsMenuColumn
                width: children.width
                height: parent.height
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
                    text: qsTr("Offset Fix")
                    isChecked: settings.setFeature_isOffsetFixOn
                    onCheckedChanged: settings.setFeature_isOffsetFixOn = checked
                }
            }
        }

        TitleFrame{
            id: screenButtons
            anchors.top: parent.top
            anchors.left: toolsMenuText.right
            anchors.margins: 30
            width: screenButtonsRow.width *2  + screenButtonsRow.spacing
            height: screenButtonsRow.height
            title: "Screen Buttons"
            Row{
                id: screenButtonsRow
                spacing: 20
                width: children.width
                height: children.height
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.left: parent.left
                IconButtonColor{
                    id: uturn
                    icon.source: "/images/Images/z_TurnManual.png"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    text: qsTr("U-Turn")
                    isChecked: settings.setFeature_isYouTurnOn
                    onCheckedChanged: settings.setFeature_isYouTurnOn = checked
                }
                IconButtonColor{
                    id: lateral
                    icon.source: "/images/Images/z_LateralManual.png"
                    text: qsTr("Lateral")
                    Layout.alignment: Qt.AlignCenter
                    isChecked: settings.setFeature_isLateralOn
                    onCheckedChanged: settings.setFeature_isLateralOn = checked
                }
            }
        }
        TitleFrame{
            id: sound
            anchors.top: parent.top
            anchors.right: parent.right
            width: tramAll.width
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height *.3
            anchors.margins: 100
            title: "Sound"
            ColumnLayout{
                width: children.width
                height: parent.height

            IconButtonColor{
                id: autoSteerSound
                text: qsTr("Auto Steer")
                icon.source: "/images/Config/ConF_SteerSound.png"
                isChecked: settings.setSound_isAutoSteerOn
                onCheckedChanged: settings.setSound_isAutoSteerOn = checked
                    Layout.alignment: Qt.AlignCenter
            }
            IconButtonColor{
                id: youTurnSound
                text: qsTr("You Turn")
                icon.source: "/images/Config/ConF_SteerSound.png"
                isChecked: settings.setSound_isUturnOn
                onCheckedChanged: settings.setSound_isUturnOn = checked
                    Layout.alignment: Qt.AlignCenter
            }

            IconButtonColor{
                id: hydLiftSound
                text: qsTr("Hyd Lift")
                icon.source: "/images/Config/ConF_SteerSound.png"
                isChecked: settings.setSound_isHydLiftOn
                onCheckedChanged: settings.setSound_isHydLiftOn = checked
                    Layout.alignment: Qt.AlignCenter
            }
            /*IconButtonColor{
                id: boundaryApproachSound
                    Layout.alignment: Qt.AlignCenter
                visible: false // not implemented
                text: qsTr("Boundary Approach")
                icon.source: "/images/Config/ConF_SteerSound.png"
                isChecked: .setSound_isAutoSteerOn
                onCheckedChanged: settings.setSound_isAutoSteerOn = checked
            }*/
            }
        }
    }
}
