import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQml.Models
import "components"
import ".."

Dialog {
    id: abCurvePickerDialog
    width: 600
    height: 400

    modal: true
    standardButtons: StandardButton.NoButton
    title: qsTr("AB Curve")

    signal updateABCurves()
    signal switchToCurve(int lineno) //redundant? use aogproperty
    signal deleteCurve(int lineno)

    Connections {
        target: linesInterface
        function onAbCurvesListChanged() {
            abCurvePickerDialog.reloadModel()
        }
    }

    function reloadModel() {
        abcurveModel.clear()
        for( var i = 0; i < linesInterface.abCurvesList.length ; i++ ) {
            abcurveModel.append(linesInterface.abCurvesList[i])
        }
        if (aog.currentABCurve >-1){
            abcurveView.currentIndex = aog.currentABCurve
        }

    }

    onVisibleChanged:  {
        //when we show or hide the dialog, ask the main
        //program to update our lines list in the
        //AOGInterface object
        updateABCurves()
        abcurveView.currentIndex = aog.currentABCurve
        //preselect first AB line if none was in use before
        //to make it faster for user
        if (abcurveView.currentIndex < 0)
            if (linesInterface.abCurvesList.length > 0)
                abcurveView.currentIndex = 0
    }

    Rectangle{
        anchors.fill: parent
        border.width: 1
        border.color: aog.blackDayWhiteNight
        color: aog.borderColor
        TopLine{
            id: topLine
            titleText: "AB Curve"
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
                icon.source: "/images/FileCopy.png"
                onClicked: copyCurveName.visible = true
            }
            IconButtonTransparent{
                 onClicked: editCurveName.visible = true
               icon.source: "/images/FileEditName.png"
            }
            IconButtonTransparent{
                icon.source: "/images/ABSwapPoints.png"
            }
            IconButtonTransparent{
                icon.source: "/images/OK64.png"
                onClicked: {
                    if (abcurveView.currentIndex > -1) {
                        aog.currentABCurve = abcurveView.currentIndex
                        abCurvePickerDialog.accept()
                    } else
                        abCurvePickerDialog.reject()
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
                    icon.source: "/images/ABLineDelete.png"
                }
                IconButtonTransparent{
                    icon.source: "/images/SwitchOff.png"
                    onClicked: {
                        aog.currentABCurve = -1
                        abcurveView.currentIndex = -1
                        abCurvePickerDialog.reject()
                    }
                }
                IconButtonTransparent{
                    icon.source: "/images/AddNew.png"
                    onClicked: abSetter.visible = true
                }
                IconButtonTransparent{
                    icon.source: "/images/BoundaryLoadFromGE.png"
                }
            }
        }
        Rectangle{ //don't ask any questions
            anchors.top: topLine.bottom
            height: 10
            color: parent.color
            width: abCurvePickerDialog.width
            anchors.left: abCurvePickerDialog.left
            z: 1
        }
        Rectangle{
            id: abSetter
            anchors.left: parent.left
            anchors.top: parent.top
            width: 300
            height: 400
            color: aog.backgroundColor
            border.width: 1
            border.color: aog.blackDayWhiteNight
            z: 1
            visible: false
            TopLine{
                id: settertopLine
                titleText: "AB Curve"
            }
            IconButtonTransparent{
                id: a
                anchors.top: settertopLine.bottom
                anchors.left: parent.left
                anchors.margins: 5
                isChecked: false
                icon.source: "/images/LetterABlue.png"
            }
            IconButtonTransparent{
                anchors.top: settertopLine.bottom
                anchors.right: parent.right
                anchors.margins: 5
                isChecked: true
                onClicked: newLineName.visible = true
                icon.source: "/images/LetterBBlue.png"
            }
            IconButtonTransparent{
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.margins: 20
                icon.source: "/images/boundaryPause.png"
            }
            IconButtonTransparent{
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 20
                icon.source: "/images/Cancel64.png"
                onClicked:{
                    parent.visible = false
                }
            }
            LineName{
                id: newLineName
                anchors.top: parent.top
                anchors.left: parent.left
                title: "AB Curve"
                visible: false
            }
        }

        LineName{
            id: copyCurveName
            anchors.top:parent.top
            anchors.left: parent.left
            title: "AB Curve"
            visible: false
            z: 2
        }
        LineName{
            id: editCurveName
            anchors.top:parent.top
            anchors.left: parent.left
            title: "AB Curve"
            visible: false
            z: 1
        }

        Rectangle{
            id: picker
            anchors.left: parent.left
            anchors.top:topLine.bottom
            anchors.right: rightColumn.left
            anchors.bottom: bottomRow.top
            anchors.bottomMargin: 0
            anchors.margins: 10
            color:aog.borderColor

            ListModel {
                id: abcurveModel
            }

            Component.onCompleted: {
                reloadModel()
            }

            ListView {
                id: abcurveView
                anchors.fill: parent
                model: abcurveModel
                //property int currentIndex: -1

                delegate: RadioButton{
                    id: control
                    checked: abcurveView.currentIndex === index ? true : false
                    indicator: Rectangle{
                        anchors.fill: parent
                        anchors.margins: 2
                        color: control.down ? aog.backgroundColor : "blue"
                        visible: control.checked
                    }
                    onDownChanged: {
                        abcurveView.currentIndex = index
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
                        color: control.checked ? aog.backgroundColor : aog.blackDayWhiteNight
                        z: 2
                    }
                }
            }
        }
    }
}
