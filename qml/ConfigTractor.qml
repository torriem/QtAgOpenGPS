import QtQuick 2.0

Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent
        visible: true
        color: "ghostwhite"
        Text {
            id: text
            text: qsTr("Choose Vehicle Type")
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Row{
            anchors.top: text.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            Rectangle{
                width:150
                height:100
                radius:5
                id: harvester
                IconButtonTransparent{
                    icon: "qrc:/images/vehiclePageHarvester.png"
                    anchors.fill: parent
                    colorHover1: "transparent"
                    checkable: true
                    onClicked: {
                        harvester.color = "green"
                        tractor.color = "white"
                        tr4wd.color = "white"
                    }
                }
            }
            Rectangle{
                width:150
                height:100
                radius:5
                id: tractor
                IconButtonTransparent{
                    anchors.fill:parent
                    icon: "qrc:/images/vehiclePageTractor.png"
                    colorHover1: "transparent"
                    checkable: true
                    onClicked: {
                        harvester.color = "white"
                        tractor.color = "green"
                        tr4wd.color = "white"
                    }
                }
            }
            Rectangle{
                width:150
                height:100
                radius:5
                id: tr4wd
                IconButtonTransparent{
                    anchors.fill: parent
                    icon: "qrc:/images/vehiclePage4WD.png"
                    colorHover1: "transparent"
                    checkable: true
                    onClicked: {
                        harvester.color = "white"
                        tractor.color = "white"
                        tr4wd.color = "green"
                    }
                }
            }
        }
            Text {
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("I consider the rest of this page nonessential for now and will be added at a later date")
            }
    }
}
