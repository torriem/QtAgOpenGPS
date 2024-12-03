import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

import ".."
import "../components"
/*This is where the track type is selected. 
 * This will open the window for the type of track.
 */

MoveablePopup {
    function show(){
        trackNew.visible = true
    }
    id: trackNew
    x: 40
    y: 40
    width: 600 * theme.scaleWidth
    height: 400 * theme.scaleHeight
    TitleFrame{
        color: "#f2f2f2"
        border.color: "black"
        title: "Tracks"
		anchors.fill: parent
		IconButtonColor{
			icon.source: prefix + "/images/Cancel64.png"
			anchors.bottom: parent.bottom
			anchors.left: parent.left
			anchors.bottomMargin: 10 * theme.scaleHeight
			anchors.leftMargin: 10 * theme.scaleWidth
			implicitWidth: 70 * theme.scaleWidth
			implicitHeight: 70 * theme.scaleHeight
			color: "#ffffff"
			onClicked: trackNew.visible = false
		}
		IconButtonColor{ //todo
			id: btnLoadFromKML
			icon.source: prefix + "/images/BoundaryLoadFromGE.png"
			anchors.verticalCenter: parent.verticalCenter
			anchors.left: parent.left
			anchors.leftMargin: 40 * theme.scaleWidth
			text: qsTr("AB + Curve")
			onClicked: {
				trackNew.visible = false
				trackNewSet.showFromKML()
			}
		}
		GridLayout{
            id: grid
			anchors.top: parent.top
			anchors.right: parent.right
			anchors.bottom: parent.bottom
			anchors.left: btnLoadFromKML.right
			anchors.topMargin: 10 * theme.scaleHeight
			anchors.leftMargin: 40 * theme.scaleWidth
			anchors.rightMargin: 10 * theme.scaleWidth
			anchors.bottomMargin: 10 * theme.scaleHeight
            flow: Grid.TopToBottom
            rows: 3
            columns: 2
			IconButtonColor{
				icon.source: prefix + "/images/ABLatLonLatLon.png"
				implicitWidth: 100 * theme.scaleWidth
				implicitHeight: 100 * theme.scaleHeight
				color: "#ffffff"
				text: qsTr("Lat/Lon")
				onClicked: {
					trackNew.visible = false
					trackNewSet.showLatLonLatLon()
				}
			}
			IconButtonColor{
				icon.source: prefix + "/images/ABLatLonHeading.png"
				implicitWidth: 100 * theme.scaleWidth
				implicitHeight: 100 * theme.scaleHeight
				color: "#ffffff"
				text: qsTr("Lat/Heading")
				onClicked: {
					trackNew.visible = false
					trackNewSet.showLatLonHeading()
				}
			}
			IconButtonColor{
				icon.source: prefix + "/images/ABPivot.png"
				implicitWidth: 100 * theme.scaleWidth
				implicitHeight: 100 * theme.scaleHeight
				color: "#ffffff"
				text: qsTr("Pivot")
				onClicked: {
					trackNew.visible = false
					trackNewSet.showCircleTrack()
				}
			}
            IconButtonColor{
                icon.source: prefix + "/images/ABTrackA+.png"
                implicitWidth: 100 * theme.scaleWidth
                implicitHeight: 100 * theme.scaleHeight
                color: "#ffffff"
				text: qsTr("A+")
				onClicked: {
					trackNew.visible = false
					trackNewSet.showAPlus()
				}
            }
            IconButtonColor{
                icon.source: prefix + "/images/ABTrackAB.png"
                implicitWidth: 100 * theme.scaleWidth
                implicitHeight: 100 * theme.scaleHeight
                color: "#ffffff"
				text: qsTr("AB Line")
				onClicked: {
					trackNew.visible = false
					trackNewSet.showAB()
				}
            }
            IconButtonColor{
                icon.source: prefix + "/images/ABTrackCurve.png"
                implicitWidth: 100 * theme.scaleWidth
                implicitHeight: 100 * theme.scaleHeight
                color: "#ffffff"
				text: qsTr("AB Curve")
				onClicked: {
					trackNew.visible = false
					trackNewSet.showABCurve()
				}
            }
        }
    }
}
