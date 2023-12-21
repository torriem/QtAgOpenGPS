import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4
import Qt.labs.folderlistmodel 2.2

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
                buttonText: qsTr("Field Texture")
                icon: "qrc:/images/Config/ConD_FloorTexture.png"
                isChecked: false
            }
            IconButtonColor{
                id: autoDayNight
                objectName: "btnAutoDayNight"
                isChecked: false
                buttonText: qsTr("Auto Day Night")
                icon: "qrc:/images/Config/ConD_AutoDayNight.png"
            }
            IconButtonColor{
                id:startFullScreen
                objectName: "btnStartFullScreen"
                buttonText: qsTr("Start FullScreen")
                icon: "qrc:/images/Config/ConD_FullScreenBegin.png"
                isChecked: false
            }
            IconButtonColor{
                id:polygons
                objectName: "btnPolygons"
                buttonText: qsTr("Polygons")
                icon: "qrc:/images/Config/ConD_Poligons.png"
                isChecked: false
            }
            IconButtonColor{
                id:grid
                objectName: "btnGrid"
                buttonText: qsTr("Grid")
                icon: "qrc:/images/Config/ConD_Grid.png"
                isChecked: false
            }
            IconButtonColor{
                id:sky
                objectName: "btnSky"
                buttonText:qsTr("Sky")
                icon: "qrc:/images/Config/ConD_Sky.png"
            }
            IconButtonColor{
                id:brightness
                buttonText:qsTr("Brightness")
                objectName: "btnBrightness"
                icon: "qrc:/images/BrightnessUp.png"
                isChecked: false
            }
            IconButtonColor{
                id:lightBar
                objectName: "btnLightBar"
                buttonText:qsTr("Lightbar")
                icon: "qrc:/images/Config/ConD_LightBar.png"
                isChecked: false
            }
            IconButtonColor{
                id:logNMEA
                objectName: "btnLonNMEA"
                buttonText: qsTr("Log NMEA")
                isChecked: false
                icon: "qrc:/images/Config/ConD_LogNMEA.png"
            }
            IconButtonColor{
                id:keyboard
                objectName: "btnKeyboard"
                buttonText:qsTr("Keyboard")
                icon: "qrc:/images/Config/ConD_KeyBoard.png"
                isChecked: false
            }
            IconButtonColor{
                id: guideLines
                objectName: "btnGuidelines"
                buttonText: qsTr("GuideLines")
                icon: "qrc:/images/Config/ConD_ExtraGuides.png"
                isChecked: false
            }
            IconButtonColor{
                id:svennArrow
                objectName: "btnSvennArrow"
                buttonText: qsTr("Svenn Arrow")
                icon: "qrc:/images/SvennArrow.png"
                isChecked: false
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
                icon: "qrc:/images/Config/ConD_Metric.png"
                buttonText: ""
                onClicked:{
                    metric.isChecked = true
                    imperial.isChecked = false
                }
            }
            IconButtonColor{
                id:imperial
                objectName: "btnImperial"
                icon: "qrc:/images/Config/ConD_Imperial.png"
                buttonText: ""
                onClicked:{
                    imperial.isChecked = true
                    metric.isChecked = false
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
                icon.source: "qrc:/images/VehFileSaveAs"
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
            icon.source: "qrc:/images/VehFileLoad.png"
            //border.width: 3
            buttonText: qsTr("Load")
        }
        IconButtonText{
            id: deletefolder
            anchors.top:load.bottom
            anchors.topMargin: 50
            anchors.horizontalCenter: load.horizontalCenter
            icon.source: "qrc:/images/VehFileDelete.png"
            buttonText: qsTr("Delete")
            //border.width: 3
        }
    }
}
