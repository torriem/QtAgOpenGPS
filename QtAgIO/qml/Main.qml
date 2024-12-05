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
            if((!agio.isAndroid) && (!agio.isWindows))
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

        Column{
            id: latLonColumn
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.leftMargin: 10
            spacing: 3
            height: childrenRect.height
            width: childrenRect.width
            Text{
                text: qsTr("Lat: ") + (Number(agio.latitude).toLocaleString(Qt.locale(), 'f', 7))
            }
            Text{
                text: qsTr("Lon: ") + (Number(agio.longitude).toLocaleString(Qt.locale(), 'f', 7))
            }
        }
        Column{
            id: ntripColumn
            anchors.left: parent.left
            anchors.top: latLonColumn.bottom
            anchors.topMargin: 40
            anchors.leftMargin: 10
            spacing: 3
            height: children.height
            width: children.width
            Text{
                text: "kb"
                property int tripBytes: agio.tripBytes
                onTripBytesChanged: {
                    const kilobytes = tripBytes >> 10; // Shift right by 10 bits to convert to KB
                    text = kilobytes.toLocaleString() + " kb"; // Format with commas and add "kb"
                }

                }
            Text{
                text: (settings.setNTRIP_isOn === false ? "Off":
                    agio.ntripStatus === 0 ? "Invalid" :
                    agio.ntripStatus === 1 ? "Authorizing" :
                    agio.ntripStatus === 2 ? "Waiting" :
                    agio.ntripStatus === 3 ? "Send GGA" :
                    agio.ntripStatus === 4 ? "Listening NTRIP":
                    agio.ntripStatus === 5 ? "Wait GPS":
                    "Unknown")
            }

           Button {
               property int ntripCounter: agio.ntripCounter
               text: ""
               onNtripCounterChanged: {
                    if (ntripCounter > 59) text = (ntripCounter >> 6) + " Min";
                    else if (ntripCounter < 60 && ntripCounter > 25) text = ntripCounter + " Secs";
                    else text = "In " + (Math.abs(ntripCounter - 25)) + " secs";
               }
            }
        }

        Column {
            id: commStatus
            anchors.top: ntripColumn.bottom
            anchors.left: parent.left
            anchors.bottom: bottomButtons.top
            anchors.bottomMargin: 5 * theme.scaleHeight
            anchors.topMargin: 15 * theme.scaleHeight
            anchors.leftMargin: 5 * theme.scaleWidth
            Comp.IconButtonColor {
                id: btnEthernetStatus
                text: qsTr("Ethernet")
                icon.source: imagePath + "B_UDP.png"
                color: agio.ethernetConnected ? "green" : "red"
                onClicked: ethernetConfig.visible = !ethernetConfig.visible
            }
            Comp.IconButtonTransparent {
                icon.source: prefix + "/images/Nmea.png"
                width: btnEthernetStatus.width
                height: btnEthernetStatus.height
                visible: true
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
                onClicked: mainWindow.showMinimized()
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
    CloseAgIO{
        id: closeAgIO
        visible: false
    }
}
