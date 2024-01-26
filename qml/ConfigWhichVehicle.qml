import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4

/*todo:
  couldn't find the setting for the polygons
  or logNMEA
  */
Item {
    anchors.fill: parent

    onVisibleChanged: {
        if(visible)
            //ask backend to refresh our list of vehicles
            aog.vehicle_update_list()
    }

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
                checkable: true
                isChecked: settings.setDisplay_isTextureOn
                onCheckedChanged: settings.setDisplay_isTextureOn = checked
            }
            IconButtonColor{
                id: autoDayNight
                objectName: "btnAutoDayNight"
                isChecked: settings.setDisplay_isAutoDayNight
                onCheckedChanged: settings.setDisplay_isAutoDayNight = !checked
                checkable: true
                text: qsTr("Auto Day Night")
                icon.source: "/images/Config/ConD_AutoDayNight.png"
            }
            IconButtonColor{
                id:startFullScreen
                objectName: "btnStartFullScreen"
                text: qsTr("Start FullScreen")
                icon.source: "/images/Config/ConD_FullScreenBegin.png"
                checkable: true
                isChecked: settings.setDisplay_isStartFullScreen
                onCheckedChanged: settings.setDisplay_isStartFullScreen = checked
            }
            IconButtonColor{
                id:grid
                objectName: "btnGrid"
                text: qsTr("Grid")
                icon.source: "/images/Config/ConD_Grid.png"
                checkable: true
                isChecked: settings.setMenu_isGridOn
                onCheckedChanged: settings.setMenu_isGridOn = checked
            }
            IconButtonColor{
                id:sky
                objectName: "btnSky"
                text:qsTr("Sky")
                icon.source: "/images/Config/ConD_Sky.png"
                checkable: true
                isChecked: settings.setMenu_isSkyOn
                onCheckedChanged: settings.setMenu_isSkyOn = checked
            }
            IconButtonColor{
                id:brightness
                text:qsTr("Brightness")
                objectName: "btnBrightness"
                icon.source: "/images/BrightnessUp.png"
                checkable: true
                isChecked: settings.setDisplay_isBrightnessOn
                onCheckedChanged: settings.setDisplay_isBrightnessOn = checked
            }
            IconButtonColor{
                id:lightBar
                objectName: "btnLightBar"
                text:qsTr("Lightbar")
                icon.source: "/images/Config/ConD_LightBar.png"
                checkable: true
                isChecked: settings.setMenu_isLightbarOn
                onCheckedChanged: settings.setMenu_isLightbarOn = checked
            }
            IconButtonColor{
                id:logNMEA
                objectName: "btnLonNMEA"
                text: qsTr("Log NMEA")
                checkable: true
                icon.source: "/images/Config/ConD_LogNMEA.png"
            }
            /*
            IconButtonColor{ //delete this?
                id:keyboard
                objectName: "btnKeyboard"
                text:qsTr("Keyboard")
                icon.source: "/images/Config/ConD_KeyBoard.png"
                checkable: true
                isChecked: false
            }*/
            IconButtonColor{
                id: guideLines
                objectName: "btnGuidelines"
                text: qsTr("GuideLines")
                icon.source: "/images/Config/ConD_ExtraGuides.png"
                checkable: true
                isChecked: settings.setMenu_isSideGuideLines
                onCheckedChanged: settings.setMenu_isSideGuideLines = checked
            }
            IconButtonColor{
                id:svennArrow
                objectName: "btnSvennArrow"
                text: qsTr("Svenn Arrow")
                icon.source: "/images/SvennArrow.png"
                checkable: true
                isChecked: settings.setDisplay_isSvennArrowOn
                onCheckedChanged: settings.setDisplay_isSvennArrowOn
            }
        }
        Row{
            id:units
            anchors.left: configWhichVehicle.left
            anchors.bottom: configWhichVehicle.bottom
            width: childrenRect.width
            height: childrenRect.height
            ButtonGroup {
                id: metricimp
                buttons: [metric, imperial]
            }

            IconButtonColor{
                id:metric
                objectName: "btnMetric"
                icon.source: "/images/Config/ConD_Metric.png"
                //text: qsTr("Metric")
                property bool settingsChecked: settings.setMenu_isMetric
                checkable: true
                isChecked: settings.setMenu_isMetric
                onCheckedChanged: settings.setMenu_isMetric = checked
            }
            IconButtonColor{
                id:imperial
                objectName: "btnImperial"
                icon.source: "/images/Config/ConD_Imperial.png"
                text: ""
                checkable: true
                isChecked: !settings.setMenu_isMetric
                onCheckedChanged: settings.setMenu_isMetric = !checked
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

            function refresh_model() {
                fieldList.clear()
                for (var i=0; i < aog.vehicle_list.length ; i++) {
                    //console.debug(aog.vehicle_list[i])
                    fieldList.append( { index: aog.vehicle_list[i].index,
                                         name: aog.vehicle_list[i].name })
                }
            }

            Connections {
                target: aog
                function onVehicle_listChanged() {
                    vehicleList.refresh_model()
                }
            }

            Component.onCompleted: {
                refresh_model()
            }

            ListView {
                id: vehicleListView
                anchors.fill: parent
                anchors.margins: 1

                model : ListModel{
                    id: fieldList
                }

                property string selectedVehicle: ""

                delegate: RadioButton{
                    id: control

                    indicator: Rectangle{
                        anchors.fill: parent
                        anchors.margins: 2
                        color: control.down ? "white" : "blue"
                        visible: control.checked
                    }

                    onCheckedChanged: {
                        if(checked) {
                            vehicleListView.selectedVehicle = model.name
                            saveAsVehicle.text = model.name
                        }
                    }

                    width: vehicleListView.width
                    height: 50
                    //anchors.fill: parent
                    //color: "light gray"
                    Text{
                        anchors.left: parent.left
                        anchors.leftMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                        text: model.name
                        font.pixelSize: 20
                        font.bold: true
                        color: control.checked ? "white" : "black"
                        z: 2
                    }
                }
            }
        }
        Label {
            id: currentVehicle
            text: qsTr("Current vehicle is") + "<h2>" + settings.setVehicle_vehicleName + "</h2>"
            anchors.top: configWhichVehicle.top
            anchors.horizontalCenter: entryBox.horizontalCenter
            anchors.margins: 15
        }
        Rectangle{
            id: entryBox
            anchors.top: currentVehicle.bottom
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
                onClicked: {
                    if (saveAsVehicle.text != "") {
                        //console.debug("Going to save", saveAsVehicle.text)
                        aog.vehicle_saveas(saveAsVehicle.text)
                        //just setting the name is probably enough to get it to save the vehicle
                        settings.setVehicle_vehicleName = saveAsVehicle.text
                        saveAsVehicle.text = ""
                        aog.vehicle_update_list()
                    }
                }
            }
            Text {
                id: saveAs
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter

                text: qsTr("Save As")
            }

            TextField{
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: saveAs.right
                anchors.right: vehFileSaveAs.left

                id: saveAsVehicle
                anchors.margins: 5
                placeholderText: "vehicle and implement"
                selectByMouse: true
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
            onClicked: {
                if (vehicleListView.selectedVehicle != "" ) {
                    aog.vehicle_load(vehicleListView.selectedVehicle)
                    settings.setVehicle_vehicleName = vehicleListView.selectedVehicle
                }
            }

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
            onClicked: {
                //settings.setMenu_isMetric = !utils.isTrue(settings.setMenu_isMetric)
                //console.debug("qml says settings ismetric is",settings.setMenu_isMetric)
                //aog.vehicle_delete("testing123")
                if (vehicleListView.selectedVehicle != "" ) {
                    aog.vehicle_delete(vehicleListView.selectedVehicle)
                    aog.vehicle_update_list()
                }
            }
        }
    }
}
