import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts
//import QtQuick.Extras 1.4

import ".."
import "../components"

/*todo:
  couldn't find the setting for the polygons
  or logNMEA
  */
Item {
    anchors.fill: parent

    onVisibleChanged: {
        if(visible)
            //ask backend to refresh our list of vehicles
            vehicleInterface.vehicle_update_list()
    }

    Rectangle{
        id: configWhichVehicle
        anchors.fill: parent
        color: aog.backgroundColor
    Column{
        id: column
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: vehicleList.left
        anchors.margins: 10
        anchors.topMargin: 130
        spacing: 15
        TextLine{ text: qsTr("Units: ")+ (utils.isMetric() ? "Metric" : "Imperial")}
        TextLine{ text: qsTr("Width: ")+ utils.m_to_ft_string(settings.setVehicle_toolWidth)}
        TextLine{ text: qsTr("Sections: ")+ settings.setVehicle_numSections}
        TextLine{ text: qsTr("Offset: ")+  utils.cm_to_unit_string(settings.setVehicle_toolOffset, 0)+ " "+ utils.cm_unit_abbrev()}
        TextLine{ text: qsTr("Overlap: ")+  utils.cm_to_unit_string(settings.setVehicle_toolOverlap, 0)+ " "+ utils.cm_unit_abbrev()}
        TextLine{ text: qsTr("LookAhead: ")+ settings.setVehicle_toolLookAheadOn}
        TextLine{ text: qsTr("Nudge: ")+ utils.cm_to_unit_string(settings.setVehicle_setAS_snapDistance, 0)+ " "+ utils.cm_unit_abbrev()}
        TextLine{ text: qsTr("Tram Width: ")+ utils.m_to_ft_string(settings.setTram_tramWidth )}
        TextLine{ text: qsTr("Wheelbase: ")+ utils.cm_to_unit_string(settings.setVehicle_wheelbase, 0)+ " "+ utils.cm_unit_abbrev() }
    }
        Rectangle{
            id: vehicleListRect
            border.color: aog.blackDayWhiteNight
            color: "light gray"
            visible: true
            anchors.bottom:configWhichVehicle.bottom
            width:configWhichVehicle.width /2
            anchors.top:entryBox.bottom
            anchors.right: load.left
            anchors.rightMargin: 30

            function refresh_model() {
                vehicleList.clear()
                for (var i=0; i < vehicleInterface.vehicle_list.length ; i++) {
                    //console.debug(vehicleInterface.vehicle_list[i])
                    vehicleList.append( { index: vehicleInterface.vehicle_list[i].index,
                                         name: vehicleInterface.vehicle_list[i].name })
                }
            }

            Connections {
                target: vehicleInterface
                function onVehicle_listChanged() {
                    vehicleListRect.refresh_model()
                }
            }

            Component.onCompleted: {
                refresh_model()
            }

            ListView {
                id: vehicleListView
                anchors.fill: parent
                anchors.margins: 1

                model : ListModel{
                    id: vehicleList
                }

                property string selectedVehicle: ""

                delegate: RadioButton{
                    id: control

                    indicator: Rectangle{
                        anchors.fill: parent
                        anchors.margins: 2
                        color: control.down ? aog.backgroundColor : "blue"
                        visible: control.checked
                    }

                    onCheckedChanged: {
                        if(checked) {
                            vehicleListView.selectedVehicle = model.name
                            saveAsVehicle.text = model.name
                        }
                    }

                    width: vehicleListView.width
                    height: 50
                    //anchors.fill: parent
                    //color: "light gray"
                    Text{
                        anchors.left: parent.left
                        anchors.leftMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                        text: model.name
                        font.pixelSize: 20
                        font.bold: true
                        color: control.checked ? aog.backgroundColor : aog.blackDayWhiteNight
                        z: 2
                    }
                }
            }
        }
        Label {
            id: currentVehicle
            text: qsTr("Current vehicle is") + "<h2>" + settings.setVehicle_vehicleName + "</h2>"
            anchors.top: configWhichVehicle.top
            anchors.horizontalCenter: entryBox.horizontalCenter
            anchors.margins: 15
        }
        Rectangle{
            id: entryBox
            anchors.top: currentVehicle.bottom
            anchors.right: configWhichVehicle.right
            height: 75
            width: vehicleListRect.width
            color: parent.color
            IconButtonTransparent{
                id: vehFileSaveAs
                anchors.right: parent.right
                anchors.rightMargin: 30
                anchors.verticalCenter: parent.verticalCenter
                width: 75
                height: 75
                icon.source: "/images/VehFileSaveAs"
                onClicked: {
                    if (saveAsVehicle.text != "") {
                        //console.debug("Going to save", saveAsVehicle.text)
                        vehicleInterface.vehicle_saveas(saveAsVehicle.text)
                        //just setting the name is probably enough to get it to save the vehicle
                        settings.setVehicle_vehicleName = saveAsVehicle.text
                        saveAsVehicle.text = ""
                        vehicleInterface.vehicle_update_list()
                    }
                }
            }
            Text {
                id: saveAs
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter

                text: qsTr("Save As")
            }

            TextField{
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: saveAs.right
                anchors.right: vehFileSaveAs.left

                id: saveAsVehicle
                anchors.margins: 5
                placeholderText: "vehicle and implement"
                selectByMouse: true
            }
        }
        IconButtonText{
            id: load
            anchors.right: parent.right
            anchors.margins: 30
            anchors.top: vehicleList.top
            icon.source: "/images/VehFileLoad.png"
            text: qsTr("Load")
            color3: "white"
            border: 2
            onClicked: {
                if (vehicleListView.selectedVehicle != "" ) {
                    vehicleInterface.vehicle_load(vehicleListView.selectedVehicle)
                    settings.setVehicle_vehicleName = vehicleListView.selectedVehicle
                }
            }

        }
        IconButtonText{
            id: deletefolder
            anchors.top:load.bottom
            anchors.margins: 30
            anchors.horizontalCenter: load.horizontalCenter
            icon.source: "/images/VehFileDelete.png"
            text: qsTr("Delete")
            color3: "white"
            border: 2
            onClicked: {
                //settings.setMenu_isMetric = !utils.isTrue(settings.setMenu_isMetric)
                //console.debug("qml says settings ismetric is",settings.setMenu_isMetric)
                //vehicleInterface.vehicle_delete("testing123")
                if (vehicleListView.selectedVehicle != "" ) {
                    vehicleInterface.vehicle_delete(vehicleListView.selectedVehicle)
                    vehicleInterface.vehicle_update_list()
                }
            }
        }
    }
}
