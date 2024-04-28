import QtQuick
import QtQuick.Controls.Fusion

import ".."
import "../components"

/*todo:
  not sure about this whole file. The IconButtonColor isn't connected to anything
  */
Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent
        color: aog.backgroundColor
        Image {
            source: "/images/Config/ConT_TramSpacing.png"
            id: imageTram
            anchors.right: parent.horizontalCenter
            anchors.top: parent.top
			anchors.topMargin: 10 * theme.scaleHeight
			anchors.bottomMargin: 10 * theme.scaleHeight
			anchors.leftMargin: 10 * theme.scaleWidth
			anchors.rightMargin: 10 * theme.scaleWidth
            width: 200 * theme.scaleWidth
            height: 200 * theme.scaleHeight
        }
        SpinBoxCM{
            id: tramWidth
            anchors.verticalCenter: imageTram.verticalCenter
            anchors.left: parent.horizontalCenter
			anchors.topMargin: 3 * theme.scaleHeight
			anchors.bottomMargin: 3 * theme.scaleHeight
			anchors.leftMargin: 3 * theme.scaleWidth
			anchors.rightMargin: 3 * theme.scaleWidth
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
            anchors.bottomMargin: 100 * theme.scaleHeight
            width: 200 * theme.scaleWidth
            height: 200 * theme.scaleHeight
            icon.source: "/images/Config/ConT_TramOverride.png"
        }
    }
}
