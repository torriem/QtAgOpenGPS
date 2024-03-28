import QtQuick

Item {
    id: vehicleInterface

    signal vehicle_saveas(string vehicle_name)
    signal vehicle_load(string vehicle_name)
    signal vehicle_delete(string vehicle_name)
    signal vehicle_update_list()

    property var vehicle_list: [
        { index: 0, name: "tractor" }
    ]
}
