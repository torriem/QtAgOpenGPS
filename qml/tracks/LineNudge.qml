import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

import ".."
import "../components"

MoveablePopup{
    id: lineNudge
    x: 40
    y: 40
    height: 400
    width: 200
    function show(){
        lineNudge.visible = true
    }

    Rectangle{
        id: rootRect
        color: "#b3ccff"
        anchors.fill: parent
        Text{
            id: nudgeDist
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.leftMargin: 8
            text: "< 3"
        }
        IconButtonTransparent{
            id: closeBtn
            icon.source: "/images/WindowClose.png"
            implicitHeight: 40
            implicitWidth: 40
            anchors.top: parent.top
            anchors.right: parent.right
            onClicked: lineNudge.visible = false
        }
        Rectangle{
            id: whiteRect
            anchors.top: closeBtn.bottom
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
            color: "white"
            ColumnLayout{
                id: column
                anchors.fill: parent
                RowLayout{
                    Layout.alignment: Qt.AlignCenter
                    implicitWidth: parent.width
                    IconButtonTransparent{
                        icon.source: "/images/SnapLeftHalf.png"
                        Layout.alignment: Qt.AlignLeft
                    }
                    IconButtonTransparent{
                        icon.source: "/images/SnapRightHalf.png"
                        Layout.alignment: Qt.AlignRight
                    }
                }
                RowLayout{
                    Layout.alignment: Qt.AlignCenter
                    implicitWidth: parent.width
                    IconButtonTransparent{
                        icon.source: "/images/SnapLeft.png"
                        Layout.alignment: Qt.AlignLeft
                    }
                    IconButtonTransparent{
                        icon.source: "/images/SnapRight.png"
                        Layout.alignment: Qt.AlignRight
                    }
                }
                TextField{
                    Layout.alignment: Qt.AlignCenter
                    implicitWidth: 75
                }

                RowLayout{
                    Layout.alignment: Qt.AlignCenter
                    implicitWidth: parent.width
                    IconButtonTransparent{
                        icon.source: "/images/SnapToPivot.png"
                        Layout.alignment: Qt.AlignLeft
                    }
                    IconButtonTransparent{
                        icon.source: "/images/SteerZero.png"
                        Layout.alignment: Qt.AlignRight
                    }
                }
            }
        }
    }
}
