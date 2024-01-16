import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Rectangle{
    id: configModulesPins
    anchors.fill: parent
    color: "ghostwhite"
    visible: false
    GridLayout{
        flow: Grid.LeftToRight
        columns: 5
        rows: 5
        anchors.bottom: back.top
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        PinButtons{
            objectName: "pin1"
            pinNumber: "Pin 1"
        }
        PinButtons{
            objectName: "pin2"
            pinNumber: "Pin 2"
        }
        PinButtons{
            objectName: "pin3"
            pinNumber: "Pin 3"
        }
        PinButtons{
            objectName: "pin4"
            pinNumber: "Pin 4"
        }
        PinButtons{
            objectName: "pin5"
            pinNumber: "Pin 5"
        }
        PinButtons{
            objectName: "pin6"
            pinNumber: "Pin 6"
        }
        PinButtons{
            objectName: "pin7"
            pinNumber: "Pin 7"
        }
        PinButtons{
            objectName: "pin8"
            pinNumber: "Pin 8"
        }
        PinButtons{
            objectName: "pin9"
            pinNumber: "Pin 9"
        }
        PinButtons{
            objectName: "pin10"
            pinNumber: "Pin 10"
        }
        PinButtons{
            objectName: "pin11"
            pinNumber: "Pin 11"
        }
        PinButtons{
            objectName: "pin12"
            pinNumber: "Pin 12"
        }
        PinButtons{
            objectName: "pin13"
            pinNumber: "Pin 13"
        }
        PinButtons{
            objectName: "pin14"
            pinNumber: "Pin 14"
        }
        PinButtons{
            objectName: "pin15"
            pinNumber: "Pin 15"
        }
        PinButtons{
            objectName: "pin16"
            pinNumber: "Pin 16"
        }
        PinButtons{
            objectName: "pin16"
            pinNumber: "Pin 17"
        }
        PinButtons{
            objectName: "pin18"
            pinNumber: "Pin 18"
        }
        PinButtons{
            objectName: "pin19"
            pinNumber: "Pin 19"
        }
        PinButtons{
            objectName: "pin20"
            pinNumber: "Pin 20"
        }
        PinButtons{
            objectName: "pin21"
            pinNumber: "Pin 21"
        }
        PinButtons{
            objectName: "pin22"
            pinNumber: "Pin 22"
        }
        PinButtons{
            objectName: "pin23"
            pinNumber: "Pin 23"
        }
        PinButtons{
            objectName: "pin24"
            pinNumber: "Pin 24"
        }
    }
    IconButtonTransparent{
        id: back
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 20
        icon.source: "/images/back-button.png"
    }
    IconButtonTransparent{
        anchors.bottom: parent.bottom
        anchors.left: back.right
        anchors.margins: 20
        icon.source: "/images/UpArrow64.png"
    }
    IconButtonTransparent{
        objectName: "btnPinsSave"
        anchors.right: mandatory.left
        anchors.margins: 20
        anchors.bottom: parent.bottom
        icon.source: "/images/ToolAcceptChange.png"
        Text{
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.left
            anchors.rightMargin: 5
            text: qsTr("Send + Save")
        }
    }

    Image{
        id: mandatory
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 20
        visible: false
        source: "/images/Config/ConSt_Mandatory.png"
        width: back.width
        height: back.height
    }
}
