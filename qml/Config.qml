import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4
import QtQuick.Dialogs 1.3
import Qt.labs.folderlistmodel 2.2

Dialog {
    height: 768
    width:1024
    modality: Qt.WindowModal
    standardButtons: StandardButton.NoButton
    title: qsTr("General Settings")


    function closeAllConfig(){
        configWhichVehicle.visible = false
        configTractor.visible = false
        configTractorAntenna.visible = false
        configTractorDimensions.visible = false
        configTractorSteerSettings.visible = false
        configImplement.visible = false
        configImplementDimensions.visible = false
        configImplementSections.visible = false
        configImplementSwitches.visible = false
        configImplementTiming.visible = false
        configSources.visible = false
        configSourcesRoll.visible = false
		configModules.visible = false
        configModulesPins.visible = false
		configYouTurn.visible = false
		configTrams.visible = false
		configFeatures.visible = false
		tractorMenu.visible = false
		implementMenu.visible = false
        sourcesMenu.visible = false
        modulesMenu.visible = false
	}

	Rectangle{
		id: configMain
		color: "light gray"
		border.color: "black"
		border.width: 8
		visible: true
        anchors.fill: parent
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

        ScrollView {
            id: leftColumnView
            anchors.top: topLine.bottom
            anchors.left: topLine.left
            width: 110
            anchors.bottom: configMain.bottom

            //ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            clip: true
            ScrollBar.vertical  : ScrollBar {
                policy: ScrollBar.AlwaysOn
                anchors.rightMargin: 10
                contentItem: Rectangle {
                    color: "red"
                }
            }

		Column{
            id:leftColumn
            anchors.fill: parent
            //anchors.top:topLine.bottom
            //anchors.left: topLine.left
            //width: childrenRect.width
			visible: true
            spacing: 15
			IconButtonTransparent{
				id:vehicleMenu
				objectName: "btnVehicleMenu"
				icon.source: "/images/Config/Con_VehicleMenu.png"
				width:100
				height:75

				function toggle_configTractor(){
					if(tractorMenu.visible == true){
						closeAllConfig()
						configWhichVehicle.visible = true
						tractorMenu.visible = false
					}else{
						closeAllConfig()
						configTractor.visible = true
						tractorMenu.visible = true
					}
				}
				onClicked: {
					toggle_configTractor()
				}
			}
			Column{
				id: tractorMenu
				visible: false
				height: children.height
				IconButton{
					id: configTrSettings
					icon.source: "/images/Config/ConS_VehicleConfig.png"
					onClicked:{
						closeAllConfig()
						configTractor.visible = true
						tractorMenu.visible = true
					}
				}
				IconButton{
					id: configTrDim
					icon.source: "/images/Config/ConS_ImplementHitch.png"
					onClicked: {
						closeAllConfig()
						configTractorDimensions.visible = true
						tractorMenu.visible = true
					}
				}
				IconButton{
					id: btnconfigTrAntDim
					icon.source: "/images/Config/ConS_ImplementAntenna.png"
					onClicked: {
						closeAllConfig()
						configTractorAntenna.visible = true
						tractorMenu.visible = true
					}
				}
				IconButton{
					id: btnconfigTrSteerSett
					icon.source: "/images/Config/ConS_ModulesSteer.png"

					onClicked: {
						closeAllConfig()
						configTractorSteerSettings.visible = true
						tractorMenu.visible = true
					}
				}
			}
			IconButtonTransparent{
				id:implementMenuBtn
				objectName: "btnImplementMenu"
				icon.source: "/images/Config/Con_ImplementMenu.png"
				width:100
				height:75
				onClicked:{
					closeAllConfig()
					implementMenu.visible = true
					configImplement.visible = true
				}
			}
			Column{
				id: implementMenu
				visible: false
				height: children.height
				IconButton{
					id: configImpSettings
					icon.source: "/images/Config/ConS_VehicleConfig.png"
					onClicked: {
						closeAllConfig()
						configImplement.visible = true
						implementMenu.visible = true
					}
				}
				IconButton{
					id: configImpDim
					icon.source: "/images/Config/ConS_ImplementHitch.png"
					onClicked:{
						closeAllConfig();
						configImplementDimensions.visible = true
						implementMenu.visible = true
					}
				}
				IconButton{
					id: configImpSection
					icon.source: "/images/Config/ConS_ImplementSection.png"
					onClicked:{
						closeAllConfig();
                        configImplementSections.visible = true
						implementMenu.visible = true
					}
				}
				IconButton{
					id: configImpTiming
					icon.source: "/images/Config/ConS_ImplementSettings.png"
					onClicked:{
						closeAllConfig();
						configImplementTiming.visible = true
						implementMenu.visible = true
					}

				}
				IconButton{
					id: configImpSwitches
					icon.source: "/images/Config/ConS_ImplementSwitch.png"
					onClicked:{
						closeAllConfig();
						configImplementSwitches.visible = true
						implementMenu.visible = true
					}

				}
			}
			IconButtonTransparent{
                id:sourcesMenubtn
				objectName: "btnSourcesMenu"
				icon.source: "/images/Config/Con_SourcesMenu.png"
				width:100
				onClicked:{
					closeAllConfig()
					configSources.visible = true
                    sourcesMenu.visible = true
				}
				height:75
			}
            Column{
                id: sourcesMenu
                visible: false
                height: children.height
                IconButton{
                    id: configsrcHeading
                    icon.source: "/images/Config/ConS_VehicleConfig.png"
                    onClicked:{
                        closeAllConfig()
                        sourcesMenu.visible = true
                        configSources = true
                    }
                }
                IconButton{
                    id: configsrcRoll
                    icon.source: "/images/Config/ConS_ImplementHitch.png"
                    onClicked: {
                        closeAllConfig()
                        sourcesMenu.visible = true
                        configSourcesRoll.visible = true
                    }
                }
            }
            IconButtonTransparent{
				id:uTurnMenu
				objectName: "btnUTurnMenu"
				icon.source: "/images/Config/Con_UTurnMenu.png"
				width:100
				height:75
				onClicked:{
					closeAllConfig()
					configYouTurn.visible = true
				}
			}
			IconButtonTransparent{
                id:modulesMenubtn
				objectName: "btnModulesMenu"
				icon.source: "/images/Config/Con_ModulesMenu.png"
				width:100
				height:75
				onClicked:{
					closeAllConfig()
					configModules.visible = true
                    modulesMenu.visible = true
				}
			}
            Column{
                id: modulesMenu
                visible: false
                height: children.height
                IconButton{
                    id: configModulesSettings
                    icon.source: "/images/Config/ConS_ModulesMachine.png"
                    onClicked:{
                        closeAllConfig()
                        modulesMenu.visible = true
                        configModules.visible = true
                        console.log("btnclicked")
                    }
                }
                IconButton{
                    id: configModulesPinsbtn
                    icon.source: "/images/Config/ConS_ImplementSection.png"
                    onClicked:{
                        closeAllConfig()
                        modulesMenu.visible = true
                        configModulesPins.visible = true
                    }
                }
            }
            IconButtonTransparent{
				id:tramMenu
				objectName: "btnTramMenu"
				icon.source: "/images/Config/Con_TramMenu.png"
				width:100
				height:75
				onClicked: {
					closeAllConfig()
					configTrams.visible = true
				}
			}
			IconButtonTransparent{
				id:featureMenu
				objectName: "btnFeatureMenu"
				icon.source: "/images/Config/Con_FeatureMenu.png"
				width:100
				height:75
				onClicked:{
					closeAllConfig()
					configFeatures.visible = true
				}
			}
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
				icon.source: "/images/OK64"
                onClicked: accept()
			}
		}
		Rectangle{
			id: mainConfig
			anchors.top: topLine.bottom
			anchors.right: parent.right
            anchors.left: leftColumnView.right
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
		ConfigTractorAntenna{
			id:configTractorAntenna
			anchors.fill: mainConfig
			anchors.margins:1
			visible: false
		}
		ConfigTractorDimensions{
			id:configTractorDimensions
			anchors.fill: mainConfig
			anchors.margins:1
			visible: false
		}
		ConfigTractorSteerSettings{
			id:configTractorSteerSettings
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
		ConfigImplementDimensions{
			id:configImplementDimensions
			anchors.fill: mainConfig
			anchors.margins:1
			visible: false
		}
		ConfigImplementSections{
			id:configImplementSections
			anchors.fill: mainConfig
			anchors.margins:1
			visible: false
		}
		ConfigImplementTiming{
			id:configImplementTiming
			anchors.fill: mainConfig
			anchors.margins:1
			visible: false
		}
		ConfigImplementSwitches{
			id:configImplementSwitches
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
        ConfigSourcesRoll{
            id:configSourcesRoll
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
        ConfigModulesPins{
            id:configModulesPins
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
