import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent
        color: "ghostwhite"
        Rectangle{
            anchors.top: parent.top
            anchors.topMargin: 200
            anchors.right: parent.left
            width: children.width
            id: subMenuModules
            visible: true
            color: "white"
            height: children.height
            Column{
                visible: true
                width: childrenRect.width
                anchors.right: parent.right
                anchors.top: parent.top
                IconButton{
                    id: configModulesSettings
                    icon.source: "qrc:/images/Config/ConS_ModulesMachine.png"
                    /* function toggle_(){
                    if(configImplement.visible == true){
                        configImplement.visible = false
                    }else{
                        configImplement.visible = true
                    }
                }

                onClicked: toggle_configImplement()*/
                }
                IconButton{
                    id: configModulesPins
                    icon.source: "qrc:/images/Config/ConS_ImplementSection.png"
                    function toggle_modulespinswin(){
                        if(configModulesPinsWin.visible == true){
                            configModulesPinsWin.visible = false
                        }else{
                            configModulesPinsWin.visible = true
                        }
                    }

                    onClicked: toggle_modulespinswin()
                }
            }
        }
        Text {
            id: titleText
            text: qsTr("Machine Module")
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top:parent.top
            anchors.margins: 10
        }
        Rectangle{
            id: hydConfig
            anchors.left: parent.left
            color: "ghostwhite"
            border.color: "black"
            anchors.top: titleText.bottom
            anchors.topMargin: 20
            anchors.bottom: invertRel.top
            anchors.leftMargin: 10
            width: parent.width * .66
            Text {
                id: hydText
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Hydraulic Lift Config")
            }
            GridLayout{
                anchors.fill: parent
                flow: Grid.LeftToRight
                anchors.leftMargin: 10
                rows: 2
                columns: 3
                IconButtonColor{
                    objectName: "btnHydLiftEnable"
                    height: 130
                    width: 170
                    icon: "qrc:/images/SwitchOn.png"
                }
                SpinBox{
                    id: raiseTime
                    from:1
                    value: 2
                    to: 255
                    editable: true
                    Text{
                        anchors.bottom: parent.top
                        anchors.right: parent.right
                        text: qsTr("Raise Time(secs)")
                    }
                }
                Image{
                    source: "qrc:/images/Config/ConMa_LiftRaiseTime"
                    width: 200
                    height: 200
                }
                SpinBox{
                    id: liftLookAhead
                    from:1
                    value: 2
                    to: 20
                    editable: true
                    Text{
                        anchors.bottom: parent.top
                        anchors.right: parent.right
                        text: qsTr("Hydraulic Lift Look Ahead
(secs)")
                        width: parent.width
                    }
                }
                SpinBox{
                    id: lowerTime
                    from:1
                    value: 5
                    to: 255
                    editable: true
                    Text{
                        anchors.bottom: parent.top
                        anchors.right: parent.right
                        text: qsTr("Lower Time(secs)")
                    }
                }
                Image{
                    source: "qrc:/images/Config/ConMa_LiftLowerTime"
                    width: 200
                    height: 200
                }
            }
        }
        IconButtonColor{
            id: invertRel
            anchors.horizontalCenter: hydConfig.horizontalCenter
            anchors.bottom: parent.bottom
            buttonText: "Invert Relays"
            anchors.margins: 10
            icon: "qrc:/images/Config/ConSt_InvertRelay.png"
        }
        Rectangle{
            anchors.bottom: parent.bottom
            anchors.left: hydConfig.right
            anchors.right: parent.right
            anchors.top: titleText.bottom
            anchors.margins: 5
            Column{
                anchors.top: parent.top
                height: children.height
                spacing: 40
                anchors.horizontalCenter: parent.horizontalCenter
                SpinBox{
                    id: user1
                    from: 0
                    value: 1
                    to: 255
                    editable: true
                    Text{
                        anchors.bottom: parent.top
                        anchors.right: parent.right
                        text: qsTr("User 1")
                    }
                }
                SpinBox{
                    id: user2
                    from: 0
                    value: 2
                    to: 255
                    editable: true
                    Text{
                        anchors.bottom: parent.top
                        anchors.right: parent.right
                        text: qsTr("User 2")
                    }
                }
                SpinBox{
                    id: user3
                    from: 0
                    value: 3
                    to: 255
                    editable: true
                    Text{
                        anchors.bottom: parent.top
                        anchors.right: parent.right
                        text: qsTr("User 3")
                    }
                }
                SpinBox{
                    id: user4
                    from: 0
                    value: 4
                    to: 255
                    editable: true
                    Text{
                        anchors.bottom: parent.top
                        anchors.right: parent.right
                        text: qsTr("User 4")
                    }
                }
            }
        IconButtonTransparent{
            id: modulesSave
            objectName: "btnModulesSave"
            anchors.right: parent.right
            anchors.margins: 20
            anchors.bottom: parent.bottom
            icon.source: "qrc:/images/ToolAcceptChange.png"
            Text{
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.left
                anchors.rightMargin: 5
                text: "Send + Save"
            }
        }
        }
    Rectangle{
        id: configModulesPinsWin
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
            icon.source: "qrc:/images/back-button.png"
        }
        IconButtonTransparent{
            anchors.bottom: parent.bottom
            anchors.left: back.right
            anchors.margins: 20
            icon.source: "qrc:/images/UpArrow64.png"
        }
        IconButtonTransparent{
            objectName: "btnPinsSave"
            anchors.right: mandatory.left
            anchors.margins: 20
            anchors.bottom: parent.bottom
            icon.source: "qrc:/images/ToolAcceptChange.png"
            Text{
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.left
                anchors.rightMargin: 5
                text: "Send + Save"
            }
        }

        Image{
            id: mandatory
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 20
            visible: false
            source: "qrc:/images/Config/ConSt_Mandatory.png"
            width: back.width
            height: back.height
        }
    }
}

    }
