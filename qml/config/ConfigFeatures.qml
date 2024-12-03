// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Headland, trams, etc
import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

import ".."
import "../components"
import "components"

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
                DisplayAndFeaturesBtns{
                    id: tramAll
                    icon.source: prefix + "/images/TramAll.png"
                    text: "Tram Lines"
                    isChecked: settings.setFeature_isTramOn
                    onCheckedChanged: settings.setFeature_isTramOn = checked
                }
                DisplayAndFeaturesBtns{
                    icon.source: prefix + "/images/HeadlandOn.png"
                    text: "Headland"
                    isChecked: settings.setFeature_isHeadlandOn
                    onCheckedChanged: settings.setFeature_isHeadlandOn = checked
                }
                DisplayAndFeaturesBtns{
                    icon.source: prefix + "/images/BoundaryOuter.png"
                    text: "Boundary"
                    isChecked: settings.setFeature_isBoundaryOn
                    onCheckedChanged: settings.setFeature_isBoundaryOn = checked
                }
                DisplayAndFeaturesBtns{
                    icon.source: prefix + "/images/RecPath.png"
                    text: "Rec Path"
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
                DisplayAndFeaturesBtns{
                    icon.source: prefix + "/images/ABSmooth.png"
                    text: "AB Smooth"
                    isChecked: settings.setFeature_isABSmoothOn
                    onCheckedChanged: settings.setFeature_isABSmoothOn = checked
                }
                DisplayAndFeaturesBtns{
                    icon.source: prefix + "/images/HideContour.png"
                    text: "Hide Contour"
                    isChecked: settings.setFeature_isHideContourOn
                    onCheckedChanged: settings.setFeature_isHideContourOn = checked
                }
                DisplayAndFeaturesBtns{
                    icon.source: prefix + "/images/Webcam.png"
                    text: "WebCam"
                    isChecked: settings.setFeature_isWebCamOn
                    onCheckedChanged: settings.setFeature_isWebCamOn = checked
                }
                DisplayAndFeaturesBtns{
                    icon.source: prefix + "/images/YouTurnReverse.png"
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
            RowLayout{
                id: screenButtonsRow
                spacing: 20 * theme.scaleWidth
                width: children.width
                height: children.height
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.left: parent.left
                DisplayAndFeaturesBtns{
                    id: uturn
                    icon.source: prefix + "/images/Images/z_TurnManual.png"
                    text: qsTr("U-Turn")
                    isChecked: settings.setFeature_isYouTurnOn
                    onCheckedChanged: settings.setFeature_isYouTurnOn = checked
                }
                DisplayAndFeaturesBtns{
                    id: lateral
                    icon.source: prefix + "/images/Images/z_LateralManual.png"
                    text: qsTr("Lateral")
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
            DisplayAndFeaturesBtns{
                text: qsTr("Auto Start AgIO")
                icon.source: prefix	+ "/images/AgIO.png"
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

                DisplayAndFeaturesBtns{
                    id: autoSteerSound
                    text: qsTr("Auto Steer")
                    icon.source: prefix + "/images/Config/ConF_SteerSound.png"
                    isChecked: settings.setSound_isAutoSteerOn
                    onCheckedChanged: settings.setSound_isAutoSteerOn = checked
                }
                DisplayAndFeaturesBtns{
                    id: youTurnSound
                    text: qsTr("You Turn")
                    icon.source: prefix + "/images/Config/ConF_SteerSound.png"
                    isChecked: settings.setSound_isUturnOn
                    onCheckedChanged: settings.setSound_isUturnOn = checked
                }

                DisplayAndFeaturesBtns{
                    id: hydLiftSound
                    text: qsTr("Hyd Lift")
                    icon.source: prefix + "/images/Config/ConF_SteerSound.png"
                    isChecked: settings.setSound_isHydLiftOn
                    onCheckedChanged: settings.setSound_isHydLiftOn = checked
                }
                /*DisplayAndFeaturesBtns{
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
