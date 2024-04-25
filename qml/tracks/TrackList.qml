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
    id: trackPickerDialog

    width: 600
    height: 450

    modal: true

    //signal updateTracks()
    //signal deleteLine(int lineno)
    //signal changeName(int lineno)
    //signal addLine(string name, double easting, double northing, double heading)
    //signal setA(bool start_cancel); //true to mark an A point, false to cancel new point

    Connections {
        target: linesInterface
        function onAbLinesListChanged() {
            trackPickerDialog.reloadModel()
        }
    }
	function show() {
		trackPickerDialog.visible = true
	}

    function reloadModel() {
        trackModel.clear()
        for( var i = 0; i < linesInterface.abLinesList.length ; i++ ) {
            trackModel.append(linesInterface.abLinesList[i])
        }
        if (aog.currentABCurve >-1)
            trackView.currentIndex = aog.currentABCurve

    }

    onVisibleChanged:  {
        //when we show or hide the dialog, ask the main
        //program to update our lines list in the
        //AOGInterface object
        linesInterface.abLine_updateLines()
        trackView.currentIndex = aog.currentTrack
        //preselect first AB line if none was in use before
        //to make it faster for user
        if (trackView.currentIndex < 0)
            if (linesInterface.abLinesList.length > 0)
                trackView.currentIndex = 0
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
					if (trackView.currentIndex > -1) {
						if (aog.currentTrack == trackView.currentIndex)
						aog.currentTrack = -1
						linesInterface.abLine_deleteLine(trackView.currentIndex)
						trackView.currentIndex = -1
					}
				}
            }
            IconButtonTransparent{
                icon.source: "/images/FileEditName.png"
                onClicked: {
                    if (trackView.currentIndex > -1) {
                        editLineName.set_name(linesInterface.abLinesList[trackView.currentIndex].name)
                        editLineName.visible = true
                    }
                }
            }
            IconButtonTransparent{
                objectName: "btnLineCopy"
                icon.source: "/images/FileCopy.png"
                onClicked: {
                    if(trackView.currentIndex > -1) {
                        copyLineName.set_name("Copy of " + linesInterface.abLinesList[trackView.currentIndex].name)
                        copyLineName.visible = true
                    }
                }
            }
            IconButtonTransparent{
                objectName: "btnLineSwapPoints"
                icon.source: "/images/ABSwapPoints.png"
                onClicked: {
                    if(trackView.currentIndex > -1)
                        linesInterface.abLine_swapHeading(trackView.currentIndex);
                }
            }
            IconButtonTransparent{
				icon.source: "/images/Cancel64.png"
				onClicked: {
					aog.currentTrack = -1
					trackView.currentIndex = -1
					trackPickerDialog.reject()
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
					trackPickerDialog.close()
				}
			}
            IconButtonTransparent{
                objectName: "btnLineExit" //this is not cancel, rather, save and exit
                icon.source: "/images/OK64.png"
                onClicked: {
                    if (trackView.currentIndex > -1) {
                        aog.currentTrack = trackView.currentIndex
                        trackPickerDialog.accept()
                    } else
                        trackPickerDialog.reject()
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
            color: "green"

            ListModel { //this will be populated by the backend cpp code
                id: trackModel
                //objectName: "trackModel"
			}

            Component.onCompleted: {
                reloadModel()
            }

            ListView {
                id: trackView
                anchors.fill: parent
                model: trackModel
                //property int currentIndex: -1
                clip: true

				delegate: Rectangle{
					id: controlRect
					anchors.fill: parent
					width:listrect.width
					height:50
					Image{
						id: isLineOrCurve
						height: parent.height
						width: height
						source: "/images/TrackLine.png"
					}

					RadioButton{
						id: control
						checked: trackView.currentIndex === index ? true : false
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
						
						anchors.left: isLineOrCurve.right
						height: parent.height

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
					ButtonColor{
						id: isHidden
						anchors.right: parent.right
						height: parent.height * .8
						width: height
						anchors.verticalCenter: parent.verticalCenter
						color: "green"
						colorChecked: "red"
				}
            }
        }

        Rectangle{ //for some reason, listview will display on top of its parent, this blocks that
			id: bottomRow
            anchors.bottom: parent.bottom
            height: 10
            color: parent.color
            width: trackPickerDialog.width
            anchors.left: trackPickerDialog.left
            z: 1
        }
    }
}
