import QtQuick
import QtQuick.Controls
//import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQml.Models
import "../components"

import ".."

MoveablePopup {
    //AOGInterface {
    //    id: aog //temporary
    //}
    id: abLinePickerDialog

    width: 600
    height: 450

    modal: true

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
	function show() {
		abLinePickerDialog.visible = true
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
            titleText: "Tracks"
        }
        ColumnLayout{
            id: leftColumn
            anchors.top: topLine.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.rightMargin: 1
            anchors.bottomMargin: 1
            width: childrenRect.width
            IconButtonTransparent{
				icon.source: "/images/Trash.png"
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
                icon.source: "/images/FileEditName.png"
                onClicked: {
                    if (ablineView.currentIndex > -1) {
                        editLineName.set_name(linesInterface.abLinesList[ablineView.currentIndex].name)
                        editLineName.visible = true
                    }
                }
            }
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
                objectName: "btnLineSwapPoints"
                icon.source: "/images/ABSwapPoints.png"
                onClicked: {
                    if(ablineView.currentIndex > -1)
                        linesInterface.abLine_swapHeading(ablineView.currentIndex);
                }
            }
            IconButtonTransparent{
				icon.source: "/images/Cancel64.png"
				onClicked: {
					aog.currentABLine = -1
					ablineView.currentIndex = -1
					abLinePickerDialog.reject()
				}
			}
		}
		ColumnLayout{
			id: rightColumn
			anchors.top: topLine.bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: 1
            anchors.bottomMargin: 1
            width: childrenRect.width
            IconButtonTransparent{ //not sure what this does in aog--doesn't work on wine
                icon.source: "/images/UpArrow64.png"
            }
            IconButtonTransparent{
                icon.source: "/images/DnArrow64.png"
            }
            IconButtonTransparent{
                icon.source: "/images/ABLinesHideShow.png"
            }
			IconButtonTransparent{
				icon.source: "/images/AddNew.png"
				onClicked: {
					trackNew.show()
					abLinePickerDialog.close()
				}
			}
            IconButtonTransparent{
                objectName: "btnLineExit" //this is not cancel, rather, save and exit
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
            id: listrect
            anchors.left: leftColumn.right
            anchors.top:topLine.bottom
            anchors.right: rightColumn.left
            anchors.bottom: bottomRow.top
            anchors.bottomMargin: 0
            anchors.margins: 10
            color: aog.backgroundColor

            /*ListModel { //this will be populated by the backend cpp code
                id: ablineModel
                objectName: "ablineModel"
			}*/
			ListModel { //this is for testing
			id: ablineModel
			ListElement {text: "Line 1"}
			ListElement {text: "Line 2"}
			ListElement {text: "Line 3"}
			ListElement {text: "Line 4"}
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

        Rectangle{ //for some reason, listview will display on top of its parent, this blocks that
			id: bottomRow
            anchors.top: topLine.bottom
            height: 10
            color: parent.color
            width: abLinePickerDialog.width
            anchors.left: abLinePickerDialog.left
            z: 1
        }
    }
}
