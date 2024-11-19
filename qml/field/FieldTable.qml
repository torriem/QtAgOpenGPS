// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Loaded by FieldOpen.qml. Contains the list of fields
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import ".."
import "../components"

ListView {
    id: tableView
    //frameVisible: true
    //sortIndicatorVisible: true
    //sortIndicatorColumn: 0

    //needed for qmlscene testing
    Component.onCompleted: update_model()
    clip: true

    property int sortBy: 1 //1 = name, 2 = distance, negative is reverse

    onSortByChanged: sort()

    function clear_selection() {
        currentIndex = -1
        currentFieldName = ""
    }

    function update_model() {
        var distance = 0.0

        fieldsModel.clear()
        for( var i=0; i < fieldInterface.field_list.length ;i++)  {
            distance = utils.distanceLatLon(aog.latitude, aog.longitude, fieldInterface.field_list[i].latitude, fieldInterface.field_list[i].longitude)
            fieldsModel.append( { index: i,
                                  name: fieldInterface.field_list[i].name,
                                  distance: distance,
                                  boundaryArea: fieldInterface.field_list[i].boundaryArea
                              } )
        }
        sort()
    }

    function listModelSort(listModel, compare_function) {
        let indexes = [ ...Array(listModel.count).keys() ]

        indexes.sort( (a, b) => compare_function( listModel.get(a), listModel.get(b) ) )
        let sorted = 0

        while ( sorted < indexes.length && sorted === indexes[sorted] ) sorted++

        if ( sorted === indexes.length ) return

        for ( let i = sorted; i < indexes.length; i++ ) {
            listModel.move( indexes[i], listModel.count - 1, 1 )
            listModel.insert( indexes[i], { } )
        }

        listModel.remove( sorted, indexes.length - sorted )

        //fix stupid indexes
        for( let j = 0; j < fieldsModel.count ; j++) {
            fieldsModel.get(j).index = j
        }
    }

    function sort() {
        if (sortBy === -1) {
            listModelSort( fieldsModel, (a, b) => - a.name.localeCompare(b.name) )
        } else if (sortBy === 1) {
            listModelSort( fieldsModel, (a, b) => a.name.localeCompare(b.name) )
        } else if (sortBy === 2) {
            listModelSort( fieldsModel, (a, b) => (a.distance - b.distance) )
        } else if (sortBy === -2) {
            listModelSort( fieldsModel, (a, b) => -(a.distance - b.distance) )
        } else if (sortBy === -3) {
            listModelSort( fieldsModel, (a, b) => -(a.boundaryArea - b.boundaryArea) )
        } else {
            listModelSort( fieldsModel, (a, b) => (a.boundaryArea - b.boundaryArea) )
        }

        currentIndex = -1
        currentFieldName = ""
    }

    //TODO implement a model sort function

    Connections {
        target: fieldInterface
        function onField_listChanged() {
            update_model()
        }
    }

    property string currentFieldName: ""

    //Layout.minimumWidth: 200
    //Layout.minimumHeight: 200
    //Layout.preferredWidth: 400
    //Layout.preferredHeight: 400

    keyNavigationEnabled: true

    model: fieldsModel
    currentIndex: -1
    focus: true
    headerPositioning: ListView.OverlayHeader

    header: Rectangle {
        z: 2
        color: "white"
        implicitWidth: tableView.width - scrollbar.width
        height: childrenRect.height

        Rectangle {
            id: nameHeaderRect

            anchors.top: parent.top
            anchors.left: parent.left
            width: tableView.width * 0.5
            height: nameHeaderText.height + 10

            border.color: "black"
            border.width: 1
            color: "white"

            Text {
                id: nameHeaderText
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 5
                //anchors.verticalCenter: parent.verticalCenter

                text: qsTr("Field name")
                font.pointSize: 20
            }
            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if (Math.abs(sortBy) != 1) {
                        sortBy = 1
                    } else {
                        sortBy = -sortBy
                    }
                }
            }
        }

        Rectangle {
            id: distanceHeaderRect

            anchors.top: parent.top
            anchors.left: nameHeaderRect.right
            height: nameHeaderText.height + 10

            width: tableView.width * 0.2
            border.color: "black"
            border.width: 1
            color: "white"

            Text {
                anchors.fill: parent
                anchors.margins: 5
                //anchors.verticalCenter: parent.verticalCenter

                id: distanceHeader

                text: qsTr("Distance")
                font.pointSize: 20
            }
            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if (Math.abs(sortBy) != 2) {
                        sortBy = 2
                    } else {
                        sortBy = -sortBy
                    }
                }
            }

        }

        Rectangle {
            id: areaHeaderRect

            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: distanceHeaderRect.right
            border.color: "black"
            border.width: 1
            color: "white"

            height: nameHeaderText.height + 10
            Text {
                anchors.fill: parent
                anchors.margins: 5
                //anchors.verticalCenter: parent.verticalCenter

                id: areaHeader

                text: qsTr("Area")
                font.pointSize: 20
            }
            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if (Math.abs(sortBy) != 3) {
                        sortBy = 3
                    } else {
                        sortBy = -sortBy
                    }
                }
            }
        }
    }

    spacing: 2

    delegate: Rectangle {
        height: childrenRect.height
        implicitWidth: tableView.width - scrollbar.width

        color: ListView.isCurrentItem ? "light blue" : "light grey"

        Text {
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width * 0.5

            anchors.topMargin: 5
            anchors.leftMargin: 5

            id: name
            text: model.name
            elide: Text.ElideRight
            font.pointSize: 18
        }
        Text {
            anchors.top: parent.top
            anchors.left: name.right
            anchors.leftMargin: 5
            anchors.topMargin: 5
            width: parent.width * 0.2

            id: distanceArea
            text: utils.km_to_unit_string(model.distance,1)+ " " + utils.km_unit()
            font.pointSize: 16
        }
        Text {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: distanceArea.right

            text: (model.boundaryArea < 1 ?
                       qsTr("No boundary") :
                       utils.area_to_unit_string(model.boundaryArea,1) + " " + utils.area_unit())
            font.pointSize: 16
        }

        MouseArea {
            id: thisisdumb
            anchors.fill: parent
            onClicked: {
                currentIndex = index
                currentFieldName = model.name
            }
        }
    }

    ListModel {
        id: fieldsModel
        ListElement {
            index: 0
            name: "Field 1"
            hasBoundary: false
            boundaryArea: -10
            distance: 1023
        }
        ListElement {
            index: 1
            name: "Field 2"
            hasBoundary: true
            boundaryArea: 25234
            distance: 500
        }
        ListElement {
            index: 2
            name: "Field 3"
            hasBoundary: true
            boundaryArea: 493718
            distance: 1023
        }
    }
}
