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
            id: sourceRow
            anchors.top: text.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            height: childrenRect.height
            width: childrenRect.width
            spacing: 10

            ButtonGroup {
                buttons: [ dualBtn, fixBtn ]
            }

            IconButtonColor{
                id: dualBtn
                width:150
                height:100
                text: "Dual"
                checkable: true
                icon.source: "/images/Config/Con_SourcesGPSDual.png"

                property string headingSource: settings.setGPS_headingFromWhichSource

                checked: (settings.setGPS_headingFromWhichSource === "Dual" ? true: false)

                onCheckedChanged: {
                    if (checked)
                        settings.setGPS_headingFromWhichSource = "Dual"
                }

                onHeadingSourceChanged: {
                    if(headingSource === "Dual"){
                        dualBtn.checked = true
                    }else{
                        dualBtn.checked = false
                    }
                }

            }
            IconButtonColor{
                id: fixBtn
                width:150
                height:100
                text: "Fix"
                checkable: true
                icon.source: "/images/Config/Con_SourcesGPSSingle.png"

                property string headingSource: settings.setGPS_headingFromWhichSource

                checked: (settings.setGPS_headingFromWhichSource === "Fix" ? true : false)

                onCheckedChanged: {
                    if(checked)
                        settings.setGPS_headingFromWhichSource = "Fix"
                }

                onHeadingSourceChanged: {
                    if(headingSource === "Fix"){
                        fixBtn.checked = true
                    }else{
                        fixBtn.checked = false
                    }
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

            IconButtonColor{
                width:150
                height:100
                id: alarm
                icon.source: "/images/Config/Con_SourcesRTKAlarm.png"
                isChecked: settings.setGPS_ageAlarm
                onClicked: settings.setGPS_ageAlarm = true
            }
            Text{
                text: "  ->  "
            }
            IconButtonColor{
                width:150
                height:100
                checkable: true
                id: killAutoSteer
                icon.source: "/images/AutoSteerOff.png"
                isChecked: settings.setGPS_isRTK_KillAutoSteer
                onClicked: settings.setGPS_isRTK_KillAutoSteer = true
            }
        }
    }
    Rectangle{
        id:singleAntennaSettings
        anchors.bottom: parent.bottom
        border.color: fixBtn.checked ? "black" : "grey"
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
            color: fixBtn.checked ? "black" : "grey"
        }

        Row {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 10

            id: minGPSStep
            Label {
                color: fixBtn.checked ? "black" : "grey"
                text: qsTr("Minimum GPS Step")
                width: singleAntennaSettings.width * 0.5
            }
            ButtonColor{
                id: minGPSStepBtn
                width: 180
                height: 50
                checkable: true
                enabled: fixBtn.checked

                color: "light blue"

                onCheckedChanged: {
                    if(checked){
                        settings.setGPS_minimumStepLimit = 0.1
                        settings.setF_minHeadingStepDistance = 1
                    }else{
                        settings.setGPS_minimumStepLimit = 0.05
                        settings.setF_minHeadingStepDistance = 0.5
                    }
                }
                text: settings.setGPS_minimumStepLimit * 100 + " " + qsTr("cm", "centimeter abbreviation")
            }
        }
        Row {
            id: headingDistance
            anchors.top: minGPSStep.bottom
            anchors.left: parent.left
            anchors.topMargin: 10

            Label {
                color: fixBtn.checked ? "black" : "grey"
                text: qsTr("Heading Distance")
                width: singleAntennaSettings.width * 0.5
            }
            Label {
                id: headingDistanceText
                width: minGPSStepBtn.width
                color: fixBtn.checked ? "black" : "grey"
                text: settings.setF_minHeadingStepDistance * 100 + " " + qsTr("cm", "centimeter abbreviation")
            }
        }

        Rectangle{//fusion adjuster
            id: fusionRow
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: 360
            height: 100
            border.color: fixBtn.checked ? "black" : "grey"
            color: "ghostwhite"
            SliderCustomized{
                enabled: fixBtn.checked
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

                property int fusionWeight: settings.setIMU_fusionWeight2

                onFusionWeightChanged: {
                    value = fusionWeight * 100
                }

                value: settings.setIMU_fusionWeight2 * 100
                onValueChanged: {
                    settings.setIMU_fusionWeight2 = value / 100
                }
                stepSize: 1
            }
        }

        Text{
            anchors.left: fusionRow.left
            anchors.top: fusionRow.bottom
            anchors.topMargin: 15
            color: fixBtn.checked ? "black" : "grey"
            text: qsTr("Default: 70%")
        }


        ButtonColor{
            text: qsTr("Reverse Detection")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            enabled: fixBtn.checked
            color: "white"
            colorChecked: "green"
            isChecked: settings.setIMU_isReverseOn
            onClicked: settings.setIMU_isReverseOn = checked
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
            color: dualBtn.checked ? "black" : "grey"
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
            enabled: dualBtn.checked ? "black" : "grey"
            from: -100
            boundValue: settings.setGPS_dualHeadingOffset
            onValueChanged: settings.setGPS_dualHeadingOffset = value
            to: 100
            editable: true
        }
        Text{
            anchors.bottom: headingOffSet.top
            anchors.bottomMargin: 15
            anchors.horizontalCenter: headingOffSet.horizontalCenter
            text: qsTr("Heading Offset (Degree)")
            color: dualBtn.checked ? "black" : "grey"
        }
        ButtonColor{
            id: dualAsIMU
            objectName: "dualAsIMU"
            text: qsTr("Dual As IMU")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            enabled: dualBtn.checked
            checkable: true
            color: "white"
            colorChecked: "green"
            isChecked: settings.setIMU_isDualAsIMU
            onClicked: settings.setIMU_isDualAsIMU = checked
        }
    }
}
