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
                if( utils.isTrue(settings.setTool_isSectionsNotZones)){
                    settings.setTool_isSectionsNotZones = false
                }else{
                    settings.setTool_isSectionsNotZones = true
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

                    source: utils.isTrue(settings.setTool_isSectionsNotZones) ? "/images/Config/ConT_Asymmetric.png" : "/images/Config/ConT_Symmetric.png"
                    anchors.fill: parent
                }
            }
        }
        SpinBoxCustomized{
            id: percentCoverage
            objectName: "percentConverage"
            from: 0
            to: 100
            value: Number(settings.setVehicle_minCoverage)
            anchors.bottom: parent.bottom
            text: qsTr("% Coverage")
            onValueModified: settings.setVehicle_minCoverage = value
            Connections {
                target: settings
                function onSetVehicle_minCoverageChanged() {
                    percentCoverage.setValue(Number(settings.setVehicle_minCoverage))
                }
            }
        }
        IconButton{
            objectName: "boundaryOff"
            icon.source: "/images/SectionOffBoundary.png"
            iconChecked: "/images/SectionOnBoundary.png"
            anchors.bottom: parent.bottom
            implicitWidth: 100
            implicitHeight: 100
            border: 1
            radius: 0
            color3: "white"
            colorChecked1: "green"
            colorChecked2: "green"
            colorChecked3: "green"
            checked: utils.isTrue(settings.setTool_isSectionOffWhenOut)
            onCheckedChanged: settings.setTool_isSectionOffWhenOut = checked
            Connections {
                target: settings
                function onSetTool_isSectionOffWhenOutChanged() {
                    checked = utils.isTrue(settings.setTool_isSectionOffWhenOut)
                }
            }
        }
        SpinBoxOneDecimal{
            //todo: this should be made english/metric
            id: slowSpeedCutoff
            from: utils.speed_to_unit(0)
            to: utils.speed_to_unit(30)
            value: utils.speed_to_unit(Number(settings.setVehicle_slowSpeedCutoff))
            anchors.bottom: parent.bottom
            onValueModified: settings.setVehicle_slowSpeedCutoff = utils.speed_from_unit(value)
            text: utils.speed_unit()

            Connections {
                target: settings
                function onSetVehicle_slowSpeedCutoffChanged() {
                    slowSpeedCutoff.setValue(utils.speed_to_unit(Number(settings.setVehicle_slowSpeedCutoff)))
                }
            }

            Image{
                anchors.bottom: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                source: "/images/SectionOffBelow.png"
            }
        }
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
        visible: utils.isTrue(settings.setTool_isSectionsNotZones)
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
        visible: !utils.isTrue(settings.setTool_isSectionsNotZones)

    }
}
