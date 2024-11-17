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
        anchors.margins: 4
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
        anchors.bottom: sentences.top
        rows: 4
        flow: Grid.TopToBottom

		Comp.Text {
            font.pixelSize: 10
            text: qsTr("Lat: ") + (Number(agio.latitude).toLocaleString(Qt.locale(), 'f', 7))
			Layout.alignment: Qt.AlignLeft
		}

		Comp.Text {
            font.pixelSize: 10
            text: qsTr("Lon: ") + (Number(agio.longitude).toLocaleString(Qt.locale(), 'f', 7))
			Layout.alignment: Qt.AlignLeft
		}

        Comp.Text {
            font.pixelSize: 10
            text: qsTr("Speed KMH: ") + Math.round(agio.speed * 100) / 100
            Layout.alignment: Qt.AlignLeft
        }
        //quality
        Comp.Text {
            font.pixelSize: 10
            text: qsTr("Quality: ") + agio.quality // this will need more
            Layout.alignment: Qt.AlignLeft
        }

        //HDOP
        Comp.Text {
            font.pixelSize: 10
            text: "HDOP: " + agio.hdop
            Layout.alignment: Qt.AlignLeft
        }
        //# Sats
        Comp.Text {
            font.pixelSize: 10
            text: qsTr("# Sats: ", "Number of satellites") + agio.sats // note that
            Layout.alignment: Qt.AlignLeft
        }
        Comp.Text {
            font.pixelSize: 10
            text: qsTr("Altitude: ") + (Number(agio.altitude).toLocaleString(Qt.locale(), 'f', 2))
			Layout.alignment: Qt.AlignLeft
		}
        //age
        Comp.Text {
            font.pixelSize: 10
            text: qsTr("Age: ") + (Number(agio.age).toLocaleString(Qt.locale(), 'f', 1))
            Layout.alignment: Qt.AlignLeft
        }
        Comp.Text {
            font.pixelSize: 10
            text: qsTr("VTG: ") + Math.round(agio.gpsHeading * 100) / 100
            Layout.alignment: Qt.AlignLeft
        }
        Comp.Text {
            font.pixelSize: 10
            text: qsTr("Dual: ") + Math.round(agio.dualHeading * 100) / 100
            Layout.alignment: Qt.AlignLeft
        }
        Comp.Text {
            font.pixelSize: 10
            text: qsTr("Roll: ") + agio.imuRoll
			Layout.alignment: Qt.AlignLeft
		}
        Column{
            Comp.Text{
                font.pixelSize: 10
                text: qsTr("Hz: ") + Math.round(agio.gpsHz * 100) / 100
                Layout.alignment: Qt.AlignLeft
            }
            Comp.Text{
                font.pixelSize: 10
                text: qsTr("Raw Hz: ") + Math.round(agio.nowHz * 100) / 100
                Layout.alignment: Qt.AlignLeft
            }
        }

		Comp.Text {
            font.pixelSize: 10
            text: qsTr("Yaw: ") + Number(agio.yawrate).toLocaleString(Qt.locale(), 'f', 1)
			Layout.alignment: Qt.AlignLeft
		}
		Comp.Text {
            font.pixelSize: 10
            text: qsTr("Roll: ") + (Number(agio.imuRoll).toLocaleString(Qt.locale(), 'f', 1))
			Layout.alignment: Qt.AlignLeft
		}
		Comp.Text {
            font.pixelSize: 10
            text: qsTr("Pitch: ") + (Number(agio.imuPitch).toLocaleString(Qt.locale(), 'f', 1))
			Layout.alignment: Qt.AlignLeft
		}
	}
    Comp.TitleFrame{
        id: sentences
        title: qsTr("NMEA Sentences")
        height: parent.height *.66 // 2/3 of parent
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        ColumnLayout {
            anchors.fill: parent
            id: strings
            Comp.Text{
                font.pixelSize: 10
                text: "GGA: " + agio.gga
                Layout.alignment: Qt.AlignLeft
            }
            Comp.Text{
                font.pixelSize: 10
                text: "VTG: " + agio.vtg
                Layout.alignment: Qt.AlignLeft
            }
            Comp.Text{
                font.pixelSize: 10
                text: "NDA: " + agio.panda
                Layout.alignment: Qt.AlignLeft
            }
            Comp.Text{
                font.pixelSize: 10
                text: "OGI: " + agio.paogi
                Layout.alignment: Qt.AlignLeft
            }
            Comp.Text{
                font.pixelSize: 10
                text: "HDT: " + agio.hdt
                Layout.alignment: Qt.AlignLeft
            }
            Comp.Text{
                font.pixelSize: 10
                text: "AVR: " + agio.avr
                Layout.alignment: Qt.AlignLeft
            }
            Comp.Text{
                font.pixelSize: 10
                text: "HPD: " + agio.hpd
                Layout.alignment: Qt.AlignLeft
            }
            Comp.Text{
                font.pixelSize: 10
                text: "SXT: " + agio.sxt
                Layout.alignment: Qt.AlignLeft
            }
        }
    }
}
