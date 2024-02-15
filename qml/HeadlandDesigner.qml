import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import AgOpenGPS 1.0

Rectangle{
    id: headlandDesigner
    width: 1024
    height: 763
    color: "ghostwhite"
    function show(){
        headlandDesigner.visible = true
    }

    property int sliceCount: 0
    property int backupCount: 0
    property int hdlIndex: -1
    property bool curveLine: false
    property double lineDistance: 0

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
    signal isSectionControlled(bool wellIsIt)

    onVisibleChanged: {
        if(visible) {
            load()
        } else {
            close()
        }
    }

    AOGRenderer {//renderer goes here
        id: headlandRenderer
        objectName: "headlandRenderer"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        height: parent.height
        width: parent.width * .7
        //color: "white"

        MouseArea {
            id: headlandMouseArea
            anchors.fill: parent

            property int fromX: 0
            property int fromY: 0

            onClicked: {
                if (cboxIsZoom.checked && headlandDesigner.zoom === 1) {
                    sX = ((parent.width / 2 - mouseX) / parent.width) * 1.1
                    sY = ((parent.width / 2 - mouseY) / -parent.width) * 1.1
                    zoom = 0.1
                } else {
                    headlandDesigner.mouseClicked(mouseX, mouseY)
                    zoom = 1.0;
                    sX = 0;
                    xY = 0;
                }
            }

            onPressed: {
                //save a copy of the coordinates
                fromX = mouseX
                fromY = mouseY
            }

            onPositionChanged: {
                headlandDesigner.mouseDragged(fromX, fromY, mouseX, mouseY)
                fromX = mouseX
                fromY = mouseY
            }

            //onWheel: {}
        }
    }

    GridLayout{
        id: buttons
        anchors.left: headlandRenderer.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 10
        flow: Grid.LeftToRight // NOTE!! this is not how I normally do it
                               //but it seems to make the most sense here
        columns: 2
        rows: 9
        ButtonGroup{
            buttons: [ headlandAB, headlandCurve ]
        }

        IconButtonTransparent{
            objectName: "btnBLength"
            icon.source: "/images/APlusPlusB.png"
            Layout.alignment: Qt.AlignCenter
            onClicked: {
                headlandDesigner.blength()
            }
        }
        IconButtonTransparent{
            objectName: "btnBShrink"
            icon.source: "/images/APlusMinusB.png"
            Layout.alignment: Qt.AlignCenter
            onClicked: {
                headlandDesigner.bshrink()
            }
        }
        IconButtonTransparent{
            objectName: "btnALength"
            icon.source: "/images/APlusPlusA.png"
            Layout.alignment: Qt.AlignCenter
            onClicked: {
                headlandDesigner.alength()
            }
        }
        IconButtonTransparent{
            objectName: "btnAShrink"
            icon.source: "/images/APlusMinusA.png"
            Layout.alignment: Qt.AlignCenter
            onClicked: {
                headlandDesigner.ashrink()
            }
        }
        IconButtonColor{
            id: headlandCurve
            objectName: "rbtnLine"
            checkable: true
            isChecked: true
            icon.source: "/images/ABTrackCurve.png"
            Layout.alignment: Qt.AlignCenter
            onClicked: curveLine = true
        }
        IconButtonColor{
            id: headlandAB
            objectName: "rbtnCurve"
            checkable: true
            icon.source: "/images/ABTrackAB.png"
            Layout.alignment: Qt.AlignCenter
            onClicked: curveLine = false
        }
        SpinBoxM {
            objectName: "nudSetDistance"
            from: 0
            to: 2000
            boundValue: numTracks.value * settings.setVehicle_toolWidth
            Layout.alignment: Qt.AlignCenter
            TextLine {
                anchors.top: parent.bottom;
                text: "( "+ utils.m_unit_abbrev()+" )"
            }
            onValueChanged: {
                lineDistance = value
            }
        }
        SpinBoxCustomized{
            id: numTracks
            from: 0
            to: 10
            value: 0
            Layout.alignment: Qt.AlignCenter
            TextLine {
                anchors.top: parent.bottom;
                text: qsTr("Tool: ")+ utils.m_to_ft_string(settings.setVehicle_toolWidth)
            }
        }
        IconButtonColor{
            id: cboxIsZoom
            objectName: "cboxIsZoom"
            checkable: true
            icon.source: "/images/ZoomOGL.png"
            Layout.alignment: Qt.AlignCenter
        }

        IconButtonTransparent{
            objectName: "btnSlice"
            icon.source: "/images/HeadlandSlice.png"
            enabled: (sliceCount > 0)
            Layout.alignment: Qt.AlignCenter
            onClicked: slice()
        }
        IconButtonTransparent{
            objectName: "btnBndLoop"
            icon.source: "/images/HeadlandBuild.png"
            Layout.alignment: Qt.AlignCenter
        }
        IconButtonTransparent{
            objectName: "btnDeletePoints"
            icon.source: "/images/HeadlandReset.png"
            Layout.alignment: Qt.AlignCenter
            onClicked: deletePoints()
        }
        IconButtonTransparent{
            objectName: "btnUndo"
            icon.source: "/images/back-button.png"
            enabled: (backupCount > 0)
            Layout.alignment: Qt.AlignCenter
            onClicked: undo()
        }
        IconButtonTransparent{
            objectName: "cBoxIsSectionControlled"
            icon.source: "/images/HeadlandSectionOff.png"
            iconChecked: "/images/HeadlandSectionOn.png"
            checkable: true
            isChecked: settings.setHeadland_isSectionControlled
            Layout.alignment: Qt.AlignCenter
            onCheckedChanged: isSectionControlled(checked)
        }
        IconButtonTransparent{
            icon.source: "/images/SwitchOff.png"
            onClicked: headlandDesigner.headlandOff()
            Layout.alignment: Qt.AlignCenter
        }
        IconButtonTransparent{
            icon.source: "/images/OK64.png"
            Layout.alignment: Qt.AlignCenter
            onClicked: {
                boundaryInterface.isHeadlandOn = true
                headlandDesigner.visible = false
            }
        }
    }
}
