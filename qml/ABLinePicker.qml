import QtQuick
import QtQuick.Controls.Fusion
//import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQml.Models
import "components"

Dialog {
    //AOGInterface {
    //    id: aog //temporary
    //}
    id: abLinePickerDialog

    width: 600
    height: 400

    modal: true
    standardButtons: "NoButton"
    title: qsTr("AB Line")

    //signal updateABLines()
    //signal deleteLine(int lineno)
    //signal changeName(int lineno)
    //signal addLine(string name, double easting, double northing, double heading)
    //signal setA(bool start_cancel); //true to mark an A point, false to cancel new point

    Connections {
        target: linesInterface
        function onAbLinesListChanged() {
            abLinePickerDialog.reloadModel()
        }
    }

    function reloadModel() {
        ablineModel.clear()
        for( var i = 0; i < linesInterface.abLinesList.length ; i++ ) {
            ablineModel.append(linesInterface.abLinesList[i])
        }
        if (aog.currentABCurve >-1)
            ablineView.currentIndex = aog.currentABCurve

    }

    onVisibleChanged:  {
        //when we show or hide the dialog, ask the main
        //program to update our lines list in the
        //AOGInterface object
        linesInterface.abLine_updateLines()
        ablineView.currentIndex = aog.currentABLine
        //preselect first AB line if none was in use before
        //to make it faster for user
        if (ablineView.currentIndex < 0)
            if (linesInterface.abLinesList.length > 0)
                ablineView.currentIndex = 0
    }

    Rectangle{
        anchors.fill: parent
        border.width: 1
        border.color: aog.blackDayWhiteNight
        color: aog.backgroundColor
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
                onClicked: {
                    if(ablineView.currentIndex > -1) {
                        copyLineName.set_name("Copy of " + linesInterface.abLinesList[ablineView.currentIndex].name)
                        copyLineName.visible = true
                    }
                }
            }
            IconButtonTransparent{
                objectName: "btnLineEdit"
                icon.source: "/images/FileEditName.png"
                onClicked: {
                    if (ablineView.currentIndex > -1) {
                        editLineName.set_name(linesInterface.abLinesList[ablineView.currentIndex].name)
                        editLineName.visible = true
                    }
                }
            }
            IconButtonTransparent{
                objectName: "btnLineSwapPoints"
                icon.source: "/images/ABSwapPoints.png"
                onClicked: {
                    if(ablineView.currentIndex > -1)
                        linesInterface.abLine_swapHeading(ablineView.currentIndex);
                }
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

                    onClicked: {
                        if (ablineView.currentIndex > -1) {
                            if (aog.currentABLine == ablineView.currentIndex)
                                aog.currentABLine = -1
                            linesInterface.abLine_deleteLine(ablineView.currentIndex)
                            ablineView.currentIndex = -1
                        }
                    }
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
                        abLinePickerDialog.close()
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
            modal: true
            height: 550
            //border.width: 1
            //border.color: "black"
            //z: 1

            standardButtons: "NoButton"
            property double a_easting
            property double a_northing
            property double a_longitude
            property double a_latitude
            property double b_easting
            property double b_northing
            property double b_latitude
            property double b_longitude
            property bool a_set
            property double heading //radians
            property double heading_degrees
            background: Rectangle{
                id: backgroundABSetter
                border.width: 1
                border.color: aog.blackDayWhiteNight
                color: aog.backgroundColor
                anchors.fill: parent
            }

            visible: false

            onVisibleChanged: {
                if (visible === true) {
                    a_set = false;
                    a_stuff.visible = true;
                    b_stuff.visible = false
                    headingSpinbox.value = 0
                }
            }

            onRejected: {
                abLinePickerDialog.visible = true
            }

            onAccepted: {
                //emit signal to create the line
                //abLinePickerDialog.addLine(
                linesInterface.abLine_addLine(newLineName.abLineName,a_easting, a_northing, heading)
                linesInterface.abLine_updateLines()
                var count = linesInterface.abLinesList.length
                //ablineView.currentIndex = count -1
                //aog.currentABLine = count - 1
                abSetter.close()
                abLinePickerDialog.open()
            }

            Rectangle {
                id: a_stuff
                anchors.left: parent.left
                anchors.right: parent.right
                height: childrenRect.height
                color: "transparent"

                Text {
                    id: a_label
                    text:  qsTr("Drive to AB line <b>start</b> point and press the A button or enter Latitude and Longitude.")
                    wrapMode: Text.WordWrap
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 5
                }
                IconButtonTransparent{
                    objectName: "a"
                    id: a
                    anchors.top: a_label.bottom
                    anchors.left: parent.left
                    anchors.margins: 5
                    checkable: false
                    icon.source: "/images/LetterABlue.png"
                    onClicked: {
                        abSetter.a_easting = aog.toolEasting
                        abSetter.a_northing = aog.toolNorthing
                        abSetter.heading = aog.toolHeading
                        abSetter.heading_degrees = aog.toolHeading * 180 / Math.PI
                        linesInterface.abLine_setA(true, abSetter.a_easting, abSetter.a_northing, abSetter.heading)
                        a_manual_latitude.set_without_onchange(aog.toolLatitude)
                        a_manual_longitude.set_without_onchange(aog.toolLongitude)
                        b_stuff.visible = true

                        headingSpinbox.set_without_onchange(abSetter.heading_degrees)
                    }
                }

                GridLayout {
                    anchors.top: a_label.bottom
                    anchors.left: a.right
                    anchors.right: parent.right
                    anchors.margins: 5
                    columns: 2
                    rows: 2

                    Text {
                        text: qsTr("Lat","Latitude abbreviation")
                    }
                    LatLonTextField {
                        id: a_manual_latitude
                        Layout.fillWidth: true
                        placeholderText: qsTr("Latitude")

                        onManualTextChanged: {
                            if (a_manual_longitude.text != "") {
                                b_stuff.visible = true
                            }
                        }

                        onEditingFinished: {
                            if (a_manual_longitude.text != "") {
                                const [northing, easting] = aog.convertWGS84ToLocal(Number(a_manual_latitude.text), Number(a_manual_longitude.text))
                                abSetter.a_easting = easting
                                abSetter.a_northing = northing
                                abSetter.heading = aog.toolHeading
                                abSetter.heading_degrees = aog.toolHeading * 180 / Math.PI
                                linesInterface.abLine_setA(true, abSetter.a_easting, abSetter.a_northing, abSetter.heading)
                            }
                        }
                    }
                    Text {
                        text: qsTr("Lon","Longitude abbreviation")
                    }
                    LatLonTextField {
                        id: a_manual_longitude
                        Layout.fillWidth: true
                        placeholderText: qsTr("Longitude")
                        onManualTextChanged: {
                             if (a_manual_latitude.text != "") {
                                b_stuff.visible = true;
                             }
                        }
                        onEditingFinished: {
                             if (a_manual_latitude.text != "") {
                                const [northing, easting] = aog.convertWGS84ToLocal(Number(a_manual_latitude.text), Number(a_manual_longitude.text))
                                abSetter.a_easting = easting
                                abSetter.a_northing = northing
                                linesInterface.abLine_setA(true, abSetter.a_easting, abSetter.a_northing, abSetter.heading)
                            }
                        }
                    }
                }
            }

            Rectangle {
                id: b_stuff
                anchors.top: a_stuff.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                color: "transparent"

                Text {
                    id: b_label
                    text:  qsTr("For an A+ line, enter the heading.  Otherwise drive to AB line <b>end</b> point and press the B button, or manually enter Latitude and Longitude.")
                    wrapMode: Text.WordWrap
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 5
                }

                SpinBox{
                    id: headingSpinbox
                    objectName: "heading"
                    from: 0
                    to: 35999999
                    stepSize: 10000
                    value: 0
                    editable: true

                    property real realValue: value/ 100000
                    property int decimals: 2

                    property bool suppress_onchange: false

                    function set_without_onchange(new_value) {
                        suppress_onchange = true
                        value = new_value * 100000
                        suppress_onchange = false
                    }

                    anchors.top: b_label.bottom
                    anchors.margins: 5
                    anchors.horizontalCenter: parent.horizontalCenter

                    onValueChanged: {
                        if (! suppress_onchange) {
                            abSetter.heading = value / 100000 * Math.PI / 180.0
                            abSetter.heading_degrees = value / 100000

                            linesInterface.abLine_setA(true, abSetter.a_easting, abSetter.a_northing, abSetter.heading)

                            //calculate b latitude and longitude for the display
                            //use 100m
                            abSetter.b_easting = Math.cos(abSetter.heading) * 100 + abSetter.a_easting
                            abSetter.b_northing = Math.sin(abSetter.heading) * 100 + abSetter.a_northing

                            const [latitude, longitude] = aog.convertLocalToWGS84(abSetter.b_northing, abSetter.b_easting)
                            b_manual_latitude.set_without_onchange(latitude)
                            b_manual_longitude.set_without_onchange(longitude)
                        }
                    }

                    textFromValue: function(value, locale) {
                        return Number(value / 100000).toLocaleString(locale, 'f', decimals)
                    }

                    valueFromText: function(text, locale) {
                        return Number.fromLocaleString(locale, text) * 100000
                    }
                }

                IconButtonTransparent{
                    objectName: "b"
                    id: b
                    anchors.top: headingSpinbox.bottom
                    anchors.left: parent.left
                    anchors.margins: 5
                    checkable: false
                    icon.source: "/images/LetterBBlue.png"

                    onClicked: {
                        abSetter.b_easting = aog.toolEasting
                        abSetter.b_northing = aog.toolNorthing

                        b_manual_latitude.set_without_onchange(aog.toolLatitude)
                        b_manual_longitude.set_without_onchange(aog.toolLongitude)

                        abSetter.heading = Math.atan2(abSetter.b_easting - abSetter.a_easting,
                                                 abSetter.b_northing - abSetter.a_northing)

                        if (abSetter.heading < 0) abSetter.heading += 2 * Math.PI

                        abSetter.heading_degrees = abSetter.heading * 180.0 / Math.PI

                        headingSpinbox.set_without_onchange(abSetter.heading_degrees)
                    }
                }
                GridLayout {
                    anchors.top: headingSpinbox.bottom
                    anchors.left: b.right
                    anchors.right: parent.right
                    anchors.margins: 5
                    columns: 2
                    rows: 2

                    Text {
                        text: qsTr("Lat","Latitude abbreviation")
                    }
                    LatLonTextField {
                        id: b_manual_latitude
                        Layout.fillWidth: true
                        placeholderText: qsTr("Latitude")
                        onEditingFinished: {
                            if (b_manual_longitude.text != "") {
                                const [northing, easting] = aog.convertWGS84ToLocal(Number(b_manual_latitude.text), Number(b_manual_longitude.text))
                                abSetter.b_easting = easting
                                abSetter.b_northing = northing

                                abSetter.heading = Math.atan2(abSetter.b_easting - abSetter.a_easting,
                                                 abSetter.b_northing - abSetter.a_northing)

                                if (abSetter.heading < 0) abSetter.heading += 2 * Math.PI

                                abSetter.heading_degrees = abSetter.heading * 180.0 / Math.PI

                                headingSpinbox.set_without_onchange(abSetter.heading_degrees)
                            }
                        }
                    }

                    Text {
                        text: qsTr("Lon","Longitude abbreviation")
                    }
                    LatLonTextField {
                        id: b_manual_longitude
                        Layout.fillWidth: true
                        placeholderText: qsTr("Latitude")
                        onEditingFinished: {
                            if (b_manual_latitude.text != "") {
                                const [northing, easting] = aog.convertWGS84ToLocal(Number(b_manual_latitude.text), Number(b_manual_longitude.text))
                                abSetter.b_easting = easting
                                abSetter.b_northing = northing
                                if (abSetter.heading < 0) abSetter.heading += 2 * Math.PI

                                abSetter.heading_degrees = abSetter.heading * 180.0 / Math.PI

                                headingSpinbox.set_without_onchange(abSetter.heading_degrees)
                            }
                        }
                    }
                }
            }


            IconButtonTransparent{
                objectName: "btnCancel"
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.margins: 10
                icon.source: "/images/Cancel64.png"
                 onClicked:{
                     //cancel
                     linesInterface.abLine_setA(false,0,0,0) //turn off line setting
                     abSetter.rejected()
                     abSetter.close()
                }
           }
           IconButtonTransparent{
               objectName: "btnOk"
               anchors.bottom: parent.bottom
               anchors.right: parent.right
               anchors.margins: 10
               icon.source: "/images/OK64.png"
               onClicked: {
                   newLineName.visible = true
                   newLineName.generate_ab_name(abSetter.heading_degrees)
                   abSetter.visible = false
               }
            }
            LineName{
                id: newLineName
                objectName: "newLineName"
                anchors.top: parent.top
                anchors.left: parent.left
                title: "AB Line"
                visible: false
                onRejected: {
                    //go back to A/B dialog
                    //do nothing
                    abSetter.visible = true
                }

                onAccepted: {
                    abSetter.accepted()
                }
            }
        }

        LineName{
            id: copyLineName
            objectName: "copyLineName"
            anchors.top:parent.top
            anchors.left: parent.left
            title: "Copy AB Line"
            visible: false
            z: 2
            onAccepted: {
                if(ablineView.currentIndex > -1) {
                    var heading = linesInterface.abLinesList[ablineView.currentIndex].heading
                    var easting = linesInterface.abLinesList[ablineView.currentIndex].easting
                    var northing = linesInterface.abLinesList[ablineView.currentIndex].northing
                    linesInterface.abLine_addLine(copyLineName.abLineName, easting, northing, heading)
                    //linesInterface.abLine_updateLines()
                    ablineView.positionViewAtEnd()
                }
            }
        }

        LineName{
            id: editLineName
            objectName: "editLineName"
            anchors.top:parent.top
            anchors.left: parent.left
            title: "AB Line"
            visible: false
            z: 1
            onAccepted: {
                if(ablineView.currentIndex > -1) {
                    linesInterface.abLine_changeName(ablineView.currentIndex, editLineName.abLineName)
                    //linesInterface.abLine_updateLines()
                    //ablineView.positionViewAtEnd()
                }
            }
        }

        Rectangle{
            id: listrect
            anchors.left: parent.left
            anchors.top:topLine.bottom
            anchors.right: rightColumn.left
            anchors.bottom: bottomRow.top
            anchors.bottomMargin: 0
            anchors.margins: 10
            color: aog.backgroundColor

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
                //property int currentIndex: -1
                clip: true

                delegate: RadioButton{
                    id: control
                    checked: ablineView.currentIndex === index ? true : false
                    indicator: Rectangle{
                        anchors.fill: parent
                        anchors.margins: 2
                        //color: (control.down) ? aog.backgroundColor : aog.blackDayWhiteNight
                        //color: (control.down) ? aog.blackDayWhiteNight : aog.backgroundColor
                        color: control.checked ? "blue" : "white"
                        visible: control.checked
                    }

                    onDownChanged: {
                        ablineView.currentIndex = index
                    }

                    width:listrect.width
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
                        //color: control.checked ? aog.backgroundColor : aog.blackDayWhiteNight
                        color: control.checked ? aog.blackDayWhiteNight : aog.backgroundColor
                        z: 2
                    }
                }
            }
        }
    }
}
