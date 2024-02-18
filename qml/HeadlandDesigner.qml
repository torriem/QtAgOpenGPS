import QtQuick 2.15
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Shapes 1.15

Popup{
    id: headlandDesigner
    width: parent.width
    height: parent.height
    closePolicy: Popup.NoAutoClose

    //color: "ghostwhite"
    function show(){
        headlandDesigner.visible = true
    }

    property int sliceCount: 0
    property int backupCount: 0
    property int hdlIndex: -1
    property bool curveLine: true
    property double lineDistance: 0

    property double zoom: 1
    property double sX: 0
    property double sY: 0

    property double maxFieldDistance;
    property double fieldCenterX;
    property double fieldCenterY;
    property point apoint: Qt.point(0,0)
    property point bpoint: Qt.point(0,0)
    property bool showa: false
    property bool showb: false
    property color acolor: "red"
    property color bcolor: "blue"

    signal load()
    signal update_lines();

    signal save_exit()
    signal mouseClicked(int x, int y)
    //signal mouseDragged(int fromX, int fromY, int toX, int toY)
    //signal zoom(bool checked)
    signal slice()
    signal deletePoints()
    signal create_headland()
    signal undo()
    signal ashrink()
    signal alength()
    signal bshrink()
    signal blength()
    signal headlandOff()
    signal isSectionControlled(bool wellIsIt)

    //onClosed: { console.debug("closed.") }
    onWidthChanged: if(aog.isJobStarted) update_lines()
    onHeightChanged: if(aog.isJobStarted) update_lines()

    property var boundaryLines: [
        {
            index: 0,
            color: "#FF0000",
            width: 4,
            points: [
                Qt.point(50, 50),
                Qt.point(100, 50),
                Qt.point(100, 100),
                Qt.point(50, 100),
                Qt.point(50, 50)
            ]
        },
        {
            index: 1,
            color: "#00FF00",
            width: 4,
            points: [
                Qt.point(25, 25),
                Qt.point(75, 25),
                Qt.point(75, 75),
                Qt.point(25, 75),
                Qt.point(25, 25)
            ]
        }
    ]

    property var headlandLine: [
        Qt.point(0,0),
        Qt.point(100,20)
    ]

    property var sliceLine: [
        Qt.point(0,0),
        Qt.point(20,100)
    ]


    onHeadlandLineChanged: {
        if (headlandLine.length > 0)
            headlandShapePath.p = headlandLine
    }

    onSliceLineChanged: {
        sliceShapePath.p = sliceLine
    }

    onVisibleChanged: {
        if(visible) {
            load()
        } else {
            close()
        }
    }

    Rectangle {//renderer goes here
        id: headlandRenderer
        objectName: "headlandRenderer"
        property string myname: "headlandRenderer"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        height: parent.height
        width: parent.width * .7
        color: "black"

        Rectangle {
            id: a_rect
            visible: headlandDesigner.showa
            width: 24
            height: 24
            radius: 12
            color: "#ffc059"
            x: headlandDesigner.apoint.x - 12
            y: headlandDesigner.apoint.y - 12
            z: 1
        }

        Rectangle {
            id: b_rect
            visible: headlandDesigner.showb
            width: 24
            height: 24
            radius: 12
            color:  "#80c0ff"
            x: headlandDesigner.bpoint.x - 12
            y: headlandDesigner.bpoint.y - 12
            z: 1
        }

        Repeater {
            id: boundaryRepeater

            model: boundaryLines.length

            Shape {
                property int outerIndex: index

                anchors.fill: parent
                Connections {
                    target: headlandDesigner
                    function onBoundaryLinesChanged() {
                        shapePath.draw_boundaries()
                    }
                }

                ShapePath {
                    id: shapePath
                    strokeColor: boundaryLines[index].color
                    strokeWidth: boundaryLines[index].width
                    fillColor: "transparent"
                    startX: p[0].x
                    startY: p[0].y
                    scale: Qt.size(1,1)
                    joinStyle: ShapePath.RoundJoin

                    property var p: [Qt.point(0,0), Qt.point(headlandRenderer.width, headlandRenderer.height)]

                    PathPolyline {
                        id: ps
                        path: shapePath.p
                    }


                    Component.onCompleted: draw_boundaries()


                    function draw_boundaries()
                    {
                    //    console.debug(boundaryLines[index].points)
                        p = boundaryLines[index].points
                    }
                }
            }
        }

        Shape {
            id: headlandShape
            visible: headlandLine.length > 0
            anchors.fill: parent
            ShapePath {
                id: headlandShapePath
                strokeColor: "#f1e817"
                strokeWidth: 8
                fillColor: "transparent"
                startX: p[0].x
                startY: p[0].y
                joinStyle: ShapePath.RoundJoin

                property var p: [
                    Qt.point(0,0),
                    Qt.point(20,100),
                    Qt.point(200,150)
                ]

                PathPolyline {
                    id: headlandShapePolyLine
                    path: headlandShapePath.p
                }
            }
        }

        Shape {
            id: sliceShape
            visible: sliceCount != 0
            anchors.fill: parent
            ShapePath {
                id: sliceShapePath
                strokeColor: btnABCurve.checked ? "#f31700" : "#21f305"
                strokeWidth: 8
                fillColor: "transparent"
                startX: p[0].x
                startY: p[0].y
                joinStyle: ShapePath.RoundJoin

                property var p: [
                    Qt.point(0,0),
                    Qt.point(100,20),
                ]

                PathPolyline {
                    id: sliceShapePolyLine
                    path: sliceShapePath.p
                }
            }
        }

        MouseArea {
            id: headlandMouseArea
            anchors.fill: parent

            property int fromX: 0
            property int fromY: 0

            onClicked: {
                if (cboxIsZoom.checked && headlandDesigner.zoom === 1) {
                    sX = ((parent.width / 2 - mouseX) / parent.width) * 1.1
                    sY = ((parent.height / 2 - mouseY) / -parent.height) * 1.1
                    //console.debug("width,mouse, sx,sy",parent.width / 2, mouseX, mouseY, sX,sY);
                    zoom = 0.1
                    headlandDesigner.update_lines()
                } else {
                    headlandDesigner.mouseClicked(mouseX, mouseY)
                    if (zoom != 1.0) {
                        zoom = 1.0;
                        sX = 0;
                        sY = 0;
                        headlandDesigner.update_lines()
                    }
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
            onClicked: create_headland()
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
            Layout.alignment: Qt.AlignCenter
            onClicked: {
                headlandDesigner.headlandOff()
                headlandDesigner.visible = false
            }
            Layout.alignment: Qt.AlignCenter
        }
        IconButtonTransparent{
            icon.source: "/images/OK64.png"
            Layout.alignment: Qt.AlignCenter
            onClicked: {
                save_exit()
                boundaryInterface.isHeadlandOn = true
                headlandDesigner.visible = false
            }
        }
    }
}
