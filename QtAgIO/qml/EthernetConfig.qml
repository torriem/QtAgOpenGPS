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
        function load_settings(){
                spIP1.value = settings.setUDP_IP1
                spIP2.value = settings.setUDP_IP2
                spIP3.value = settings.setUDP_IP3
        }

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
            Text {
                Layout.alignment: Qt.AlignCenter
                text: qsTr("IP Address")
            }

            Row{
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 4
                Comp.SpinBoxCustomized{
                    id: spIP1
                    width: 30 * theme.scaleWidth
                   from: 0
                   to: 255
                }
                Comp.SpinBoxCustomized{
                    id: spIP2
                    width: 30 * theme.scaleWidth
                   from: 0
                   to: 255
                }
                Comp.SpinBoxCustomized{
                    id: spIP3
                    width: 30 * theme.scaleWidth
                   from: 0
                   to: 255
                }

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
                    onClicked: {
                        settings.setUDP_IP1 = spIP1.value
                        settings.setUDP_IP2 = spIP2.value
                        settings.setUDP_IP3 = spIP3.value
                        timedMessage.addMessage(2000, "IP Address Change", ("IP address changed to " +
                        settings.setUDP_IP1 + "." + settings.setUDP_IP2 + "." + settings.setUDP_IP3 + "!"))
                        agio.btnSendSubnet_clicked()
                    }
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
