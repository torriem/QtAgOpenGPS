import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
Item {
    Rectangle{
        id: configTractor
        color: "light gray"
        border.color: "black"
        border.width: 8
        visible: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        height:parent.height
        Rectangle{
            id:topLine
            color:"gray"
            visible: true
            width: parent.width-16
            height: 40
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            Button {
                id: help
                text: qsTr("?")
                width: parent.height+3
                height:parent.height
                anchors.right: parent.right
            }
        }
        ColumnLayout{
            id:leftColumn
            anchors.top:topLine.bottom
            anchors.left: topLine.left
            width: childrenRect.width
            anchors.bottom: bottomLine.top
            visible: true
            IconButtonTransparent{
               id:vehicleMenu
               objectName: "btnVehicleMenu"
               icon: "qrc:/images/Config/Con_VehicleMenu.png"
               width:100
               height:75
            }
            IconButtonTransparent{
               id:implementMenu
               objectName: "btnImplementMenu"
               icon: "qrc:/images/Config/Con_ImplementMenu.png"
               width:100
               height:75
            }
            IconButtonTransparent{
               id:sourcesMenu
               objectName: "btnSourcesMenu"
               icon: "qrc:/images/Config/Con_SourcesMenu.png"
               width:100
               height:75
            }
            IconButtonTransparent{
               id:uTurnMenu
               objectName: "btnUTurnMenu"
               icon: "qrc:/images/Config/Con_UTurnMenu.png"
               width:100
               height:75
            }
            IconButtonTransparent{
               id:modulesMenu
               objectName: "btnModulesMenu"
               icon: "qrc:/images/Config/Con_ModulesMenu.png"
               width:100
               height:75
            }
            IconButtonTransparent{
               id:tramMenu
               objectName: "btnTramMenu"
               icon: "qrc:/images/Config/Con_TramMenu.png"
               width:100
               height:75
            }
            IconButtonTransparent{
               id:featureMenu
               objectName: "btnFeatureMenu"
               icon: "qrc:/images/Config/Con_FeatureMenu.png"
               width:100
               height:75
            }
        }

        Rectangle{
            id:bottomLine
            color:"light gray"
            visible: true
            width: parent.width - 16
            height:80
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8
            anchors.horizontalCenter: parent.horizontalCenter
            Text{
                id: vehicle
                text:qsTr("vehicle, units and tool width all not working")
                anchors.left: parent.left
                anchors.horizontalCenter: parent.horizontalCenter
                width:parent.width/2
                height: parent.height
            }
            IconButtonTransparent{
                id: saveAndClose
                anchors.right: parent.right
                width:parent.height
                height: parent.height
                icon: "qrc:/images/OK64"
                onClicked: config.visible = false
            }
        }
    }
}
