import QtQuick 2.0
import QtQuick.Controls 2.1

Item {

    Rectangle {

        id: toolsMenu
        width: childrenRect.width+10
        height: childrenRect.height+10
        color: "black"
        visible: true
        border.color: "lime"

        Grid {
            id: toolsGrid
            width: childrenRect.width
            height: childrenRect.height
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5
            spacing: 10
            flow: Grid.TopToBottom
            rows: 7
            columns: 1


            Button {
                id: wizards
                width: 250
                height: 50
                text: qsTr("Wizards")
                property bool hideSteerWiz: true

                onHideSteerWizChanged: {
                    if (hideSteerWiz == true) {
                        wizardMenu.visible = false
                    } else {
                        wizardMenu.visible = true
                    }
                }

                function toggle_steerwiz() {
                    if (hideSteerWiz == true) {
                        hideSteerWiz = false
                    } else {
                        hideSteerWiz = true
                    }
                }

                onClicked: {
                    toggle_steerwiz();
                }
            }

            Button {
                id: charts
                width: 250
                height: 50
                text: qsTr("Charts")
                property bool hideCharts: true

                onHideChartsChanged: {
                    if (hideCharts == true) {
                        chartMenu.visible = false
                    } else {
                        chartMenu.visible = true
                    }
                }

                function toggle_chart() {
                    if (hideCharts == true) {
                        hideCharts = false
                    } else {
                        hideCharts = true
                    }
                }

                onClicked: {
                    toggle_chart();
                }

            }

            Button {
                id: smABCurve
                width: 250
                height: 50
                text: qsTr("Smooth AB Curve")
            }

            Button {
                id: delContourPaths
                width: 250
                height: 50
                text: qsTr("Delete Contour Paths")
            }

            Button {
                id: delAppliedArea
                width: 250
                height: 50
                text: qsTr("Delete Applied Area")
            }

            Button {
                id: webcam
                width: 250
                height: 50
                text: qsTr("WebCam")
            }
            Button {
                id: offsetFix
                width: 250
                height: 50
                text: qsTr("Offset Fix")
            }
        }
    }
    Rectangle{
        id: wizardMenu
        width: childrenRect.width+10
        height: childrenRect.height+10
        visible: false
        color: "black"
        border.color: "lime"
        anchors.left: toolsMenu.right



        Grid {
            id: grid2
            height: childrenRect.height
            width: childrenRect.width
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5
            spacing: 10
            flow: Grid.TopToBottom
            rows: 1
            columns: 1

            Button{
                id: steerWiz
                width: 250
                height: 50
                text: qsTr("Steer Wizard")
            }
        }
    }
    Rectangle{
        id: chartMenu
        width: childrenRect.width+10
        height: childrenRect.height+10
        visible: false
        color: "black"
        border.color: "lime"
        anchors.left: toolsMenu.right



        Grid {
            id: grid3
            height: childrenRect.height
            width: childrenRect.width
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5
            spacing: 10
            flow: Grid.TopToBottom
            rows: 4
            columns: 1

            Button{
                id: steerChart
                width: 250
                height: 50
                text: qsTr("Steer Chart")
            }
            Button{
                id: headingChart
                width: 250
                height: 50
                text: qsTr("Heading Chart")
            }
            Button{
                id: xteChart
                width: 250
                height: 50
                text: qsTr("XTE Chart")
            }
            Button{
                id: rollChart
                width: 250
                height: 50
                text: qsTr("Roll Chart")
            }

        }
    }
}
