// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Interface between backend and recorded path UI stuff--yet to be set up
import QtQuick

Item {
    id: recordedPathInterface

    property bool isDrivingRecordedPath: false

    property var recordedPathList: [
        { index: 0, name: "test1" },
        { index: 1, name: "test2" }
    ]

    signal updateLines()
    signal open_path(string name)
    signal delete_path(string name)
    signal start_driving()
    signal stop_driving()
    signal clear_path()

}
