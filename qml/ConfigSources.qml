import QtQuick 2.0
import QtQuick.Controls 2.5

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
                buttonText: "Dual"
                id: dual
                icon: "/images/Config/Con_SourcesGPSDual.png"
                isChecked: false
            }
            IconButtonColor{
                width:150
                height:100
                id: fix
                buttonText: "Fix"
                isChecked: false
                icon: "/images/Config/Con_SourcesGPSSingle.png"
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
                icon: "/images/Config/Con_SourcesRTKAlarm.png"
                isChecked: false
            }
            Text{
                text: qsTr("  ->  ")
            }
            IconButtonColor{
                width:150
                height:100
                id: killAutoSteer
                icon: "/images/AutoSteerOff.png"
                isChecked: false
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
            SliderCustomized{
                leftText: value
                rightText: value
                leftTopText: "IMU <"
                colorLeftTopText: "green"
                anchors.centerIn: parent
                rightTopText: "> GPS"
                colorRightTopText: "red"
                centerTopText: "Fusion"
                from: 50
                to: 90
                value: 70
                stepSize: 1
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
            source: "/images/Config/Con_SourcesHead.png"
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
        IconButtonText{
            id: dualAsIMU
            objectName: "dualAsIMU"
            text: qsTr("Dual As IMU")
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
