import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

MoveablePopup {
    id: steerConfigWindow
    closePolicy: Popup.NoAutoClose
    modal: false
    visible: false
    height: 600
    width:450
    x: settings.setWindow_steerSettingsLocation.x
    y: settings.setWindow_steerSettingsLocation.y

    Rectangle{
        id: steerConfigFirst
        color: "#cccccc"
        border.color: "black"
        border.width: 1
        visible: true
        anchors.fill: parent
        Rectangle{
            id:topLine
            color:"white"
            visible: true
            width: parent.width-2
            height: 40
            anchors.top: parent.top
            anchors.topMargin: 1
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
            anchors.left: parent.left
            anchors.top: topLine.bottom
            height: 500
            width:450
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
                    checkable: true
                    checked: true
                    colorChecked: "lightgray"
                }
                IconButtonColor{
                    id: gainBtn
                    icon.source: "/images/Steer/ST_GainTab.png"
                    implicitWidth: parent.width /4 - 4
                    checkable: true
                    colorChecked: "lightgray"
                }
                IconButtonColor{
                    id: stanleyBtn
                    icon.source: "/images/Steer/ST_StanleyTab.png"
                    implicitWidth: parent.width /4 - 4
                    checkable: true
                    colorChecked: "lightgray"
                }
                IconButtonColor{
                    id: ppBtn
                    icon.source: "/images/Steer/Sf_PPTab.png"
                    implicitWidth: parent.width /4 - 4
                    checkable: true
                    colorChecked: "lightgray"
                }
            }

            Item{
                id: steerTab
                visible: steerBtn.checked
                anchors.top: buttonsTop.bottom
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.bottom: angleInfo.top
                Column{
                    id: steerColumn
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    height: childrenRect.height
                    width: childrenRect.width
                    anchors.margins: 5
                    anchors.leftMargin: 20
                    spacing: 45
                    SliderCustomized {
                        id: wasZeroSlider
                        objectName: "wasZeroSlider"
                        width: 200
                        leftText: value
                        from: -30//idk
                        to: 30
                        value: 5
                        centerTopText: "WAS Zero"
                    }
                    SliderCustomized {
                        id: cpDegSlider
                        objectName: "cpDegSlider"
                        width: 200
                        centerTopText: "Counts per Degree"
                        from: 1
                        to: 255
                        value: Math.round(settings.setAS_countsPerDegree, 0)
                        onValueChanged: settings.setAS_countsPerDegree = value
                        leftText: value
                        stepSize: 1
                    }
                    SliderCustomized {
                        id: ackermannSlider
                        objectName: "ackermannSlider"
                        width: 200
                        centerTopText: "AckerMann"
                        from: 1
                        to: 200
                        leftText: value
                        value: Math.round(settings.setAS_ackerman, 0)
                        onValueChanged: settings.setAS_ackerman = value
                        stepSize: 1
                    }
                    SliderCustomized {
                        id: maxSteerSlider
                        objectName: "maxSteerSlider"
                        width: 200
                        leftText: value
                        centerTopText:"Max Steer Angle"
                        from: 10
                        to: 80
                        value: Math.round(settings.setVehicle_maxSteerAngle)
                        onValueChanged: settings.setVehicle_maxSteerAngle= value
                        stepSize: 1
                    }
                }
                Image {
                    width: parent.width
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    height: ppColumn.height
                    source: "/images/Steer/Sf_SteerTab.png"
                }

            }
            Item{
                id: gainTab
                anchors.top: buttonsTop.bottom
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.bottom: angleInfo.top
                visible: gainBtn.checked
                Column{
                    id: gainColumn
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    height: childrenRect.height
                    width: childrenRect.width
                    anchors.margins: 5
                    spacing: 45
                    anchors.leftMargin: 20
                    SliderCustomized {
                        id: propGainlider
                        objectName: "progGainSlider"
                        leftText: value
                        width: 200
                        from: 0
                        to: 200
                        value: Math.round(settings.setAS_Kp, 0)
                        onValueChanged: settings.setAS_Kp = value
                        centerTopText: "Proportional Gain"
                        stepSize: 1
                    }
                    SliderCustomized {
                        id: maxLimitSlider
                        objectName: "maxLimitSlider"
                        centerTopText: "Maximum Limit"
                        leftText: value
                        width: 200
                        from: 0
                        to: 254
                        value: Math.round(settings.setAS_highSteerPWM, 0)
                        onValueChanged: settings.setAS_highSteerPWM = value
                        stepSize: 1
                    }
                    SliderCustomized {
                        id: min2moveSlider
                        objectName: "min2moveSlider"
                        width: 200
                        from: 0
                        to: 100
                        value: Math.round(settings.setAS_minSteerPWM, 0)
                        onValueChanged: settings.setAS_minSteerPWM = value
                        leftText: value
                        centerTopText: "Minimum to Move"
                        stepSize: 1
                    }
                }
                Image {
                    width: parent.width
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    height: ppColumn.height
                    source: "/images/Steer/Sf_GainTab.png"
                }

            }
            Item{
                id: stanleyTab
                anchors.top: buttonsTop.bottom
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.bottom: angleInfo.top
                visible: stanleyBtn.checked
                Column{
                    id: stanleyColumn
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    height: childrenRect.height
                    width: childrenRect.width
                    anchors.margins: 5
                    spacing: 45
                    anchors.leftMargin: 20
                    SliderCustomized {
                        id: stanleyAggressivenessSlider
                        objectName: "stanleyAgressivenessSlider"
                        width: 200
                        from: .1
                        to: 4
                        value: Math.round(settings.stanleyDistanceErrorGain, 0)
                        onValueChanged: settings.stanleyDistanceErrorGain = value
                        leftText: value
                        centerTopText: "Agressiveness"
                        stepSize: .1
                    }
                    SliderCustomized {
                        id: overShootReductionSlider
                        objectName: "overShootReductionSlider"
                        width: 200
                        from: .1
                        to: 1.5
                        value: Math.round(settings.stanleyHeadingErrorGain, 0)
                        onValueChanged: settings.stanleyHeadingErrorGain = value
                        leftText: value
                        centerTopText: "OverShoot Reduction"
                        stepSize: .1
                    }
                    SliderCustomized {
                        id: integralStanleySlider
                        objectName: "integralStanleySlider"
                        width: 200
                        from: 0
                        to: 100
                        value: Math.round(settings.stanleyIntegralGainAB * 100, 0)
                        onValueChanged: settings.stanleyIntegralGainAB = value /100
                        leftText: value
                        centerTopText: "Integral"
                        stepSize: 1
                    }
                }
                Image {
                    width: parent.width
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    height: ppColumn.height
                    source: "/images/Steer/Sf_Stanley.png"
                }

            }
            Item{
                id: ppTab
                visible: ppBtn.checked
                anchors.top: buttonsTop.bottom
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.bottom: angleInfo.top
                Column{
                    id: ppColumn
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    height: childrenRect.height
                    width: childrenRect.width
                    anchors.margins: 5
                    spacing: 45
                    anchors.leftMargin: 20
                    SliderCustomized {
                        id: acqLookAheadSlider
                        objectName: "acqLookAheadSlider"
                        width: 200
                        from: 1
                        to: 5
                        value: Math.round(settings.setVehicle_goalPointLookAhead, 1)
                        onValueChanged: settings.setVehicle_goalPointLookAhead = value
                        leftText: value
                        centerTopText: "Acquire Look Ahead"
                        stepSize: .1
                    }
                    SliderCustomized {
                        id: holdLookAheadSlider
                        objectName: "holdLookAheadSlider"
                        width: 200
                        from: 1
                        to: 5
                        value: Math.round(settings.setVehicle_goalPointLookAheadHold, 1)
                        onValueChanged: settings.setVehicle_goalPointLookAheadHold = value
                        leftText: value
                        centerTopText: "Hold Look Ahead"
                        stepSize: .1
                    }
                    SliderCustomized {
                        id: lookAheadSpeedGainSlider
                        objectName: "lookAheadSpeedGainSlider"
                        width: 200
                        from: .5
                        to: 3
                        value: Math.round(settings.setVehicle_goalPointLookAheadMult, 1)
                        onValueChanged: settings.setVehicle_goalPointLookAheadMult = value
                        leftText: value
                        centerTopText: "Look Ahead Speed Gain"
                        stepSize: .1
                    }
                    SliderCustomized {
                        id: ppIntegralSlider
                        objectName: "ppIntegralSlider"
                        width: 200
                        from: 0
                        to: 100
                        value: Math.round(settings.purePursuitIntegralGainAB *100, 0)
                        onValueChanged: settings.purePursuitIntegralGainAB = value /100
                        leftText: value
                        centerTopText: "Integral"
                        stepSize: 1
                    }
                }
                Image {
                    width: parent.width
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    height: ppColumn.height
                    source: "/images/Steer/Sf_PP.png"
                }
            }
            Rectangle{
                id: angleInfo
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 8
                anchors.left: buttonsTop.left
                width: buttonsTop.width
                height: 30
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
                                steerConfigWindow.height = 600
                                steerConfigWindow.width=450
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
                    text: qsTr("Set: ")
                }
                Text {
                    text: qsTr("Act: ")
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    text: qsTr("Set: ")
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
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
                    objectName: "pwmYesNo"
                    isChecked: false
                    icon.source: "/images/SteerDriveOff.png"
                    iconChecked: "/images/SteerDriveOn.png"
                    implicitWidth: parent.width/4.5
                    implicitHeight: parent.height
                    color3: "white"
                    border: 2
                }
                IconButton{
                    objectName: "pwmLeft"
                    icon.source: "/images/SnapLeft.png"
                    implicitWidth: parent.width/4.5
                    implicitHeight: parent.height
                    color3: "white"
                    border: 2
                }
                IconButton{
                    objectName: "pwmRight"
                    icon.source: "/images/SnapRight.png"
                    implicitWidth: parent.width/4.5
                    implicitHeight: parent.height
                    color3: "white"
                    border: 2
                }
                IconButton{
                    id: pwmZero
                    objectName: "pwmZero"
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
                objectName: "steerRecord"
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
            visible: false
            color: "lightgray"
            ButtonGroup{
                buttons: settingsBtns.children
            }

            Row{
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
                    width: parent.width /3 -4
                }
                IconButtonTextBeside{
                    id: configBtn
                    icon.source: "/images/Config/ConS_Pins.png"
                    objectName: "btnCurrentTurnSensor"
                    buttonText: "Config"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    colorChecked: "lightgray"
                    width: parent.width /3 -4
                }
                IconButtonTextBeside{
                    id: settingsBtn
                    icon.source: "/images/Config/ConS_ImplementConfig.png"
                    buttonText: "Settings"
                    Layout.alignment: Qt.AlignCenter
                    checkable: true
                    colorChecked: "lightgray"
                    width: parent.width /3 -4
                }
            }

            RowLayout{
                id: sensorsWindow
                visible: sensorsBtn.checked
                width: parent.width -100
                height: parent.height -100
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                IconButtonColor{
                    icon.source: "/images/Config/ConSt_TurnSensor.png"
                    objectName: "btnTurnSensor"
                    isChecked: false
                    buttonText: "Turn Sensor"
                    Layout.alignment: Qt.AlignCenter
                }
                IconButtonColor{
                    icon.source: "/images/Config/ConSt_TurnSensorPressure.png"
                    isChecked: false
                    objectName: "btnPressureTurnSensor"
                    buttonText: "Pressure Turn Sensor"
                    Layout.alignment: Qt.AlignCenter
                }
                IconButtonColor{
                    icon.source: "/images/Config/ConSt_TurnSensorCurrent.png"
                    objectName: "btnCurrentTurnSensor"
                    isChecked: false
                    buttonText: "Current Turn Sensor"
                    Layout.alignment: Qt.AlignCenter
                }
            }
            Item{
                id: configWindow
                visible: configBtn.checked
                anchors.top: settingsBtns.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 5
                GridLayout{
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.right: columnOfDropDown.left
                    rows: 4
                    columns: 2
                    flow: Grid.TopToBottom
                    IconButtonColor{
                        icon.source: "/images/Config/ConST_Danfoss.png"
                        objectName: "btnDanfoss"
                        isChecked: false
                        buttonText: "Danfoss"
                        Layout.alignment: Qt.AlignCenter
                    }
                    IconButtonColor{
                        icon.source: "/images/Config/ConSt_InvertWAS.png"
                        objectName: "btnInvertWAS"
                        isChecked: false
                        Layout.alignment: Qt.AlignCenter
                        buttonText: "Invert WAS"
                    }
                    IconButtonColor{
                        icon.source: "/images/Config/ConSt_InvertDirection.png"
                        objectName: "btnInvertMotorDirection"
                        isChecked: false
                        buttonText: "Invert Motor Dir"
                        Layout.alignment: Qt.AlignCenter
                    }
                    IconButtonColor{
                        icon.source: "/images/Config/ConSt_InvertRelay.png"
                        objectName: "btnSteerInvertRelays"
                        isChecked: false
                        buttonText: "Invert Relays"
                        Layout.alignment: Qt.AlignCenter
                    }
                }
            ColumnLayout{
                id: columnOfDropDown
                anchors.top: parent.top
                anchors.margins: 10
                anchors.bottomMargin: 50
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                height: parent.height /2
                ComboBoxCustomized {
                    id: motorDriver
                    editable: true
                    model: ListModel {
                        id: modelmotorDriver
                        ListElement {text: "Cytron"}
                        ListElement {text: "IBT2"}
                    }
                        text: qsTr("Motor Driver")
                }
                ComboBoxCustomized {
                    id: a2Dconverter
                    editable: true
                    model: ListModel {
                        id: a2Dmodel
                        ListElement {text: "Single"}
                        ListElement {text: "Differential"}
                    }
                        text: qsTr("A2D Converter")
                }
                ComboBoxCustomized {
                    id: imuAxis
                    editable: true
                    model: ListModel {
                        id: imuAxismodel
                        ListElement {text: "X"}
                        ListElement {text: "Y"}
                    }
                        text: qsTr("IMU X or Y Axis")
                }

                ComboBoxCustomized {
                    id: steerEnable

                    //editable: true
                    model: ListModel {
                     //   id: steerEnablemodel
                        ListElement {text: "None"}
                        ListElement {text: "Switch"}
                        ListElement {text: "Button"}
                    }
                    text: qsTr("Steer Enable")
            Text{
                anchors.top: steerEnable.bottom
                anchors.left: steerEnable.left
                text: qsTr("Button- Push On, Push Off
Switch- Pushed On, Release Off")
                font.pixelSize: 10
            }
                    }
                }

            }
                Item{
                    id: settingsWindow
                    visible: settingsBtn.checked
                    anchors.centerIn: parent
                    SpinBoxOneDecimal{
                        text: "Panic Stop"
                        implicitWidth: 100
                        implicitHeight: 65
                        from: 0
                        value: 0
                        to: 10
                        editable: true
                    }
                }
            }
    }
}
