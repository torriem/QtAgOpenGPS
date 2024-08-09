import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import "components" as Comp

Window {
    id: ethernetConfig
    height: 190
    width: 260
    visible: true
    title: qsTr("Ethernet Configuration")

    Rectangle {
        id: ethIP
        visible: true
        color: theme.backgroundColor
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        border.width: 2
        border.color: theme.blackDayWhiteNight
        ColumnLayout {
            id: column
            anchors.fill: parent
            Comp.TextLine {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("Ethernet IP")
            }
            TextField {
                id: ethIPText
                Layout.alignment: Qt.AlignCenter
                placeholderText: qsTr("IP Address")
                width: parent.width
                selectByMouse: true
            }

            Comp.Spacer{}

            RowLayout {
                width: column.width
                Layout.alignment: Qt.AlignCenter
                Comp.IconButtonText {
                    id: ethIPSet
                    text: qsTr("IP Set")
                    icon.source: "/images/SubnetSend.png"
                    Layout.alignment: Qt.AlignCenter
                    onClicked: agio.btnSendSubnet_clicked()
                    implicitWidth: btnOK.width
                    implicitHeight: btnOK.height
                }
                Comp.IconButtonTransparent {
                    id: btnOK
                    Layout.alignment: Qt.AlignCenter
                    icon.source: "/images/OK64.png"
                    onClicked: ethernetConfig.visible = false
                }
            }
        }
    }
}
