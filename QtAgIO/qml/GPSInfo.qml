import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

import "components" as Comp
import "interfaces"

Window {
	title: "GPS Info"
	visible: true
	width: 500
	height: 500
    NTripInterface {
		id: ntrip
	}

	GridLayout {
		id: layout
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		height: parent.height / 2
        rows: 5
        flow: Grid.TopToBottom

		Comp.Text {
			text: qsTr("Latitude: ") + agio.latitude
			Layout.alignment: Qt.AlignCenter
		}

		Comp.Text {
			text: qsTr("Longitude: ") + agio.longitude
			Layout.alignment: Qt.AlignCenter
		}

		Comp.Text {
			text: qsTr("Altitude: ") + agio.altitude
			Layout.alignment: Qt.AlignCenter
		}
		Comp.Text {
			text: qsTr("Speed KMH: ") + agio.speed
			Layout.alignment: Qt.AlignCenter
		}
		Comp.Text {
			text: qsTr("GPS Heading: ") + agio.heading
			Layout.alignment: Qt.AlignCenter
		}
		Comp.Text {
			text: qsTr("IMU Heading: ") + agio.imuheading
			Layout.alignment: Qt.AlignCenter
		}
		Comp.Text {
			text: qsTr("IMU Roll: ") + agio.imuroll
			Layout.alignment: Qt.AlignCenter
		}
		Comp.Text {
			text: qsTr("IMU Pitch: ") + agio.imupitch
			Layout.alignment: Qt.AlignCenter
		}
		//age
		Comp.Text {
			text: qsTr("Age: ") + agio.age
			Layout.alignment: Qt.AlignCenter
		}
		//quality
		Comp.Text {
			text: qsTr("Quality: ") + agio.quality // this will need more
			Layout.alignment: Qt.AlignCenter
		}
		//# Sats
		Comp.Text {
			text: qsTr("# Sats: ", "Number of satellites") + agio.sats // note that 
			Layout.alignment: Qt.AlignCenter
		}
		//yaw rate
		Comp.Text {
			text: qsTr("Yaw Rate: ") + agio.yawrate
			Layout.alignment: Qt.AlignCenter
		}
		//HDOP
		Comp.Text {
			text: "HDOP: " + agio.hdop
			Layout.alignment: Qt.AlignCenter
		}
	}
	ColumnLayout {
		id: strings
        anchors.top: layout.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: parent.bottom
		Comp.Text{
			text: "GGA: " + agio.gga
			Layout.alignment: Qt.AlignCenter
		}
		Comp.Text{
			text: "VTG: " + agio.vtg
			Layout.alignment: Qt.AlignCenter
		}
		Comp.Text{
			text: "NDA: " + agio.panda
			Layout.alignment: Qt.AlignCenter
		}
		Comp.Text{
			text: "OGI: " + agio.paogi
			Layout.alignment: Qt.AlignCenter
		}
		Comp.Text{
			text: "HDT: " + agio.hdt
			Layout.alignment: Qt.AlignCenter
		}
		Comp.Text{
			text: "AVR: " + agio.avr
			Layout.alignment: Qt.AlignCenter
		}
		Comp.Text{
			text: "HPD: " + agio.hpd
			Layout.alignment: Qt.AlignCenter
		}
		Comp.Text{
			text: "SXT: " + agio.sxt
			Layout.alignment: Qt.AlignCenter
		}
	}
}
