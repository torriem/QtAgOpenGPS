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

    function closeAll() {
        //uncheck all the buttons maybe


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
                ButtonGroup {
                    buttons: [ vehicleMenu, implementMenuBtn, sourcesMenubtn, uTurnMenu,  modulesMenubtn, tramMenu, displayMenu, featureMenu ]
                }
                IconButtonTransparent{
                    id:vehicleMenu
                    objectName: "btnVehicleMenu"
                    icon.source: "/images/Config/Con_VehicleMenu.png"
                    width:100
                    height:75

                    //colorChecked1: "#aaaaaa"
                    //colorChecked2: "#aaaaaa"
                    //colorChecked3: "#777777"
                    checkable: true
                    radius: 0

                    //todo put logic in IconButton
                    property bool toggle: false
                    onClicked: if (toggle) { checked = false ; toggle = false }
                    onPressed: if (checked) toggle=true

                }
                Column{
                    x: 25
                    id: tractorMenu
                    visible: vehicleMenu.checked
                    height: children.height

                    ButtonGroup {
                        buttons: [configTrSettings, configTrDim, btnconfigTrAntDim, btnconfigTrSteerSett ]
                    }

                    IconButtonTransparent{
                        id: configTrSettings
                        icon.source: "/images/Config/ConS_VehicleConfig.png"
                        checkable: true
                        radius: 0
                    }
                    IconButtonTransparent{
                        id: configTrDim
                        icon.source: "/images/Config/ConS_ImplementHitch.png"
                        checkable: true
                        radius: 0
                    }
                    IconButtonTransparent{
                        id: btnconfigTrAntDim
                        icon.source: "/images/Config/ConS_ImplementAntenna.png"
                        checkable: true
                        radius: 0
                    }
                    IconButtonTransparent{
                        id: btnconfigTrSteerSett
                        icon.source: "/images/Config/ConS_ModulesSteer.png"
                        checkable: true
                        radius: 0
                    }
                }
                IconButtonTransparent{
                    id:implementMenuBtn
                    objectName: "btnImplementMenu"
                    icon.source: "/images/Config/Con_ImplementMenu.png"
                    width:100
                    height:75
                    radius: 0
                    checkable: true
                    //todo put logic in IconButton
                    property bool toggle: false
                    onClicked: if (toggle) { checked = false ; toggle = false }
                    onPressed: if (checked) toggle=true
                }
                Column{
                    id: implementMenu
                    visible: implementMenuBtn.checked
                    height: children.height
                    x: 25

                    ButtonGroup {
                        buttons: [ configImpSettings, configImpDim, configImpSection, configImpTiming, configImpSwitches ]
                    }
                    IconButton{
                        id: configImpSettings
                        icon.source: "/images/Config/ConS_VehicleConfig.png"
                        checkable: true
                    }
                    IconButton{
                        id: configImpDim
                        icon.source: "/images/Config/ConS_ImplementHitch.png"
                        checkable: true
                    }
                    IconButton{
                        id: configImpSection
                        icon.source: "/images/Config/ConS_ImplementSection.png"
                        checkable: true
                    }
                    IconButton{
                        id: configImpTiming
                        icon.source: "/images/Config/ConS_ImplementSettings.png"
                        checkable: true
                    }
                    IconButton{
                        id: configImpSwitches
                        icon.source: "/images/Config/ConS_ImplementSwitch.png"
                        checkable: true
                    }
                }
                IconButtonTransparent{
                    id:sourcesMenubtn
                    objectName: "btnSourcesMenu"
                    icon.source: "/images/Config/Con_SourcesMenu.png"
                    width:100
                    radius: 0
                    checkable: true
                    height:75
                    //todo put logic in IconButton
                    property bool toggle: false
                    onClicked: if (toggle) { checked = false ; toggle = false }
                    onPressed: if (checked) toggle=true
                }
                ButtonGroup {
                    buttons: [ sourcesMenu, configsrcRoll ]
                }
                Column{
                    id: sourcesMenu
                    visible: sourcesMenubtn.checked
                    height: children.height
                    IconButton{
                        id: configsrcHeading
                        checkable: true
                        icon.source: "/images/Config/ConS_VehicleConfig.png"
                    }
                    IconButton{
                        id: configsrcRoll
                        checkable: true
                        icon.source: "/images/Config/ConS_ImplementHitch.png"
                    }
                }
                IconButtonTransparent{
                    id:uTurnMenu
                    objectName: "btnUTurnMenu"
                    icon.source: "/images/Config/Con_UTurnMenu.png"
                    width:100
                    height:75
                    radius: 0
                    checkable: true
                    //todo put logic in IconButton
                    property bool toggle: false
                    onClicked: if (toggle) { checked = false ; toggle = false }
                    onPressed: if (checked) toggle=true
                }
                IconButtonTransparent{
                    id:modulesMenubtn
                    objectName: "btnModulesMenu"
                    icon.source: "/images/Config/Con_ModulesMenu.png"
                    width:100
                    height:75
                    radius: 0
                    checkable: true
                    //todo put logic in IconButton
                    property bool toggle: false
                    onClicked: if (toggle) { checked = false ; toggle = false }
                    onPressed: if (checked) toggle=true
                }
                ButtonGroup {
                    buttons: [ configModulesSettings, configModulesPinsbtn ]
                }
                Column{
                    id: modulesMenu
                    visible: modulesMenubtn.checked
                    height: children.height
                    IconButton{
                        id: configModulesSettings
                        checkable: true
                        icon.source: "/images/Config/ConS_ModulesMachine.png"
                    }
                    IconButton{
                        id: configModulesPinsbtn
                        checkable: true
                        icon.source: "/images/Config/ConS_ImplementSection.png"
                    }
                }
                IconButtonTransparent{
                    id:tramMenu
                    objectName: "btnTramMenu"
                    icon.source: "/images/Config/Con_TramMenu.png"
                    checkable: true
                    width:100
                    height:75
                    radius: 0
                    //todo put logic in IconButton
                    property bool toggle: false
                    onClicked: if (toggle) { checked = false ; toggle = false }
                    onPressed: if (checked) toggle=true
                }
                IconButtonTransparent{
                    id:displayMenu
                    icon.source: "/images/Config/Con_Display.png"
                    checkable: true
                    width:100
                    height:75
                    radius: 0
                    //todo put logic in IconButton
                    property bool toggle: false
                    onClicked: if (toggle) { checked = false ; toggle = false }
                    onPressed: if (checked) toggle=true
                }
                IconButtonTransparent{
                    id:featureMenu
                    objectName: "btnFeatureMenu"
                    icon.source: "/images/Config/Con_FeatureMenu.png"
                    width:100
                    height:75
                    radius: 0
                    checkable: true
                    //todo put logic in IconButton
                    property bool toggle: false
                    onClicked: if (toggle) { checked = false ; toggle = false }
                    onPressed: if (checked) toggle=true
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
                onClicked: {
                    //closeAllConfig()
                    //fix up signs on implement dimensions

                    //TODO, have to do this if rejected() also.

                    if ((utils.isTrue(settings.setTool_isToolFront) && Number(settings.setVehicle_hitchLength < 0)) ||
                            (!utils.isTrue(settings.setTool_isToolFront) && Number(settings.setVehicle_hitchLength) > 0)) {
                        //if front-mounted tool, make sure the hitchLength is positive and if rear-mounted, make sure
                        //hitchLength is negative
                        settings.setVehicle_hitchLength = -Number(settings.setVehicle_hitchLength)
                        //console.debug("corrected sign on hitchLength")
                    }

                    accept()
                }
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
            /*
            visible: ! tractorMenu.visible &&
                     ! implementMenu.visible &&
                     ! sourcesMenu.visible &&
                     ! uTurnMenu.visible &&
                     ! modulesMenu.visible &&
                     ! tramMenu.visible &&
                     ! configFeatures.visible
                     */
        }

        ConfigTractor{
            id:configTractor
            anchors.fill: mainConfig
            anchors.margins:1
            visible: tractorMenu.visible && configTrSettings.checked
        }
        ConfigTractorAntenna{
            id:configTractorAntenna
            anchors.fill: mainConfig
            anchors.margins:1
            visible: tractorMenu.visible && configTrDim.checked
        }
        ConfigTractorDimensions{
            id:configTractorDimensions
            anchors.fill: mainConfig
            anchors.margins:1
            visible: tractorMenu.visible && btnconfigTrAntDim.checked
        }
        ConfigTractorSteerSettings{
            id:configTractorSteerSettings
            anchors.fill: mainConfig
            anchors.margins:1
            visible: tractorMenu.visible && btnconfigTrSteerSett.checked
        }
        ConfigImplement{
            id:configImplement
            anchors.fill: mainConfig
            anchors.margins:1
            visible: implementMenu.visible && configImpSettings.checked
        }
        ConfigImplementFrontDimensions{
            id:configImplementFrontDimensions
            anchors.fill: mainConfig
            anchors.margins:1
            visible: implementMenu.visible && configImpDim.checked && settings.setTool_isToolFront
        }
        ConfigImplementRearDimensions{
            id:configImplementRearDimensions
            anchors.fill: mainConfig
            anchors.margins:1
            visible: implementMenu.visible && configImpDim.checked && settings.setTool_isToolRearFixed
        }
        ConfigImplementTBTDimensions{
            id:configImplementTBTDimensions
            anchors.fill: mainConfig
            anchors.margins:1
            visible: implementMenu.visible && configImpDim.checked && settings.setTool_isToolTBT
        }
        ConfigImplementTrailingDimensions{
            id:configImplementTrailingDimensions
            anchors.fill: mainConfig
            anchors.margins:1
            visible: implementMenu.visible && configImpDim.checked && settings.setTool_isToolTrailing && !settings.setTool_isToolTBT
        }
        ConfigImplementSections{
            id:configImplementSections
            anchors.fill: mainConfig
            anchors.margins:1
            visible: implementMenu.visible && configImpSection.checked
        }
        ConfigImplementTiming{
            id:configImplementTiming
            anchors.fill: mainConfig
            anchors.margins:1
            visible: implementMenu.visible && configImpTiming.checked
        }
        ConfigImplementSwitches{
            id:configImplementSwitches
            anchors.fill: mainConfig
            anchors.margins:1
            visible: implementMenu.visible && configImpSwitches.checked
        }
        ConfigSources{
            id:configSources
            anchors.fill: mainConfig
            anchors.margins:1
            visible: sourcesMenu.visible && configsrcHeading.checked
        }
        ConfigSourcesRoll{
            id:configSourcesRoll
            anchors.fill: mainConfig
            anchors.margins:1
            visible: sourcesMenu.visible && configsrcRoll.checked
        }
        ConfigYouTurn{
            id:configYouTurn
            anchors.fill: mainConfig
            anchors.margins:1
            visible: uTurnMenu.checked
        }
        ConfigModules{
            id:configModules
            anchors.fill: mainConfig
            anchors.margins:1
            visible: modulesMenu.visible && configModulesSettings.checked
        }
        ConfigModulesPins{
            id:configModulesPins
            anchors.fill: mainConfig
            anchors.margins:1
            visible: modulesMenu.visible && configModulesPinsbtn.checked
        }
        ConfigTrams{
            id:configTrams
            anchors.fill: mainConfig
            anchors.margins:1
            visible: tramMenu.checked
        }
        ConfigDisplay{
            id: configDisplay
            anchors.fill: mainConfig
            anchors.margins: 1
            visible: displayMenu.checked
        }

        ConfigFeatures{
            id:configFeatures
            anchors.fill: mainConfig
            anchors.margins:1
            visible: featureMenu.checked
            onVisibleChanged: {
                console.debug("feature menu visibility changed")
            }
        }
    }
}
