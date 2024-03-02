import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
//import AgOpenGPS 1.0

Item {
    id: lineDrawer
    function show(){
        lineDrawer.visible = true
    }

    TopLine{
        id: topLine
        titleText: "Click 2 points on the Boundary to Begin"
    }
    Rectangle{
        anchors.top: topLine.bottom
        anchors.left: parent.left
        anchors.right: buttons.left
        anchors.bottom:  parent.bottom
        color: "black"

        Rectangle { //temporary for qmlscene testing
        //AOGRenderer{
            id: lineDrawerField
            objectName: "lineDrawerField"
            anchors.fill: parent
            TextLine{ text: "nothing to see here yet"}

            signal clicked(var mouse)

            MouseArea {
                id: mainMouseArea
                anchors.fill: parent

                onClicked: {
                    parent.clicked(mouse);
                }
            }
        }
    }
    Rectangle{
        id: buttons
        color: "lightgray"
        width: 250
        anchors.bottom: parent.bottom
        anchors.top: topLine.bottom
        anchors.right: parent.right

        //  I'll add this later, not sure how it works now.
        GridLayout{
            id: top6Buttons
            anchors.top: parent.top
            anchors.bottom: boundaryCurve.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.margins: 5
            columns: 2
            rows: 6
            flow: Grid.TopToBottom
            IconButtonTransparent{
                icon.source: "/images/APlusPlusB.png"
                Layout.alignment: Qt.AlignCenter
            }
            IconButtonTransparent{
                icon.source: "/images/APlusPlusA.png"
                Layout.alignment: Qt.AlignCenter
            }
            IconButtonTransparent{
                icon.source: "/images/TrackVisible.png"
                Layout.alignment: Qt.AlignCenter
            }
            IconButtonTransparent{
                icon.source: "/images/ZoomOGL.png"
                Layout.alignment: Qt.AlignCenter
            }
            IconButtonTransparent{
                icon.source: "/images/ABTrackCurve.png"
                Layout.alignment: Qt.AlignCenter
            }
            IconButtonTransparent{
                icon.source: "/images/ABLineCycleBk.png"
                Layout.alignment: Qt.AlignCenter
            }
            IconButtonTransparent{
                icon.source: "/images/MappingOff.png"
                Layout.alignment: Qt.AlignCenter
            }
            IconButtonTransparent{
                icon.source: "/images/HeadlandDeletePoints.png"
                Layout.alignment: Qt.AlignCenter
            }

        IconButtonTransparent{
            id: boundaryCurve
            icon.source: "/images/BoundaryCurveLine.png"
            text: "Boundary Curve"
        }

            IconButtonTransparent{
                icon.source: "/images/Trash.png"
            }
            IconButtonTransparent{
                icon.source: "/images/ABTrackAB.png"
            }

            IconButtonTransparent{
                icon.source: "/images/ABLineCycle.png"
            }
        }
        Rectangle{
            id: curveNameInput
            width: parent.width - 10
            anchors.margins: 15
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: curveRow.top
            color: "ghostwhite"
            border.color: "darkgray"
            border.width: 1
            height: 40
            TextInput{
                objectName: "drawerCurveName"
                anchors.fill: parent
            }
        }
        RowLayout{
            id: curveRow
            anchors.bottom:  bottomRow.top
            anchors.margins: 15
            width: parent.width - 10
            anchors.horizontalCenter: parent.horizontalCenter
            height: children.height
            Text{
                text: "1/16"
                Layout.alignment: Qt.AlignCenter
            }
            IconButtonTransparent{
                icon.source: "/images/Time.png"
                Layout.alignment: Qt.AlignCenter
            }
        }

        RowLayout{
            id: bottomRow
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width -10
            height: children.height + anchors.margins
            anchors.margins: 15
            IconButtonTransparent{
                icon.source: "/images/Cancel64.png"
                Layout.alignment: Qt.AlignCenter
                onClicked: lineDrawer.visible = false
            }

            IconButtonTransparent{
                objectName: "btnDrawerSave"
                icon.source: "/images/OK64.png"
                onClicked: lineDrawer.visible = false
                Layout.alignment: Qt.AlignCenter
            }
        }
    }
}
