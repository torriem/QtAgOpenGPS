// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


IconButton {
    id: icon_button_text

    property double iconHeightScale: 0.75

    property string buttonText: ""

    onButtonTextChanged: {
        text = buttonText
    }
    Layout.alignment: Qt.AlignCenter
    implicitWidth: theme.buttonSize
    implicitHeight: theme.buttonSize
}
