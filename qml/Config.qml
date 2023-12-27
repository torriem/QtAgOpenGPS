import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4
import Qt.labs.folderlistmodel 2.2

Item {
    Rectangle{
        id: configMain
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
                icon.source: "qrc:/images/Config/Con_VehicleMenu.png"
                width:100
                height:75
                function toggle_configTractor(){
                    if(configTractor.visible == true){
                        configTractor.visible = false
                    }else{
                        configTractor.visible = true

                    }
                }

                onClicked: toggle_configTractor()
            }
            IconButtonTransparent{
                id:implementMenu
                objectName: "btnImplementMenu"
                icon.source: "qrc:/images/Config/Con_ImplementMenu.png"
                width:100
                height:75
                function toggle_configImplement(){
                    if(configImplement.visible == true){
                        configImplement.visible = false
                    }else{
                        configImplement.visible = true
                    }
                }

                onClicked: toggle_configImplement()
            }
            IconButtonTransparent{
                id:sourcesMenu
                objectName: "btnSourcesMenu"
                icon.source: "qrc:/images/Config/Con_SourcesMenu.png"
                width:100
                function toggle_configSources(){
                    if(configSources.visible == true){
                        configSources.visible = false
                    }else{
                        configSources.visible = true
                    }
                }

                onClicked: toggle_configSources()
                height:75
            }
            IconButtonTransparent{
                id:uTurnMenu
                objectName: "btnUTurnMenu"
                icon.source: "qrc:/images/Config/Con_UTurnMenu.png"
                width:100
                height:75
                function toggle_configYouTurn(){
                    if(configYouTurn.visible == true){
                        configYouTurn.visible = false
                    }else{
                        configYouTurn.visible = true
                    }
                }

                onClicked: toggle_configYouTurn()
            }
            IconButtonTransparent{
                id:modulesMenu
                objectName: "btnModulesMenu"
                icon.source: "qrc:/images/Config/Con_ModulesMenu.png"
                width:100
                height:75
                function toggle_configModules(){
                    if(configModules.visible == true){
                        configModules.visible = false
                    }else{
                        configModules.visible = true
                    }
                }

                onClicked: toggle_configModules()
            }
            IconButtonTransparent{
                id:tramMenu
                objectName: "btnTramMenu"
                icon.source: "qrc:/images/Config/Con_TramMenu.png"
                width:100
                height:75
                function toggle_configTrams(){
                    if(configTrams.visible == true){
                        configTrams.visible = false
                    }else{
                        configTrams.visible = true
                    }
                }

                onClicked: toggle_configTrams()
            }
            IconButtonTransparent{
                id:featureMenu
                objectName: "btnFeatureMenu"
                icon.source: "qrc:/images/Config/Con_FeatureMenu.png"
                width:100
                height:75
                function toggle_configFeatures(){
                    if(configFeatures.visible == true){
                        configFeatures.visible = false
                    }else{
                        configFeatures.visible = true
                    }
                }

                onClicked: toggle_configFeatures()
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
            z: 3
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
                icon.source: "qrc:/images/OK64"
                onClicked: config.visible = false
            }
        }
        Rectangle{
            id: mainConfig
            anchors.top: topLine.bottom
            anchors.right: parent.right
            anchors.left: leftColumn.right
            anchors.bottom: bottomLine.top
            anchors.rightMargin: 8
            color: "ghostwhite"
            border.color: "lime"
        }
        ConfigWhichVehicle{
            id: configWhichVehicle
            anchors.fill: mainConfig
            anchors.margins: 1
            visible: true
        }

        ConfigTractor{
            id:configTractor
            anchors.fill: mainConfig
            anchors.margins:1
            visible: false
        }
        ConfigImplement{
            id:configImplement
            anchors.fill: mainConfig
            anchors.margins:1
            visible: false
        }
        ConfigSources{
            id:configSources
            anchors.fill: mainConfig
            anchors.margins:1
            visible: false
        }
        ConfigYouTurn{
            id:configYouTurn
            anchors.fill: mainConfig
            anchors.margins:1
            visible: false
        }
        ConfigModules{
            id:configModules
            anchors.fill: mainConfig
            anchors.margins:1
            visible: false
        }
        ConfigTrams{
            id:configTrams
            anchors.fill: mainConfig
            anchors.margins:1
            visible: false
        }
        ConfigFeatures{
            id:configFeatures
            anchors.fill: mainConfig
            anchors.margins:1
            visible: false
        }
    }
}
