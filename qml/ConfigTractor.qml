import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle{
    id: configTractor
    anchors.fill: parent
    visible: true
    color: "ghostwhite"
    Text {
        id: text
        text: qsTr("Choose Vehicle Type")
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Row{
        anchors.top: text.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        IconButtonColor{
            id: harvester
            width:150
            height:100
            icon.source: "/images/vehiclePageHarvester.png"
            isChecked: (settings.setVehicle_vehicleType = 1 ? false: true)
            onClicked: settings.setVehicle_vehicleType = 1
            Connections{
                target: settings
                function onSetVehicle_vehicleTypeChanged(){
                    if(settings.setVehicle_vehicleType === 1){
                        harvester.checked = true
                        harvester.checkable = false
                    }else{
                        harvester.checked = false
                        harvester.checkable = true
                    }
                }
            }
        }
        IconButtonColor{
            width:150
            height:100
            id: tractor
            icon.source: "/images/vehiclePageTractor.png"
            isChecked: (settings.setVehicle_vehicleType = 0 ? false: true)
            onClicked: settings.setVehicle_vehicleType = 0
            Connections{
                target: settings
                function onSetVehicle_vehicleTypeChanged(){
                    if(settings.setVehicle_vehicleType === 0){
                        tractor.checked = true
                        tractor.checkable = false
                    }else{
                        tractor.checked = false
                        tractor.checkable = true
                    }
                }
            }
        }
        IconButtonColor{
            width:150
            height:100
            id: tr4wd
            icon.source: "/images/vehiclePage4WD.png"
            isChecked: (settings.setVehicle_vehicleType = 2 ? false: true)
            onClicked: settings.setVehicle_vehicleType = 2
            Connections{
                target: settings
                function onSetVehicle_vehicleTypeChanged(){
                    if(settings.setVehicle_vehicleType === 2){
                        tr4wd.checked = true
                        tr4wd.checkable = false
                    }else{
                        tr4wd.checked = false
                        tr4wd.checkable = true
                    }
                }
            }
        }
    }
    Text {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("I consider the rest of this page nonessential for now and will be added at a later date")
    }
}
