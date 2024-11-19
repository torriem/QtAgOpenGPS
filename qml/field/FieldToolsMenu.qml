// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Menu displayed when the "Field Tools" button is clicked
import QtQuick
import QtQuick.Controls.Fusion

import ".."
import "../components"

Drawer {
    id: fieldToolsMenu

    width: 300
    visible: false
    height: mainWindow.height
    modal: true

    contentItem: Rectangle {
        id: fieldToolsMenuRect
        anchors.bottom: parent.bottom
        height: parent.height
        anchors.top: parent.top
        anchors.left: parent.left

        color: "black"

        ScrollViewExpandableColumn {
            anchors.fill: fieldToolsMenuRect
            id: fieldToolsMenuColumn
            height: fieldToolsMenuRect.height
            spacing: 0
            IconButtonTextBeside{
                text: "Boundary"
                icon.source: prefix + "/images/MakeBoundary.png"
                width: 300
                visible: settings.setFeature_isBoundaryOn
                onClicked: {
                    fieldToolsMenu.visible = false
                    boundaryMenu.show()
                }
            }
            IconButtonTextBeside{
                text: "Headland"
                icon.source: prefix + "/images/HeadlandMenu.png"
                width: 300
                visible: settings.setFeature_isHeadlandOn
                onClicked: {
                    fieldToolsMenu.visible = false
                    headlandDesigner.show()
                }
            }
            IconButtonTextBeside{
                text: "Headland (Build)"
                icon.source: prefix + "/images/Headache.png"
                visible: settings.setFeature_isHeadlandOn
                width: 300
                onClicked: {
                    fieldToolsMenu.visible = false
                    headacheDesigner.show()
                }
            }
            IconButtonTextBeside{
                text: "Tram Lines"
                icon.source: prefix + "/images/TramLines.png"
                width: 300
                visible: settings.setFeature_isTramOn
                onClicked: tramLinesEditor.visible = true
            }
            IconButtonTextBeside{
                text: "Recorded Path"
                icon.source: prefix + "/images/RecPath.png"
                width: 300
                visible: settings.setFeature_isHeadlandOn
                onClicked:{
                    fieldToolsMenu.visible = false
                    recPath.show()
                }
            }
        }
    }
}
