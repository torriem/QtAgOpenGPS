import QtQuick 2.0
import QtQuick.Controls 2.1

import ".."
import "components"

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
        ScrollViewExpandableColumn {
            id: toolsGrid
            anchors.fill: parent
            IconButtonTextBeside {
                id: wizards
                width: 250
                height: 50
                text: qsTr("Wizards")
                onClicked: wizardMenu.visible = !wizardMenu.visible
                visible: false //todo later
            }

            IconButtonTextBeside {
                id: charts
                width: 250
                height: 50
                text: qsTr("Charts")
                onClicked: chartMenu.visible = !chartMenu.visible
                visible: false //todo later
            }

            IconButtonTextBeside {
                id: smABCurve
                width: 250
                height: 50
                text: qsTr("Smooth AB Curve")
                visible: settings.setFeature_isABSmoothOn
            }

            IconButtonTextBeside {
                id: delContourPaths
                width: 250
                height: 50
                text: qsTr("Delete Contour Paths")
                visible:settings.setFeature_isHideContourOn
            }

            IconButtonTextBeside {
                id: delAppliedArea
                width: 250
                height: 50
                text: qsTr("Delete Applied Area")
            }

            IconButtonTextBeside {
                id: webcam
                width: 250
                height: 50
                text: qsTr("WebCam")
                visible:settings.setFeature_isWebCamOn
            }
            IconButtonTextBeside {
                id: offsetFix
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
