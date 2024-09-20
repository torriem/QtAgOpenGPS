import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import "components" as Comp
import "interfaces"

Window{
    id: advancedMenu
    height: 300
    width: 300
    visible: false
    title: qsTr("Advanced")

    Rectangle{
        id: content
        visible: true
        anchors.fill: parent
        border.width: 2
        border.color: theme.blackDayWhiteNight
        color: theme.backgroundColor
        Grid{
            id: contGrid
            anchors.fill: parent
            anchors.margins: 10
            flow: Grid.TopToBottom
            columns: 2
            CheckBox{
                id: ckUDPListenOnly
                text: "UDP Listen Only"
                checked: false
                onCheckedChanged: agio.btnUDPListenOnly_clicked(ckUDPListenOnly.checkState)
            }
            CheckBox {
                id: ckNtripDebug
                text: "Console NTRIP Debug"
                checked: false
                onCheckedChanged: agio.ntripDebug(ckNtripDebug.checkState)
            }
        }
    }
}
