import QtQuick 2.0
import QtQuick.Controls 2.5

//todo: sections not populated from .config yet
Item{
    id: configImplementSectionsSection
    z: 8
    ListModel {
        id: section_model

    }

    GridView {
        id: section_width_grid
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: middleRow.top
        model: section_model
        cellWidth: 200

        delegate: Rectangle {
            SpinBox {
                id: spinner
                from: 3
                to: 300
                editable: true
                value: model.width
                onValueChanged: {
                    if (value == 3) {
                        spin_message.visible = true
                        spin_message.text = "Must be 3 or greater"
                    } else if(value == 300){
                        spin_message.visible = true
                        spin_message.text = "Can't be larger than 300"

                    }else {
                        spin_message.visible = false
                    }

                    //some validation here
                    //emit signal.  We know our section number because it's in the model
                }
            }

            Text {
                text: qsTr("Section " + Number(model.secNum+1).toLocaleString(Qt.locale(),"f",0))
                anchors.bottom: spinner.top
                anchors.left: spinner.left
            }
            Text {
                id: spin_message
                visible: false
                text: qsTr("message")
                color: "red"
                anchors.top: spinner.bottom
                anchors.left: spinner.left
            }
        }
    }
    Row{
        id: middleRow
        anchors.top: parent.verticalCenter
        anchors.right: parent.right
        anchors.margins: 80
        width: children.width
        height: children.height
        spacing: 120

        SpinBoxCustomized{
            id: middleRowSpinbox
            objectName: "sectionWidthAll"
            implicitWidth: 150
            implicitHeight: 50
            from: 10
            value: settings.setTool_defaultSectionWidth
            to: 1000
            editable: true
            text: qsTr("Section Width")
            //onValueChanged: settings.setTool_defaultSectionWidth = value
        }
        ComboBox{
            id: numOfSections
            objectName: "numOfSections"
            valueRole: settings.setVehicle_numSections
            onActivated: function (which) {
                settings.setVehicle_numSections = which
                console.warn("We got this one: " + which);
                section_model.clear();
                for (var i=0; i < which+1; i++) {
                    console.warn("Writing new section " + i)
                    section_model.append( { secNum: i, width: 5 } )
                }
            }

            Component.onCompleted: {
                if (section_model.count < (currentIndex + 1)) {
                    for (var i=section_model.count; i < currentIndex+1 ; i ++ ) {
                         section_model.append( { secNum: i, width: 5 } )
                    }
                }
            }
            model: ListModel{
                ListElement {text: qsTr("1")}
                ListElement {text: qsTr("2")}
                ListElement {text: qsTr("3")}
                ListElement {text: qsTr("4")}
                ListElement {text: qsTr("5")}
                ListElement {text: qsTr("6")}
                ListElement {text: qsTr("7")}
                ListElement {text: qsTr("8")}
                ListElement {text: qsTr("9")}
                ListElement {text: qsTr("10")}
                ListElement{text: qsTr("11")}
                ListElement{text: qsTr("12")}
                ListElement{text: qsTr("13")}
                ListElement{text: qsTr("14")}
                ListElement{text: qsTr("15")}
                ListElement{text: qsTr("16")}
            }
            implicitHeight:60
            implicitWidth: 90
            Text{
                anchors.bottom: parent.top
                text: qsTr("Sections")
            }
        }
        Column{
            spacing: 8
            Text {
                text: qsTr("000")
            }
            Text {
                text: qsTr("Inch")
                color: "green"
            }
        }
    }
}
