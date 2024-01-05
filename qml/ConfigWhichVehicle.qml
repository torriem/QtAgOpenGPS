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
                icon: "/images/Config/ConD_FloorTexture.png"
                isChecked: false
            }
            IconButtonColor{
                id: autoDayNight
                objectName: "btnAutoDayNight"
                isChecked: false
                buttonText: qsTr("Auto Day Night")
                icon: "/images/Config/ConD_AutoDayNight.png"
            }
            IconButtonColor{
                id:startFullScreen
                objectName: "btnStartFullScreen"
                buttonText: qsTr("Start FullScreen")
                icon: "/images/Config/ConD_FullScreenBegin.png"
                isChecked: false
            }
            IconButtonColor{
                id:polygons
                objectName: "btnPolygons"
                buttonText: qsTr("Polygons")
                icon: "/images/Config/ConD_Poligons.png"
                isChecked: false
            }
            IconButtonColor{
                id:grid
                objectName: "btnGrid"
                buttonText: qsTr("Grid")
                icon: "/images/Config/ConD_Grid.png"
                isChecked: false
            }
            IconButtonColor{
                id:sky
                objectName: "btnSky"
                buttonText:qsTr("Sky")
                icon: "/images/Config/ConD_Sky.png"
            }
            IconButtonColor{
                id:brightness
                buttonText:qsTr("Brightness")
                objectName: "btnBrightness"
                icon: "/images/BrightnessUp.png"
                isChecked: false
            }
            IconButtonColor{
                id:lightBar
                objectName: "btnLightBar"
                buttonText:qsTr("Lightbar")
                icon: "/images/Config/ConD_LightBar.png"
                isChecked: false
            }
            IconButtonColor{
                id:logNMEA
                objectName: "btnLonNMEA"
                buttonText: qsTr("Log NMEA")
                isChecked: false
                icon: "/images/Config/ConD_LogNMEA.png"
            }
            IconButtonColor{
                id:keyboard
                objectName: "btnKeyboard"
                buttonText:qsTr("Keyboard")
                icon: "/images/Config/ConD_KeyBoard.png"
                isChecked: false
            }
            IconButtonColor{
                id: guideLines
                objectName: "btnGuidelines"
                buttonText: qsTr("GuideLines")
                icon: "/images/Config/ConD_ExtraGuides.png"
                isChecked: false
            }
            IconButtonColor{
                id:svennArrow
                objectName: "btnSvennArrow"
                buttonText: qsTr("Svenn Arrow")
                icon: "/images/SvennArrow.png"
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
                icon: "/images/Config/ConD_Metric.png"
                buttonText: ""
                onClicked:{
                    metric.isChecked = true
                    imperial.isChecked = false
                }
            }
            IconButtonColor{
                id:imperial
                objectName: "btnImperial"
                icon: "/images/Config/ConD_Imperial.png"
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
            buttonText: qsTr("Load")
            color3: "white"
            border: 2
        }
        IconButtonText{
            id: deletefolder
            anchors.top:load.bottom
            anchors.topMargin: 50
            anchors.horizontalCenter: load.horizontalCenter
            icon.source: "/images/VehFileDelete.png"
            buttonText: qsTr("Delete")
            color3: "white"
            border: 2
        }
    }
}
