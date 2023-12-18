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
                anchors.left: configWhichVehicle.left
                anchors.bottom: configWhichVehicle.bottom
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

}
