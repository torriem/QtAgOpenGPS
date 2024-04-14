import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ".."
import "../components"
Item{
    id: configWindow
    GridLayout{
        anchors.fill: parent
        rows: 4
        columns: 2
        flow: Grid.TopToBottom
        IconButtonColor{
            icon.source: "/images/Config/ConST_Danfoss.png"
            isChecked: ((settings.setArdSteer_setting1 & 1) != 0)
            buttonText: "Danfoss"
            Layout.alignment: Qt.AlignCenter
        }
        IconButtonColor{
            icon.source: "/images/Config/ConSt_InvertWAS.png"
            isChecked: ((settings.setArdSteer_setting0 & 1) != 0)
            Layout.alignment: Qt.AlignCenter
            buttonText: "Invert WAS"
        }
        IconButtonColor{
            icon.source: "/images/Config/ConSt_InvertDirection.png"
            isChecked: ((settings.setArdSteer_setting0 & 4) != 0)
            buttonText: "Invert Motor Dir"
            Layout.alignment: Qt.AlignCenter
        }
        IconButtonColor{
            icon.source: "/images/Config/ConSt_InvertRelay.png"
            isChecked: ((settings.setArdSteer_setting0 & 2) != 0)
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
            Component.onCompleted: currentIndex = ((settings.setArdSteer_setting0 & 16) == 0) ? 1 : 0
            model: ListModel {
                id: modelmotorDriver
                ListElement {text: "Cytron"}
                ListElement {text: "IBT2"}
            }
            text: ("Motor Driver")
        }
        ComboBoxCustomized {
            id: a2Dconverter
            editable: true
            Component.onCompleted: currentIndex = ((settings.setArdSteer_setting0 & 8) == 0) ? 1 : 0
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
            Component.onCompleted: currentIndex = ((settings.setArdSteer_setting1 & 8) == 0) ? 0 : 1
            model: ListModel {
                id: imuAxismodel
                ListElement {text: "X"}
                ListElement {text: "Y"}
            }
            text: qsTr("IMU X or Y Axis")
        }

        ComboBoxCustomized {
            id: steerEnable
            Component.onCompleted: if((settings.setArdSteer_setting0 & 32) == 32)
                                       currentIndex = 1
                                   else if((settings.setArdSteer_setting0 & 64) == 64)
                                       currentIndex = 2
                                   else
                                       currentIndex = 0
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
