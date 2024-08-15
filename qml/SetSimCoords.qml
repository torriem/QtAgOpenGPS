import QtQuick 2.0
import QtQuick.Layouts 1.3
import "components"

Rectangle{
    id: setSimCoordsRoot
    anchors.fill: mainWindow
    color: aog.backgroundColor
    visible: false
    function show(){
        setSimCoordsRoot.visible = true
        console.log("showing in setsim")
    }
    TextLine{
        id: textSimCoords
        anchors.top: parent.top
        anchors.margins: 10
        font.pixelSize: 30
        text: qsTr("Set Sim Coords")
    }

    Image { // map image
        id: setSimCoordsImage
        source: "/images/LatLon.png"
        anchors.top: textSimCoords.bottom
        anchors.left: parent.left
        width: parent.width *.9
        fillMode: Image.PreserveAspectFit
        Rectangle{ //lat text
            color: setSimCoordsRoot.color
            anchors.left: parent.left
            anchors.leftMargin: 30
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height *.6
            width: children.width
            TextLine{
                anchors.centerIn: parent
                text: qsTr("Latitude")
                font.pixelSize: 30
                font.bold: true
                rotation: -90
            }
        }
        Rectangle{// longitude text
            color: setSimCoordsRoot.color
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width *.45
            height: children.height
            TextLine{
                text: qsTr("Longituge")
                anchors.centerIn: parent
                font.pixelSize: 30
                font.bold: true
            }
        }
    }
    Item{
        id: textInputField
        anchors.left: parent.left
        anchors.right: buttons.left
        anchors.bottom: parent.bottom
        anchors.margins: 10
        TextLine{
            text: qsTr("Latitude")
            anchors.bottom: latFromTo.top
            anchors.horizontalCenter: latInput.horizontalCenter
        }
        TextLine{
            id: latFromTo
            text: qsTr("From -180 to +180")
            anchors.bottom: latInput.top
            anchors.horizontalCenter: latInput.horizontalCenter
        }

        LatLonTextField{
            id: latInput
            width: textInputField*.45
            height: 50
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            //placeholderText: settings.setGPS_simLatitude
        }
        TextLine{
            text: qsTr("Longitude")
            anchors.bottom: lonFromTo.top
            anchors.horizontalCenter: lonInput.horizontalCenter
        }
        TextLine{
            id: lonFromTo
            text: qsTr("From -180 to +180")
            anchors.bottom: lonInput.top
            anchors.horizontalCenter: lonInput.horizontalCenter
        }
        LatLonTextField{
            id: lonInput
            width: textInputField.width *.45
            height: 50
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            //placeholderText: settings.setGPS_simLongitude
        }
    }

    RowLayout{
        id: buttons
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 10
        height: children.height
        width: parent.width *.3
        IconButtonTransparent{
            icon.source: "/images/Cancel64.png"
            onClicked: setSimCoordsRoot.visible = false
        }
        IconButtonTransparent{
            icon.source: "/images/OK64.png"
            onClicked: {
                settings.setGPS_simLatitude = latInput.text
                settings.setGPS_simLongitude = lonInput.text
                setSimCoordsRoot.visible = false
            }
        }
    }
}
