// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Buttons that are shown across the top of "SteerConfigSettings.qml"
import QtQuick
import QtQuick.Layouts
import "../components"

IconButtonTextBeside{
    implicitWidth: parent.width / 4 - 4
    Layout.alignment: Qt.AlignCenter
    checkable: true
    checked: false
    colorChecked: "lightgray"
}
