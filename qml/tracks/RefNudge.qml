import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

import ".."
import "../components"

MoveablePopup {
    id: refNudge
    x: 40
    y: 40
    height: 400
    width: 230
    function show(){
        refNudge.visible = true
    }

    TitleFrame{
        id: rootRect
        color: "#ff6666"
        anchors.fill: parent
        title: qsTr("Ref Adjust")
        Rectangle{
            anchors.top: parent.top
            anchors.bottom: nudgeText.top
            anchors.bottomMargin: 15
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - 16
            ColumnLayout{
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
            }
        }
        TextLine{
            id: nudgeText
            font.pixelSize: 25
            text: qsTr("0 ")+ utils.cm_unit_abbrev()
            anchors.bottom: bottomButtons.top
            anchors.bottomMargin: 15
        }
        RowLayout{
            id: bottomButtons
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 15
            IconButtonTransparent{
                icon.source: "/images/Cancel64.png"
                onClicked: refNudge.visible = false
                Layout.alignment: Qt.AlignCenter
            }
            IconButtonTransparent{
                icon.source: "/images/OK64.png"
                onClicked: refNudge.visible = false
                Layout.alignment: Qt.AlignCenter
            }
        }
    }
}
