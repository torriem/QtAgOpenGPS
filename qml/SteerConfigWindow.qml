import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Item {
    Rectangle{
        id: steerConfigFirst
        color: "ghostwhite"
        border.color: "black"
        border.width: 8
        visible: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        height: parent.height
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
        TabBar{
            id: tabButtons
            anchors.top: topLine.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            spacing: 4
            height: parent.height * .65
            width: 400
            visible: true
            TabButton{

                Image {
                    anchors.fill: parent
                    source: "qrc:/images/Steer/ST_SteerTab.png"
                }
            }
            TabButton{
                Image{
                    anchors.fill: parent
                    source: "qrc:/images/Steer/ST_GainTab.png"
                }
            }
            TabButton{
                Image{
                    anchors.fill: parent

                    source: "qrc:/images/Steer/ST_StanleyTab.png"
                }
            }
            TabButton{
                Image{
                    anchors.fill: parent
                    source: "qrc:/images/Steer/Sf_PPTab.png"
                }
            }
        }

        StackLayout{
            width: tabButtons.width
            anchors.bottom: tabButtons.bottom
            anchors.left: tabButtons.left
            height: parent.height * .65 - 50
            currentIndex: tabButtons.currentIndex


            Rectangle{
                id: steerTab
                visible: true
                color: "light gray"
                Column{
                    id: steerColumn
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    height: childrenRect.height
                    width: childrenRect.width
                    anchors.margins: 5
                    spacing: 30
                    Text{
                        text: "WAS Zero"
                    }
                    Slider {
                        id: wasZeroSlider
                        objectName: "wasZeroSlider"
                        width: 200
                        from: -30
                        to: 30
                        value: 5
                    }
                    Text{
                        text: "Counts per Degree"
                    }
                    Slider {
                        id: cpDegSlider
                        objectName: "cpDegSlider"
                        width: 200
                        from: -30
                        to: 30
                        value: 5
                    }
                    Text{
                        text: "AckerMann"
                    }
                    Slider {
                        id: ackermannSlider
                        objectName: "ackermannSlider"
                        width: 200
                        from: -30
                        to: 30
                        value: 5
                    }
                    Text{
                        text: "Max Steer Angle"
                    }
                    Slider {
                        id: maxSteerSlider
                        objectName: "maxSteerSlider"
                        width: 200
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
                    source: "qrc:/images/Steer/Sf_SteerTab.png"
                }

            }
            Rectangle{
                id: gainTab
                height: parent.height * .66
                visible: false
                color: "light gray"
                Column{
                    id: gainColumn
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    height: childrenRect.height
                    width: childrenRect.width
                    anchors.margins: 5
                    spacing: 30
                    Text{
                        text: "Proportional Gain"
                    }
                    Slider {
                        id: propGainlider
                        objectName: "progGainSlider"
                        width: 200
                        from: -30
                        to: 30
                        value: 5
                    }
                    Text{
                        text: "Maximum Limit"
                    }
                    Slider {
                        id: maxLimitSlider
                        objectName: "maxLimitSlider"
                        width: 200
                        from: -30
                        to: 30
                        value: 5
                    }
                    Text{
                        text: "Minimum to Move"
                    }
                    Slider {
                        id: min2moveSlider
                        objectName: "min2moveSlider"
                        width: 200
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
                    source: "qrc:/images/Steer/Sf_GainTab.png"
                }

            }
            Rectangle{
                id: stanleyTab
                height: parent.height * .66
                visible: false
                color: "light gray"
                Column{
                    id: stanleyColumn
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    height: childrenRect.height
                    width: childrenRect.width
                    anchors.margins: 5
                    spacing: 30
                    Text{
                        text: "Agressiveness"
                    }
                    Slider {
                        id: stanleyAggressivenessSlider
                        objectName: "stanleyAgressivenessSlider"
                        width: 200
                        from: -30
                        to: 30
                        value: 5
                    }
                    Text{
                        text: "Overshoot Reduction"
                    }
                    Slider {
                        id: overShootReductionSlider
                        objectName: "overShootReductionSlider"
                        width: 200
                        from: -30
                        to: 30
                        value: 5
                    }
                    Text{
                        text: "Integral"
                    }
                    Slider {
                        id: integralStanleySlider
                        objectName: "integralStanleySlider"
                        width: 200
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
                    source: "qrc:/images/Steer/Sf_Stanley.png"
                }

            }
            Rectangle{
                id: ppTab
                visible: false
                color: "light gray"
                Column{
                    id: ppColumn
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    height: childrenRect.height
                    width: childrenRect.width
                    anchors.margins: 5
                    spacing: 30
                    Text{
                        text: "Acquire Look Ahead"
                    }
                    Slider {
                        id: acqLookAheadSlider
                        objectName: "acqLookAheadSlider"
                        width: 200
                        from: -30
                        to: 30
                        value: 5
                    }
                    Text{
                        text: "Hold Look Ahead"
                    }
                    Slider {
                        id: holdLookAheadSlider
                        objectName: "holdLookAheadSlider"
                        width: 200
                        from: -30
                        to: 30
                        value: 5
                    }
                    Text{
                        text: "Look Ahead Speed Gain"
                    }
                    Slider {
                        id: lookAheadSpeedGainSlider
                        objectName: "lookAheadSpeedGainSlider"
                        width: 200
                        from: -30
                        to: 30
                        value: 5
                    }
                    Text{
                        text: "Integral"
                    }
                    Slider {
                        id: ppIntegralSlider
                        objectName: "ppIntegralSlider"
                        width: 200
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
                    source: "qrc:/images/Steer/Sf_PP.png"
                }
            }
        }
        Rectangle{
            id: angleInfo
            anchors.top: tabButtons.bottom
            anchors.topMargin: 5
            anchors.left: tabButtons.left
            width: tabButtons.width
            height: 30
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
        Rectangle{
            id: pwmWindow
            anchors.top: angleInfo.bottom
            anchors.left: angleInfo.left
            width: angleInfo.width
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8
            anchors.topMargin: 5
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
                    icon: "qrc:/images/SteerDriveOff.png"
                    iconChecked: "qrc:/images/SteerDriveOn.png"
                    width: parent.width/4.5
                    height: parent.height
                    border.color: "black"
                    bordersize: 3
                }
                IconButton{
                    objectName: "pwmLeft"
                    icon: "qrc:/images/SnapLeft.png"
                    width: parent.width/4.5
                    height: parent.height
                    border.color: "black"
                    bordersize: 3
                }
                IconButton{
                    objectName: "pwmRight"
                    icon: "qrc:/images/SnapRight.png"
                    width: parent.width/4.5
                    height: parent.height
                    border.color: "black"
                    bordersize: 3
                }
                IconButton{
                    id: pwmZero
                    objectName: "pwmZero"
                    icon: "qrc:/images/SteerZeroSmall.png"
                    width: parent.width/4.5
                    height: parent.height
                    border.color: "black"
                    bordersize: 3
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
                border.color: "black"
                bordersize: 3
                icon: "qrc:/images/BoundaryRecord.png"
                iconChecked: "qrc:/images/Stop"
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
            anchors.left: tabButtons.right
            anchors.top: topLine.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 10
            Grid{
                id: toggleSteerButtons
                anchors.top: parent.top
                anchors.left: parent.left
                width: childrenRect.width
                height: childrenRect.height
                anchors.margins: 10
                rows: 4
                columns: 2
                spacing: 20
                flow: Grid.TopToBottom
                IconButtonColor{
                    icon: "qrc:/images/Config/ConST_Danfoss.png"
                    objectName: "btnDanfoss"
                    isChecked: false
                    buttonText: "Danfoss"
                }
                IconButtonColor{
                    icon: "qrc:/images/Config/ConSt_TurnSensor.png"
                    objectName: "btnTurnSensor"
                    isChecked: false
                    buttonText: "Turn Sensor"
                }
                IconButtonColor{
                    icon: "qrc:/images/Config/ConSt_TurnSensorPressure.png"
                    isChecked: false
                    objectName: "btnPressureTurnSensor"
                    buttonText: "Pressure Turn Sensor"
                }
                IconButtonColor{
                    icon: "qrc:/images/Config/ConSt_TurnSensorCurrent.png"
                    objectName: "btnCurrentTurnSensor"
                    isChecked: false
                    buttonText: "Current Turn Sensor"
                }
                IconButtonColor{
                    icon: "qrc:/images/Config/ConSt_InvertWAS.png"
                    objectName: "btnInvertWAS"
                    isChecked: false
                    buttonText: "Invert WAS"
                }
                IconButtonColor{
                    icon: "qrc:/images/Config/ConSt_InvertDirection.png"
                    objectName: "btnInvertMotorDirection"
                    isChecked: false
                    buttonText: "Invert Motor Dir"
                }
                IconButtonColor{
                    icon: "qrc:/images/Config/ConSt_InvertRelay.png"
                    objectName: "btnSteerInvertRelays"
                    isChecked: false
                    buttonText: "Invert Relays"
                }
                IconButtonColor{
                    icon: "qrc:/images/Config/ConSt_InvertRelay.png"
                    isChecked: false
                    buttonText: "Panic Stop(love the input box)"
                }
            }
            ColumnLayout{
                id: columnOfDropDown
                anchors.top: parent.top
                anchors.left: toggleSteerButtons. right
                anchors.leftMargin: 100
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
