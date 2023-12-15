import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent
        visible: true
        color: "ghostwhite"
        Rectangle{
            anchors.top: parent.top
            anchors.topMargin: 300
            anchors.right: parent.left
            width: 100
            id: subMenuSources
            visible: true
            color: "white"
            height: childrenRect.height
            Column{
                visible: true
                anchors.fill: parent
                IconButton{
                    id: configsrcHeading
                    icon: "qrc:/images/Config/ConS_VehicleConfig.png"
                    /* function toggle_(){
                    if(configImplement.visible == true){
                        configImplement.visible = false
                    }else{
                        configImplement.visible = true
                    }
                }

                onClicked: toggle_configImplement()*/
                }
                IconButton{
                    id: configsrcRoll
                    icon: "qrc:/images/Config/ConS_ImplementHitch.png"
                    function toggle_srcrollwin(){
                        if(configSrcRollWin.visible == true){
                            configSrcRollWin.visible = false
                        }else{
                            configSrcRollWin.visible = true
                        }
                    }

                    onClicked: toggle_srcrollwin()
                }
            }
        }
        Rectangle{
            id:antennaType
            width: 360
            height: childrenRect.height+ 10
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            color: "ghostwhite"
            border.color: "black"
            visible: true

            Text {
                id: text
                text: qsTr("Antenna Type")
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Row{
                anchors.top: text.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10
                Rectangle{
                    Text{
                        text: qsTr("Dual")
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    width:150
                    height:100
                    radius:5
                    border.color: "black"
                    id: dual
                    IconButtonTransparent{
                        icon: "qrc:/images/Config/Con_SourcesGPSDual.png"
                        anchors.fill: parent
                        colorHover1: "transparent"
                        checkable: true
                        /*onClicked: {
                        harvester.color = "green"
                        tractor.color = "white"
                        tr4wd.color = "white"
                    }*/
                    }
                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    border.color: "black"
                    id: fix
                    Text{
                        text: qsTr("Fix")
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    IconButtonTransparent{
                        anchors.fill:parent
                        icon: "qrc:/images/Config/Con_SourcesGPSSingle.png"
                        colorHover1: "transparent"
                        checkable: true
                        /* onClicked: {
                        harvester.color = "white"
                        tractor.color = "green"
                        tr4wd.color = "white"
                    }*/
                    }
                }
            }
        }

        Rectangle{
            id:rtkAlarm
            width: parent.width /2
            height: childrenRect.height+ 10
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            color: "ghostwhite"
            visible: true

            Text {
                id: textRTK
                text: qsTr("RTK Alarm")
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Row{
                anchors.top: textRTK.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 10
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    border.color: "black"
                    id: alarm
                    IconButtonTransparent{
                        icon: "qrc:/images/Config/Con_SourcesRTKAlarm.png"
                        anchors.fill: parent
                        colorHover1: "transparent"
                        checkable: true
                        /*onClicked: {
                        harvester.color = "green"
                        tractor.color = "white"
                        tr4wd.color = "white"
                    }*/
                    }
                }
                Text{
                    text: qsTr("  ->  ")
                }
                Rectangle{
                    width:150
                    height:100
                    radius:5
                    border.color: "black"
                    id: killAutoSteer
                    IconButtonTransparent{
                        anchors.fill:parent
                        icon: "qrc:/images/AutoSteerOff.png"
                        colorHover1: "transparent"
                        checkable: true
                        /* onClicked:
                        harvester.color = "white"
                        tractor.color = "green"
                        tr4wd.color = "white"
                    }*/
                    }
                }
            }
        }
        Rectangle{
            id:singleAntennaSettings
            anchors.bottom: parent.bottom
            border.color: "black"
            anchors.right: parent.right
            anchors.left: rtkAlarm.right
            anchors.leftMargin: 5
            anchors.top: antennaType.bottom
            anchors.topMargin: 20
            color: "ghostwhite"
            Text{
                id: singleText
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Single Antenna Settings")
            }
            Rectangle{
                id: minGPSStep
                width: 180
                height: 50
                anchors.top: singleText.bottom
                anchors.right: parent.right
                anchors.margins: 5
                border.color: "black"
                color: "blue"
                Text{
                    anchors.right: stepButton.left
                    anchors.rightMargin: 15
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Minimum GPS Step")
                }

                Button{
                    id: stepButton
                    Text{
                        text: qsTr("5 Centimeter")
                        anchors.fill: parent
                        font.bold: true
                    }
                    flat: true
                    anchors.fill: parent
                    onClicked: minGPSStep.color = "green"

                }
                Text{
                    id: cmText
                    anchors.top: minGPSStep.bottom
                    anchors.topMargin: 10
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTr("50 Centimeter")
                    font.bold: true
                }
                Text{
                    anchors.top: minGPSStep.bottom
                    anchors.topMargin: 10
                    anchors.right: stepButton.left
                    text: qsTr("Heading Distance")

                }
            }

            Rectangle{//fusion adjuster
                id: fusionRow
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                width: 360
                height: 100
                border.color: "black"
                color: "ghostwhite"
                Text{
                    anchors.verticalCenter: fusionText.verticalCenter
                    anchors.horizontalCenter: imuFusion.horizontalCenter
                    text: qsTr("IMU <")
                    color: "green"
                }
                Text{
                    id: fusionText
                    anchors.bottom: sliderRectangle.top
                    anchors.horizontalCenter: sliderRectangle.horizontalCenter
                    anchors.bottomMargin: 10
                    text: qsTr("Fusion")
                }
                Text{
                    anchors.verticalCenter: fusionText.verticalCenter
                    anchors.horizontalCenter: gpsFusion.horizontalCenter
                    text: qsTr("> GPS")
                    color: "red"
                }
                Text {
                    id: imuFusion
                    anchors.right: sliderRectangle.left
                    anchors.rightMargin: 5
                    anchors.verticalCenter: sliderRectangle.verticalCenter
                    objectName: "imuFusion"
                    text: qsTr("%")
                    font.bold: true
                }
                Rectangle{
                    id:sliderRectangle
                    width: 280
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 10
                    height: 50
                    //border.color: "black"
                    color: "lightgray"
                    Slider{
                        id: fusionSlider
                        objectName: "fusionSlider"
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        width: 270
                        from: 90
                        to: 50
                        value: 70
                    }
                }
                Text {
                    id: gpsFusion
                    anchors.left: sliderRectangle.right
                    anchors.leftMargin: 5
                    anchors.verticalCenter: sliderRectangle.verticalCenter
                    text: qsTr("%")
                    font.bold: true
                }
            }
            Text{
                anchors.left: fusionRow.left
                anchors.top: fusionRow.bottom
                anchors.topMargin: 15
                text: qsTr("Default: 70%")
            }
            Button{
                text: qsTr("Reverse Detection")
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 30
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: childRectangle.color = "green"
                background:Rectangle{
                    id: childRectangle
                    color: "white"
                    border.color: "black"
                }
            }
        }
        Rectangle{
            id: dualAntennaSettings
            anchors.top: antennaType.bottom
            anchors.right: singleAntennaSettings.left
            anchors.left: parent.left
            anchors.bottom: rtkAlarm.top
            anchors.margins: 5
            border.color: "black"
            color: "ghostwhite"
            Text{
                id: textdual
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 5
                text: qsTr("Dual Antenna Settings")
            }
            Image {
                id: head
                source: "qrc:images/Config/Con_SourcesHead.png"
                height: 100
                width: 100
                anchors.top: parent.top
                anchors.topMargin: 30
                anchors.left: parent.left
                anchors.leftMargin: 30
            }
            SpinBox{
                id: headingOffSet
                anchors.left: head.right
                anchors.bottom: head.bottom
                anchors.leftMargin: 50
                from: -100
                value: 0
                to: 100
                editable: true
            }
            Text{
                anchors.bottom: headingOffSet.top
                anchors.bottomMargin: 15
                anchors.horizontalCenter: headingOffSet.horizontalCenter
                text: qsTr("Heading Offset (Degree)")
            }
            Button{
                id: dualAsIMU
                text: qsTr("Dual As IMU")
                font.bold: true
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 30
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: childofdual.color = "green"
                background:Rectangle{
                    id: childofdual
                    color: "white"
                    border.color: "black"
                }
            }
        }
    }
    Rectangle{
        id: configSrcRollWin
        anchors.fill: parent
        color: "ghostwhite"
        visible: false
        Text{
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.topMargin: 20
            text: qsTr("Remove Offset")
            Rectangle{
                width:150
                anchors.left: parent.left
                anchors.top: parent.bottom
                height:100
                radius:5
                border.color: "black"
                IconButtonTransparent{
                    anchors.fill:parent
                    icon: "qrc:/images/Config/ConDa_RemoveOffset.png"
                    colorHover1: "transparent"
                    checkable: true
                }
            }
        }
        Text{
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 20
            text: qsTr("Zero Roll")
            Rectangle{
                width:150
                anchors.left: parent.left
                anchors.top: parent.bottom
                height:100
                radius:5
                border.color: "black"
                IconButtonTransparent{
                    anchors.fill:parent
                    icon: "qrc:/images/Config/ConDa_RollSetZero.png"
                    colorHover1: "transparent"
                    checkable: true
                }
            }
        }
        Rectangle{
            width:150
            anchors.left: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20
            height:100
            radius:5
            border.color: "black"
            IconButtonTransparent{
                anchors.fill:parent
                icon: "qrc:/images/Config/ConDa_ResetIMU.png"
                colorHover1: "transparent"
                checkable: true
            }
        }
        Text{
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.horizontalCenter
            anchors.topMargin: 20
            text: qsTr("Invert Roll")
            Rectangle{
                width:150
                anchors.left: parent.left
                anchors.top: parent.bottom
                height:100
                radius:5
                border.color: "black"
                IconButtonTransparent{
                    anchors.fill:parent
                    icon: "qrc:/images/Config/ConDa_InvertRoll.png"
                    colorHover1: "transparent"
                    checkable: true
                }
            }
        }
        Text{
            anchors.bottom:  rollFilterSlider.top
            anchors.left: rollFilterSlider.left
            text: qsTr("Less")
        }
        Text{
            anchors.bottom: rollFilterSlider.top
            anchors.horizontalCenter: rollFilterSlider.horizontalCenter
            text: qsTr("Roll Filter")
        }
        Text {
            anchors.right: rollFilterSlider.right
            anchors.bottom: rollFilterSlider.top
            text: qsTr("More")
        }
       Rectangle{
            id: rollFilterSlider
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.bottomMargin: 50
            anchors.leftMargin: 50
            width: 270
            height: 50
            color: "lightgray"
        Slider{
            objectName: "rollFilterSlider"
            anchors.fill: parent
            from: 0
            to: 98
            value: 0
        }
       }
       Image {
           source: "qrc:/images/Config/ConD_RollHelper"
           anchors.right: parent.right
           anchors.rightMargin: 50
           width: 150
           height: 200
           anchors.verticalCenter: parent.verticalCenter
       }
    }
}
