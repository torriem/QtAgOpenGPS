import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 1.4

TableView {
    id: tableView
    //frameVisible: false
    //sortIndicatorVisible: true
    //sortIndicatorColumn: 2
    /*
    AOGInterface {
        id: aog
    }

    UnitConversion {
        id: utils
    }

    MockSettings {
        id: settings
    }*/

    //needed for qmlscene testing
    Component.onCompleted: update_model()

    function update_model() {
        var distance = 0.0

        fieldsModel.clear()
        for( var i=0; i < aog.field_list.length ;i++)  {
            distance = utils.distanceLatLon(aog.latitude, aog.longitude, aog.field_list[i].latitude, aog.field_list[i].longitude)
            fieldsModel.append( { index: i,
                                  name: aog.field_list[i].name,
                                  distance: distance,
                                  boundaryArea: aog.field_list[i].boundaryArea
                              } )
        }
    }

    Connections {
        target: aog
        function onField_listChanged() {
            update_model()
        }
    }

    selectionMode: SelectionMode.SingleSelection

    Layout.minimumWidth: 600
    Layout.minimumHeight: 300
    Layout.preferredWidth: 700
    Layout.preferredHeight: 400

    TableViewColumn {
        id: nameColumn
        title: "Field name"
        role: "name"
        movable: false
        resizable: true
        width: tableView.width - distanceColumn.width - areaColumn.width
    }

    TableViewColumn {
        id: distanceColumn
        title: "Distance"
        role: "distance"
        movable: false
        resizable: true
        delegate: Text {
            //text: utils.km_to_unit_string(tableView.model.get(styleData.row).distance,1) + " " + utils.km_unit()
            text: tableView.model.get(styleData.row) ?
                      utils.km_to_unit_string(tableView.model.get(styleData.row).distance,1) + " " + utils.km_unit()
                    : "unknown"

        }
        width: tableView.width > 300 ? tableView.width / 5 : 60
    }

    TableViewColumn {
        id: areaColumn
        title: "Area"
        role: "boundaryArea"
        movable: false
        resizable: true
        delegate: Text {
            text: tableView.model.get(styleData.row) ?
                      tableView.model.get(styleData.row).boundaryArea < 0 ?
                          "No Boundary" :
                          utils.area_to_unit_string(tableView.model.get(styleData.row).boundaryArea,1) + " " + utils.area_unit()
            : "unknown"
        }
        width: tableView.width > 300 ? tableView.width / 4 : 60
    }

    //TODO proxy model for sorting by distance, etc
    model: fieldsModel

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
