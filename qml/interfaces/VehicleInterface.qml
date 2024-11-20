// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Interface between formgps_UI_vehicle.cpp and frontend vehicle loading stuff
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
