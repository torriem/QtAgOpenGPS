import QtQuick 2.0
import QtQuick.Controls 2.5

/* todo:
  */
Rectangle{
    id: configSources
    anchors.fill: parent
    visible: true
    color: "ghostwhite"
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
            height: childrenRect.height
            width: childrenRect.width
            spacing: 10
            IconButtonColor{
                width:150
                height:100
                text: "Dual"
                id: dual
                icon.source: "/images/Config/Con_SourcesGPSDual.png"
                checked: setting.setGPS_fixFromWhichSource === "Fix"
                onClicked: settings.setGPS_fixFromWhichSource = "Fix"
            }
            IconButtonColor{
                width:150
                height:100
                id: fix
                text: "Fix"
                checked: setting.setGPS_fixFromWhichSource === "Dual"
                onClicked: settings.setGPS_fixFromWhichSource = "Dual"
                icon.source: "/images/Config/Con_SourcesGPSSingle.png"
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
            IconButtonColor{
                width:150
                height:100
                id: alarm
                icon.source: "/images/Config/Con_SourcesRTKAlarm.png"
                checked: settings.setGPS_ageAlarm
                onCheckedChanged: settings.setGPS_ageAlarm
            }
            Text{
                text: "  ->  "
            }
            IconButtonColor{
                width:150
                height:100
                id: killAutoSteer
                icon.source: "/images/AutoSteerOff.png"
                checked: settings.setGPS_isRTK_KillAutoSteer
                onCheckedChanged: settings.setGPS_isRTK_KillAutoSteer
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

        ButtonColor{
            id: minGPSStep
            width: 180
            height: 50
            anchors.top: singleText.bottom
            anchors.right: parent.right
            anchors.margins: 5
            color: "blue"
            colorChecked: "green"
            onCheckedChanged: {
                if(checked){
                    settings.setGPS_minimumStepLimit = 0.1
                }else{
                    settings.setGPS_minimumStepLimit = 0.05
                }
            }
            text: qsTr(settings.setGPS_minimumStepLimit * 100 + "Centimeter")
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
                anchors.right: minGPSStep.left
                text: qsTr("Heading Distance")
            }
            Text{
                anchors.top: minGPSStep.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr(settings.setGPS_minimumStepLimit * 1000 + "Centimeter")
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
            SliderCustomized{
                leftText: 100 - value
                rightText: value
                leftTopText: "IMU <"
                colorLeftTopText: "green"
                anchors.centerIn: parent
                rightTopText: "> GPS"
                colorRightTopText: "red"
                centerTopText: "Fusion"
                from: 20
                to: 40
                value: settings.setIMU_fusionWeight2 * 100
                onValueChanged: {
                    settings.setIMU_fusionWeight2 = value / 100
                    console.log(settings.setIMU_fusionWeight2)
                }
                stepSize: 1
            }
        }
        Text{
            anchors.left: fusionRow.left
            anchors.top: fusionRow.bottom
            anchors.topMargin: 15
            text: qsTr("Default: 70%")
        }
        ButtonColor{
            text: qsTr("Reverse Detection")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            colorChecked: "green"
            checked: settings.setIMU_isReverseOn
            onCheckedChanged: settings.setIMU_isReverseOn = checked
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
            source: "/images/Config/Con_SourcesHead.png"
            height: 100
            width: 100
            anchors.top: parent.top
            anchors.topMargin: 30
            anchors.left: parent.left
            anchors.leftMargin: 30
        }
        SpinBoxOneDecimal{
            id: headingOffSet
            anchors.left: head.right
            anchors.bottom: head.bottom
            anchors.leftMargin: 50
            from: -100
            value: settings.setGPS_dualHeadingOffset
            //onValueChanged: settings.setGPS_dualHeadingOffset = value
            to: 100
            editable: true
        }
        Text{
            anchors.bottom: headingOffSet.top
            anchors.bottomMargin: 15
            anchors.horizontalCenter: headingOffSet.horizontalCenter
            text: qsTr("Heading Offset (Degree)")
        }
        ButtonColor{
            id: dualAsIMU
            objectName: "dualAsIMU"
            text: qsTr("Dual As IMU")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
            colorChecked: "green"
            checked: settings.setIMU_isDualAsIMU
            onCheckedChanged: settings.setIMU_isDualAsIMU = checked
        }
    }
}
