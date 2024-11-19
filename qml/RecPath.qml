// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Recorded path display buttons
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
            icon.source: prefix + "/images/Play.png"
            iconChecked: prefix + "/images/boundaryStop.png"
            onClicked: recordedPathInterface.recPathFollowStop()
        }
        Comp.IconButtonTransparent{
            icon.source: prefix + "/images/Stop.png"
            onClicked: recordedPathInterface.recPathResumeStyle()
        }
        Comp.IconButtonTransparent{
            icon.source: prefix + "/images/BoundaryRecord.png"
            iconChecked: prefix + "/images/boundaryStop.png"
            onClicked: recordedPathInterface.recPathRecordStop()
        }
        Comp.IconButtonTransparent{
            icon.source: prefix + "/images/FileExplorerWindows.png"
        }
        Comp.IconButtonTransparent{
            icon.source: prefix + "/images/ABSwapPoints.png"
            onClicked: recordedPathInterface.recPathSwapAB()
        }
    }
}
