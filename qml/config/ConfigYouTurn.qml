// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Youturn dimensions settings
import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

import ".."
import "../components"
/* todo:
  not sure about uTurn Style. setting seems to be an int.
  these don't need the SpinBoxCM thing, because they're already metric.
  Should be made to convert to Imperial some day.
  */
Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent
        color: aog.backgroundColor
        GridLayout{
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            flow:Grid.LeftToRight
            rows: 3
            columns: 4
            height: parent.height /2

            Image {
                source: prefix + "/images/Config/ConU_UturnRadius.png"
                height: 150
                width: 150
            }
            Image {
                source: prefix + "/images/Config/ConU_UturnDistance.png"
                height: 150
                width: 150
            }
            Image {
                source: prefix + "/images/Config/ConU_UturnLength.png"
                height: 150
                width: 150
            }
            Image {
                source: prefix + "/images/Config/ConU_UturnSmooth.png"
                height: 150
                width: 150
            }
            SpinBoxM{
                objectName: "UturnRadius"
                from: 2
                boundValue: settings.set_youTurnRadius
                stepSize: settings.setMenu_isMetric ?  1 : 5
                onValueChanged: settings.set_youTurnRadius = value
                to: 100
                editable: true
				decimals: 1
            }
            SpinBoxM{
                objectName: "UturnDistance"
                from: 0
                boundValue: settings.set_youTurnDistanceFromBoundary
                onValueChanged: settings.set_youTurnDistanceFromBoundary = value
                to: 100
                editable: true
				decimals: 1
            }
            SpinBoxM{
                objectName: "UturnExtensionLength"
                from: 3
                boundValue: settings.set_youTurnExtensionLength
                onValueChanged: settings.set_youTurnExtensionLength = value
                to:50
                editable: true
            }
            SpinBoxM{
                objectName: "UturnSmoothing"
                from: 8
                boundValue: settings.setAS_uTurnSmoothing
                onValueChanged: settings.setAS_uTurnSmoothing = value
                to:50
                editable: true
            }
            Text{
                text: utils.m_unit()
                font.bold: true
            }
            Text{
                text: utils.m_unit()
                font.bold: true
            }
            Text{
                text: utils.m_unit() + " \n" + qsTr("Set Extension
Length to 2 or
3x Radius")
                font.bold: true
            }
            Text{
                text: utils.m_unit() + " \n" + qsTr("Set Smoothing
 to 3 or 4x
Radius")
                font.bold: true
            }
        }
        TitleFrame{
            id: uturnStyle
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 40
            width: 150
            height: 150
            Text{
                id: styleText
                text: qsTr("UTurn Style")
                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Row{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: styleText.bottom
                width:childrenRect.width
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.topMargin: 5
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                spacing: 20
                ButtonGroup {
                    buttons: [uTurnUBtn, uTurnHBtn]
                }

                IconButtonColor{
                    id: uTurnUBtn
                    icon.source: prefix + "/images/YouTurnU.png"
                    checkable: true
                    isChecked: settings.set_uTurnStyle === 0
                    onClicked: settings.set_uTurnStyle = 1
                }
                IconButtonColor{
                    id: uTurnHBtn
                    checkable: true
                    icon.source: prefix + "/images/YouTurnH.png"
                    isChecked: settings.set_uTurnStyle === 1
                    onClicked: settings.set_uTurnStyle = 1
                }
            }
        }
    }
}
