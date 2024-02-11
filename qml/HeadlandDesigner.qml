import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Rectangle{
    id: headlandDesigner
    width: 1024
    height: 763
    color: "ghostwhite"
    function show(){
        headlandDesigner.visible = true
    }

    Rectangle{//renderer goes here
        id: headlandRenderer
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        height: parent.height
        width: parent.width * .6
        color: "white"
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
            icon.source: "/images/APlusPlusB.png"
        }
        IconButtonTransparent{
            icon.source: "/images/APlusMinusB.png"
        }
        IconButtonTransparent{
            icon.source: "/images/APlusPlusA.png"
        }
        IconButtonTransparent{
            icon.source: "/images/APlusPlusB.png"
        }
        IconButtonColor{
            id: headlandCurve
            checked: true
            icon.source: "/images/ABTrackCurve.png"
        }
        IconButtonColor{
            id: headlandAB
            icon.source: "/images/ABTrackAB.png"
        }
        SpinBoxM{
            from: 0
            to: 200
            value: 0
            TextLine{anchors.top: parent.bottom; text: "( m )"}
        }
        SpinBoxCustomized{
           from: 0
           to: 10
           value: 0
           TextLine{anchors.top: parent.bottom; text: qsTr("Tool: ")+ settings.setVehicle_toolWidth + " m"}
        }
        IconButtonColor{
            icon.source: "/images/ZoomOGL.png"
        }

        IconButtonTransparent{
            icon.source: "/images/HeadlandSlice.png"
        }
        IconButtonTransparent{
            icon.source: "/images/HeadlandBuild.png"
        }
        IconButtonTransparent{
            icon.source: "/images/HeadlandReset.png"
        }
        IconButtonTransparent{
            icon.source: "/images/back-button.png"
        }
        IconButtonTransparent{
            icon.source: "/images/HeadlandSectionOff.png"
        }
        IconButtonTransparent{
            icon.source: "/images/SwitchOff.png"
            onClicked: headlandDesigner.visible = false
        }
        IconButtonTransparent{
            icon.source: "/images/OK64.png"
            onClicked: headlandDesigner.visible = false
        }
    }
}
