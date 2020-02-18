import QtQuick 2.0

Rectangle {
    id: rectangle
    width: childrenRect.width+10
    height: childrenRect.height+10
    color: "#bf163814"
    visible: true
    border.color: "#c3ecc0"
    radius: 20

    signal btnGPSDataBtnClicked

    Grid {
        id: grid
        width: childrenRect.width
        height: childrenRect.height
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
        spacing: 6
        flow: Grid.TopToBottom
        rows: 4
        columns: 3

        IconButton {
            id: btnSnap
            objectName: "btnSnap"
            icon: "qrc:/images/Snap64.png"
        }

        IconButtonText {
            id: btnTripOdometer
            objectName: "btnTripOdometer"
            buttonText: qsTr("Trip")
            icon: "qrc:/images/TripOdometer.png"
            iconHeightScale: 1.0 //don't shrink icon to make room for text
        }

        IconButtonText {
            id: btnGPSData
            objectName: "btnGPSData"
            buttonText: qsTr("GPS Data")
            icon: "qrc:/images/Satellite64.png"
            onClicked: {
                console.log("GPS Data button clicked  on QML....")
                btnGPSDataBtnClicked();
            }
        }

        IconButtonText {
            id: btnSettings
            objectName: "btnSettings"
            buttonText: qsTr("Settings")
            icon: "qrc:/images/Settings64.png"
        }

        IconButton {
            id: btnJob
            objectName: "btnJob"
            icon: "qrc:/images/JobActive.png"
        }

        IconButtonText {
            id: btnBoundaryMenu
            objectName: "btnBoundaryMenu"
            buttonText: qsTr("Boundary")
            icon: "qrc:/images/BoundaryMenu.png"
        }

        Rectangle {
            color: "transparent"
            width: 100
            height: 100

        }

        IconButtonText {
            id: btnComm
            objectName: "btnComm"
            buttonText: qsTr("USB Ports")
            icon: "qrc:/images/ComPorts.png"
        }

        IconButtonText {
            id: btnUnits
            objectName: "btnUnits"
            buttonText: qsTr("Units")
            icon: "qrc:/images/AcHa.png"
        }

        IconButtonText {
            id: btnFileExploer
            objectName: "btnFileExplorer"
            icon: "qrc:/images/FileExplorerWindows.png"
            buttonText: qsTr("Explorer")
        }

        Rectangle {
            color: "transparent"
            width: 100
            height: 100
        }

        IconButtonText {
            id: btnAutoSteer
            objectName: "btnAutoSteerConfig"
            buttonText: qsTr("Autosteer Config")
            icon: "qrc:/images/AutoSteerConfig.png"
        }
    }
}
