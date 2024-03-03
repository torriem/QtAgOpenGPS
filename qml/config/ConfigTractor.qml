import QtQuick 2.0
import QtQuick.Controls 2.5

import ".."

Rectangle{
    id: configTractor
    anchors.fill: parent
    visible: true
    color: "ghostwhite"
    Text {
        id: text
        text: qsTr("<h1>Choose Vehicle Type</h1>")
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
    }

    ButtonGroup {
        buttons:  typesButtons.children
    }

    Row{
        id: typesButtons
        anchors.top: text.bottom
        anchors.horizontalCenter: parent.horizontalCenter


        IconButtonColor{
            id: harvester
            width:150
            height:100
            checkable: true
            icon.source: "/images/vehiclePageHarvester.png"
            isChecked: settings.setVehicle_vehicleType === 1
            onClicked: settings.setVehicle_vehicleType = 1
        }
        IconButtonColor{
            width:150
            height:100
            id: tractor
            icon.source: "/images/vehiclePageTractor.png"
            checkable: true
            isChecked: settings.setVehicle_vehicleType === 0
            onClicked: settings.setVehicle_vehicleType = 0
        }
        IconButtonColor{
            width:150
            height:100
            id: tr4wd
            icon.source: "/images/vehiclePage4WD.png"
            checkable: true
            isChecked: settings.setVehicle_vehicleType === 2
            onClicked: settings.setVehicle_vehicleType = 2
        }
    }
    /*
    Text {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("I consider the rest of this page nonessential for now and will be added at a later date")
    }
    */
}
