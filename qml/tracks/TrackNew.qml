import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import ".."
import "../components"

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
