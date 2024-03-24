import QtQuick 2.0
import QtQuick.Controls 2.5

import "components"

MoveablePopup{
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
                          aog.recPathLoad()
                      else
                          aog.recPathClear()

    ScrollViewExpandableColumn{
        id: recPathColumn
        width: children.width
        height: parent.height-20
        anchors.centerIn: parent
        IconButtonTransparent{
            icon.source: "/images/Play.png"
            iconChecked: "/images/boundaryStop.png"
            onClicked: aog.recPathFollowStop()
        }
        IconButtonTransparent{
            icon.source: "/images/Stop.png"
            onClicked: aog.recPathResumeStyle()
        }
        IconButtonTransparent{
            icon.source: "/images/BoundaryRecord.png"
            iconChecked: "/images/boundaryStop.png"
            onClicked: aog.recPathRecordStop()
        }
        IconButtonTransparent{
            icon.source: "/images/FileExplorerWindows.png"
        }
        IconButtonTransparent{
            icon.source: "/images/ABSwapPoints.png"
            onClicked: aog.recPathSwapAB()
        }
    }
}
