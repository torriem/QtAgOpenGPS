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
        anchors.margins: 15
        model: section_model
        cellWidth: 200


        delegate: Rectangle {
            SpinBox {
                anchors.left: parent.right
                implicitWidth: 150
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
            Text{
                text: qsTr(1+"->")
                anchors.right: spinner.left
                anchors.verticalCenter: spinner.verticalCenter
                font.bold: true
                font.pixelSize: 15
            }

            Text {
                text: qsTr("Zone " + Number(model.secNum+1).toLocaleString(Qt.locale(),"f",0))
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
        SpinBox{
            id: numOfZones
			anchors.verticalCenter: parent.verticalCenter
            objectName: "numOfZones"
            from: 1
            value: 4
            to: 8
            editable: true
            onValueChanged: function (which = value) {
                console.warn("We got this one: " + which);
                section_model.clear();
				for (var i=0; i < which; i++) {
                    console.warn("Writing new section " + i)
                    section_model.append( { secNum: i, width: 5 } )
                }
            }

            Component.onCompleted: {
                if (section_model.count < (value)) {
                    for (var i=section_model.count; i < value ; i ++ ) {
                         section_model.append( { secNum: i, width: 5 } )
                    }
                }
            }
            Text{
                anchors.bottom: parent.top
                text: qsTr("Zones")
            }
        }
        SpinBoxCustomized{
            id: sectionWidthAllZones
            objectName: "sectionWidthAllZones"
            implicitWidth: 150
            implicitHeight: 50
            from: 1
            value: 240
            to: 300
            text: qsTr("Section Width")
        }
        SpinBoxCustomized{
            id: sectionsNumber
            objectName: "sectionsNumber"
            implicitWidth: 150
            implicitHeight: 50
            from: 1
            value: 40
            to: 60
            text: qsTr("Sections")

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
