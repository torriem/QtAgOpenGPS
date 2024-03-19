import QtQuick 2.0
import QtQuick.Controls 2.5

import ".."
import "../components"

/*todo:
  not sure about this whole file. The IconButtonColor isn't connected to anything
  */
Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent
        color: "ghostwhite"
        Image {
            source: "/images/Config/ConT_TramSpacing.png"
            id: imageTram
            anchors.right: parent.horizontalCenter
            anchors.top: parent.top
            anchors.margins: 10
            width: 200
            height: 200
        }
        SpinBoxCM{
            id: tramWidth
            anchors.verticalCenter: imageTram.verticalCenter
            anchors.left: parent.horizontalCenter
            anchors.margins: 3
            editable: true
            text: qsTr("Tram Width")
            from: 1
            value: settings.setTram_tramWidth
            Connections {
                target: settings
                function onSetTram_tramWidthChanged(){
                    tramWidth.value = settings.setTram_tramWidth
                }
            }
            to: 5000
            onValueChanged: settings.setTram_tramWidth = value
        }
        IconButtonColor{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 100
            width: 200
            height: 200
            icon.source: "/images/Config/ConT_TramOverride.png"
        }
    }
}
