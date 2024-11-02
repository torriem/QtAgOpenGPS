import QtQuick
import QtQuick.Controls.Fusion

import ".."
import "../components"

Rectangle{
    id: configTractor
    anchors.fill: parent
    visible: true
    color: aog.backgroundColor
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
            width:150  * theme.scaleWidth
            height:100 * theme.scaleHeight
            checkable: true
            icon.source: prefix + "/images/vehiclePageHarvester.png"
            isChecked: settings.setVehicle_vehicleType === 1
            onClicked: settings.setVehicle_vehicleType = 1
        }
        IconButtonColor{
            width:150 * theme.scaleWidth
            height:100 * theme.scaleHeight
            id: tractor
            icon.source: prefix + "/images/vehiclePageTractor.png"
            checkable: true
            isChecked: settings.setVehicle_vehicleType === 0
            onClicked: settings.setVehicle_vehicleType = 0
        }
        IconButtonColor{
            width:150 * theme.scaleWidth
            height:100 * theme.scaleHeight
            id: tr4wd
            icon.source: prefix + "/images/vehiclePage4WD.png"
            checkable: true
            isChecked: settings.setVehicle_vehicleType === 2
            onClicked: settings.setVehicle_vehicleType = 2
        }
    }
	IconButtonColor{//triangle, no vehicle image
		anchors.top: typesButtons.bottom
		anchors.horizontalCenter: typesButtons.horizontalCenter
		anchors.topMargin: 20
		width:150 * theme.scaleWidth
		height:100 * theme.scaleHeight
		icon.source: prefix + "/images/Config/Brand/TriangleVehicle.png"
		checkable: true
		isChecked: !settings.setDisplay_isVehicleImage
		onCheckedChanged: {
			settings.setDisplay_isVehicleImage = !checked
			console.log(settings.setDisplay_isVehicleImage)
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
