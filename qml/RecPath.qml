// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// Recorded path display buttons
import QtQuick
import QtQuick.Layouts
import Qt.labs.folderlistmodel
import QtQuick.Controls.Fusion
import QtQuick.Controls.Material

import ".."
import "./components"

MoveablePopup{
    id: recPath
    height: 500  * theme.scaleHeight
    width: 100  * theme.scaleWidth
    x: 40
    y: 40
    closePolicy: Popup.NoAutoClose
    function show(){
        recPath.visible = !recPath.visible
    }
    onVisibleChanged: if(visible)
                          recordedPathInterface.recPathLoad()
                      else
                          recordedPathInterface.recPathClear()

    ScrollViewExpandableColumn{
        id: recPathColumn
        width: children.width
        height: parent.height-20
        anchors.centerIn: parent
        IconButtonTransparent{
            icon.source: prefix + "/images/Play.png"
            iconChecked: prefix + "/images/boundaryStop.png"
            onClicked: recordedPathInterface.recPathFollowStop()
        }
        IconButtonTransparent{
            icon.source: prefix + "/images/Stop.png"
            onClicked: recordedPathInterface.recPathResumeStyle()
        }
        IconButtonTransparent{
            icon.source: prefix + "/images/BoundaryRecord.png"
            iconChecked: prefix + "/images/boundaryStop.png"
            onClicked: recordedPathInterface.recPathRecordStop()
        }
        IconButtonTransparent{
            icon.source: prefix + "/images/FileExplorerWindows.png"
        }
        IconButtonTransparent{
            icon.source: prefix + "/images/ABSwapPoints.png"
            onClicked: recordedPathInterface.recPathSwapAB()
        }
    }
}
