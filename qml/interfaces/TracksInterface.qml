// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Interface between formgps_ui_tracks and frontend tracks stuff. 
// Should be renamed to TracksInterface maybe?
import QtQuick

Item {
    id: tracksInterface

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

    //Assume trk.model is always updated, although I guess we could call for an update?
    //to select a track, set trk.idx to the index. -1 means tracks are off.

    //modes:
    //AB=2, Curve = 4, waterPivot = 64
    signal tracks_setA(int mode, bool start_or_cancel, double easting, double northing, double heading)
    signal tracks_addLine(string name, double easting, double northing, double heading)
    signal tracks_pauseOrResumeLine(bool pause)

    signal tracks_changeName(int index, string new_name)
    signal tracks_swapHeading(int index)

    signal tracks_updateModel()


}
