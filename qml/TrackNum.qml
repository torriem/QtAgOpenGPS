// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
//
import QtQuick 2.0
import "components" as Comp

Comp.OutlineText {
    id: tracknum
    font.pixelSize: 24

    property int currentTrack: 0
    property bool useDirNames: false
    property double trackHeading: 0

    function generate_text(track_no) {
        var track_num = track_no

        if (utils.isTrue(settings.setDisplay_useTrackZero)) {
            if (track_num >  0)
                track_num -=1
        }

        if(track_num === 0)
            return qsTr("Track", "Track as in \"track number.\"") + " " + Number(0).toLocaleString(Qt.locale(),'f',0)

        var dir = ""
        if (useDirNames === true) {
            dir = utils.findTrackDirection(trackHeading, track_num)
        } else {
            if (track_num > 0)
                dir = qsTr("R", "Abbreviation for right-hand direction.")
            else
                dir = qsTr("L", "Abbreviation for left-hand direction.")
        }

        return qsTr("Track", "Track as in \"track number.\"") + " " +
                Number(Math.abs(currentTrack)).toLocaleString(Qt.locale(),'f',0) + " " +
                dir
    }

    //TODO: check settings to see if we should have a track 0

    text: generate_text(currentTrack)

    color: "white"
    visible: (aog.currentABLine > -1)
}
