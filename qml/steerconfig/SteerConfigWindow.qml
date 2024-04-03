import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ".."
import "../components"

MoveablePopup {
    id: steerConfigWindow
    closePolicy: Popup.NoAutoClose
    modal: false
    visible: false
    height: 475
    width:400
    x: settings.setWindow_steerSettingsLocation.x
    y: settings.setWindow_steerSettingsLocation.y
    function show (){
        steerConfigWindow.visible = true
    }

    Rectangle{
        id: steerConfigFirst
        color: aog.backgroundColor
        border.color: aog.blackDayWhiteNight
        border.width: 1
        visible: true
        anchors.fill: parent
        Rectangle{
            id:topLine
            color:"white"
            visible: true
            width: parent.width
            height: 25
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            Text{
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Auto Steer Config")
            }

            Button {
                id: close
                text: qsTr("×")
                font.pixelSize: parent.height
                width: parent.height+3
                height:parent.height
                anchors.right: parent.right
                onClicked: {
                    if(!pwmWindow.visible){
                        settings.setWindow_steerSettingsLocation.x = steerConfigWindow.x
                        settings.setWindow_steerSettingsLocation.y = steerConfigWindow.y
                    }
                    steerConfigWindow.visible = false
                }
            }
            Button {
                id: help
                text: qsTr("?")
                width: parent.height+3
                height:parent.height
                anchors.right: close.left
                anchors.rightMargin: 20
            }
        }
        Item{
            id: steerSlidersConfig
            height: 475
            width:400
            anchors.left: parent.left
            anchors.top: topLine.bottom
            ButtonGroup {
                buttons: buttonsTop.children
            }

            RowLayout{
                id: buttonsTop
                width: parent.width
                IconButtonColor{
                    id: steerBtn
                    icon.source: "/images/Steer/ST_SteerTab.png"
                    implicitWidth: parent.width /4 - 4
                    implicitHeight: 50
                    checkable: true
                    checked: true
                    colorChecked: "lightgray"
                }
                IconButtonColor{
                    id: gainBtn
                    icon.source: "/images/Steer/ST_GainTab.png"
                    implicitWidth: parent.width /4 - 4
                    implicitHeight: 50
                    checkable: true
                    colorChecked: "lightgray"
                }
                IconButtonColor{
                    id: stanleyBtn
                    icon.source: "/images/Steer/ST_StanleyTab.png"
                    implicitWidth: parent.width /4 - 4
                    implicitHeight: 50
                    checkable: true
                    colorChecked: "lightgray"
                }
                IconButtonColor{
                    id: ppBtn
                    icon.source: "/images/Steer/Sf_PPTab.png"
                    implicitWidth: parent.width /4 - 4
                    implicitHeight: 50
                    checkable: true
                    colorChecked: "lightgray"
                }
            }

            Item{
                id: tabArea
                anchors.top: buttonsTop.bottom
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.bottom: angleInfo.top
                WheelAngleSensorTab{
                    anchors.fill: parent
                    visible: steerBtn.checked
                }
                PWMSettingsTab{
                    anchors.fill: parent
                    visible: gainBtn.checked
                }
                StanleyTab{
                    anchors.fill: parent
                    visible: stanleyBtn.checked
                }
                PurePursuitTab{
                    anchors.fill: parent
                    visible: ppBtn.checked
                }
            }

            Rectangle{
                id: angleInfo
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 50
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        function showHide(){
                            if(pwmWindow.visible == false){//show full menu
                                steerConfigWindow.height = 768
                                steerConfigWindow.width=1024
                                pwmWindow.visible = true
                                steerConfig.visible = true
                                settings.setWindow_steerSettingsLocation.x = steerConfigWindow.x
                                settings.setWindow_steerSettingsLocation.y = steerConfigWindow.y
                                steerConfigWindow.x = 0
                                steerConfigWindow.y = 0
                            }else{//hide
                                steerConfigWindow.height = 465
                                steerConfigWindow.width=400
                                pwmWindow.visible = false
                                steerConfig.visible = false
                                steerConfigWindow.x = settings.setWindow_steerSettingsLocation.x
                                steerConfigWindow.y = settings.setWindow_steerSettingsLocation.y
                            }
                        }
                        showHide()
                    }
                }

                Text {
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Set: " + aog.steerAngleSetRounded)
                }
                Text {
                    text: qsTr("Act: " + aog.steerAngleActualRounded)
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    id: errorlbl
                    property double err: aog.steerAngleActualRounded - aog.steerAngleSetRounded
                    text: qsTr("Err: " + err)
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    onErrChanged: err > 0 ? errorlbl.color = "red" : errorlbl.color = "darkgreen"
                }
            }
        }
        Rectangle{
            id: pwmWindow
            visible: false
            anchors.top: steerSlidersConfig.bottom
            anchors.left: steerSlidersConfig.left
            width: steerSlidersConfig.width
            anchors.bottom: parent.bottom
            anchors.margins: 8
            RowLayout{
                id: pwmRow
                anchors.top: parent.top
                anchors.margins: 10
                anchors.left: parent.left
                width: parent.width
                height: 50
                IconButton{
                    id: pwmSteer
                    isChecked: false
                    icon.source: "/images/SteerDriveOff.png"
                    iconChecked: "/images/SteerDriveOn.png"
                    implicitWidth: parent.width/4.5
                    implicitHeight: parent.height
                    color3: "white"
                    border: 2
                }
                IconButton{
                    icon.source: "/images/SnapLeft.png"
                    implicitWidth: parent.width/4.5
                    implicitHeight: parent.height
                    color3: "white"
                    border: 2
                }
                IconButton{
                    icon.source: "/images/SnapRight.png"
                    implicitWidth: parent.width/4.5
                    implicitHeight: parent.height
                    color3: "white"
                    border: 2
                }
                IconButton{
                    id: pwmZero
                    icon.source: "/images/SteerZeroSmall.png"
                    implicitWidth: parent.width/4.5
                    implicitHeight: parent.height
                    color3: "white"
                    border: 2
                }
            }
            Text{
                anchors.left: pwmRow.left
                anchors.top: pwmRow.bottom
                text: qsTr("PWM: ")
            }
            Text{
                anchors.right: pwmRow.right
                anchors.rightMargin: 50
                anchors.top: pwmRow.bottom
                text: qsTr("0r +5")
                font.pixelSize: 15
            }
            IconButton{
                id: steerRecord
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                isChecked: false
                width: 75
                height: 75
                color3: "white"
                border: 2
                icon.source: "/images/BoundaryRecord.png"
                iconChecked: "/images/Stop"
            }
            Text{
                anchors.top: steerRecord.top
                anchors.left: steerRecord.right
                text: qsTr("Steer Angle: ")
            }
            Text{
                anchors.bottom: steerRecord.bottom
                anchors.left: steerRecord.right
                text: qsTr("Diameter: ")
            }
        }
        Rectangle{
            id: steerConfig
            anchors.left: steerSlidersConfig.right
            anchors.top: topLine.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 10
            color: "lightgray"
            visible: false
            ButtonGroup{
                buttons: settingsBtns.children
            }

            RowLayout{
                id: settingsBtns
                spacing: 3
                width: parent.width
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                IconButtonTextBeside{
                    id: sensorsBtn
                    icon.source: "/images/Config/ConD_Speedometer.png"
                    buttonText: "Sensors"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    checked: true
                    colorChecked: "lightgray"
                    implicitWidth: parent.width /3 -4
                }
                IconButtonTextBeside{
                    id: configBtn
                    icon.source: "/images/Config/ConS_Pins.png"
                    buttonText: "Config"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    colorChecked: "lightgray"
                    implicitWidth: parent.width /3 -4
                }
                IconButtonTextBeside{
                    id: settingsBtn
                    icon.source: "/images/Config/ConS_ImplementConfig.png"
                    buttonText: "Settings"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    colorChecked: "lightgray"
                    implicitWidth: parent.width /3 -4
                }
            }
            Item{
                id: settingsArea
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: settingsBtns.bottom
                anchors.bottom: bottomRightButtons.top
                anchors.margins: 10
                SensorsTab{
                    anchors.fill: parent
                    visible: sensorsBtn.checked
                }
                ConfigTab{
                    anchors.fill: parent
                    visible: configBtn.checked
                }
                SettingsTab{
                    anchors.fill: parent
                    visible: settingsBtn.checked
                }
            }
            RowLayout{
                id: bottomRightButtons
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                height: 70
                IconButtonText{
                    id: wizard
                    text: qsTr("Wizard")
                    icon.source: "/images/WizardWand.png"
                    Layout.alignment: Qt.AlignCenter
                }
                IconButtonText{
                    id: reset
                    text: qsTr("Reset to Defaults")
                    icon.source: "/images/Reset_Default.png"
                    Layout.alignment: Qt.AlignCenter
                }
                Text {
                    text: qsTr("Send + Save")
                    Layout.alignment: Qt.AlignRight
                }
                IconButton{
                    id: send
                    Layout.alignment: Qt.AlignLeft
                    icon.source: "/images/ToolAcceptChange.png"
                    implicitWidth: 130
                }
            }
        }
    }
}
