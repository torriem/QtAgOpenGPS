// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Displayed by ConfigImplementSections if we are using zones, not sections
import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

import ".."
import "../components"

/*todo: sections not populated from .config yet
  also not at all sure about switching these to SpinBoxCM... Don't want to break anything
  */
Item {
    id: configImplementSectionsZones


    ListModel {
        id: section_model
    }

    onVisibleChanged: {
        if (visible)
            loadZonesFromSettings()
    }

    function loadZonesFromSettings()
    {
        var num_zones = Number(settings.setTool_zones[0])

        var i=0
        var max_width=0
        var w = 0

        //find the maximum width of the zones so we know how
        //much room we have for making changes in the spinboxes
        var current_section = 1
        for (i=0; i < num_zones; i++) {
            w = Number(settings.setTool_zones[i+1]) - current_section
            if (w > max_width) max_width = w
            current_section += w + 1
        }

        section_model.clear()
        current_section = 1
        for( i=0; i < num_zones ; i++ ) {
            if (i == num_zones-1) {
                section_model.append( {secNum: i,
                                         start_number: current_section,
                                         min_number: Number(settings.setTool_zones[i+1]),
                                         max_number: Number(settings.setTool_zones[i+1]),
                                         end_number: Number(settings.setTool_zones[i+1])
                                     } )
            } else {
                section_model.append( { secNum: i,
                                         start_number: current_section,
                                         min_number: current_section,
                                         end_number: Number(settings.setTool_zones[i+1]),
                                         max_number: Number(settings.setTool_numSectionsMulti) -
                                                     Number(settings.setTool_zones[0]) + i + 1
                                     } )
            }

            current_section = Number(settings.setTool_zones[i+1]) + 1
        }
    }

    function modifyZone(zone, new_value) {
        //modify the zone, and bump all the values going down the list
        var i = 0
        var current_section = 1
        var item
        var num_zones = Number(settings.setTool_zones[0])
        var max_width = 0
        var w = 0
        var widths = []


        for( i=0; i < num_zones ; i++ ) {
            item = section_model.get(i)
            if(i === zone)
                item.end_number = new_value

            if (item.start_number < current_section)
                item.start_number = current_section

            if (item.start_number > item.end_number)
                item.end_number = item.start_number

            if (item.start_number > current_section)
                item.start_number = current_section

            //let's fix these up later
            item.min_number = 1
            item.max_number = 64

            current_section = item.end_number + 1
        }

        fixZoneMinMax()
        saveZonesToSettings()
    }

    function fixZoneMinMax() {
        var item
        var num_zones = Number(settings.setTool_zones[0])
        var current_section = 1
        for (var i = 0; i < num_zones; i++) {
            item = section_model.get(i)
            if (i == num_zones-1) {
                item.min_number = item.end_number
                item.max_number = item.end_number
            } else {
                item.min_number = current_section
                item.max_number = Number(settings.setTool_numSectionsMulti) -
                                  Number(settings.setTool_zones[0]) + i + 1
            }
            current_section = item.end_number + 1
        }
    }

    function sectionsChanged() {
        //if we've added sections, add it to the last section
        //if we've removed sections, adjust each zone

        var item
        var i = 0

        var current_section = Number(settings.setTool_numSectionsMulti)
        for (i = section_model.count-1; i >= 0; i--) {
            item = section_model.get(i)

            //easy case.  We've added sections, so stick them on
            //the end
            if (item.end_number < current_section) {
                item.end_number = current_section
                //that's all we need to do. Just go on to calculate min and max
                break
            }

            //we've removed setions.  Start shrinking zones from last to first
            //until they all fit

            item.min_number = 1
            item.max_number = 64

            if (item.end_number > current_section)
                item.end_number = current_section
            if (item.start_number > current_section)
                item.start_number = current_section
            else
                //we took everything out that we need to
                break
            current_section = item.end_number -1
        }

        //now adjust the max and min numbers
        fixZoneMinMax()
    }

    function adjustZones(num_zones) {
        //deal with adding or removing zones
        var zone = settings.setTool_zones
        zone[0] = num_zones
        settings.setTool_zones = zone

        if(num_zones < section_model.count) {
            //easy case. Add last zone's sections to previous zone
            while(num_zones < section_model.count)
                section_model.remove(section_model.count - 1)
            section_model.get(section_model.count - 1).end_number = Number(settings.setTool_numSectionsMulti)
            fixZoneMinMax()
        } else {
            //hard case, adding zones!  Have to take away sections from preceding zones
            //similar case to removing sections
            while(section_model.count < num_zones) {
                section_model.append( { secNum: section_model.count,
                                         min_number: 1,
                                         max_number: 64,
                                         start_number: 64,
                                         end_number: 64} )
            }
            sectionsChanged()
        }
    }

    function layoutZones() {
        var num_zones = Number(settings.setTool_zones[0])
        var num_sections = Number(settings.setTool_numSectionsMulti)
        var sectionsPerZone = num_sections / num_zones
        var current_section = 1.0
        section_model.clear()

        for (var i=0; i < num_zones; i++) {
            if (i == num_zones - 1) {
                section_model.append( { secNum: i,
                                         start_number: Math.floor(current_section),
                                         end_number: num_sections,
                                         min_number: num_sections,
                                         max_number: num_sections } )
            } else {
                section_model.append( { secNum: i,
                                         start_number: Math.floor(current_section),
                                         end_number: Math.floor(current_section + sectionsPerZone - 1),
                                         min_number: 1,
                                         max_number: 64 } )
            }
            current_section += sectionsPerZone
        }
        fixZoneMinMax()
        saveZonesToSettings()
        /*
        var item
        for (i=0; i < num_zones; i++) {
            item = section_model.get(i)
            console.debug (i, item.start_number, item.end_number, item.min_number, item.max_number)
        }
        */
    }

    function saveZonesToSettings() {
        var item
        var zones = settings.setTool_zones
        //number of zones should be current.

        for( var i=0; i < section_model.count; i++) {
            item = section_model.get(i)
            zones[i+1] = item.end_number
        }
        //console.debug(zones)
        settings.setTool_zones = zones
    }

    GridView {
        id: section_width_grid
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: middleRow.top
		anchors.topMargin: 7 * theme.scaleHeight
		anchors.bottomMargin: 7 * theme.scaleHeight
		anchors.leftMargin: 7 * theme.scaleWidth
		anchors.rightMargin: 7 * theme.scaleWidth
        model: section_model
        cellWidth: 200 * theme.scaleWidth
		cellHeight: 85 * theme.scaleHeight


        delegate: Rectangle {
            implicitWidth: childrenRect.width
            implicitHeight: childrenRect.height

            SpinBoxCustomized {
                anchors.left: zntext.right
                anchors.top: parent.top

                implicitWidth: 150 * theme.scaleWidth
                id: spinner
                from: model.min_number
                to: model.max_number
                text: qsTr("Zone","Zone as in zone of sections") + " " + Number(model.secNum+1).toLocaleString(Qt.locale(),"f",0)
                editable: model.min_number < model.max_number ? true : false
                value: model.end_number
                onValueModified: {
                    modifyZone(model.secNum, value)
                }
            }
            Text{
                id: zntext
                text: Number(model.start_number).toLocaleString(Qt.locale(),"f",0) +
                      " " + qsTr("\u21a6","In LTR languages, right arrow") + " "
                anchors.left: parent.left
                anchors.verticalCenter: spinner.verticalCenter
                font.bold: true
                font.pixelSize: 15
            }
        }
    }

    RowLayout{
        id: middleRow
        anchors.top: parent.verticalCenter
        anchors.right: parent.right
        anchors.left: parent.left
		anchors.rightMargin: 15 * theme.scaleWidth
		anchors.leftMargin: 15 * theme.scaleWidth
		anchors.topMargin: 15 * theme.scaleHeight
		anchors.bottomMargin: 15 * theme.scaleHeight
        //width: children.width
        height: children.height

        SpinBoxCustomized{
            Layout.fillWidth: true
            id: numOfZones
            from: 1
            boundValue: settings.setTool_zones[0]
            to: settings.setTool_numSectionsMulti > 8 ? 8 : settings.setTool_numSectionsMulti
            text: qsTr("Zones", "Zones as in zones of sections")
            editable: true
            onValueModified: {
                //adjustZones(value)
                var zone = settings.setTool_zones
                zone[0] = value
                settings.setTool_zones = zone
                layoutZones()
            }
        }

        /*
        Button {
            id: spaceOutButton
            text: qsTr("Auto\nLayout","automatic zone layout")

            onClicked: {
                layoutZones()

            }
        }
        */

        SpinBoxCM{
            Layout.fillWidth: true
            id: sectionWidthAllZones
            implicitWidth: 150 * theme.scaleWidth
            implicitHeight: 50 * theme.scaleHeight
            from: 10
            boundValue: settings.setTool_sectionWidthMulti
            to: 1000
            text: qsTr("Uniform Section Width")
            onValueModified: settings.setTool_sectionWidthMulti = value
        }

        SpinBoxCustomized{
            Layout.fillWidth: true
            id: sectionsNumber
            implicitWidth: 150 * theme.scaleWidth
            implicitHeight: 50 * theme.scaleHeight
            from: numOfZones.value
            boundValue: settings.setTool_numSectionsMulti
            to: 64
            text: qsTr("Sections")
            editable: true
            onValueModified: {
                settings.setTool_numSectionsMulti = value
                //sectionsChanged() //adjust the model and save
                layoutZones()
            }

        }
        Column{
            Layout.fillWidth: true
            spacing: 8 * theme.scaleHeight
            Text {
                text: utils.cm_to_unit_string(Number(settings.setTool_numSectionsMulti) * Number(settings.setTool_sectionWidthMulti),0)
            }
            Text {
                text: utils.cm_unit()
                color: "green"
            }
        }
    }
}
