// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Transparent, instead of any color
import QtQuick


IconButton {
    id: icon_button_transparent
    color1: "#00000000"
    color2: "#00000000"
    color3: "#00000000"
    property double iconHeightScale: (text.length > 0) ? .75 : 1

    property string buttonText: ""

    onButtonTextChanged: {
         text = buttonText
     }
}
