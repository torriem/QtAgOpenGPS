// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Column that expands when stretched out, and scrolls when pushed in
import QtQuick
import QtQuick.Controls.Fusion

ScrollView{
    id: rootScrollView
    default property alias content: expandableColumn.children
    onHeightChanged: expandableColumn.adjust(height)
    onVisibleChanged: expandableColumn.adjust(height)
    clip: true
    width: expandableColumn.width
    ExpandableColumn{
        id: expandableColumn
        width: children.width
    }
}
