import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3
import QtQml.Models 2.3

Dialog {
    //AOGInterface {
    //    id: aog //temporary
    //}
    id: abLinePickerDialog

    width: 600
    height: 400

    modality: Qt.WindowModal
    standardButtons: StandardButton.NoButton
    title: qsTr("AB Line")

    signal updateABLines()
    signal switchToLine(int lineno) //redundant? use aogproperty
    signal deleteLine(int lineno)
    signal changeName(int lineno)
    signal addLine(string name, double easting, double northing, double heading)
    signal setA(bool start_cancel); //true to mark an A point, false to cancel new point

    Connections {
        target: aog
        function onAbLinesListChanged() {
            abLinePickerDialog.reloadModel()
        }
    }

    function reloadModel() {
        ablineModel.clear()
        for( var i = 0; i < aog.abLinesList.length ; i++ ) {
            ablineModel.append(aog.abLinesList[i])
        }
        if (aog.currentABCurve >-1)
            ablineView.currentIndex = aog.currentABCurve

    }

    onVisibleChanged:  {
        //when we show or hide the dialog, ask the main
        //program to update our lines list in the
        //AOGInterface object
        updateABLines()
        ablineView.currentIndex = aog.currentABLine
        //preselect first AB line if none was in use before
        //to make it faster for user
        if (ablineView.currentIndex < 0)
            if (aog.abLinesList.length > 0)
                ablineView.currentIndex = 0
    }

    Rectangle{
        anchors.fill: parent
        border.width: 1
        border.color: "black"
        color: "lightgray"
        TopLine{
            id: topLine
            titleText: "AB Line"
        }
        ColumnLayout{
            id: rightColumn
            anchors.top: topLine.bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: 1
            anchors.bottomMargin: 1
            width: childrenRect.width
            IconButtonTransparent{
                objectName: "btnLineCopy"
                icon.source: "/images/FileCopy.png"
                onClicked: copyLineName.visible = true
            }
            IconButtonTransparent{
                objectName: "btnLineEdit"
                icon.source: "/images/FileEditName.png"
                onClicked: editLineName.visible = true
            }
            IconButtonTransparent{
                objectName: "btnLineSwapPoints"
                icon.source: "/images/ABSwapPoints.png"
            }
            IconButtonTransparent{
                objectName: "btnLineExit"
                icon.source: "/images/OK64.png"
                onClicked: {
                    if (ablineView.currentIndex > -1) {
                        aog.currentABLine = ablineView.currentIndex
                        abLinePickerDialog.accept()
                    } else
                        abLinePickerDialog.reject()
                }
            }
        }

        Rectangle{
            id: bottomRow
            anchors.bottom: parent.bottom
            anchors.right: rightColumn.left
            anchors.left: parent.left
            anchors.bottomMargin: 1
            anchors.leftMargin: 1
            height: btnLineDelete.height + 10
            color: parent.color
            z: 1


            RowLayout{
                anchors.fill: parent

                IconButtonTransparent{
                    id: btnLineDelete
                    objectName: "btnLineDelete"
                    icon.source: "/images/ABLineDelete.png"
                }
                IconButtonTransparent{
                    objectName: "btnLineExit"
                    icon.source: "/images/SwitchOff.png"
                    onClicked: {
                        aog.currentABLine = -1
                        ablineView.currentIndex = -1
                        abLinePickerDialog.reject()
                    }
                }
                IconButtonTransparent{
                    objectName: "btnLineAdd"
                    icon.source: "/images/AddNew.png"
                    onClicked: {
                        abSetter.visible = true
                        abLinePickerDialog.visible = false
                    }
                }
                IconButtonTransparent{
                    objectName: "btnLineLoadFromKML"
                    icon.source: "/images/BoundaryLoadFromGE.png"
                }
            }
        }
        Rectangle{ //don't ask any questions
            anchors.top: topLine.bottom
            height: 10
            color: parent.color
            width: abLinePickerDialog.width
            anchors.left: abLinePickerDialog.left
            z: 1
        }
        Dialog {
            id: abSetter
            width: 300
            height: 400
            //color: "lightgray"
            //border.width: 1
            //border.color: "black"
            //z: 1
            modality: Qt.WindowModal
            standardButtons: StandardButton.NoButton

            property double a_easting
            property double a_northing
            property double b_easting
            property double b_northing
            property bool a_set;

            visible: false

            onVisibleChanged: {
                if (visible === true) {
                    a_set = false;
                    a.visible = true;
                    b.visible = false
                    headingSpinbox.value = 0
                }
            }

            onRejected: {
                console.debug("new ab line aborted.")
                abLinePickerDialog.visible = true
            }

            TopLine{
                id: settertopLine
                titleText: "AB Line"
            }
            IconButtonTransparent{
                objectName: "a"
                id: a
                anchors.top: settertopLine.bottom
                anchors.left: parent.left
                anchors.margins: 5
                checkable: false
                icon.source: "/images/LetterABlue.png"
                onClicked: {
                    abLinePickerDialog.setA(true)
                    abSetter.a_easting = aog.easting
                    abSetter.a_northing = aog.northing
                    b.visible = true
                    headingSpinbox.value = 0

                    //debugging
                    aog.easting += 5
                }
            }

            IconButtonTransparent{
                objectName: "b"
                id: b
                anchors.top: settertopLine.bottom
                anchors.right: parent.right
                anchors.margins: 5
                checkable: false
                visible: false
                icon.source: "/images/LetterBBlue.png"

                onClicked: {
                    abSetter.b_easting = aog.easting
                    abSetter.b_northing = aog.northing

                    var heading = Math.atan2(abSetter.b_easting - abSetter.a_easting,
                                             abSetter.b_northing - abSetter.a_northing)

                    heading = heading * 180.0 / Math.PI

                    headingSpinbox.value = heading * 100

                    //debugging
                    aog.northing += 5
                }
            }

//            Rectangle{
//                id: headingTextInput
//                anchors.topMargin: 20
//                anchors.top: a.bottom
//                height: 50
//                width: parent.width -50
//                anchors.horizontalCenter: parent.horizontalCenter
//                color: "white"
//                border.color: "gray"
//                border.width: 1
//                TextInput {
//                    objectName: "heading"
//                    anchors.fill: parent
//                    validator: RegExpValidator {
//                        regExp: /(\d{1,3})([.,]\d{1,5})?$/
//                    }
//                }
//            }
            SpinBox{
                id: headingSpinbox
                objectName: "heading"
                from: 0
                to: 35999
                stepSize: 10
                value: 0
                editable: true
                property real realValue: value/ 100
                property int decimals: 2

                anchors.topMargin: 20
                anchors.top: a.bottom
                width: parent.width -50
                anchors.horizontalCenter: parent.horizontalCenter
                onValueChanged: {
                    if (value == from) {
                        spin_message.visible = true
                        spin_message.text = "Must be "+from/1000000+" or greater"
                    } else if(value == to){
                        spin_message.visible = true
                        spin_message.text = "Can't be larger than " + to/ 1000000
                    }else {
                        spin_message.visible = false
                    }

                    //some validation here
                    //emit signal.  We know our section number because it's in the model
                }
                textFromValue: function(value, locale) {
                    return Number(value / 100).toLocaleString(locale, 'f', decimals)
                }

                valueFromText: function(text, locale) {
                    return Number.fromLocaleString(locale, text) * 100
                }
                Text {
                    id: spin_message
                    visible: false
                    text: "message"
                    color: "red"
                    anchors.top: parent.bottom
                    anchors.left: parent.left
                }
            }

            IconButtonTransparent{
               id: fancyEditor
               anchors.top: headingSpinbox.bottom
               anchors.topMargin: 20
               anchors.horizontalCenter: parent.horizontalCenter
               icon.source: "/images/FileEditName.png"
            }

           IconButtonTransparent{
               objectName: "btnCancel"
               anchors.bottom: parent.bottom
               anchors.left: parent.left
               anchors.margins: 20
               icon.source: "/images/Cancel64.png"
               onClicked:{
                   parent.visible = false
               }
           }
           IconButtonTransparent{
               objectName: "btnOk"
               anchors.bottom: parent.bottom
               anchors.right: parent.right
               anchors.margins: 20
               icon.source: "/images/OK64.png"
               onClicked: newLineName.visible = true
           }
           LineName{
               id: newLineName
               objectName: "newLineName"
               anchors.top: parent.top
               anchors.left: parent.left
               title: "AB Line"
               visible: false
           }
        }

        LineName{
            id: copyLineName
            objectName: "copyLineName"
            anchors.top:parent.top
            anchors.left: parent.left
            title: "AB Line"
            visible: false
            z: 2
        }
        LineName{
            id: editLineName
            objectName: "editLineName"
            anchors.top:parent.top
            anchors.left: parent.left
            title: "AB Line"
            visible: false
            z: 1
        }

        Rectangle{
            anchors.left: parent.left
            anchors.top:topLine.bottom
            anchors.right: rightColumn.left
            anchors.bottom: bottomRow.top
            anchors.bottomMargin: 0
            anchors.margins: 10
            color: "white"

            ListModel {
                id: ablineModel
                objectName: "ablineModel"
            }

            Component.onCompleted: {
                reloadModel()
            }

            ListView {
                id: ablineView
                anchors.fill: parent
                model: ablineModel
                property int currentIndex: -1

                delegate: RadioButton{
                    id: control
                    checked: ablineView.currentIndex === index ? true : false
                    indicator: Rectangle{
                        anchors.fill: parent
                        anchors.margins: 2
                        color: (control.down) ? "white" : "blue"
                        visible: control.checked
                    }

                    onDownChanged: {
                        ablineView.currentIndex = index
                    }

                    width:parent.width
                    height:50
                    //anchors.fill: parent
                    //color: "light gray"
                    Text{
                        anchors.left: parent.left
                        anchors.leftMargin: 5
                        anchors.verticalCenter: parent.verticalCenter
                        text: model.name
                        font.pixelSize: 25
                        font.bold: true
                        color: control.checked ? "white" : "black"
                        z: 2
                    }
                }
            }
        }
    }
}
