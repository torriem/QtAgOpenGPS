import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle{
    id: configTractor
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
        IconButtonColor{
            id: harvester
            width:150
            height:100
            icon: "qrc:/images/vehiclePageHarvester.png"
            isChecked: false
        }
        IconButtonColor{
            width:150
            height:100
            id: tractor
            icon: "qrc:/images/vehiclePageTractor.png"
            isChecked: false
        }
        IconButtonColor{
            width:150
            height:100
            id: tr4wd
            icon: "qrc:/images/vehiclePage4WD.png"
            isChecked: false
        }
    }
    Text {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("I consider the rest of this page nonessential for now and will be added at a later date")
    }
}
