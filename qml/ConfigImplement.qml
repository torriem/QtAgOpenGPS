import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4 as OldCtrl

Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent
        visible: true
        color: "ghostwhite"
        Rectangle{
            anchors.top: parent.top
            anchors.topMargin: 200
            anchors.right: parent.left
            width: children.width
            id: subMenuImplement
            visible: true
            color: "white"
            height: children.height
            Column{
                visible: true
                width: childrenRect.width
                anchors.right: parent.right
                anchors.top: parent.top
                IconButton{
                    id: configImpSettings
                    icon.source: "/images/Config/ConS_VehicleConfig.png"
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
                    id: configImpDim
                    icon.source: "/images/Config/ConS_ImplementHitch.png"
                    function toggle_impdimwin(){
                        if(configImpDimWin.visible == true){
                            configImpDimWin.visible = false
                        }else{
                            configImpDimWin.visible = true
                        }
                    }

                    onClicked: toggle_impdimwin()
                }
                IconButton{
                    id: configImpSection
                    icon.source: "/images/Config/ConS_ImplementSection.png"
                    function toggle_impsecwin(){
                        if(configImpSecWin.visible == true){
                            configImpSecWin.visible = false
                        }else{
                            configImpSecWin.visible = true
                        }
                    }

                    onClicked: toggle_impsecwin()
                }
                IconButton{
                    id: configImpTiming
                    icon.source: "/images/Config/ConS_ImplementSettings.png"
                    function toggle_imptimwin(){
                        if(configImpTimWin.visible == true){
                            configImpTimWin.visible = false
                        }else{
                            configImpTimWin.visible = true
                        }
                    }

                    onClicked: toggle_imptimwin()

                }
                IconButton{
                    id: configImpSwitches
                    icon.source: "/images/Config/ConS_ImplementSwitch.png"
                    function toggle_impswitchwin(){
                        if(configImpSwitchWin.visible == true){
                            configImpSwitchWin.visible = false
                        }else{
                            configImpSwitchWin.visible = true
                        }
                    }

                    onClicked: toggle_impswitchwin()

                }
            }
        }

        Text{
            id: text
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Attachment Style")
        }
        GridLayout{
            anchors.top: text.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: 500
            height: 400
            rows:2
            columns: 2
            flow:Grid.TopToBottom
            IconButtonColor{
                width:200
                height:150
                id: i3pt
                icon: "/images/ToolChkRear.png"
                isChecked: false
            }

            IconButtonColor{
                width:200
                height:150
                id: i3ptfront
                icon: "/images/ToolChkFront.png"
                isChecked: false
            }
            IconButtonColor{
                width:200
                height:150
                id: itrailed
                icon: "/images/ToolChkTrailing.png"
                isChecked: false
            }
            IconButtonColor{
                width:200
                height:150
                id: iTBT
                icon: "/images/ToolChkTBT.png"
                isChecked: false
            }
        }

    }
    Rectangle{
        id: configImpDimWin
        anchors.fill: parent
        color: "ghostwhite"
        visible: false
        Image{
            source: "/images/ToolHitchPageTrailing.png"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 15
            height: parent.height*.75
            SpinBox{
                anchors.bottom: parent.top
                anchors.right: parent.right
                anchors.rightMargin: 500
                from: 4
                value : 1
                to:1181
                editable: true
            }
            SpinBox{
                anchors.bottom: parent.top
                anchors.right: parent.right
                anchors.rightMargin: 50
                from: 4
                value : 1
                to:1181
                editable: true
            }
        }
    }
    Rectangle{
        id: configImpSecWin
        anchors.fill: parent
        color: "ghostwhite"
        visible: false
        Item{
            id: sectionWidthBoxes
            anchors.top: parent.top
            anchors.topMargin: 80
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: middleRow.top
            anchors.bottomMargin: 30
            anchors.margins: 15
            z: 8
            Item{
                id: secWidthButtons
                Button{
                    implicitHeight: 50
                    implicitWidth: 80
                    background: Rectangle{
                        border.width: 1
                        border.color: "black"
                        color: "white"
                    }
                    contentItem: Rectangle{
                        id: text11
                        color: "transparent"
                        Text{
                            id: text12
                            anchors.centerIn: parent
                            text: middleRowSpinbox.value
                        }
                    }
                    onClicked: textEntryBox.visible=true
                    Text{
                        anchors.bottom: parent.top
                        anchors.left: parent.left
                        text: "1"
                    }
                }
            }

            Rectangle{
                id: textEntryBox
                visible: false
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                width: 200
                height: 200
                border.color: "black"
                border.width: 1
                color: "lightgray"
//                Text{ //-----these are for displaying the highest and lowest numbers possible
//                    anchors.left: parent.left
//                    anchors.top: parent.top
//                    anchors.margins: 20
//                    font.pixelSize: 20
//                    text: textEntry_validator.bottom
//                }
//                Text{
//                    anchors.right: parent.right
//                    anchors.top: parent.top
//                    anchors.margins: 20
//                    font.pixelSize: 20
//                    text: textEntry_validator.top
//                }
                TextField{
                    id: textEntry
                    implicitWidth: 90
                    implicitHeight: 40
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    //text: text12.text
                    background: Rectangle{
                        id: textEntry_rect
                        border.color: "black"
                        border.width: 1
                        color: "white"
                    }
                    validator: DoubleValidator{
//                        id: textEntry_validator
//                        bottom: 3
//                        top: 350

                    }
                    onTextChanged: {//for some reason, "text" is returned as 0x[string of numbers]
                        //so I don't know if we can use textfield or not
//                        console.log(text)
//                        console.log(parseInt(text))
//                        //console.log(parseString(text))
//                            if (parseInt(text) < 3 || parseInt(text) > 350) {
//                                console.log("Invalid input");
//                                //Display whatever warning here
//                                textEntry_rect.color = "red";
//                            } else {
//                                console.log("Input good");
//                                //Hide warning here
//                                textEntry_rect.color = "white";
//                            }
                        }
                }
                IconButtonTransparent{
                    id: cancel
                    icon.source: "/images/Cancel64.png"
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.margins: 10
                    onClicked: parent.visible = false
                }
                IconButtonTransparent{
                    objectName: "btnNumberWindowSave"
                    icon.source: "/images/OK64.png"
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.margins: 10

                }
            }

           /* GridLayout{
                anchors.fill: parent
                rows: 2
                columns: 8
                flow: Grid.LeftToRight
                z:9
                SectionWidth{
                    objectName: "sectionWidth1"
                    value: middleRowSpinbox.value
                    titleText: 1
                    bottomNum: 3
                    topNum: 300
                    z: -1
                }
                SectionWidth{
                    objectName: "sectionWidth2"
                    value: middleRowSpinbox.value
                    titleText: 2
                    z: -1
                }
                SectionWidth{
                    objectName: "sectionWidth3"
                    value: middleRowSpinbox.value
                    titleText: 3
                    z: 5
                }
                SectionWidth{
                    objectName: "sectionWidth4"
                    value: middleRowSpinbox.value
                    titleText: 4
                    z: 5
                }
                SectionWidth{
                    objectName: "sectionWidth5"
                    value: middleRowSpinbox.value
                    titleText: 5
                }
                SectionWidth{
                    objectName: "sectionWidth6"
                    value: middleRowSpinbox.value
                    titleText: 6
                }
                SectionWidth{
                    objectName: "sectionWidth7"
                    value: middleRowSpinbox.value
                    titleText: 7
                }
                SectionWidth{
                    objectName: "sectionWidth8"
                    value: middleRowSpinbox.value
                    titleText: 8
                }
                SectionWidth{
                    objectName: "sectionWidth9"
                    value: middleRowSpinbox.value
                    titleText: 9
                }
                SectionWidth{
                    objectName: "sectionWidth10"
                    value: middleRowSpinbox.value
                    titleText: 10
                }
                SectionWidth{
                    objectName: "sectionWidth11"
                    value: middleRowSpinbox.value
                    titleText: 11
                }
                SectionWidth{
                    objectName: "sectionWidth12"
                    value: middleRowSpinbox.value
                    titleText: 12
                }
                SectionWidth{
                    objectName: "sectionWidth13"
                    value: middleRowSpinbox.value
                    titleText: 13
                }
                SectionWidth{
                    objectName: "sectionWidth14"
                    value: middleRowSpinbox.value
                    titleText: 14
                }
                SectionWidth{
                    objectName: "sectionWidth15"
                    value: middleRowSpinbox.value
                    titleText: 15
                }
                SectionWidth{
                    objectName: "sectionWidth16"
                    value: middleRowSpinbox.value
                    titleText: 16
                }
            }*/
        }

        Row{
            id: middleRow
            anchors.top: parent.verticalCenter
            anchors.right: parent.right
            anchors.margins: 80
            width: children.width
            height: children.height
            spacing: 120

            SpinBox{ //-------------this is the spinbox i tried to remove the + and - from.
                // I did, but now I can't edit it.
                id: middleRowSpinbox
                objectName: "sectionWidthAll"
                implicitWidth: 50
                implicitHeight: 50

                from: 3
                value: 240
                to: 39400
                editable: true


                //up.destroy: true
                up.indicator: Rectangle{
                    visible: false
                    width: 1
                    anchors.left: parent.right
                }
                down.indicator:Rectangle{
                    visible: false
                    width: 1
                    anchors.right: parent.left
                }

                Text{
                    anchors.bottom: parent.top
                    text: "Section Width"
                }
            }
            ComboBox{
                id: numOfSections
                objectName: "numOfSections"
                model: ListModel{
                        ListElement {text: "1"}
                        ListElement {text: "2"}
                        ListElement {text: "3"}
                        ListElement {text: "4"}
                        ListElement {text: "5"}
                        ListElement {text: "6"}
                        ListElement {text: "7"}
                        ListElement {text: "8"}
                        ListElement {text: "9"}
                        ListElement {text: "10"}
                    ListElement{text: "11"}
                    ListElement{text: "12"}
                    ListElement{text: "13"}
                    ListElement{text: "14"}
                    ListElement{text: "15"}
                    ListElement{text: "16"}
                }
                    implicitHeight:60
                    implicitWidth: 90
                Text{
                    anchors.bottom: parent.top
                    text: "Sections"
                }
            }
            Column{
                spacing: 8
                Text {
                    text: "000"
                }
                Text {
                    text: "Inch"
                    color: "green"
                }
            }
        }
            Row{
                id: bottomRow
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.right: bottomRightText.left
                anchors.margins: 10
                spacing: 90
                Button{
                    function toggleZones(){
                        if(image.source == "/images/Config/ConT_Asymmetric.png"){
                            image.source = "/images/Config/ConT_Symmetric.png"
                        }else{
                            image.source ="/images/Config/ConT_Asymmetric.png"
                        }
                    }
                    width: 180
                    height: 130
                    id: chooseZones
                    objectName: "zonesOrSections"
                    onClicked: {
                        toggleZones()
                    }
                    background: Rectangle{
                        color: "white"
                        border.color: "black"
                        border.width: 1
                        Image{
                            id: image

                            source:"/images/Config/ConT_Asymmetric.png"
                            anchors.fill: parent
                        }
                    }
                }
                SpinBox{
                    objectName: "percentConverage"
                    from: 0
                    to: 100
                    value: 100
                    editable: true
                    anchors.bottom: parent.bottom
                    Text{
                        anchors.bottom: parent.top
                        text: "% Coverage"
                    }
                }
               IconButton{
                   objectName: "boundaryOff"
                   icon.source: "/images/SectionOffBoundary.png"
                   iconChecked: "/images/SectionOnBounary.png"
                    anchors.bottom: parent.bottom
                    implicitWidth: 100
                    implicitHeight: 100
                    border: 1
                    color3: "white"
                    colorChecked1: "green"
                    colorChecked2: "green"
                    colorChecked3: "green"
               }
               SpinBox{
                   objectName: "speedBelowSectionOff"
                   from: 0
                   to: 30
                   value: 0
                   editable: true
                    anchors.bottom: parent.bottom
                   Image{
                       anchors.bottom: parent.top
                       width: parent.width
                       height: parent.height +30
                       source: "/images/SectionOffBelow.png"
                   }
               }
            }
            Text{
                id: bottomRightText
                anchors.right: parent.right
                text: "Km/H"
                anchors.verticalCenter: bottomRow.verticalCenter
                font.pixelSize: 25
            }
    }
    Rectangle{
        id: configImpTimWin
        anchors.fill: parent
        color: "ghostwhite"
        visible: false
        Image{
            id: configImpImage
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottomMargin: 70
            anchors.rightMargin: 30
            anchors.leftMargin: 200
            anchors.topMargin: 100
            source: "/images/ImplementSettings.png"

            SpinBox{
                anchors.right: parent.left
                anchors.top: parent.top
                from: 0
                value: 1
                to: 22
                editable: true
                Text{
                    anchors.left: parent.left
                    anchors.bottom: parent.top
                    text: qsTr("On (secs)")
                }
            }
            SpinBox{
                anchors.right: parent.left
                anchors.verticalCenter: parent.verticalCenter
                from: 0
                value:0
                to: 20
                editable: true
                Text{
                    anchors.left: parent.left
                    anchors.bottom: parent.top
                    text: qsTr("Off (secs)")
                }
            }
            SpinBox{
                anchors.right: parent.left
                anchors.bottom: parent.bottom
                from: 0
                value: 0
                to: 20
                editable: true
                Text{
                    anchors.left: parent.left
                    anchors.bottom: parent.top
                    text: qsTr("Delay (secs)")
                }
            }
            SpinBox{
                anchors.left: parent.left
                anchors.leftMargin: 500
                anchors.top: parent.bottom
                anchors.bottomMargin: -10
                from: -984
                value: 0
                to: 984
                editable: true
                Text{
                    anchors.left: parent.left
                    anchors.top: parent.bottom
                    text: qsTr("Offset")
                }
            }
            SpinBox{
                anchors.right: parent.right
                anchors.rightMargin: 120
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 100
                from: -1181
                value: 0
                to: 1181
                editable: true
                Text{
                    anchors.left: parent.left
                    anchors.top: parent.bottom
                    text: qsTr("Overlap / Gap")
                }
            }
        }
    }
    Rectangle{
        id: configImpSwitchWin
        anchors.fill: parent
        color: "ghostwhite"
        visible: false
        Rectangle{
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            width: workswitchcolumn.width + 40
            anchors.margins: 40
            border.color: "black"
            color: "ghostwhite"
            Text{
                id: workTitle
                anchors.top: parent.top
                anchors.left: parent.left
                text: qsTr("Work Switch")
            }
            //for now I'll leave this like it is... I should maybe create another object, with the icon beside the text
            ColumnLayout{
                id: workswitchcolumn
                anchors.top: workTitle.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: childrenRect.width
                anchors.margins: 15
                IconButtonTextBeside{
                    icon.source: "/images/HydraulicLiftOff.png"
                    objectName: "btnWorkSwitch"
                    isChecked: false
                    text: qsTr("Work Switch")
                }
                IconButtonTextBeside{
                    icon.source: "/images/ManualOff.png"
                    objectName: "btnWorkSwitchManual"
                    isChecked: false
                    text: qsTr("Manual Sections")
                }

                IconButtonTextBeside{
                    objectName: "btnWorkSwitchAuto"
                    icon.source: "/images/SectionMasterOff.png"
                    isChecked: false
                    text: qsTr("Auto Sections")
                }
                IconButton{
                    implicitWidth: 270
                    implicitHeight: 80
                    border: 1
                    id: switchStateButton
                    objectName: "workSwitchstatebutton"
                    isChecked: false
                    icon.source: "/images/SwitchActiveClosed.png"
                    color3: "white"
                    colorChecked1: "green"
                    colorChecked2: "green"
                    colorChecked3: "green"
                }
            }
        }
        Rectangle{
            anchors.right:  parent.right
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            width: steerswitchcolumn.width + 40
            anchors.margins: 40
            border.color: "black"
            color: "ghostwhite"
            Text{
                id: steerTitle
                anchors.top: parent.top
                anchors.left: parent.left
                text: qsTr("Steer Switch")
            }
            ColumnLayout{
                id: steerswitchcolumn
                anchors.top: steerTitle.bottom
                anchors.bottom: parent.bottom
                width: childrenRect.width
                anchors.right: parent.right
                anchors.margins: 15
                IconButtonTextBeside{
                    objectName: "btnSteerSwitch"
                    isChecked: false
                    text: qsTr("Steer Switch")
                    icon.source: "/images/AutoSteerOff.png"
                }
                IconButtonTextBeside{
                    objectName: "btnSteerSwitchManual"
                    isChecked: false
                    text: qsTr("Manual Sections")
                    icon.source: "/images/ManualOff.png"
                }
                IconButtonTextBeside{
                    objectName: "btnSteerSwitchAuto"
                    isChecked: false
                    text: qsTr("Auto Sections")
                    icon.source: "/images/SectionMasterOff.png"
                }
            }
        }
    }
}
