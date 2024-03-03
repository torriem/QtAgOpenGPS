import QtQuick 2.15
import QtQuick.Controls 2.15

import ".."

/*todo: sections not populated from .config yet
  also not at all sure about switching these to SpinBoxCM... Don't want to break anything
  */
Item{
    id: configImplementSectionsSection
    z: 8
    ListModel {
        id: section_model

    }

    onVisibleChanged: {
        if (visible) loadModelFromSettings()
    }

    property int totalWidth: 0

    function loadModelFromSettings() {
        if(!utils.isTrue(settings.setTool_isSectionsNotZones))
            return

        section_model.clear()

        totalWidth = 0

        var w = Number(settings.setSection_position2) - Number(settings.setSection_position1)
        section_model.append({secNum: 0, width: w })
        totalWidth += w
        if(Number(settings.setVehicle_numSections < 2)) return

        w = Number(settings.setSection_position3) - Number(settings.setSection_position2)
        section_model.append({secNum: 1, width: w })
        totalWidth += w
        if(Number(settings.setVehicle_numSections < 3)) return

        w = Number(settings.setSection_position4) - Number(settings.setSection_position3)
        section_model.append({secNum: 2, width: w })
        totalWidth += w
        if(Number(settings.setVehicle_numSections < 4)) return

        w = Number(settings.setSection_position5) - Number(settings.setSection_position4)
        section_model.append({secNum: 3, width: w })
        totalWidth += w
        if(Number(settings.setVehicle_numSections < 5)) return

        w = Number(settings.setSection_position6) - Number(settings.setSection_position5)
        section_model.append({secNum: 4, width: w })
        totalWidth += w
        if(Number(settings.setVehicle_numSections < 6)) return

        w = Number(settings.setSection_position7) - Number(settings.setSection_position6)
        section_model.append({secNum: 5, width: w })
        totalWidth += w
        if(Number(settings.setVehicle_numSections < 7)) return

        w = Number(settings.setSection_position8) - Number(settings.setSection_position7)
        section_model.append({secNum: 6, width: w })
        totalWidth += w
        if(Number(settings.setVehicle_numSections < 8)) return

        w = Number(settings.setSection_position9) - Number(settings.setSection_position8)
        section_model.append({secNum: 7, width: w })
        totalWidth += w
        if(Number(settings.setVehicle_numSections < 9)) return

        w = Number(settings.setSection_position10) - Number(settings.setSection_position9)
        section_model.append({secNum: 8, width: w })
        totalWidth += w
        if(Number(settings.setVehicle_numSections < 10)) return

        w = Number(settings.setSection_position11) - Number(settings.setSection_position10)
        section_model.append({secNum: 9, width: w })
        totalWidth += w
        if(Number(settings.setVehicle_numSections < 11)) return

        w = Number(settings.setSection_position12) - Number(settings.setSection_position11)
        section_model.append({secNum: 10, width: w })
        totalWidth += w
        if(Number(settings.setVehicle_numSections < 12)) return

        w = Number(settings.setSection_position13) - Number(settings.setSection_position12)
        section_model.append({secNum: 11, width: w })
        totalWidth += w
        if(Number(settings.setVehicle_numSections < 13)) return

        w = Number(settings.setSection_position14) - Number(settings.setSection_position13)
        section_model.append({secNum: 12, width: w })
        totalWidth += w
        if(Number(settings.setVehicle_numSections < 14)) return

        w = Number(settings.setSection_position15) - Number(settings.setSection_position14)
        section_model.append({secNum: 13, width: w })
        totalWidth += w
        if(Number(settings.setVehicle_numSections < 15)) return

        w = Number(settings.setSection_position16) - Number(settings.setSection_position15)
        section_model.append({secNum: 14, width: w })
        totalWidth += w
        if(Number(settings.setVehicle_numSections < 16)) return

        w = Number(settings.setSection_position17) - Number(settings.setSection_position16)
        section_model.append({secNum: 15, width: w })
        totalWidth += w
    }

    function saveModelToSettings() {
        //get total width
        var w = 0.0
        for (var i=0; i < Number(settings.setVehicle_numSections) ; i ++)
            w += section_model.get(i).width

        settings.setVehicle_toolWidth = w;
        totalWidth = w
        var pos = -w/2
        var n = Number(settings.setVehicle_numSections)

        settings.setSection_position1 = pos
        settings.setSection_position2 = pos + section_model.get(0).width
        pos += section_model.get(0).width
        if (n < 2) {
            settings.setSection_position3 = 0
        } else {
            settings.setSection_position3 = pos + section_model.get(1).width
            pos += section_model.get(1).width
        }

        if (n < 3) {
            settings.setSection_position4 = 0
        } else {
            settings.setSection_position4 = pos + section_model.get(2).width
            pos += section_model.get(2).width
        }

        if (n < 4) {
            settings.setSection_position5 = 0
        } else {
            settings.setSection_position5 = pos + section_model.get(3).width
            pos += section_model.get(3).width
        }

        if (n < 5) {
            settings.setSection_position6 = 0
        } else {
            settings.setSection_position6 = pos + section_model.get(4).width
            pos += section_model.get(4).width
        }

        if (n < 6) {
            settings.setSection_position7 = 0
        } else {
            settings.setSection_position7 = pos + section_model.get(5).width
            pos += section_model.get(5).width
        }

        if (n < 7) {
            settings.setSection_position8 = 0
        } else {
            settings.setSection_position8 = pos + section_model.get(6).width
            pos += section_model.get(6).width
        }

        if (n < 8) {
            settings.setSection_position9 = 0
        } else {
            settings.setSection_position9 = pos + section_model.get(7).width
            pos += section_model.get(7).width
        }

        if (n < 9) {
            settings.setSection_position10 = 0
        } else {
            settings.setSection_position10 = pos + section_model.get(8).width
            pos += section_model.get(8).width
        }

        if (n < 10) {
            settings.setSection_position11 = 0
        } else {
            settings.setSection_position11 = pos + section_model.get(9).width
            pos += section_model.get(9).width
        }

        if (n < 11) {
            settings.setSection_position12 = 0
        } else {
            settings.setSection_position12 = pos + section_model.get(10).width
            pos += section_model.get(10).width
        }

        if (n < 12) {
            settings.setSection_position13 = 0
        } else {
            settings.setSection_position13 = pos + section_model.get(11).width
            pos += section_model.get(11).width
        }

        if (n < 13) {
            settings.setSection_position14 = 0
        } else {
            settings.setSection_position14 = pos + section_model.get(12).width
            pos += section_model.get(12).width
        }

        if (n < 14) {
            settings.setSection_position15 = 0
        } else {
            settings.setSection_position15 = pos + section_model.get(13).width
            pos += section_model.get(13).width
        }

        if (n < 15) {
            settings.setSection_position16 = 0
        } else {
            settings.setSection_position16 = pos + section_model.get(14).width
            pos += section_model.get(14).width
        }

        if (n < 16) {
            settings.setSection_position17 = 0
        } else {
            settings.setSection_position17 = w / 2
        }

        debug_sections()

    }
    function debug_sections() {
        console.debug("Sections changed")
        console.debug(settings.setSection_position1)
        console.debug(settings.setSection_position2)
        console.debug(settings.setSection_position3)
        console.debug(settings.setSection_position4)
        console.debug(settings.setSection_position5)
        console.debug(settings.setSection_position6)
        console.debug(settings.setSection_position7)
        console.debug(settings.setSection_position8)
        console.debug(settings.setSection_position9)
        console.debug(settings.setSection_position10)
        console.debug(settings.setSection_position11)
        console.debug(settings.setSection_position12)
        console.debug(settings.setSection_position13)
        console.debug(settings.setSection_position14)
        console.debug(settings.setSection_position15)
        console.debug(settings.setSection_position16)
        console.debug(settings.setSection_position17)


    }

    GridView {
        id: section_width_grid
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: middleRow.top
        model: section_model
        cellWidth: 200

        clip: true


        delegate:
            SpinBoxCM {
                text: qsTr("Section", "Refers to an implement section") +
                      " " + Number(model.secNum+1).toLocaleString(Qt.locale(),"f",0)

                id: spinner
                from: 3
                to: 7000
                editable: true
                value: model.width
                onValueModified: {
                    model.width = value
                    saveModelToSettings()
                    //debug_sections()
                }
            }
    }
    Row{
        id: middleRow
        anchors.top: parent.verticalCenter
        anchors.right: parent.right
        anchors.margins: 80
        width: children.width
        height: children.height
        spacing: 120

        SpinBoxCM{
            id: defaultSectionWidth
            implicitWidth: 150
            implicitHeight: 50
            from: 10
            boundValue: settings.setTool_defaultSectionWidth
            to: 7000
            editable: true
            text: qsTr("Default Section Width")
            onValueModified: settings.setTool_defaultSectionWidth = value
        }

        ComboBox{
            id: numOfSections
            currentIndex: settings.setVehicle_numSections - 1

            onActivated: function(which) {
                which += 1
                if (which === Number(settings.setVehicle_numSections)) return

                var newstart = 0
                if (which > Number(settings.setVehicle_numSections)) {
                    //add on some new sections
                    newstart = Number(settings.setVehicle_numSections)
                } else {
                    settings.setVehicle_numSections = which

                    //remove all the model items past the new number of
                    //sections
                    for ( ; which < section_model.count ; ) {
                        section_model.remove(which)
                    }
                    saveModelToSettings()
                    //debug_sections()
                    return
                }

                settings.setVehicle_numSections = which

                //append new sections to the model, but don't everwrite the
                //existing ones

                for (var sn = newstart; sn < which ; sn++) {
                    section_model.append({secNum: sn, width: Number(settings.setTool_defaultSectionWidth) })
                }

                saveModelToSettings()
                //debug_sections()
            }

            model: ListModel{
                ListElement {text: qsTr("1")}
                ListElement {text: qsTr("2")}
                ListElement {text: qsTr("3")}
                ListElement {text: qsTr("4")}
                ListElement {text: qsTr("5")}
                ListElement {text: qsTr("6")}
                ListElement {text: qsTr("7")}
                ListElement {text: qsTr("8")}
                ListElement {text: qsTr("9")}
                ListElement {text: qsTr("10")}
                ListElement{text: qsTr("11")}
                ListElement{text: qsTr("12")}
                ListElement{text: qsTr("13")}
                ListElement{text: qsTr("14")}
                ListElement{text: qsTr("15")}
                ListElement{text: qsTr("16")}
            }
            implicitHeight:60
            implicitWidth: 90
            Text{
                anchors.bottom: parent.top
                text: qsTr("Sections")
            }
        }
        Column{
            spacing: 8
            Text {
                text: utils.cm_to_unit_string(totalWidth,0)
            }
            Text {
                text: utils.cm_unit()
                color: "green"
            }
        }
    }
}
