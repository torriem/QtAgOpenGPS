import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ListView {
    id: tableView
    //frameVisible: true
    //sortIndicatorVisible: true
    //sortIndicatorColumn: 0

    //needed for qmlscene testing
    Component.onCompleted: update_model()
    clip: true

    signal changeDriveThrough(int index, bool drive_through)

    //property int sortBy: 1 //1 = name, 2 = distance, negative is reverse

    //onSortByChanged: sort()

    function clear_selection() {
        currentIndex = -1
    }

    function update_model() {
        var distance = 0.0

        boundariesModel.clear()
        for( var i=0; i < boundaryInterface.boundary_list.length ;i++)  {
            boundariesModel.append( { index: i,
                                      area: boundaryInterface.boundary_list[i].area,
                                      drive_through: boundaryInterface.boundary_list[i].drive_through
                              } )
        }
        //sort()
    }

    /*
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
    */

    /*
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
    */

    Connections {
        target: boundaryInterface
        function onBoundary_listChanged() {
            update_model()
        }
    }

    keyNavigationEnabled: true

    model: boundariesModel
    currentIndex: -1
    focus: true
    headerPositioning: ListView.OverlayHeader

    header: Rectangle {
        z: 2
        color: "white"
        implicitWidth: tableView.width - scrollbar.width
        height: childrenRect.height

        Rectangle {
            id: bndNumHeader

            anchors.top: parent.top
            anchors.left: parent.left
            width: tableView.width * 0.333
            height: bndNumHeaderText.height + 10

            border.color: "black"
            border.width: 1
            color: "white"

            Text {
                id: bndNumHeaderText
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 5
                //anchors.verticalCenter: parent.verticalCenter

                text: qsTr("Boundary")
                font.pointSize: 16
            }
            /*
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
            */
        }

        Rectangle {
            id: bndAreaHeader

            anchors.top: parent.top
            anchors.left: bndNumHeader.right
            height: bndNumHeaderText.height + 10

            width: tableView.width * 0.333
            border.color: "black"
            border.width: 1
            color: "white"

            Text {
                anchors.fill: parent
                anchors.margins: 5
                //anchors.verticalCenter: parent.verticalCenter

                id: bndAreaHeaderText

                text: qsTr("Area")
                font.pointSize: 16
            }
            /*
            MouseArea {
                anchors.fill: parent

                onClicked: {
                    if (Math.abs(sortBy) != 2) {
                        sortBy = 2
                    } else {
                        sortBy = -sortBy
                    }
                }
            }*/

        }

        Rectangle {
            id: areaHeaderRect

            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: bndAreaHeader.right
            border.color: "black"
            border.width: 1
            color: "white"

            height: bndNumHeaderText.height + 10
            Text {
                anchors.fill: parent
                anchors.margins: 5
                //anchors.verticalCenter: parent.verticalCenter

                id: areaHeader

                text: qsTr("Drive Through")
                font.pointSize: 16
            }
            /*
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
            */
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
            width: parent.width * 0.333

            anchors.topMargin: 5
            anchors.leftMargin: 5

            id: bndIndex
            text: model.index === 0 ?
                      qsTr("Outer", "outer boundary") :
                      qsTr("Inner", "inner boundary") + " " +
                      Number(model.index).toLocaleString(Qt.locale(),'f',0)
            elide: Text.ElideRight
            font.pointSize: 16
        }
        Text {
            anchors.top: parent.top
            anchors.left: bndIndex.right
            anchors.leftMargin: 5
            anchors.topMargin: 5
            width: parent.width * 0.33

            id: bndArea
            text: utils.area_to_unit_string(model.area,1)+ " " + utils.area_unit()
            font.pointSize: 16
        }
        Button {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: bndArea.right

            text: model.drive_through ? qsTr("Yes") : qsTr("No")
            enabled: model.index !== 0
            font.pointSize: 16
            onClicked: {
                model.drive_through = !model.drive_through
                changeDriveThrough(model.index, model.drive_through)
            }
        }

        MouseArea {
            id: thisisdumb
            anchors.fill: parent
            onClicked: {
                currentIndex = index
                mouse.accepted = false
            }
            onPressed: {
                currentIndex = index
                mouse.accepted = false
            }
        }
    }

    ListModel {
        id: boundariesModel
        ListElement {
            index: 0
            area: 75
            drive_through: false
        }
        ListElement {
            index: 1
            area: 5
            drive_through: false
        }
        ListElement {
            index: 2
            area: 6
            drive_through: true
        }
    }
}
