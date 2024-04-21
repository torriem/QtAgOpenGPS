import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

import ".."
import "../components"
Item{
    id: configWindow
	onVisibleChanged:{
		load_settings()
	}
	function load_settings(){
		if (visible) {
			var sett = settings.setArdSteer_setting1

            if ((sett & 1) == 0) chkInvertWAS.Checked = false;
            else chkInvertWAS.Checked = true;

            if ((sett & 2) == 0) chkSteerInvertRelays.Checked = false;
            else chkSteerInvertRelays.Checked = true;

            if ((sett & 4) == 0) chkInvertSteer.Checked = false;
            else chkInvertSteer.Checked = true;

            if ((sett & 8) == 0) cboxConv.Text = "Differential";
            else cboxConv.Text = "Single";

            if ((sett & 16) == 0) cboxMotorDrive.Text = "IBT2";
            else cboxMotorDrive.Text = "Cytron";

            if ((sett & 32) == 32) cboxSteerEnable.Text = "Switch";
            else if ((sett & 64) == 64) cboxSteerEnable.Text = "Button";
            else cboxSteerEnable.Text = "None";

            if ((sett & 128) == 0) cboxEncoder.Checked = false;
            else cboxEncoder.Checked = true;

            /*nudMaxCounts.Value = (decimal)Properties.Settings.Default.setArdSteer_maxPulseCounts;
            hsbarSensor.Value = (int)Properties.Settings.Default.setArdSteer_maxPulseCounts;
			lblhsbarSensor.Text = ((int)((double)hsbarSensor.Value * 0.3921568627)).ToString() + "%";*/

            sett = settings.setArdSteer_setting1;

            if ((sett & 1) == 0) cboxDanfoss.Checked = false;
            else cboxDanfoss.Checked = true;

            if ((sett & 8) == 0) cboxXY.Text = "X";
            else cboxXY.Text = "Y";

            if ((sett & 2) == 0) cboxPressureSensor.Checked = false;
            else cboxPressureSensor.Checked = true;

            if ((sett & 4) == 0) cboxCurrentSensor.Checked = false;
            else cboxCurrentSensor.Checked = true;

		}
	}
    GridLayout{
        anchors.fill: parent
        rows: 4
        columns: 2
        flow: Grid.TopToBottom
        IconButtonColor{
			id: cboxDanfoss
            icon.source: "/images/Config/ConST_Danfoss.png"
            isChecked: ((settings.setArdSteer_setting1 & 1) != 0)
            buttonText: "Danfoss"
            Layout.alignment: Qt.AlignCenter
        }
        IconButtonColor{
			id: chkInvertWAS
            icon.source: "/images/Config/ConSt_InvertWAS.png"
            isChecked: ((settings.setArdSteer_setting0 & 1) != 0)
            Layout.alignment: Qt.AlignCenter
            buttonText: "Invert WAS"
        }
        IconButtonColor{
			id: chkInvertSteer
            icon.source: "/images/Config/ConSt_InvertDirection.png"
            isChecked: ((settings.setArdSteer_setting0 & 4) != 0)
            buttonText: "Invert Motor Dir"
            Layout.alignment: Qt.AlignCenter
        }
        IconButtonColor{
			id: chkSteerInvertRelays
            icon.source: "/images/Config/ConSt_InvertRelay.png"
            isChecked: ((settings.setArdSteer_setting0 & 2) != 0)
            buttonText: "Invert Relays"
            Layout.alignment: Qt.AlignCenter
        }
    }
    ColumnLayout{
        id: columnOfDropDown
        anchors.top: parent.top
        anchors.margins: 10
        anchors.bottomMargin: 50
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        height: parent.height /2
        ComboBoxCustomized {
			id: cboxMotorDrive
            editable: true
            Component.onCompleted: currentIndex = ((settings.setArdSteer_setting0 & 16) == 0) ? 1 : 0
            model: ListModel {
                id: modelmotorDriver
                ListElement {text: "Cytron"}
                ListElement {text: "IBT2"}
            }
            text: ("Motor Driver")
        }
        ComboBoxCustomized {
			id: cboxConv
            editable: true
            Component.onCompleted: currentIndex = ((settings.setArdSteer_setting0 & 8) == 0) ? 1 : 0
            model: ListModel {
                id: a2Dmodel
                ListElement {text: "Single"}
                ListElement {text: "Differential"}
            }
            text: qsTr("A2D Converter")
        }
        ComboBoxCustomized {
			id: cboxXY
            editable: true
            Component.onCompleted: currentIndex = ((settings.setArdSteer_setting1 & 8) == 0) ? 0 : 1
            model: ListModel {
                id: imuAxismodel
                ListElement {text: "X"}
                ListElement {text: "Y"}
            }
            text: qsTr("IMU X or Y Axis")
        }

        ComboBoxCustomized {
			id: cboxSteerEnable
            Component.onCompleted: if((settings.setArdSteer_setting0 & 32) == 32)
                                       currentIndex = 1
                                   else if((settings.setArdSteer_setting0 & 64) == 64)
                                       currentIndex = 2
                                   else
                                       currentIndex = 0
            model: ListModel {
                //   id: steerEnablemodel
                ListElement {text: "None"}
                ListElement {text: "Switch"}
                ListElement {text: "Button"}
            }
            text: qsTr("Steer Enable")
            Text{
                anchors.top: steerEnable.bottom
                anchors.left: steerEnable.left
                text: qsTr("Button- Push On, Push Off
Switch- Pushed On, Release Off")
                font.pixelSize: 10
            }
        }
    }
}
