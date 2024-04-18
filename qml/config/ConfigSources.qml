import QtQuick
import QtQuick.Controls.Fusion

import ".."
import "../components"

/* todo:
  */
Rectangle{
    id: configSources
    anchors.fill: parent
    visible: true
    color: aog.backgroundColor
    TitleFrame {
        id:antennaType
        width: 360
        height: sourceRow.height + titleHeight + 10
        anchors.top: parent.top
        anchors.margins: 5
        anchors.horizontalCenter: parent.horizontalCenter
        color: aog.backgroundColor
        border.color: aog.blackDayWhiteNight
        visible: true
        title: qsTr("Antenna Type", "GPS Antenna type, fixed or dual")
        font.pointSize: 16

        Row{
            id: sourceRow
            anchors.top: parent.top
            anchors.margins: 10
            anchors.topMargin: 25
            anchors.horizontalCenter: parent.horizontalCenter
            height: childrenRect.height  + 25
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

    TitleFrame {
        id:rtkAlarm
        width: parent.width /2
        height: rtkAlarmRow.height + titleHeight + 10
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 5
        color: aog.backgroundColor
        visible: true
        title: qsTr("RTK Alarm")
        font.pointSize: 16

        Row{
            id: rtkAlarmRow
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10
            height: childrenRect.height + 10

            Component.onCompleted: console.debug(height)

            IconButtonColor{
                width:150
                height:100
                id: alarm
                icon.source: "/images/Config/Con_SourcesRTKAlarm.png"
                isChecked: settings.setGPS_isRTK
                onClicked: settings.setGPS_isRTK = true
            }
            Rectangle {
                height: 100
                width: childrenRect.width

                Text{
                    anchors.verticalCenter: parent.verticalCenter
                    text: "  ->  "
                }
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
    TitleFrame{
        id:singleAntennaSettings
        anchors.bottom: parent.bottom
        enabled: fixBtn.checked
        anchors.right: parent.right
        anchors.left: rtkAlarm.right
        anchors.top: antennaType.bottom
        anchors.margins: 5

        border.color: enabled ? aog.blackDayWhiteNight : "grey"

        color: aog.backgroundColor

        title: qsTr("Single Antenna Settings")
        font.pointSize: 16

        Row {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 10
            anchors.leftMargin: 10

            id: minGPSStep
            Label {
                color: fixBtn.checked ? aog.blackDayWhiteNight : "grey"
                text: qsTr("Minimum GPS Step")
                width: singleAntennaSettings.width * 0.5
            }
            ButtonColor{
                id: minGPSStepBtn
                width: 180
                height: 50
                checkable: true

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
            anchors.leftMargin: 10

            Label {
                color: fixBtn.checked ? aog.blackDayWhiteNight : "grey"
                text: qsTr("Heading Distance")
                width: singleAntennaSettings.width * 0.5
            }
            Label {
                id: headingDistanceText
                width: minGPSStepBtn.width
                color: fixBtn.checked ? aog.blackDayWhiteNight : "grey"
                text: settings.setF_minHeadingStepDistance * 100 + " " + qsTr("cm", "centimeter abbreviation")
            }
        }

        Rectangle{//fusion adjuster
            id: fusionRow
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: 360
            height: 100
            border.color: fixBtn.checked ? aog.blackDayWhiteNight : "grey"
            color: aog.backgroundColor
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
            color: fixBtn.checked ? aog.blackDayWhiteNight : "grey"
            text: qsTr("Default: 70%")
        }


        ButtonColor{
            text: qsTr("Reverse Detection")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            color: aog.backgroundColor
            colorChecked: "green"
            isChecked: settings.setIMU_isReverseOn
            onClicked: settings.setIMU_isReverseOn = checked
        }
    }
    TitleFrame {
        id: dualAntennaSettings
        anchors.top: antennaType.bottom
        anchors.right: singleAntennaSettings.left
        anchors.left: parent.left
        anchors.bottom: rtkAlarm.top
        anchors.margins: 5
        border.color: aog.blackDayWhiteNight
        color: aog.backgroundColor
        title: qsTr("Dual Antenna Settings")
        font.pointSize: 16
        enabled: dualBtn.checked

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
            boundValue: settings.setGPS_dualHeadingOffset
            onValueChanged: settings.setGPS_dualHeadingOffset = value
            to: 100
            editable: true
            text: qsTr("Heading Offset (Degrees)")
        }
        ButtonColor{
            id: dualAsIMU
            objectName: "dualAsIMU"
            text: qsTr("Dual As IMU")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30
            anchors.horizontalCenter: parent.horizontalCenter
            checkable: true
            color: aog.backgroundColor
            colorChecked: "green"
            isChecked: settings.setIMU_isDualAsIMU
            onClicked: settings.setIMU_isDualAsIMU = checked
        }
    }
}
