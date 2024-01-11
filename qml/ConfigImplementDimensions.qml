import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle{
    id: configImpDimWin
    anchors.fill: parent
    color: "ghostwhite"
    visible: false
    Image{
        source: "/images/ToolHitchPageTrailing.png"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 15
        height: parent.height*.75
        SpinBoxCustomized{
            anchors.bottom: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 500
            from: 4
            value: 1
            to:1181
        }
        SpinBoxCustomized{
            anchors.bottom: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 50
            from: 4
            value: 1
            to:1181
        }
    }
}
