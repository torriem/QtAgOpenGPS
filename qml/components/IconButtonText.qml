// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Like IconButton, But has text
import QtQuick


IconButton {
    id: icon_button_text

    implicitWidth: 100  * theme.scaleWidth
    implicitHeight: 100 * theme.scaleHeight

    property double iconHeightScale: 0.75

    property string buttonText: ""

    onButtonTextChanged: {
        text = buttonText
    }
}
