// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
//Contains the customized slider for the steer config pages
import "../components"
import QtQuick
import QtQuick.Layouts

SliderCustomized{
    id: rootSCSlider
    Layout.alignment: Qt.AlignCenter
    width: 200 * theme.scaleWidth
    implicitHeight: 50 * theme.scaleHeight
}
