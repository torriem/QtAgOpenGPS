// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Headland, trams, etc
import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

import ".."
import "../components"

Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent
        color: aog.backgroundColor
        TitleFrame{
            id: fieldMenuText
            anchors.top: parent.top
            anchors.left: parent.left
			anchors.topMargin: 30 * theme.scaleHeight
			anchors.rightMargin: 30 * theme.scaleWidth
			anchors.leftMargin: 30 * theme.scaleWidth
			anchors.bottomMargin: 30 * theme.scaleHeight
            title: "Field Menu"
            font.bold: true
            width: tramAll.width
            anchors.bottom: parent.bottom
            ColumnLayout{
                id: fieldMenuColumn
                anchors.fill: parent
                IconButtonColor{
                    id: tramAll
                    icon.source: prefix + "/images/TramAll.png"
                    text: "Tram Lines"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    isChecked: settings.setFeature_isTramOn
                    onCheckedChanged: settings.setFeature_isTramOn = checked
                }
                IconButtonColor{
                    icon.source: prefix + "/images/HeadlandOn.png"
                    text: "Headland"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    isChecked: settings.setFeature_isHeadlandOn
                    onCheckedChanged: settings.setFeature_isHeadlandOn = checked
                }
                IconButtonColor{
                    icon.source: prefix + "/images/BoundaryOuter.png"
                    text: "Boundary"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    isChecked: settings.setFeature_isBoundaryOn
                    onCheckedChanged: settings.setFeature_isBoundaryOn = checked
                }
                IconButtonColor{
                    icon.source: prefix + "/images/RecPath.png"
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
			anchors.topMargin: 30 * theme.scaleHeight
			anchors.rightMargin: 30 * theme.scaleWidth
			anchors.leftMargin: 30 * theme.scaleWidth
			anchors.bottomMargin: 30 * theme.scaleHeight
            title: "Tools Menu"
            font.bold: true
            ColumnLayout{
                id: toolsMenuColumn
                width: children.width
                height: parent.height
                IconButtonColor{
                    icon.source: prefix + "/images/ABSmooth.png"
                    text: "AB Smooth"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    isChecked: settings.setFeature_isABSmoothOn
                    onCheckedChanged: settings.setFeature_isABSmoothOn = checked
                }
                IconButtonColor{
                    icon.source: prefix + "/images/HideContour.png"
                    text: "Hide Contour"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    isChecked: settings.setFeature_isHideContourOn
                    onCheckedChanged: settings.setFeature_isHideContourOn = checked
                }
                IconButtonColor{
                    icon.source: prefix + "/images/Webcam.png"
                    text: "WebCam"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    isChecked: settings.setFeature_isWebCamOn
                    onCheckedChanged: settings.setFeature_isWebCamOn = checked
                }
                IconButtonColor{
                    icon.source: prefix + "/images/YouTurnReverse.png"
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
			anchors.topMargin: 30 * theme.scaleHeight
			anchors.rightMargin: 30 * theme.scaleWidth
			anchors.leftMargin: 30 * theme.scaleWidth
			anchors.bottomMargin: 30 * theme.scaleHeight
            width: screenButtonsRow.width *2  + screenButtonsRow.spacing
            height: screenButtonsRow.height
            title: "Screen Buttons"
            Row{
                id: screenButtonsRow
                spacing: 20 * theme.scaleWidth
                width: children.width
                height: children.height
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.left: parent.left
                IconButtonColor{
                    id: uturn
                    icon.source: prefix + "/images/Images/z_TurnManual.png"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    text: qsTr("U-Turn")
                    isChecked: settings.setFeature_isYouTurnOn
                    onCheckedChanged: settings.setFeature_isYouTurnOn = checked
                }
                IconButtonColor{
                    id: lateral
                    icon.source: prefix + "/images/Images/z_LateralManual.png"
                    text: qsTr("Lateral")
                    checkable: true
                    Layout.alignment: Qt.AlignCenter
                    isChecked: settings.setFeature_isLateralOn
                    onCheckedChanged: settings.setFeature_isLateralOn = checked
                }
            }
        }
        ColumnLayout{//Bottom, Right menu, Autostart AgIO
            anchors.top: screenButtons.bottom
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: screenButtons.horizontalCenter
            anchors.topMargin: 20 * theme.scaleHeight
            anchors.bottomMargin: 20 * theme.scaleHeight
            IconButtonColor{
                checkable: true
                text: qsTr("Auto Start AgIO")
                icon.source: "/images/AgIO.png"
                isChecked: settings.setFeature_isAgIOOn
                onCheckedChanged: settings.setFeature_isAgIOOn = checked
            }
        }

        TitleFrame{
            id: sound
            anchors.top: parent.top
            anchors.right: parent.right
            width: tramAll.width
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height *.3
			anchors.topMargin: 100 * theme.scaleHeight
			anchors.rightMargin: 30 * theme.scaleWidth
			anchors.leftMargin: 30 * theme.scaleWidth
            title: "Sound"
            ColumnLayout{
                width: children.width
                height: parent.height

            IconButtonColor{
                id: autoSteerSound
                text: qsTr("Auto Steer")
                icon.source: prefix + "/images/Config/ConF_SteerSound.png"
                checkable: true
                isChecked: settings.setSound_isAutoSteerOn
                onCheckedChanged: settings.setSound_isAutoSteerOn = checked
                    Layout.alignment: Qt.AlignCenter
            }
            IconButtonColor{
                id: youTurnSound
                text: qsTr("You Turn")
                icon.source: prefix + "/images/Config/ConF_SteerSound.png"
                isChecked: settings.setSound_isUturnOn
                onCheckedChanged: settings.setSound_isUturnOn = checked
                    Layout.alignment: Qt.AlignCenter
            }

            IconButtonColor{
                id: hydLiftSound
                text: qsTr("Hyd Lift")
                icon.source: prefix + "/images/Config/ConF_SteerSound.png"
                isChecked: settings.setSound_isHydLiftOn
                onCheckedChanged: settings.setSound_isHydLiftOn = checked
                    Layout.alignment: Qt.AlignCenter
            }
            /*IconButtonColor{
                id: boundaryApproachSound
                    Layout.alignment: Qt.AlignCenter
                visible: false // not implemented
                text: qsTr("Boundary Approach")
                icon.source: prefix + "/images/Config/ConF_SteerSound.png"
                isChecked: .setSound_isAutoSteerOn
                onCheckedChanged: settings.setSound_isAutoSteerOn = checked
            }*/
            }
        }
    }
}
