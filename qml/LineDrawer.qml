import QtQuick 2.0
import AgOpenGPS 1.0

Item {
    id: lineDrawer
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

        AOGRenderer{
            id: lineDrawerField
            objectName: "lineDrawerField"
            anchors.fill: parent

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
        /*
          I'll add this later, not sure how it works now.
        Grid{
            id: top4Buttons
            anchors.top: parent.top
            anchors.bottom: boundaryCurve.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.margins: 5
            spacing: 5
            rows: 2
            columns: 2
            flow: Grid.TopToBottom
            IconButtonTransparent{
                objectName: "btnDrawer"
                icon.source: "qrc:/images/.png"
            }
            IconButtonTransparent{
                objectName: "btnDrawer"
                icon.source: "qrc:/images/.png"
            }
            IconButtonTransparent{
                objectName: "btnDrawer"
                icon.source: "qrc:/images/.png"
            }
            IconButtonTransparent{
                objectName: "btnDrawer"
                icon.source: "qrc:/images/.png"
            }
        }*/

        IconButtonTransparent{
            id: boundaryCurve
            objectName: "btnDrawerGenerateBoundaryCurve"
            icon.source: "qrc:/images/BoundaryCurveLine.png"
            anchors.right: parent.right
            anchors.bottom: curveRow.top
            anchors.margins: 5
            Text{
                anchors.right: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 10
                text: "Boundary Curve"
                font.bold: true
                font.pixelSize: 15
            }
        }

        Row{
            id: curveRow
            anchors.bottom: curveNameInput.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width -10
            anchors.margins: 5
            spacing: 20
            IconButtonTransparent{
                id: marker
                objectName: "btnDrawerCreateCurve"
                icon.source: "qrc:/images/CurveOn.png"
            }
            IconButtonTransparent{
                objectName: "btnDrawerCurveCycle"
                icon.source: "qrc:/images/ABLineCycle.png"
            }

            IconButtonTransparent{
                objectName: "btnDrawerDeleteLine"
                icon.source: "qrc:/images/HideContour.png"
            }
        }
        Rectangle{
            id: curveNameInput
            width: parent.width - 10
            anchors.margins: 5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: lineRow.top
            color: "ghostwhite"
            border.color: "darkgray"
            border.width: 1
            height: 40
            TextInput{
                objectName: "drawerCurveName"
                anchors.fill: parent
            }
        }
        Row{
            id: lineRow
            anchors.bottom: lineNameInput.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width -10
            anchors.margins: 5
            spacing: 20
            IconButtonTransparent{
                objectName: "btnDrawerCreateLine"
                icon.source: "qrc:/images/ABLineOn.png"
            }
            IconButtonTransparent{
                objectName: "btnDrawerLineCycle"
                icon.source: "qrc:/images/ABLineCycle.png"
            }

            IconButtonTransparent{
                objectName: "btnDrawerDeleteLine"
                icon.source: "qrc:/images/ABLineDelete.png"
            }
        }
        Rectangle{
            id: lineNameInput
            width: parent.width - 10
            anchors.margins: 5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: bottomRow.top
            color: "ghostwhite"
            border.color: "darkgray"
            height: 40
            border.width: 1
            TextInput{
                objectName: "drawerLineName"
                anchors.fill: parent
            }
        }

        Row{
            id: bottomRow
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width -10
            anchors.margins: 5
            spacing: 20
            IconButtonTransparent{
                objectName: "btnDrawerDeletePoints"
                icon.source: "qrc:/images/HeadlandDeletePoints.png"
            }
            IconButtonTransparent{
                objectName: "btnDrawerShowMapping"
                text: "Mapping"
                isChecked: false
                icon.source: "qrc:/images/MappingOff.png"
                iconChecked: "qrc:/images/MappingOn.png"
                width: marker.width
                height: marker.height
            }

            IconButtonTransparent{
                objectName: "btnDrawerSave"
                icon.source: "qrc:/images/OK64.png"
                onClicked: lineDrawer.visible = false
            }
        }
    }
}
