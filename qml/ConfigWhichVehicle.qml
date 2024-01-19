import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4
import Qt.labs.folderlistmodel 2.2


/*todo:
  couldn't find the setting for the polygons
  or logNMEA
  */
Item {
    anchors.fill: parent
    Rectangle{
        id: configWhichVehicle
        anchors.fill: parent
        color: "ghostwhite"
        GridLayout{
            id:buttonsMain
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.bottom: units.top
            anchors.left: parent.left
            anchors.right: load.left
            rows: 6
            columns:2
            rowSpacing: 10
            flow: Grid.TopToBottom
            IconButtonColor{
                id: fieldTexture
                objectName: "btnFieldTexture"
                text: qsTr("Field Texture")
                icon.source: "/images/Config/ConD_FloorTexture.png"
                isChecked: settings.setDisplay_isTextureOn
                onCheckedChanged: settings.setDisplay_isTextureOn = checked
            }
            IconButtonColor{
                id: autoDayNight
                objectName: "btnAutoDayNight"
                checked: settings.setDisplay_isAutoDayNight
                onCheckedChanged: settings.setDisplay_isAutoDayNight = !checked
                text: qsTr("Auto Day Night")
                icon.source: "/images/Config/ConD_AutoDayNight.png"
            }
            IconButtonColor{
                id:startFullScreen
                objectName: "btnStartFullScreen"
                text: qsTr("Start FullScreen")
                icon.source: "/images/Config/ConD_FullScreenBegin.png"
                checked: settings.setDisplay_isStartFullScreen
                onCheckedChanged: settings.setDisplay_isStartFullScreen = checked
            }
            IconButtonColor{
                id:polygons
                objectName: "btnPolygons"
                text: qsTr("Polygons")
                icon.source: "/images/Config/ConD_Poligons.png"
               // checked: settings.set
            }
            IconButtonColor{
                id:grid
                objectName: "btnGrid"
                text: qsTr("Grid")
                icon.source: "/images/Config/ConD_Grid.png"
                checked: settings.setMenu_isGridOn
                onCheckedChanged: settings.setMenu_isGridOn = checked
            }
            IconButtonColor{
                id:sky
                objectName: "btnSky"
                text:qsTr("Sky")
                icon.source: "/images/Config/ConD_Sky.png"
                checked: settings.setMenu_isSkyOn
                onCheckedChanged: settings.setMenu_isSkyOn = checked
            }
            IconButtonColor{
                id:brightness
                text:qsTr("Brightness")
                objectName: "btnBrightness"
                icon.source: "/images/BrightnessUp.png"
                checked: settings.setDisplay_isBrightnessOn
                onCheckedChanged: settings.setDisplay_isBrightnessOn = checked
            }
            IconButtonColor{
                id:lightBar
                objectName: "btnLightBar"
                text:qsTr("Lightbar")
                icon.source: "/images/Config/ConD_LightBar.png"
                checked: settings.setMenu_isLightbarOn
                onCheckedChanged: settings.setMenu_isLightbarOn = checked
            }
            IconButtonColor{
                id:logNMEA
                objectName: "btnLonNMEA"
                text: qsTr("Log NMEA")
                isChecked: false
                icon.source: "/images/Config/ConD_LogNMEA.png"
            }
            IconButtonColor{ //delete this?
                id:keyboard
                objectName: "btnKeyboard"
                text:qsTr("Keyboard")
                icon.source: "/images/Config/ConD_KeyBoard.png"
                isChecked: false
            }
            IconButtonColor{
                id: guideLines
                objectName: "btnGuidelines"
                text: qsTr("GuideLines")
                icon.source: "/images/Config/ConD_ExtraGuides.png"
                checked: settings.setMenu_isSideGuideLines
                onCheckedChanged: settings.setMenu_isSideGuideLines = checked
            }
            IconButtonColor{
                id:svennArrow
                objectName: "btnSvennArrow"
                text: qsTr("Svenn Arrow")
                icon.source: "/images/SvennArrow.png"
                checked: settings.setDisplay_isSvennArrowOn
                onCheckedChanged: settings.setDisplay_isSvennArrowOn
            }
        }
        Row{
            id:units
            anchors.left: configWhichVehicle.left
            anchors.bottom: configWhichVehicle.bottom
            width: childrenRect.width
            height: childrenRect.height
            IconButtonColor{
                id:metric
                objectName: "btnMetric"
                icon.source: "/images/Config/ConD_Metric.png"
                text: ""
                isChecked: utils.isMetric()
                onClicked:{
                    imperial.checkable = true
                    metric.checkable = false
                    if(utils.isMetric){
                        settings.setMenu_isMetric = true
                    }
                }
                Connections{
                    target: settings
                    function onSetMenu_isMetricChanged(){
                        metric.checked = settings.setMenu_isMetric
                    }
                }
            }
            IconButtonColor{
                id:imperial
                objectName: "btnImperial"
                icon.source: "/images/Config/ConD_Imperial.png"
                text: ""
                isChecked: !utils.isMetric()
                onClicked:{
                    metric.checkable = true
                    imperial.checkable = false
                    console.log("starting")
                    if(utils.isMetric){
                        settings.setMenu_isMetric = false
                        console.log(settings.setMenu_isMetric)
                    }
                }
                Connections{
                    target: settings
                    function onSetMenu_isMetricChanged(){
                        imperial.checked = !settings.setMenu_isMetric
                        console.log(settings.setMenu_isMetric)
                    }
                }
            }
        }
        Rectangle{
            id: vehicleList
            border.color: "black"
            color: "light gray"
            visible: true
            anchors.bottom:configWhichVehicle.bottom
            width:configWhichVehicle.width /2
            anchors.top:entryBox.bottom
            anchors.right: parent.right


            ListView {
                anchors.fill: parent
                anchors.margins: 1

                property Component mycomponent: fileName
                model :FolderListModel{
                    id: fieldList
                    showDirs: true
                    folder: "file:/home/davidwedel/Documents/QtAgOpenGPS/Vehicles/"
                }

                delegate: RadioButton{
                    id: control
                        indicator: Rectangle{
                            anchors.fill: parent
                            anchors.margins: 2
                            color: control.down ? "white" : "blue"
                            visible: control.checked
                        }

                    width:parent.width
                    height:50
                    //anchors.fill: parent
                    //color: "light gray"
                    Text{
                        anchors.left: parent.left
                        anchors.leftMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                        text: fileName
                        font.pixelSize: 25
                        font.bold: true
                        color: control.checked ? "white" : "black"
                        z: 2
                    }
                }
            }
        }
        Rectangle{
            id: entryBox
            anchors.top: configWhichVehicle.top
            anchors.right: configWhichVehicle.right
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
                icon.source: "/images/VehFileSaveAs"
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
        IconButtonText{
            id: load
            anchors.right: entryBox.left
            anchors.rightMargin: 30
            anchors.top: vehicleList.top
            icon.source: "/images/VehFileLoad.png"
            text: qsTr("Load")
            color3: "white"
            border: 2
        }
        IconButtonText{
            id: deletefolder
            anchors.top:load.bottom
            anchors.topMargin: 50
            anchors.horizontalCenter: load.horizontalCenter
            icon.source: "/images/VehFileDelete.png"
            text: qsTr("Delete")
            color3: "white"
            border: 2
        }
    }
}
