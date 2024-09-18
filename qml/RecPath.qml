import QtQuick 2.0
import QtQuick.Controls.Fusion

import "components" as Comp

Comp.MoveablePopup{
    id: recPath
    height: parent.height *.5
    x: 40
    y: 40
    width: recPathColumn.width + 20
    closePolicy: Popup.NoAutoClose
    function show(){
        recPath.visible = !recPath.visible
    }
    onVisibleChanged: if(visible)
                          recordedPathInterface.recPathLoad()
                      else
                          recordedPathInterface.recPathClear()

    Comp.ScrollViewExpandableColumn{
        id: recPathColumn
        width: children.width
        height: parent.height-20
        anchors.centerIn: parent
        Comp.IconButtonTransparent{
            icon.source: "/images/Play.png"
            iconChecked: "/images/boundaryStop.png"
            onClicked: recordedPathInterface.recPathFollowStop()
        }
        Comp.IconButtonTransparent{
            icon.source: "/images/Stop.png"
            onClicked: recordedPathInterface.recPathResumeStyle()
        }
        Comp.IconButtonTransparent{
            icon.source: "/images/BoundaryRecord.png"
            iconChecked: "/images/boundaryStop.png"
            onClicked: recordedPathInterface.recPathRecordStop()
        }
        Comp.IconButtonTransparent{
            icon.source: "/images/FileExplorerWindows.png"
        }
        Comp.IconButtonTransparent{
            icon.source: "/images/ABSwapPoints.png"
            onClicked: recordedPathInterface.recPathSwapAB()
        }
    }
}
