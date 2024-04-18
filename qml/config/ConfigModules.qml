import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

import ".."
import "../components"

/*todo:
 can't find setting for "invert relays"
 not sure about the "send button"
 */
    Rectangle{
        id: configModules
        anchors.fill: parent
        color: aog.backgroundColor

        onVisibleChanged: {
            load_settings()
        }

        function load_settings()
        {
            if (visible) {
                var sett = settings.setArdMac_setting0

                if ((sett & 1) === 0 ) cboxMachInvertRelays.checked = false
                else cboxMachInvertRelays.checked = true

                if ((sett & 2) === 0 ) cboxIsHydOn.checked = false
                else cboxIsHydOn.checked = true

                nudRaiseTime.value = settings.setArdMac_hydRaiseTime
                nudLowerTime.value = settings.setArdMac_hydLowerTime

                nudUser1.value = settings.setArdMac_user1
                nudUser2.value = settings.setArdMac_user2
                nudUser3.value = settings.setArdMac_user3
                nudUser4.value = settings.setArdMac_user4

                unsaved.visible = false
            }
        }

        function save_settings() {
            var set = 1
            var reset = 2046
            var sett = 0

            if (cboxMachInvertRelays.checked) sett |= set
            else sett &= reset

            set <<=1
            reset <<= 1
            reset += 1

            if(cboxIsHydOn.checked) sett |= set
            else sett &= reset

            settings.setArdMac_setting0 = sett
            settings.setArdMac_hydRaiseTime = nudRaiseTime.value
            settings.setArdMac_hydLowerTime = nudLowerTime.value

            settings.setArdMac_user1 = nudUser1.value
            settings.setArdMac_user2 = nudUser2.value
            settings.setArdMac_user3 = nudUser3.value
            settings.setArdMac_user4 = nudUser4.value

            settings.setVehicle_hydraulicLiftLookAhead = nudHydLiftLookAhead.value
            settings.setArdMac_isHydEnabled = cboxIsHydOn.checked

            //set pgns
            //signal handler will do the folling:
            /*
            mf.p_238.pgn[mf.p_238.set0] = property_setArdMac_setting0
            mf.p_238.pgn[mf.p_238.raiseTime] = property_setArdMac_hydRaiseTime
            mf.p_238.pgn[mf.p_238.lowerTime] = property_setArdMac_hydLowerTime

            mf.p_238.pgn[mf.p_238.user1] = property_setArd_user1
            mf.p_238.pgn[mf.p_238.user2] = property_setArd_user2
            mf.p_238.pgn[mf.p_238.user3] = property_setArd_user3
            mf.p_238.pgn[mf.p_238.user4] = property_setArd_user3
            */

            aog.modules_send_238()
            //pboxSendMachine.Visible = false
        }

        Text {
            id: titleText
            text: qsTr("Machine Module")
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top:parent.top
            anchors.margins: 10
        }
        Rectangle{
            id: hydConfig
            anchors.left: parent.left
            color: aog.backgroundColor
            border.color: aog.blackDayWhiteNight
            anchors.top: titleText.bottom
            anchors.topMargin: 20
            anchors.bottom: cboxMachInvertRelays.top
            anchors.leftMargin: 10
            width: parent.width * .66
            Text {
                id: hydText
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Hydraulic Lift Config")
            }
            GridLayout{
                anchors.fill: parent
                flow: Grid.LeftToRight
                anchors.leftMargin: 10
                rows: 2
                columns: 3
                IconButtonColor{
                    id: cboxIsHydOn
                    height: 130
                    width: 170
                    icon.source: "/images/SwitchOff.png"
                    iconChecked: "/images/SwitchOn.png"

                    checkable: true
                    onClicked: unsaved.visible = true
                }
                SpinBoxCustomized{
                    id: nudRaiseTime
                    from:1
                    //boundValue: settings.setArdMac_hydRaiseTime
                    //onValueChanged: settings.setArdMac_hydRaiseTime = value
                    to: 255
                    editable: true
                    enabled: cboxIsHydOn.checked
                    text: qsTr("Raise Time(secs)")
                    onValueChanged: unsaved.visible = true
                }
                Image{
                    source: "/images/Config/ConMa_LiftRaiseTime"
                    width: 200
                    height: 200
                }
                SpinBoxOneDecimal{
                    id: nudHydLiftLookAhead
                    from:1
                    //boundValue: settings.setVehicle_hydraulicLiftLookAhead
                    //onValueChanged: settings.setVehicle_hydraulicLiftLookAhead = value
                    to: 20
                    editable: true
                    enabled: cboxIsHydOn.checked
                    text: qsTr("Hyd Lift Look Ahead (secs)")
                    onValueChanged: unsaved.visible = true
                }
                SpinBoxCustomized{
                    id: nudLowerTime
                    from:1
                    //boundValue: settings.setArdMac_hydLowerTime
                    //onValueChanged: settings.setArdMac_hydLowerTime = value
                    to: 255
                    editable: true
                    enabled: cboxIsHydOn.checked
                    text: qsTr("Lower Time(secs)")
                    onValueChanged: unsaved.visible = true
                }
                Image{
                    source: "/images/Config/ConMa_LiftLowerTime"
                    width: 200
                    height: 200
                }
            }
        }
        IconButtonColor{
            id: cboxMachInvertRelays
            anchors.horizontalCenter: hydConfig.horizontalCenter
            anchors.bottom: parent.bottom
            text: qsTr("Invert Relays")
            anchors.margins: 10
            icon.source: "/images/Config/ConSt_InvertRelay.png"
            checkable: true
            enabled: cboxIsHydOn.checked
            onClicked: unsaved.visible = true
        }

        Rectangle{
            anchors.bottom: parent.bottom
            anchors.left: hydConfig.right
            anchors.right: parent.right
            anchors.top: titleText.bottom
            anchors.margins: 5
            ColumnLayout{
                anchors.top: parent.top
                width: parent.width
                anchors.bottom: modulesSave.top
                anchors.bottomMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                SpinBoxCustomized{
                    id: nudUser1
                    from: 0
                    //boundValue: settings.setArdMac_user1
                    //onValueChanged: settings.setArdMac_user1 = value
                    to: 255
                    editable: true
                    text: qsTr("User 1")
                    onValueChanged: unsaved.visible = true
                }
                SpinBoxCustomized{
                    id: nudUser2
                    from: 0
                    //boundValue: settings.setArdMac_user2
                    //onValueChanged: settings.setArdMac_user2 = value
                    to: 255
                    editable: true
                    text: qsTr("User 2")
                    onValueChanged: unsaved.visible = true
                }
                SpinBoxCustomized{
                    id: nudUser3
                    from: 0
                    //boundValue: settings.setArdMac_user3
                    //onValueChanged: settings.setArdMac_user3 = value
                    to: 255
                    editable: true
                    text: qsTr("User 3")
                    onValueChanged: unsaved.visible = true
                }
                SpinBoxCustomized{
                    id: nudUser4
                    from: 0
                    //boundValue: settings.setArdMac_user4
                    //onValueChanged: settings.setArdMac_user4 = value
                    to: 255
                    editable: true
                    text: qsTr("User 4")
                    onValueChanged: unsaved.visible = true
                }
            }
            IconButtonTransparent{
                id: modulesSave
                objectName: "btnModulesSave"
                anchors.right: parent.right
                anchors.margins: 20
                anchors.bottom: parent.bottom
                icon.source: "/images/ToolAcceptChange.png"
                onClicked: { save_settings() ; unsaved.visible = false }

                Text{
                    id: modulesSaveLabel
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.left
                    anchors.rightMargin: 5
                    text: qsTr("Send + Save")
                }
            }
            Image {
                id: unsaved
                width: 100
                anchors.right: modulesSave.left
                anchors.rightMargin: modulesSaveLabel.width + 5
                anchors.verticalCenter: modulesSave.verticalCenter
                visible: false
                source: "/images/Config/ConSt_Mandatory.png"
            }
        }
    }
