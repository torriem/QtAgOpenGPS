// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Displays the GPS Data on main screen.
import QtQuick
import QtQuick.Controls.Fusion
import "components" as Comp

Rectangle{
    id: gpsData
    width: 200
    height: childrenRect.height + 30
    color: "#4d4d4d"
    Column{
        id: column
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: 15
        Comp.TextLine{ color: "white"; text: qsTr("Lat ", "abbreviation for latitude")+(Number(aog.latitude).toLocaleString(Qt.locale(), 'f', 9))}
        Comp.TextLine{ color: "white"; text: qsTr("Lon ", "abbreviation for longitude")+(Number(aog.longitude).toLocaleString(Qt.locale(), 'f', 9))}
        Comp.TextLine{ color: "white"; text: qsTr("N ", "abbreviation for northing") + (Number(aog.northing).toLocaleString(Qt.locale(), 'f', 3))}
        Comp.TextLine{ color: "white"; text: qsTr("E ", "abbreviation for easting")+ (Number(aog.easting).toLocaleString(Qt.locale(), 'f', 3))}
        Comp.TextLine{ color: "white"; text: qsTr("Elev ", "abbreviation for elevation")+ (Number(aog.altitude).toLocaleString(Qt.locale(), 'f', 2))}
        Comp.TextLine{ color: "white"; text: qsTr("# Sats ", "abbreviation for # Satellites")+ aog.satellitesTracked}
        Comp.TextLine{ color: "white"; text: qsTr("HDOP ", "acronym for Horizontal Degree Of Position") + (Number(aog.hdop).toLocaleString(Qt.locale(), 'f', 3))}
        Comp.TextLine{ color: "white"; text: qsTr("Frame ")+ (Number(aog.frameTime).toLocaleString(Qt.locale(), 'f', 1))}
        Comp.TextLine{ color: "white"; text: qsTr("Raw Hz ", "abbreviation for Raw Hertz")+ (Number(aog.rawHz).toLocaleString(Qt.locale(), 'f', 1))}
        Comp.TextLine{ color: "white"; text: qsTr("Hz ", "abbreviation for Hertz")+ (Number(aog.hz).toLocaleString(Qt.locale(), 'f', 1))}
        Comp.TextLine{ color: "white"; text: qsTr("Dropped ")+ aog.droppedSentences}
        Comp.TextLine{ color: "white"; text: qsTr("Fix2Fix ")+ (Number(aog.heading * 180 / Math.PI).toLocaleString(Qt.locale(), 'f', 1))}//convert from radians
        Comp.TextLine{ color: "white"; text: qsTr("IMU ")+ aog.dispImuHeading;} //rounding taken care of in AOGInterface.qml
        Comp.TextLine{ color: "white"; text: qsTr("Heading ")+ (Number(aog.fusedHeading * 180 / Math.PI).toLocaleString(Qt.locale(), 'f', 1))}
        Item{
            visible: false //should reference a setting if rtk is turned on
            Comp.TextLine{ color: "white"; text: "<br>"}
            Comp.TextLine{ color: "white"; text: qsTr("RTK Age", "abbreviation for Real Time Kinematic Age")+ aog.rtkAge}
            Comp.TextLine{ color: "white"; text: qsTr("Fix: ")+aog.gpsFixQuality}
        }
    }
}
