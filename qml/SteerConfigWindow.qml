import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Item {
    id: steerConfigWindow
        Rectangle{
            id: steerConfigFirst
            color: "ghostwhite"
            border.color: "black"
            border.width: 8
            visible: true
            anchors.fill: parent
            Rectangle{
                id:topLine
                color:"gray"
                visible: true
                width: parent.width-16
                height: 40
                anchors.top: parent.top
                anchors.topMargin: 8
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
                    onClicked: steerConfigWindow.visible = false

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
            TabBar{
                id: tabButtons
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
                spacing: 4
                anchors.bottom: angleInfo.top
                anchors.bottomMargin: 5
                width: 400
                visible: true
                TabButton{

                    Image {
                        anchors.fill: parent
                        source: "/images/Steer/ST_SteerTab.png"
                    }
                }
                TabButton{
                    Image{
                        anchors.fill: parent
                        source: "/images/Steer/ST_GainTab.png"
                    }
                }
                TabButton{
                    Image{
                        anchors.fill: parent

                        source: "/images/Steer/ST_StanleyTab.png"
                    }
                }
                TabButton{
                    Image{
                        anchors.fill: parent
                        source: "/images/Steer/Sf_PPTab.png"
                    }
                }
            }

            StackLayout{
                width: tabButtons.width
                anchors.bottom: tabButtons.bottom
                anchors.left: tabButtons.left
                height: parent.height - 180
                currentIndex: tabButtons.currentIndex


                Rectangle{
                    id: steerTab
                    visible: true
                    color: "white"
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
                            from: -30
                            to: 30
                            value: 5
                            centerTopText: "WAS Zero"
                        }
                        SliderCustomized {
                            id: cpDegSlider
                            objectName: "cpDegSlider"
                            width: 200
                            centerTopText: "Counts per Degree"
                            from: -30
                            to: 30
                            value: 5
                            leftText: value
                        }
                        SliderCustomized {
                            id: ackermannSlider
                            objectName: "ackermannSlider"
                            width: 200
                            centerTopText: "AckerMann"
                            from: -30
                            to: 30
                            leftText: value
                            value: 5
                        }
                        SliderCustomized {
                            id: maxSteerSlider
                            objectName: "maxSteerSlider"
                            width: 200
                            leftText: value
                            centerTopText:"Max Steer Angle"
                            from: -30
                            to: 30
                            value: 5
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
                Rectangle{
                    id: gainTab
                    height: parent.height
                    visible: false
                    color: "white"
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
                            from: -30
                            to: 30
                            value: 5
                            centerTopText: "Proportional Gain"
                        }
                        SliderCustomized {
                            id: maxLimitSlider
                            objectName: "maxLimitSlider"
                            centerTopText: "Maximum Limit"
                            leftText: value
                            width: 200
                            from: -30
                            to: 30
                            value: 5
                        }
                        SliderCustomized {
                            id: min2moveSlider
                            objectName: "min2moveSlider"
                            width: 200
                            from: -30
                            to: 30
                            value: 5
                            leftText: value
                            centerTopText: "Minimum to Move"
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
                Rectangle{
                    id: stanleyTab
                    height: parent.height * .66
                    visible: false
                    color: "white"
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
                            from: -30
                            to: 30
                            value: 5
                            leftText: value
                            centerTopText: "Agressiveness"
                        }
                        SliderCustomized {
                            id: overShootReductionSlider
                            objectName: "overShootReductionSlider"
                            width: 200
                            from: -30
                            to: 30
                            value: 5
                            leftText: value
                            centerTopText: "OverShoot Reduction"
                        }
                        SliderCustomized {
                            id: integralStanleySlider
                            objectName: "integralStanleySlider"
                            width: 200
                            from: -30
                            to: 30
                            value: 5
                            leftText: value
                            centerTopText: "Integral"
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
                Rectangle{
                    id: ppTab
                    visible: false
                    color: "white"
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
                            from: -30
                            to: 30
                            value: 5
                            leftText: value
                            centerTopText: "Acquire Look Ahead"
                        }
                        SliderCustomized {
                            id: holdLookAheadSlider
                            objectName: "holdLookAheadSlider"
                            width: 200
                            from: -30
                            to: 30
                            value: 5
                            leftText: value
                            centerTopText: "Hold Look Ahead"
                        }
                        SliderCustomized {
                            id: lookAheadSpeedGainSlider
                            objectName: "lookAheadSpeedGainSlider"
                            width: 200
                            from: -30
                            to: 30
                            value: 5
                            leftText: value
                            centerTopText: "Look Ahead Speed Gain"
                        }
                        SliderCustomized {
                            id: ppIntegralSlider
                            objectName: "ppIntegralSlider"
                            width: 200
                            from: -30
                            to: 30
                            value: 5
                            leftText: value
                            centerTopText: "Integral"
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
            }
            Rectangle{
                id: angleInfo
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 8
                anchors.left: tabButtons.left
                width: tabButtons.width
                height: 30
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        function showHide(){
                            if(pwmWindow.visible == false){
                                steerConfigWindow.height = 768
                                steerConfigWindow.width=1024
                                pwmWindow.visible = true
                                steerConfig.visible = true
                            }else{
                                steerConfigWindow.height = 600
                                steerConfigWindow.width=450
                                pwmWindow.visible = false
                                steerConfig.visible = false
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
                Grid{
                    id: toggleSteerButtons
                    anchors.top: parent.top
                    anchors.left: parent.left
                    width: childrenRect.width
                    height: childrenRect.height
                    anchors.margins: 5
                    rows: 4
                    columns: 2
                    spacing: 20
                    flow: Grid.TopToBottom
                    IconButtonColor{
                        icon: "/images/Config/ConST_Danfoss.png"
                        objectName: "btnDanfoss"
                        isChecked: false
                        buttonText: "Danfoss"
                    }
                    IconButtonColor{
                        icon: "/images/Config/ConSt_TurnSensor.png"
                        objectName: "btnTurnSensor"
                        isChecked: false
                        buttonText: "Turn Sensor"
                    }
                    IconButtonColor{
                        icon: "/images/Config/ConSt_TurnSensorPressure.png"
                        isChecked: false
                        objectName: "btnPressureTurnSensor"
                        buttonText: "Pressure Turn Sensor"
                    }
                    IconButtonColor{
                        icon: "/images/Config/ConSt_TurnSensorCurrent.png"
                        objectName: "btnCurrentTurnSensor"
                        isChecked: false
                        buttonText: "Current Turn Sensor"
                    }
                    IconButtonColor{
                        icon: "/images/Config/ConSt_InvertWAS.png"
                        objectName: "btnInvertWAS"
                        isChecked: false
                        buttonText: "Invert WAS"
                    }
                    IconButtonColor{
                        icon: "/images/Config/ConSt_InvertDirection.png"
                        objectName: "btnInvertMotorDirection"
                        isChecked: false
                        buttonText: "Invert Motor Dir"
                    }
                    IconButtonColor{
                        icon: "/images/Config/ConSt_InvertRelay.png"
                        objectName: "btnSteerInvertRelays"
                        isChecked: false
                        buttonText: "Invert Relays"
                    }
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
                ColumnLayout{
                    id: columnOfDropDown
                    anchors.top: parent.top
                    anchors.left: toggleSteerButtons. right
                    anchors.leftMargin: 30
                    anchors.topMargin: 30
                    anchors.right: parent.right
                    height: parent.height /2
                    ComboBox {
                        id: motorDriver
                        objectName: "listMotorDriver"
                        editable: true
                        model: ListModel {
                            id: modelmotorDriver
                            ListElement {text: "Cytron"}
                            ListElement {text: "IBT2"}
                        }
                        onAccepted: {
                            if (motorDriver.find(currentText) === -1){
                                model.append({text: editText})
                                curentIndex = motorDriver.find(editText)
                            }
                        }
                        implicitHeight:50
                        implicitWidth: 200
                        Text{
                            anchors.bottom: parent.top
                            anchors.left: parent.left
                            text: qsTr("Motor Driver")
                        }
                    }
                    ComboBox {
                        id: a2Dconverter
                        objectName: "listA2DConverter"
                        editable: true
                        model: ListModel {
                            id: a2Dmodel
                            ListElement {text: "Single"}
                            ListElement {text: "Differential"}
                        }
                        onAccepted: {
                            if (skips.find(currentText) === -1){
                                a2Dmodel.append({text: editText})
                                curentIndex = a2Dconverter.find(editText)
                            }
                        }
                        implicitHeight:50
                        implicitWidth: 200
                        Text{
                            anchors.bottom: parent.top
                            anchors.left: parent.left
                            text: qsTr("A2D Converter")
                        }
                    }
                    ComboBox {
                        id: imuAxis
                        objectName: "listIMUAxis"
                        editable: true
                        model: ListModel {
                            id: imuAxismodel
                            ListElement {text: "X"}
                            ListElement {text: "Y"}
                        }
                        onAccepted: {
                            if (imuAxis.find(currentText) === -1){
                                imuAxismodel.append({text: editText})
                                curentIndex = imuAxis.find(editText)
                            }
                        }
                        implicitHeight:50
                        implicitWidth: 200
                        Text{
                            anchors.bottom: parent.top
                            anchors.left: parent.left
                            text: qsTr("IMU X or Y Axis")
                        }
                    }
                    ComboBox {
                        id: steerEnable
                        objectName: "listSteerEnable"
                        editable: true
                        model: ListModel {
                            id: steerEnablemodel
                            ListElement {text: "None"}
                            ListElement {text: "Switch"}
                            ListElement {text: "Button"}
                        }
                        onAccepted: {
                            if (steerEnable.find(currentText) === -1){
                                steerEnablemodel.append({text: editText})
                                curentIndex = steerEnable.find(editText)
                            }
                        }
                        implicitHeight:50
                        implicitWidth: 200
                        Text{
                            anchors.bottom: parent.top
                            anchors.left: parent.left
                            text: qsTr("Steer Enable")
                        }
                    }

                }
                Text{
                    anchors.top: columnOfDropDown.bottom
                    anchors.left: columnOfDropDown.left
                    text: qsTr("Button- Push On, Push Off
Switch- Pushed On, Release Off")
                    font.pixelSize: 10
                }
            }
        }
    }
