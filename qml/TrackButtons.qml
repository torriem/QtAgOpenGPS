import QtQuick 2.0
import QtQuick.Controls 2.5

TimedRectangle {
    id: trackButtons
    color: "white"
    width: children.width+20
    height: children.height+20
    Row{
        spacing: 4
        anchors.margins: 10
        width: marker.width*6+16
        height: 70
        IconButtonTransparent{
            id: marker
            icon.source: "/images/ABSnapNudgeMenuRef.png"
        }
        IconButtonColor{
            icon.source: "/images/AutoSteerSnapToPivot.png"
        }
        IconButtonTransparent{
            icon.source: "/images/SwitchOff.png"
        }
        IconButtonTransparent{
            icon.source: "/images/ABDraw.png"
        }
        IconButtonTransparent{
            icon.source: "/images/ABTracks.png"
        }
        IconButtonTransparent{
            icon.source: "/images/ABSnapNudgeMenu.png"
        }
    }
}
