import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

import ".."
import "../components" as Comp

Comp.MoveablePopup {
    function show(){
        trackNew.visible = true
    }
    id: trackNew
    x: 40
    y: 40
    width: 250
    height: 300
    Comp.TitleFrame{
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
            Comp.IconButtonColor{
                icon.source: "/images/ABTrackA+.png"
                implicitWidth: 100
                implicitHeight: 100
                color: "#ffffff"
            }
            Comp.IconButtonColor{
                icon.source: "/images/ABTrackAB.png"
                implicitWidth: 100
                implicitHeight: 100
                color: "#ffffff"
            }
            Comp.IconButtonColor{
                icon.source: "/images/Cancel64.png"
                implicitWidth: 70
                implicitHeight: 70
                color: "#ffffff"
                Layout.alignment:  Qt.alignBottom && Qt.alignLeft
                onClicked: trackNew.visible = false
            }
            Comp.IconButtonColor{
                icon.source: "/images/ABTrackCurve.png"
                implicitWidth: 100
                implicitHeight: 100
                color: "#ffffff"
            }
        }
    }
}
