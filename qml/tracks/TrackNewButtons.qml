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
    width: 250
    height: 300
    TitleFrame{
        color: "#f2f2f2"
        border.color: "black"
        title: "Tracks"
        anchors.fill: parent
        GridLayout{
            id: grid
            anchors.fill: parent
            anchors.margins: 10
            flow: Grid.LeftToRight
            rows: 2
            columns: 2
            IconButtonColor{
                icon.source: "/images/ABTrackA+.png"
                implicitWidth: 100
                implicitHeight: 100
                color: "#ffffff"
            }
            IconButtonColor{
                icon.source: "/images/ABTrackAB.png"
                implicitWidth: 100
                implicitHeight: 100
                color: "#ffffff"
            }
            IconButtonColor{
                icon.source: "/images/Cancel64.png"
                implicitWidth: 70
                implicitHeight: 70
                color: "#ffffff"
                Layout.alignment:  Qt.alignBottom && Qt.alignLeft
                onClicked: trackNew.visible = false
            }
            IconButtonColor{
                icon.source: "/images/ABTrackCurve.png"
                implicitWidth: 100
                implicitHeight: 100
                color: "#ffffff"
            }
        }
    }
}
