import QtQuick
import QtQuick.Window
import QtQuick.Controls.Fusion
import "components" as Comp

Window {

    property string imagePath: prefix + "/images/"

	AgIOTheme {
		id: theme
		objectName: "theme"
	}
	AgIOInterface {
		id: agio
		objectName: "agio"
	}

    UnitConversion {
        id: utils
    }

	id: mainWindow
    //height: theme.defaultHeight
    //width: theme.defaultWidth
    height: 500
    width: 300
    visible: true
    onVisibleChanged: console.log("Visible!")

        //This is a popup message that dismisses itself after a timeout
    Comp.TimedMessage {
        id: timedMessage
        objectName: "timedMessage"
    }
    Component.onCompleted: {//shows a window to warn us we might need to open up a port on the firewall
        if(utils.isTrue(settings.run_isFirstRun)){
                               isFirewall.show()
                               settings.run_isFirstRun = false
                           }
    }

	Rectangle {
		id: background
		 color: theme.backgroundColor
		 anchors.fill: parent
		 border.width: 2
		 border.color: theme.blackDayWhiteNight
		 Column {
			 id: commStatus
			 anchors.top: parent.top
			 anchors.left: parent.left
			 anchors.bottom: bottomButtons.top
			 anchors.bottomMargin: 5 * theme.scaleHeight
			 anchors.topMargin: 5 * theme.scaleHeight
			 anchors.leftMargin: 5 * theme.scaleWidth
             Comp.IconButtonColor {
				 id: btnAOGSStatus
				 text: qsTr("AOG")
				 icon.source: imagePath + "AgIOBtn.png"
				 color: agio.aogConnected ? "green" : "red"
			 }
             Comp.IconButtonColor {
				 id: btnEthernetStatus
				 text: qsTr("Ethernet")
				 icon.source: imagePath + "B_UDP.png"
				 color: agio.ethernetConnected ? "green" : "red"
                 onClicked: ethernetConfig.visible = !ethernetConfig.visible
			 }
             Comp.IconButtonColor {
				 id: btnNTRIPStatus
				 text: qsTr("NTRIP")
                 //Oicon.source: "../../images/qtSpecific/NTRIP_Client.png"//qtAOG specific image--not in AOG
                 icon.source: imagePath + "AgIOBtn.png"
                 color: agio.ntripConnected ? "green" : "red"
			 }
             Comp.IconButtonTransparent {
                 icon.source: prefix + "/images/Nmea.png"
                 visible: agio.gpsConnected
                 onClicked: gpsInfo.visible = !gpsInfo.visible
             }
		 }
		 Column {
			 id: moduleStatus
			 anchors.top: parent.top
			 anchors.left: commStatus.right
			 anchors.bottom: bottomButtons.top
			 anchors.bottomMargin: 5 * theme.scaleHeight
			 anchors.topMargin: 5 * theme.scaleHeight
			 anchors.leftMargin: 5 * theme.scaleWidth
             Comp.IconButtonColor {
				 id: btnModuleIMU
				 text: qsTr("IMU")
				 icon.source: imagePath + "B_IMU.png"
				 color: agio.imuConnected ? "green" : "red"
			 }
             Comp.IconButtonColor {
				 id: btnModuleSteer
				 text: qsTr("Steer")
				 icon.source: imagePath + "Com_AutosteerModule.png"
				 color: agio.steerConnected ? "green" : "red"
			 }
             Comp.IconButtonColor {
				 id: btnModuleGPS
				 text: qsTr("GPS")
				 icon.source: imagePath + "B_GPS.png"
				 color: agio.gpsConnected ? "green" : "red"
			 }
             Comp.IconButtonColor {
				 id: btnModuleMachine
				 text: qsTr("Machine")
				 icon.source: imagePath + "B_Machine.png"
				 color: agio.machineConnected ? "green" : "red"
			 }
			 
		 }
		 Row {
			 id: bottomButtons
			 anchors.bottom: parent.bottom
			 anchors.left: parent.left
			 width: parent.width - 10 * theme.scaleWidth 
			 anchors.leftMargin: 5 * theme.scaleWidth
			 anchors.bottomMargin: 5 * theme.scaleHeight
			 spacing: (width - (btnHide.implicitWidth * 3)) / 2 //fill the parent up with equal space
             Comp.IconButtonText {
				 id: btnSettings
				 implicitWidth: btnNTRIP.width
				 implicitHeight: btnNTRIP.height
				 text: qsTr("Settings")
				 icon.source: imagePath + "Settings48.png"
                 onClicked: settingsWindow.visible = true
			 }
             Comp.IconButton {
				 id: btnNTRIP
				 icon.source: imagePath + "NtripSettings.png"
                 onClicked: ntrip.visible = !ntrip.visible
			 }
             Comp.IconButtonText {
				 id: btnHide
				 implicitWidth: btnNTRIP.width
				 implicitHeight: btnNTRIP.height
				 text: qsTr("Hide")
				 icon.source: imagePath + "AgIOBtn.png"
				 onClicked: mainWindow.visible = false
			 }
		 }
	}

    /************** - Put all child windows here - ********************/
    Comp.Message {
		id: message
	}
    NTrip{
        id: ntrip
        visible: false
    }
	EthernetConfig {
		id: ethernetConfig
		visible: false
        onVisibleChanged: {
            if(visible)
                ethernetConfig.load_settings()
        }
	}
    GPSInfo {
        id: gpsInfo
        visible: false
    }
    AgDiag {
        id: agdiag
    }

    SettingsWindow{
        id: settingsWindow
        visible: false
    }
    AdvancedMenu{
        id: advancedMenu
        function showMenu(){
            advancedMenu.visible = true
        }
    }
    Window{//warn that we will need to open the firewall if we are using Linux
        //I've forgotten this several times--a real pain.
        modality: Qt.WindowModal
        id: isFirewall
        visible: false
        width: 200
        height: 200
        title: qsTr("Firewall")

        Text{
            anchors.centerIn: parent
            text: qsTr("We noticed this is your first
time running AgIO. Do you have
a firewall enabled? If you do,
 you must allow AgIO through.")
        }
    }
}
