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

        Comp.TextLine {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 100
            anchors.leftMargin: 100
            text: "Ntrip Stat: " +
                     (agio.ntripStatus === 0 ? "Invalid" :
                     agio.ntripStatus === 1 ? "Authorizing" :
                     agio.ntripStatus === 2 ? "Waiting" :
                     agio.ntripStatus === 3 ? "Send GGA" :
                     agio.ntripStatus === 4 ? "Listening NTRIP":
                     agio.ntripStatus === 5 ? "Wait GPS":
                     "Unknown")
        }

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
                text: settings.setNTRIP_url
				width: parent.width
				selectByMouse: true
				onTextChanged: 
                    if (text.length > 0) {
                        settings.setNTRIP_url = text
                    }

			}
            Comp.ButtonColor {
				text: qsTr("Verify")
                onClicked: agio.setIPFromUrl(ntripURL.text)
                width: parent.width * .9
            }

            Comp.Text {
                text: "IP: " + settings.setNTRIP_ipAddress

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
                text: settings.setNTRIP_userName
				width: parent.width
				selectByMouse: true
                onTextChanged: settings.setNTRIP_userName = text
			}

            Comp.Spacer{}

            Comp.Text {
				text: qsTr("Password")
			}
			TextField {
				id: ntripPass
                text: settings.setNTRIP_userPassword
                onTextChanged: settings.setNTRIP_userPassword = text
				width: parent.width
				selectByMouse: true
			}
            Comp.Spacer{}
            Comp.ButtonColor {
				text: qsTr("Get Source Table")
                width: parent.width * .9
			}
            Comp.Text {
				text: qsTr("Mount:", "Ntrip Mountpoint")
			}
			TextField {
				id: ntripMount
                text: settings.setNTRIP_mount
                onTextChanged: settings.setNTRIP_mount = text
				width: parent.width
				selectByMouse: true
			}
            Comp.Spacer{}
            Comp.SpinBoxCustomized { //ntrip caster port number
                value: settings.setNTRIP_casterPort
                onValueChanged: settings.setNTRIP_casterPort = value
				from: 0
				to: 65535
                text: qsTr("Caster Port:")
			}
            Comp.Text {
                text: qsTr("Default: 2101")
                color: "red"
                visible: settings.setNTRIP_casterPort !== 2101
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
        Comp.ButtonColor {
			id: ntripOn
            property bool statusChanged: false
            color: "white"
            colorChecked: "green"
			text: qsTr("NTRIP On")
			height: parent.height
			width: height * 2.5
            checkable: true
            checked: settings.setNTRIP_isOn
            onClicked: {
                settings.setNTRIP_isOn = checked
                ntripOn.statusChanged = true;
            }
		}
        Comp.IconButtonTransparent {
			id: cancel
            visible: false //not sure if we even want/need this
			height: parent.height
			width: height
			icon.source: prefix + "/images/Cancel64.png"
            onClicked: ntrip.visible = false
		}
        Comp.IconButtonTransparent {
			id: btnSave
			height: parent.height
			width: height
			icon.source: prefix + "/images/OK64.png"
            onClicked: {
                agio.configureNTRIP()
                ntrip.close()
                //restart if anything changed
                if(ntripOn.statusChanged)
                    message.addMessage("", "Restart of AgIO is Required - Restarting", true)
            }
        }
	}
}
