import QtQuick 2.0
import QtQuick.Controls 2.5

//todo: noticed % coverage was in wrong spot
Rectangle{
    id: configImplementSection
    anchors.fill: parent
    color: "ghostwhite"
    visible: false

    Row{
        id: bottomRow
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: bottomRightText.left
        anchors.margins: 10
        spacing: 90
        Button{
            function toggleZones(){
                if( settings.setTool_isSectionsNotZones){
                    image.source = "/images/Config/ConT_Symmetric.png"
                    settings.setTool_isSectionsNotZones = false
                    console.log("zones")
                }else{
                    image.source ="/images/Config/ConT_Asymmetric.png"
                    settings.setTool_isSectionsNotZones = true
                    console.log("sections")
                }
            }
            width: 180
            height: 130
            id: chooseZones
            objectName: "zonesOrSections"
            onClicked: {
                toggleZones()
            }
            background: Rectangle{
                color: "white"
                border.color: "black"
                border.width: 1
                Image{
                    id: image

                    source:"/images/Config/ConT_Asymmetric.png"
                    anchors.fill: parent
                }
            }
        }
        SpinBoxCustomized{
            id: percentCoverage
            objectName: "percentConverage"
            from: 0
            to: 100
            value: settings.setVehicle_minCoverage
            anchors.bottom: parent.bottom
            text: qsTr("% Coverage")
            onValueChanged: settings.setVehicle_minCoverage
        }
        IconButton{
            objectName: "boundaryOff"
            icon.source: "/images/SectionOffBoundary.png"
            iconChecked: "/images/SectionOnBoundary.png"
            anchors.bottom: parent.bottom
            implicitWidth: 100
            implicitHeight: 100
            border: 1
            color3: "white"
            colorChecked1: "green"
            colorChecked2: "green"
            colorChecked3: "green"
            checked: settings.setTool_isSectionOffWhenOut
            onCheckedChanged: settings.setTool_isSectionOffWhenOut = checked
        }
        SpinBoxOneDecimal{
            //todo: this should be made english/metric
            objectName: "speedBelowSectionOff"
            from: 0.0
            to: 30
            value: settings.setVehicle_slowSpeedCutoff
            anchors.bottom: parent.bottom
            onValueChanged: settings.setVehicle_slowSpeedCutoff = value
            Image{
                anchors.bottom: parent.top
                width: parent.width
                height: parent.height +30
                source: "/images/SectionOffBelow.png"
            }
        }
    }
    Text{
        id: bottomRightText
        anchors.right: parent.right
        text: qsTr("Km/H")
        anchors.verticalCenter: bottomRow.verticalCenter
        font.pixelSize: 25
    }
    ConfigImplementSectionsSection{
        id: configImplementSectionsSection
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: bottomRow.top
        anchors.bottomMargin: 30
        anchors.margins: 15
        visible: settings.setTool_isSectionsNotZones
    }
    ConfigImplementSectionsZones{
        id: configImplementSectionsZones
        anchors.top: parent.top
        anchors.topMargin: 80
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: bottomRow.top
        anchors.bottomMargin: 30
        anchors.margins: 15
        visible: !settings.setTool_isSectionsNotZones

    }
}
