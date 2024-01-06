import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    id: configImplementSectionsZones

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
                text: "Section " + Number(model.secNum+1).toLocaleString(Qt.locale(),"f",0)
                anchors.bottom: spinner.top
                anchors.left: spinner.left
            }
            Text {
                id: spin_message
                visible: false
                text: "message"
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
            id: zonesNumber
            objectName: "zonesNumber"
            implicitWidth: 150
            implicitHeight: 50
            fromVal: 1
            valueVal: 4
            toVal: 8
            title: "Zones"
        }
        SpinBoxCustomized{
            id: sectionWidthAllZones
            objectName: "sectionWidthAllZones"
            implicitWidth: 150
            implicitHeight: 50
            fromVal: 1
            valueVal: 240
            toVal: 300
            title: "Section Width"
        }
        SpinBox{
            id: numOfSections
            objectName: "numOfSections"
            from: 0
            value: 40
            to: 64
            onValueChanged: function (which = value) {
                console.warn("We got this one: " + which);
                section_model.clear();
                for (var i=0; i < which+1; i++) {
                    console.warn("Writing new section " + i)
                    section_model.append( { secNum: i, width: 5 } )
                }
            }

            Component.onCompleted: {
                if (section_model.count < (value + 1)) {
                    for (var i=section_model.count; i < value+1 ; i ++ ) {
                         section_model.append( { secNum: i, width: 5 } )
                    }
                }
            }
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
