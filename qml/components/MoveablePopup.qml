// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Popup that can be moved around 
import QtQuick
import QtQuick.Controls.Fusion

Popup {
    id: rootPopup
    padding: 0
    property alias topBar: topBar
    MouseArea {
            anchors.fill: topBar
            property point lastMousePos: Qt.point(0, 0)
            onPressed: { lastMousePos = Qt.point(mouseX, mouseY); }
            onMouseXChanged: rootPopup.x += (mouseX - lastMousePos.x)
            onMouseYChanged: rootPopup.y += (mouseY - lastMousePos.y)
        }
    Rectangle{
        id: topBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 50
    }

}
