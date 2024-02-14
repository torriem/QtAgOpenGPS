import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Rectangle{
    id: headAcheDesigner
    width: 1024
    height: 763
    color: "ghostwhite"
    function show(){
        headAcheDesigner.visible = true
    }

/*    property int sliceCount: 0
    property int backupCount: 0
    property int hdlIndex: -1
    property bool curveLine: false
    property double zoom: 1
    property double sX: 0
    property double sY: 0

    signal load()
    signal close() //vs exit?
    signal mouseClicked(int x, int y)
    //signal mouseDragged(int fromX, int fromY, int toX, int toY)
    //signal zoom(bool checked)
    signal slice()
    signal deletePoints()
    signal undo()
    signal ashrink()
    signal alength()
    signal bshrink()
    signal blength()
    signal headlandOff()
    signal isSectionControlled(bool wellIsIt)*/

    Rectangle{//renderer goes here
        id: headAcheRenderer
        objectName: "headAcheRenderer"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        height: parent.height
        width: parent.width * .6
        color: "white"

        MouseArea {
            id: headAcheMouseArea
            anchors.fill: parent

            property int fromX: 0
            property int fromY: 0

            onClicked: {
                if (cboxIsZoom.checked && headAcheDesigner.zoom === 1) {
                    sX = ((parent.width / 2 - mouseX) / parent.width) * 1.1
                    sY = ((parent.width / 2 - mouseY) / -parent.width) * 1.1
                    zoom = 0.1
                } else {
                    headAcheDesigner.mouseClicked(mouseX, mouseY)
                }
            }

            onPressed: {
                //save a copy of the coordinates
                fromX = mouseX
                fromY = mouseY
            }

            onPositionChanged: {
                headAcheDesigner.mouseDragged(fromX, fromY, mouseX, mouseY)
                fromX = mouseX
                fromY = mouseY
            }

            //onWheel: {}
        }
    }

    GridLayout{
        id: buttons
        anchors.left: headAcheRenderer.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 10
        flow: Grid.LeftToRight // NOTE!! this is not how I normally do it
                               //but it seems to make the most sense here
        columns: 2
        rows: 9
        ButtonGroup{
            buttons: [ headAcheAB, headAcheCurve ]
        }

        IconButtonTransparent{
            //objectName: "btnBLength"
            icon.source: "/images/APlusPlusB.png"
        }
        IconButtonTransparent{
            //objectName: "btnBShrink"
            icon.source: "/images/APlusMinusB.png"
        }
        IconButtonTransparent{
            //objectName: "btnALength"
            icon.source: "/images/APlusPlusA.png"
        }
        IconButtonTransparent{
            //objectName: "btnAShrink"
            icon.source: "/images/APlusMinusA.png"
        }
        IconButtonColor{
            id: headAcheCurve
            //objectName: "rbtnLine"
            checkable: true
            isChecked: true
            icon.source: "/images/ABTrackCurve.png"
        }
        IconButtonColor{
            id: headAcheAB
            //objectName: "rbtnCurve"
            checkable: true
            icon.source: "/images/ABTrackAB.png"
        }
        SpinBoxM {
            //objectName: "nudSetDistance"
            from: 0
            to: 2000
            boundValue: numTracks.value * settings.setVehicle_toolWidth
            TextLine{anchors.top: parent.bottom; text: "( "+ utils.m_unit_abbrev()+" )"}
        }
        SpinBoxCustomized{
            id: numTracks
            from: 0
            to: 10
            value: 0
            TextLine{anchors.top: parent.bottom; text: qsTr("Tool: ")+ utils.m_to_ft_string(settings.setVehicle_toolWidth)}
        }
        IconButtonColor{
            id: cboxIsZoom
            //objectName: "cboxIsZoom"
            checkable: true
            icon.source: "/images/ZoomOGL.png"
        }

        IconButtonTransparent{
            //objectName: "btnSlice"
            icon.source: "/images/HeadlandSlice.png"
        }
        IconButtonTransparent{
            //objectName: "btnBndLoop"
            icon.source: "/images/HeadlandBuild.png"
        }
        IconButtonTransparent{
            //objectName: "btnDeletePoints"
            icon.source: "/images/HeadlandReset.png"
        }
        IconButtonTransparent{
            //objectName: "btnUndo"
            icon.source: "/images/back-button.png"
        }
        IconButtonTransparent{
            //objectName: "cBoxIsSectionControlled"
            icon.source: "/images/HeadlandSectionOff.png"
            iconChecked: "/images/HeadlandSectionOn.png"
            checkable: true
        }
        IconButtonTransparent{
            icon.source: "/images/SwitchOff.png"
            onClicked: {
                boundaryInterface.isHeadlandOn = false
                headAcheDesigner.visible = false
            }
        }
        IconButtonTransparent{
            icon.source: "/images/OK64.png"
            onClicked: {
                boundaryInterface.isHeadlandOn = true
                headAcheDesigner.visible = false
            }
        }
    }
}
