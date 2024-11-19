// Copyright (C) 2024 Michael Torrie and the QtAgOpenGPS Dev Team
// SPDX-License-Identifier: GNU General Public License v3.0 or later
//
// The "Tools" button on main screen
import QtQuick 2.0
import QtQuick.Controls.Fusion

import ".."
import "components" as Comp

//Item{
//    id: toolsWindowItem
//    visible: false
//    height: mainWindow.height
    Drawer {
        id: toolsMenu
        width: 250
        height: mainWindow.height
        modal: true
//        onVisibleChanged: if (visible === false){
//                             toolsWindowItem.visible = false
//                          }

        contentItem: Rectangle{
            id: toolsMenuContent
            anchors.fill: parent
            height: toolsMenu.height
            color: aog.blackDayWhiteNight
        }

        Comp.ScrollViewExpandableColumn {
            id: toolsGrid
            anchors.fill: parent

            Comp.IconButtonTextBeside {
                id: wizards
                icon.source: prefix + "/images/WizardWand.png"
                width: 250
                height: 50
                text: qsTr("Wizards")
                onClicked: wizardMenu.visible = !wizardMenu.visible
                visible: false //todo later
            }

            Comp.IconButtonTextBeside {
                id: charts
                icon.source: prefix + "/images/Chart.png"
                width: 250
                height: 50
                text: qsTr("Charts")
                onClicked: chartMenu.visible = !chartMenu.visible
                visible: false //todo later
            }

            Comp.IconButtonTextBeside {
                id: smABCurve
                icon.source: prefix + "/images/ABSmooth.png"
                width: 250
                height: 50
                text: qsTr("Smooth AB Curve")
                visible: settings.setFeature_isABSmoothOn
            }

            Comp.IconButtonTextBeside {
                id: delContourPaths
                icon.source: prefix + "/images/TrashContourRef.png"
                width: 250
                height: 50
                text: qsTr("Delete Contour Paths")
                visible:settings.setFeature_isHideContourOn
            }

            Comp.IconButtonTextBeside {
                id: delAppliedArea
                icon.source: prefix + "/images/TrashApplied.png"
                width: 250
                height: 50
                text: qsTr("Delete Applied Area")
                onClicked: aog.deleteAppliedArea()
            }

            Comp.IconButtonTextBeside {
                id: webcam
                icon.source: prefix + "/images/Webcam.png"
                width: 250
                height: 50
                text: qsTr("WebCam")
                visible:settings.setFeature_isWebCamOn
            }

            Comp.IconButtonTextBeside {
                id: offsetFix
                icon.source: prefix + "/images/YouTurnReverse.png" // this is horrible. This has nothing to do with YouTurnReverse.
                width: 250
                height: 50
                text: qsTr("Offset Fix")
                visible: settings.setFeature_isOffsetFixOn
            }
        }
    }

//    Rectangle{ //this all needs to be done sometime
//        id: wizardMenu
//        width: childrenRect.width+10
//        height: childrenRect.height+10
//        visible: false
//        color: "black"
//        border.color: "lime"
//        anchors.left: toolsMenu.right



//        Grid {
//            id: grid2
//            height: childrenRect.height
//            width: childrenRect.width
//            anchors.left: parent.left
//            anchors.leftMargin: 5
//            anchors.top: parent.top
//            anchors.topMargin: 5
//            spacing: 10
//            flow: Grid.TopToBottom
//            rows: 1
//            columns: 1

//            IconButtonTextBeside{
//                id: steerWiz
//                width: 250
//                height: 50
//                text: qsTr("Steer Wizard")
//            }
//        }
//    }
//    Rectangle{
//        id: chartMenu
//        width: childrenRect.width+10
//        height: childrenRect.height+10
//        visible: false
//        color: "black"
//        border.color: "lime"
//        anchors.left: toolsMenu.right



//        Grid {
//            id: grid3
//            height: childrenRect.height
//            width: childrenRect.width
//            anchors.left: parent.left
//            anchors.leftMargin: 5
//            anchors.top: parent.top
//            anchors.topMargin: 5
//            spacing: 10
//            flow: Grid.TopToBottom
//            rows: 4
//            columns: 1

//            IconButtonTextBeside{
//                id: steerChart
//                width: 250
//                height: 50
//                text: qsTr("Steer Chart")
//            }
//            IconButtonTextBeside{
//                id: headingChart
//                width: 250
//                height: 50
//                text: qsTr("Heading Chart")
//            }
//            IconButtonTextBeside{
//                id: xteChart
//                width: 250
//                height: 50
//                text: qsTr("XTE Chart")
//            }
//            IconButtonTextBeside{
//                id: rollChart
//                width: 250
//                height: 50
//                text: qsTr("Roll Chart")
//            }
//        }
//    }
//}
