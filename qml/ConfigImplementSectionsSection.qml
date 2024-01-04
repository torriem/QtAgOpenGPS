import QtQuick 2.0
import QtQuick.Controls 2.5

Item{
    id: configImplementSectionsSection
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
}
