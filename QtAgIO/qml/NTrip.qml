import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import "components" as Comp
import "interfaces"

Window {
	id: ntrip
    height: 490
    width: 648
	visible: true
    title: qsTr("NTRIP RTK")

	NTripInterface {
		id: ntripInterface
	}

	Rectangle {
		id: source
		visible: true
		color: theme.backgroundColor
		anchors.top: parent.top
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: bottomButtons.top
		border.width: 2
		border.color: theme.blackDayWhiteNight

        CheckBox {
            id: ckNtripDebug
            text: "Console NTRIP Debug"
            checked: false
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 100
            anchors.leftMargin: 100
            onCheckedChanged: agio.ntripDebug(ckNtripDebug.checkState)
        }
        Comp.TextLine {
            anchors.top: ckNtripDebug.bottom
            anchors.left: ckNtripDebug.left
            text: "Ntrip Stat: " +
                     (agio.ntripStatus === 0 ? "Invalid" :
                     agio.ntripStatus === 1 ? "Authorizing" :
                     agio.ntripStatus === 2 ? "Waiting" :
                     agio.ntripStatus === 3 ? "Send GGA" :
                     agio.ntripStatus === 4 ? "Listening":
                     "Unknown")        }

		ColumnLayout {
			id: leftColumn
			anchors.top: parent.verticalCenter
			anchors.left: parent.left
            width: parent.width / 2 - (30 * theme.scaleWidth)
			anchors.bottom: parent.bottom
            anchors.leftMargin: 15 * theme.scaleWidth
            anchors.rightMargin: 15 * theme.scaleWidth
            anchors.topMargin: 15 * theme.scaleHeight
            anchors.bottomMargin: 15 * theme.scaleHeight
            Comp.Text {
				text: qsTr("Enter Broadcaster URL or IP")
			}
			TextField {
				id: ntripURL
				placeholderText: qsTr("URL")
				width: parent.width
				selectByMouse: true
				onTextChanged: 
					if (text.length > 0) ntripInterface.setUrl(text)

			}
            Comp.Button {
				text: qsTr("Verify")
				onClicked: ntripInterface.setUrl(ntripURL.text)
                width: parent.width * .9
            }

            Comp.Text {
				text: "IP: " + "0.0.0.0"
			}
		}
		ColumnLayout {
			id: rightColumn
            anchors.top: parent.top
			anchors.right: parent.right
			anchors.bottom: parent.bottom
            width: parent.width / 2 - (30 * theme.scaleWidth)
            anchors.leftMargin: 15 * theme.scaleWidth
            anchors.rightMargin: 15 * theme.scaleWidth
            anchors.topMargin: 15 * theme.scaleHeight
            anchors.bottomMargin: 15 * theme.scaleHeight
            Comp.Text {
				text: qsTr("Username")
			}
			TextField {
				id: ntripUser
				placeholderText: qsTr("Username")
				width: parent.width
				selectByMouse: true
			}

            Comp.Spacer{}

            Comp.Text {
				text: qsTr("Password")
			}
			TextField {
				id: ntripPass
				placeholderText: qsTr("Password")
				width: parent.width
				selectByMouse: true
			}
            Comp.Spacer{}
            Comp.Button {
				text: qsTr("Get Source Table")
                width: parent.width * .9
			}
            Comp.Text {
				text: qsTr("Mount:", "Ntrip Mountpoint")
			}
			TextField {
				id: ntripMount
				placeholderText: qsTr("Mount")
				width: parent.width
				selectByMouse: true
			}
            Comp.Spacer{}
            Comp.SpinBoxCustomized { //ntrip caster port number
				value: 2101
				from: 0
				to: 65535
                text: qsTr("Caster Port:")
			}
            Comp.Text {
                text: qsTr("Default: 2101")
            }
        }
	}
	Row {
		height: 50* theme.scaleHeight
		id: bottomButtons
		anchors.bottom: parent.bottom
		anchors.right: parent.right
		anchors.bottomMargin: 5 * theme.scaleHeight
		anchors.rightMargin: 5 * theme.scaleWidth
		spacing: 5 * theme.scaleWidth
        Comp.Button {
			id: ntripOn
			text: qsTr("NTRIP On")
			height: parent.height
			width: height * 2.5
		}
        Comp.IconButtonTransparent {
			id: cancel
			height: parent.height
			width: height
			icon.source: "/images/Cancel64.png"
            onClicked: ntrip.visible = false
		}
        Comp.IconButtonTransparent {
			id: btnSave
			height: parent.height
			width: height
			icon.source: "/images/OK64.png"
            onClicked: ntrip.visible = false
        }
	}
}
