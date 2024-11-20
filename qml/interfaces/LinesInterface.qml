// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Interface between formgps_ui_tracks and frontend tracks stuff. 
// Should be renamed to TracksInterface maybe?
import QtQuick

Item {
    id: linesInterface

    property var abLinesList: [
        {index: 0, name: "one", easting: 3, northing: 4, heading: 75, visible: true },
        {index: 1, name: "two", easting: 3, northing: 4, heading: 75, visible: true },
        {index: 2, name: "three", easting: 3, northing: 4, heading: 75, visible: true },
        {index: 3, name: "four", easting: 3, northing: 4, heading: 75, visible: true }
    ]

    property var abCurvesList: [
        {index: 0, name: "one", visible: true },
        {index: 1, name: "two", visible: true },
        {index: 2, name: "three", visible: true }
    ]

    signal abLine_updateLines()

    signal abLine_addLine(string name, double easting, double northing, double heading)
    signal abLine_deleteLine(int index)
    //signal changeABLineName(int lineno, string new_name)
    signal abLine_changeName(int which_one, string new_name)
    signal abLine_setA(bool start_or_cancel, double easting, double northing, double heading) //true to mark point, false to cancel new point
    signal abLine_swapHeading(int index)



}
