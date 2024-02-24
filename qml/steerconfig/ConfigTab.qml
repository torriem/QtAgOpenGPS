import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import ".."
Item{
    id: configWindow
    GridLayout{
        anchors.fill: parent
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
