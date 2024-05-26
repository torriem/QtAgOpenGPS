import QtQuick

Item {
    id: fieldInterface

    //Field loading saving interface
    signal field_update_list()
    signal field_new(string field_name)
    signal field_open(string field_name)
    signal field_new_from(string existing_field, string new_field, int flags)
    signal field_close()
    signal field_delete(string field_name)

    readonly property int loadMapping: 1
    readonly property int loadHeadland: 2
    readonly property int loadLines: 4
    readonly property int loadFlags: 8

    property var field_list: [
        { index: 0, name: "test3", latitude: 53, longitude: -111, hasBoundary: true, boundaryArea: 1232312 },
        { index: 1, name: "test1", latitude: 53.004, longitude: -111, hasBoundary: true, boundaryArea: -10 },
        { index: 2, name: "test2", latitude: 53.1, longitude: -111.2, hasBoundary: true, boundaryArea: -10 },
        { index: 3, name: "test3", latitude: 53.02, longitude: -111, hasBoundary: true, boundaryArea: 1232312 },
        { index: 4, name: "test1", latitude: 53.004, longitude: -111, hasBoundary: true, boundaryArea: -10 },
        { index: 5, name: "test2", latitude: 53.1, longitude: -111.2, hasBoundary: true, boundaryArea: -10 },
        { index: 6, name: "test3", latitude: 53.009, longitude: -111, hasBoundary: true, boundaryArea: 1232312 },
        { index: 7, name: "test1", latitude: 53.004, longitude: -111, hasBoundary: true, boundaryArea: -10 },
        { index: 8, name: "test2", latitude: 53.1, longitude: -111.2, hasBoundary: true, boundaryArea: -10 },
        { index: 9, name: "test3", latitude: 53.2, longitude: -111, hasBoundary: true, boundaryArea: 1232312 },
        { index: 10, name: "test1", latitude: 53.004, longitude: -111, hasBoundary: true, boundaryArea: -10 },
        { index: 11, name: "test2", latitude: 53.1, longitude: -111.2, hasBoundary: true, boundaryArea: -10 }
    ]
}
