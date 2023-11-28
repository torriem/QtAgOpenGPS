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
                icon: "qrc:/images/Config/Con_VehicleMenu.png"
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
                icon: "qrc:/images/Config/Con_ImplementMenu.png"
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
                icon: "qrc:/images/Config/Con_SourcesMenu.png"
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
                icon: "qrc:/images/Config/Con_UTurnMenu.png"
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
                icon: "qrc:/images/Config/Con_ModulesMenu.png"
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
                icon: "qrc:/images/Config/Con_TramMenu.png"
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
                icon: "qrc:/images/Config/Con_FeatureMenu.png"
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
        Rectangle{
            id: mainConfig
            anchors.top: topLine.bottom
            anchors.right: parent.right
            anchors.left: leftColumn.right
            anchors.bottom: bottomLine.top
            anchors.rightMargin: 8
            color: "ghostwhite"
            border.color: "lime"
            GridLayout{
                id:buttonsMain
                anchors.top: parent.top
                anchors.bottom: units.top
                anchors.left: parent.left
                anchors.right: vehicleList.left
                rows: 6
                columns:2
                flow: Grid.TopToBottom
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id: fieldTexture
                    IconButtonTransparent{
                        Text{
                            text: qsTr("Field Texture")
                        }

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        icon: "qrc:/images/Config/ConD_FloorTexture.png"
                        colorHover1: "transparent"
                        checkable: true
                        property bool toggleBtn: false
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                fieldTexture.color = "green"
                            }else{
                                fieldTexture.color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }
                    }
                    color: "white"
                    border.color: "black"

                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id: autoDayNight
                    IconButtonTransparent{
                        Text{
                            text: qsTr("Auto Day Night")
                        }

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        icon: "qrc:/images/Config/ConD_AutoDayNight.png"
                        colorHover1: "transparent"
                        checkable: true
                        property bool toggleBtn: false
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                autoDayNight.color = "green"
                            }else{
                                autoDayNight.color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }
                    }
                    color: "white"
                    border.color: "black"

                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id:startFullScreen
                    IconButtonTransparent{
                        Text {
                            text: qsTr("Start FullScreen")
                        }
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        icon: "qrc:/images/Config/ConD_FullScreenBegin.png"
                        colorHover1: "transparent"
                        checkable: true
                        property bool toggleBtn: false
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                startFullScreen.color = "green"
                            }else{
                                startFullScreen.color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }
                    }
                    color: "white"
                    border.color: "black"

                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id:polygons
                    IconButtonTransparent{
                        Text {
                            text: qsTr("Polygons")
                        }
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        icon: "qrc:/images/Config/ConD_Poligons.png"
                        colorHover1: "transparent"
                        checkable: true
                        property bool toggleBtn: false
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                polygons.color = "green"
                            }else{
                                polygons.color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }
                    }
                    color: "white"
                    border.color: "black"

                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id:grid
                    IconButtonTransparent{
                        Text{
                            text: qsTr("Grid")
                        }

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        icon: "qrc:/images/Config/ConD_Grid.png"
                        colorHover1: "transparent"
                        checkable: true
                        property bool toggleBtn: false
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                grid.color = "green"
                            }else{
                                grid.color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }
                    }
                    color: "white"
                    border.color: "black"

                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id:sky
                    IconButtonTransparent{
                        Text{
                            text:qsTr("Sky")
                        }

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        icon: "qrc:/images/Config/ConD_Sky.png"
                        colorHover1: "transparent"
                        checkable: true
                        property bool toggleBtn: false
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                sky.color = "green"
                            }else{
                                sky.color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }
                    }
                    color: "white"
                    border.color: "black"

                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id:brightness
                    IconButtonTransparent{
                        Text{
                            text:qsTr("Brightness")
                        }

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        icon: "qrc:/images/BrightnessUp.png"
                        colorHover1: "transparent"
                        checkable: true
                        property bool toggleBtn: false
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                brightness.color = "green"
                            }else{
                                brightness.color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }
                    }
                    color: "white"
                    border.color: "black"

                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id:lightBar
                    IconButtonTransparent{
                        Text{
                            text:qsTr("Lightbar")
                        }

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        icon: "qrc:/images/Config/ConD_LightBar.png"
                        colorHover1: "transparent"
                        checkable: true
                        property bool toggleBtn: false
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                lightBar.color = "green"
                            }else{
                                lightBar.color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }
                    }
                    color: "white"
                    border.color: "black"

                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id:logNMEA
                    IconButtonTransparent{
                        Text{
                            text: qsTr("Log NMEA")
                        }

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        icon: "qrc:/images/Config/ConD_LogNMEA.png"
                        colorHover1: "transparent"
                        checkable: true
                        property bool toggleBtn: false
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                logNMEA.color = "green"
                            }else{
                                logNMEA.color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }
                    }
                    color: "white"
                    border.color: "black"

                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id:keyboard
                    IconButtonTransparent{
                        Text{
                            text:qsTr("Keyboard")
                        }

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        icon: "qrc:/images/Config/ConD_KeyBoard.png"
                        colorHover1: "transparent"
                        checkable: true
                        property bool toggleBtn: false
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                keyboard.color = "green"
                            }else{
                                keyboard.color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }
                    }
                    color: "white"
                    border.color: "black"

                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id: guideLines
                    IconButtonTransparent{
                        Text{
                            text: qsTr("GuideLines")
                        }

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        icon: "qrc:/images/Config/ConD_ExtraGuides.png"
                        colorHover1: "transparent"
                        checkable: true
                        property bool toggleBtn: false
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                guideLines.color = "green"
                            }else{
                                guideLines.color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }
                    }
                    color: "white"
                    border.color: "black"

                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id:svennArrow
                    IconButtonTransparent{
                        Text{
                            text: qsTr("Svenn Arrow")
                        }

                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        icon: "qrc:/images/SvennArrow.png"
                        colorHover1: "transparent"
                        checkable: true
                        property bool toggleBtn: false
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                svennArrow.color = "green"
                            }else{
                                svennArrow.color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }
                    }
                    color: "white"
                    border.color: "black"

                }
            }
            Row{
                id:units
                anchors.left: mainConfig.left
                anchors.bottom: mainConfig.bottom
                width: childrenRect.width
                height: childrenRect.height
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id:metric
                    IconButtonTransparent{
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        icon: "qrc:/images/Config/ConD_Metric.png"
                        colorHover1: "transparent"
                        checkable: true
                        /* property bool toggleBtn: false
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                .color = "green"
                            }else{
                                .color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }*/
                        onClicked:{
                            metric.color = "green"
                            imperial.color = "white"
                        }
                    }
                    color: "white"
                    border.color: "black"

                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    id:imperial
                    IconButtonTransparent{
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        icon: "qrc:/images/Config/ConD_Imperial.png"
                        colorHover1: "transparent"
                        checkable: true
                        /*property bool toggleBtn: true
                        onToggleBtnChanged: {
                            if(toggleBtn == true){
                                .color = "green"
                            }else{
                                .color = "white"
                            }
                        }

                        function toggleState(){
                            if(toggleBtn == true){
                                toggleBtn = false;
                            }else{
                                toggleBtn = true;
                            }
                        }
                        onClicked: {
                            toggleState()
                        }*/
                        onClicked:{
                            imperial.color = "green"
                            metric.color = "white"
                        }
                    }

                    color: "white"
                    border.color: "black"

                }
            }
            Rectangle{
                id: vehicleList
                border.color: "black"
                color: "light gray"
                visible: true
                anchors.bottom:mainConfig.bottom
                width:mainConfig.width /2
                anchors.top:entryBox.bottom
                anchors.right: parent.right


                ListView {
                    anchors.fill: parent

                    property Component mycomponent: fileName
                    model :FolderListModel{
                        id: fieldList
                        showDirs: true
                        folder: "file:/home/davidwedel/Documents/QtAgOpenGPS/Vehicles/"
                    }

                    delegate: RadioButton{
                        width:parent.width
                        height:50
                        //color: "light gray"
                        Text{
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            text: fileName
                        }
                    }
                }
            }
            Rectangle{
                id: entryBox
                anchors.top: mainConfig.top
                anchors.right: mainConfig.right
                height: 75
                width: vehicleList.width
                color: parent.color
                IconButtonTransparent{
                    id: vehFileSaveAs
                    anchors.right: parent.right
                    anchors.rightMargin: 30
                    anchors.verticalCenter: parent.verticalCenter
                    width: 75
                    height: 75
                    icon: "qrc:/images/VehFileSaveAs"
                }
                Text {
                    id: saveAs
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter

                    text: qsTr("Save As")
                }

                Rectangle{
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: saveAs.right
                    anchors.right: vehFileSaveAs.left
                    anchors.rightMargin: 50
                    height: 30
                    color:"white"
                    border.color: "gray"
                    TextInput{
                        anchors.fill: parent
                        text:""
                        cursorVisible: true
                    }
                }
            }
                IconButtonTransparent{
                    id: load
                    anchors.right: entryBox.left
                    anchors.rightMargin: 30
                    anchors.top: vehicleList.top
                    icon: "qrc:/images/VehFileLoad.png"
                    Text {
                        text: qsTr("Load")
                    }
                }
                IconButtonTransparent{
                    id: deletefolder
                    anchors.top:load.bottom
                    anchors.topMargin: 50
                    anchors.horizontalCenter: load.horizontalCenter
                    icon: "qrc:/images/VehFileDelete.png"
                    Text {
                        text: qsTr("Delete")
                    }
                }
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
